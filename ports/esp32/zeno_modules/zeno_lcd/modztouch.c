/*
 * modztouch.c -- "touch" MicroPython module: Zeno OS touch backend.
 *
 * All GPIO/ADC time-critical work happens in touch_drv.c, in C, under the
 * shared bus lock. Python only ever sees already-converted integers/tuples,
 * matching requirement 10 ("Provide an interface that MicroPython can call
 * without requiring Python to perform time-critical GPIO or ADC operations").
 */
#include "py/obj.h"
#include "py/runtime.h"

#include "touch_drv.h"
#include "touch_calib.h"
#include "ili9488_panel.h"
#include "zeno_mp_err.h"

static void require_init(void)
{
    if (!zl_touch_is_init()) {
        mp_raise_msg(&mp_type_OSError, MP_ERROR_TEXT("touch.init() must be called first"));
    }
}

/* touch.init() */
static mp_obj_t ztouch_init(void)
{
    esp_err_t e = zl_touch_init();
    if (e != ESP_OK) {
        zl_raise_esp_err(e);
    }
    zl_calib_load_nvs();   /* best-effort; absent/invalid -> raw passthrough */
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_0(ztouch_init_obj, ztouch_init);

/* touch.raw() -> (rx, ry, z, valid) -- for calibration tooling / debugging */
static mp_obj_t ztouch_raw(void)
{
    require_init();
    zl_touch_raw_t r;
    esp_err_t e = zl_touch_read_raw(&r);
    if (e != ESP_OK) {
        zl_raise_esp_err(e);
    }
    mp_obj_t t[4] = {
        mp_obj_new_int(r.rx), mp_obj_new_int(r.ry),
        mp_obj_new_int(r.z), mp_obj_new_bool(r.valid),
    };
    return mp_obj_new_tuple(4, t);
}
static MP_DEFINE_CONST_FUN_OBJ_0(ztouch_raw_obj, ztouch_raw);

/* touch.read() -> (x, y) in panel coordinates, or None if not touched.
 * Debounced (down_confirm/up_confirm), so a single noisy sample neither
 * registers a phantom tap nor a phantom release -- see zt_debounce_step(). */
static mp_obj_t ztouch_read(void)
{
    require_init();
    int rx, ry;
    if (!zl_touch_poll(&rx, &ry)) {
        return mp_const_none;
    }
    int w = zl_panel_ready() ? zl_panel_width()  : 320;
    int h = zl_panel_ready() ? zl_panel_height() : 480;
    int x, y;
    zl_calib_apply(rx, ry, w, h, &x, &y);
    mp_obj_t t[2] = { mp_obj_new_int(x), mp_obj_new_int(y) };
    return mp_obj_new_tuple(2, t);
}
static MP_DEFINE_CONST_FUN_OBJ_0(ztouch_read_obj, ztouch_read);

/* ---- calibration session: touch.calib_begin()/add_point()/solve()/save() ---- */
static mp_obj_t ztouch_calib_begin(void) { zl_calib_begin(); return mp_const_none; }
static MP_DEFINE_CONST_FUN_OBJ_0(ztouch_calib_begin_obj, ztouch_calib_begin);

/* touch.calib_add_point(raw_x, raw_y, screen_x, screen_y) */
static mp_obj_t ztouch_calib_add_point(size_t n_args, const mp_obj_t *a)
{
    bool ok = zl_calib_add_point(mp_obj_get_int(a[0]), mp_obj_get_int(a[1]),
                                 mp_obj_get_int(a[2]), mp_obj_get_int(a[3]));
    if (!ok) {
        mp_raise_msg(&mp_type_OSError, MP_ERROR_TEXT("too many calibration points"));
    }
    return mp_obj_new_int(zl_calib_count());
}
static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(ztouch_calib_add_point_obj, 4, 4, ztouch_calib_add_point);

/* touch.calib_solve() -> (rms_px, max_err_px); raises if < 3 points or the
 * points are degenerate (e.g. collinear). */
static mp_obj_t ztouch_calib_solve(void)
{
    double rms = 0, me = 0;
    esp_err_t e = zl_calib_solve(&rms, &me);
    if (e != ESP_OK) {
        mp_raise_msg(&mp_type_ValueError,
                     MP_ERROR_TEXT("need >=3 non-collinear points to solve calibration"));
    }
    mp_obj_t t[2] = { mp_obj_new_float(rms), mp_obj_new_float(me) };
    return mp_obj_new_tuple(2, t);
}
static MP_DEFINE_CONST_FUN_OBJ_0(ztouch_calib_solve_obj, ztouch_calib_solve);

static mp_obj_t ztouch_calib_save(void)
{
    esp_err_t e = zl_calib_save_nvs();
    if (e != ESP_OK) {
        zl_raise_esp_err(e);
    }
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_0(ztouch_calib_save_obj, ztouch_calib_save);

static mp_obj_t ztouch_calib_clear(void) { zl_calib_clear(); return mp_const_none; }
static MP_DEFINE_CONST_FUN_OBJ_0(ztouch_calib_clear_obj, ztouch_calib_clear);

static mp_obj_t ztouch_calib_active(void) { return mp_obj_new_bool(zl_calib_active(NULL)); }
static MP_DEFINE_CONST_FUN_OBJ_0(ztouch_calib_active_obj, ztouch_calib_active);

static const mp_rom_map_elem_t ztouch_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__),        MP_ROM_QSTR(MP_QSTR_touch) },
    { MP_ROM_QSTR(MP_QSTR_init),            MP_ROM_PTR(&ztouch_init_obj) },
    { MP_ROM_QSTR(MP_QSTR_raw),             MP_ROM_PTR(&ztouch_raw_obj) },
    { MP_ROM_QSTR(MP_QSTR_read),            MP_ROM_PTR(&ztouch_read_obj) },
    { MP_ROM_QSTR(MP_QSTR_calib_begin),     MP_ROM_PTR(&ztouch_calib_begin_obj) },
    { MP_ROM_QSTR(MP_QSTR_calib_add_point), MP_ROM_PTR(&ztouch_calib_add_point_obj) },
    { MP_ROM_QSTR(MP_QSTR_calib_solve),     MP_ROM_PTR(&ztouch_calib_solve_obj) },
    { MP_ROM_QSTR(MP_QSTR_calib_save),      MP_ROM_PTR(&ztouch_calib_save_obj) },
    { MP_ROM_QSTR(MP_QSTR_calib_clear),     MP_ROM_PTR(&ztouch_calib_clear_obj) },
    { MP_ROM_QSTR(MP_QSTR_calib_active),    MP_ROM_PTR(&ztouch_calib_active_obj) },
};
static MP_DEFINE_CONST_DICT(ztouch_globals, ztouch_globals_table);

const mp_obj_module_t mp_module_touch = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t *)&ztouch_globals,
};
MP_REGISTER_MODULE(MP_QSTR_touch, mp_module_touch);
