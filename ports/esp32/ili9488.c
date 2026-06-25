/**
 * @file ili9488_new.c
 * @brief Minimal ILI9488 driver — ESP32-S3, ESP-IDF 5.5
 *        Pure GPIO bit-bang. No esp_lcd. No LVGL. No DMA.
 *
 * ── WRITE PATH ───────────────────────────────────────────────────
 * Byte writes use two precomputed 256-entry tables (s_set / s_clr).
 * Each table entry is the GPIO_OUT bitmask for all 8 data pins at once.
 * Writing one byte = 4 register writes: CLR data pins, SET data pins,
 * CLR WR (falling edge latches data), SET WR. Zero branches, zero
 * per-bit shifts in the hot path.
 *
 * ── READ PATH ────────────────────────────────────────────────────
 * Data is sampled WHILE RD IS LOW.
 * ILI9488 §7.4: tOH (data hold after RD↑) = 20 ns minimum.
 * At 240 MHz a gpio_set_level call takes ~100–150 ns, so by the time
 * gpio_set_level(RD,1) returns, the ILI9488 has already released the
 * bus. Sampling after RD rises reads a floating bus (root cause of the
 * 0xFC constant return seen in earlier debugging).
 * Correct sequence: RD↓ → delay ≥ tACC(40ns) → sample GPIO_IN → RD↑ →
 * delay ≥ tRHRL(90ns).
 * We use esp_rom_delay_us(1) = 1 µs which satisfies all minima.
 *
 * ── GPIO REGISTER MAP ────────────────────────────────────────────
 * All data pins D0-D7 and control pins RS/WR/RST are GPIO0-31:
 *   GPIO_OUT_W1TS_REG (0x60004008) — set pins HIGH
 *   GPIO_OUT_W1TC_REG (0x6000400C) — set pins LOW
 *   GPIO_IN_REG       (0x60004038) — read all GPIO0-31
 *   GPIO_ENABLE_W1TS_REG / GPIO_ENABLE_W1TC_REG — output enable
 *
 * RD (GPIO47) and BL (GPIO38) are GPIO32-63:
 *   GPIO_OUT1_W1TS_REG (0x60004014) — set HIGH
 *   GPIO_OUT1_W1TC_REG (0x60004018) — set LOW
 *
 * ── BIT MAPPING ──────────────────────────────────────────────────
 * D0=GPIO16=bit0, D1=GPIO15=bit1, D2=GPIO11=bit2, D3=GPIO10=bit3
 * D4=GPIO9=bit4,  D5=GPIO8=bit5,  D6=GPIO18=bit6, D7=GPIO17=bit7
 *
 * DATA_MASK = (1<<8)|(1<<9)|(1<<10)|(1<<11)|(1<<15)|(1<<16)|(1<<17)|(1<<18)
 *           = 0x00078F00
 *
 * RD in GPIO_OUT1: bit (47-32)=15 → mask 0x00008000
 * BL in GPIO_OUT1: bit (38-32)= 6 → mask 0x00000040
 */

#include "ili9488.h"

#include <string.h>
#include <stdbool.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_rom_sys.h"    /* esp_rom_delay_us()  */
#include "soc/gpio_reg.h"   /* GPIO_OUT_W1TS_REG etc. */

static const char *TAG = "ILI9488";

/* ═══════════════════════════════════════════════════════════════
 * PIN CONSTANTS
 * ═══════════════════════════════════════════════════════════════ */
#define PIN_D0   16
#define PIN_D1   15
#define PIN_D2   11
#define PIN_D3   10
#define PIN_D4    9
#define PIN_D5    8
#define PIN_D6   18
#define PIN_D7   17
#define PIN_RS   13    /* Register Select: 0=command, 1=data */
#define PIN_WR   14
#define PIN_RST  12
#define PIN_RD   47    /* GPIO_OUT1 bank */
#define PIN_BL   38    /* GPIO_OUT1 bank */

/* ═══════════════════════════════════════════════════════════════
 * GPIO BITMASKS — GPIO_OUT bank (pins 0-31)
 * ═══════════════════════════════════════════════════════════════ */
