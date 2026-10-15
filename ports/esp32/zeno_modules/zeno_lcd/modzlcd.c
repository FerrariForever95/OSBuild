#include "vendor/lcd_config.h"
/*
 * lcd - MicroPython binding for the nopnop2002/esp-idf-parallel-tft core (MIT)
 * ILI9488 320x480, 8-bit parallel (REGISTER I/O), shared-pin 4-wire resistive touch.
 *
 * NOT compiled or run by the author of this file: first build may need small fixes.
 * All MicroPython calls run under the GIL, so no extra locking is needed for threads;
 * do not call any function from an IRQ handler.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "py/runtime.h"
#include "py/obj.h"
#include "py/objstr.h"
#include "py/mperrno.h"

#include "driver/gpio.h"
#include "esp_err.h"
#include "esp_rom_sys.h"
#include "esp_spiffs.h"

#include "vendor/lcd_com.h"
#include "vendor/fontx.h"
#include "vendor/lcd_lib.h"
#include "vendor/ili9488.h"
#include "vendor/pngle.h"
#include "vendor/decode_png.h"
#include "vendor/decode_jpeg.h"

static TFT_t dev;
static bool g_ready = false;
static bool g_touch_ready = false;
static int g_w = 320, g_h = 480;

#define FONT_SLOTS 4
static FontxFile g_fonts[FONT_SLOTS][2];
static char g_font_path[FONT_SLOTS][48];
static bool g_font_used[FONT_SLOTS];

static char g_label[17] = "assets";
static char g_base[16] = "/assets";
static bool g_mounted = false;

static void need_ready(void) {
    if (!g_ready) {
        mp_raise_msg(&mp_type_RuntimeError, MP_ERROR_TEXT("call lcd.init() first"));
    }
}

static void raise_esp(const char *what, esp_err_t err) {
    mp_raise_msg_varg(&mp_type_OSError, MP_ERROR_TEXT("%s: %s"), what, esp_err_to_name(err));
}

static int clampi(int v, int lo, int hi) {
    return v < lo ? lo : (v > hi ? hi : v);
}

/* ------------------------------------------------------------------ display */

static mp_obj_t mplcd_init(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { ARG_width, ARG_height, ARG_invert };
    static const mp_arg_t allowed[] = {
        { MP_QSTR_width,  MP_ARG_INT,  { .u_int = 320 } },
        { MP_QSTR_height, MP_ARG_INT,  { .u_int = 480 } },
        { MP_QSTR_invert, MP_ARG_BOOL, { .u_bool = false } },
    };
    mp_arg_val_t a[MP_ARRAY_SIZE(allowed)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed), allowed, a);

    if (g_ready) {
        mp_raise_msg(&mp_type_RuntimeError, MP_ERROR_TEXT("already initialised"));
    }

    g_w = a[ARG_width].u_int;
    g_h = a[ARG_height].u_int;
    if (g_w < 1 || g_w > 320 || g_h < 1 || g_h > 480) {
        mp_raise_ValueError(MP_ERROR_TEXT("ILI9488 panel is at most 320x480"));
    }

    memset(&dev, 0, sizeof(dev));
    esp_err_t err = lcd_interface_cfg(&dev);
    if (err != ESP_OK) {
        raise_esp("lcd_interface_cfg", err);
    }
    ili9488_lcdInit(&dev, g_w, g_h, 0, 0);
    if (a[ARG_invert].u_bool) {
        lcdInversionOn(&dev);
    }
    lcdFillScreen(&dev, 0x0000);
    g_ready = true;
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_KW(mplcd_init_obj, 0, mplcd_init);

