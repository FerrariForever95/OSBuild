/*
 * racer_core.h -- pure game logic + software rasterizer for "racer".
 *
 * No ESP-IDF / MicroPython dependencies, so it also builds on a desktop
 * compiler (see tests/host_test.c).  Pixels are written as RGB565 and are
 * stored BYTE-SWAPPED (PX() macro) so that a plain uint16_t store on the
 * little-endian ESP32-S3 puts the high byte first, which is what
 * zl_blit()/lcd.blit() expect on the wire.
 *
 * PIE (ESP32-S3 vector unit): the hot loop of the renderer is "fill N pixels
 * with one colour" (grass / curb / road spans).  fill16() does that with
 * 128-bit EE.VST.128.IP stores (8 pixels per instruction) when RC_USE_PIE=1.
 */
#ifndef ZENO_RACER_CORE_H
#define ZENO_RACER_CORE_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

/* ---- PIE switch ---------------------------------------------------------- */
#ifndef RC_USE_PIE
#  if defined(__XTENSA__) && defined(CONFIG_IDF_TARGET_ESP32S3)
#    define RC_USE_PIE 1
#  else
#    define RC_USE_PIE 0
#  endif
#endif

/* ---- Geometry (RC_W must be a multiple of 8, RC_W <= 480) ----------------- *
 * The LCD is bit-banged (no DMA): frame time ~ bytes sent. Smaller window =
 * higher fps. 320x192 = 123 KB per frame.                                    */
#ifndef RC_W
#define RC_W            320
#endif
#ifndef RC_H
#define RC_H            192
#endif
#define RC_ROW_BYTES    (RC_W * 2)

#define RC_D_NEAR       6.0f      /* metres ahead of camera at bottom row      */
#define RC_D_PLAYER     6.06f     /* player car depth (its wheels' row)        */
#define RC_FAR          130.0f    /* draw distance, metres                     */
#define RC_R_MIN        9         /* rows above this are not worth drawing     */
#define RC_ROAD_SLOPE   0.62f     /* road half-width px per row                */
#define RC_CURVE_PX     80        /* horizon shift (px) at curvature 1.0       */
#define RC_MAX_CARS     8
#define RC_POLE_STEP    24.0f

#define RC_CAR_LEN      4.2f      /* collision: |dz| < this (m)                */
#define RC_CAR_W        0.34f     /* collision: |du| < this (road half-widths) */

/* ---- Colours: PX() = RGB565, byte-swapped for the wire --------------------- */
#define RGB565(r,g,b)   ((uint16_t)((((r) & 0xF8) << 8) | (((g) & 0xFC) << 3) | ((b) >> 3)))
#define SW16(x)         ((uint16_t)((((x) & 0xFF) << 8) | (((x) >> 8) & 0xFF)))
#define PX(r,g,b)       SW16(RGB565(r,g,b))

#define C_GRASS     PX( 30,118, 44)
#define C_ROAD_A    PX( 72, 72, 80)
#define C_ROAD_B    PX( 62, 62, 70)
#define C_CURB_A    PX(214, 32, 32)
#define C_CURB_B    PX(238,238,238)
#define C_LINE      PX(240,240,240)
#define C_TIRE      PX( 16, 16, 18)
#define C_SHADOW    PX( 20, 24, 20)
#define C_GLASS     PX( 20, 30, 48)
#define C_RED       PX(255, 20, 20)
#define C_YELLOW    PX(255,224, 40)
#define C_CARBON    PX( 22, 22, 26)
#define C_CHROME    PX(200,200,208)
#define C_GTR       PX( 24,100,250)
#define C_GTR_DK    PX( 14, 56,160)
#define C_POLE      PX(150,150,158)

/* ---- Game state ------------------------------------------------------------ */
typedef struct { float z, u, v; uint8_t col; bool on; } rc_car_t;

enum { RC_RUN = 0, RC_CRASH = 1, RC_OVER = 2 };

