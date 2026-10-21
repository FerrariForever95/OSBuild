/*
 * zimg.h -- core image decoders for the zeno_img MicroPython module.
 *
 * Pure C: no MicroPython and no ESP-IDF includes in this header, so the same
 * code is unit-tested on a desktop compiler (tests/host/). Everything that
 * touches the platform (allocator, inflate backend) lives in zimg_port.h.
 *
 * Output of every decoder is the pixel format `lcd.blit` expects:
 * RGB565, big-endian (high byte first), row-major, no padding, so
 * len(buf) == width * height * 2 exactly.
 */
#ifndef ZIMG_H
#define ZIMG_H

#include <stddef.h>
#include <stdint.h>

/* ---- error codes (zi_strerror() gives the text) ------------------------- */
enum {
    ZI_OK = 0,
    ZI_ERR_NOMEM = -1,      /* allocation failed                              */
    ZI_ERR_IO = -2,         /* reader returned fewer bytes than required       */
    ZI_ERR_FORMAT = -3,     /* not a valid file of this type / corrupt         */
    ZI_ERR_UNSUPPORTED = -4,/* valid file, feature not supported               */
    ZI_ERR_TOO_BIG = -5,    /* cannot be reduced enough to fit                 */
    ZI_ERR_DATA = -6        /* corrupt compressed data                         */
};
const char *zi_strerror(int err);

/* ---- input: a plain "read up to n bytes" callback ------------------------ */
typedef struct {
    /* Return the number of bytes actually read: n normally, less only at
     * end of data. Return 0 at EOF or on error. Must not return > n. */
    size_t (*read)(void *ctx, uint8_t *dst, size_t n);
    void *ctx;
} zi_reader_t;

/* ---- options -------------------------------------------------------------- */
typedef struct {
    uint16_t fit_w, fit_h;  /* if both non-zero: shrink (integer divider) until
                               the result fits inside fit_w x fit_h            */
    uint16_t div;           /* minimum integer shrink divider (0 or 1 = none)   */
    uint32_t bg;            /* 0xRRGGBB, transparent pixels are blended on it   */
} zi_opts_t;

/* ---- result --------------------------------------------------------------- */
typedef struct {
    uint16_t width, height; /* size of `data` in pixels (after shrinking)       */
    uint16_t src_w, src_h;  /* size of the file's image                         */
    uint16_t div;           /* divider actually applied (1 = full size)         */
    uint8_t *data;          /* width*height*2 bytes, RGB565 big-endian          */
    size_t len;             /* == width*height*2                                */
    size_t alloc_len;       /* bytes actually allocated (>= len); for zi_free   */
} zi_image_t;

void zi_image_free(zi_image_t *img);

int zi_png_decode(const zi_reader_t *rd, const zi_opts_t *o, zi_image_t *out);
int zi_jpeg_decode(const zi_reader_t *rd, const zi_opts_t *o, zi_image_t *out);
int zi_bmp_decode(const zi_reader_t *rd, const zi_opts_t *o, zi_image_t *out);

/* Header-only size query; reads as little of the file as possible. */
int zi_png_info(const zi_reader_t *rd, uint16_t *w, uint16_t *h);
int zi_jpeg_info(const zi_reader_t *rd, uint16_t *w, uint16_t *h);
int zi_bmp_info(const zi_reader_t *rd, uint16_t *w, uint16_t *h);

/* ---- shared internals (used by the three decoders) ---------------------- */
typedef struct {
    uint16_t sw, sh;        /* source size                                      */
    uint16_t ow, oh;        /* output size                                      */
    uint16_t div;
    uint8_t bg_r, bg_g, bg_b;
    uint8_t *out;           /* ow*oh*2                                          */
    uint32_t *acc;          /* ow*3 box-filter accumulators (div > 1 only)      */
    int32_t cur_oy;         /* output row being accumulated, -1 = none          */
    uint32_t rows;          /* source rows accumulated into cur_oy              */
    size_t out_len, acc_len;
} zi_sink_t;

/* Pick the divider from opts and the source size. pow2_max: if non-zero the
 * divider is rounded up to a power of two and must be <= pow2_max (JPEG). */
int zi_pick_div(const zi_opts_t *o, uint16_t sw, uint16_t sh, int pow2_max, uint16_t *div);
int zi_sink_init(zi_sink_t *s, const zi_opts_t *o, uint16_t sw, uint16_t sh, uint16_t div);
void zi_sink_free(zi_sink_t *s);                 /* frees acc and, if not taken, out */
/* One whole source row of RGBA (4 bytes/pixel, straight alpha), rows of the
 * same output row must arrive contiguously (any direction). Box-filtered. */
void zi_sink_row(zi_sink_t *s, uint16_t y, const uint8_t *rgba);
/* Single pixel (used for Adam7 interlaced PNG): nearest-neighbour when div>1. */
void zi_sink_pixel(zi_sink_t *s, uint32_t x, uint32_t y,
                   uint8_t r, uint8_t g, uint8_t b, uint8_t a);
void zi_sink_flush(zi_sink_t *s);                /* call after the last row  */
void zi_sink_take(zi_sink_t *s, zi_image_t *img);/* move the buffer to img   */

#endif
