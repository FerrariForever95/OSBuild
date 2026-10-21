#include <string.h>
#include "esp_rom_sys.h"
#include "ili9488_panel.h"
#include "lcd_bus.h"
#include "zeno_lcd_config.h"
#include "zeno_pure.h"

#define TFTLCD_DELAY8 0x7F          /* same marker as lcd_com.h */

static int      s_rot;
static uint8_t  s_madctl;
static uint16_t s_w = ZLCD_NATIVE_HEIGHT;   /* default landscape until init */
static uint16_t s_h = ZLCD_NATIVE_WIDTH;
static bool     s_ready;
static void   (*s_delay_fn)(uint32_t);

static void delay_ms(uint32_t ms)
{
    if (s_delay_fn) {
        s_delay_fn(ms);
    } else {
        esp_rom_delay_us(ms * 1000u);
    }
}

void zl_panel_set_delay_fn(void (*fn)(uint32_t ms)) { s_delay_fn = fn; }

/* Hardware reset: RST high (settle) -> low (>=10 ms, datasheet min is 10 us)
 * -> high, then wait 150 ms before ANY command (ILI9488 needs ~120 ms after
 * RST release). The first call after power-up also waits for the panel supply
 * and RC reset network to stabilise -- the cause of "blank on cold boot,
 * fine on warm reboot" (on a warm reboot the panel is already powered). */
void zl_panel_hw_reset(void)
{
    static bool s_first = true;
    if (s_first) {
        delay_ms(120);              /* cold-boot power settle */
        s_first = false;
    }
    zl_bus_reset_level(1);
    delay_ms(20);
    zl_bus_reset_level(0);
    delay_ms(20);
    zl_bus_reset_level(1);
    delay_ms(150);                  /* controller loads defaults after RST */
    s_ready = false;                /* panel is now uninitialised */
}

/* ---- init tables: byte-for-byte from components/tft_library/ili9488.c ---- */
static const uint8_t reset_off[] = {
    0x01, 0,                /* Soft Reset */
    TFTLCD_DELAY8, 150,
    0x28, 0,                /* Display Off */
    0x3A, 1, 0x55,          /* Pixel read=565, write=565 */
};

/* regValues of the original, split around the 0x36 (Memory Access) entry so
 * the rotation value can be substituted without changing the command order. */
static const uint8_t reg_before_madctl[] = {
    0xC0, 2, 0x10, 0x10,
    0xC1, 1, 0x41,
    0xC5, 4, 0x00, 0x22, 0x80, 0x40,
};
/* 0x36, 1, <madctl>   (original: 0x98) */
static const uint8_t reg_after_madctl[] = {
    0xB0, 1, 0x00,
    0xB1, 2, 0xB0, 0x11,
    0xB4, 1, 0x02,
    0xB6, 3, 0x02, 0x02, 0x3B,
    0xB7, 1, 0xC6,
    0x3A, 1, 0x55,
    0xF7, 4, 0xA9, 0x51, 0x2C, 0x82,
    /* original gamma tables (0xE0/0xE1) are commented out; kept disabled */
};

static const uint8_t wake_on[] = {
    0x11, 0,                /* Sleep Out */
    TFTLCD_DELAY8, 150,
    0x29, 0,                /* Display On */
};

/* Same walk as lcd_write_table(); delays run WITHOUT the bus lock, each
 * command is its own locked transaction, parameters are sent one byte per
 * transaction exactly like lcd_write_data_byte(). */
static esp_err_t exec_table(const uint8_t *p, int size)
{
    while (size > 0) {
        uint8_t cmd = *p++;
        uint8_t len = *p++;
        if (cmd == TFTLCD_DELAY8) {
            delay_ms(len);
            len = 0;
        } else {
            if (!zl_bus_lock()) {
                return ESP_ERR_TIMEOUT;
            }
            zl_bus_write_cmd(cmd);
            for (int i = 0; i < len; i++) {
                uint8_t b = *p++;
                zl_bus_write_data(&b, 1);
            }
            zl_bus_unlock();
        }
        size -= len + 2;
    }
    return ESP_OK;
}

static esp_err_t write_madctl(uint8_t m)
{
    if (!zl_bus_lock()) {
        return ESP_ERR_TIMEOUT;
    }
    zl_bus_write_cmd(0x36);
    zl_bus_write_data(&m, 1);
    zl_bus_unlock();
    return ESP_OK;
}