#define BIT_D0   (1U << 16)
#define BIT_D1   (1U << 15)
#define BIT_D2   (1U << 11)
#define BIT_D3   (1U << 10)
#define BIT_D4   (1U <<  9)
#define BIT_D5   (1U <<  8)
#define BIT_D6   (1U << 18)
#define BIT_D7   (1U << 17)
#define BIT_RS   (1U << 13)
#define BIT_WR   (1U << 14)
#define BIT_RST  (1U << 12)

/* All 8 data pins combined */
#define DATA_MASK (BIT_D0|BIT_D1|BIT_D2|BIT_D3|BIT_D4|BIT_D5|BIT_D6|BIT_D7)
/* = 0x00078F00 */

/* ═══════════════════════════════════════════════════════════════
 * GPIO BITMASKS — GPIO_OUT1 bank (pins 32-63)
 * ═══════════════════════════════════════════════════════════════ */
#define BIT1_RD  (1U << (47 - 32))   /* bit15 → 0x00008000 */
#define BIT1_BL  (1U << (38 - 32))   /* bit6  → 0x00000040 */

/* ═══════════════════════════════════════════════════════════════
 * REGISTER ACCESS MACROS
 *
 * W1TS/W1TC (write-1-to-set / write-1-to-clear): atomic, no
 * read-modify-write, interrupt-safe.
 * ═══════════════════════════════════════════════════════════════ */
#define GPIO_SET(m)   REG_WRITE(GPIO_OUT_W1TS_REG,  (m))
#define GPIO_CLR(m)   REG_WRITE(GPIO_OUT_W1TC_REG,  (m))
#define GPIO1_SET(m)  REG_WRITE(GPIO_OUT1_W1TS_REG, (m))
#define GPIO1_CLR(m)  REG_WRITE(GPIO_OUT1_W1TC_REG, (m))
#define GPIO_READ()   REG_READ(GPIO_IN_REG)

/* Output-enable for GPIO0-31 (used to switch D0-D7 in/out for reads) */
#define GPIO_OE_SET(m)  REG_WRITE(GPIO_ENABLE_W1TS_REG, (m))
#define GPIO_OE_CLR(m)  REG_WRITE(GPIO_ENABLE_W1TC_REG, (m))

/* ═══════════════════════════════════════════════════════════════
 * WRITE LOOKUP TABLES
 *
 * s_set[v] — GPIO_OUT bits to SET for data byte v
 * s_clr[v] — GPIO_OUT bits to CLEAR for data byte v (= DATA_MASK & ~s_set[v])
 *
 * Bit mapping:
 *   byte bit 0 → D0 → GPIO16
 *   byte bit 1 → D1 → GPIO15
 *   byte bit 2 → D2 → GPIO11
 *   byte bit 3 → D3 → GPIO10
 *   byte bit 4 → D4 → GPIO9
 *   byte bit 5 → D5 → GPIO8
 *   byte bit 6 → D6 → GPIO18
 *   byte bit 7 → D7 → GPIO17
 *
 * Generated and verified by the Python script in build notes.
 * All 256 entries round-trip correctly (encode→decode = identity).
 * ═══════════════════════════════════════════════════════════════ */
