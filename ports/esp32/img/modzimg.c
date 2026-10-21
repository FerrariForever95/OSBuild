/*
 * modzimg.c -- MicroPython bindings: modules `png`, `jpeg` (alias `jpg`),
 * `bmp` and the auto-detecting `img`.
 *
 *   import png
 *   p = png.open("/sd/logo.png")             # path, open file, or bytes
 *   lcd.blit(10, 20, p.width, p.height, p.buf)
 *
 * Every module offers:
 *   open(src, fit=None, div=1, bg=0x000000) -> Image
 *   decode(...)  same as open
 *   info(src) -> (width, height)             # header only, no pixel decode
 * Image has .width .height .buf (bytearray, RGB565 big-endian, exactly
 * width*height*2 bytes) .src_size .div, and supports the buffer protocol, so
 * lcd.blit(x, y, p.width, p.height, p) works as well.
 */
#include <string.h>
#include "py/obj.h"
#include "py/runtime.h"
#include "py/builtin.h"
#include "py/stream.h"

#include "zimg.h"

/* ------------------------------------------------------------------------ */
/* Image object                                                              */

typedef struct {
    mp_obj_base_t base;
    mp_obj_t buf;               /* bytearray over the pixel memory */
    uint16_t width, height, src_w, src_h, div;
} zimage_obj_t;

static mp_int_t zimage_get_buffer(mp_obj_t self_in, mp_buffer_info_t *bi, mp_uint_t flags)
{
    zimage_obj_t *self = MP_OBJ_TO_PTR(self_in);
    (void)flags;
    return mp_get_buffer(self->buf, bi, MP_BUFFER_READ) ? 0 : 1;
}

static void zimage_attr(mp_obj_t self_in, qstr attr, mp_obj_t *dest)
{
    zimage_obj_t *self = MP_OBJ_TO_PTR(self_in);
    if (dest[0] != MP_OBJ_NULL) {
        return;                               /* read-only: leave dest[1] set -> AttributeError */
    }
    switch (attr) {
    case MP_QSTR_width:  dest[0] = MP_OBJ_NEW_SMALL_INT(self->width); break;
    case MP_QSTR_height: dest[0] = MP_OBJ_NEW_SMALL_INT(self->height); break;
    case MP_QSTR_buf:    dest[0] = self->buf; break;
    case MP_QSTR_div:    dest[0] = MP_OBJ_NEW_SMALL_INT(self->div); break;
    case MP_QSTR_size:
        dest[0] = mp_obj_new_tuple(2, (mp_obj_t[]){MP_OBJ_NEW_SMALL_INT(self->width),
                                                   MP_OBJ_NEW_SMALL_INT(self->height)});
        break;
    case MP_QSTR_src_size:
        dest[0] = mp_obj_new_tuple(2, (mp_obj_t[]){MP_OBJ_NEW_SMALL_INT(self->src_w),
                                                   MP_OBJ_NEW_SMALL_INT(self->src_h)});
        break;
    default: break;
    }
}

static void zimage_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind)
{
    zimage_obj_t *self = MP_OBJ_TO_PTR(self_in);
    (void)kind;
    mp_printf(print, "<Image %dx%d RGB565 (source %dx%d, div %d)>",
              self->width, self->height, self->src_w, self->src_h, self->div);
}

MP_DEFINE_CONST_OBJ_TYPE(
    zimage_type,
    MP_QSTR_Image,
    MP_TYPE_FLAG_NONE,
    print, zimage_print,
    attr, zimage_attr,
    buffer, zimage_get_buffer
    );

/* ------------------------------------------------------------------------ */
/* Sources: path / stream / bytes-like, with an 8-byte push-back for sniffing */

typedef struct {
    mp_obj_t stream;            /* MP_OBJ_NULL for memory sources */
    const mp_stream_p_t *sp;
    const uint8_t *mem;
    size_t mem_len, mem_pos;
    uint8_t pb[8];
    size_t pb_len, pb_pos;
    int io_error;
} zsrc_t;

