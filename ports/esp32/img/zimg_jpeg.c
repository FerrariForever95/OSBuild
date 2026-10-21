/*
 * zimg_jpeg.c -- baseline JPEG decoder on top of Tiny JPEG Decompressor
 * (TJpgDec). On the ESP32-S3 the decoder is in chip ROM (rom/tjpgd.h), so it
 * adds no flash. Baseline (sequential Huffman) JPEG only; progressive files are
 * rejected with "unsupported".
 *
 * Shrinking uses TJpgDec's native 1/2, 1/4, 1/8 scaling (cheaper than decoding
 * full size), so for JPEG the divider is always 1, 2, 4 or 8 and `fit` fails
 * with "too large" beyond 8x.
 */
#include <string.h>
#include "zimg.h"
#include "zimg_port.h"

#if defined(ZIMG_HOST)
  #include "mock_tjpgd.h"          /* host tests: stand-in with the ROM API */
  typedef unsigned int zi_jd_out_t;
#elif defined(ZIMG_JPEG_EXTERNAL_TJPGD)
  #include "tjpgd.h"               /* drop tjpgd.c/.h next to this file */
  typedef int zi_jd_out_t;
#else
  #include "esp_rom_caps.h"
  #if defined(ESP_ROM_HAS_JPEG_DECODE)
    #include "rom/tjpgd.h"
    typedef unsigned int zi_jd_out_t;   /* ROM version returns UINT */
  #else
    #error "This chip has no ROM TJpgDec: add tjpgd.c/.h and define ZIMG_JPEG_EXTERNAL_TJPGD"
  #endif
#endif

typedef struct {
    const zi_reader_t *rd;
    zi_sink_t *sink;         /* NULL until sized (direct writes, see below)  */
    uint8_t *out;
    uint16_t ow, oh;
    uint32_t bg;
    uint16_t max_x, max_y;   /* highest written pixel + 1                    */
    int io_err;
} jpg_dev_t;

static unsigned int in_cb(JDEC *jd, uint8_t *buf, unsigned int len)
{
    jpg_dev_t *dev = (jpg_dev_t *)jd->device;
    if (buf) {
        size_t n = dev->rd->read(dev->rd->ctx, buf, len);
        if (n < len) dev->io_err = 1;
        return (unsigned int)n;
    }
    uint8_t tmp[64];
    unsigned int left = len;
    while (left) {
        size_t c = left > sizeof(tmp) ? sizeof(tmp) : left;
        size_t n = dev->rd->read(dev->rd->ctx, tmp, c);
        if (n < c) { dev->io_err = 1; return len - left + (unsigned int)n; }
        left -= (unsigned int)c;
    }
    return len;
}

static zi_jd_out_t out_cb(JDEC *jd, void *bitmap, JRECT *rect)
{
    jpg_dev_t *dev = (jpg_dev_t *)jd->device;
    const uint8_t *in = (const uint8_t *)bitmap;
    unsigned int rw = (unsigned int)rect->right - rect->left + 1u;
    for (unsigned int y = rect->top; y <= rect->bottom; y++) {
        for (unsigned int x = rect->left; x <= rect->right; x++, in += 3) {
            if (y < dev->oh && x < dev->ow) {
                uint8_t *d = dev->out + ((size_t)y * dev->ow + x) * 2u;
                d[0] = (uint8_t)((in[0] & 0xF8) | (in[1] >> 5));
                d[1] = (uint8_t)(((in[1] & 0x1C) << 3) | (in[2] >> 3));
                if (x + 1u > dev->max_x) dev->max_x = (uint16_t)(x + 1u);
                if (y + 1u > dev->max_y) dev->max_y = (uint16_t)(y + 1u);
            }
        }
    }
    (void)rw;
    return 1;
}

static int jr_to_err(int jr)
{
    switch (jr) {
    case JDR_INP:  return ZI_ERR_IO;
    case JDR_MEM1: case JDR_MEM2: return ZI_ERR_NOMEM;
    case JDR_FMT1: return ZI_ERR_FORMAT;
    case JDR_FMT3: return ZI_ERR_UNSUPPORTED;   /* progressive / unsupported */
    default:       return ZI_ERR_FORMAT;
    }
}

