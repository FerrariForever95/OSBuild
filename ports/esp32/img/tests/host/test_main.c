/* CLI: test_zimg <png|jpeg|bmp> <file> <out.raw> [fit_w fit_h div bg_hex]
 * Prints:  OK w h div src_w src_h   or   ERR <code> <text>.
 * Also checks that out size == w*h*2 and exercises info(). */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "zimg.h"

typedef struct { FILE *f; } fctx;
static size_t rd(void *c, uint8_t *d, size_t n) { return fread(d, 1, n, ((fctx *)c)->f); }

int main(int argc, char **argv)
{
    if (argc < 4) return 2;
    FILE *f = fopen(argv[2], "rb");
    if (!f) { printf("ERR -99 cannot open\n"); return 1; }
    fctx c = {f};
    zi_reader_t r = {rd, &c};
    zi_opts_t o = {0};
    if (argc > 4) o.fit_w = (uint16_t)atoi(argv[4]);
    if (argc > 5) o.fit_h = (uint16_t)atoi(argv[5]);
    if (argc > 6) o.div = (uint16_t)atoi(argv[6]);
    if (argc > 7) o.bg = (uint32_t)strtoul(argv[7], NULL, 16);
    uint16_t iw = 0, ih = 0;
    int (*info)(const zi_reader_t *, uint16_t *, uint16_t *) =
        !strcmp(argv[1], "png") ? zi_png_info : !strcmp(argv[1], "jpeg") ? zi_jpeg_info : zi_bmp_info;
    int ie = info(&r, &iw, &ih);
    rewind(f);
    zi_image_t im;
    int (*dec)(const zi_reader_t *, const zi_opts_t *, zi_image_t *) =
        !strcmp(argv[1], "png") ? zi_png_decode : !strcmp(argv[1], "jpeg") ? zi_jpeg_decode : zi_bmp_decode;
    int e = dec(&r, &o, &im);
    if (e) { printf("ERR %d %s\n", e, zi_strerror(e)); return 1; }
    if (im.len != (size_t)im.width * im.height * 2) { printf("ERR -98 len mismatch\n"); return 1; }
    if (ie == 0 && (iw != im.src_w || ih != im.src_h)) { printf("ERR -97 info mismatch %d %d\n", iw, ih); return 1; }
    FILE *o2 = fopen(argv[3], "wb");
    fwrite(im.data, 1, im.len, o2);
    fclose(o2);
    printf("OK %u %u %u %u %u\n", im.width, im.height, im.div, im.src_w, im.src_h);
    zi_image_free(&im);
    fclose(f);
    return 0;
}