typedef struct {
    float z;            /* camera position along road (m)               */
    float u;            /* lateral position, road half-width = 1.0      */
    float v;            /* speed (m/s)                                  */
    float steer;        /* smoothed steering -1..1                      */
    float cur;          /* road curvature at the horizon, ~ -1..1       */
    float t;            /* seconds survived                             */
    float dist;         /* metres driven                                */
    float crash_t;
    float next_spawn_z;
    int   state;
    bool  offroad;
    uint32_t rng;
    rc_car_t cars[RC_MAX_CARS];
} rc_game_t;

/* ---- Fills ------------------------------------------------------------------ */
static inline void rc_fill16(uint16_t *p, uint16_t c, int n)
{
#if RC_USE_PIE
    while (n > 0 && ((uintptr_t)p & 15u)) { *p++ = c; n--; }
    int blocks = n >> 3;                      /* 8 px = 16 bytes */
    n -= blocks << 3;
    uint16_t cv = c;
    while (blocks >= 4) {                     /* 64 bytes per asm block */
        __asm__ volatile(
            "ee.vldbc.16   q0, %1        \n"
            "ee.vst.128.ip q0, %0, 16    \n"
            "ee.vst.128.ip q0, %0, 16    \n"
            "ee.vst.128.ip q0, %0, 16    \n"
            "ee.vst.128.ip q0, %0, 16    \n"
            : "+r"(p) : "r"(&cv) : "memory");
        blocks -= 4;
    }
    while (blocks-- > 0) {
        __asm__ volatile(
            "ee.vldbc.16   q0, %1        \n"
            "ee.vst.128.ip q0, %0, 16    \n"
            : "+r"(p) : "r"(&cv) : "memory");
    }
    while (n-- > 0) *p++ = c;
#else
    if (n > 0 && ((uintptr_t)p & 2u)) { *p++ = c; n--; }
    uint32_t w = ((uint32_t)c << 16) | c;
    uint32_t *q = (uint32_t *)p;
    int pairs = n >> 1;
    while (pairs-- > 0) *q++ = w;
    if (n & 1) *(uint16_t *)q = c;
#endif
}

static inline uint16_t *rc_row(uint8_t *buf, int y) { return (uint16_t *)(buf + (size_t)y * RC_ROW_BYTES); }

static inline void rc_span(uint16_t *row, int x0, int x1, uint16_t c)   /* [x0,x1) */
{
    if (x0 < 0) x0 = 0;
    if (x1 > RC_W) x1 = RC_W;
    if (x1 > x0) rc_fill16(row + x0, c, x1 - x0);
}

static void rc_rect(uint8_t *buf, int x, int y, int w, int h, uint16_t c)
{
    if (w <= 0 || h <= 0) return;
    int y1 = y + h;
    if (y < 0) y = 0;
    if (y1 > RC_H) y1 = RC_H;
    for (; y < y1; y++) rc_span(rc_row(buf, y), x, x + w, c);
}

static void rc_trap(uint8_t *buf, int cx, int y0, int y1, int wt, int wb, uint16_t c)
{
    int dy = y1 - y0;
    if (dy <= 0) return;
    for (int y = y0; y < y1; y++) {
        if (y < 0 || y >= RC_H) continue;
        int w = wt + ((wb - wt) * (y - y0)) / dy;
        rc_span(rc_row(buf, y), cx - w / 2, cx - w / 2 + w, c);
    }
}

/* ---- Projection tables ------------------------------------------------------ */
static int16_t  rc_half[RC_H], rc_curb[RC_H], rc_line[RC_H], rc_cs[RC_H];
static uint32_t rc_depthq[RC_H];   /* depth * 4 */
static float    rc_depth[RC_H];
static bool     rc_tables_ready;

