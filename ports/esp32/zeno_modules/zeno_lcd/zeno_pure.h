/*
 * zeno_pure.h
 *
 * Hardware-independent logic (no ESP-IDF, no MicroPython includes) so that it
 * can be unit-tested on a host compiler. See tests/host_test_pure.c.
 */
#ifndef ZENO_PURE_H
#define ZENO_PURE_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <limits.h>
#include <math.h>

#define ZP_UNUSED __attribute__((unused))

/* ------------------------------------------------------------------------
 * Data bus lookup table.
 * lut[v] = mask of GPIO bits that must be HIGH to put byte v on D0..D7.
 * Returns the mask of all eight data pins. Bits to clear = mask ^ lut[v].
 * ---------------------------------------------------------------------- */
static inline uint32_t zl_lut_build(const uint8_t pins[8], uint32_t lut[256])
{
    uint32_t mask = 0;
    for (int b = 0; b < 8; b++) {
        mask |= 1u << pins[b];
    }
    for (int v = 0; v < 256; v++) {
        uint32_t m = 0;
        for (int b = 0; b < 8; b++) {
            if (v & (1 << b)) {
                m |= 1u << pins[b];
            }
        }
        lut[v] = m;
    }
    return mask;
}

/* ------------------------------------------------------------------------
 * Rotation / MADCTL.
 *
 * Index 0 = 0x98 is VERIFIED: it is the value hard-coded in the working
 * Nopnop ili9488.c (MY|ML|BGR, portrait 320x480).
 *
 * Indices 1..3 are DERIVED, NOT VERIFIED ON HARDWARE. They are the other
 * three non-mirrored orientations reachable by MADCTL, keeping the ML|BGR
 * bits (0x18) of the original. Index 1 matches the 0x28 landscape value used
 * by the existing moclcd module (that value had no ML bit).
 * Any entry can be overridden at runtime with the madctl= argument.
 * ---------------------------------------------------------------------- */
#define ZL_NUM_ROTATIONS 4
static const uint8_t zl_madctl_table[ZL_NUM_ROTATIONS] ZP_UNUSED = {
    0x98,   /* rot 0: 320x480 (verified)                 */
    0x38,   /* rot 1: 480x320 (derived, matches legacy)  */
    0x58,   /* rot 2: 320x480 (derived)                  */
    0xF8,   /* rot 3: 480x320 (derived)                  */
};

static inline bool zl_rot_is_landscape(int rot) { return (rot & 1) != 0; }

/* ------------------------------------------------------------------------
 * Touch acquisition helpers: copied behaviour of lcd_com.c
 * ---------------------------------------------------------------------- */

/* touch_avr_analog(): drop min and max, average the rest. n must be > 2. */
static inline int zt_trimmed_mean(const int *v, int n)
{
    if (n <= 2) {
        return 0;
    }
    int sum = 0, max = 0, min = INT_MAX;
    for (int i = 0; i < n; i++) {
        if (v[i] > max) max = v[i];
        if (v[i] < min) min = v[i];
        sum += v[i];
    }
    return (sum - min - max) / (n - 2);
}

/* touch_getx()/touch_gety(): accept only if the two rounds agree. */
static inline int zt_axis_combine(int s0, int s1, int tol)
{
    int d = s0 > s1 ? s0 - s1 : s1 - s0;
    return d <= tol ? s0 + s1 : 0;
}

static inline int zt_median3(int a, int b, int c)
{
    int t;
    if (a > b) { t = a; a = b; b = t; }
    if (b > c) { t = b; b = c; c = t; }
    if (a > b) { t = a; a = b; b = t; }
    return b;
}

typedef struct {
    int min_raw;      /* minimum axis sum to count as pressed        */
    int z_max;        /* maximum Z (|Y+ - X-|); >= 4095 disables gate */
    int settle_us;    /* delay after switching electrodes             */
    int down_confirm; /* consecutive valid samples => touch down      */
    int up_confirm;   /* consecutive invalid samples => touch up      */
} zt_config_t;

static inline bool zt_sample_valid(int rx, int ry, int z, const zt_config_t *c)
{
    if (rx == 0 || ry == 0) return false;          /* unstable (original rule) */
    if (rx < c->min_raw || ry < c->min_raw) return false;
    if (c->z_max < 4095 && z > c->z_max) return false;
    return true;
}

typedef struct { int down; int up; bool touched; } zt_debounce_t;

static inline bool zt_debounce_step(zt_debounce_t *s, bool valid, int down_n, int up_n)
{
    if (valid) {
        s->up = 0;
        if (s->down < 1000) s->down++;
        if (!s->touched && s->down >= down_n) s->touched = true;
    } else {
        s->down = 0;
        if (s->up < 1000) s->up++;
        if (s->touched && s->up >= up_n) s->touched = false;
    }
    return s->touched;
}

/* ------------------------------------------------------------------------
 * Affine calibration:  sx = a*rx + b*ry + c ;  sy = d*rx + e*ry + f
 * Least squares over n >= 3 points (n >= 5 recommended so residuals mean
 * something). Handles offset, scale, rotation, skew and axis swap, which the
 * original two-point separable mapping cannot.
 * ---------------------------------------------------------------------- */
typedef struct { float a, b, c, d, e, f; bool valid; } zt_cal_t;

