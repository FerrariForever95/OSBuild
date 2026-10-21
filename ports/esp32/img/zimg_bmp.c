/*
 * zimg_bmp.c -- BMP decoder.
 *
 * Supports uncompressed 1/4/8-bit palette, 16-bit (555 or bitfields), 24-bit
 * and 32-bit images, bottom-up and top-down, OS/2 core headers and Windows
 * 40/52/56/108/124-byte headers. RLE4/RLE8 and embedded JPEG/PNG are rejected
 * with "unsupported". Alpha in 32-bit BMPs is ignored (treated as opaque):
 * most writers leave that byte 0.
 */
#include <string.h>
#include "zimg.h"
#include "zimg_port.h"

static inline uint32_t le16(const uint8_t *p) { return p[0] | ((uint32_t)p[1] << 8); }
static inline uint32_t le32(const uint8_t *p)
{
    return p[0] | ((uint32_t)p[1] << 8) | ((uint32_t)p[2] << 16) | ((uint32_t)p[3] << 24);
}

typedef struct {
    uint32_t data_off;
    uint32_t hsize;
    int32_t w, h;            /* h < 0: top-down */
    uint16_t bpp;
    uint32_t comp;
    uint32_t clr_used;
    uint32_t mask[3];        /* R G B for 16/32 bit */
    uint8_t pal[256 * 3];    /* RGB */
    uint32_t pal_n;
    uint32_t consumed;       /* bytes read from the stream so far */
} bmp_t;

static int rd(const zi_reader_t *r, uint8_t *dst, size_t n)
{
    return r->read(r->ctx, dst, n) == n ? ZI_OK : ZI_ERR_IO;
}

static int read_headers(const zi_reader_t *r, bmp_t *b, int need_palette)
{
    uint8_t h[18];
    int e;
    memset(b, 0, sizeof(*b));
    if ((e = rd(r, h, 18))) return e;
    if (h[0] != 'B' || h[1] != 'M') return ZI_ERR_FORMAT;
    b->data_off = le32(h + 10);
    b->hsize = le32(h + 14);
    b->consumed = 18;
    if (b->hsize != 12 && b->hsize != 40 && b->hsize != 52 && b->hsize != 56 &&
        b->hsize != 64 && b->hsize != 108 && b->hsize != 124) {
        return ZI_ERR_FORMAT;
    }
    uint8_t d[124];
    if ((e = rd(r, d, b->hsize - 4))) return e;
    b->consumed += b->hsize - 4;
    if (b->hsize == 12) {
        b->w = (int32_t)le16(d); b->h = (int32_t)le16(d + 2);
        b->bpp = (uint16_t)le16(d + 6);
        b->comp = 0;
    } else {
        b->w = (int32_t)le32(d); b->h = (int32_t)le32(d + 4);
        b->bpp = (uint16_t)le16(d + 10);
        b->comp = le32(d + 12);
        b->clr_used = le32(d + 28);
    }
    if (b->w <= 0 || b->h == 0 || b->h == INT32_MIN) return ZI_ERR_FORMAT;
    if (b->w > 65535 || (b->h > 65535) || (b->h < -65535)) return ZI_ERR_TOO_BIG;
    if (!need_palette) return ZI_OK;

    if (b->comp == 1 || b->comp == 2) return ZI_ERR_UNSUPPORTED;      /* RLE   */
    if (b->comp == 4 || b->comp == 5) return ZI_ERR_UNSUPPORTED;      /* JPEG/PNG inside */
    if (b->comp != 0 && b->comp != 3 && b->comp != 6) return ZI_ERR_UNSUPPORTED;
    if (b->bpp != 1 && b->bpp != 4 && b->bpp != 8 && b->bpp != 16 && b->bpp != 24 && b->bpp != 32)
        return ZI_ERR_UNSUPPORTED;

    if (b->bpp == 16 || b->bpp == 32) {
        if (b->comp == 3 || b->comp == 6) {
            if (b->hsize >= 52) {
                b->mask[0] = le32(d + 36); b->mask[1] = le32(d + 40); b->mask[2] = le32(d + 44);
            } else if (b->hsize == 40) {
                uint8_t m[12];
                if ((e = rd(r, m, 12))) return e;
                b->consumed += 12;
                b->mask[0] = le32(m); b->mask[1] = le32(m + 4); b->mask[2] = le32(m + 8);
            }
            if (!b->mask[0] || !b->mask[1] || !b->mask[2]) return ZI_ERR_FORMAT;
        } else if (b->bpp == 16) {
            b->mask[0] = 0x7C00; b->mask[1] = 0x03E0; b->mask[2] = 0x001F;
        } else {
            b->mask[0] = 0x00FF0000; b->mask[1] = 0x0000FF00; b->mask[2] = 0x000000FF;
        }
    }
    if (b->bpp <= 8) {
        uint32_t n = b->clr_used ? b->clr_used : (1u << b->bpp);
        if (n > 256 || n > (1u << b->bpp)) n = 1u << b->bpp;
        uint32_t esz = (b->hsize == 12) ? 3 : 4;
        uint8_t e4[4];
        for (uint32_t i = 0; i < n; i++) {
            if ((e = rd(r, e4, esz))) return e;
            b->pal[i * 3] = e4[2]; b->pal[i * 3 + 1] = e4[1]; b->pal[i * 3 + 2] = e4[0];
        }
        b->consumed += n * esz;
        b->pal_n = n;
    }
    return ZI_OK;
}