static void rc_init_tables(void)
{
    if (rc_tables_ready) return;
    for (int r = 1; r < RC_H; r++) {
        float d = ((float)(RC_H - 1) * RC_D_NEAR) / (float)r;
        rc_depth[r]  = d;
        rc_depthq[r] = (uint32_t)(d * 4.0f);
        int half = (int)((float)r * RC_ROAD_SLOPE);
        rc_half[r] = (int16_t)(half < 1 ? 1 : half);
        int curb = (int)((float)r * 0.075f);
        rc_curb[r] = (int16_t)(curb < 1 ? 1 : curb);
        rc_line[r] = (int16_t)(r / 90);
        float n = d / RC_FAR;
        if (n > 1.0f) n = 1.0f;
        rc_cs[r] = (int16_t)(n * n * 1024.0f);
    }
    rc_depth[0] = 1e9f; rc_depthq[0] = 0xFFFFFF; rc_cs[0] = 1024;
    rc_tables_ready = true;
}

/* ---- RNG -------------------------------------------------------------------- */
static inline uint32_t rc_rand(rc_game_t *g)
{
    uint32_t x = g->rng;
    x ^= x << 13; x ^= x >> 17; x ^= x << 5;
    return g->rng = x;
}

static void rc_reset(rc_game_t *g, uint32_t seed)
{
    memset(g, 0, sizeof(*g));
    g->rng = seed ? seed : 0x1234567u;
    g->next_spawn_z = 90.0f;
    g->state = RC_RUN;
    rc_init_tables();
}

static inline float rc_curve_at(float z, float t)
{
    float amp = 0.45f + t / 110.0f;
    if (amp > 1.0f) amp = 1.0f;
    return amp * (0.62f * sinf(z / 260.0f + 0.5f) + 0.38f * sinf(z / 97.0f + 2.1f));
}

/* ---- Update ----------------------------------------------------------------- */
static void rc_spawn(rc_game_t *g)
{
    float sz = g->z + 122.0f;
    /* which lanes already hold a car near the spawn point? */
    int occ = 0, nocc = 0;
    static const float lane_u[3] = { -0.667f, 0.0f, 0.667f };
    for (int i = 0; i < RC_MAX_CARS; i++) {
        if (!g->cars[i].on) continue;
        if (fabsf(g->cars[i].z - sz) < 48.0f) {
            for (int l = 0; l < 3; l++)
                if (fabsf(g->cars[i].u - lane_u[l]) < 0.2f && !(occ & (1 << l))) { occ |= 1 << l; nocc++; }
        }
    }
    float gap_min = 18.0f + 0.9f * g->v;
    if (nocc >= 2) { g->next_spawn_z = g->z + 122.0f + 8.0f; return; }   /* keep one lane open */
    int free_l[3], nf = 0;
    for (int l = 0; l < 3; l++) if (!(occ & (1 << l))) free_l[nf++] = l;
    int lane = free_l[rc_rand(g) % (uint32_t)nf];
    for (int i = 0; i < RC_MAX_CARS; i++) {
        if (g->cars[i].on) continue;
        g->cars[i].on  = true;
        g->cars[i].z   = sz;
        g->cars[i].u   = lane_u[lane];
        g->cars[i].v   = 12.0f + (float)(rc_rand(g) % 1300u) * 0.01f;   /* 12..25 m/s */
        g->cars[i].col = (uint8_t)(rc_rand(g) % 6u);
        break;
    }
    g->next_spawn_z = sz + gap_min * (1.0f + (float)(rc_rand(g) % 80u) * 0.01f);
}

