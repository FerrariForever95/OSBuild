/*
 * car_core.c -- see car_core.h.
 *
 * Projection: flat-ground pseudo-3D. For a screen row y below the horizon,
 *   s = (y + .5 - horizon) / (h - horizon)      perspective scale, 0..1
 *   z = 10 / s                                  depth (same units as the
 *                                               original highway demo)
 * Anything at depth z sits on row  horizon + (10/z) * (h - horizon), and a
 * lateral offset of `off` world units is `off * pxu * s` pixels from the
 * road centre. Road bends are an integrated curvature profile (OutRun style)
 * so corners really scroll towards the player.
 */
#include "car_core.h"
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "zeno_font.h"

/* ------------------------------------------------------------------------ */
/* tuning                                                                   */
/* ------------------------------------------------------------------------ */
#define ACC_BASE        18.0f
#define ACC_K           0.05f
#define OFFROAD_DECEL   620.0f
#define LAT_SPEED       300.0f    /* world units / s at full steer            */
#define STEER_RESP      10.0f     /* steer smoothing (1/s)                    */
#define CURVE_PUSH      115.0f    /* outward drift at curvature 1, top speed  */
#define CURVE_K         0.0167f
#define TRAFFIC_V_MIN   220.0f
#define TRAFFIC_V_MAX   380.0f
#define SPAWN_D         950.0f
#define REACT_TIME      1.15f     /* s of spacing between cars                */
#define HIT_X           CAR_WIDTH
#define HIT_NEAR        1.5f
#define HIT_FAR         1.0f
#define CAR_DESIGN_W    100.0f    /* width of the sprite below, design px     */
#define FOG_Z           340.0f    /* beyond this, stripes fade to a flat tone */

#define BE(c) ((uint16_t)((((c) & 0xFF) << 8) | (((c) >> 8) & 0xFF)))
#define RGB5(r, g, b) ((uint16_t)((((r) & 31) << 11) | (((g) & 63) << 5) | ((b) & 31)))

static inline float clampf(float v, float lo, float hi) { return v < lo ? lo : (v > hi ? hi : v); }

static uint32_t rnd(car_game_t *g)
{
    uint32_t x = g->rng;
    x ^= x << 13; x ^= x >> 17; x ^= x << 5;
    g->rng = x ? x : 0x1234567u;
    return g->rng;
}
static float rndf(car_game_t *g) { return (float)(rnd(g) & 0xFFFFFF) / 16777216.0f; }

/* ------------------------------------------------------------------------ */
/* colour helpers                                                           */
/* ------------------------------------------------------------------------ */
static uint16_t blend565(uint16_t bg, uint16_t fg, uint32_t a /*0..255*/)
{
    uint32_t ia = 255u - a;
    uint32_t r = (((bg >> 11) & 31u) * ia + ((fg >> 11) & 31u) * a + 127u) / 255u;
    uint32_t gg = (((bg >> 5) & 63u) * ia + ((fg >> 5) & 63u) * a + 127u) / 255u;
    uint32_t b = ((bg & 31u) * ia + (fg & 31u) * a + 127u) / 255u;
    return (uint16_t)((r << 11) | (gg << 5) | b);
}
static inline uint16_t darken(uint16_t c) { return (uint16_t)((c >> 1) & 0x7BEF); }
static inline uint16_t swap16(uint16_t v) { return (uint16_t)((v << 8) | (v >> 8)); }

typedef uint32_t __attribute__((may_alias)) u32a;   /* fb is also touched as uint16_t */

/* ------------------------------------------------------------------------ */
/* low-level fills (all clip to the framebuffer)                            */
/* ------------------------------------------------------------------------ */
static void span(car_game_t *g, int y, int x0, int x1, uint16_t be)
{
    if (y < 0 || y >= g->h) return;
    if (x0 < 0) x0 = 0;
    if (x1 > g->w) x1 = g->w;
    if (x0 >= x1) return;
    uint16_t *p = g->fb + (size_t)y * g->w + x0;
    int n = x1 - x0;
    if (((uintptr_t)p & 3u) && n) { *p++ = be; n--; }
    uint32_t v = ((uint32_t)be << 16) | be;
    u32a *q = (u32a *)p;
    for (int i = n >> 1; i > 0; i--) *q++ = v;
    if (n & 1) *(uint16_t *)q = be;
}

