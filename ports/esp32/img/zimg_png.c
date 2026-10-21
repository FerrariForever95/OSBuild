/*
 * zimg_png.c -- streaming PNG decoder.
 *
 * Handles every standard variant: gray / RGB / palette / gray+alpha / RGBA,
 * bit depths 1, 2, 4, 8, 16, tRNS transparency, and Adam7 interlacing.
 * Memory use is one 32 KiB inflate ring + ~11 KiB decompressor + two scanline
 * buffers + the output image; the file is never held in RAM.
 * Chunk CRCs and the zlib Adler-32 are not verified (speed; the inflater still
 * rejects structurally corrupt data).
 */
#include <string.h>
#include "zimg.h"
#include "zimg_port.h"

#define IN_BUF 1024

typedef struct {
    const zi_reader_t *rd;
    /* IDAT input stream */
    uint8_t inbuf[IN_BUF];
    size_t in_pos, in_len;
    uint32_t chunk_left;    /* bytes left in the current IDAT chunk            */
    int idat_done;
    int err;
    /* header */
    uint32_t w, h;
    uint8_t depth, ctype, interlace;
    uint8_t channels, bpp;
    uint8_t plte[256 * 3];
    uint16_t plte_n;
    uint8_t trns_a[256];     /* palette alpha (default 255)                    */
    uint16_t trns_key[3];    /* tRNS colour key for ctype 0 / 2                */
    int has_key;
} png_t;

static int rd_exact(const zi_reader_t *rd, uint8_t *dst, size_t n)
{
    return rd->read(rd->ctx, dst, n) == n ? ZI_OK : ZI_ERR_IO;
}

static int skip_bytes(const zi_reader_t *rd, uint32_t n)
{
    uint8_t tmp[64];
    while (n) {
        size_t c = n > sizeof(tmp) ? sizeof(tmp) : n;
        if (rd_exact(rd, tmp, c)) return ZI_ERR_IO;
        n -= (uint32_t)c;
    }
    return ZI_OK;
}

static inline uint32_t be32(const uint8_t *p)
{
    return ((uint32_t)p[0] << 24) | ((uint32_t)p[1] << 16) | ((uint32_t)p[2] << 8) | p[3];
}

static const uint8_t PNG_SIG[8] = {0x89, 'P', 'N', 'G', 0x0D, 0x0A, 0x1A, 0x0A};

/* Reads signature + IHDR; leaves the reader right after IHDR's CRC. */
static int read_ihdr(png_t *p)
{
    uint8_t b[8 + 8 + 13 + 4];
    if (rd_exact(p->rd, b, sizeof(b))) return ZI_ERR_IO;
    if (memcmp(b, PNG_SIG, 8) != 0) return ZI_ERR_FORMAT;
    if (be32(b + 8) != 13 || memcmp(b + 12, "IHDR", 4) != 0) return ZI_ERR_FORMAT;
    const uint8_t *h = b + 16;
    p->w = be32(h); p->h = be32(h + 4);
    p->depth = h[8]; p->ctype = h[9];
    if (h[10] != 0 || h[11] != 0 || h[12] > 1) return ZI_ERR_UNSUPPORTED;
    p->interlace = h[12];
    if (p->w == 0 || p->h == 0) return ZI_ERR_FORMAT;
    if (p->w > 65535 || p->h > 65535) return ZI_ERR_TOO_BIG;
    switch (p->ctype) {
    case 0: p->channels = 1; if (p->depth != 1 && p->depth != 2 && p->depth != 4 && p->depth != 8 && p->depth != 16) return ZI_ERR_FORMAT; break;
    case 2: p->channels = 3; if (p->depth != 8 && p->depth != 16) return ZI_ERR_FORMAT; break;
    case 3: p->channels = 1; if (p->depth != 1 && p->depth != 2 && p->depth != 4 && p->depth != 8) return ZI_ERR_FORMAT; break;
    case 4: p->channels = 2; if (p->depth != 8 && p->depth != 16) return ZI_ERR_FORMAT; break;
    case 6: p->channels = 4; if (p->depth != 8 && p->depth != 16) return ZI_ERR_FORMAT; break;
    default: return ZI_ERR_FORMAT;
    }
    uint32_t bits = (uint32_t)p->channels * p->depth;
    p->bpp = (uint8_t)((bits + 7) / 8);
    return ZI_OK;
}