/* steer_in: -1 (left), 0, +1 (right) */
static void rc_update(rc_game_t *g, float dt, int steer_in)
{
    if (g->state == RC_OVER) return;
    if (dt > 0.05f) dt = 0.05f;

    if (g->state == RC_CRASH) {
        g->v -= 45.0f * dt; if (g->v < 0) g->v = 0;
        g->steer = 0;
        g->crash_t += dt;
        g->z += g->v * dt;
        for (int i = 0; i < RC_MAX_CARS; i++) if (g->cars[i].on) g->cars[i].z += g->cars[i].v * dt;
        if (g->crash_t > 1.3f) g->state = RC_OVER;
        return;
    }

    g->t += dt;

    /* auto accelerate toward a cap that rises with time */
    float vcap = 30.0f + 0.35f * g->t;
    if (vcap > 80.0f) vcap = 80.0f;
    g->offroad = fabsf(g->u) > 1.12f;
    if (g->offroad) {
        g->v -= 32.0f * dt;
        if (g->v < 14.0f) g->v = 14.0f;
    } else if (g->v < vcap) {
        g->v += (7.0f + 6.0f * (1.0f - g->v / vcap)) * dt;
    } else {
        g->v -= 6.0f * dt;
    }

    /* steering: smoothed, authority grows with speed */
    float k = 9.0f * dt; if (k > 1.0f) k = 1.0f;
    g->steer += ((float)steer_in - g->steer) * k;
    float auth = g->v / 10.0f; if (auth > 1.0f) auth = 1.0f;
    g->u += g->steer * 1.8f * auth * dt;
    /* the curve pushes you to the outside */
    g->u -= g->cur * (g->v / 45.0f) * 0.65f * dt;
    if (g->u >  1.8f) g->u =  1.8f;
    if (g->u < -1.8f) g->u = -1.8f;

    g->z += g->v * dt;
    g->dist += g->v * dt;
    g->cur = rc_curve_at(g->z + 60.0f, g->t);

    /* traffic */
    float pz = g->z + RC_D_PLAYER;
    for (int i = 0; i < RC_MAX_CARS; i++) {
        rc_car_t *c = &g->cars[i];
        if (!c->on) continue;
        c->z += c->v * dt;
        if (c->z - g->z < -12.0f) { c->on = false; continue; }
        if (fabsf(c->z - pz) < RC_CAR_LEN && fabsf(c->u - g->u) < RC_CAR_W) {
            g->state = RC_CRASH;
            g->crash_t = 0;
        }
    }
    if (g->z >= g->next_spawn_z - 122.0f + 122.0f - 122.0f && g->z + 122.0f >= g->next_spawn_z)
        rc_spawn(g);
}

/* ---- Rendering ---------------------------------------------------------------- */
static inline int rc_x_at(int r, float u, int cu_q, int cv_q)
{
    int uq = (int)(u * 256.0f);
    return RC_W / 2 + ((cv_q * rc_cs[r] * RC_CURVE_PX) >> 18) + (((uq - cu_q) * rc_half[r]) >> 8);
}

static void rc_render_track(uint8_t *buf, const rc_game_t *g)
{
    const int cu_q = (int)(g->u * 256.0f);
    const int cv_q = (int)(g->cur * 256.0f);
    const uint32_t pos_q = (uint32_t)(g->z * 4.0f);

    rc_fill16(rc_row(buf, 0), C_GRASS, RC_W);
    for (int r = 1; r < RC_H; r++) {
        uint16_t *row = rc_row(buf, r);
        const int half = rc_half[r], curb = rc_curb[r];
        const int c = RC_W / 2 + ((cv_q * rc_cs[r] * RC_CURVE_PX) >> 18) - ((cu_q * half) >> 8);
        const uint32_t wq = rc_depthq[r] + pos_q;
        const int seg = (int)((wq >> 5) & 1u);
        const int rl = c - half, rr = c + half;

        rc_fill16(row, C_GRASS, RC_W);
        rc_span(row, rl - curb, rl, seg ? C_CURB_A : C_CURB_B);
        rc_span(row, rr, rr + curb,  seg ? C_CURB_A : C_CURB_B);
        rc_span(row, rl, rr, seg ? C_ROAD_A : C_ROAD_B);
        if (((wq >> 4) & 1u) == 0) {                 /* dashed lane dividers */
            int d3 = half / 3, lw = rc_line[r];
            rc_span(row, c - d3 - lw, c - d3 + lw + 1, C_LINE);
            rc_span(row, c + d3 - lw, c + d3 + lw + 1, C_LINE);
        }
    }
}