static const uint32_t s_set[256] = {
    0x00000000U, 0x00010000U, 0x00008000U, 0x00018000U, 0x00000800U, 0x00010800U, 0x00008800U, 0x00018800U,
    0x00000400U, 0x00010400U, 0x00008400U, 0x00018400U, 0x00000C00U, 0x00010C00U, 0x00008C00U, 0x00018C00U,
    0x00000200U, 0x00010200U, 0x00008200U, 0x00018200U, 0x00000A00U, 0x00010A00U, 0x00008A00U, 0x00018A00U,
    0x00000600U, 0x00010600U, 0x00008600U, 0x00018600U, 0x00000E00U, 0x00010E00U, 0x00008E00U, 0x00018E00U,
    0x00000100U, 0x00010100U, 0x00008100U, 0x00018100U, 0x00000900U, 0x00010900U, 0x00008900U, 0x00018900U,
    0x00000500U, 0x00010500U, 0x00008500U, 0x00018500U, 0x00000D00U, 0x00010D00U, 0x00008D00U, 0x00018D00U,
    0x00000300U, 0x00010300U, 0x00008300U, 0x00018300U, 0x00000B00U, 0x00010B00U, 0x00008B00U, 0x00018B00U,
    0x00000700U, 0x00010700U, 0x00008700U, 0x00018700U, 0x00000F00U, 0x00010F00U, 0x00008F00U, 0x00018F00U,
    0x00040000U, 0x00050000U, 0x00048000U, 0x00058000U, 0x00040800U, 0x00050800U, 0x00048800U, 0x00058800U,
    0x00040400U, 0x00050400U, 0x00048400U, 0x00058400U, 0x00040C00U, 0x00050C00U, 0x00048C00U, 0x00058C00U,
    0x00040200U, 0x00050200U, 0x00048200U, 0x00058200U, 0x00040A00U, 0x00050A00U, 0x00048A00U, 0x00058A00U,
    0x00040600U, 0x00050600U, 0x00048600U, 0x00058600U, 0x00040E00U, 0x00050E00U, 0x00048E00U, 0x00058E00U,
    0x00040100U, 0x00050100U, 0x00048100U, 0x00058100U, 0x00040900U, 0x00050900U, 0x00048900U, 0x00058900U,
    0x00040500U, 0x00050500U, 0x00048500U, 0x00058500U, 0x00040D00U, 0x00050D00U, 0x00048D00U, 0x00058D00U,
    0x00040300U, 0x00050300U, 0x00048300U, 0x00058300U, 0x00040B00U, 0x00050B00U, 0x00048B00U, 0x00058B00U,
    0x00040700U, 0x00050700U, 0x00048700U, 0x00058700U, 0x00040F00U, 0x00050F00U, 0x00048F00U, 0x00058F00U,
    0x00020000U, 0x00030000U, 0x00028000U, 0x00038000U, 0x00020800U, 0x00030800U, 0x00028800U, 0x00038800U,
    0x00020400U, 0x00030400U, 0x00028400U, 0x00038400U, 0x00020C00U, 0x00030C00U, 0x00028C00U, 0x00038C00U,
    0x00020200U, 0x00030200U, 0x00028200U, 0x00038200U, 0x00020A00U, 0x00030A00U, 0x00028A00U, 0x00038A00U,
    0x00020600U, 0x00030600U, 0x00028600U, 0x00038600U, 0x00020E00U, 0x00030E00U, 0x00028E00U, 0x00038E00U,
    0x00020100U, 0x00030100U, 0x00028100U, 0x00038100U, 0x00020900U, 0x00030900U, 0x00028900U, 0x00038900U,
    0x00020500U, 0x00030500U, 0x00028500U, 0x00038500U, 0x00020D00U, 0x00030D00U, 0x00028D00U, 0x00038D00U,
    0x00020300U, 0x00030300U, 0x00028300U, 0x00038300U, 0x00020B00U, 0x00030B00U, 0x00028B00U, 0x00038B00U,
    0x00020700U, 0x00030700U, 0x00028700U, 0x00038700U, 0x00020F00U, 0x00030F00U, 0x00028F00U, 0x00038F00U,
    0x00060000U, 0x00070000U, 0x00068000U, 0x00078000U, 0x00060800U, 0x00070800U, 0x00068800U, 0x00078800U,
    0x00060400U, 0x00070400U, 0x00068400U, 0x00078400U, 0x00060C00U, 0x00070C00U, 0x00068C00U, 0x00078C00U,
    0x00060200U, 0x00070200U, 0x00068200U, 0x00078200U, 0x00060A00U, 0x00070A00U, 0x00068A00U, 0x00078A00U,
    0x00060600U, 0x00070600U, 0x00068600U, 0x00078600U, 0x00060E00U, 0x00070E00U, 0x00068E00U, 0x00078E00U,
    0x00060100U, 0x00070100U, 0x00068100U, 0x00078100U, 0x00060900U, 0x00070900U, 0x00068900U, 0x00078900U,
    0x00060500U, 0x00070500U, 0x00068500U, 0x00078500U, 0x00060D00U, 0x00070D00U, 0x00068D00U, 0x00078D00U,
    0x00060300U, 0x00070300U, 0x00068300U, 0x00078300U, 0x00060B00U, 0x00070B00U, 0x00068B00U, 0x00078B00U,
    0x00060700U, 0x00070700U, 0x00068700U, 0x00078700U, 0x00060F00U, 0x00070F00U, 0x00068F00U, 0x00078F00U,
};

