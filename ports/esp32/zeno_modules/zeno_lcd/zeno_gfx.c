/*
 * zeno_gfx.c -- see zeno_gfx.h. Pure geometry on top of the panel layer;
 * no GPIO, no MicroPython. Every shape is reduced to zl_fill_rect() spans
 * (merged into taller rects where rows repeat) so the number of bus
 * transactions stays low.
 */
#include <math.h>
#include <stdlib.h>
#include "lcd_bus.h"
#include "ili9488_panel.h"
#include "zeno_gfx.h"
#include "zeno_pure.h"
#include "zeno_lcd_config.h"

_Static_assert(ZL_LINE_MAX_PX >= ZLCD_NATIVE_WIDTH && ZL_LINE_MAX_PX >= ZLCD_NATIVE_HEIGHT,
               "line buffer must fit the longest panel edge");

static uint8_t s_linebuf[ZL_LINE_MAX_PX * 2];
uint8_t *zl_gfx_linebuf(void) { return s_linebuf; }

static inline int imin(int a, int b) { return a < b ? a : b; }
static inline int imax(int a, int b) { return a > b ? a : b; }

/* inclusive bbox test */
static bool bbox_visible(int x0, int y0, int x1, int y1)
{
    return !(x1 < 0 || y1 < 0 || x0 >= (int)zl_panel_width() || y0 >= (int)zl_panel_height());
}

/* ------------------------------------------------------------------ ellipse */

void zl_draw_ellipse(int xm, int ym, int a, int b, uint16_t c)
{
    if (a < 0 || b < 0) return;
    if (!bbox_visible(xm - a, ym - b, xm + a, ym + b)) return;
    if (a == 0 || b == 0) { zl_fill_rect(xm - a, ym - b, 2 * a + 1, 2 * b + 1, c); return; }

    /* Zingl's integer ellipse; quadrant-symmetric. */
    int64_t dx = 0, dy = b;
    int64_t a2 = (int64_t)a * a, b2 = (int64_t)b * b;
    int64_t err = b2 - (2 * b - 1) * a2, e2;
    do {
        zl_draw_pixel(xm + (int)dx, ym + (int)dy, c);
        zl_draw_pixel(xm - (int)dx, ym + (int)dy, c);
        zl_draw_pixel(xm - (int)dx, ym - (int)dy, c);
        zl_draw_pixel(xm + (int)dx, ym - (int)dy, c);
        e2 = 2 * err;
        if (e2 <  (2 * dx + 1) * b2) { dx++; err += (2 * dx + 1) * b2; }
        if (e2 > -(2 * dy - 1) * a2) { dy--; err -= (2 * dy - 1) * a2; }
    } while (dy >= 0);
    while (dx++ < a) {                       /* finish the tips of flat ellipses */
        zl_draw_pixel(xm + (int)dx, ym, c);
        zl_draw_pixel(xm - (int)dx, ym, c);
    }
}

void zl_fill_ellipse(int xm, int ym, int a, int b, uint16_t c)
{
    if (a < 0 || b < 0) return;
    if (!bbox_visible(xm - a, ym - b, xm + a, ym + b)) return;
    if (a == 0 || b == 0) { zl_fill_rect(xm - a, ym - b, 2 * a + 1, 2 * b + 1, c); return; }

    int dy0 = imax(-b, -ym);
    int dy1 = imin(b, (int)zl_panel_height() - 1 - ym);
    int run_half = -1, run_y = 0, run_h = 0;
    const float inv_b2 = 1.0f / ((float)b * (float)b);

    for (int dy = dy0; dy <= dy1; dy++) {
        float t = 1.0f - (float)(dy * dy) * inv_b2;
        int half = (int)((float)a * sqrtf(t < 0 ? 0 : t) + 0.5f);
        if (run_h > 0 && half == run_half) { run_h++; continue; }
        if (run_h > 0) zl_fill_rect(xm - run_half, run_y, 2 * run_half + 1, run_h, c);
        run_half = half; run_y = ym + dy; run_h = 1;
    }
    if (run_h > 0) zl_fill_rect(xm - run_half, run_y, 2 * run_half + 1, run_h, c);
}

/* --------------------------------------------------------------- round rect */

/* Horizontal inset of corner row i (0 = outermost row) for radius r. */
static int rr_inset(int r, int i)
{
    int d = r - i;                                   /* r .. 1 */
    int hx = (int)(sqrtf((float)(r * r - d * d)) + 0.5f);
    return r - hx;
}