static void rc_draw_pole(uint8_t *buf, int r, int x)
{
    int h = (r * 3) / 10; if (h < 2) h = 2;
    int w = r / 40 + 1;
    rc_rect(buf, x - w / 2, r - h, w, h, C_POLE);
    rc_rect(buf, x - w / 2, r - h, w, (h > 6 ? 2 : 1), C_RED);
}

static void rc_draw_traffic(uint8_t *buf, int cx, int rb, int w, uint8_t col)
{
    static const uint16_t body[6] = {
        PX(230,230,235), PX(220,170,30), PX(40,170,90), PX(150,40,160), PX(240,110,30), PX(160,170,180) };
    int h = (w * 3) / 4; if (h < 3) h = 3;
    int x0 = cx - w / 2, y0 = rb - h;
    uint16_t b = body[col % 6];
    int tw = w / 6 > 1 ? w / 6 : 1, wh = h / 5 > 1 ? h / 5 : 1;

    rc_rect(buf, x0 - 1, rb - 1, w + 2, 2, C_SHADOW);
    rc_rect(buf, x0, rb - wh, tw, wh, C_TIRE);
    rc_rect(buf, x0 + w - tw, rb - wh, tw, wh, C_TIRE);
    rc_rect(buf, x0, y0 + (h * 4) / 10, w, (h * 6) / 10 - 1, b);          /* body  */
    rc_rect(buf, x0 + w / 6, y0, (w * 4) / 6, (h * 5) / 10, b);           /* cabin */
    rc_rect(buf, x0 + w / 6 + 1, y0 + h / 10, (w * 4) / 6 - 2, h / 4, C_GLASS);
    int lh = h / 7 > 1 ? h / 7 : 1;
    rc_rect(buf, x0 + 1, y0 + (h * 5) / 10, w / 5 > 1 ? w / 5 : 1, lh, C_RED);
    rc_rect(buf, x0 + w - 1 - (w / 5 > 1 ? w / 5 : 1), y0 + (h * 5) / 10, w / 5 > 1 ? w / 5 : 1, lh, C_RED);
}

static void rc_draw_player(uint8_t *buf, const rc_game_t *g)
{
    int lean = (int)(g->steer * 5.0f);
    int bob  = (g->v > 5.0f) ? (int)(sinf(g->z * 0.35f) * 1.2f) : 0;
    int jit  = 0;
    if (g->state == RC_CRASH) jit = (int)((int32_t)((uint32_t)(g->crash_t * 9000.0f) % 7u) - 3);
    else if (g->offroad)      jit = (int)(sinf(g->z * 2.1f) * 2.0f);
    const int cx = RC_W / 2 + jit;
    const int by = RC_H - 3 + bob;
    const int cc = cx + lean;

    rc_rect(buf, cx - 31, by - 3, 62, 5, C_SHADOW);
    rc_rect(buf, cx - 29, by - 14, 9, 14, C_TIRE);
    rc_rect(buf, cx + 20, by - 14, 9, 14, C_TIRE);
    rc_rect(buf, cx - 22, by - 8, 44, 7, C_CARBON);                      /* diffuser   */
    rc_rect(buf, cx - 20, by - 7, 5, 4, C_CHROME);                       /* 4 exhausts */
    rc_rect(buf, cx - 13, by - 7, 5, 4, C_CHROME);
    rc_rect(buf, cx +  8, by - 7, 5, 4, C_CHROME);
    rc_rect(buf, cx + 15, by - 7, 5, 4, C_CHROME);
    rc_rect(buf, cx - 18, by - 6, 3, 2, C_TIRE);
    rc_rect(buf, cx - 11, by - 6, 3, 2, C_TIRE);
    rc_rect(buf, cx + 10, by - 6, 3, 2, C_TIRE);
    rc_rect(buf, cx + 17, by - 6, 3, 2, C_TIRE);
    rc_rect(buf, cx - 26, by - 18, 52, 11, C_GTR_DK);                    /* bumper     */
    rc_rect(buf, cx - 26, by - 27, 52, 9, C_GTR);                        /* trunk/body */
    rc_rect(buf, cx - 9, by - 17, 18, 6, C_CARBON);                      /* plate      */
    rc_rect(buf, cx - 7, by - 16, 14, 4, C_CHROME);
    /* four round taillights (outer big, inner small) */
    rc_rect(buf, cx - 25, by - 25, 7, 7, C_RED);  rc_rect(buf, cx - 23, by - 23, 3, 3, C_YELLOW);
    rc_rect(buf, cx - 15, by - 24, 5, 5, C_RED);  rc_rect(buf, cx - 14, by - 23, 2, 2, C_YELLOW);
    rc_rect(buf, cx + 10, by - 24, 5, 5, C_RED);  rc_rect(buf, cx + 11, by - 23, 2, 2, C_YELLOW);
    rc_rect(buf, cx + 18, by - 25, 7, 7, C_RED);  rc_rect(buf, cx + 20, by - 23, 3, 3, C_YELLOW);
    rc_trap(buf, cc, by - 41, by - 27, 32, 44, C_GTR);                   /* greenhouse */
    rc_trap(buf, cc, by - 39, by - 29, 24, 36, C_GLASS);
    rc_rect(buf, cc - 24 + (lean >> 1), by - 35, 48, 3, C_CARBON);       /* spoiler    */
    rc_rect(buf, cc - 16 + (lean >> 1), by - 32, 3, 5, C_CARBON);
    rc_rect(buf, cc + 13 + (lean >> 1), by - 32, 3, 5, C_CARBON);
}

