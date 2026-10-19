/*
 * zeno_font.c -- see zeno_font.h.
 *
 * Opaque text (bg given): each glyph cell (advance x line height) is sent as
 * ONE address window, streamed row by row, with 4bpp coverage blended
 * against bg -> proper anti-aliasing. Transparent text (bg < 0): only lit
 * pixels are drawn as horizontal runs; the panel cannot be read back, so
 * anti-aliased coverage is thresholded (>= 8/15) instead of blended.
 */
#include <string.h>
#include "lcd_bus.h"
#include "ili9488_panel.h"
#include "zeno_gfx.h"
#include "zeno_font.h"
#include "zeno_pure.h"

#define ZF_HDR 16u
#define ZF_ENT 12u

static inline uint32_t rd16(const uint8_t *p) { return (uint32_t)p[0] | ((uint32_t)p[1] << 8); }
static inline uint32_t rd32(const uint8_t *p) { return rd16(p) | (rd16(p + 2) << 16); }

bool zl_font_parse(const uint8_t *b, size_t len, zl_font_t *f)
{
    if (b == NULL || len < ZF_HDR || memcmp(b, "ZFNT", 4) != 0) return false;
    if (b[4] != 1 || (b[5] != 1 && b[5] != 4) || b[6] == 0) return false;
    uint32_t count = rd16(b + 10);
    if (count == 0) return false;
    size_t table_end = ZF_HDR + (size_t)count * ZF_ENT;
    if (table_end > len) return false;
    f->bpp = b[5]; f->height = b[6]; f->ascent = b[7];
    f->first = (uint16_t)rd16(b + 8); f->count = (uint16_t)count;
    f->table = b + ZF_HDR;
    f->blob = b + table_end;
    f->blob_len = len - table_end;
    return true;
}

/* ------------------------------------------------------------ bitmap engine */

typedef struct { const uint8_t *data; int stride; int bpp; } bmp_t;

static inline int bmp_alpha(const bmp_t *b, int x, int y)       /* 0..15 */
{
    if (b->bpp == 1) {
        return (b->data[y * b->stride + (x >> 3)] & (0x80 >> (x & 7))) ? 15 : 0;
    }
    uint8_t v = b->data[y * b->stride + (x >> 1)];
    return (x & 1) ? (v & 0x0F) : (v >> 4);
}

/* Draw a cw x ch cell at (x,y) (scaled by `scale`). Bitmap b (bw x bh) sits at
 * (bx,by) inside the cell. bg >= 0 paints the whole cell. */
static void bmp_draw(int x, int y, int cw, int ch, const bmp_t *b,
                     int bx, int by, int bw, int bh, int scale,
                     uint16_t fg, int32_t bg)
{
    if (bg >= 0) {
        int vx = x, vy = y, vw = cw * scale, vh = ch * scale;
        if (!zl_clip_rect(&vx, &vy, &vw, &vh)) return;

        uint16_t lut[16];
        for (int a = 0; a < 16; a++) lut[a] = zl_blend565((uint16_t)bg, fg, (uint8_t)(a * 17));

        uint8_t *row = zl_gfx_linebuf();
        zl_set_window(vx, vy, vx + vw - 1, vy + vh - 1);
        int last_cy = -1;
        for (int py = vy; py < vy + vh; py++) {
            int cy = (py - y) / scale;
            if (cy != last_cy) {                              /* rebuild only on a new source row */
                last_cy = cy;
                int brow = cy - by;
                bool row_in = brow >= 0 && brow < bh;
                for (int i = 0; i < vw; i++) {
                    int bcol = (vx + i - x) / scale - bx;
                    int a = (row_in && bcol >= 0 && bcol < bw) ? bmp_alpha(b, bcol, brow) : 0;
                    uint16_t col = lut[a];
                    row[2 * i] = (uint8_t)(col >> 8);
                    row[2 * i + 1] = (uint8_t)(col & 0xFF);
                }
            }
            zl_bus_write_bytes(row, (size_t)vw * 2u);
        }
        return;
    }

    if (bw <= 0 || bh <= 0) return;
    int X0 = x + bx * scale, Y0 = y + by * scale;
    if (X0 + bw * scale <= 0 || Y0 + bh * scale <= 0 ||
        X0 >= (int)zl_panel_width() || Y0 >= (int)zl_panel_height()) return;
    for (int r = 0; r < bh; r++) {
        int py = Y0 + r * scale;
        if (py + scale <= 0 || py >= (int)zl_panel_height()) continue;
        int c = 0;
        while (c < bw) {
            while (c < bw && bmp_alpha(b, c, r) < 8) c++;
            if (c >= bw) break;
            int s = c;
            while (c < bw && bmp_alpha(b, c, r) >= 8) c++;
            zl_fill_rect(X0 + s * scale, py, (c - s) * scale, scale, fg);
        }
    }
}

