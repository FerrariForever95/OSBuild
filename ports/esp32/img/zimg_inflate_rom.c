/*
 * zimg_inflate_rom.c -- inflate backend for the ESP32-S3 build: the miniz
 * "tinfl" decompressor that lives in chip ROM (same one pngle.c uses), so it
 * costs no flash. Not compiled for the host tests (they use zlib instead).
 */
#include <string.h>
#include "zimg_port.h"
#include "esp_idf_version.h"
#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 0, 0)
#include "miniz.h"
#else
#include "rom/miniz.h"
#endif

struct zi_inflate {
    tinfl_decompressor d;
};

zi_inflate_t *zi_inflate_new(void)
{
    zi_inflate_t *z = zi_alloc(sizeof(*z));
    if (z) {
        tinfl_init(&z->d);
    }
    return z;
}

void zi_inflate_free(zi_inflate_t *z)
{
    if (z) {
        zi_free(z, sizeof(*z));
    }
}

int zi_inflate_run(zi_inflate_t *z, uint8_t *ring, const uint8_t *in, size_t *in_n,
                   uint8_t *out_pos, size_t *out_n, int has_more_input)
{
    mz_uint32 flags = TINFL_FLAG_PARSE_ZLIB_HEADER;
    if (has_more_input) {
        flags |= TINFL_FLAG_HAS_MORE_INPUT;
    }
    tinfl_status st = tinfl_decompress(&z->d, (const mz_uint8 *)in, in_n,
                                       (mz_uint8 *)ring, (mz_uint8 *)out_pos, out_n, flags);
    if (st < 0) return -1;
    if (st == TINFL_STATUS_DONE) return 0;
    if (st == TINFL_STATUS_NEEDS_MORE_INPUT) return 1;
    return 2;   /* TINFL_STATUS_HAS_MORE_OUTPUT */
}