int zi_png_info(const zi_reader_t *rd, uint16_t *w, uint16_t *h)
{
    png_t *p = zi_alloc(sizeof(*p));
    if (!p) return ZI_ERR_NOMEM;
    p->rd = rd;
    int r = read_ihdr(p);
    if (r == ZI_OK) { *w = (uint16_t)p->w; *h = (uint16_t)p->h; }
    zi_free(p, sizeof(*p));
    return r;
}

/* ---- IDAT byte supply --------------------------------------------------- */
/* Advance to the next IDAT chunk. Returns 1 if one was found. */
static int next_idat(png_t *p)
{
    uint8_t hdr[8];
    for (;;) {
        if (rd_exact(p->rd, hdr, 8)) { p->err = ZI_ERR_IO; return 0; }
        uint32_t len = be32(hdr);
        if (memcmp(hdr + 4, "IDAT", 4) == 0) {
            p->chunk_left = len;
            return 1;
        }
        if (memcmp(hdr + 4, "IEND", 4) == 0) return 0;
        /* ancillary or critical chunk between/after IDATs: skip data + CRC */
        if (skip_bytes(p->rd, len + 4)) { p->err = ZI_ERR_IO; return 0; }
    }
}

/* Refill inbuf. Returns bytes available (0 = no more IDAT data). */
static size_t refill(png_t *p)
{
    p->in_pos = p->in_len = 0;
    while (p->chunk_left == 0) {
        if (p->idat_done) return 0;
        if (!next_idat(p)) { p->idat_done = 1; return 0; }
    }
    size_t want = p->chunk_left > IN_BUF ? IN_BUF : p->chunk_left;
    size_t got = p->rd->read(p->rd->ctx, p->inbuf, want);
    if (got != want) { p->err = ZI_ERR_IO; p->idat_done = 1; return 0; }
    p->chunk_left -= (uint32_t)got;
    p->in_len = got;
    if (p->chunk_left == 0) {
        uint8_t crc[4];
        if (rd_exact(p->rd, crc, 4)) { p->err = ZI_ERR_IO; p->idat_done = 1; }
    }
    return got;
}

/* ---- scanline machinery -------------------------------------------------- */
typedef struct {
    png_t *p;
    zi_sink_t *sink;
    uint8_t *cur, *prev;     /* [1 + rowbytes] / [rowbytes] each plus bpp pad */
    uint8_t *rgba;           /* w * 4                                          */
    /* position in the image */
    int pass;                /* 0..6 for Adam7, 0 for sequential              */
    uint32_t pw, ph;         /* current pass dimensions                       */
    uint32_t py;             /* current row within the pass                   */
    size_t rowbytes;         /* bytes per row of the current pass (no filter) */
    size_t fill;             /* bytes in cur (incl. filter byte)              */
    int finished;
} dec_t;

static const uint8_t ADAM_XS[7] = {0, 4, 0, 2, 0, 1, 0};
static const uint8_t ADAM_YS[7] = {0, 0, 4, 0, 2, 0, 1};
static const uint8_t ADAM_DX[7] = {8, 8, 4, 4, 2, 2, 1};
static const uint8_t ADAM_DY[7] = {8, 8, 8, 4, 4, 2, 2};

static int start_pass(dec_t *d)
{
    png_t *p = d->p;
    for (;;) {
        if (p->interlace) {
            if (d->pass > 6) return 0;
            d->pw = (p->w + ADAM_DX[d->pass] - 1 - ADAM_XS[d->pass]) / ADAM_DX[d->pass];
            d->ph = (p->h + ADAM_DY[d->pass] - 1 - ADAM_YS[d->pass]) / ADAM_DY[d->pass];
            if (p->w <= ADAM_XS[d->pass] || p->h <= ADAM_YS[d->pass]) { d->pw = 0; d->ph = 0; }
        } else {
            if (d->pass > 0) return 0;
            d->pw = p->w; d->ph = p->h;
        }
        if (d->pw && d->ph) break;
        d->pass++;
    }
    d->py = 0;
    d->rowbytes = ((size_t)d->pw * p->channels * p->depth + 7) / 8;
    memset(d->prev, 0, d->rowbytes + p->bpp);
    d->fill = 0;
    return 1;
}