static const uint32_t s_clr[256] = {
    0x00078F00U, 0x00068F00U, 0x00070F00U, 0x00060F00U, 0x00078700U, 0x00068700U, 0x00070700U, 0x00060700U,
    0x00078B00U, 0x00068B00U, 0x00070B00U, 0x00060B00U, 0x00078300U, 0x00068300U, 0x00070300U, 0x00060300U,
    0x00078D00U, 0x00068D00U, 0x00070D00U, 0x00060D00U, 0x00078500U, 0x00068500U, 0x00070500U, 0x00060500U,
    0x00078900U, 0x00068900U, 0x00070900U, 0x00060900U, 0x00078100U, 0x00068100U, 0x00070100U, 0x00060100U,
    0x00078E00U, 0x00068E00U, 0x00070E00U, 0x00060E00U, 0x00078600U, 0x00068600U, 0x00070600U, 0x00060600U,
    0x00078A00U, 0x00068A00U, 0x00070A00U, 0x00060A00U, 0x00078200U, 0x00068200U, 0x00070200U, 0x00060200U,
    0x00078C00U, 0x00068C00U, 0x00070C00U, 0x00060C00U, 0x00078400U, 0x00068400U, 0x00070400U, 0x00060400U,
    0x00078800U, 0x00068800U, 0x00070800U, 0x00060800U, 0x00078000U, 0x00068000U, 0x00070000U, 0x00060000U,
    0x00038F00U, 0x00028F00U, 0x00030F00U, 0x00020F00U, 0x00038700U, 0x00028700U, 0x00030700U, 0x00020700U,
    0x00038B00U, 0x00028B00U, 0x00030B00U, 0x00020B00U, 0x00038300U, 0x00028300U, 0x00030300U, 0x00020300U,
    0x00038D00U, 0x00028D00U, 0x00030D00U, 0x00020D00U, 0x00038500U, 0x00028500U, 0x00030500U, 0x00020500U,
    0x00038900U, 0x00028900U, 0x00030900U, 0x00020900U, 0x00038100U, 0x00028100U, 0x00030100U, 0x00020100U,
    0x00038E00U, 0x00028E00U, 0x00030E00U, 0x00020E00U, 0x00038600U, 0x00028600U, 0x00030600U, 0x00020600U,
    0x00038A00U, 0x00028A00U, 0x00030A00U, 0x00020A00U, 0x00038200U, 0x00028200U, 0x00030200U, 0x00020200U,
    0x00038C00U, 0x00028C00U, 0x00030C00U, 0x00020C00U, 0x00038400U, 0x00028400U, 0x00030400U, 0x00020400U,
    0x00038800U, 0x00028800U, 0x00030800U, 0x00020800U, 0x00038000U, 0x00028000U, 0x00030000U, 0x00020000U,
    0x00058F00U, 0x00048F00U, 0x00050F00U, 0x00040F00U, 0x00058700U, 0x00048700U, 0x00050700U, 0x00040700U,
    0x00058B00U, 0x00048B00U, 0x00050B00U, 0x00040B00U, 0x00058300U, 0x00048300U, 0x00050300U, 0x00040300U,
    0x00058D00U, 0x00048D00U, 0x00050D00U, 0x00040D00U, 0x00058500U, 0x00048500U, 0x00050500U, 0x00040500U,
    0x00058900U, 0x00048900U, 0x00050900U, 0x00040900U, 0x00058100U, 0x00048100U, 0x00050100U, 0x00040100U,
    0x00058E00U, 0x00048E00U, 0x00050E00U, 0x00040E00U, 0x00058600U, 0x00048600U, 0x00050600U, 0x00040600U,
    0x00058A00U, 0x00048A00U, 0x00050A00U, 0x00040A00U, 0x00058200U, 0x00048200U, 0x00050200U, 0x00040200U,
    0x00058C00U, 0x00048C00U, 0x00050C00U, 0x00040C00U, 0x00058400U, 0x00048400U, 0x00050400U, 0x00040400U,
    0x00058800U, 0x00048800U, 0x00050800U, 0x00040800U, 0x00058000U, 0x00048000U, 0x00050000U, 0x00040000U,
    0x00018F00U, 0x00008F00U, 0x00010F00U, 0x00000F00U, 0x00018700U, 0x00008700U, 0x00010700U, 0x00000700U,
    0x00018B00U, 0x00008B00U, 0x00010B00U, 0x00000B00U, 0x00018300U, 0x00008300U, 0x00010300U, 0x00000300U,
    0x00018D00U, 0x00008D00U, 0x00010D00U, 0x00000D00U, 0x00018500U, 0x00008500U, 0x00010500U, 0x00000500U,
    0x00018900U, 0x00008900U, 0x00010900U, 0x00000900U, 0x00018100U, 0x00008100U, 0x00010100U, 0x00000100U,
    0x00018E00U, 0x00008E00U, 0x00010E00U, 0x00000E00U, 0x00018600U, 0x00008600U, 0x00010600U, 0x00000600U,
    0x00018A00U, 0x00008A00U, 0x00010A00U, 0x00000A00U, 0x00018200U, 0x00008200U, 0x00010200U, 0x00000200U,
    0x00018C00U, 0x00008C00U, 0x00010C00U, 0x00000C00U, 0x00018400U, 0x00008400U, 0x00010400U, 0x00000400U,
    0x00018800U, 0x00008800U, 0x00010800U, 0x00000800U, 0x00018000U, 0x00008000U, 0x00010000U, 0x00000000U,
};

