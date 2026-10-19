/*
 * zeno_gfx.h -- extended drawing primitives, built ONLY on top of
 * zl_fill_rect() / zl_draw_pixel() / zl_set_window() from ili9488_panel.h.
 *
 * Locking: every function here is [LOCK] -- call with the bus lock held.
 * Clipping: everything clips silently to the panel (via zl_fill_rect /
 * zl_draw_pixel), and shapes entirely off-screen return immediately.
 *
 * Angles are in DEGREES, 0 = 3 o'clock, increasing CLOCKWISE on screen
 * (same as atan2(dy, dx) with y pointing down).
 */
#ifndef ZENO_GFX_H
#define ZENO_GFX_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define ZL_POLY_MAX      64      /* max polygon vertices                    */
#define ZL_LINE_MAX_PX   480     /* >= max(native width, native height)     */

/* Shared one-scanline RGB565 staging buffer (ZL_LINE_MAX_PX * 2 bytes).
 * Only valid while the bus lock is held. */
uint8_t *zl_gfx_linebuf(void);

void zl_draw_ellipse(int cx, int cy, int rx, int ry, uint16_t color);
void zl_fill_ellipse(int cx, int cy, int rx, int ry, uint16_t color);

void zl_draw_round_rect(int x, int y, int w, int h, int r, uint16_t color);
void zl_fill_round_rect(int x, int y, int w, int h, int r, uint16_t color);

void zl_draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint16_t color);
void zl_fill_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint16_t color);

/* pts = x0,y0,x1,y1,... (n vertices, n <= ZL_POLY_MAX). Fill is even-odd,
 * handles concave shapes, and draws the outline too so edges are exact. */
void zl_draw_polygon(const int *pts, int n, uint16_t color);
void zl_fill_polygon(const int *pts, int n, uint16_t color);

/* Annulus sector. r_in = 0 gives a pie slice, a0=0,a1=360 a full ring. */
void zl_fill_ring_sector(int cx, int cy, int r_in, int r_out,
                         float a0_deg, float a1_deg, uint16_t color);
/* Arc of given thickness (>=1) whose OUTER radius is r. */
void zl_draw_arc(int cx, int cy, int r, float a0_deg, float a1_deg,
                 int thick, uint16_t color);

/* width <= 1 -> plain line. width >= 2 -> filled quad, round caps if >= 3. */
void zl_draw_thick_line(int x0, int y0, int x1, int y1, int width, uint16_t color);

void zl_draw_bezier3(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3,
                     uint16_t color);
void zl_draw_bezier2(int x0, int y0, int x1, int y1, int x2, int y2, uint16_t color);

void zl_fill_rect_gradient(int x, int y, int w, int h, uint16_t c0, uint16_t c1,
                           bool vertical);

#endif