static void rc_render(uint8_t *buf, const rc_game_t *g)
{
    rc_render_track(buf, g);

    const int cu_q = (int)(g->u * 256.0f);
    const int cv_q = (int)(g->cur * 256.0f);

    /* roadside poles, far -> near */
    int k0 = (int)(g->z / RC_POLE_STEP) + 1;
    int kmax = k0 + (int)(RC_FAR / RC_POLE_STEP) + 1;
    for (int k = kmax; k >= k0; k--) {
        float d = (float)k * RC_POLE_STEP - g->z;
        if (d < RC_D_NEAR || d > RC_FAR) continue;
        int r = (int)(((float)(RC_H - 1) * RC_D_NEAR) / d);
        if (r < RC_R_MIN || r >= RC_H) continue;
        rc_draw_pole(buf, r, rc_x_at(r, -1.45f, cu_q, cv_q));
        rc_draw_pole(buf, r, rc_x_at(r,  1.45f, cu_q, cv_q));
    }

    /* traffic, far -> near (insertion sort on depth) */
    int idx[RC_MAX_CARS]; float dep[RC_MAX_CARS]; int n = 0;
    for (int i = 0; i < RC_MAX_CARS; i++) {
        if (!g->cars[i].on) continue;
        float d = g->cars[i].z - g->z;
        if (d < RC_D_NEAR * 0.9f || d > RC_FAR) continue;
        int j = n++;
        while (j > 0 && dep[j - 1] < d) { dep[j] = dep[j - 1]; idx[j] = idx[j - 1]; j--; }
        dep[j] = d; idx[j] = i;
    }
    for (int s = 0; s < n; s++) {
        const rc_car_t *c = &g->cars[idx[s]];
        int r = (int)(((float)(RC_H - 1) * RC_D_NEAR) / dep[s]);
        if (r < RC_R_MIN) continue;
        if (r > RC_H - 1) r = RC_H - 1;
        int w = (rc_half[r] * 88) >> 8;              /* 0.34 of a half-width */
        if (w < 3) w = 3;
        rc_draw_traffic(buf, rc_x_at(r, c->u, cu_q, cv_q), r, w, c->col);
    }

    rc_draw_player(buf, g);
}

#endif /* ZENO_RACER_CORE_H */ 
