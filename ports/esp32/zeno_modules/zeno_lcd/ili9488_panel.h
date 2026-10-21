/*
 * ili9488_panel.h -- ILI9488 init, address window and drawing primitives.
 *
 * Port of Nopnop ili9488.c (init table) and ili9341.c (DrawPixel,
 * DrawMultiPixels, DrawFillRect, display on/off, inversion) plus the
 * lcd_lib.c line helpers, on top of lcd_bus.h.
 *
 * Locking: zl_panel_init() and zl_panel_set_rotation() take the bus lock
 * themselves (they sleep between commands) -- call them WITHOUT the lock.
 * Every other function marked [LOCK] must be called WITH the lock held.
 */
#ifndef ZENO_ILI9488_PANEL_H
#define ZENO_ILI9488_PANEL_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "esp_err.h"

/* The panel layer must sleep (reset pulse, 150 ms waits). The MicroPython
 * binding installs mp_hal_delay_ms so other Python threads keep running. */
void zl_panel_set_delay_fn(void (*fn)(uint32_t ms));

/* HW reset pulse on RST (high/low/high + 150 ms settle). Marks the panel
 * not-ready: call zl_panel_init() (or lcd.init()) afterwards. */
void zl_panel_hw_reset(void);

/* Full bring-up: HW reset + the exact Nopnop ILI9488 table. rotation 0..3;
 * madctl_override < 0 means "use table value". Needs zl_bus_init() first. */
esp_err_t zl_panel_init(int rotation, int madctl_override);
esp_err_t zl_panel_set_rotation(int rotation, int madctl_override);

bool     zl_panel_ready(void);
int      zl_panel_rotation(void);
uint8_t  zl_panel_madctl(void);
uint16_t zl_panel_width(void);
uint16_t zl_panel_height(void);

/* ---- [LOCK] ---- */
void zl_panel_cmd(uint8_t cmd, const uint8_t *params, size_t n);
void zl_panel_display(bool on);
void zl_panel_invert(bool on);

bool zl_clip_rect(int *x, int *y, int *w, int *h);
void zl_set_window(int x1, int y1, int x2, int y2);          /* inclusive, pre-clipped */
void zl_fill_rect(int x, int y, int w, int h, uint16_t color);   /* clips silently */
void zl_fill_screen(uint16_t color);
void zl_draw_pixel(int x, int y, uint16_t color);                /* clips silently */
void zl_blit(int x, int y, int w, int h, const uint8_t *buf);    /* caller checked bounds */
void zl_draw_line(int x0, int y0, int x1, int y1, uint16_t color);
void zl_draw_rect(int x, int y, int w, int h, uint16_t color);
void zl_draw_circle(int x0, int y0, int r, uint16_t color);
void zl_fill_circle(int x0, int y0, int r, uint16_t color);

#endif
