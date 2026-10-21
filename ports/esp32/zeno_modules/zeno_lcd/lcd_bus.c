/*
 * lcd_bus.c  --  see lcd_bus.h
 *
 * Differences from the original reg_lcd_write_data()/lcd_write_*:
 *  - The original wrote each of the 8 data pins with its own GPIO register
 *    write, then toggled WR with gpio_set_level(). Here the 8 data pins are
 *    changed with ONE set-mask write and ONE clear-mask write (LUT), and WR is
 *    toggled with direct register writes. Pin order and polarity are unchanged:
 *    data is settled before WR falls and held until after WR rises.
 *  - Solid fills and blits stream inside ONE CS/RS transaction instead of one
 *    malloc()'d column at a time.
 *  - WR low/high hold is a tunable NOP count (see zeno_lcd_config.h).
 */
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "driver/gpio.h"
#include "soc/soc.h"
#include "soc/gpio_reg.h"
#include "esp_log.h"

#include "lcd_bus.h"
#include "zeno_lcd_config.h"
#include "zeno_pure.h"

static const char *TAG = "zeno_lcd_bus";

_Static_assert(ZLCD_PIN_D0 < 32 && ZLCD_PIN_D1 < 32 && ZLCD_PIN_D2 < 32 &&
               ZLCD_PIN_D3 < 32 && ZLCD_PIN_D4 < 32 && ZLCD_PIN_D5 < 32 &&
               ZLCD_PIN_D6 < 32 && ZLCD_PIN_D7 < 32 && ZLCD_PIN_WR < 32 &&
               ZLCD_PIN_RS < 32 && ZLCD_PIN_CS < 32,
               "fast path writes GPIO_OUT_W1TS/W1TC (GPIO0-31) only");

#define PINBIT(p)  (1UL << (p))
#define CS_MASK    PINBIT(ZLCD_PIN_CS)
#define RS_MASK    PINBIT(ZLCD_PIN_RS)
#define WR_MASK    PINBIT(ZLCD_PIN_WR)

static SemaphoreHandle_t s_mutex;
static uint32_t s_lut[256];
static uint32_t s_data_mask;
static volatile uint8_t s_low_nops  = ZLCD_WR_LOW_NOPS_DEFAULT;
static volatile uint8_t s_high_nops = ZLCD_WR_HIGH_NOPS_DEFAULT;
static bool s_inited;

static inline __attribute__((always_inline)) void nops(unsigned n)
{
    while (n--) {
        __asm__ __volatile__("nop");
    }
}

static inline __attribute__((always_inline))
void put_byte(uint32_t set, unsigned lo, unsigned hi)
{
    REG_WRITE(GPIO_OUT_W1TS_REG, set);                 /* data bits that are 1 */
    REG_WRITE(GPIO_OUT_W1TC_REG, s_data_mask ^ set);   /* data bits that are 0 */
    REG_WRITE(GPIO_OUT_W1TC_REG, WR_MASK);             /* WR low               */
    nops(lo);
    REG_WRITE(GPIO_OUT_W1TS_REG, WR_MASK);             /* WR high: latch       */
    nops(hi);
}

static inline __attribute__((always_inline)) void toggle_wr(unsigned lo, unsigned hi)
{
    REG_WRITE(GPIO_OUT_W1TC_REG, WR_MASK);
    nops(lo);
    REG_WRITE(GPIO_OUT_W1TS_REG, WR_MASK);
    nops(hi);
}

