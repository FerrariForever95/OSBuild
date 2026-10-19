/*
 * mock_bus.c -- host replacement for lcd_bus.c. Emulates the ILI9488 address
 * window (CASET 0x2A / PASET 0x2B / RAMWR 0x2C) into a 480x480 RGB565
 * framebuffer, so the REAL ili9488_panel.c / zeno_gfx.c / zeno_font.c /
 * modzlcd.c run unmodified. Also registers a tiny "mocklcd" module that lets
 * Python tests inspect the framebuffer.
 *
 * Strictness: it counts pixels written PAST the end of the window ("extra")
 * and pixels outside the framebuffer ("oob"); a correct driver keeps both 0.
 */
#include <stdio.h>
#include <string.h>
#include "py/obj.h"
#include "py/runtime.h"
#include "lcd_bus.h"

#define FB_W 480
#define FB_H 480
static uint16_t fb[FB_W * FB_H];
static bool s_init;
static uint8_t s_lo = 6, s_hi = 2;

static int last_cmd = -1, pcount;
static uint8_t params[4];
static int x1w, x2w = FB_W - 1, y1w, y2w = FB_H - 1, cx, cy;
static bool ramwr, have_hi, window_full;
static uint8_t hi_byte;
static unsigned long extra_px, oob_px, txn;

esp_err_t zl_bus_init(void) { s_init = true; return ESP_OK; }
bool zl_bus_is_init(void) { return s_init; }
/* real (non-recursive) lock semantics, so a leaked lock is detectable */
static bool s_locked;
bool zl_bus_lock(void) { if (s_locked) return false; s_locked = true; return true; }
void zl_bus_unlock(void) { s_locked = false; }
void zl_bus_set_timing(uint8_t l, uint8_t h) { s_lo = l; s_hi = h; }
void zl_bus_get_timing(uint8_t *l, uint8_t *h) { *l = s_lo; *h = s_hi; }
void zl_bus_reset_level(int level) { (void)level; }
void zl_bus_idle(void) {}

static void put_pixel(uint16_t c)
{
    if (window_full) extra_px++;
    if (cx >= 0 && cx < FB_W && cy >= 0 && cy < FB_H) fb[cy * FB_W + cx] = c; else oob_px++;
    if (cx == x2w && cy == y2w) window_full = true;
    if (++cx > x2w) { cx = x1w; if (++cy > y2w) { cy = y1w; } }
}

static void feed_byte(uint8_t b)
{
    if (ramwr) {
        if (!have_hi) { hi_byte = b; have_hi = true; }
        else { put_pixel((uint16_t)((hi_byte << 8) | b)); have_hi = false; }
    } else if (last_cmd == 0x2A || last_cmd == 0x2B) {
        if (pcount < 4) params[pcount++] = b;
        if (pcount == 4) {
            int a = (params[0] << 8) | params[1], z = (params[2] << 8) | params[3];
            if (last_cmd == 0x2A) { x1w = a; x2w = z; } else { y1w = a; y2w = z; }
        }
    }
}

void zl_bus_write_cmd(uint8_t cmd)
{
    txn++;
    last_cmd = cmd; pcount = 0; ramwr = false; have_hi = false;
    if (cmd == 0x2C) { ramwr = true; cx = x1w; cy = y1w; window_full = false; }
}
void zl_bus_write_data(const uint8_t *d, size_t n) { txn++; for (size_t i = 0; i < n; i++) feed_byte(d[i]); }
void zl_bus_write_bytes(const uint8_t *d, size_t n) { zl_bus_write_data(d, n); }
void zl_bus_write_color(uint16_t c, uint32_t npix)
{
    txn++;
    for (uint32_t i = 0; i < npix; i++) { feed_byte((uint8_t)(c >> 8)); feed_byte((uint8_t)c); }
}