static void apply_geometry(int rot, uint8_t m)
{
    s_rot = rot;
    s_madctl = m;
    if (m & 0x20) {                       /* MV set: rows/columns exchanged */
        s_w = ZLCD_NATIVE_HEIGHT;
        s_h = ZLCD_NATIVE_WIDTH;
    } else {
        s_w = ZLCD_NATIVE_WIDTH;
        s_h = ZLCD_NATIVE_HEIGHT;
    }
}

esp_err_t zl_panel_init(int rotation, int madctl_override)
{
    if (!zl_bus_is_init()) {
        return ESP_ERR_INVALID_STATE;
    }
    if (rotation < 0 || rotation >= ZL_NUM_ROTATIONS ||
        madctl_override > 0xFF) {
        return ESP_ERR_INVALID_ARG;
    }
    uint8_t m = madctl_override >= 0 ? (uint8_t)madctl_override
                                     : zl_madctl_table[rotation];
    esp_err_t e;

    zl_panel_hw_reset();
    if ((e = exec_table(reset_off, sizeof(reset_off))) != ESP_OK) return e;
    if ((e = exec_table(reg_before_madctl, sizeof(reg_before_madctl))) != ESP_OK) return e;
    if ((e = write_madctl(m)) != ESP_OK) return e;
    if ((e = exec_table(reg_after_madctl, sizeof(reg_after_madctl))) != ESP_OK) return e;
    if ((e = exec_table(wake_on, sizeof(wake_on))) != ESP_OK) return e;

    apply_geometry(rotation, m);
    s_ready = true;
    return ESP_OK;
}

esp_err_t zl_panel_set_rotation(int rotation, int madctl_override)
{
    if (!s_ready) return ESP_ERR_INVALID_STATE;
    if (rotation < 0 || rotation >= ZL_NUM_ROTATIONS || madctl_override > 0xFF) {
        return ESP_ERR_INVALID_ARG;
    }
    uint8_t m = madctl_override >= 0 ? (uint8_t)madctl_override
                                     : zl_madctl_table[rotation];
    esp_err_t e = write_madctl(m);
    if (e != ESP_OK) return e;
    apply_geometry(rotation, m);
    return ESP_OK;
}

bool     zl_panel_ready(void)    { return s_ready; }
int      zl_panel_rotation(void) { return s_rot; }
uint8_t  zl_panel_madctl(void)   { return s_madctl; }
uint16_t zl_panel_width(void)    { return s_w; }
uint16_t zl_panel_height(void)   { return s_h; }

/* ------------------------------------------------------------------------ */

void zl_panel_cmd(uint8_t cmd, const uint8_t *params, size_t n)
{
    zl_bus_write_cmd(cmd);
    if (params != NULL && n > 0) {
        zl_bus_write_data(params, n);
    }
}

void zl_panel_display(bool on) { zl_bus_write_cmd(on ? 0x29 : 0x28); }
void zl_panel_invert(bool on)  { zl_bus_write_cmd(on ? 0x21 : 0x20); }

bool zl_clip_rect(int *x, int *y, int *w, int *h)
{
    if (*x < 0) { *w += *x; *x = 0; }
    if (*y < 0) { *h += *y; *y = 0; }
    if (*x + *w > s_w) *w = (int)s_w - *x;
    if (*y + *h > s_h) *h = (int)s_h - *y;
    return (*w > 0 && *h > 0 && *x < s_w && *y < s_h);
}

/* CASET / PASET / RAMWR exactly as ili9341_lcdDrawFillRect(): 0x2A + addr,
 * 0x2B + addr, 0x2C, then pixel data with no further command. */
void zl_set_window(int x1, int y1, int x2, int y2)
{
    uint16_t ax1 = (uint16_t)(x1 + ZLCD_OFFSET_X), ax2 = (uint16_t)(x2 + ZLCD_OFFSET_X);
    uint16_t ay1 = (uint16_t)(y1 + ZLCD_OFFSET_Y), ay2 = (uint16_t)(y2 + ZLCD_OFFSET_Y);
    uint8_t a[4];

    zl_bus_write_cmd(0x2A);
    a[0] = ax1 >> 8; a[1] = ax1 & 0xFF; a[2] = ax2 >> 8; a[3] = ax2 & 0xFF;
    zl_bus_write_data(a, 4);

    zl_bus_write_cmd(0x2B);
    a[0] = ay1 >> 8; a[1] = ay1 & 0xFF; a[2] = ay2 >> 8; a[3] = ay2 & 0xFF;
    zl_bus_write_data(a, 4);

    zl_bus_write_cmd(0x2C);
}