/* ═══════════════════════════════════════════════════════════════
 * BUS WRITE PRIMITIVES
 * ═══════════════════════════════════════════════════════════════ */

/*
 * write8 — place byte v on D0-D7 and pulse WR.
 * RS must be set before calling.
 * 4 register writes: clear wrong bits, set correct bits, WR↓, WR↑.
 */
static inline void write8(uint8_t v)
{
    GPIO_CLR(s_clr[v]);
    GPIO_SET(s_set[v]);
    GPIO_CLR(BIT_WR);   /* WR falls — ILI9488 latches on this edge */
    GPIO_SET(BIT_WR);   /* WR rises */
}

/*
 * send_cmd — RS=0, write command byte.
 */
static inline void send_cmd(uint8_t cmd)
{
    GPIO_CLR(BIT_RS);
    write8(cmd);
}

/*
 * send_data — RS=1, write data byte.
 */
static inline void send_dat(uint8_t d)
{
    GPIO_SET(BIT_RS);
    write8(d);
}

/* ═══════════════════════════════════════════════════════════════
 * BUS READ PRIMITIVES
 *
 * decode_bus — extract byte from GPIO_IN snapshot.
 *
 * GPIO_IN bit → byte bit mapping (inverse of s_set):
 *   GPIO16 (bit16) → byte bit0  (shift right 16)
 *   GPIO15 (bit15) → byte bit1  (shift right 14)
 *   GPIO11 (bit11) → byte bit2  (shift right  9)
 *   GPIO10 (bit10) → byte bit3  (shift right  7)
 *   GPIO9  (bit9)  → byte bit4  (shift right  5)
 *   GPIO8  (bit8)  → byte bit5  (shift right  3)
 *   GPIO18 (bit18) → byte bit6  (shift right 12)
 *   GPIO17 (bit17) → byte bit7  (shift right 10)
 *
 * 8 shifts + 8 ANDs + 7 ORs = 23 instructions, no branches, no table.
 * ═══════════════════════════════════════════════════════════════ */