static inline int paeth(int a, int b, int c)
{
    int pa = b - c, pb = a - c, pc = pa + pb;
    if (pa < 0) pa = -pa;
    if (pb < 0) pb = -pb;
    if (pc < 0) pc = -pc;
    if (pa <= pb && pa <= pc) return a;
    return pb <= pc ? b : c;
}

/* In-place unfilter of cur[1..rowbytes] against prev[0..rowbytes). Returns 0 on bad filter. */
static int unfilter(uint8_t *cur, const uint8_t *prev, size_t n, uint8_t bpp)
{
    uint8_t ft = cur[0];
    uint8_t *x = cur + 1;
    size_t i;
    switch (ft) {
    case 0: break;
    case 1: for (i = bpp; i < n; i++) x[i] = (uint8_t)(x[i] + x[i - bpp]); break;
    case 2: for (i = 0; i < n; i++) x[i] = (uint8_t)(x[i] + prev[i]); break;
    case 3:
        for (i = 0; i < bpp && i < n; i++) x[i] = (uint8_t)(x[i] + (prev[i] >> 1));
        for (i = bpp; i < n; i++) x[i] = (uint8_t)(x[i] + ((x[i - bpp] + prev[i]) >> 1));
        break;
    case 4:
        for (i = 0; i < bpp && i < n; i++) x[i] = (uint8_t)(x[i] + prev[i]);
        for (i = bpp; i < n; i++) x[i] = (uint8_t)(x[i] + paeth(x[i - bpp], prev[i], prev[i - bpp]));
        break;
    default: return 0;
    }
    return 1;
}

/* Convert `n` pixels of an unfiltered row to RGBA. */
static void row_to_rgba(const png_t *p, const uint8_t *s, uint32_t n, uint8_t *o)
{
    uint32_t i;
    switch (p->ctype) {
    case 0: /* gray */
        for (i = 0; i < n; i++, o += 4) {
            uint32_t v; uint8_t g;
            if (p->depth == 16) {
                v = ((uint32_t)s[i * 2] << 8) | s[i * 2 + 1]; g = s[i * 2];
            } else if (p->depth == 8) {
                v = s[i]; g = (uint8_t)v;
            } else {
                uint32_t bit = i * p->depth;
                v = (s[bit >> 3] >> (8 - p->depth - (bit & 7))) & ((1u << p->depth) - 1u);
                g = (uint8_t)(v * 255u / ((1u << p->depth) - 1u));
            }
            o[0] = o[1] = o[2] = g;
            o[3] = (p->has_key && v == p->trns_key[0]) ? 0 : 255;
        }
        break;
    case 2: /* RGB */
        for (i = 0; i < n; i++, o += 4) {
            if (p->depth == 8) {
                o[0] = s[i * 3]; o[1] = s[i * 3 + 1]; o[2] = s[i * 3 + 2];
                o[3] = (p->has_key && o[0] == p->trns_key[0] && o[1] == p->trns_key[1] &&
                        o[2] == p->trns_key[2]) ? 0 : 255;
            } else {
                const uint8_t *q = s + i * 6;
                o[0] = q[0]; o[1] = q[2]; o[2] = q[4];
                uint16_t r16 = (uint16_t)((q[0] << 8) | q[1]);
                uint16_t g16 = (uint16_t)((q[2] << 8) | q[3]);
                uint16_t b16 = (uint16_t)((q[4] << 8) | q[5]);
                o[3] = (p->has_key && r16 == p->trns_key[0] && g16 == p->trns_key[1] &&
                        b16 == p->trns_key[2]) ? 0 : 255;
            }
        }
        break;
    case 3: /* palette */
        for (i = 0; i < n; i++, o += 4) {
            uint32_t idx;
            if (p->depth == 8) {
                idx = s[i];
            } else {
                uint32_t bit = i * p->depth;
                idx = (s[bit >> 3] >> (8 - p->depth - (bit & 7))) & ((1u << p->depth) - 1u);
            }
            if (idx >= p->plte_n) idx = 0;
            o[0] = p->plte[idx * 3]; o[1] = p->plte[idx * 3 + 1]; o[2] = p->plte[idx * 3 + 2];
            o[3] = p->trns_a[idx];
        }
        break;
    case 4: /* gray + alpha */
        for (i = 0; i < n; i++, o += 4) {
            uint8_t g = (p->depth == 8) ? s[i * 2] : s[i * 4];
            uint8_t a = (p->depth == 8) ? s[i * 2 + 1] : s[i * 4 + 2];
            o[0] = o[1] = o[2] = g; o[3] = a;
        }
        break;
    default: /* 6: RGBA */
        if (p->depth == 8) {
            memcpy(o, s, (size_t)n * 4);
        } else {
            for (i = 0; i < n; i++, o += 4) {
                const uint8_t *q = s + i * 8;
                o[0] = q[0]; o[1] = q[2]; o[2] = q[4]; o[3] = q[6];
            }
        }
        break;
    }
}