void zl_fill_rect(int x, int y, int w, int h, uint16_t color)
{
    if (!zl_clip_rect(&x, &y, &w, &h)) return;
    zl_set_window(x, y, x + w - 1, y + h - 1);
    zl_bus_write_color(color, (uint32_t)w * (uint32_t)h);
}

void zl_fill_screen(uint16_t color)
{
    zl_fill_rect(0, 0, s_w, s_h, color);
}

void zl_draw_pixel(int x, int y, uint16_t color)
{
    if (x < 0 || y < 0 || x >= s_w || y >= s_h) return;
    zl_set_window(x, y, x, y);
    zl_bus_write_color(color, 1);
}

void zl_blit(int x, int y, int w, int h, const uint8_t *buf)
{
    zl_set_window(x, y, x + w - 1, y + h - 1);
    zl_bus_write_bytes(buf, (size_t)w * (size_t)h * 2u);
}

void zl_draw_line(int x0, int y0, int x1, int y1, uint16_t color)
{
    if (y0 == y1) {
        int x = x0 < x1 ? x0 : x1;
        zl_fill_rect(x, y0, (x0 < x1 ? x1 - x0 : x0 - x1) + 1, 1, color);
        return;
    }
    if (x0 == x1) {
        int y = y0 < y1 ? y0 : y1;
        zl_fill_rect(x0, y, 1, (y0 < y1 ? y1 - y0 : y0 - y1) + 1, color);
        return;
    }
    int dx = x1 > x0 ? x1 - x0 : x0 - x1;
    int sx = x0 < x1 ? 1 : -1;
    int dy = y1 > y0 ? -(y1 - y0) : -(y0 - y1);      /* always -|dy| */
    int sy = y0 < y1 ? 1 : -1;
    int err = dx + dy;
    int x = x0, y = y0;
    for (;;) {
        zl_draw_pixel(x, y, color);
        if (x == x1 && y == y1) break;
        int e2 = 2 * err;
        if (e2 >= dy) { err += dy; x += sx; }
        if (e2 <= dx) { err += dx; y += sy; }
    }
}

void zl_draw_rect(int x, int y, int w, int h, uint16_t color)
{
    if (w <= 0 || h <= 0) return;
    zl_fill_rect(x, y, w, 1, color);
    zl_fill_rect(x, y + h - 1, w, 1, color);
    zl_fill_rect(x, y, 1, h, color);
    zl_fill_rect(x + w - 1, y, 1, h, color);
}

void zl_draw_circle(int x0, int y0, int r, uint16_t color)
{
    if (r < 0) return;
    int f = 1 - r, ddF_x = 1, ddF_y = -2 * r, x = 0, y = r;
    zl_draw_pixel(x0, y0 + r, color);
    zl_draw_pixel(x0, y0 - r, color);
    zl_draw_pixel(x0 + r, y0, color);
    zl_draw_pixel(x0 - r, y0, color);
    while (x < y) {
        if (f >= 0) { y--; ddF_y += 2; f += ddF_y; }
        x++; ddF_x += 2; f += ddF_x;
        zl_draw_pixel(x0 + x, y0 + y, color);
        zl_draw_pixel(x0 - x, y0 + y, color);
        zl_draw_pixel(x0 + x, y0 - y, color);
        zl_draw_pixel(x0 - x, y0 - y, color);
        zl_draw_pixel(x0 + y, y0 + x, color);
        zl_draw_pixel(x0 - y, y0 + x, color);
        zl_draw_pixel(x0 + y, y0 - x, color);
        zl_draw_pixel(x0 - y, y0 - x, color);
    }
}

void zl_fill_circle(int x0, int y0, int r, uint16_t color)
{
    if (r < 0) return;
    zl_fill_rect(x0, y0 - r, 1, 2 * r + 1, color);
    int f = 1 - r, ddF_x = 1, ddF_y = -2 * r, x = 0, y = r;
    while (x < y) {
        if (f >= 0) { y--; ddF_y += 2; f += ddF_y; }
        x++; ddF_x += 2; f += ddF_x;
        zl_fill_rect(x0 + x, y0 - y, 1, 2 * y + 1, color);
        zl_fill_rect(x0 - x, y0 - y, 1, 2 * y + 1, color);
        zl_fill_rect(x0 + y, y0 - x, 1, 2 * x + 1, color);
        zl_fill_rect(x0 - y, y0 - x, 1, 2 * x + 1, color);
    }
}