void zl_blit_mono(int x, int y, int w, int h, const uint8_t *bits, int scale,
                  uint16_t fg, int32_t bg)
{
    if (w <= 0 || h <= 0) return;
    if (scale < 1) scale = 1;
    if (scale > 16) scale = 16;
    bmp_t b = { bits, (w + 7) / 8, 1 };
    bmp_draw(x, y, w, h, &b, 0, 0, w, h, scale, fg, bg);
}

/* --------------------------------------------------------------------- text */

typedef struct { uint32_t off; uint8_t w, h, adv; int8_t xoff, yoff; } glyph_t;

static bool glyph_get(const zl_font_t *f, uint32_t cp, glyph_t *g)
{
    if (cp < f->first || cp - f->first >= f->count) return false;
    const uint8_t *e = f->table + (size_t)(cp - f->first) * ZF_ENT;
    g->off = rd32(e); g->w = e[4]; g->h = e[5]; g->adv = e[6];
    g->xoff = (int8_t)e[7]; g->yoff = (int8_t)e[8];
    return true;
}

static int glyph_advance(const zl_font_t *f, uint32_t cp)
{
    glyph_t g;
    if (!glyph_get(f, cp, &g) && !glyph_get(f, '?', &g)) return f->height / 2 > 0 ? f->height / 2 : 1;
    return g.adv;
}

static int draw_glyph(int x, int y, const zl_font_t *f, uint32_t cp, int scale,
                      uint16_t fg, int32_t bg)
{
    glyph_t g;
    if (!glyph_get(f, cp, &g) && !glyph_get(f, '?', &g)) {
        int adv = f->height / 2 > 0 ? f->height / 2 : 1;      /* unknown char: blank cell */
        bmp_draw(x, y, adv, f->height, NULL, 0, 0, 0, 0, scale, fg, bg);
        return adv * scale;
    }
    bmp_t b = { NULL, 0, f->bpp };
    int bw = g.w, bh = g.h;
    if (bw > 0 && bh > 0) {
        b.stride = f->bpp == 1 ? (bw + 7) / 8 : (bw + 1) / 2;
        size_t need = (size_t)b.stride * (size_t)bh;
        if ((size_t)g.off > f->blob_len || need > f->blob_len - (size_t)g.off) {
            bw = bh = 0;                                      /* corrupt entry: draw blank */
        } else {
            b.data = f->blob + g.off;
        }
    } else {
        bw = bh = 0;
    }
    bmp_draw(x, y, g.adv, f->height, &b, g.xoff, g.yoff, bw, bh, scale, fg, bg);
    return g.adv * scale;
}

int zl_text(int x, int y, const char *s, size_t len, const zl_font_t *f,
            int scale, uint16_t fg, int32_t bg)
{
    if (scale < 1) scale = 1;
    if (scale > 16) scale = 16;
    const uint8_t *u = (const uint8_t *)s;
    int cx = x;
    size_t i = 0;
    while (i < len) {
        uint32_t cp = zl_utf8_next(u, len, &i);
        if (cp == '\n') { cx = x; y += f->height * scale; continue; }
        if (cp < 32 || cp == 0x7F) continue;
        cx += draw_glyph(cx, y, f, cp, scale, fg, bg);
    }
    return cx;
}

void zl_text_size(const char *s, size_t len, const zl_font_t *f, int scale, int *w, int *h)
{
    if (scale < 1) scale = 1;
    if (scale > 16) scale = 16;
    const uint8_t *u = (const uint8_t *)s;
    int cur = 0, maxw = 0, lines = 1;
    size_t i = 0;
    while (i < len) {
        uint32_t cp = zl_utf8_next(u, len, &i);
        if (cp == '\n') { if (cur > maxw) maxw = cur; cur = 0; lines++; continue; }
        if (cp < 32 || cp == 0x7F) continue;
        cur += glyph_advance(f, cp) * scale;
    }
    if (cur > maxw) maxw = cur;
    *w = maxw;
    *h = lines * f->height * scale;
}