static int rr_clamp_r(int w, int h, int r)
{
    int m = imin(w, h) / 2;
    if (r > m) r = m;
    return r < 0 ? 0 : r;
}

void zl_fill_round_rect(int x, int y, int w, int h, int r, uint16_t c)
{
    if (w <= 0 || h <= 0) return;
    if (!bbox_visible(x, y, x + w - 1, y + h - 1)) return;
    r = rr_clamp_r(w, h, r);
    if (r == 0) { zl_fill_rect(x, y, w, h, c); return; }

    int i = 0;
    while (i < r) {
        int ins = rr_inset(r, i), j = i;
        while (j + 1 < r && rr_inset(r, j + 1) == ins) j++;
        int rows = j - i + 1;
        zl_fill_rect(x + ins, y + i,             w - 2 * ins, rows, c);
        zl_fill_rect(x + ins, y + h - 1 - j,     w - 2 * ins, rows, c);
        i = j + 1;
    }
    zl_fill_rect(x, y + r, w, h - 2 * r, c);
}

void zl_draw_round_rect(int x, int y, int w, int h, int r, uint16_t c)
{
    if (w <= 0 || h <= 0) return;
    if (!bbox_visible(x, y, x + w - 1, y + h - 1)) return;
    r = rr_clamp_r(w, h, r);
    if (r == 0) { zl_draw_rect(x, y, w, h, c); return; }

    zl_fill_rect(x + r, y,         w - 2 * r, 1, c);
    zl_fill_rect(x + r, y + h - 1, w - 2 * r, 1, c);
    zl_fill_rect(x,         y + r, 1, h - 2 * r, c);
    zl_fill_rect(x + w - 1, y + r, 1, h - 2 * r, c);

    for (int i = 1; i < r; i++) {
        int ins = rr_inset(r, i);
        int prev = rr_inset(r, i - 1);
        int run = prev - ins;
        if (run < 1) run = 1;
        zl_fill_rect(x + ins,           y + i,         run, 1, c);   /* top-left     */
        zl_fill_rect(x + w - ins - run, y + i,         run, 1, c);   /* top-right    */
        zl_fill_rect(x + ins,           y + h - 1 - i, run, 1, c);   /* bottom-left  */
        zl_fill_rect(x + w - ins - run, y + h - 1 - i, run, 1, c);   /* bottom-right */
    }
}

/* ----------------------------------------------------------------- triangle */

void zl_draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint16_t c)
{
    zl_draw_line(x0, y0, x1, y1, c);
    zl_draw_line(x1, y1, x2, y2, c);
    zl_draw_line(x2, y2, x0, y0, c);
}

static inline void swap2(int *a, int *b, int *c, int *d)
{
    int t = *a; *a = *c; *c = t;
    t = *b; *b = *d; *d = t;
}

void zl_fill_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint16_t c)
{
    if (y0 > y1) swap2(&x0, &y0, &x1, &y1);
    if (y1 > y2) swap2(&x1, &y1, &x2, &y2);
    if (y0 > y1) swap2(&x0, &y0, &x1, &y1);

    if (!bbox_visible(imin(x0, imin(x1, x2)), y0, imax(x0, imax(x1, x2)), y2)) return;

    if (y0 == y2) {                                   /* degenerate: single row */
        int lo = imin(x0, imin(x1, x2)), hi = imax(x0, imax(x1, x2));
        zl_fill_rect(lo, y0, hi - lo + 1, 1, c);
        return;
    }
    int ys = imax(y0, 0), ye = imin(y2, (int)zl_panel_height() - 1);
    for (int y = ys; y <= ye; y++) {
        int xa = x0 + (int)(((int64_t)(x2 - x0) * (y - y0)) / (y2 - y0));
        int xb;
        if (y < y1)       xb = x0 + (int)(((int64_t)(x1 - x0) * (y - y0)) / (y1 - y0));
        else if (y2 != y1) xb = x1 + (int)(((int64_t)(x2 - x1) * (y - y1)) / (y2 - y1));
        else               xb = x1;
        if (xa > xb) { int t = xa; xa = xb; xb = t; }
        zl_fill_rect(xa, y, xb - xa + 1, 1, c);
    }
}

/* ------------------------------------------------------------------ polygon */

void zl_draw_polygon(const int *p, int n, uint16_t c)
{
    if (n <= 0) return;
    if (n > ZL_POLY_MAX) n = ZL_POLY_MAX;
    if (n == 1) { zl_draw_pixel(p[0], p[1], c); return; }
    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n;
        zl_draw_line(p[2 * i], p[2 * i + 1], p[2 * j], p[2 * j + 1], c);
    }
}