static size_t zsrc_read(void *ctx, uint8_t *dst, size_t n)
{
    zsrc_t *s = ctx;
    size_t done = 0;
    while (done < n && s->pb_pos < s->pb_len) {
        dst[done++] = s->pb[s->pb_pos++];
    }
    if (s->stream == MP_OBJ_NULL) {
        size_t c = s->mem_len - s->mem_pos;
        if (c > n - done) c = n - done;
        memcpy(dst + done, s->mem + s->mem_pos, c);
        s->mem_pos += c;
        return done + c;
    }
    while (done < n) {
        int err = 0;
        mp_uint_t r = s->sp->read(s->stream, dst + done, n - done, &err);
        if (r == MP_STREAM_ERROR) { s->io_error = err ? err : 5; break; }
        if (r == 0) break;
        done += r;
    }
    return done;
}

/* Look at the first bytes without losing them: memory sources are just peeked,
 * streams get the bytes pushed back in front of the next read. */
static size_t zsrc_peek(zsrc_t *s, uint8_t *hdr)
{
    if (s->stream == MP_OBJ_NULL) {
        size_t n = s->mem_len < 8 ? s->mem_len : 8;
        memcpy(hdr, s->mem, n);
        return n;
    }
    s->pb_pos = 0;
    s->pb_len = 0;
    uint8_t tmp[8];
    size_t n = zsrc_read(s, tmp, 8);       /* pb_len is 0 here, so this reads the stream */
    memcpy(s->pb, tmp, n);
    memcpy(hdr, tmp, n);
    s->pb_len = n;
    return n;
}

typedef struct {
    int (*decode)(const zi_reader_t *, const zi_opts_t *, zi_image_t *);
    int (*info)(const zi_reader_t *, uint16_t *, uint16_t *);
    const char *name;
} zfmt_t;

static const zfmt_t FMT_PNG = {zi_png_decode, zi_png_info, "png"};
static const zfmt_t FMT_JPEG = {zi_jpeg_decode, zi_jpeg_info, "jpeg"};
static const zfmt_t FMT_BMP = {zi_bmp_decode, zi_bmp_info, "bmp"};

static const zfmt_t *sniff_format(const uint8_t *h, size_t n)
{
    if (n >= 8 && h[0] == 0x89 && h[1] == 'P' && h[2] == 'N' && h[3] == 'G') return &FMT_PNG;
    if (n >= 3 && h[0] == 0xFF && h[1] == 0xD8) return &FMT_JPEG;
    if (n >= 2 && h[0] == 'B' && h[1] == 'M') return &FMT_BMP;
    return NULL;
}

static NORETURN void raise_zi(const zfmt_t *f, int err, const zsrc_t *s)
{
    if (s && s->io_error) {
        mp_raise_OSError(s->io_error);
    }
    switch (err) {
    case ZI_ERR_NOMEM:
        mp_raise_msg(&mp_type_MemoryError, MP_ERROR_TEXT("not enough RAM for image"));
    case ZI_ERR_IO:
        mp_raise_msg_varg(&mp_type_EOFError, MP_ERROR_TEXT("%s: file truncated"), f ? f->name : "image");
    case ZI_ERR_UNSUPPORTED:
        mp_raise_msg_varg(&mp_type_NotImplementedError, MP_ERROR_TEXT("%s: unsupported variant"),
                          f ? f->name : "image");
    default:
        mp_raise_msg_varg(&mp_type_ValueError, MP_ERROR_TEXT("%s: %s"), f ? f->name : "image",
                          zi_strerror(err));
    }
}

