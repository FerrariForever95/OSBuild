/*
 * modzlcd.c -- "lcd" MicroPython module: Zeno OS low-level display backend.
 *
 * Thin bindings over lcd_bus.h / ili9488_panel.h. Kept intentionally small:
 * drawing primitives return quickly (a handful of locked bus transactions),
 * so no background task or buffering is introduced here. All blocking waits
 * inside panel init use mp_hal_delay_ms so the scheduler/other threads are
 * not starved during the ~450 ms bring-up sequence.
 */
#include <string.h>
#include "py/obj.h"
#include "py/runtime.h"
#include "py/mphal.h"

#include "lcd_bus.h"
#include "ili9488_panel.h"
#include "zeno_gfx.h"
#include "zeno_font.h"
#include "zeno_pure.h"
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

/* lcd.line(x0, y0, x1, y1, color, width=1) */
static mp_obj_t zlcd_line(size_t n_args, const mp_obj_t *a)
{
    require_ready();
    int width = (n_args > 5) ? mp_obj_get_int(a[5]) : 1;
    take_lock_or_raise();
    if (width <= 1) {
        zl_draw_line(mp_obj_get_int(a[0]), mp_obj_get_int(a[1]),
                    mp_obj_get_int(a[2]), mp_obj_get_int(a[3]), (uint16_t)mp_obj_get_int(a[4]));
    } else {
        zl_draw_thick_line(mp_obj_get_int(a[0]), mp_obj_get_int(a[1]),
                           mp_obj_get_int(a[2]), mp_obj_get_int(a[3]), width,
                           (uint16_t)mp_obj_get_int(a[4]));
    }
    zl_bus_unlock();
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(zlcd_line_obj, 5, 6, zlcd_line);

/* lcd.rect(x, y, w, h, color, fill=False) */
static mp_obj_t zlcd_rect(size_t n_args, const mp_obj_t *a)
{
    require_ready();
    bool fill = (n_args > 5) && mp_obj_is_true(a[5]);
    take_lock_or_raise();
    if (fill) {
        zl_fill_rect(mp_obj_get_int(a[0]), mp_obj_get_int(a[1]),
                    mp_obj_get_int(a[2]), mp_obj_get_int(a[3]), (uint16_t)mp_obj_get_int(a[4]));
    } else {
        zl_draw_rect(mp_obj_get_int(a[0]), mp_obj_get_int(a[1]),
                    mp_obj_get_int(a[2]), mp_obj_get_int(a[3]), (uint16_t)mp_obj_get_int(a[4]));
    }
    zl_bus_unlock();
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(zlcd_rect_obj, 5, 6, zlcd_rect);

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


/* ======================================================================== *
 * Extended primitives, text and fonts (zeno_gfx.c / zeno_font.c).
 * All of these accept keyword arguments. Arguments are fully parsed and
 * validated BEFORE the bus lock is taken, so a bad argument can never leave
 * the lock held.
 * ======================================================================== */
#define ARG_INT(n)          { MP_QSTR_##n, MP_ARG_REQUIRED | MP_ARG_INT, {.u_int = 0} }
#define ARG_INT_OPT(n, d)   { MP_QSTR_##n, MP_ARG_INT, {.u_int = (d)} }
#define ARG_BOOL_OPT(n, d)  { MP_QSTR_##n, MP_ARG_BOOL, {.u_bool = (d)} }
#define ARG_OBJ(n)          { MP_QSTR_##n, MP_ARG_REQUIRED | MP_ARG_OBJ, {.u_rom_obj = MP_ROM_NONE} }
#define ARG_OBJ_OPT(n, d)   { MP_QSTR_##n, MP_ARG_OBJ, {.u_obj = (d)} }
#define ARGS_PARSE(tbl) \
    mp_arg_val_t a[MP_ARRAY_SIZE(tbl)]; \
    mp_arg_parse_all(n_args, pos, kw, MP_ARRAY_SIZE(tbl), tbl, a)

/* lcd.ellipse(x, y, rx, ry, color, fill=False) */
static mp_obj_t zlcd_ellipse(size_t n_args, const mp_obj_t *pos, mp_map_t *kw)
{
    enum { x, y, rx, ry, color, fill };
    static const mp_arg_t t[] = { ARG_INT(x), ARG_INT(y), ARG_INT(rx), ARG_INT(ry), ARG_INT(color),
                                  ARG_BOOL_OPT(fill, false) };
    ARGS_PARSE(t);
    require_ready();
    take_lock_or_raise();
    if (a[fill].u_bool) zl_fill_ellipse(a[x].u_int, a[y].u_int, a[rx].u_int, a[ry].u_int, (uint16_t)a[color].u_int);
    else                zl_draw_ellipse(a[x].u_int, a[y].u_int, a[rx].u_int, a[ry].u_int, (uint16_t)a[color].u_int);
    zl_bus_unlock();
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_KW(zlcd_ellipse_obj, 0, zlcd_ellipse);

/* lcd.round_rect(x, y, w, h, r, color, fill=False) */
static mp_obj_t zlcd_round_rect(size_t n_args, const mp_obj_t *pos, mp_map_t *kw)
{
    enum { x, y, w, h, r, color, fill };
    static const mp_arg_t t[] = { ARG_INT(x), ARG_INT(y), ARG_INT(w), ARG_INT(h), ARG_INT(r), ARG_INT(color),
                                  ARG_BOOL_OPT(fill, false) };
    ARGS_PARSE(t);
    require_ready();
    take_lock_or_raise();
    if (a[fill].u_bool) zl_fill_round_rect(a[x].u_int, a[y].u_int, a[w].u_int, a[h].u_int, a[r].u_int, (uint16_t)a[color].u_int);
    else                zl_draw_round_rect(a[x].u_int, a[y].u_int, a[w].u_int, a[h].u_int, a[r].u_int, (uint16_t)a[color].u_int);
    zl_bus_unlock();
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_KW(zlcd_round_rect_obj, 0, zlcd_round_rect);

/* lcd.triangle(x0, y0, x1, y1, x2, y2, color, fill=False) */
static mp_obj_t zlcd_triangle(size_t n_args, const mp_obj_t *pos, mp_map_t *kw)
{
    enum { x0, y0, x1, y1, x2, y2, color, fill };
    static const mp_arg_t t[] = { ARG_INT(x0), ARG_INT(y0), ARG_INT(x1), ARG_INT(y1), ARG_INT(x2), ARG_INT(y2),
                                  ARG_INT(color), ARG_BOOL_OPT(fill, false) };
    ARGS_PARSE(t);
    require_ready();
    take_lock_or_raise();
    if (a[fill].u_bool) zl_fill_triangle(a[x0].u_int, a[y0].u_int, a[x1].u_int, a[y1].u_int, a[x2].u_int, a[y2].u_int, (uint16_t)a[color].u_int);
    else                zl_draw_triangle(a[x0].u_int, a[y0].u_int, a[x1].u_int, a[y1].u_int, a[x2].u_int, a[y2].u_int, (uint16_t)a[color].u_int);
    zl_bus_unlock();
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_KW(zlcd_triangle_obj, 0, zlcd_triangle);

/* points: [(x,y), (x,y), ...] or flat [x0, y0, x1, y1, ...] -> pts[], returns n */
static int get_points(mp_obj_t seq, int *pts)
{
    size_t len;
    mp_obj_t *items;
    mp_obj_get_array(seq, &len, &items);
    if (len == 0) {
        return 0;
    }
    int n;
    if (mp_obj_is_int(items[0])) {
        if (len & 1) {
            mp_raise_ValueError(MP_ERROR_TEXT("flat point list needs an even length"));
        }
        n = (int)(len / 2);
        if (n > ZL_POLY_MAX) mp_raise_ValueError(MP_ERROR_TEXT("too many points (max 64)"));
        for (int i = 0; i < 2 * n; i++) pts[i] = mp_obj_get_int(items[i]);
    } else {
        n = (int)len;
        if (n > ZL_POLY_MAX) mp_raise_ValueError(MP_ERROR_TEXT("too many points (max 64)"));
        for (int i = 0; i < n; i++) {
            mp_obj_t *xy;
            mp_obj_get_array_fixed_n(items[i], 2, &xy);
            pts[2 * i] = mp_obj_get_int(xy[0]);
            pts[2 * i + 1] = mp_obj_get_int(xy[1]);
        }
    }
    return n;
}

/* lcd.polygon(points, color, fill=False) */
static mp_obj_t zlcd_polygon(size_t n_args, const mp_obj_t *pos, mp_map_t *kw)
{
    enum { points, color, fill };
    static const mp_arg_t t[] = { ARG_OBJ(points), ARG_INT(color), ARG_BOOL_OPT(fill, false) };
    ARGS_PARSE(t);
    int pts[2 * ZL_POLY_MAX];
    int n = get_points(a[points].u_obj, pts);
    require_ready();
    take_lock_or_raise();
    if (a[fill].u_bool) zl_fill_polygon(pts, n, (uint16_t)a[color].u_int);
    else                zl_draw_polygon(pts, n, (uint16_t)a[color].u_int);
    zl_bus_unlock();
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_KW(zlcd_polygon_obj, 0, zlcd_polygon);

/* lcd.arc(x, y, r, start, end, color, thick=1)   degrees, 0 = 3 o'clock, clockwise */
static mp_obj_t zlcd_arc(size_t n_args, const mp_obj_t *pos, mp_map_t *kw)
{
    enum { x, y, r, start, end, color, thick };
    static const mp_arg_t t[] = { ARG_INT(x), ARG_INT(y), ARG_INT(r), ARG_OBJ(start), ARG_OBJ(end),
                                  ARG_INT(color), ARG_INT_OPT(thick, 1) };
    ARGS_PARSE(t);
    float s0 = mp_obj_get_float_to_f(a[start].u_obj), e0 = mp_obj_get_float_to_f(a[end].u_obj);
    require_ready();
    take_lock_or_raise();
    zl_draw_arc(a[x].u_int, a[y].u_int, a[r].u_int, s0, e0, a[thick].u_int, (uint16_t)a[color].u_int);
    zl_bus_unlock();
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_KW(zlcd_arc_obj, 0, zlcd_arc);

/* lcd.pie(x, y, r, start, end, color) -- filled sector */
static mp_obj_t zlcd_pie(size_t n_args, const mp_obj_t *pos, mp_map_t *kw)
{
    enum { x, y, r, start, end, color };
    static const mp_arg_t t[] = { ARG_INT(x), ARG_INT(y), ARG_INT(r), ARG_OBJ(start), ARG_OBJ(end), ARG_INT(color) };
    ARGS_PARSE(t);
    float s0 = mp_obj_get_float_to_f(a[start].u_obj), e0 = mp_obj_get_float_to_f(a[end].u_obj);
    require_ready();
    take_lock_or_raise();
    zl_fill_ring_sector(a[x].u_int, a[y].u_int, 0, a[r].u_int, s0, e0, (uint16_t)a[color].u_int);
    zl_bus_unlock();
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_KW(zlcd_pie_obj, 0, zlcd_pie);

/* lcd.ring(x, y, r_in, r_out, color, start=0, end=360) -- filled annulus / gauge segment */
static mp_obj_t zlcd_ring(size_t n_args, const mp_obj_t *pos, mp_map_t *kw)
{
    enum { x, y, r_in, r_out, color, start, end };
    static const mp_arg_t t[] = { ARG_INT(x), ARG_INT(y), ARG_INT(r_in), ARG_INT(r_out), ARG_INT(color),
                                  ARG_OBJ_OPT(start, MP_OBJ_NEW_SMALL_INT(0)),
                                  ARG_OBJ_OPT(end, MP_OBJ_NEW_SMALL_INT(360)) };
    ARGS_PARSE(t);
    float s0 = mp_obj_get_float_to_f(a[start].u_obj), e0 = mp_obj_get_float_to_f(a[end].u_obj);
    require_ready();
    take_lock_or_raise();
    zl_fill_ring_sector(a[x].u_int, a[y].u_int, a[r_in].u_int, a[r_out].u_int, s0, e0, (uint16_t)a[color].u_int);
    zl_bus_unlock();
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_KW(zlcd_ring_obj, 0, zlcd_ring);

/* lcd.bezier(x0,y0, x1,y1, x2,y2, x3,y3, color) cubic */
static mp_obj_t zlcd_bezier(size_t n_args, const mp_obj_t *pos, mp_map_t *kw)
{
    enum { x0, y0, x1, y1, x2, y2, x3, y3, color };
    static const mp_arg_t t[] = { ARG_INT(x0), ARG_INT(y0), ARG_INT(x1), ARG_INT(y1), ARG_INT(x2), ARG_INT(y2),
                                  ARG_INT(x3), ARG_INT(y3), ARG_INT(color) };
    ARGS_PARSE(t);
    require_ready();
    take_lock_or_raise();
    zl_draw_bezier3(a[x0].u_int, a[y0].u_int, a[x1].u_int, a[y1].u_int, a[x2].u_int, a[y2].u_int,
                    a[x3].u_int, a[y3].u_int, (uint16_t)a[color].u_int);
    zl_bus_unlock();
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_KW(zlcd_bezier_obj, 0, zlcd_bezier);

/* lcd.qbezier(x0,y0, x1,y1, x2,y2, color) quadratic */
static mp_obj_t zlcd_qbezier(size_t n_args, const mp_obj_t *pos, mp_map_t *kw)
{
    enum { x0, y0, x1, y1, x2, y2, color };
    static const mp_arg_t t[] = { ARG_INT(x0), ARG_INT(y0), ARG_INT(x1), ARG_INT(y1), ARG_INT(x2), ARG_INT(y2),
                                  ARG_INT(color) };
    ARGS_PARSE(t);
    require_ready();
    take_lock_or_raise();
    zl_draw_bezier2(a[x0].u_int, a[y0].u_int, a[x1].u_int, a[y1].u_int, a[x2].u_int, a[y2].u_int,
                    (uint16_t)a[color].u_int);
    zl_bus_unlock();
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_KW(zlcd_qbezier_obj, 0, zlcd_qbezier);

/* lcd.gradient(x, y, w, h, c0, c1, vertical=True) */
static mp_obj_t zlcd_gradient(size_t n_args, const mp_obj_t *pos, mp_map_t *kw)
{
    enum { x, y, w, h, c0, c1, vertical };
    static const mp_arg_t t[] = { ARG_INT(x), ARG_INT(y), ARG_INT(w), ARG_INT(h), ARG_INT(c0), ARG_INT(c1),
                                  ARG_BOOL_OPT(vertical, true) };
    ARGS_PARSE(t);
    require_ready();
    take_lock_or_raise();
    zl_fill_rect_gradient(a[x].u_int, a[y].u_int, a[w].u_int, a[h].u_int,
                          (uint16_t)a[c0].u_int, (uint16_t)a[c1].u_int, a[vertical].u_bool);
    zl_bus_unlock();
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_KW(zlcd_gradient_obj, 0, zlcd_gradient);

/* ---- colour helpers (no hardware access) ---- */
static mp_obj_t zlcd_color565(mp_obj_t r, mp_obj_t g, mp_obj_t b)
{
    return mp_obj_new_int(zl_color565((uint8_t)mp_obj_get_int(r), (uint8_t)mp_obj_get_int(g),
                                      (uint8_t)mp_obj_get_int(b)));
}
static MP_DEFINE_CONST_FUN_OBJ_3(zlcd_color565_obj, zlcd_color565);

/* lcd.blend(c0, c1, alpha) -- alpha 0..255; 0 -> c0, 255 -> c1 */
static mp_obj_t zlcd_blend(mp_obj_t c0, mp_obj_t c1, mp_obj_t al)
{
    int v = mp_obj_get_int(al);
    if (v < 0) v = 0;
    if (v > 255) v = 255;
    return mp_obj_new_int(zl_blend565((uint16_t)mp_obj_get_int(c0), (uint16_t)mp_obj_get_int(c1), (uint8_t)v));
}
static MP_DEFINE_CONST_FUN_OBJ_3(zlcd_blend_obj, zlcd_blend);

/* ---- fonts and text ---- */
enum { ZFONT_MONO12, ZFONT_MONO16, ZFONT_SANS16, ZFONT_SANS24B, ZFONT_DIGITS48 };

/* font: int id of a built-in font, or any bytes-like .zfont image */
static void resolve_font(mp_obj_t o, zl_font_t *f)
{
    if (mp_obj_is_int(o)) {
        mp_int_t i = mp_obj_get_int(o);
        if (i < 0 || i >= zl_builtin_font_count) {
            mp_raise_ValueError(MP_ERROR_TEXT("unknown built-in font id"));
        }
        if (!zl_font_parse(zl_builtin_fonts[i].data, zl_builtin_fonts[i].len, f)) {
            mp_raise_ValueError(MP_ERROR_TEXT("built-in font corrupt"));
        }
        return;
    }
    mp_buffer_info_t bi;
    mp_get_buffer_raise(o, &bi, MP_BUFFER_READ);
    if (!zl_font_parse((const uint8_t *)bi.buf, bi.len, f)) {
        mp_raise_ValueError(MP_ERROR_TEXT("not a valid .zfont image"));
    }
}

static int check_scale(int s)
{
    if (s < 1 || s > 16) {
        mp_raise_ValueError(MP_ERROR_TEXT("scale must be 1..16"));
    }
    return s;
}

/* lcd.text(x, y, s, fg, bg=None, font=FONT_MONO16, scale=1) -> x after the text */
static mp_obj_t zlcd_text(size_t n_args, const mp_obj_t *pos, mp_map_t *kw)
{
    enum { x, y, s, fg, bg, font, scale };
    static const mp_arg_t t[] = { ARG_INT(x), ARG_INT(y), ARG_OBJ(s), ARG_INT(fg),
                                  ARG_OBJ_OPT(bg, mp_const_none),
                                  ARG_OBJ_OPT(font, MP_OBJ_NEW_SMALL_INT(ZFONT_MONO16)),
                                  ARG_INT_OPT(scale, 1) };
    ARGS_PARSE(t);
    size_t len;
    const char *str = mp_obj_str_get_data(a[s].u_obj, &len);
    zl_font_t f;
    resolve_font(a[font].u_obj, &f);
    int sc = check_scale(a[scale].u_int);
    int32_t bgc = (a[bg].u_obj == mp_const_none) ? -1 : (int32_t)(uint16_t)mp_obj_get_int(a[bg].u_obj);
    require_ready();
    take_lock_or_raise();
    int end_x = zl_text(a[x].u_int, a[y].u_int, str, len, &f, sc, (uint16_t)a[fg].u_int, bgc);
    zl_bus_unlock();
    return mp_obj_new_int(end_x);
}
static MP_DEFINE_CONST_FUN_OBJ_KW(zlcd_text_obj, 0, zlcd_text);

/* lcd.text_size(s, font=FONT_MONO16, scale=1) -> (width, height); no hardware access */
static mp_obj_t zlcd_text_size(size_t n_args, const mp_obj_t *pos, mp_map_t *kw)
{
    enum { s, font, scale };
    static const mp_arg_t t[] = { ARG_OBJ(s), ARG_OBJ_OPT(font, MP_OBJ_NEW_SMALL_INT(ZFONT_MONO16)),
                                  ARG_INT_OPT(scale, 1) };
    ARGS_PARSE(t);
    size_t len;
    const char *str = mp_obj_str_get_data(a[s].u_obj, &len);
    zl_font_t f;
    resolve_font(a[font].u_obj, &f);
    int w, h;
    zl_text_size(str, len, &f, check_scale(a[scale].u_int), &w, &h);
    mp_obj_t tup[2] = { mp_obj_new_int(w), mp_obj_new_int(h) };
    return mp_obj_new_tuple(2, tup);
}
static MP_DEFINE_CONST_FUN_OBJ_KW(zlcd_text_size_obj, 0, zlcd_text_size);

/* lcd.font_info(font) -> (height, ascent, first_char, last_char, bpp) */
static mp_obj_t zlcd_font_info(mp_obj_t font)
{
    zl_font_t f;
    resolve_font(font, &f);
    mp_obj_t tup[5] = { mp_obj_new_int(f.height), mp_obj_new_int(f.ascent), mp_obj_new_int(f.first),
                        mp_obj_new_int((int)f.first + (int)f.count - 1), mp_obj_new_int(f.bpp) };
    return mp_obj_new_tuple(5, tup);
}
static MP_DEFINE_CONST_FUN_OBJ_1(zlcd_font_info_obj, zlcd_font_info);

/* lcd.fonts() -> tuple of built-in font names (index == font id) */
static mp_obj_t zlcd_fonts(void)
{
    mp_obj_t names[16];
    int n = zl_builtin_font_count > 16 ? 16 : zl_builtin_font_count;
    for (int i = 0; i < n; i++) {
        names[i] = mp_obj_new_str(zl_builtin_fonts[i].name, strlen(zl_builtin_fonts[i].name));
    }
    return mp_obj_new_tuple((size_t)n, names);
}
static MP_DEFINE_CONST_FUN_OBJ_0(zlcd_fonts_obj, zlcd_fonts);

/* lcd.blit_mono(x, y, w, h, bits, fg, bg=None, scale=1)
 * bits: 1bpp, rows padded to whole bytes, MSB first; len must be ceil(w/8)*h */
static mp_obj_t zlcd_blit_mono(size_t n_args, const mp_obj_t *pos, mp_map_t *kw)
{
    enum { x, y, w, h, bits, fg, bg, scale };
    static const mp_arg_t t[] = { ARG_INT(x), ARG_INT(y), ARG_INT(w), ARG_INT(h), ARG_OBJ(bits), ARG_INT(fg),
                                  ARG_OBJ_OPT(bg, mp_const_none), ARG_INT_OPT(scale, 1) };
    ARGS_PARSE(t);
    if (a[w].u_int <= 0 || a[h].u_int <= 0 || a[w].u_int > 1024 || a[h].u_int > 1024) {
        mp_raise_ValueError(MP_ERROR_TEXT("bad bitmap size"));
    }
    mp_buffer_info_t bi;
    mp_get_buffer_raise(a[bits].u_obj, &bi, MP_BUFFER_READ);
    if (bi.len != (size_t)((a[w].u_int + 7) / 8) * (size_t)a[h].u_int) {
        mp_raise_ValueError(MP_ERROR_TEXT("buffer size must be ceil(w/8)*h"));
    }
    int sc = check_scale(a[scale].u_int);
    int32_t bgc = (a[bg].u_obj == mp_const_none) ? -1 : (int32_t)(uint16_t)mp_obj_get_int(a[bg].u_obj);
    require_ready();
    take_lock_or_raise();
    zl_blit_mono(a[x].u_int, a[y].u_int, a[w].u_int, a[h].u_int, (const uint8_t *)bi.buf, sc,
                 (uint16_t)a[fg].u_int, bgc);
    zl_bus_unlock();
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_KW(zlcd_blit_mono_obj, 0, zlcd_blit_mono);

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
    { MP_ROM_QSTR(MP_QSTR_ellipse),     MP_ROM_PTR(&zlcd_ellipse_obj) },
    { MP_ROM_QSTR(MP_QSTR_round_rect),  MP_ROM_PTR(&zlcd_round_rect_obj) },
    { MP_ROM_QSTR(MP_QSTR_triangle),    MP_ROM_PTR(&zlcd_triangle_obj) },
    { MP_ROM_QSTR(MP_QSTR_polygon),     MP_ROM_PTR(&zlcd_polygon_obj) },
    { MP_ROM_QSTR(MP_QSTR_arc),         MP_ROM_PTR(&zlcd_arc_obj) },
    { MP_ROM_QSTR(MP_QSTR_pie),         MP_ROM_PTR(&zlcd_pie_obj) },
    { MP_ROM_QSTR(MP_QSTR_ring),        MP_ROM_PTR(&zlcd_ring_obj) },
    { MP_ROM_QSTR(MP_QSTR_bezier),      MP_ROM_PTR(&zlcd_bezier_obj) },
    { MP_ROM_QSTR(MP_QSTR_qbezier),     MP_ROM_PTR(&zlcd_qbezier_obj) },
    { MP_ROM_QSTR(MP_QSTR_gradient),    MP_ROM_PTR(&zlcd_gradient_obj) },
    { MP_ROM_QSTR(MP_QSTR_blit_mono),   MP_ROM_PTR(&zlcd_blit_mono_obj) },
    { MP_ROM_QSTR(MP_QSTR_color565),    MP_ROM_PTR(&zlcd_color565_obj) },
    { MP_ROM_QSTR(MP_QSTR_blend),       MP_ROM_PTR(&zlcd_blend_obj) },
    { MP_ROM_QSTR(MP_QSTR_text),        MP_ROM_PTR(&zlcd_text_obj) },
    { MP_ROM_QSTR(MP_QSTR_text_size),   MP_ROM_PTR(&zlcd_text_size_obj) },
    { MP_ROM_QSTR(MP_QSTR_font_info),   MP_ROM_PTR(&zlcd_font_info_obj) },
    { MP_ROM_QSTR(MP_QSTR_fonts),       MP_ROM_PTR(&zlcd_fonts_obj) },
    { MP_ROM_QSTR(MP_QSTR_FONT_MONO12),   MP_ROM_INT(ZFONT_MONO12) },
    { MP_ROM_QSTR(MP_QSTR_FONT_MONO16),   MP_ROM_INT(ZFONT_MONO16) },
    { MP_ROM_QSTR(MP_QSTR_FONT_SANS16),   MP_ROM_INT(ZFONT_SANS16) },
    { MP_ROM_QSTR(MP_QSTR_FONT_SANS24B),  MP_ROM_INT(ZFONT_SANS24B) },
    { MP_ROM_QSTR(MP_QSTR_FONT_DIGITS48), MP_ROM_INT(ZFONT_DIGITS48) },
};
static MP_DEFINE_CONST_DICT(zlcd_globals, zlcd_globals_table);

const mp_obj_module_t mp_module_lcd = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t *)&zlcd_globals,
};
MP_REGISTER_MODULE(MP_QSTR_lcd, mp_module_lcd);