void zl_fill_polygon(const int *p, int n, uint16_t c)
{
    if (n < 3) { zl_draw_polygon(p, n, c); return; }
    if (n > ZL_POLY_MAX) n = ZL_POLY_MAX;

    int ymin = p[1], ymax = p[1];
    for (int i = 1; i < n; i++) { ymin = imin(ymin, p[2 * i + 1]); ymax = imax(ymax, p[2 * i + 1]); }
    ymin = imax(ymin, 0);
    ymax = imin(ymax, (int)zl_panel_height() - 1);

    int xs[ZL_POLY_MAX];
    for (int y = ymin; y <= ymax; y++) {
        int k = 0;
        for (int i = 0; i < n; i++) {
            int j = (i + 1) % n;
            int xa = p[2 * i], ya = p[2 * i + 1], xb = p[2 * j], yb = p[2 * j + 1];
            if (ya == yb) continue;
            if (y < imin(ya, yb) || y >= imax(ya, yb)) continue;     /* half-open */
            xs[k++] = xa + (int)(((int64_t)(y - ya) * (xb - xa)) / (yb - ya));
        }
        for (int a = 1; a < k; a++) {                                 /* insertion sort */
            int v = xs[a], b = a - 1;
            while (b >= 0 && xs[b] > v) { xs[b + 1] = xs[b]; b--; }
            xs[b + 1] = v;
        }
        for (int m = 0; m + 1 < k; m += 2) {
            zl_fill_rect(xs[m], y, xs[m + 1] - xs[m] + 1, 1, c);
        }
    }
    zl_draw_polygon(p, n, c);          /* closes the bottom row / exact edges */
}

/* -------------------------------------------------------- arcs, rings, pies */

void zl_fill_ring_sector(int cx, int cy, int r_in, int r_out,
                         float a0, float a1, uint16_t c)
{
    if (r_out < 0 || r_in > r_out) return;
    if (r_in < 0) r_in = 0;
    if (!bbox_visible(cx - r_out, cy - r_out, cx + r_out, cy + r_out)) return;

    float sweep = a1 - a0;
    bool full = sweep >= 360.0f || sweep <= -360.0f;
    if (!full) {
        sweep = fmodf(sweep, 360.0f);
        if (sweep < 0) sweep += 360.0f;
        if (sweep == 0) return;
    }
    const float D2R = 0.017453292519943295f;
    float sx = cosf(a0 * D2R), sy = sinf(a0 * D2R);
    float ex = cosf((a0 + sweep) * D2R), ey = sinf((a0 + sweep) * D2R);
    bool big = sweep > 180.0f;

    float hi2 = ((float)r_out + 0.5f) * ((float)r_out + 0.5f);
    float lo2 = r_in > 0 ? ((float)r_in - 0.5f) * ((float)r_in - 0.5f) : -1.0f;

    int y0 = imax(cy - r_out, 0), y1 = imin(cy + r_out, (int)zl_panel_height() - 1);
    for (int y = y0; y <= y1; y++) {
        int dy = y - cy;
        float rem = hi2 - (float)(dy * dy);
        if (rem < 0) continue;
        int half = (int)sqrtf(rem);
        int xa = imax(cx - half, 0), xb = imin(cx + half, (int)zl_panel_width() - 1);
        int run_s = 0;
        bool in_run = false;
        for (int x = xa; x <= xb + 1; x++) {
            bool in = false;
            if (x <= xb) {
                int dx = x - cx;
                float d2 = (float)(dx * dx + dy * dy);
                if (d2 < hi2 && d2 >= lo2) {
                    if (full || (dx == 0 && dy == 0)) in = true;
                    else {
                        float cs = sx * (float)dy - sy * (float)dx;   /* cross(s,p) */
                        float ce = (float)dx * ey - (float)dy * ex;   /* cross(p,e) */
                        if (!big) in = (cs >= 0 && ce >= 0);
                        else      in = !(cs < 0 && ce < 0);
                    }
                }
            }
            if (in && !in_run) { run_s = x; in_run = true; }
            else if (!in && in_run) { zl_fill_rect(run_s, y, x - run_s, 1, c); in_run = false; }
        }
    }
}

void zl_draw_arc(int cx, int cy, int r, float a0, float a1, int thick, uint16_t c)
{
    if (thick < 1) thick = 1;
    int r_in = r - thick + 1;
    if (r_in < 1) r_in = 1;
    zl_fill_ring_sector(cx, cy, r_in, r, a0, a1, c);
}