/* ---- mocklcd module ---- */
static mp_obj_t m_get(mp_obj_t x, mp_obj_t y)
{
    int xi = mp_obj_get_int(x), yi = mp_obj_get_int(y);
    if (xi < 0 || yi < 0 || xi >= FB_W || yi >= FB_H) mp_raise_ValueError(MP_ERROR_TEXT("get oob"));
    return mp_obj_new_int(fb[yi * FB_W + xi]);
}
static MP_DEFINE_CONST_FUN_OBJ_2(m_get_obj, m_get);

static mp_obj_t m_clear(mp_obj_t c)
{
    uint16_t v = (uint16_t)mp_obj_get_int(c);
    for (int i = 0; i < FB_W * FB_H; i++) fb[i] = v;
    extra_px = oob_px = txn = 0;
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_1(m_clear_obj, m_clear);

static mp_obj_t m_stats(void)
{
    mp_obj_t t[3] = { mp_obj_new_int_from_uint(extra_px), mp_obj_new_int_from_uint(oob_px), mp_obj_new_int_from_uint(txn) };
    return mp_obj_new_tuple(3, t);
}
static MP_DEFINE_CONST_FUN_OBJ_0(m_stats_obj, m_stats);

static mp_obj_t m_count(mp_obj_t c)
{
    uint16_t v = (uint16_t)mp_obj_get_int(c);
    int w = 0, h = 0; (void)w; (void)h;
    long n = 0;
    for (int i = 0; i < FB_W * FB_H; i++) n += (fb[i] == v);
    return mp_obj_new_int((mp_int_t)n);
}
static MP_DEFINE_CONST_FUN_OBJ_1(m_count_obj, m_count);

/* dump(path, w, h, scale=1) -> binary PPM of the top-left w x h region */
static mp_obj_t m_dump(size_t n, const mp_obj_t *a)
{
    const char *path = mp_obj_str_get_data(a[0], &(size_t){0});
    int w = mp_obj_get_int(a[1]), h = mp_obj_get_int(a[2]);
    int sc = n > 3 ? mp_obj_get_int(a[3]) : 1;
    FILE *f = fopen(path, "wb");
    if (!f) mp_raise_OSError(1);
    fprintf(f, "P6\n%d %d\n255\n", w * sc, h * sc);
    for (int y = 0; y < h * sc; y++) for (int x = 0; x < w * sc; x++) {
        uint16_t c = fb[(y / sc) * FB_W + (x / sc)];
        uint8_t rgb[3] = { (uint8_t)(((c >> 11) & 31) * 255 / 31), (uint8_t)(((c >> 5) & 63) * 255 / 63), (uint8_t)((c & 31) * 255 / 31) };
        fwrite(rgb, 1, 3, f);
    }
    fclose(f);
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(m_dump_obj, 3, 4, m_dump);

static mp_obj_t m_locked(void) { return mp_obj_new_bool(s_locked); }
static MP_DEFINE_CONST_FUN_OBJ_0(m_locked_obj, m_locked);

static const mp_rom_map_elem_t m_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_mocklcd) },
    { MP_ROM_QSTR(MP_QSTR_get), MP_ROM_PTR(&m_get_obj) },
    { MP_ROM_QSTR(MP_QSTR_clear), MP_ROM_PTR(&m_clear_obj) },
    { MP_ROM_QSTR(MP_QSTR_stats), MP_ROM_PTR(&m_stats_obj) },
    { MP_ROM_QSTR(MP_QSTR_count), MP_ROM_PTR(&m_count_obj) },
    { MP_ROM_QSTR(MP_QSTR_dump), MP_ROM_PTR(&m_dump_obj) },
    { MP_ROM_QSTR(MP_QSTR_locked), MP_ROM_PTR(&m_locked_obj) },
};
static MP_DEFINE_CONST_DICT(m_globals, m_table);
const mp_obj_module_t mp_module_mocklcd = { .base = { &mp_type_module }, .globals = (mp_obj_dict_t *)&m_globals };
MP_REGISTER_MODULE(MP_QSTR_mocklcd, mp_module_mocklcd);