/* Marker scan for info(): finds the SOF segment, no decoding. */
int zi_jpeg_info(const zi_reader_t *rd, uint16_t *w, uint16_t *h)
{
    uint8_t b[4];
    if (rd->read(rd->ctx, b, 2) != 2) return ZI_ERR_IO;
    if (b[0] != 0xFF || b[1] != 0xD8) return ZI_ERR_FORMAT;
    for (int guard = 0; guard < 512; guard++) {
        uint8_t m;
        do {
            if (rd->read(rd->ctx, &m, 1) != 1) return ZI_ERR_IO;
        } while (m != 0xFF);
        do {
            if (rd->read(rd->ctx, &m, 1) != 1) return ZI_ERR_IO;
        } while (m == 0xFF);
        if (m == 0xD8 || m == 0x01 || (m >= 0xD0 && m <= 0xD7)) continue;   /* no length */
        if (m == 0xD9 || m == 0xDA) return ZI_ERR_FORMAT;
        if (rd->read(rd->ctx, b, 2) != 2) return ZI_ERR_IO;
        uint32_t len = ((uint32_t)b[0] << 8) | b[1];
        if (len < 2) return ZI_ERR_FORMAT;
        if ((m >= 0xC0 && m <= 0xC3) || (m >= 0xC5 && m <= 0xC7) ||
            (m >= 0xC9 && m <= 0xCB) || (m >= 0xCD && m <= 0xCF)) {
            uint8_t s[5];
            if (len < 7 || rd->read(rd->ctx, s, 5) != 5) return ZI_ERR_IO;
            *h = (uint16_t)((s[1] << 8) | s[2]);
            *w = (uint16_t)((s[3] << 8) | s[4]);
            return (*w && *h) ? ZI_OK : ZI_ERR_FORMAT;
        }
        for (uint32_t left = len - 2; left;) {
            uint8_t t[64];
            size_t c = left > sizeof(t) ? sizeof(t) : left;
            if (rd->read(rd->ctx, t, c) != c) return ZI_ERR_IO;
            left -= (uint32_t)c;
        }
    }
    return ZI_ERR_FORMAT;
}

int zi_jpeg_decode(const zi_reader_t *rd, const zi_opts_t *o, zi_image_t *out)
{
    static const unsigned int POOLS[2] = {16384, 8192};   /* TJpgDec needs ~3.1 KB minimum */
    JDEC jd;
    jpg_dev_t dev;
    void *pool = NULL;
    size_t pool_len = 0;
    uint8_t *buf = NULL;
    size_t buf_len = 0;
    int jr = JDR_MEM1, ret;

    memset(out, 0, sizeof(*out));
    memset(&dev, 0, sizeof(dev));
    dev.rd = rd;

    /* jd_prepare consumes the stream, so a too-small pool cannot be retried on a
     * non-seekable source: take the biggest pool that allocates (16 KB, else 8 KB). */
    for (int i = 0; i < 2 && !pool; i++) {
        pool = zi_alloc(POOLS[i]);
        if (pool) pool_len = POOLS[i];
    }
    if (!pool) return ZI_ERR_NOMEM;

    jr = jd_prepare(&jd, in_cb, pool, (unsigned int)pool_len, &dev);
    if (jr != JDR_OK) { ret = dev.io_err ? ZI_ERR_IO : jr_to_err(jr); goto done; }

    uint16_t sw = (uint16_t)jd.width, sh = (uint16_t)jd.height;
    uint16_t div;
    if ((ret = zi_pick_div(o, sw, sh, 8, &div)) != ZI_OK) goto done;
    int scale = (div == 1) ? 0 : (div == 2) ? 1 : (div == 4) ? 2 : 3;

    dev.ow = (uint16_t)(((uint32_t)sw + div - 1) / div);
    dev.oh = (uint16_t)(((uint32_t)sh + div - 1) / div);
    buf_len = (size_t)dev.ow * dev.oh * 2u;
    buf = zi_alloc(buf_len);
    if (!buf) { ret = ZI_ERR_NOMEM; goto done; }
    dev.out = buf;
    /* background fill so any edge pixel the decoder does not cover is not garbage */
    {
        uint8_t hi = (uint8_t)((((o->bg >> 16) & 0xF8)) | (((o->bg >> 8) & 0xFF) >> 5));
        uint8_t lo = (uint8_t)(((((o->bg >> 8) & 0xFF) & 0x1C) << 3) | ((o->bg & 0xFF) >> 3));
        if (hi || lo) for (size_t i = 0; i < buf_len; i += 2) { buf[i] = hi; buf[i + 1] = lo; }
    }

    jr = jd_decomp(&jd, out_cb, (uint8_t)scale);
    if (jr != JDR_OK) { ret = dev.io_err ? ZI_ERR_IO : jr_to_err(jr); goto done; }
    if (dev.max_x == 0 || dev.max_y == 0) { ret = ZI_ERR_FORMAT; goto done; }

    /* The decoder's real descaled size may be one pixel smaller than ceil(w/div).
     * lcd.blit needs len == w*h*2 exactly, so repack to the size that was written. */
    uint16_t fw = dev.max_x, fh = dev.max_y;
    if (fw != dev.ow) {
        for (uint16_t y = 1; y < fh; y++) {
            memmove(buf + (size_t)y * fw * 2u, buf + (size_t)y * dev.ow * 2u, (size_t)fw * 2u);
        }
    }
    out->width = fw; out->height = fh;
    out->src_w = sw; out->src_h = sh; out->div = div;
    out->data = buf;
    out->len = (size_t)fw * fh * 2u;
    out->alloc_len = buf_len;
    buf = NULL;
    ret = ZI_OK;
done:
    if (buf) zi_free(buf, buf_len);
    zi_free(pool, pool_len);
    return ret;
}