static inline uint8_t decode_bus(uint32_t raw)
{
    return (uint8_t)(
        ((raw >> 16) & 0x01U) |   /* bit0 ← GPIO16 */
        ((raw >> 14) & 0x02U) |   /* bit1 ← GPIO15 */
        ((raw >>  9) & 0x04U) |   /* bit2 ← GPIO11 */
        ((raw >>  7) & 0x08U) |   /* bit3 ← GPIO10 */
        ((raw >>  5) & 0x10U) |   /* bit4 ← GPIO9  */
        ((raw >>  3) & 0x20U) |   /* bit5 ← GPIO8  */
        ((raw >> 12) & 0x40U) |   /* bit6 ← GPIO18 */
        ((raw >> 10) & 0x80U)     /* bit7 ← GPIO17 */
    );
}

/*
 * read8_while_low — one RD pulse, sample GPIO_IN while RD is LOW.
 *
 * ILI9488 §7.4 timing:
 *   tACC  (RD↓ → data valid) MAX 40 ns  — esp_rom_delay_us(1) >> 40ns ✓
 *   tRLRL (RD low width)     MIN 45 ns  — delay keeps RD low >1µs    ✓
 *   tOH   (hold after RD↑)   MIN 20 ns  — we sample BEFORE RD rises  ✓
 *   tRHRL (RD high between)  MIN 90 ns  — delay after RD↑            ✓
 *
 * CRITICAL: sample is taken AFTER delay but BEFORE GPIO1_SET(RD).
 * Sampling after RD rises risks reading a floating bus (root cause
 * of constant 0xFC return seen during bring-up debugging).
 */
static inline uint8_t read8_while_low(void)
{
    GPIO1_CLR(BIT1_RD);           /* RD falls                              */
    esp_rom_delay_us(1);          /* wait ≥ tACC = 40 ns max (1 µs >> 40) */
    uint32_t raw = GPIO_READ();   /* sample ALL GPIO0-31 while RD is LOW   */
    GPIO1_SET(BIT1_RD);           /* RD rises                              */
    esp_rom_delay_us(1);          /* tRHRL ≥ 90 ns (1 µs >> 90 ns)        */
    return decode_bus(raw);
}

/* ═══════════════════════════════════════════════════════════════
 * GPIO INITIALISATION
 *
 * gpio_reset_pin() selects IOMUX function 0 (direct GPIO mode),
 * bypassing the GPIO matrix entirely. This is critical: any previous
 * esp_lcd session may have left the GPIO matrix routing LCD peripheral
 * signals to these pins. gpio_reset_pin() clears that routing and
 * returns the pin to direct CPU control, immune to peripheral state.
 * ═══════════════════════════════════════════════════════════════ */
static void gpio_init_pins(void)
{
    const int all[] = {
        PIN_D0, PIN_D1, PIN_D2, PIN_D3,
        PIN_D4, PIN_D5, PIN_D6, PIN_D7,
        PIN_RS, PIN_WR, PIN_RST, PIN_RD, PIN_BL
    };
    for (int i = 0; i < (int)(sizeof(all) / sizeof(all[0])); i++) {
        gpio_reset_pin(all[i]);
        gpio_set_direction(all[i], GPIO_MODE_OUTPUT);
    }

    /* Idle states */
    GPIO_SET(BIT_RS | BIT_WR | BIT_RST | DATA_MASK);  /* RS/WR/RST/data HIGH */
    GPIO1_SET(BIT1_RD);                                /* RD HIGH (idle)      */
    GPIO1_CLR(BIT1_BL);                               /* BL OFF — must stay
                                                          off during reset      */
}

