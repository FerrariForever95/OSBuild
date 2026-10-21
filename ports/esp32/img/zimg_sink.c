/*
 * zimg_sink.c -- RGBA rows/pixels -> RGB565 big-endian buffer, with optional
 * integer box-filter shrinking and alpha blending onto a background colour.
 */
#include <string.h>
#include "zimg.h"
#include "zimg_port.h"

const char *zi_strerror(int err)
{
    switch (err) {
    case ZI_OK:              return "ok";
    case ZI_ERR_NOMEM:       return "out of memory";
    case ZI_ERR_IO:          return "unexpected end of file";
    case ZI_ERR_FORMAT:      return "bad or corrupt image";
    case ZI_ERR_UNSUPPORTED: return "unsupported image variant";
    case ZI_ERR_TOO_BIG:     return "image too large for fit";
    case ZI_ERR_DATA:        return "corrupt compressed data";
    default:                 return "image error";
    }
}

void zi_image_free(zi_image_t *img)
{
    if (img->data) {
        zi_free(img->data, img->alloc_len);
    }
    memset(img, 0, sizeof(*img));
}

int zi_pick_div(const zi_opts_t *o, uint16_t sw, uint16_t sh, int pow2_max, uint16_t *div)
{
    uint32_t d = o->div ? o->div : 1;
    if (o->fit_w && o->fit_h) {
        uint32_t dw = ((uint32_t)sw + o->fit_w - 1) / o->fit_w;
        uint32_t dh = ((uint32_t)sh + o->fit_h - 1) / o->fit_h;
        if (dw > d) d = dw;
        if (dh > d) d = dh;
    }
    if (pow2_max) {
        uint32_t p = 1;
        while (p < d) p <<= 1;
        if (p > (uint32_t)pow2_max) return ZI_ERR_TOO_BIG;
        d = p;
    }
    if (d > 255) return ZI_ERR_TOO_BIG;
    *div = (uint16_t)d;
    return ZI_OK;
}

int zi_sink_init(zi_sink_t *s, const zi_opts_t *o, uint16_t sw, uint16_t sh, uint16_t div)
{
    memset(s, 0, sizeof(*s));
    if (sw == 0 || sh == 0 || div == 0) return ZI_ERR_FORMAT;
    s->sw = sw; s->sh = sh; s->div = div;
    s->ow = (uint16_t)(((uint32_t)sw + div - 1) / div);
    s->oh = (uint16_t)(((uint32_t)sh + div - 1) / div);
    s->bg_r = (o->bg >> 16) & 0xFF;
    s->bg_g = (o->bg >> 8) & 0xFF;
    s->bg_b = o->bg & 0xFF;
    s->cur_oy = -1;
    s->out_len = (size_t)s->ow * s->oh * 2u;
    s->out = zi_alloc(s->out_len);
    if (!s->out) return ZI_ERR_NOMEM;
    if (div > 1) {
        s->acc_len = (size_t)s->ow * 3u * sizeof(uint32_t);
        s->acc = zi_alloc(s->acc_len);
        if (!s->acc) {
            zi_free(s->out, s->out_len);
            s->out = NULL;
            return ZI_ERR_NOMEM;
        }
    }
    return ZI_OK;
}

void zi_sink_free(zi_sink_t *s)
{
    if (s->acc) { zi_free(s->acc, s->acc_len); s->acc = NULL; }
    if (s->out) { zi_free(s->out, s->out_len); s->out = NULL; }
}

static inline void put565(uint8_t *d, uint32_t r, uint32_t g, uint32_t b)
{
    d[0] = (uint8_t)((r & 0xF8) | (g >> 5));
    d[1] = (uint8_t)(((g & 0x1C) << 3) | (b >> 3));
}

/* straight-alpha blend of one channel onto the background */
static inline uint32_t blend(uint32_t c, uint32_t a, uint32_t bg)
{
    return (c * a + bg * (255u - a) + 127u) / 255u;
}

void zi_sink_pixel(zi_sink_t *s, uint32_t x, uint32_t y,
                   uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    if (s->div > 1) {
        if ((x % s->div) || (y % s->div)) return;
        x /= s->div; y /= s->div;
    }
    if (x >= s->ow || y >= s->oh) return;
    uint32_t R = r, G = g, B = b;
    if (a != 255) {
        R = blend(R, a, s->bg_r); G = blend(G, a, s->bg_g); B = blend(B, a, s->bg_b);
    }
    put565(s->out + ((size_t)y * s->ow + x) * 2u, R, G, B);
}

void zi_sink_flush(zi_sink_t *s)
{
    if (s->div <= 1 || s->cur_oy < 0 || s->rows == 0) return;
    uint32_t oy = (uint32_t)s->cur_oy;
    uint8_t *d = s->out + (size_t)oy * s->ow * 2u;
    for (uint32_t ox = 0; ox < s->ow; ox++) {
        uint32_t cx = s->sw - ox * s->div;
        if (cx > s->div) cx = s->div;
        uint32_t n = cx * s->rows;
        uint32_t *a = &s->acc[ox * 3u];
        put565(d + ox * 2u, (a[0] + n / 2) / n, (a[1] + n / 2) / n, (a[2] + n / 2) / n);
        a[0] = a[1] = a[2] = 0;
    }
    s->rows = 0;
}

void zi_sink_row(zi_sink_t *s, uint16_t y, const uint8_t *p)
{
    if (y >= s->sh) return;
    if (s->div <= 1) {
        uint8_t *d = s->out + (size_t)y * s->ow * 2u;
        for (uint32_t x = 0; x < s->sw; x++, p += 4, d += 2) {
            uint32_t a = p[3];
            if (a == 255) {
                put565(d, p[0], p[1], p[2]);
            } else {
                put565(d, blend(p[0], a, s->bg_r), blend(p[1], a, s->bg_g), blend(p[2], a, s->bg_b));
            }
        }
        return;
    }
    int32_t oy = (int32_t)(y / s->div);
    if (oy != s->cur_oy) {
        zi_sink_flush(s);
        s->cur_oy = oy;
    }
    for (uint32_t x = 0; x < s->sw; x++, p += 4) {
        uint32_t a = p[3], r = p[0], g = p[1], b = p[2];
        if (a != 255) {
            r = blend(r, a, s->bg_r); g = blend(g, a, s->bg_g); b = blend(b, a, s->bg_b);
        }
        uint32_t *acc = &s->acc[(x / s->div) * 3u];
        acc[0] += r; acc[1] += g; acc[2] += b;
    }
    s->rows++;
}

void zi_sink_take(zi_sink_t *s, zi_image_t *img)
{
    img->width = s->ow;
    img->height = s->oh;
    img->src_w = s->sw;
    img->src_h = s->sh;
    img->div = s->div;
    img->data = s->out;
    img->len = s->out_len;
    img->alloc_len = s->out_len;
    s->out = NULL;
}
