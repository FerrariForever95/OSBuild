/*
 * modcar.c -- `car` MicroPython module: a touch-steered endless racer.
 *
 *   import car
 *   car.run(400, 300, 60)      # window width, height, FPS cap
 *
 * Built on the zeno_lcd driver (lcd_bus / ili9488_panel / touch_drv), so the
 * `lcd` and `touch` modules must be in the same firmware. The game core
 * (car_core.c) renders into one off-screen RGB565 buffer and each frame is
 * pushed to the panel with a single zl_blit().
 *
 * Controls: touch the LEFT half of the display to steer left, the RIGHT half
 * to steer right. The car accelerates by itself. Hit traffic or the roadside
 * barrier and the run ends; touch the screen to retry. Ctrl-C returns to the
 * REPL.
 */
#include <stdio.h>
#include <string.h>

#include "py/runtime.h"
#include "py/obj.h"
#include "py/mphal.h"
#include "py/nlr.h"

#include "esp_heap_caps.h"
#include "esp_timer.h"
#include "esp_rom_sys.h"

#include "lcd_bus.h"
#include "ili9488_panel.h"
#include "touch_drv.h"
#include "touch_calib.h"
#include "zeno_mp_err.h"
#include "car_core.h"

#define DEFAULT_W    400
#define DEFAULT_H    300
#define DEFAULT_FPS  60
#define ROT_LANDSCAPE 1          /* zl_madctl_table[1] = 480x320 */

static float s_last_fps = 0.0f;
static float s_best_m   = 0.0f;

static void delay_ms_wrapper(uint32_t ms) { mp_hal_delay_ms((mp_uint_t)ms); }

/* One frame onto the panel. Returns false if the bus was busy (frame skipped). */
static bool present(int wx, int wy, const car_game_t *g)
{
    if (!zl_bus_lock()) {
        return false;
    }
    zl_blit(wx, wy, g->w, g->h, (const uint8_t *)g->fb);
    zl_bus_unlock();
    return true;
}

/* Touch -> steering. Returns true while touched; *steer = -1 / +1 by screen half. */
static bool read_steer(int screen_w, int screen_h, int *steer)
{
    int rx, ry;
    if (!zl_touch_poll(&rx, &ry)) {
        *steer = 0;
        return false;
    }
    int x, y;
    zl_calib_apply(rx, ry, screen_w, screen_h, &x, &y);
    *steer = (x < screen_w / 2) ? -1 : 1;
    return true;
}

static void game_loop(car_game_t *g, int wx, int wy, int screen_w, int screen_h, int fps)
{
    const int64_t frame_us = 1000000 / fps;
    uint32_t seed = (uint32_t)esp_timer_get_time() | 1u;

    car_reset(g, seed);

    int64_t last = esp_timer_get_time();
    int64_t fps_t0 = last;
    int fps_frames = 0;
    bool released_since_over = false;
    int over_drawn = 0;            /* 0 = scene live, 1 = overlay shown, 2 = retry prompt shown */

    for (;;) {
        int64_t t0 = esp_timer_get_time();
        mp_handle_pending(true);                       /* Ctrl-C -> KeyboardInterrupt */

        int steer = 0;
        bool touched = read_steer(screen_w, screen_h, &steer);

        float dt = (float)(t0 - last) * 1e-6f;
        last = t0;
        if (dt < 0.001f) dt = 0.001f;
        if (dt > 0.1f) dt = 0.1f;

        if (!g->over) {
            car_update(g, dt, steer);
            if (g->over) {
                released_since_over = false;
                over_drawn = 0;
                if (g->best > s_best_m) s_best_m = g->best;
            }
        } else {
            car_update(g, dt, 0);                      /* advances over_t */
            if (!touched) released_since_over = true;
            if (touched && released_since_over && g->over_t > 0.8f) {
                car_reset(g, (uint32_t)esp_timer_get_time() | 1u);
                last = esp_timer_get_time();
                over_drawn = 0;
            }
        }

        /* A finished run is static: draw the overlay once, then once more when the
         * "touch to retry" prompt appears, and otherwise only poll the touch. */
        bool draw = true;
        if (g->over) {
            if (over_drawn == 0) over_drawn = 1;
            else if (over_drawn == 1 && g->over_t > 0.8f) over_drawn = 2;
            else draw = false;
        }
        if (draw) {
            car_render(g);
            present(wx, wy, g);
        }

        /* frame limiter: sleep (letting other tasks run), then spin the remainder */
        int64_t target = g->over && !draw ? 20000 : frame_us;
        int64_t elapsed = esp_timer_get_time() - t0;
        if (elapsed < target) {
            int64_t rem = target - elapsed;
            if (rem > 2500) {
                mp_hal_delay_ms((mp_uint_t)((rem - 1500) / 1000));
            }
            elapsed = esp_timer_get_time() - t0;
            if (elapsed < target) {
                esp_rom_delay_us((uint32_t)(target - elapsed));
            }
        }

        if (!g->over && ++fps_frames >= 20) {
            int64_t now = esp_timer_get_time();
            if (now > fps_t0) s_last_fps = (float)fps_frames * 1e6f / (float)(now - fps_t0);
            fps_t0 = now;
            fps_frames = 0;
        }
    }
}