static inline bool zt_cal_solve(const double *rx, const double *ry,
                                const double *sx, const double *sy, int n,
                                zt_cal_t *out, double *rms, double *maxerr)
{
    if (n < 3) return false;
    double mx = 0, my = 0, msx = 0, msy = 0;
    for (int i = 0; i < n; i++) { mx += rx[i]; my += ry[i]; msx += sx[i]; msy += sy[i]; }
    mx /= n; my /= n; msx /= n; msy /= n;

    double sxx = 0, sxy = 0, syy = 0, sxs = 0, sys = 0, sxt = 0, syt = 0;
    for (int i = 0; i < n; i++) {
        double dx = rx[i] - mx, dy = ry[i] - my;
        double ex = sx[i] - msx, ey = sy[i] - msy;
        sxx += dx * dx; sxy += dx * dy; syy += dy * dy;
        sxs += dx * ex; sys += dy * ex;
        sxt += dx * ey; syt += dy * ey;
    }
    if (sxx <= 0 || syy <= 0) return false;
    double det = sxx * syy - sxy * sxy;
    if (!(det > 1e-4 * sxx * syy)) return false;      /* collinear / degenerate */

    double a = (sxs * syy - sys * sxy) / det;
    double b = (sys * sxx - sxs * sxy) / det;
    double d = (sxt * syy - syt * sxy) / det;
    double e = (syt * sxx - sxt * sxy) / det;
    double c = msx - a * mx - b * my;
    double f = msy - d * mx - e * my;

    double se = 0, me = 0;
    for (int i = 0; i < n; i++) {
        double px = a * rx[i] + b * ry[i] + c;
        double py = d * rx[i] + e * ry[i] + f;
        double er = sqrt((px - sx[i]) * (px - sx[i]) + (py - sy[i]) * (py - sy[i]));
        se += er * er;
        if (er > me) me = er;
    }
    out->a = (float)a; out->b = (float)b; out->c = (float)c;
    out->d = (float)d; out->e = (float)e; out->f = (float)f;
    out->valid = true;
    if (rms) *rms = sqrt(se / n);
    if (maxerr) *maxerr = me;
    return true;
}

/* raw -> screen, rounded to nearest, clamped to [0,w-1] x [0,h-1] */
static inline void zt_cal_apply(const zt_cal_t *k, int rx, int ry, int w, int h,
                                int *x, int *y)
{
    float fx = k->a * (float)rx + k->b * (float)ry + k->c;
    float fy = k->d * (float)rx + k->e * (float)ry + k->f;
    long ix = lroundf(fx), iy = lroundf(fy);
    if (ix < 0) ix = 0;
    if (iy < 0) iy = 0;
    if (ix > w - 1) ix = w - 1;
    if (iy > h - 1) iy = h - 1;
    *x = (int)ix;
    *y = (int)iy;
}

/* ------------------------------------------------------------------------
 * Colour + text helpers (used by zeno_gfx.c / zeno_font.c)
 * ---------------------------------------------------------------------- */

/* 8-bit-per-channel RGB -> RGB565 */
static inline uint16_t zl_color565(uint8_t r, uint8_t g, uint8_t b)
{
    return (uint16_t)(((r & 0xF8u) << 8) | ((g & 0xFCu) << 3) | (b >> 3));
}

/* Blend two RGB565 colours. a = 0 -> bg exactly, a = 255 -> fg exactly. */
static inline uint16_t zl_blend565(uint16_t bg, uint16_t fg, uint8_t a)
{
    uint32_t ia = 255u - a;
    uint32_t r = (((bg >> 11) & 0x1Fu) * ia + ((fg >> 11) & 0x1Fu) * a + 127u) / 255u;
    uint32_t g = (((bg >> 5)  & 0x3Fu) * ia + ((fg >> 5)  & 0x3Fu) * a + 127u) / 255u;
    uint32_t b = ((bg & 0x1Fu) * ia + (fg & 0x1Fu) * a + 127u) / 255u;
    return (uint16_t)((r << 11) | (g << 5) | b);
}

/* Decode one UTF-8 code point from s[*i..len). Advances *i. Malformed or
 * truncated sequences yield U+FFFD and consume exactly one byte. */
static inline uint32_t zl_utf8_next(const uint8_t *s, size_t len, size_t *i)
{
    uint32_t c = s[*i];
    int extra;
    uint32_t cp, min;
    if (c < 0x80) { (*i)++; return c; }
    if      ((c & 0xE0) == 0xC0) { extra = 1; cp = c & 0x1F; min = 0x80; }
    else if ((c & 0xF0) == 0xE0) { extra = 2; cp = c & 0x0F; min = 0x800; }
    else if ((c & 0xF8) == 0xF0) { extra = 3; cp = c & 0x07; min = 0x10000; }
    else { (*i)++; return 0xFFFD; }
    if (*i + (size_t)extra >= len) { (*i)++; return 0xFFFD; }   /* truncated */
    for (int k = 1; k <= extra; k++) {
        uint32_t cc = s[*i + k];
        if ((cc & 0xC0) != 0x80) { (*i)++; return 0xFFFD; }
        cp = (cp << 6) | (cc & 0x3F);
    }
    if (cp < min || cp > 0x10FFFF || (cp >= 0xD800 && cp <= 0xDFFF)) { (*i)++; return 0xFFFD; }
    *i += (size_t)extra + 1;
    return cp;
}

#endif /* ZENO_PURE_H */
