/*
 * car_core.h -- hardware-independent core of the `car` racing game.
 *
 * No MicroPython / ESP-IDF includes: physics, traffic, rasteriser and HUD
 * text all live here so they can be unit-tested and rendered on a desktop
 * compiler (see host/). modcar.c is the thin MicroPython/LCD wrapper.
 *
 * Framebuffer format: w*h uint16_t, each pixel an RGB565 value ALREADY
 * BYTE-SWAPPED (high byte first in memory), i.e. exactly what lcd.blit() /
 * zl_blit() put on the 8-bit bus. The buffer can be blitted as-is.
 */
#ifndef CAR_CORE_H
#define CAR_CORE_H

#include <stdint.h>
#include <stdbool.h>

#define CAR_MAX_W        480
#define CAR_MAX_H        320
#define CAR_MIN_W        160
#define CAR_MIN_H        120
#define CAR_MAX_TRAFFIC  6
#define CAR_NUM_STARS    36

/* ---- gameplay tunables (world units: road half-width = 100) ------------- */
#define CAR_ROAD_HALF      100.0f   /* road edge, lane centres at +-50          */
#define CAR_CURB_W          12.0f   /* curb beyond the edge; past it = grass    */
#define CAR_WIDTH           54.0f   /* car width in world units                 */
#define CAR_BARRIER        150.0f   /* |off| beyond this = crash into the barrier */
#define CAR_V_START        520.0f   /* start speed  (world units / s)           */
#define CAR_V_MAX          950.0f   /* top speed                                */
#define CAR_V_OFFROAD_MIN  260.0f   /* off-road drag never slows below this     */

typedef struct {
    bool     active;
    float    d;        /* distance ahead of the player, in z units */
    float    off;      /* lateral position, world units            */
    float    speed;
    uint16_t body;     /* RGB565 (native order) */
} car_traffic_t;

typedef struct car_game {
    int       w, h;
    uint16_t *fb;                 /* w*h, byte-swapped RGB565 */

    /* derived geometry */
    int       horizon, car_y;
    float     road_px;            /* road half-width in px at the bottom row   */
    float     pxu;                /* px per world unit at perspective scale 1  */
    float     scale_p, z_p;       /* perspective scale / depth of player row   */
    float     cs_p;               /* player sprite scale                       */

    /* game state */
    float     pos, speed, off, steer_v, heading, t;
    float     score, best;
    bool      over;
    float     over_t;
    uint32_t  rng;
    car_traffic_t tr[CAR_MAX_TRAFFIC];

    /* pre-computed tables */
    uint16_t  sky[CAR_MAX_H];     /* byte-swapped */
    float     row_z[CAR_MAX_H];
    float     row_s[CAR_MAX_H];
    float     row_cx[CAR_MAX_H];  /* per-frame road centre, px */
    int16_t   star_x[CAR_NUM_STARS], star_y[CAR_NUM_STARS];
} car_game_t;

void car_init(car_game_t *g, int w, int h, uint16_t *fb);
void car_reset(car_game_t *g, uint32_t seed);      /* keeps `best` */
/* dt seconds; steer: -1 left, 0 none, +1 right */
void car_update(car_game_t *g, float dt, int steer);
void car_render(car_game_t *g);                    /* scene + HUD (+ overlay if over) */

#endif