static mp_obj_t mplcd_display(mp_obj_t on) {
    need_ready();
    if (mp_obj_is_true(on)) lcdDisplayOn(&dev); else lcdDisplayOff(&dev);
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_1(mplcd_display_obj, mplcd_display);

static mp_obj_t mplcd_invert(mp_obj_t on) {
    need_ready();
    if (mp_obj_is_true(on)) lcdInversionOn(&dev); else lcdInversionOff(&dev);
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_1(mplcd_invert_obj, mplcd_invert);

static mp_obj_t mplcd_size(void) {
    mp_obj_t t[2] = { mp_obj_new_int(g_w), mp_obj_new_int(g_h) };
    return mp_obj_new_tuple(2, t);
}
static MP_DEFINE_CONST_FUN_OBJ_0(mplcd_size_obj, mplcd_size);

static mp_obj_t mplcd_color565(mp_obj_t r, mp_obj_t g, mp_obj_t b) {
    return mp_obj_new_int(rgb565_conv(mp_obj_get_int(r) & 0xFF, mp_obj_get_int(g) & 0xFF, mp_obj_get_int(b) & 0xFF));
}
static MP_DEFINE_CONST_FUN_OBJ_3(mplcd_color565_obj, mplcd_color565);

static mp_obj_t mplcd_fill(mp_obj_t color) {
    need_ready();
    lcdFillScreen(&dev, (uint16_t)mp_obj_get_int(color));
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_1(mplcd_fill_obj, mplcd_fill);

static mp_obj_t mplcd_fill_rect(size_t n_args, const mp_obj_t *args) {
    need_ready();
    int x = mp_obj_get_int(args[0]), y = mp_obj_get_int(args[1]);
    int w = mp_obj_get_int(args[2]), h = mp_obj_get_int(args[3]);
    uint16_t c = (uint16_t)mp_obj_get_int(args[4]);
    if (w <= 0 || h <= 0) return mp_const_none;
    int x1 = clampi(x, 0, g_w - 1), y1 = clampi(y, 0, g_h - 1);
    int x2 = clampi(x + w - 1, 0, g_w - 1), y2 = clampi(y + h - 1, 0, g_h - 1);
    if (x + w - 1 < 0 || y + h - 1 < 0 || x > g_w - 1 || y > g_h - 1) return mp_const_none;
    lcdDrawFillRect(&dev, x1, y1, x2, y2, c);
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(mplcd_fill_rect_obj, 5, 5, mplcd_fill_rect);

static mp_obj_t mplcd_pixel(mp_obj_t xo, mp_obj_t yo, mp_obj_t co) {
    need_ready();
    int x = mp_obj_get_int(xo), y = mp_obj_get_int(yo);
    if (x >= 0 && y >= 0 && x < g_w && y < g_h) {
        lcdDrawPixel(&dev, x, y, (uint16_t)mp_obj_get_int(co));
    }
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_3(mplcd_pixel_obj, mplcd_pixel);

/* blit(x, y, w, h, buf): buf = native-endian RGB565, w*h*2 bytes (same layout as framebuf.RGB565) */
static mp_obj_t mplcd_blit(size_t n_args, const mp_obj_t *args) {
    need_ready();
    int x = mp_obj_get_int(args[0]), y = mp_obj_get_int(args[1]);
    int w = mp_obj_get_int(args[2]), h = mp_obj_get_int(args[3]);
    mp_buffer_info_t bi;
    mp_get_buffer_raise(args[4], &bi, MP_BUFFER_READ);
    if (w <= 0 || h <= 0) return mp_const_none;
    if (bi.len < (size_t)w * (size_t)h * 2) {
        mp_raise_ValueError(MP_ERROR_TEXT("buffer too small"));
    }
    if (((uintptr_t)bi.buf) & 1) {
        mp_raise_ValueError(MP_ERROR_TEXT("buffer must be 2-byte aligned"));
    }
    int cx0 = x < 0 ? -x : 0;
    int cw = w - cx0;
    if (x + cx0 + cw > g_w) cw = g_w - (x + cx0);
    if (cw <= 0) return mp_const_none;
    for (int row = 0; row < h; row++) {
        int sy = y + row;
        if (sy < 0 || sy >= g_h) continue;
        uint16_t *p = (uint16_t *)((uint8_t *)bi.buf + (size_t)row * (size_t)w * 2) + cx0;
        lcdDrawMultiPixels(&dev, x + cx0, sy, cw, p);
    }
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(mplcd_blit_obj, 5, 5, mplcd_blit);

/* ------------------------------------------------- assets partition (SPIFFS) */

static mp_obj_t mplcd_mount(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { ARG_label, ARG_base, ARG_max_files };
    static const mp_arg_t allowed[] = {
        { MP_QSTR_label,     MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_base,      MP_ARG_OBJ, { .u_obj = MP_OBJ_NULL } },
        { MP_QSTR_max_files, MP_ARG_INT, { .u_int = 8 } },
    };
    mp_arg_val_t a[MP_ARRAY_SIZE(allowed)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed), allowed, a);

    if (a[ARG_label].u_obj != MP_OBJ_NULL) {
        const char *s = mp_obj_str_get_str(a[ARG_label].u_obj);
        if (strlen(s) >= sizeof(g_label)) mp_raise_ValueError(MP_ERROR_TEXT("label too long"));
        strcpy(g_label, s);
    }
    if (a[ARG_base].u_obj != MP_OBJ_NULL) {
        const char *s = mp_obj_str_get_str(a[ARG_base].u_obj);
        if (strlen(s) >= sizeof(g_base)) mp_raise_ValueError(MP_ERROR_TEXT("base path too long"));
        strcpy(g_base, s);
    }
    if (!g_mounted) {
        esp_vfs_spiffs_conf_t conf = {
            .base_path = g_base,
            .partition_label = g_label,
            .max_files = a[ARG_max_files].u_int,
            .format_if_mount_failed = false,   /* never silently erase the partition */
        };
        esp_err_t err = esp_vfs_spiffs_register(&conf);
        if (err != ESP_OK) {
            raise_esp("spiffs mount", err);
        }
        g_mounted = true;
    }
    size_t total = 0, used = 0;
    esp_err_t err = esp_spiffs_info(g_label, &total, &used);
    if (err != ESP_OK) {
        raise_esp("spiffs info", err);
    }
    mp_obj_t t[2] = { mp_obj_new_int_from_uint(total), mp_obj_new_int_from_uint(used) };
    return mp_obj_new_tuple(2, t);
}
static MP_DEFINE_CONST_FUN_OBJ_KW(mplcd_mount_obj, 0, mplcd_mount);

static mp_obj_t mplcd_unmount(void) {
    if (g_mounted) {
        for (int i = 0; i < FONT_SLOTS; i++) {
            if (g_font_used[i]) {
                CloseFontx(&g_fonts[i][0]);
                g_font_used[i] = false;
            }
        }
        esp_vfs_spiffs_unregister(g_label);
        g_mounted = false;
    }
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_0(mplcd_unmount_obj, mplcd_unmount);

/* -------------------------------------------------------------------- fonts */

static int get_slot(mp_obj_t o) {
    int s = mp_obj_get_int(o);
    if (s < 0 || s >= FONT_SLOTS) {
        mp_raise_ValueError(MP_ERROR_TEXT("font slot must be 0..3"));
    }
    return s;
}

/* font_load(slot, path) -> (glyph_w, glyph_h).  path e.g. "/assets/ILGH16XB.FNT" (FONTX2 file) */
static mp_obj_t mplcd_font_load(mp_obj_t slot_o, mp_obj_t path_o) {
    int slot = get_slot(slot_o);
    const char *p = mp_obj_str_get_str(path_o);
    if (strlen(p) >= sizeof(g_font_path[0])) {
        mp_raise_ValueError(MP_ERROR_TEXT("font path too long"));
    }
    if (g_font_used[slot]) {
        CloseFontx(&g_fonts[slot][0]);
        g_font_used[slot] = false;
    }
    strcpy(g_font_path[slot], p);
    InitFontx(g_fonts[slot], g_font_path[slot], "");
    if (!OpenFontx(&g_fonts[slot][0])) {
        mp_raise_OSError(MP_ENOENT);
    }
    g_font_used[slot] = true;
    mp_obj_t t[2] = { mp_obj_new_int(getFortWidth(&g_fonts[slot][0])),
                      mp_obj_new_int(getFortHeight(&g_fonts[slot][0])) };
    return mp_obj_new_tuple(2, t);
}
static MP_DEFINE_CONST_FUN_OBJ_2(mplcd_font_load_obj, mplcd_font_load);

/* text(slot, x, y, str, color, direction=0) -> next x.  direction 0..3 = 0/90/180/270 deg (repo semantics) */
static mp_obj_t mplcd_text(size_t n_args, const mp_obj_t *args) {
    need_ready();
    int slot = get_slot(args[0]);
    if (!g_font_used[slot]) {
        mp_raise_msg(&mp_type_RuntimeError, MP_ERROR_TEXT("font slot empty: call font_load()"));
    }
    int x = mp_obj_get_int(args[1]), y = mp_obj_get_int(args[2]);
    const char *s = mp_obj_str_get_str(args[3]);
    uint16_t color = (uint16_t)mp_obj_get_int(args[4]);
    int dir = (n_args > 5) ? mp_obj_get_int(args[5]) : 0;
    if (dir < 0 || dir > 3) {
        mp_raise_ValueError(MP_ERROR_TEXT("direction must be 0..3"));
    }
    lcdSetFontDirection(&dev, dir);
    int nx = lcdDrawString(&dev, g_fonts[slot], x, y, (uint8_t *)(uintptr_t)s, color);
    return mp_obj_new_int(nx);
}
static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(mplcd_text_obj, 5, 6, mplcd_text);

/* font_bg(color) fills glyph background; font_bg(None) makes it transparent */
static mp_obj_t mplcd_font_bg(mp_obj_t c) {
    need_ready();
    if (c == mp_const_none) lcdUnsetFontFill(&dev);
    else lcdSetFontFill(&dev, (uint16_t)mp_obj_get_int(c));
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_1(mplcd_font_bg_obj, mplcd_font_bg);

/* --------------------------------------------------------------- PNG / JPEG */

static void draw_rows(int x0, int y0, int iw, int ih, uint16_t **rows_png, pixel_jpeg **rows_jpg) {
    int cx0 = x0 < 0 ? -x0 : 0;
    int cw = iw - cx0;
    if (x0 + cx0 + cw > g_w) cw = g_w - (x0 + cx0);
    if (cw <= 0) return;
    uint16_t *line = (uint16_t *)malloc(sizeof(uint16_t) * cw);
    if (line == NULL) {
        mp_raise_msg(&mp_type_MemoryError, MP_ERROR_TEXT("image row buffer"));
    }
    for (int row = 0; row < ih; row++) {
        int sy = y0 + row;
        if (sy < 0 || sy >= g_h) continue;
        for (int c = 0; c < cw; c++) {
            line[c] = rows_png ? rows_png[row][cx0 + c] : rows_jpg[row][cx0 + c];
        }
        lcdDrawMultiPixels(&dev, x0 + cx0, sy, cw, line);
    }
    free(line);
}

static void place(size_t n_args, const mp_obj_t *args, int iw, int ih, int *x0, int *y0) {
    *x0 = (n_args > 1 && args[1] != mp_const_none) ? mp_obj_get_int(args[1]) : (g_w - iw) / 2;
    *y0 = (n_args > 2 && args[2] != mp_const_none) ? mp_obj_get_int(args[2]) : (g_h - ih) / 2;
}

/* png(path, x=None, y=None): top-left position; None = centered. Large images are shrunk to fit. */
static mp_obj_t mplcd_png(size_t n_args, const mp_obj_t *args) {
    need_ready();
    const char *path = mp_obj_str_get_str(args[0]);
    FILE *fp = fopen(path, "rb");
    if (fp == NULL) {
        mp_raise_OSError(MP_ENOENT);
    }
    pngle_t *pngle = pngle_new(g_w, g_h);
    if (pngle == NULL) {
        fclose(fp);
        mp_raise_msg(&mp_type_MemoryError, MP_ERROR_TEXT("png: out of memory"));
    }
    pngle_set_init_callback(pngle, png_init);
    pngle_set_draw_callback(pngle, png_draw);
    pngle_set_done_callback(pngle, png_finish);
    pngle_set_display_gamma(pngle, 2.2);

    char buf[1024];
    char emsg[64] = "";
    size_t remain = 0;
    bool failed = false;
    while (!feof(fp)) {
        if (remain >= sizeof(buf)) {
            strcpy(emsg, "buffer exceeded");
            failed = true;
            break;
        }
        int len = fread(buf + remain, 1, sizeof(buf) - remain, fp);
        if (len <= 0) break;
        int fed = pngle_feed(pngle, buf, remain + len);
        if (fed < 0) {
            strncpy(emsg, pngle_error(pngle), sizeof(emsg) - 1);
            emsg[sizeof(emsg) - 1] = 0;
            failed = true;
            break;
        }
        remain = remain + len - fed;
        if (remain > 0) memmove(buf, buf + fed, remain);
    }
    fclose(fp);
    if (failed) {
        pngle_destroy(pngle, g_w, g_h);
        mp_raise_msg_varg(&mp_type_OSError, MP_ERROR_TEXT("png: %s"), emsg);
    }

    int pw = pngle_get_width(pngle), ph = pngle_get_height(pngle);
    int x0, y0;
    place(n_args, args, pw, ph, &x0, &y0);
    draw_rows(x0, y0, pw, ph, (uint16_t **)pngle->pixels, NULL);
    pngle_destroy(pngle, g_w, g_h);
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(mplcd_png_obj, 1, 3, mplcd_png);

/* jpeg(path, x=None, y=None): baseline JPEG only (no progressive). Shrunk by 1/2,1/4,1/8 to fit. */
static mp_obj_t mplcd_jpeg(size_t n_args, const mp_obj_t *args) {
    need_ready();
    const char *path = mp_obj_str_get_str(args[0]);
    pixel_jpeg **pixels = NULL;
    int iw = 0, ih = 0;
    esp_err_t err = decode_jpeg(&pixels, (char *)(uintptr_t)path, g_w, g_h, &iw, &ih);
    if (err != ESP_OK) {
        raise_esp("jpeg decode", err);
    }
    int x0, y0;
    place(n_args, args, iw, ih, &x0, &y0);
    draw_rows(x0, y0, iw, ih, NULL, pixels);
    release_image(&pixels, g_w, g_h);
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(mplcd_jpeg_obj, 1, 3, mplcd_jpeg);

/* -------------------------------------------------------------------- touch */

typedef struct { int x, y, z; } raw_t;

/* Tunable from Python via touch_config().  Defaults scale the original 10-bit tolerance (2) to the S3's 12 bits (8). */
static struct {
    int samples;      /* ADC reads averaged per sample (>=3; min and max dropped) */
    int tolerance;    /* max difference between the two averaged samples of an axis */
    int settle_us;    /* delay after switching pin directions, before reading */
    int rail_lo;      /* reject per-sample ADC value <= this (floating / shorted) */
    int rail_hi;      /* reject per-sample ADC value >= this (saturated) */
    int z_min, z_max; /* accepted pressure range (z = |Y+ - X-| as in the original code; 0..4095 = off) */
    int jump;         /* max accepted jump between consecutive points, raw units; bigger = needs 2 agreeing reads */
    int confirm;      /* consecutive good reads before a new touch is reported */
    int release;      /* consecutive bad reads before the touch is considered released */
    int edge_px;      /* how far outside the screen (px) a mapped point may land before it is rejected */
} tc = { 4, 8, 20, 16, 4079, 0, 4095, 700, 2, 2, 12 };

static struct {
    bool set, swap;
    int xr0, xp0, xr1, xp1, yr0, yp0, yr1, yp1;
} cal;

static struct {
    bool have_pos, cand_valid;
    int valid_cnt, invalid_cnt;
    int last_x, last_y, cand_x, cand_y;
} ts;

static void touch_state_reset(void) {
    memset(&ts, 0, sizeof(ts));
}

static void need_touch(void) {
    need_ready();
    if (!g_touch_ready) {
        mp_raise_msg(&mp_type_RuntimeError, MP_ERROR_TEXT("call touch_init() first"));
    }
}

static int axis_measure(bool xaxis) {
    int chan;
    if (xaxis) {
        touch_gpio(dev._gpio_yp, MODE_INPUT, 0);
        touch_gpio(dev._gpio_ym, MODE_INPUT, 0);
        touch_gpio(dev._gpio_xp, MODE_OUTPUT, 1);
        touch_gpio(dev._gpio_xm, MODE_OUTPUT, 0);
        chan = dev._adc_yp;
    } else {
        touch_gpio(dev._gpio_yp, MODE_OUTPUT, 1);
        touch_gpio(dev._gpio_ym, MODE_OUTPUT, 0);
        touch_gpio(dev._gpio_xp, MODE_INPUT, 0);
        touch_gpio(dev._gpio_xm, MODE_INPUT, 0);
        chan = dev._adc_xm;
    }
    if (tc.settle_us > 0) esp_rom_delay_us(tc.settle_us);
    int s0 = touch_avr_analog(dev._adc1_handle, chan, tc.samples);
    int s1 = touch_avr_analog(dev._adc1_handle, chan, tc.samples);
    if (abs(s0 - s1) > tc.tolerance) return -1;
    return s0 + s1;
}

static int z_measure(void) {
    touch_gpio(dev._gpio_yp, MODE_INPUT, 0);
    touch_gpio(dev._gpio_ym, MODE_OUTPUT, 1);
    touch_gpio(dev._gpio_xp, MODE_OUTPUT, 0);
    touch_gpio(dev._gpio_xm, MODE_INPUT, 0);
    if (tc.settle_us > 0) esp_rom_delay_us(tc.settle_us);
    int z1 = 0, z2 = 0;
    if (adc_oneshot_read(dev._adc1_handle, dev._adc_yp, &z1) != ESP_OK) z1 = 0;
    if (adc_oneshot_read(dev._adc1_handle, dev._adc_xm, &z2) != ESP_OK) z2 = 0;
    return abs(z1 - z2);
}

/* Give the bus back to the LCD: all four shared lines are outputs, HIGH (CS deselected, RS idle). */
static void touch_release_bus(void) {
    touch_gpio(dev._gpio_yp, MODE_OUTPUT, 1);
    touch_gpio(dev._gpio_ym, MODE_OUTPUT, 1);
    touch_gpio(dev._gpio_xp, MODE_OUTPUT, 1);
    touch_gpio(dev._gpio_xm, MODE_OUTPUT, 1);
}

static void measure_raw(raw_t *r) {
    r->x = axis_measure(true);
    r->y = axis_measure(false);
    r->z = z_measure();
    touch_release_bus();
}

static bool raw_valid(const raw_t *r) {
    if (r->x < 0 || r->y < 0) return false;
    if (r->x <= 2 * tc.rail_lo || r->x >= 2 * tc.rail_hi) return false;
    if (r->y <= 2 * tc.rail_lo || r->y >= 2 * tc.rail_hi) return false;
    if (r->z < tc.z_min || r->z > tc.z_max) return false;
    return true;
}

static int dist(int ax, int ay, int bx, int by) {
    int dx = abs(ax - bx), dy = abs(ay - by);
    return dx > dy ? dx : dy;
}

static bool map_axis(int raw, int r0, int p0, int r1, int p1, int size, int *out) {
    if (r1 == r0) return false;
    long v = p0 + (long)(raw - r0) * (long)(p1 - p0) / (long)(r1 - r0);
    if (v < -tc.edge_px || v > (long)(size - 1) + tc.edge_px) return false;
    *out = clampi((int)v, 0, size - 1);
    return true;
}

/* touch_init()   - electrode pins and ADC1 channels come from lcd_pins.h:
 *   XP = D0, YM = D1, XM = RS, YP = CS;  ADC channel (ESP32-S3) = GPIO - 1 of YP (CS) and XM (RS).
 * Optional keyword overrides: adc_yp=, adc_xm=, xp=, xm=, yp=, ym= */
static mp_obj_t mplcd_touch_init(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { ARG_adc_yp, ARG_adc_xm, ARG_xp, ARG_xm, ARG_yp, ARG_ym };
    static const mp_arg_t allowed[] = {
        { MP_QSTR_adc_yp, MP_ARG_KW_ONLY | MP_ARG_INT, { .u_int = -1 } },
        { MP_QSTR_adc_xm, MP_ARG_KW_ONLY | MP_ARG_INT, { .u_int = -1 } },
        { MP_QSTR_xp,     MP_ARG_KW_ONLY | MP_ARG_INT, { .u_int = -1 } },
        { MP_QSTR_xm,     MP_ARG_KW_ONLY | MP_ARG_INT, { .u_int = -1 } },
        { MP_QSTR_yp,     MP_ARG_KW_ONLY | MP_ARG_INT, { .u_int = -1 } },
        { MP_QSTR_ym,     MP_ARG_KW_ONLY | MP_ARG_INT, { .u_int = -1 } },
    };
    mp_arg_val_t a[MP_ARRAY_SIZE(allowed)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed), allowed, a);
    need_ready();
    if (g_touch_ready) {
        mp_raise_msg(&mp_type_RuntimeError, MP_ERROR_TEXT("touch already initialised"));
    }
    int xp = a[ARG_xp].u_int >= 0 ? a[ARG_xp].u_int : LCD_PIN_D0;
    int xm = a[ARG_xm].u_int >= 0 ? a[ARG_xm].u_int : LCD_PIN_RS;
    int yp = a[ARG_yp].u_int >= 0 ? a[ARG_yp].u_int : LCD_PIN_CS;
    int ym = a[ARG_ym].u_int >= 0 ? a[ARG_ym].u_int : LCD_PIN_D1;
    int adc_yp = a[ARG_adc_yp].u_int >= 0 ? a[ARG_adc_yp].u_int : yp - 1;
    int adc_xm = a[ARG_adc_xm].u_int >= 0 ? a[ARG_adc_xm].u_int : xm - 1;
    if (adc_yp < 0 || adc_yp > 9 || adc_xm < 0 || adc_xm > 9) {
        mp_raise_ValueError(MP_ERROR_TEXT("ADC1 channel must be 0..9 (GPIO1..10): CS/RS must be on ADC1"));
    }
    touch_interface_cfg(&dev, adc_yp, adc_xm, xp, xm, yp, ym);   /* aborts if ADC1 is already claimed */
    touch_release_bus();
    touch_state_reset();
    g_touch_ready = true;
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_KW(mplcd_touch_init_obj, 0, mplcd_touch_init);

static mp_obj_t tc_dict(void) {
    mp_obj_t d = mp_obj_new_dict(11);
    mp_obj_dict_store(d, MP_OBJ_NEW_QSTR(MP_QSTR_samples),   mp_obj_new_int(tc.samples));
    mp_obj_dict_store(d, MP_OBJ_NEW_QSTR(MP_QSTR_tolerance), mp_obj_new_int(tc.tolerance));
    mp_obj_dict_store(d, MP_OBJ_NEW_QSTR(MP_QSTR_settle_us), mp_obj_new_int(tc.settle_us));
    mp_obj_dict_store(d, MP_OBJ_NEW_QSTR(MP_QSTR_rail_lo),   mp_obj_new_int(tc.rail_lo));
    mp_obj_dict_store(d, MP_OBJ_NEW_QSTR(MP_QSTR_rail_hi),   mp_obj_new_int(tc.rail_hi));
    mp_obj_dict_store(d, MP_OBJ_NEW_QSTR(MP_QSTR_z_min),     mp_obj_new_int(tc.z_min));
    mp_obj_dict_store(d, MP_OBJ_NEW_QSTR(MP_QSTR_z_max),     mp_obj_new_int(tc.z_max));
    mp_obj_dict_store(d, MP_OBJ_NEW_QSTR(MP_QSTR_jump),      mp_obj_new_int(tc.jump));
    mp_obj_dict_store(d, MP_OBJ_NEW_QSTR(MP_QSTR_confirm),   mp_obj_new_int(tc.confirm));
    mp_obj_dict_store(d, MP_OBJ_NEW_QSTR(MP_QSTR_release),   mp_obj_new_int(tc.release));
    mp_obj_dict_store(d, MP_OBJ_NEW_QSTR(MP_QSTR_edge_px),   mp_obj_new_int(tc.edge_px));
    return d;
}

/* touch_config(**kw) sets any of the tuning variables; always returns the current values as a dict. */
static mp_obj_t mplcd_touch_config(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { A_samples, A_tolerance, A_settle_us, A_rail_lo, A_rail_hi, A_z_min, A_z_max, A_jump, A_confirm, A_release, A_edge_px };
    static const mp_arg_t allowed[] = {
        { MP_QSTR_samples,   MP_ARG_KW_ONLY | MP_ARG_OBJ, { .u_obj = mp_const_none } },
        { MP_QSTR_tolerance, MP_ARG_KW_ONLY | MP_ARG_OBJ, { .u_obj = mp_const_none } },
        { MP_QSTR_settle_us, MP_ARG_KW_ONLY | MP_ARG_OBJ, { .u_obj = mp_const_none } },
        { MP_QSTR_rail_lo,   MP_ARG_KW_ONLY | MP_ARG_OBJ, { .u_obj = mp_const_none } },
        { MP_QSTR_rail_hi,   MP_ARG_KW_ONLY | MP_ARG_OBJ, { .u_obj = mp_const_none } },
        { MP_QSTR_z_min,     MP_ARG_KW_ONLY | MP_ARG_OBJ, { .u_obj = mp_const_none } },
        { MP_QSTR_z_max,     MP_ARG_KW_ONLY | MP_ARG_OBJ, { .u_obj = mp_const_none } },
        { MP_QSTR_jump,      MP_ARG_KW_ONLY | MP_ARG_OBJ, { .u_obj = mp_const_none } },
        { MP_QSTR_confirm,   MP_ARG_KW_ONLY | MP_ARG_OBJ, { .u_obj = mp_const_none } },
        { MP_QSTR_release,   MP_ARG_KW_ONLY | MP_ARG_OBJ, { .u_obj = mp_const_none } },
        { MP_QSTR_edge_px,   MP_ARG_KW_ONLY | MP_ARG_OBJ, { .u_obj = mp_const_none } },
    };
    mp_arg_val_t a[MP_ARRAY_SIZE(allowed)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed), allowed, a);

    int *dst[] = { &tc.samples, &tc.tolerance, &tc.settle_us, &tc.rail_lo, &tc.rail_hi,
                   &tc.z_min, &tc.z_max, &tc.jump, &tc.confirm, &tc.release, &tc.edge_px };
    for (size_t i = 0; i < MP_ARRAY_SIZE(allowed); i++) {
        if (a[i].u_obj != mp_const_none) {
            *dst[i] = mp_obj_get_int(a[i].u_obj);
        }
    }
    if (tc.samples < 3) tc.samples = 3;     /* touch_avr_analog needs >2 samples */
    if (tc.tolerance < 0) tc.tolerance = 0;
    if (tc.settle_us < 0) tc.settle_us = 0;
    if (tc.confirm < 1) tc.confirm = 1;
    if (tc.release < 1) tc.release = 1;
    if (tc.edge_px < 0) tc.edge_px = 0;
    touch_state_reset();
    return tc_dict();
}
static MP_DEFINE_CONST_FUN_OBJ_KW(mplcd_touch_config_obj, 0, mplcd_touch_config);

/* touch_calibration()                                  -> current tuple
 * touch_calibration(xr0,xp0, xr1,xp1, yr0,yp0, yr1,yp1 [, swap])
 *   two (raw, pixel) reference points per axis; maps linearly and extrapolates to the full screen.
 *   raw values are the numbers read_raw() returns. With swap=True the "x" pair is applied to raw Y and vice versa. */
static mp_obj_t mplcd_touch_calibration(size_t n_args, const mp_obj_t *args) {
    if (n_args == 0) {
        if (!cal.set) return mp_const_none;
        mp_obj_t t[9] = { mp_obj_new_int(cal.xr0), mp_obj_new_int(cal.xp0), mp_obj_new_int(cal.xr1), mp_obj_new_int(cal.xp1),
                          mp_obj_new_int(cal.yr0), mp_obj_new_int(cal.yp0), mp_obj_new_int(cal.yr1), mp_obj_new_int(cal.yp1),
                          mp_obj_new_bool(cal.swap) };
        return mp_obj_new_tuple(9, t);
    }
    if (n_args < 8) {
        mp_raise_TypeError(MP_ERROR_TEXT("need 8 numbers (+ optional swap)"));
    }
    int v[8];
    for (int i = 0; i < 8; i++) v[i] = mp_obj_get_int(args[i]);
    if (v[0] == v[2] || v[4] == v[6]) {
        mp_raise_ValueError(MP_ERROR_TEXT("the two raw reference values of an axis must differ"));
    }
    cal.xr0 = v[0]; cal.xp0 = v[1]; cal.xr1 = v[2]; cal.xp1 = v[3];
    cal.yr0 = v[4]; cal.yp0 = v[5]; cal.yr1 = v[6]; cal.yp1 = v[7];
    cal.swap = (n_args > 8) ? mp_obj_is_true(args[8]) : false;
    cal.set = true;
    touch_state_reset();
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(mplcd_touch_calibration_obj, 0, 9, mplcd_touch_calibration);

/* read_raw() -> (x, y, z) unfiltered; x/y are -1 when the two samples disagreed by more than `tolerance`.
 * x and y are sums of two ADC reads (0..8190); z = |Y+ - X-| (bigger = harder press). Use this for calibration/tuning. */
static mp_obj_t mplcd_read_raw(void) {
    need_touch();
    raw_t r;
    measure_raw(&r);
    mp_obj_t t[3] = { mp_obj_new_int(r.x), mp_obj_new_int(r.y), mp_obj_new_int(r.z) };
    return mp_obj_new_tuple(3, t);
}
static MP_DEFINE_CONST_FUN_OBJ_0(mplcd_read_raw_obj, mplcd_read_raw);

/* read_touch() -> (x, y) in screen pixels, or None when nothing (valid) is pressed. Call at ~50-200 Hz. */
static mp_obj_t mplcd_read_touch(void) {
    need_touch();
    if (!cal.set) {
        mp_raise_msg(&mp_type_RuntimeError, MP_ERROR_TEXT("not calibrated: use read_raw() then touch_calibration()"));
    }
    raw_t r;
    measure_raw(&r);

    if (!raw_valid(&r)) {
        if (++ts.invalid_cnt >= tc.release) {
            touch_state_reset();
        }
        return mp_const_none;
    }
    ts.invalid_cnt = 0;

    if (!ts.have_pos) {
        /* new touch: need `confirm` consecutive reads that agree with each other */
        if (ts.valid_cnt > 0 && dist(r.x, r.y, ts.last_x, ts.last_y) <= tc.jump) {
            ts.valid_cnt++;
        } else {
            ts.valid_cnt = 1;
        }
        ts.last_x = r.x;
        ts.last_y = r.y;
        if (ts.valid_cnt < tc.confirm) return mp_const_none;
        ts.have_pos = true;
    } else if (dist(r.x, r.y, ts.last_x, ts.last_y) > tc.jump) {
        /* sudden jump (glitch or very fast move): accept only if the next read agrees with it */
        if (ts.cand_valid && dist(r.x, r.y, ts.cand_x, ts.cand_y) <= tc.jump) {
            ts.cand_valid = false;
        } else {
            ts.cand_x = r.x;
            ts.cand_y = r.y;
            ts.cand_valid = true;
            return mp_const_none;
        }
    } else {
        ts.cand_valid = false;
    }
    ts.last_x = r.x;
    ts.last_y = r.y;

    int ax = cal.swap ? r.y : r.x;
    int ay = cal.swap ? r.x : r.y;
    int sx, sy;
    if (!map_axis(ax, cal.xr0, cal.xp0, cal.xr1, cal.xp1, g_w, &sx) ||
        !map_axis(ay, cal.yr0, cal.yp0, cal.yr1, cal.yp1, g_h, &sy)) {
        return mp_const_none;
    }
    mp_obj_t t[2] = { mp_obj_new_int(sx), mp_obj_new_int(sy) };
    return mp_obj_new_tuple(2, t);
}
static MP_DEFINE_CONST_FUN_OBJ_0(mplcd_read_touch_obj, mplcd_read_touch);

/* ------------------------------------------------------------------- module */

static const mp_rom_map_elem_t mplcd_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__),         MP_ROM_QSTR(MP_QSTR_lcd) },
    { MP_ROM_QSTR(MP_QSTR_init),             MP_ROM_PTR(&mplcd_init_obj) },
    { MP_ROM_QSTR(MP_QSTR_display),          MP_ROM_PTR(&mplcd_display_obj) },
    { MP_ROM_QSTR(MP_QSTR_invert),           MP_ROM_PTR(&mplcd_invert_obj) },
    { MP_ROM_QSTR(MP_QSTR_size),             MP_ROM_PTR(&mplcd_size_obj) },
    { MP_ROM_QSTR(MP_QSTR_color565),         MP_ROM_PTR(&mplcd_color565_obj) },
    { MP_ROM_QSTR(MP_QSTR_fill),             MP_ROM_PTR(&mplcd_fill_obj) },
    { MP_ROM_QSTR(MP_QSTR_fill_rect),        MP_ROM_PTR(&mplcd_fill_rect_obj) },
    { MP_ROM_QSTR(MP_QSTR_pixel),            MP_ROM_PTR(&mplcd_pixel_obj) },
    { MP_ROM_QSTR(MP_QSTR_blit),             MP_ROM_PTR(&mplcd_blit_obj) },
    { MP_ROM_QSTR(MP_QSTR_mount),            MP_ROM_PTR(&mplcd_mount_obj) },
    { MP_ROM_QSTR(MP_QSTR_unmount),          MP_ROM_PTR(&mplcd_unmount_obj) },
    { MP_ROM_QSTR(MP_QSTR_font_load),        MP_ROM_PTR(&mplcd_font_load_obj) },
    { MP_ROM_QSTR(MP_QSTR_font_bg),          MP_ROM_PTR(&mplcd_font_bg_obj) },
    { MP_ROM_QSTR(MP_QSTR_text),             MP_ROM_PTR(&mplcd_text_obj) },
    { MP_ROM_QSTR(MP_QSTR_png),              MP_ROM_PTR(&mplcd_png_obj) },
    { MP_ROM_QSTR(MP_QSTR_jpeg),             MP_ROM_PTR(&mplcd_jpeg_obj) },
    { MP_ROM_QSTR(MP_QSTR_touch_init),       MP_ROM_PTR(&mplcd_touch_init_obj) },
    { MP_ROM_QSTR(MP_QSTR_touch_config),     MP_ROM_PTR(&mplcd_touch_config_obj) },
    { MP_ROM_QSTR(MP_QSTR_touch_calibration), MP_ROM_PTR(&mplcd_touch_calibration_obj) },
    { MP_ROM_QSTR(MP_QSTR_read_raw),         MP_ROM_PTR(&mplcd_read_raw_obj) },
    { MP_ROM_QSTR(MP_QSTR_read_touch),       MP_ROM_PTR(&mplcd_read_touch_obj) },
};
static MP_DEFINE_CONST_DICT(mplcd_globals, mplcd_globals_table);

const mp_obj_module_t lcd_module = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t *)&mplcd_globals,
};

MP_REGISTER_MODULE(MP_QSTR_lcd, lcd_module);