static void fill_rect(car_game_t *g, int x, int y, int w, int h, uint16_t native)
{
    uint16_t be = swap16(native);
    for (int j = 0; j < h; j++) span(g, y + j, x, x + w, be);
}

/* ------------------------------------------------------------------------ */
/* road curvature profile                                                   */
/* ------------------------------------------------------------------------ */
static float curvature(float wz)
{
    return sinf(wz * 0.0011f) + 0.55f * sinf(wz * 0.0029f + 1.7f);
}

/* ------------------------------------------------------------------------ */
/* init / reset                                                             */
/* ------------------------------------------------------------------------ */
void car_init(car_game_t *g, int w, int h, uint16_t *fb)
{
    memset(g, 0, sizeof(*g));
    g->w = w; g->h = h; g->fb = fb;
    g->horizon = (int)(h * 0.40f);
    g->car_y   = (int)(h * 0.87f);
    g->road_px = 0.50f * (float)w;
    g->pxu     = g->road_px / CAR_ROAD_HALF;

    float span_h = (float)(h - g->horizon);
    for (int y = g->horizon + 1; y < h; y++) {
        float s = ((float)y + 0.5f - (float)g->horizon) / span_h;
        g->row_s[y] = s;
        g->row_z[y] = 10.0f / s;
    }
    g->scale_p = ((float)g->car_y + 0.5f - (float)g->horizon) / span_h;
    g->z_p     = 10.0f / g->scale_p;
    g->cs_p    = CAR_WIDTH * g->pxu * g->scale_p / CAR_DESIGN_W;

    /* dusk sky: navy -> purple -> orange glow at the horizon */
    for (int y = 0; y <= g->horizon; y++) {
        float t = (float)y / (float)g->horizon;
        float r, gg, b;
        if (t < 0.65f) {
            float u = t / 0.65f;
            r = 2 + u * 12;  gg = 5 + u * 10;  b = 10 + u * 10;
        } else {
            float u = (t - 0.65f) / 0.35f;
            r = 14 + u * 16; gg = 15 + u * 22; b = 20 - u * 8;
        }
        g->sky[y] = BE(RGB5((int)r, (int)gg, (int)b));
    }
    g->rng = 0xC0FFEEu;
    for (int i = 0; i < CAR_NUM_STARS; i++) {
        g->star_x[i] = (int16_t)(rnd(g) % (uint32_t)w);
        g->star_y[i] = (int16_t)(rnd(g) % (uint32_t)(g->horizon * 6 / 10 + 1));
    }
}

void car_reset(car_game_t *g, uint32_t seed)
{
    g->pos = 0; g->speed = CAR_V_START; g->off = -CAR_ROAD_HALF * 0.5f;
    g->steer_v = 0; g->heading = 0; g->t = 0; g->score = 0;
    g->over = false; g->over_t = 0;
    g->rng = seed ? seed : 0xA5A5A5u;
    memset(g->tr, 0, sizeof(g->tr));
}

/* ------------------------------------------------------------------------ */
/* simulation                                                               */
/* ------------------------------------------------------------------------ */
static void spawn_traffic(car_game_t *g)
{
    float far_d = -1e9f;
    int free_slot = -1;
    for (int i = 0; i < CAR_MAX_TRAFFIC; i++) {
        if (!g->tr[i].active) { if (free_slot < 0) free_slot = i; }
        else if (g->tr[i].d > far_d) far_d = g->tr[i].d;
    }
    if (free_slot < 0) return;

    float rel = g->speed - 300.0f;
    if (rel < 120.0f) rel = 120.0f;
    float gap = rel * REACT_TIME * (1.0f + 0.7f * rndf(g));
    if (far_d > SPAWN_D - gap) return;

    static const uint16_t colours[] = {
        0xFFFF, 0xF800, 0xFFE0, 0x07E0, 0x07FF, 0xFD20, 0xF81F, 0x9CD3,
    };
    car_traffic_t *t = &g->tr[free_slot];
    t->active = true;
    t->d      = SPAWN_D;
    t->off    = (rnd(g) & 1) ? 50.0f : -50.0f;
    t->speed  = TRAFFIC_V_MIN + (TRAFFIC_V_MAX - TRAFFIC_V_MIN) * rndf(g);
    t->body   = colours[rnd(g) % (sizeof(colours) / sizeof(colours[0]))];
}

