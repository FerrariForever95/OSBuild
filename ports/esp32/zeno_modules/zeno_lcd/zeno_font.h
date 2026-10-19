/*
 * zeno_font.h -- bitmap font engine (.zfont format) + 1bpp bitmap blitter.
 *
 * .zfont (little-endian), produced by tools/ttf2zfont.py:
 *   header  16 B : "ZFNT" | ver u8 (=1) | bpp u8 (1|4) | height u8 | ascent u8
 *                  | first u16 | count u16 | 4 B reserved
 *   table   count * 12 B : off u32 | w u8 | h u8 | adv u8 | xoff i8 | yoff i8 | 3 pad
 *                  (off is relative to the bitmap blob; yoff is measured from the
 *                   top of the line cell; w==0/h==0 = blank glyph, e.g. space)
 *   blob    rows packed MSB-first, each row padded to a whole byte
 *                  1bpp: 8 px/byte   4bpp: 2 px/byte (high nibble first, 0..15 = alpha)
 * Code points first .. first+count-1 are covered (one contiguous range).
 * Every offset is bounds-checked at draw time, so a corrupt buffer cannot
 * read out of range.
 *
 * Locking: [LOCK] -- call with the bus lock held.
 */
#ifndef ZENO_FONT_H
#define ZENO_FONT_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct {
    const uint8_t *table;
    const uint8_t *blob;
    size_t         blob_len;
    uint8_t        bpp, height, ascent;
    uint16_t       first, count;
} zl_font_t;

typedef struct {
    const char    *name;
    const uint8_t *data;
    size_t         len;
} zl_builtin_font_t;

/* generated into zeno_font_data.c by tools/gen_builtin_fonts.py */
extern const zl_builtin_font_t zl_builtin_fonts[];
extern const int               zl_builtin_font_count;

bool zl_font_parse(const uint8_t *buf, size_t len, zl_font_t *out);

/* bg < 0 => transparent. Returns the x just past the last glyph drawn on
 * the last line. '\n' starts a new line at x. scale is clamped to 1..16. */
int  zl_text(int x, int y, const char *utf8, size_t len, const zl_font_t *f,
             int scale, uint16_t fg, int32_t bg);
void zl_text_size(const char *utf8, size_t len, const zl_font_t *f, int scale,
                  int *w, int *h);

/* 1bpp bitmap (rows padded to bytes, MSB first). bg < 0 => transparent. */
void zl_blit_mono(int x, int y, int w, int h, const uint8_t *bits,
                  int scale, uint16_t fg, int32_t bg);

#endif
