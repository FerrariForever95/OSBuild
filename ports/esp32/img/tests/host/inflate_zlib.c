/* Host implementation of the zimg_port.h inflate interface using zlib. */
#include <stdlib.h>
#include <string.h>
#include <zlib.h>
#include "zimg_port.h"

struct zi_inflate { z_stream s; int inited; };

zi_inflate_t *zi_inflate_new(void)
{
    zi_inflate_t *z = calloc(1, sizeof(*z));
    if (z && inflateInit(&z->s) == Z_OK) { z->inited = 1; return z; }
    free(z);
    return NULL;
}
void zi_inflate_free(zi_inflate_t *z) { if (z) { if (z->inited) inflateEnd(&z->s); free(z); } }

int zi_inflate_run(zi_inflate_t *z, uint8_t *ring, const uint8_t *in, size_t *in_n,
                   uint8_t *out_pos, size_t *out_n, int has_more)
{
    (void)ring; (void)has_more;
    z->s.next_in = (Bytef *)in; z->s.avail_in = (uInt)*in_n;
    z->s.next_out = out_pos;    z->s.avail_out = (uInt)*out_n;
    int r = inflate(&z->s, Z_NO_FLUSH);
    *in_n -= z->s.avail_in;
    *out_n -= z->s.avail_out;
    if (r == Z_STREAM_END) return 0;
    if (r == Z_OK || r == Z_BUF_ERROR) return z->s.avail_out == 0 ? 2 : 1;
    return -1;
}