int zi_bmp_info(const zi_reader_t *rd_, uint16_t *w, uint16_t *h)
{
    bmp_t b;
    int e = read_headers(rd_, &b, 0);
    if (e == ZI_OK) {
        *w = (uint16_t)b.w;
        *h = (uint16_t)(b.h < 0 ? -b.h : b.h);
    }
    return e;
}

/* scale a masked channel to 0..255 */
static inline uint8_t chan(uint32_t v, uint32_t mask)
{
    int shift = 0;
    while (!((mask >> shift) & 1u)) shift++;
    uint32_t m = mask >> shift;
    uint32_t c = (v & mask) >> shift;
    int bits = 0;
    while (m >> bits) bits++;
    if (bits >= 8) return (uint8_t)(c >> (bits - 8));
    /* widen by bit replication: 0 -> 0x00, max -> 0xFF (5 bits: c<<3 | c>>2) */
    uint32_t r = c << (8 - bits);
    for (int sh = bits; sh < 8; sh <<= 1) r |= r >> sh;
    return (uint8_t)r;
}

int zi_bmp_decode(const zi_reader_t *r, const zi_opts_t *o, zi_image_t *out)
{
    bmp_t b;
    zi_sink_t sink;
    int sink_ok = 0, e;
    uint8_t *row = NULL, *rgba = NULL;
    size_t row_len = 0, rgba_len = 0;

    memset(out, 0, sizeof(*out));
    if ((e = read_headers(r, &b, 1))) return e;
    if (b.data_off < b.consumed) return ZI_ERR_FORMAT;
    /* skip gap (colour-profile data, padding) up to the pixel array */
    for (uint32_t gap = b.data_off - b.consumed; gap;) {
        uint8_t t[64];
        size_t c = gap > sizeof(t) ? sizeof(t) : gap;
        if (rd(r, t, c)) return ZI_ERR_IO;
        gap -= (uint32_t)c;
    }

    uint16_t W = (uint16_t)b.w;
    uint16_t H = (uint16_t)(b.h < 0 ? -b.h : b.h);
    int bottom_up = b.h > 0;
    uint16_t div;
    if ((e = zi_pick_div(o, W, H, 0, &div))) return e;
    if ((e = zi_sink_init(&sink, o, W, H, div))) return e;
    sink_ok = 1;

    row_len = (((size_t)W * b.bpp + 31) / 32) * 4;
    rgba_len = (size_t)W * 4;
    row = zi_alloc(row_len);
    rgba = zi_alloc(rgba_len);
    if (!row || !rgba) { e = ZI_ERR_NOMEM; goto done; }

    for (uint32_t i = 0; i < H; i++) {
        if (rd(r, row, row_len)) { e = ZI_ERR_IO; goto done; }
        uint8_t *q = rgba;
        switch (b.bpp) {
        case 1: case 4: case 8:
            for (uint32_t x = 0; x < W; x++, q += 4) {
                uint32_t idx;
                if (b.bpp == 8) idx = row[x];
                else {
                    uint32_t bit = x * b.bpp;
                    idx = (row[bit >> 3] >> (8 - b.bpp - (bit & 7))) & ((1u << b.bpp) - 1u);
                }
                if (idx >= b.pal_n) idx = 0;
                q[0] = b.pal[idx * 3]; q[1] = b.pal[idx * 3 + 1]; q[2] = b.pal[idx * 3 + 2]; q[3] = 255;
            }
            break;
        case 16:
            for (uint32_t x = 0; x < W; x++, q += 4) {
                uint32_t v = le16(row + x * 2);
                q[0] = chan(v, b.mask[0]); q[1] = chan(v, b.mask[1]); q[2] = chan(v, b.mask[2]); q[3] = 255;
            }
            break;
        case 24:
            for (uint32_t x = 0; x < W; x++, q += 4) {
                q[0] = row[x * 3 + 2]; q[1] = row[x * 3 + 1]; q[2] = row[x * 3]; q[3] = 255;
            }
            break;
        default: /* 32 */
            if (b.mask[0] == 0x00FF0000 && b.mask[1] == 0x0000FF00 && b.mask[2] == 0x000000FF) {
                for (uint32_t x = 0; x < W; x++, q += 4) {
                    q[0] = row[x * 4 + 2]; q[1] = row[x * 4 + 1]; q[2] = row[x * 4]; q[3] = 255;
                }
            } else {
                for (uint32_t x = 0; x < W; x++, q += 4) {
                    uint32_t v = le32(row + x * 4);
                    q[0] = chan(v, b.mask[0]); q[1] = chan(v, b.mask[1]); q[2] = chan(v, b.mask[2]); q[3] = 255;
                }
            }
            break;
        }
        zi_sink_row(&sink, (uint16_t)(bottom_up ? (H - 1u - i) : i), rgba);
    }
    zi_sink_flush(&sink);
    zi_sink_take(&sink, out);
    e = ZI_OK;
done:
    if (row) zi_free(row, row_len);
    if (rgba) zi_free(rgba, rgba_len);
    if (sink_ok) zi_sink_free(&sink);
    return e;
}
