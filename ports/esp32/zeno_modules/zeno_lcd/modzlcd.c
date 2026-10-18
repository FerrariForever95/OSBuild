/*
 * modzlcd.c -- "lcd" MicroPython module: Zeno OS low-level display backend.
 *
 * Thin bindings over lcd_bus.h / ili9488_panel.h. Kept intentionally small:
 * drawing primitives return quickly (a handful of locked bus transactions),
 * so no background task or buffering is introduced here. All blocking waits
 * inside panel init use mp_hal_delay_ms so the scheduler/other threads are
 * not starved during the ~450 ms bring-up sequence.
 */
#include "py/obj.h"
#include "py/runtime.h"
#include "py/mphal.h"

#include "lcd_bus.h"
#include "ili9488_panel.h"
#include "zeno_lcd_config.h"
#include "zeno_mp_err.h"

static bool s_delay_fn_set;

/* zl_panel_set_delay_fn() takes a plain uint32_t callback so ili9488_panel.c
 * has no MicroPython-type dependency; mp_hal_delay_ms() takes mp_uint_t,
 * which is ABI-compatible on this target but not the same C type, so a
 * thin wrapper avoids a strict-prototype warning/-Werror failure. */
static void delay_ms_wrapper(uint32_t ms) { mp_hal_delay_ms((mp_uint_t)ms); }

static void ensure_delay_fn(void)
{
    if (!s_delay_fn_set) {
        zl_panel_set_delay_fn(delay_ms_wrapper);
        s_delay_fn_set = true;
    }
}

static void require_ready(void)
{
    if (!zl_panel_ready()) {
        mp_raise_msg(&mp_type_OSError, MP_ERROR_TEXT("lcd.init() must be called first"));
    }
}

/* lcd.init(rotation=0, madctl=None) */
static mp_obj_t zlcd_init(size_t n_args, const mp_obj_t *pos, mp_map_t *kw)
{
    enum { ARG_rotation, ARG_madctl };
    static const mp_arg_t allowed[] = {
        { MP_QSTR_rotation, MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = 0} },
        { MP_QSTR_madctl,   MP_ARG_KW_ONLY | MP_ARG_OBJ, {.u_rom_obj = MP_ROM_NONE} },
    };
    mp_arg_val_t a[MP_ARRAY_SIZE(allowed)];
    mp_arg_parse_all(n_args, pos, kw, MP_ARRAY_SIZE(allowed), allowed, a);

    ensure_delay_fn();

    esp_err_t e = zl_bus_init();
    if (e != ESP_OK) {
        zl_raise_esp_err(e);
    }

    int madctl = (a[ARG_madctl].u_obj == mp_const_none) ? -1 : mp_obj_get_int(a[ARG_madctl].u_obj);
    e = zl_panel_init(a[ARG_rotation].u_int, madctl);
    if (e != ESP_OK) {
        zl_raise_esp_err(e);
    }
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_KW(zlcd_init_obj, 0, zlcd_init);