/* ═══════════════════════════════════════════════════════════════
 * PUBLIC: ili9488_reset
 *
 * Timing: RST=1 → 250ms → RST=0 → 250ms → RST=1 → 250ms
 *
 * Why 250ms (not the 5ms in the ILI9488 datasheet):
 *   The datasheet specifies timing for the controller IC only.
 *   This panel module's gate driver and source driver hardware
 *   require ~250ms after reset to fully initialise. With shorter
 *   delays, register reads work (controller ready in ~5ms) but the
 *   display stays white (panel hardware not ready). This was the
 *   single longest debugging session — confirmed by systematic
 *   delay sweeps during bring-up.
 *
 * BL is explicitly held LOW. If BL is enabled before reset
 * completes, the inrush current causes supply sag or ground bounce
 * that either corrupts the init sequence or re-triggers POR.
 * ═══════════════════════════════════════════════════════════════ */
void ili9488_reset(void)
{
    GPIO1_CLR(BIT1_BL);             /* BL OFF — enforce before touching RST */
    GPIO_SET(BIT_RST);
    vTaskDelay(pdMS_TO_TICKS(250));
    GPIO_CLR(BIT_RST);
    vTaskDelay(pdMS_TO_TICKS(250));
    GPIO_SET(BIT_RST);
    vTaskDelay(pdMS_TO_TICKS(250));
    ESP_LOGI(TAG, "reset done");
}

/* ═══════════════════════════════════════════════════════════════
 * PUBLIC: ili9488_write_cmd
 * ═══════════════════════════════════════════════════════════════ */
void ili9488_write_cmd(uint8_t cmd)
{
    send_cmd(cmd);
}

/* ═══════════════════════════════════════════════════════════════
 * PUBLIC: ili9488_write_data
 * ═══════════════════════════════════════════════════════════════ */
void ili9488_write_data(uint8_t cmd, const uint8_t *data, size_t len)
{
    send_cmd(cmd);
    for (size_t i = 0; i < len; i++) {
        send_dat(data[i]);
    }
}

/* ═══════════════════════════════════════════════════════════════
 * PUBLIC: ili9488_read
 *
 * Full read cycle — see header for protocol description.
 *
 * D0-D7 input switchover:
 *   GPIO_OE_CLR(DATA_MASK) clears the output-enable bits for all
 *   8 data pins simultaneously, tri-stating the ESP32-S3 output
 *   drivers. The pins were placed in IOMUX direct mode by
 *   gpio_reset_pin(), so no GPIO matrix interaction occurs.
 *   GPIO_OE_SET(DATA_MASK) restores output mode after the read.
 * ═══════════════════════════════════════════════════════════════ */
esp_err_t ili9488_read(uint8_t cmd, uint8_t *buf, size_t len)
{
    if (len == 0 || len > 255) return ESP_FAIL;

    /* 1. Send command byte */
    send_cmd(cmd);

    /* 2. Switch D0-D7 to inputs (tri-state ESP32-S3 output drivers) */
    GPIO_OE_CLR(DATA_MASK);

    /* 3. RS=1 (data phase), WR stays HIGH */
    GPIO_SET(BIT_RS);
    esp_rom_delay_us(1);   /* RS setup time before first RD pulse */

    /* 4. Mandatory dummy RD cycle — ILI9488 §6.5 / Table 8-3.
     *    ALL register reads produce one dummy byte before valid data.
     *    Discarding it here means buf[0] is always the first valid byte. */
    (void)read8_while_low();

    /* 5. Read len valid bytes */
    for (size_t i = 0; i < len; i++) {
        buf[i] = read8_while_low();
    }

    /* 6. Restore D0-D7 to outputs and drive them HIGH (idle) */
    GPIO_OE_SET(DATA_MASK);
    GPIO_SET(DATA_MASK);

    return ESP_OK;
}

/* ═══════════════════════════════════════════════════════════════
 * PUBLIC: ili9488_fill
 *
 * Window → RAMWR → 480×320 pixels as big-endian RGB565.
 *
 * Window:
 *   CASET: 0x0000 to 0x01DF (col 0-479)   0x01DF = 479
 *   PASET: 0x0000 to 0x013F (row 0-319)   0x013F = 319
 *
 * Pixel loop optimisation:
 *   The two bytes of the RGB565 colour are constant for a fill.
 *   Their GPIO masks are computed once before the loop, avoiding
 *   256-entry table lookups inside the 153,600-iteration hot path.
 *
 * Throughput at 240 MHz:
 *   Each pixel = 8 REG_WRITE calls (4 per byte × 2 bytes).
 *   REG_WRITE compiles to a single store instruction ~4ns.
 *   ~32ns per pixel theoretical → ~5 MP/s → full frame ~30ms.
 *   In practice ~2-3 MP/s accounting for FreeRTOS overhead.
 * ═══════════════════════════════════════════════════════════════ */