/* Called when cur holds a complete filtered row. */
static int row_done(dec_t *d)
{
    png_t *p = d->p;
    if (!unfilter(d->cur, d->prev, d->rowbytes, p->bpp)) return ZI_ERR_FORMAT;
    row_to_rgba(p, d->cur + 1, d->pw, d->rgba);
    if (!p->interlace) {
        zi_sink_row(d->sink, (uint16_t)d->py, d->rgba);
    } else {
        uint32_t y = ADAM_YS[d->pass] + d->py * ADAM_DY[d->pass];
        for (uint32_t i = 0; i < d->pw; i++) {
            const uint8_t *q = d->rgba + i * 4;
            zi_sink_pixel(d->sink, ADAM_XS[d->pass] + i * ADAM_DX[d->pass], y, q[0], q[1], q[2], q[3]);
        }
    }
    /* this row becomes `prev` (copy: keeps cur's buffer reusable) */
    memcpy(d->prev, d->cur + 1, d->rowbytes);
    d->py++;
    d->fill = 0;
    if (d->py >= d->ph) {
        d->pass++;
        if (!start_pass(d)) d->finished = 1;
    }
    return ZI_OK;
}

/* Feed decompressed bytes into the scanline assembler. */
static int feed(dec_t *d, const uint8_t *b, size_t n)
{
    while (n && !d->finished) {
        size_t need = d->rowbytes + 1 - d->fill;
        size_t c = n < need ? n : need;
        memcpy(d->cur + d->fill, b, c);
        d->fill += c; b += c; n -= c;
        if (d->fill == d->rowbytes + 1) {
            int r = row_done(d);
            if (r) return r;
        }
    }
    return ZI_OK;   /* surplus bytes after the last row are ignored */
}