/* Parse keyword arguments shared by open()/decode(). */
enum { ARG_src, ARG_fit, ARG_div, ARG_bg };
static void parse_args(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw, zi_opts_t *o,
                       mp_obj_t *src)
{
    static const mp_arg_t allowed[] = {
        {MP_QSTR_src, MP_ARG_REQUIRED | MP_ARG_OBJ, {.u_obj = MP_OBJ_NULL}},
        {MP_QSTR_fit, MP_ARG_KW_ONLY | MP_ARG_OBJ, {.u_obj = mp_const_none}},
        {MP_QSTR_div, MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = 1}},
        {MP_QSTR_bg, MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = 0}},
    };
    mp_arg_val_t a[MP_ARRAY_SIZE(allowed)];
    mp_arg_parse_all(n_args, pos_args, kw, MP_ARRAY_SIZE(allowed), allowed, a);
    memset(o, 0, sizeof(*o));
    *src = a[ARG_src].u_obj;
    if (a[ARG_fit].u_obj != mp_const_none) {
        mp_obj_t *items;
        size_t len;
        mp_obj_get_array(a[ARG_fit].u_obj, &len, &items);
        if (len != 2) mp_raise_ValueError(MP_ERROR_TEXT("fit must be (width, height)"));
        mp_int_t fw = mp_obj_get_int(items[0]), fh = mp_obj_get_int(items[1]);
        if (fw < 1 || fh < 1 || fw > 65535 || fh > 65535) mp_raise_ValueError(MP_ERROR_TEXT("bad fit size"));
        o->fit_w = (uint16_t)fw; o->fit_h = (uint16_t)fh;
    }
    if (a[ARG_div].u_int < 1 || a[ARG_div].u_int > 255) mp_raise_ValueError(MP_ERROR_TEXT("div must be 1..255"));
    o->div = (uint16_t)a[ARG_div].u_int;
    o->bg = (uint32_t)a[ARG_bg].u_int & 0xFFFFFF;
}

/* Open `src` into `s`. Returns the file object to close afterwards (or MP_OBJ_NULL). */
static mp_obj_t zsrc_open(zsrc_t *s, mp_obj_t src)
{
    mp_obj_t to_close = MP_OBJ_NULL;
    memset(s, 0, sizeof(*s));
    mp_buffer_info_t bi;
    if (mp_obj_is_str(src)) {
        mp_obj_t args[2] = {src, MP_OBJ_NEW_QSTR(MP_QSTR_rb)};
        src = mp_call_function_n_kw(MP_OBJ_FROM_PTR(&mp_builtin_open_obj), 2, 0, args);
        to_close = src;
    } else if (mp_get_buffer(src, &bi, MP_BUFFER_READ)) {
        s->stream = MP_OBJ_NULL;
        s->mem = bi.buf;
        s->mem_len = bi.len;
        return MP_OBJ_NULL;
    }
    s->stream = src;
    s->sp = mp_get_stream_raise(src, MP_STREAM_OP_READ);
    return to_close;
}

static void close_obj(mp_obj_t f)
{
    if (f != MP_OBJ_NULL) {
        mp_stream_close(f);
    }
}

static mp_obj_t make_image(zi_image_t *im)
{
    zimage_obj_t *o = mp_obj_malloc(zimage_obj_t, &zimage_type);
    /* bytearray over the decoder's heap block, no copy: length is exactly w*h*2 */
    o->buf = mp_obj_new_bytearray_by_ref(im->len, im->data);
    o->width = im->width; o->height = im->height;
    o->src_w = im->src_w; o->src_h = im->src_h; o->div = im->div;
    return MP_OBJ_FROM_PTR(o);
}

/* kind: fixed format, or NULL = sniff */
static mp_obj_t do_open(const zfmt_t *fixed, size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw)
{
    zi_opts_t o;
    mp_obj_t src;
    parse_args(n_args, pos_args, kw, &o, &src);

    zsrc_t s;
    zi_reader_t rd = {zsrc_read, &s};
    zi_image_t im;
    int err;
    const zfmt_t *f = fixed;

    mp_obj_t to_close = zsrc_open(&s, src);
    nlr_buf_t nlr;
    if (nlr_push(&nlr) == 0) {
        if (!f) {
            uint8_t hdr[8];
            size_t hn = zsrc_peek(&s, hdr);
            f = sniff_format(hdr, hn);
            if (!f) mp_raise_ValueError(MP_ERROR_TEXT("unknown image format"));
        }
        err = f->decode(&rd, &o, &im);
        nlr_pop();
    } else {
        close_obj(to_close);
        nlr_jump(nlr.ret_val);
    }
    close_obj(to_close);
    if (err != ZI_OK) raise_zi(f, err, &s);
    return make_image(&im);
}