void car_update(car_game_t *g, float dt, int steer)
{
    if (g->over) { g->over_t += dt; return; }
    if (dt > 0.1f) dt = 0.1f;
    if (dt <= 0.0f) return;

    g->t += dt;
    float vn = g->speed / CAR_V_MAX;

    /* speed: automatic acceleration, drag when on the grass */
    bool offroad = fabsf(g->off) > CAR_ROAD_HALF + CAR_CURB_W;
    if (offroad) {
        g->speed -= OFFROAD_DECEL * dt;
        if (g->speed < CAR_V_OFFROAD_MIN) g->speed = CAR_V_OFFROAD_MIN;
    } else {
        g->speed += (ACC_BASE + (CAR_V_MAX - g->speed) * ACC_K) * dt;
        if (g->speed > CAR_V_MAX) g->speed = CAR_V_MAX;
    }

    /* steering + corner drift */
    float k = STEER_RESP * dt; if (k > 1.0f) k = 1.0f;
    g->steer_v += ((float)steer - g->steer_v) * k;
    g->off += g->steer_v * LAT_SPEED * (0.75f + 0.25f * vn) * dt;

    float c = curvature(g->pos + g->z_p);
    g->off -= c * CURVE_PUSH * vn * dt;
    if (fabsf(g->off) > CAR_BARRIER) {                 /* hit the roadside barrier */
        g->over = true; g->over_t = 0;
        if (g->score > g->best) g->best = g->score;
        g->speed = 0;
        return;
    }

    g->heading += c * g->speed * dt * 0.05f;
    g->pos     += g->speed * dt;
    g->score    = g->pos * 0.05f;

    /* traffic */
    for (int i = 0; i < CAR_MAX_TRAFFIC; i++) {
        car_traffic_t *t = &g->tr[i];
        if (!t->active) continue;
        float prev = t->d;
        t->d -= (g->speed - t->speed) * dt;
        if (fabsf(t->off - g->off) < HIT_X && t->d < HIT_NEAR && prev > -HIT_FAR) {
            g->over = true; g->over_t = 0;
            if (g->score > g->best) g->best = g->score;
            g->speed = 0;
            return;
        }
        if (g->z_p + t->d < 8.0f) t->active = false;   /* gone past the bottom edge */
    }
    spawn_traffic(g);
}

/* ------------------------------------------------------------------------ */
/* GT-R sprite (ported from modhighway.c, scalable + recolourable)          */
/* ------------------------------------------------------------------------ */
static inline int sc(float v, float cs) { return (int)floorf(v * cs + 0.5f); }

static void rect_s(car_game_t *g, int cx, int cy, float cs,
                   float ox, float oy, float ow, float oh, uint16_t col)
{
    int x0 = cx + sc(ox, cs), x1 = cx + sc(ox + ow, cs);
    int y0 = cy + sc(oy, cs), y1 = cy + sc(oy + oh, cs);
    if (x1 <= x0) x1 = x0 + 1;
    if (y1 <= y0) y1 = y0 + 1;
    fill_rect(g, x0, y0, x1 - x0, y1 - y0, col);
}

static void trap_s(car_game_t *g, int cx, int cy, float cs, float ocx,
                   float oy0, float oy1, float wt, float wb, uint16_t col)
{
    int y0 = cy + sc(oy0, cs), y1 = cy + sc(oy1, cs);
    if (y1 <= y0) y1 = y0 + 1;
    float ccx = (float)cx + ocx * cs;
    uint16_t be = swap16(col);
    for (int y = y0; y < y1; y++) {
        float t = (float)(y - y0) / (float)(y1 - y0);
        float half = (wt * (1.0f - t) + wb * t) * cs * 0.5f;
        int x0 = (int)floorf(ccx - half + 0.5f), x1 = (int)floorf(ccx + half + 0.5f);
        if (x1 <= x0) x1 = x0 + 1;
        span(g, y, x0, x1, be);
    }
}