/* lcd.set_rotation(rotation, madctl=None) */
static mp_obj_t zlcd_set_rotation(size_t n_args, const mp_obj_t *args)
{
    require_ready();
    int madctl = (n_args > 1 && args[1] != mp_const_none) ? mp_obj_get_int(args[1]) : -1;
    esp_err_t e = zl_panel_set_rotation(mp_obj_get_int(args[0]), madctl);
    if (e != ESP_OK) {
        zl_raise_esp_err(e);
    }
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(zlcd_set_rotation_obj, 1, 2, zlcd_set_rotation);

static mp_obj_t zlcd_width(void)    { require_ready(); return mp_obj_new_int(zl_panel_width()); }
static mp_obj_t zlcd_height(void)   { require_ready(); return mp_obj_new_int(zl_panel_height()); }
static mp_obj_t zlcd_rotation(void) { require_ready(); return mp_obj_new_int(zl_panel_rotation()); }
static mp_obj_t zlcd_madctl(void)   { require_ready(); return mp_obj_new_int(zl_panel_madctl()); }
static MP_DEFINE_CONST_FUN_OBJ_0(zlcd_width_obj, zlcd_width);
static MP_DEFINE_CONST_FUN_OBJ_0(zlcd_height_obj, zlcd_height);
static MP_DEFINE_CONST_FUN_OBJ_0(zlcd_rotation_obj, zlcd_rotation);
static MP_DEFINE_CONST_FUN_OBJ_0(zlcd_madctl_obj, zlcd_madctl);

static bool take_lock_or_raise(void)
{
    if (!zl_bus_lock()) {
        mp_raise_msg(&mp_type_OSError, MP_ERROR_TEXT("lcd bus busy"));
    }
    return true;
}

/* lcd.fill(color) */
static mp_obj_t zlcd_fill(mp_obj_t color_in)
{
    require_ready();
    take_lock_or_raise();
    zl_fill_screen((uint16_t)mp_obj_get_int(color_in));
    zl_bus_unlock();
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_1(zlcd_fill_obj, zlcd_fill);

/* lcd.pixel(x, y, color) */
static mp_obj_t zlcd_pixel(size_t n_args, const mp_obj_t *a)
{
    require_ready();
    take_lock_or_raise();
    zl_draw_pixel(mp_obj_get_int(a[0]), mp_obj_get_int(a[1]), (uint16_t)mp_obj_get_int(a[2]));
    zl_bus_unlock();
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(zlcd_pixel_obj, 3, 3, zlcd_pixel);

/* lcd.fill_rect(x, y, w, h, color) */
static mp_obj_t zlcd_fill_rect(size_t n_args, const mp_obj_t *a)
{
    require_ready();
    take_lock_or_raise();
    zl_fill_rect(mp_obj_get_int(a[0]), mp_obj_get_int(a[1]),
                mp_obj_get_int(a[2]), mp_obj_get_int(a[3]), (uint16_t)mp_obj_get_int(a[4]));
    zl_bus_unlock();
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(zlcd_fill_rect_obj, 5, 5, zlcd_fill_rect);

/* lcd.hline(x, y, w, color) / lcd.vline(x, y, h, color) */
static mp_obj_t zlcd_hline(size_t n_args, const mp_obj_t *a)
{
    require_ready();
    take_lock_or_raise();
    zl_fill_rect(mp_obj_get_int(a[0]), mp_obj_get_int(a[1]), mp_obj_get_int(a[2]), 1,
                (uint16_t)mp_obj_get_int(a[3]));
    zl_bus_unlock();
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(zlcd_hline_obj, 4, 4, zlcd_hline);

static mp_obj_t zlcd_vline(size_t n_args, const mp_obj_t *a)
{
    require_ready();
    take_lock_or_raise();
    zl_fill_rect(mp_obj_get_int(a[0]), mp_obj_get_int(a[1]), 1, mp_obj_get_int(a[2]),
                (uint16_t)mp_obj_get_int(a[3]));
    zl_bus_unlock();
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(zlcd_vline_obj, 4, 4, zlcd_vline);

/* lcd.line(x0, y0, x1, y1, color) */
static mp_obj_t zlcd_line(size_t n_args, const mp_obj_t *a)
{
    require_ready();
    take_lock_or_raise();
    zl_draw_line(mp_obj_get_int(a[0]), mp_obj_get_int(a[1]),
                mp_obj_get_int(a[2]), mp_obj_get_int(a[3]), (uint16_t)mp_obj_get_int(a[4]));
    zl_bus_unlock();
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(zlcd_line_obj, 5, 5, zlcd_line);

/* lcd.rect(x, y, w, h, color) -- outline */
static mp_obj_t zlcd_rect(size_t n_args, const mp_obj_t *a)
{
    require_ready();
    take_lock_or_raise();
    zl_draw_rect(mp_obj_get_int(a[0]), mp_obj_get_int(a[1]),
                mp_obj_get_int(a[2]), mp_obj_get_int(a[3]), (uint16_t)mp_obj_get_int(a[4]));
    zl_bus_unlock();
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(zlcd_rect_obj, 5, 5, zlcd_rect);

/* lcd.circle(x0, y0, r, color, fill=False) */
static mp_obj_t zlcd_circle(size_t n_args, const mp_obj_t *a)
{
    require_ready();
    bool fill = (n_args > 4) && mp_obj_is_true(a[4]);
    take_lock_or_raise();
    if (fill) {
        zl_fill_circle(mp_obj_get_int(a[0]), mp_obj_get_int(a[1]), mp_obj_get_int(a[2]),
                       (uint16_t)mp_obj_get_int(a[3]));
    } else {
        zl_draw_circle(mp_obj_get_int(a[0]), mp_obj_get_int(a[1]), mp_obj_get_int(a[2]),
                       (uint16_t)mp_obj_get_int(a[3]));
    }
    zl_bus_unlock();
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(zlcd_circle_obj, 4, 5, zlcd_circle);

/* lcd.blit(x, y, w, h, buf) -- buf: RGB565 big-endian bytes, len == w*h*2 */
static mp_obj_t zlcd_blit(size_t n_args, const mp_obj_t *a)
{
    require_ready();
    int x = mp_obj_get_int(a[0]), y = mp_obj_get_int(a[1]);
    int w = mp_obj_get_int(a[2]), h = mp_obj_get_int(a[3]);
    if (w <= 0 || h <= 0 || x < 0 || y < 0 || x + w > zl_panel_width() || y + h > zl_panel_height()) {
        mp_raise_ValueError(MP_ERROR_TEXT("blit out of bounds"));
    }
    mp_buffer_info_t buf;
    mp_get_buffer_raise(a[4], &buf, MP_BUFFER_READ);
    if (buf.len != (size_t)w * (size_t)h * 2u) {
        mp_raise_ValueError(MP_ERROR_TEXT("buffer size must be w*h*2"));
    }
    take_lock_or_raise();
    zl_blit(x, y, w, h, (const uint8_t *)buf.buf);
    zl_bus_unlock();
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(zlcd_blit_obj, 5, 5, zlcd_blit);

/* lcd.display(on) / lcd.invert(on) */
static mp_obj_t zlcd_display(mp_obj_t on_in)
{
    require_ready();
    take_lock_or_raise();
    zl_panel_display(mp_obj_is_true(on_in));
    zl_bus_unlock();
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_1(zlcd_display_obj, zlcd_display);

static mp_obj_t zlcd_invert(mp_obj_t on_in)
{
    require_ready();
    take_lock_or_raise();
    zl_panel_invert(mp_obj_is_true(on_in));
    zl_bus_unlock();
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_1(zlcd_invert_obj, zlcd_invert);

/* lcd.cmd(cmd, params=None) -- raw passthrough escape hatch */
static mp_obj_t zlcd_cmd(size_t n_args, const mp_obj_t *a)
{
    require_ready();
    int cmd = mp_obj_get_int(a[0]);
    const uint8_t *p = NULL;
    size_t n = 0;
    mp_buffer_info_t buf;
    if (n_args == 2 && a[1] != mp_const_none) {
        mp_get_buffer_raise(a[1], &buf, MP_BUFFER_READ);
        p = (const uint8_t *)buf.buf;
        n = buf.len;
    }
    take_lock_or_raise();
    zl_panel_cmd((uint8_t)cmd, p, n);
    zl_bus_unlock();
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(zlcd_cmd_obj, 1, 2, zlcd_cmd);

/* lcd.bus_timing(low_nops=None, high_nops=None) -> (low, high)
 * Stage-A tuning knob: see VALIDATION.md. Not in the original (it had no
 * configurable WR timing — its timing was whatever -Og gpio_set_level()
 * call overhead happened to produce). */
static mp_obj_t zlcd_bus_timing(size_t n_args, const mp_obj_t *a)
{
    uint8_t lo, hi;
    zl_bus_get_timing(&lo, &hi);
    if (n_args >= 1 && a[0] != mp_const_none) lo = (uint8_t)mp_obj_get_int(a[0]);
    if (n_args >= 2 && a[1] != mp_const_none) hi = (uint8_t)mp_obj_get_int(a[1]);
    zl_bus_set_timing(lo, hi);
    mp_obj_t t[2] = { mp_obj_new_int(lo), mp_obj_new_int(hi) };
    return mp_obj_new_tuple(2, t);
}
static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(zlcd_bus_timing_obj, 0, 2, zlcd_bus_timing);

static const mp_rom_map_elem_t zlcd_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__),    MP_ROM_QSTR(MP_QSTR_lcd) },
    { MP_ROM_QSTR(MP_QSTR_init),        MP_ROM_PTR(&zlcd_init_obj) },
    { MP_ROM_QSTR(MP_QSTR_set_rotation),MP_ROM_PTR(&zlcd_set_rotation_obj) },
    { MP_ROM_QSTR(MP_QSTR_width),       MP_ROM_PTR(&zlcd_width_obj) },
    { MP_ROM_QSTR(MP_QSTR_height),      MP_ROM_PTR(&zlcd_height_obj) },
    { MP_ROM_QSTR(MP_QSTR_rotation),    MP_ROM_PTR(&zlcd_rotation_obj) },
    { MP_ROM_QSTR(MP_QSTR_madctl),      MP_ROM_PTR(&zlcd_madctl_obj) },
    { MP_ROM_QSTR(MP_QSTR_fill),        MP_ROM_PTR(&zlcd_fill_obj) },
    { MP_ROM_QSTR(MP_QSTR_pixel),       MP_ROM_PTR(&zlcd_pixel_obj) },
    { MP_ROM_QSTR(MP_QSTR_fill_rect),   MP_ROM_PTR(&zlcd_fill_rect_obj) },
    { MP_ROM_QSTR(MP_QSTR_hline),       MP_ROM_PTR(&zlcd_hline_obj) },
    { MP_ROM_QSTR(MP_QSTR_vline),       MP_ROM_PTR(&zlcd_vline_obj) },
    { MP_ROM_QSTR(MP_QSTR_line),        MP_ROM_PTR(&zlcd_line_obj) },
    { MP_ROM_QSTR(MP_QSTR_rect),        MP_ROM_PTR(&zlcd_rect_obj) },
    { MP_ROM_QSTR(MP_QSTR_circle),      MP_ROM_PTR(&zlcd_circle_obj) },
    { MP_ROM_QSTR(MP_QSTR_blit),        MP_ROM_PTR(&zlcd_blit_obj) },
    { MP_ROM_QSTR(MP_QSTR_display),     MP_ROM_PTR(&zlcd_display_obj) },
    { MP_ROM_QSTR(MP_QSTR_invert),      MP_ROM_PTR(&zlcd_invert_obj) },
    { MP_ROM_QSTR(MP_QSTR_cmd),         MP_ROM_PTR(&zlcd_cmd_obj) },
    { MP_ROM_QSTR(MP_QSTR_bus_timing),  MP_ROM_PTR(&zlcd_bus_timing_obj) },
};
static MP_DEFINE_CONST_DICT(zlcd_globals, zlcd_globals_table);

const mp_obj_module_t mp_module_lcd = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t *)&zlcd_globals,
};
MP_REGISTER_MODULE(MP_QSTR_lcd, mp_module_lcd);