/* car.run(width=400, height=300, fps=60) */
static mp_obj_t car_run(size_t n_args, const mp_obj_t *pos, mp_map_t *kw)
{
    enum { ARG_width, ARG_height, ARG_fps };
    static const mp_arg_t allowed[] = {
        { MP_QSTR_width,  MP_ARG_INT, {.u_int = DEFAULT_W} },
        { MP_QSTR_height, MP_ARG_INT, {.u_int = DEFAULT_H} },
        { MP_QSTR_fps,    MP_ARG_INT, {.u_int = DEFAULT_FPS} },
    };
    mp_arg_val_t a[MP_ARRAY_SIZE(allowed)];
    mp_arg_parse_all(n_args, pos, kw, MP_ARRAY_SIZE(allowed), allowed, a);

    int w = a[ARG_width].u_int, h = a[ARG_height].u_int, fps = a[ARG_fps].u_int;
    if (fps < 1) fps = 1;
    if (fps > 120) fps = 120;

    /* ---- display: bring it up in landscape if the caller has not ---- */
    if (!zl_panel_ready()) {
        zl_panel_set_delay_fn(delay_ms_wrapper);
        esp_err_t e = zl_bus_init();
        if (e != ESP_OK) zl_raise_esp_err(e);
        e = zl_panel_init(ROT_LANDSCAPE, -1);
        if (e != ESP_OK) zl_raise_esp_err(e);
    } else if ((w > (int)zl_panel_width() || h > (int)zl_panel_height()) &&
               w <= 480 && h <= 320) {
        /* panel was left in portrait; the window only fits in landscape */
        esp_err_t e = zl_panel_set_rotation(ROT_LANDSCAPE, -1);
        if (e != ESP_OK) zl_raise_esp_err(e);
    }

    int sw = zl_panel_width(), sh = zl_panel_height();
    if (w < CAR_MIN_W || h < CAR_MIN_H || w > CAR_MAX_W || h > CAR_MAX_H || w > sw || h > sh) {
        mp_raise_msg_varg(&mp_type_ValueError,
            MP_ERROR_TEXT("window must be %dx%d .. %dx%d (screen is %dx%d)"),
            CAR_MIN_W, CAR_MIN_H, sw < CAR_MAX_W ? sw : CAR_MAX_W, sh < CAR_MAX_H ? sh : CAR_MAX_H, sw, sh);
    }
    w &= ~1;                                   /* even width keeps rows 32-bit aligned */

    /* ---- touch ---- */
    esp_err_t e = zl_touch_init();
    if (e != ESP_OK) zl_raise_esp_err(e);
    if (!zl_calib_active(NULL)) zl_calib_load_nvs();   /* best effort */

    /* ---- memory: frame buffer in PSRAM (bus is bit-banged, no DMA needed) ---- */
    size_t fb_bytes = (size_t)w * (size_t)h * 2u;
    uint16_t *fb = heap_caps_malloc(fb_bytes, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    if (fb == NULL) fb = heap_caps_malloc(fb_bytes, MALLOC_CAP_8BIT);
    car_game_t *g = heap_caps_malloc(sizeof(car_game_t), MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
    if (g == NULL) g = heap_caps_malloc(sizeof(car_game_t), MALLOC_CAP_8BIT);
    if (fb == NULL || g == NULL) {
        heap_caps_free(fb);
        heap_caps_free(g);
        mp_raise_msg(&mp_type_MemoryError, MP_ERROR_TEXT("car: cannot allocate frame buffer"));
    }
    car_init(g, w, h, fb);

    int wx = (sw - w) / 2, wy = (sh - h) / 2;
    if (zl_bus_lock()) {                       /* black borders around the window */
        zl_fill_screen(0x0000);
        zl_bus_unlock();
    }

    nlr_buf_t nlr;
    if (nlr_push(&nlr) == 0) {
        game_loop(g, wx, wy, sw, sh, fps);     /* never returns normally */
        nlr_pop();
    } else {
        heap_caps_free(fb);                    /* Ctrl-C or any exception: clean up */
        heap_caps_free(g);
        nlr_jump(nlr.ret_val);
    }
    heap_caps_free(fb);
    heap_caps_free(g);
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_KW(car_run_obj, 0, car_run);

/* car.stats() -> (measured_fps, best_distance_m) from the most recent run */
static mp_obj_t car_stats(void)
{
    mp_obj_t t[2] = { mp_obj_new_float(s_last_fps), mp_obj_new_int((mp_int_t)s_best_m) };
    return mp_obj_new_tuple(2, t);
}
static MP_DEFINE_CONST_FUN_OBJ_0(car_stats_obj, car_stats);

static const mp_rom_map_elem_t car_module_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_car) },
    { MP_ROM_QSTR(MP_QSTR_run),      MP_ROM_PTR(&car_run_obj) },
    { MP_ROM_QSTR(MP_QSTR_stats),    MP_ROM_PTR(&car_stats_obj) },
};
static MP_DEFINE_CONST_DICT(car_module_globals, car_module_globals_table);

const mp_obj_module_t car_user_cmodule = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t *)&car_module_globals,
};

MP_REGISTER_MODULE(MP_QSTR_car, car_user_cmodule);