/* cx = centre, cy = design anchor row, lean = canopy shift in design px */
static void draw_car(car_game_t *g, int cx, int cy, float cs, float lean,
                     uint16_t body, uint16_t shade)
{
    const uint16_t DARK = 0x0841, TYRE = 0x18C3, DIFF = 0x10A2, CHROME = 0xCE79,
                   GLASS = 0x1125, RED = 0xF800, YEL = 0xFFE0;

    trap_s(g, cx, cy, cs, 0, 10, 22, 108, 126, DARK);                    /* shadow    */
    rect_s(g, cx, cy, cs, -50, -4, 16, 20, TYRE);                        /* tyres     */
    rect_s(g, cx, cy, cs,  34, -4, 16, 20, TYRE);
    trap_s(g, cx, cy, cs, 0, 2, 16, 86, 94, DIFF);                       /* diffuser  */
    rect_s(g, cx, cy, cs, -4, 8, 8, 4, RED);                             /* fog lamp  */

    static const float ex[4] = { -38, -26, 16, 28 };                     /* exhausts  */
    for (int i = 0; i < 4; i++) rect_s(g, cx, cy, cs, ex[i], 4, 10, 8, CHROME);
    for (int i = 0; i < 4; i++) rect_s(g, cx, cy, cs, ex[i] + 2, 6, 6, 4, DARK);

    trap_s(g, cx, cy, cs, 0, -14, 4, 98, 92, shade);                     /* bumper    */
    trap_s(g, cx, cy, cs, 0, -24, -14, 92, 98, body);
    rect_s(g, cx, cy, cs, -18, -8, 36, 10, DIFF);                        /* plate     */

    rect_s(g, cx, cy, cs, -40, -20, 10, 10, RED);                        /* tail rings*/
    rect_s(g, cx, cy, cs, -38, -18, 6, 6, YEL);
    rect_s(g, cx, cy, cs, -26, -19, 8, 8, RED);
    rect_s(g, cx, cy, cs, -24, -17, 4, 4, YEL);
    rect_s(g, cx, cy, cs,  18, -19, 8, 8, RED);
    rect_s(g, cx, cy, cs,  20, -17, 4, 4, YEL);
    rect_s(g, cx, cy, cs,  30, -20, 10, 10, RED);
    rect_s(g, cx, cy, cs,  32, -18, 6, 6, YEL);

    trap_s(g, cx, cy, cs, lean, -44, -24, 52, 70, body);                 /* canopy    */
    trap_s(g, cx, cy, cs, lean, -42, -26, 42, 58, GLASS);

    float wl = lean * 0.5f;                                              /* spoiler   */
    rect_s(g, cx, cy, cs, wl - 28, -26, 4, 8, DARK);
    rect_s(g, cx, cy, cs, wl + 24, -26, 4, 8, DARK);
    rect_s(g, cx, cy, cs, wl - 42, -30, 84, 4, DARK);
}

/* ------------------------------------------------------------------------ */
/* text (built-in .zfont fonts, anti-aliased onto the framebuffer)          */
/* ------------------------------------------------------------------------ */
#define FONT_SANS16   2
#define FONT_SANS24B  3

typedef struct { const uint8_t *table, *blob; size_t blob_len; uint8_t bpp, height; uint16_t first, count; } font_t;

static uint32_t rd16(const uint8_t *p) { return (uint32_t)p[0] | ((uint32_t)p[1] << 8); }
static uint32_t rd32(const uint8_t *p) { return rd16(p) | (rd16(p + 2) << 16); }

static bool font_load(int id, font_t *f)
{
    if (id < 0 || id >= zl_builtin_font_count) return false;
    const uint8_t *b = zl_builtin_fonts[id].data;
    size_t len = zl_builtin_fonts[id].len;
    if (len < 16 || memcmp(b, "ZFNT", 4) != 0) return false;
    uint32_t count = rd16(b + 10);
    size_t tend = 16 + (size_t)count * 12;
    if (!count || tend > len) return false;
    f->bpp = b[5]; f->height = b[6]; f->first = (uint16_t)rd16(b + 8); f->count = (uint16_t)count;
    f->table = b + 16; f->blob = b + tend; f->blob_len = len - tend;
    return true;
}

static const uint8_t *glyph_entry(const font_t *f, uint32_t cp)
{
    if (cp < f->first || cp - f->first >= f->count) return NULL;
    return f->table + (size_t)(cp - f->first) * 12;
}

static int text_width(const font_t *f, const char *s)
{
    int w = 0;
    for (; *s; s++) {
        const uint8_t *e = glyph_entry(f, (uint8_t)*s);
        w += e ? e[6] : f->height / 2;
    }
    return w;
}