void ili9488_fill(uint16_t color)
{
    /* Set full-screen window */
    static const uint8_t caset[] = {0x00, 0x00, 0x01, 0xDF};  /* 0..479 */
    static const uint8_t paset[] = {0x00, 0x00, 0x01, 0x3F};  /* 0..319 */
    ili9488_write_data(0x2A, caset, sizeof(caset));
    ili9488_write_data(0x2B, paset, sizeof(paset));

    /* RAMWR — resets GRAM write pointer to (SC, SP) = (0, 0) */
    send_cmd(0x2C);

    /* Precompute GPIO masks for both bytes of this colour */
    uint8_t  hi     = (uint8_t)(color >> 8);
    uint8_t  lo     = (uint8_t)(color & 0xFF);
    uint32_t hi_set = s_set[hi], hi_clr = s_clr[hi];
    uint32_t lo_set = s_set[lo], lo_clr = s_clr[lo];

    /* Stream 480 × 320 = 153,600 pixels, RS stays HIGH throughout */
    GPIO_SET(BIT_RS);
    for (uint32_t i = 0; i < (uint32_t)ILI9488_W * ILI9488_H; i++) {
        /* high byte */
        GPIO_CLR(hi_clr); GPIO_SET(hi_set); GPIO_CLR(BIT_WR); GPIO_SET(BIT_WR);
        /* low byte */
        GPIO_CLR(lo_clr); GPIO_SET(lo_set); GPIO_CLR(BIT_WR); GPIO_SET(BIT_WR);
    }
}

/* ═══════════════════════════════════════════════════════════════
 * PUBLIC: ili9488_init
 *
 * Exact replica of the confirmed-working Python bring-up sequence.
 * No extra registers beyond what is proven to work.
 * ═══════════════════════════════════════════════════════════════ */
bool ili9488_init(void)
{
    /* Phase 1: configure all GPIOs in IOMUX direct mode */
    gpio_init_pins();

    /* Phase 2: hardware reset */
    ili9488_reset();

    /* Phase 3: init sequence
     *
     * 0x01 SWRESET — reset all registers to default.
     *   Wait 150ms (datasheet minimum 5ms; extra margin is free).
     *
     * 0x11 SLPOUT — exit sleep, enable oscillator and power rails.
     *   Wait 150ms (datasheet minimum 5ms after SLPOUT before DISPON).
     *
     * 0x3A COLMOD = 0x55 — RGB565, 16-bit, 2 bytes per pixel.
     *   RGB666 (0x66) causes byte-alignment striping; RGB565 confirmed working.
     *
     * 0x36 MADCTL = 0x48 — MX=1 (flip columns), BGR=1 (panel colour filter).
     *   Produces correct colours and orientation for this panel.
     *
     * 0x29 DISPON — enable display output gate/source drivers.
     *   Wait 50ms for panel hardware to stabilise before BL enable.
     */
    send_cmd(0x01);
    vTaskDelay(pdMS_TO_TICKS(150));

    send_cmd(0x11);
    vTaskDelay(pdMS_TO_TICKS(150));

    {
        uint8_t d;
        d = 0x55; ili9488_write_data(0x3A, &d, 1);  /* COLMOD = RGB565 */
        d = 0x48; ili9488_write_data(0x36, &d, 1);  /* MADCTL          */
    }

    send_cmd(0x29);
    vTaskDelay(pdMS_TO_TICKS(50));

    /* Phase 4: backlight ON — only after DISPON + 50ms settling.
     * Enabling BL earlier causes supply sag that corrupts init. */
    GPIO1_SET(BIT1_BL);

    ESP_LOGI(TAG, "init complete");
    return true;
}