esp_err_t zl_bus_init(void)
{
    if (s_mutex == NULL) {
        s_mutex = xSemaphoreCreateMutex();
        if (s_mutex == NULL) {
            return ESP_ERR_NO_MEM;
        }
    }

    static const uint8_t data_pins[8] = {
        ZLCD_PIN_D0, ZLCD_PIN_D1, ZLCD_PIN_D2, ZLCD_PIN_D3,
        ZLCD_PIN_D4, ZLCD_PIN_D5, ZLCD_PIN_D6, ZLCD_PIN_D7,
    };
    s_data_mask = zl_lut_build(data_pins, s_lut);

    /* Same order and same calls as lcd_interface_cfg() in lcd_com.c */
    gpio_reset_pin(ZLCD_PIN_CS);
    gpio_set_level(ZLCD_PIN_CS, 1);            /* level first: no low glitch */
    gpio_set_direction(ZLCD_PIN_CS, GPIO_MODE_OUTPUT);

    gpio_reset_pin(ZLCD_PIN_RS);
    gpio_set_level(ZLCD_PIN_RS, 1);            /* level first: no low glitch */
    gpio_set_direction(ZLCD_PIN_RS, GPIO_MODE_OUTPUT);

    gpio_reset_pin(ZLCD_PIN_WR);
    gpio_set_level(ZLCD_PIN_WR, 1);            /* level first: no low glitch */
    gpio_set_direction(ZLCD_PIN_WR, GPIO_MODE_OUTPUT);

    gpio_reset_pin(ZLCD_PIN_RD);
    gpio_set_level(ZLCD_PIN_RD, 1);            /* level first: no low glitch */
    gpio_set_direction(ZLCD_PIN_RD, GPIO_MODE_OUTPUT);

    for (int i = 0; i < 8; i++) {
        gpio_reset_pin(data_pins[i]);
    }
    for (int i = 0; i < 8; i++) {
        gpio_set_direction(data_pins[i], GPIO_MODE_OUTPUT);
    }

    gpio_reset_pin(ZLCD_PIN_RESET);
    gpio_set_level(ZLCD_PIN_RESET, 1);          /* level first: no low glitch */
    gpio_set_direction(ZLCD_PIN_RESET, GPIO_MODE_OUTPUT);

    s_inited = true;
    ESP_LOGI(TAG, "bus ready (register mode)");
    return ESP_OK;
}

bool zl_bus_is_init(void) { return s_inited; }

bool zl_bus_lock(void)
{
    return s_mutex != NULL && xSemaphoreTake(s_mutex, 0) == pdTRUE;
}

void zl_bus_unlock(void)
{
    if (s_mutex != NULL) {
        xSemaphoreGive(s_mutex);
    }
}

void zl_bus_set_timing(uint8_t low_nops, uint8_t high_nops)
{
    s_low_nops = low_nops;
    s_high_nops = high_nops;
}

void zl_bus_get_timing(uint8_t *low_nops, uint8_t *high_nops)
{
    if (low_nops)  *low_nops  = s_low_nops;
    if (high_nops) *high_nops = s_high_nops;
}

void zl_bus_reset_level(int level)
{
    gpio_set_level(ZLCD_PIN_RESET, level);
}

void zl_bus_write_cmd(uint8_t cmd)
{
    const unsigned lo = s_low_nops, hi = s_high_nops;
    REG_WRITE(GPIO_OUT_W1TC_REG, CS_MASK | RS_MASK);   /* CS low, RS low (command) */
    put_byte(s_lut[cmd], lo, hi);
    REG_WRITE(GPIO_OUT_W1TS_REG, CS_MASK);             /* CS high */
}

void zl_bus_write_data(const uint8_t *d, size_t n)
{
    const unsigned lo = s_low_nops, hi = s_high_nops;
    REG_WRITE(GPIO_OUT_W1TC_REG, CS_MASK);             /* CS low   */
    REG_WRITE(GPIO_OUT_W1TS_REG, RS_MASK);             /* RS high (data) */
    for (size_t i = 0; i < n; i++) {
        put_byte(s_lut[d[i]], lo, hi);
    }
    REG_WRITE(GPIO_OUT_W1TS_REG, CS_MASK);             /* CS high  */
}

void zl_bus_write_bytes(const uint8_t *d, size_t n)
{
    zl_bus_write_data(d, n);
}

void zl_bus_write_color(uint16_t color, uint32_t npix)
{
    const unsigned lo = s_low_nops, hi = s_high_nops;
    const uint8_t hb = (uint8_t)(color >> 8), lb = (uint8_t)(color & 0xFF);
    const uint32_t hs = s_lut[hb], ls = s_lut[lb];

    REG_WRITE(GPIO_OUT_W1TC_REG, CS_MASK);
    REG_WRITE(GPIO_OUT_W1TS_REG, RS_MASK);
    if (hb == lb) {
        /* data lines do not change between the two bytes: set once, strobe 2*n */
        REG_WRITE(GPIO_OUT_W1TS_REG, hs);
        REG_WRITE(GPIO_OUT_W1TC_REG, s_data_mask ^ hs);
        for (uint32_t i = 0; i < npix * 2u; i++) {
            toggle_wr(lo, hi);
        }
    } else {
        for (uint32_t i = 0; i < npix; i++) {
            put_byte(hs, lo, hi);
            put_byte(ls, lo, hi);
        }
    }
    REG_WRITE(GPIO_OUT_W1TS_REG, CS_MASK);
}

void zl_bus_idle(void)
{
    REG_WRITE(GPIO_OUT_W1TS_REG, CS_MASK | RS_MASK | WR_MASK);
}