static void text_draw(car_game_t *g, int x, int y, const char *s, int font, uint16_t fg)
{
    font_t f;
    if (!font_load(font, &f)) return;
    for (; *s; s++) {
        const uint8_t *e = glyph_entry(&f, (uint8_t)*s);
        if (!e) { x += f.height / 2; continue; }
        uint32_t off = rd32(e);
        int gw = e[4], gh = e[5], adv = e[6];
        int gx = x + (int8_t)e[7], gy = y + (int8_t)e[8];
        int stride = f.bpp == 1 ? (gw + 7) / 8 : (gw + 1) / 2;
        if (gw > 0 && gh > 0 && off <= f.blob_len && (size_t)stride * gh <= f.blob_len - off) {
            const uint8_t *bm = f.blob + off;
            for (int r = 0; r < gh; r++) {
                int py = gy + r;
                if (py < 0 || py >= g->h) continue;
                for (int c = 0; c < gw; c++) {
                    int px = gx + c;
                    if (px < 0 || px >= g->w) continue;
                    int a;
                    if (f.bpp == 1) a = (bm[r * stride + (c >> 3)] & (0x80 >> (c & 7))) ? 15 : 0;
                    else { uint8_t v = bm[r * stride + (c >> 1)]; a = (c & 1) ? (v & 15) : (v >> 4); }
                    if (!a) continue;
                    uint16_t *d = &g->fb[(size_t)py * g->w + px];
                    *d = swap16(blend565(swap16(*d), fg, (uint32_t)a * 17u));
                }
            }
        }
        x += adv;
    }
}

static void text_shadow(car_game_t *g, int x, int y, const char *s, int font, uint16_t fg)
{
    text_draw(g, x + 1, y + 1, s, font, 0x0000);
    text_draw(g, x, y, s, font, fg);
}

static void text_centered(car_game_t *g, int y, const char *s, int font, uint16_t fg)
{
    font_t f;
    if (!font_load(font, &f)) return;
    text_shadow(g, (g->w - text_width(&f, s)) / 2, y, s, font, fg);
}

/* ------------------------------------------------------------------------ */
/* scene                                                                    */
/* ------------------------------------------------------------------------ */
static void render_background(car_game_t *g)
{
    const int w = g->w, hz = g->horizon;
    for (int y = 0; y <= hz; y++) span(g, y, 0, w, g->sky[y]);

    for (int i = 0; i < CAR_NUM_STARS; i++) {
        int x = g->star_x[i], y = g->star_y[i];
        g->fb[(size_t)y * w + x] = BE(((i & 3) == 0) ? 0xFFFF : 0x8C71);
    }

    /* distant mountains, scrolling with the road heading */
    const uint16_t mt = BE(RGB5(4, 8, 11));
    float hs = (float)g->h / 300.0f;
    for (int x = 0; x < w; x++) {
        float u = (float)x + g->heading;
        float m = 5.0f + 11.0f * (0.5f + 0.5f * sinf(u * 0.021f))
                       + 7.0f * (0.5f + 0.5f * sinf(u * 0.057f + 1.3f));
        int mh = (int)(m * hs);
        for (int y = hz - mh + 1; y <= hz; y++) g->fb[(size_t)y * w + x] = mt;
    }
}

static void render_road(car_game_t *g)
{
    const int w = g->w, h = g->h, hz = g->horizon;

    /* road centre per row: integrate curvature from the car outwards */
    float dx = 0.0f, xo = 0.0f, zprev = g->row_z[h - 1];
    for (int y = h - 1; y > hz; y--) {
        float z = g->row_z[y];
        float dz = z - zprev;
        float c = curvature(g->pos + z);
        dx += c * dz * CURVE_K;
        xo += dx * dz;
        g->row_cx[y] = 0.5f * (float)w + xo * g->pxu * g->row_s[y];
        zprev = z;
    }

    static uint16_t grass[3], road[3], curb[3], edge;
    static bool init;
    if (!init) {
        const uint16_t ga = 0x1341, gb = 0x0B01, ra = 0x31A6, rb = 0x2124, ca = 0xD882, cb = 0xEF7D;
        grass[0] = swap16(ga); grass[1] = swap16(gb); grass[2] = swap16(blend565(ga, gb, 128));
        road[0]  = swap16(ra); road[1]  = swap16(rb); road[2]  = swap16(blend565(ra, rb, 128));
        curb[0]  = swap16(ca); curb[1]  = swap16(cb); curb[2]  = swap16(blend565(ca, cb, 128));
        edge = swap16(0xFFFF);
        init = true;
    }
    const uint16_t dash = swap16(0xFFFF), dash_far = swap16(0x9CD3);

    for (int y = hz + 1; y < h; y++) {
        float s = g->row_s[y], z = g->row_z[y], wz = g->pos + z;
        int seg = (z > FOG_Z) ? 2 : ((int)(wz * 0.03f) & 1);
        int cx   = (int)(g->row_cx[y] + 0.5f);
        int half = (int)(g->road_px * s);
        int curbw = (int)(CAR_CURB_W * g->pxu * s); if (curbw < 1) curbw = 1;
        int ew = (int)(1.6f * g->pxu * s); if (ew < 1) ew = 1;
        int rl = cx - half, rr = cx + half;

        span(g, y, 0, w, grass[seg]);
        span(g, y, rl - curbw, rl, curb[seg]);
        span(g, y, rr + 1, rr + 1 + curbw, curb[seg]);
        span(g, y, rl, rr + 1, road[seg]);
        span(g, y, rl, rl + ew, edge);
        span(g, y, rr - ew + 1, rr + 1, edge);
        if (z < 520.0f && ((int)(wz * 0.04f) & 1) == 0) {
            int lw = (int)(1.1f * g->pxu * s); if (lw < 1) lw = 1;
            span(g, y, cx - lw, cx + lw + 1, z > FOG_Z ? dash_far : dash);
        }
    }
}