/* ------------------------------------------------------- thick line, bezier */

void zl_draw_thick_line(int x0, int y0, int x1, int y1, int w, uint16_t c)
{
    if (w <= 1) { zl_draw_line(x0, y0, x1, y1, c); return; }
    float dx = (float)(x1 - x0), dy = (float)(y1 - y0);
    float len = sqrtf(dx * dx + dy * dy);
    if (len < 0.5f) { zl_fill_circle(x0, y0, w / 2, c); return; }
    float nx = -dy / len * ((float)w * 0.5f), ny = dx / len * ((float)w * 0.5f);
    int q[8] = {
        (int)lroundf((float)x0 + nx), (int)lroundf((float)y0 + ny),
        (int)lroundf((float)x1 + nx), (int)lroundf((float)y1 + ny),
        (int)lroundf((float)x1 - nx), (int)lroundf((float)y1 - ny),
        (int)lroundf((float)x0 - nx), (int)lroundf((float)y0 - ny),
    };
    zl_fill_polygon(q, 4, c);
    if (w >= 3) {
        zl_fill_circle(x0, y0, w / 2, c);
        zl_fill_circle(x1, y1, w / 2, c);
    }
}

static int bezier_steps(float len)
{
    int n = (int)(len / 5.0f);
    return n < 8 ? 8 : (n > 80 ? 80 : n);
}

void zl_draw_bezier3(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, uint16_t c)
{
    float len = hypotf((float)(x1 - x0), (float)(y1 - y0)) + hypotf((float)(x2 - x1), (float)(y2 - y1)) +
                hypotf((float)(x3 - x2), (float)(y3 - y2));
    int n = bezier_steps(len);
    int px = x0, py = y0;
    for (int i = 1; i <= n; i++) {
        float t = (float)i / (float)n, u = 1.0f - t;
        float b0 = u * u * u, b1 = 3 * u * u * t, b2 = 3 * u * t * t, b3 = t * t * t;
        int x = (int)lroundf(b0 * x0 + b1 * x1 + b2 * x2 + b3 * x3);
        int y = (int)lroundf(b0 * y0 + b1 * y1 + b2 * y2 + b3 * y3);
        zl_draw_line(px, py, x, y, c);
        px = x; py = y;
    }
}

void zl_draw_bezier2(int x0, int y0, int x1, int y1, int x2, int y2, uint16_t c)
{
    float len = hypotf((float)(x1 - x0), (float)(y1 - y0)) + hypotf((float)(x2 - x1), (float)(y2 - y1));
    int n = bezier_steps(len);
    int px = x0, py = y0;
    for (int i = 1; i <= n; i++) {
        float t = (float)i / (float)n, u = 1.0f - t;
        int x = (int)lroundf(u * u * x0 + 2 * u * t * x1 + t * t * x2);
        int y = (int)lroundf(u * u * y0 + 2 * u * t * y1 + t * t * y2);
        zl_draw_line(px, py, x, y, c);
        px = x; py = y;
    }
}

/* ----------------------------------------------------------------- gradient */

void zl_fill_rect_gradient(int x, int y, int w, int h, uint16_t c0, uint16_t c1, bool vertical)
{
    if (w <= 0 || h <= 0) return;
    int n = vertical ? h : w;
    int denom = n > 1 ? n - 1 : 1;

    if (vertical) {
        int i0 = imax(0, -y), i1 = imin(h - 1, (int)zl_panel_height() - 1 - y);
        for (int i = i0; i <= i1; i++) {
            zl_fill_rect(x, y + i, w, 1, zl_blend565(c0, c1, (uint8_t)((i * 255) / denom)));
        }
        return;
    }
    /* horizontal: one window, the same row buffer streamed h times */
    int vx = x, vy = y, vw = w, vh = h;
    if (!zl_clip_rect(&vx, &vy, &vw, &vh)) return;
    uint8_t *row = zl_gfx_linebuf();
    for (int k = 0; k < vw; k++) {
        int i = (vx - x) + k;
        uint16_t col = zl_blend565(c0, c1, (uint8_t)((i * 255) / denom));
        row[2 * k] = (uint8_t)(col >> 8);
        row[2 * k + 1] = (uint8_t)(col & 0xFF);
    }
    zl_set_window(vx, vy, vx + vw - 1, vy + vh - 1);
    for (int r = 0; r < vh; r++) zl_bus_write_bytes(row, (size_t)vw * 2u);
}