int zi_png_decode(const zi_reader_t *rd, const zi_opts_t *o, zi_image_t *out)
{
    int ret;
    png_t *p = zi_alloc(sizeof(*p));
    dec_t d;
    zi_sink_t sink;
    zi_inflate_t *z = NULL;
    uint8_t *ring = NULL, *bufs = NULL;
    size_t bufs_len = 0, rgba_len = 0;
    uint8_t *rgba = NULL;
    int sink_ok = 0;

    memset(out, 0, sizeof(*out));
    if (!p) return ZI_ERR_NOMEM;
    p->rd = rd;
    memset(p->trns_a, 255, sizeof(p->trns_a));
    memset(&d, 0, sizeof(d));

    if ((ret = read_ihdr(p)) != ZI_OK) goto done;

    /* Chunks before the first IDAT: PLTE, tRNS (others skipped). */
    for (;;) {
        uint8_t hdr[8];
        if (rd_exact(rd, hdr, 8)) { ret = ZI_ERR_IO; goto done; }
        uint32_t len = be32(hdr);
        if (memcmp(hdr + 4, "IDAT", 4) == 0) {
            p->chunk_left = len;
            if (len == 0) { /* empty IDAT: consume CRC, look at the next */
                if (skip_bytes(rd, 4)) { ret = ZI_ERR_IO; goto done; }
                p->chunk_left = 0;
            }
            break;
        }
        if (memcmp(hdr + 4, "IEND", 4) == 0) { ret = ZI_ERR_FORMAT; goto done; }
        if (memcmp(hdr + 4, "PLTE", 4) == 0) {
            if (len % 3 || len > 768 || len == 0) { ret = ZI_ERR_FORMAT; goto done; }
            if (rd_exact(rd, p->plte, len)) { ret = ZI_ERR_IO; goto done; }
            p->plte_n = (uint16_t)(len / 3);
            if (skip_bytes(rd, 4)) { ret = ZI_ERR_IO; goto done; }
        } else if (memcmp(hdr + 4, "tRNS", 4) == 0 && len <= 768) {
            uint8_t t[768];
            if (rd_exact(rd, t, len)) { ret = ZI_ERR_IO; goto done; }
            if (p->ctype == 3) {
                for (uint32_t i = 0; i < len && i < 256; i++) p->trns_a[i] = t[i];
            } else if (p->ctype == 0 && len >= 2) {
                p->trns_key[0] = (uint16_t)((t[0] << 8) | t[1]);
                p->has_key = 1;
            } else if (p->ctype == 2 && len >= 6) {
                for (int i = 0; i < 3; i++) p->trns_key[i] = (uint16_t)((t[i * 2] << 8) | t[i * 2 + 1]);
                p->has_key = 1;
            }
            if (skip_bytes(rd, 4)) { ret = ZI_ERR_IO; goto done; }
        } else {
            if (skip_bytes(rd, len + 4)) { ret = ZI_ERR_IO; goto done; }
        }
    }
    if (p->ctype == 3 && p->plte_n == 0) { ret = ZI_ERR_FORMAT; goto done; }
    /* tRNS key for depth<16 is stored as the raw sample value; for 8-bit RGB the
     * comparison above is against the 8-bit channel values, which is what the
     * spec means (low byte significant). */

    uint16_t div;
    if ((ret = zi_pick_div(o, (uint16_t)p->w, (uint16_t)p->h, 0, &div)) != ZI_OK) goto done;
    if ((ret = zi_sink_init(&sink, o, (uint16_t)p->w, (uint16_t)p->h, div)) != ZI_OK) goto done;
    sink_ok = 1;

    size_t max_row = ((size_t)p->w * p->channels * p->depth + 7) / 8;
    bufs_len = (max_row + 1 + p->bpp) + (max_row + p->bpp);
    rgba_len = (size_t)p->w * 4;
    bufs = zi_alloc(bufs_len);
    rgba = zi_alloc(rgba_len);
    ring = zi_alloc(ZI_RING_SIZE);
    z = zi_inflate_new();
    if (!bufs || !rgba || !ring || !z) { ret = ZI_ERR_NOMEM; goto done; }

    d.p = p; d.sink = &sink; d.rgba = rgba;
    d.cur = bufs;
    d.prev = bufs + (max_row + 1 + p->bpp);
    d.pass = 0;
    if (!start_pass(&d)) { ret = ZI_ERR_FORMAT; goto done; }

    size_t ring_pos = 0;
    int status = 1;
    while (!d.finished) {
        size_t avail = p->in_len - p->in_pos;
        if (avail == 0 && status == 1) {
            avail = refill(p);
            if (avail == 0) {
                if (p->err) { ret = p->err; goto done; }
                break;                      /* ran out of IDAT before image was complete */
            }
        }
        size_t in_n = avail, out_n = ZI_RING_SIZE - ring_pos;
        /* has_more_input is always 1: a zlib stream split across IDAT chunks cannot
         * be told apart from a finished one until the next chunk header is read. */
        status = zi_inflate_run(z, ring, p->inbuf + p->in_pos, &in_n, ring + ring_pos, &out_n, 1);
        if (status < 0) { ret = ZI_ERR_DATA; goto done; }
        p->in_pos += in_n;
        if (out_n) {
            if ((ret = feed(&d, ring + ring_pos, out_n)) != ZI_OK) goto done;
            ring_pos = (ring_pos + out_n) & (ZI_RING_SIZE - 1);
        }
        if (status == 0) break;             /* zlib stream ended */
    }
    if (!d.finished) { ret = ZI_ERR_IO; goto done; }   /* truncated: refuse a half image */

    zi_sink_flush(&sink);
    zi_sink_take(&sink, out);
    ret = ZI_OK;

done:
    if (z) zi_inflate_free(z);
    if (ring) zi_free(ring, ZI_RING_SIZE);
    if (bufs) zi_free(bufs, bufs_len);
    if (rgba) zi_free(rgba, rgba_len);
    if (sink_ok) zi_sink_free(&sink);
    zi_free(p, sizeof(*p));
    return ret;
}