static void draw_traffic_car(car_game_t *g, const car_traffic_t *t)
{
    float z = g->z_p + t->d;
    if (z < 8.0f) return;
    float s = 10.0f / z;
    float yf = (float)g->horizon + s * (float)(g->h - g->horizon) - 0.5f;
    int iy = (int)yf;
    if (iy <= g->horizon) return;
    int ry = iy >= g->h ? g->h - 1 : iy;
    float cx = g->row_cx[ry] + t->off * g->pxu * s;
    float cs = CAR_WIDTH * g->pxu * s / CAR_DESIGN_W;
    draw_car(g, (int)(cx + 0.5f), iy - (int)(14.0f * cs), cs, 0.0f, t->body, darken(t->body));
}

static void render_hud(car_game_t *g)
{
    char buf[40];
    snprintf(buf, sizeof(buf), "%d m", (int)g->score);
    text_shadow(g, 8, 6, buf, FONT_SANS16, 0xFFFF);

    snprintf(buf, sizeof(buf), "%d km/h", (int)(g->speed * 0.2f));
    font_t f;
    if (font_load(FONT_SANS16, &f)) text_shadow(g, g->w - text_width(&f, buf) - 8, 6, buf, FONT_SANS16, 0xFFE0);

}

static void render_overlay(car_game_t *g)
{
    size_t n = (size_t)g->w * g->h;
    for (size_t i = 0; i < n; i++) g->fb[i] = swap16(darken(swap16(g->fb[i])));

    char buf[48];
    int y = g->h / 2 - 62;
    text_centered(g, y, "GAME OVER", FONT_SANS24B, 0xF800);
    snprintf(buf, sizeof(buf), "DISTANCE  %d m", (int)g->score);
    text_centered(g, y + 40, buf, FONT_SANS16, 0xFFFF);
    snprintf(buf, sizeof(buf), "BEST  %d m", (int)g->best);
    text_centered(g, y + 62, buf, FONT_SANS16, 0xFFE0);
    if (g->over_t > 0.8f) text_centered(g, y + 96, "TOUCH TO RETRY", FONT_SANS16, 0x07FF);
}

void car_render(car_game_t *g)
{
    render_background(g);
    render_road(g);

    /* far -> near; the player is drawn at its own depth */
    bool done[CAR_MAX_TRAFFIC] = { false };
    bool player_done = false;
    for (;;) {
        int best = -1; float bd = -1e9f;
        for (int i = 0; i < CAR_MAX_TRAFFIC; i++)
            if (g->tr[i].active && !done[i] && g->tr[i].d > bd) { bd = g->tr[i].d; best = i; }

        if (!player_done && (best < 0 || bd < 0.0f)) {
            float bob = sinf(g->pos * 0.06f) * 1.0f * g->cs_p * (g->speed / CAR_V_MAX);
            int cx = (int)(g->row_cx[g->car_y] + g->off * g->pxu * g->scale_p + 0.5f);
            int cy = g->car_y - (int)(14.0f * g->cs_p) + (int)bob;
            draw_car(g, cx, cy, g->cs_p, g->steer_v * 6.0f, 0x1A7F, 0x0974);
            player_done = true;
            continue;
        }
        if (best < 0) break;
        draw_traffic_car(g, &g->tr[best]);
        done[best] = true;
    }

    render_hud(g);
    if (g->over) render_overlay(g);
}