static mp_obj_t do_info(const zfmt_t *fixed, mp_obj_t src)
{
    zsrc_t s;
    zi_reader_t rd = {zsrc_read, &s};
    uint16_t w = 0, h = 0;
    int err;
    const zfmt_t *f = fixed;
    mp_obj_t to_close = zsrc_open(&s, src);
    nlr_buf_t nlr;
    if (nlr_push(&nlr) == 0) {
        if (!f) {
            uint8_t hdr[8];
            size_t hn = zsrc_peek(&s, hdr);
            f = sniff_format(hdr, hn);
            if (!f) mp_raise_ValueError(MP_ERROR_TEXT("unknown image format"));
        }
        err = f->info(&rd, &w, &h);
        nlr_pop();
    } else {
        close_obj(to_close);
        nlr_jump(nlr.ret_val);
    }
    close_obj(to_close);
    if (err != ZI_OK) raise_zi(f, err, &s);
    return mp_obj_new_tuple(2, (mp_obj_t[]){MP_OBJ_NEW_SMALL_INT(w), MP_OBJ_NEW_SMALL_INT(h)});
}

#define DEFINE_FORMAT_MODULE(NAME, FMT)                                                         \
    static mp_obj_t NAME##_open(size_t n, const mp_obj_t *a, mp_map_t *kw)                      \
    { return do_open(&FMT, n, a, kw); }                                                         \
    static MP_DEFINE_CONST_FUN_OBJ_KW(NAME##_open_obj, 1, NAME##_open);                         \
    static mp_obj_t NAME##_info(mp_obj_t src) { return do_info(&FMT, src); }                    \
    static MP_DEFINE_CONST_FUN_OBJ_1(NAME##_info_obj, NAME##_info);                             \
    static const mp_rom_map_elem_t NAME##_globals_table[] = {                                   \
        {MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_##NAME)},                           \
        {MP_ROM_QSTR(MP_QSTR_open), MP_ROM_PTR(&NAME##_open_obj)},                              \
        {MP_ROM_QSTR(MP_QSTR_decode), MP_ROM_PTR(&NAME##_open_obj)},                            \
        {MP_ROM_QSTR(MP_QSTR_info), MP_ROM_PTR(&NAME##_info_obj)},                              \
        {MP_ROM_QSTR(MP_QSTR_Image), MP_ROM_PTR(&zimage_type)},                                 \
    };                                                                                          \
    static MP_DEFINE_CONST_DICT(NAME##_globals, NAME##_globals_table);                          \
    const mp_obj_module_t mp_module_##NAME = {                                                  \
        .base = {&mp_type_module},                                                              \
        .globals = (mp_obj_dict_t *)&NAME##_globals,                                            \
    };

DEFINE_FORMAT_MODULE(png, FMT_PNG)
DEFINE_FORMAT_MODULE(jpeg, FMT_JPEG)
DEFINE_FORMAT_MODULE(bmp, FMT_BMP)

/* `img`: detect the format from the file's first bytes */
static mp_obj_t img_open(size_t n, const mp_obj_t *a, mp_map_t *kw) { return do_open(NULL, n, a, kw); }
static MP_DEFINE_CONST_FUN_OBJ_KW(img_open_obj, 1, img_open);
static mp_obj_t img_info(mp_obj_t src) { return do_info(NULL, src); }
static MP_DEFINE_CONST_FUN_OBJ_1(img_info_obj, img_info);
static const mp_rom_map_elem_t img_globals_table[] = {
    {MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_img)},
    {MP_ROM_QSTR(MP_QSTR_open), MP_ROM_PTR(&img_open_obj)},
    {MP_ROM_QSTR(MP_QSTR_decode), MP_ROM_PTR(&img_open_obj)},
    {MP_ROM_QSTR(MP_QSTR_info), MP_ROM_PTR(&img_info_obj)},
    {MP_ROM_QSTR(MP_QSTR_Image), MP_ROM_PTR(&zimage_type)},
};
static MP_DEFINE_CONST_DICT(img_globals, img_globals_table);
const mp_obj_module_t mp_module_img = {
    .base = {&mp_type_module},
    .globals = (mp_obj_dict_t *)&img_globals,
};

MP_REGISTER_MODULE(MP_QSTR_png, mp_module_png);
MP_REGISTER_MODULE(MP_QSTR_jpeg, mp_module_jpeg);
MP_REGISTER_MODULE(MP_QSTR_jpg, mp_module_jpeg);
MP_REGISTER_MODULE(MP_QSTR_bmp, mp_module_bmp);
MP_REGISTER_MODULE(MP_QSTR_img, mp_module_img);
