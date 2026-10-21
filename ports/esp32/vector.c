/*
 * vector - fast buffer math for MicroPython on ESP32-S3 (PIE / AI vector unit)
 *
 * Fast paths use ESP-DSP kernels (PIE-accelerated on the S3) when the
 * esp-dsp component is available at build time. Otherwise a portable scalar
 * C fallback is compiled, so the module always builds and always works.
 *
 * Supported typecodes (array.array / memoryview / any buffer with typecode):
 *   'b' int8   'h' int16   'i' or 'l' int32   'f' float32
 *
 * API (all buffer-in / buffer-out, no allocation):
 *   vector.add(a, b, out, shift=0)
 *   vector.sub(a, b, out, shift=0)
 *   vector.mul(a, b, out, shift=0)
 *   vector.dot(a, b, shift=0)  -> int or float
 *   vector.matmul(a, b, out, m, n, k, shift=0)   a:m x n, b:n x k, out:m x k (row-major)
 *   vector.transpose(a, out, m, n)               a:m x n -> out:n x m
 *   vector.scale(a, c, out, shift=0)             out = (a * c) >> shift
 *   vector.offset(a, c, out)                     out = a + c
 *   vector.sum(a, shift=0)                       -> int or float
 *   vector.info()              -> backend description string
 * Matrix add/sub: just use add()/sub() on the flat row-major buffers.
 *
 * Integer ops saturate to the element type; `shift` is an arithmetic right
 * shift applied to the wide result before saturation (use it for fixed-point,
 * e.g. Q15 multiply: shift=15). `shift` is ignored for floats.
 * `out` may be the same object as `a` or `b` (in-place).
 */

#include <stdint.h>
#include <string.h>

#include "py/runtime.h"
#include "py/obj.h"
#include "py/objarray.h"

#if defined(__has_include)
#if __has_include("dsps_add.h") && __has_include("dsps_mul.h") && __has_include("dsps_dotprod.h")
#define VECTOR_HAVE_DSP 1
#include "dsps_add.h"
#include "dsps_mul.h"
#include "dsps_dotprod.h"
#endif
#endif

#ifndef VECTOR_HAVE_DSP
#define VECTOR_HAVE_DSP 0
#endif

#if defined(__has_include)
#if __has_include("dspm_mult.h")
#define VECTOR_HAVE_DSPM 1
#include "dspm_mult.h"
#endif
#endif

#ifndef VECTOR_HAVE_DSPM
#define VECTOR_HAVE_DSPM 0
#endif

#define CLAMP(r, lo, hi) ((r) < (lo) ? (lo) : ((r) > (hi) ? (hi) : (r)))

typedef void (*bin_fn_t)(const void *a, const void *b, void *o, size_t n, int sh);

enum { OP_ADD = 0, OP_SUB = 1, OP_MUL = 2 };
typedef enum { K_S8, K_S16, K_S32, K_F32 } kind_t;

/* ------------------------------------------------------------------ */
/* Scalar integer kernels (saturating)                                  */
/* ------------------------------------------------------------------ */

#define DEF_INT_OPS(SFX, T, WT, LO, HI)                                              \
    static void sc_add_##SFX(const void *pa, const void *pb, void *po, size_t n, int sh) { \
        const T *a = pa, *b = pb; T *o = po;                                         \
        for (size_t i = 0; i < n; i++) {                                             \
            WT r = (WT)a[i] + (WT)b[i];                                              \
            if (sh > 0) r >>= sh;                                                    \
            o[i] = (T)CLAMP(r, (WT)(LO), (WT)(HI));                                  \
        }                                                                            \
    }                                                                                \
    static void sc_sub_##SFX(const void *pa, const void *pb, void *po, size_t n, int sh) { \
        const T *a = pa, *b = pb; T *o = po;                                         \
        for (size_t i = 0; i < n; i++) {                                             \
            WT r = (WT)a[i] - (WT)b[i];                                              \
            if (sh > 0) r >>= sh;                                                    \
            o[i] = (T)CLAMP(r, (WT)(LO), (WT)(HI));                                  \
        }                                                                            \
    }                                                                                \
    static void sc_mul_##SFX(const void *pa, const void *pb, void *po, size_t n, int sh) { \
        const T *a = pa, *b = pb; T *o = po;                                         \
        for (size_t i = 0; i < n; i++) {                                             \
            WT r = (WT)a[i] * (WT)b[i];                                              \
            if (sh > 0) r >>= sh;                                                    \
            o[i] = (T)CLAMP(r, (WT)(LO), (WT)(HI));                                  \
        }                                                                            \
    }

DEF_INT_OPS(s8,  int8_t,  int32_t, INT8_MIN,  INT8_MAX)
DEF_INT_OPS(s16, int16_t, int32_t, INT16_MIN, INT16_MAX)
DEF_INT_OPS(s32, int32_t, int64_t, INT32_MIN, INT32_MAX)

/* ------------------------------------------------------------------ */
/* Scalar float kernels                                                 */
/* ------------------------------------------------------------------ */

static void sc_add_f32(const void *pa, const void *pb, void *po, size_t n, int sh) {
    (void)sh; const float *a = pa, *b = pb; float *o = po;
    for (size_t i = 0; i < n; i++) o[i] = a[i] + b[i];
}
static void sc_sub_f32(const void *pa, const void *pb, void *po, size_t n, int sh) {
    (void)sh; const float *a = pa, *b = pb; float *o = po;
    for (size_t i = 0; i < n; i++) o[i] = a[i] - b[i];
}
static void sc_mul_f32(const void *pa, const void *pb, void *po, size_t n, int sh) {
    (void)sh; const float *a = pa, *b = pb; float *o = po;
    for (size_t i = 0; i < n; i++) o[i] = a[i] * b[i];
}

/* ------------------------------------------------------------------ */
/* ESP-DSP (PIE-accelerated) fast paths                                 */
/* ------------------------------------------------------------------ */

#if VECTOR_HAVE_DSP
static void fast_add_s16(const void *a, const void *b, void *o, size_t n, int sh) {
    dsps_add_s16(a, b, o, (int)n, 1, 1, 1, sh > 0 ? sh : 0);
}
static void fast_mul_s16(const void *a, const void *b, void *o, size_t n, int sh) {
    dsps_mul_s16(a, b, o, (int)n, 1, 1, 1, sh > 0 ? sh : 0);
}
static void fast_add_f32(const void *a, const void *b, void *o, size_t n, int sh) {
    (void)sh; dsps_add_f32(a, b, o, (int)n, 1, 1, 1);
}
static void fast_mul_f32(const void *a, const void *b, void *o, size_t n, int sh) {
    (void)sh; dsps_mul_f32(a, b, o, (int)n, 1, 1, 1);
}
#define ADD_S16 fast_add_s16
#define MUL_S16 fast_mul_s16
#define ADD_F32 fast_add_f32
#define MUL_F32 fast_mul_f32
#else
#define ADD_S16 sc_add_s16
#define MUL_S16 sc_mul_s16
#define ADD_F32 sc_add_f32
#define MUL_F32 sc_mul_f32
#endif

/* dispatch tables: [kind][op] */
static const bin_fn_t op_table[4][3] = {
    [K_S8]  = { sc_add_s8,  sc_sub_s8,  sc_mul_s8  },
    [K_S16] = { ADD_S16,    sc_sub_s16, MUL_S16    },
    [K_S32] = { sc_add_s32, sc_sub_s32, sc_mul_s32 },
    [K_F32] = { ADD_F32,    sc_sub_f32, MUL_F32    },
};

/* ------------------------------------------------------------------ */
/* Helpers                                                              */
/* ------------------------------------------------------------------ */

static kind_t kind_of(int tc, size_t *esz) {
    switch (tc) {
        case 'b': *esz = 1; return K_S8;
        case 'h': *esz = 2; return K_S16;
        case 'i':
        case 'l': *esz = 4; return K_S32;
        case 'f': *esz = 4; return K_F32;
        default:
            mp_raise_TypeError(MP_ERROR_TEXT("vector: typecode must be b, h, i/l or f"));
    }
}

static kind_t check_pair(mp_buffer_info_t *a, mp_buffer_info_t *b, size_t *n) {
    size_t esz_a, esz_b;
    kind_t ka = kind_of(a->typecode, &esz_a);
    kind_t kb = kind_of(b->typecode, &esz_b);
    if (ka != kb) {
        mp_raise_TypeError(MP_ERROR_TEXT("vector: typecodes differ"));
    }
    size_t na = a->len / esz_a, nb = b->len / esz_b;
    if (na != nb) {
        mp_raise_ValueError(MP_ERROR_TEXT("vector: lengths differ"));
    }
    *n = na;
    return ka;
}

enum { ARG_a, ARG_b, ARG_out, ARG_shift };

static mp_obj_t vector_binop(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args, int op) {
    static const mp_arg_t allowed[] = {
        { MP_QSTR_a,     MP_ARG_REQUIRED | MP_ARG_OBJ, {.u_obj = mp_const_none} },
        { MP_QSTR_b,     MP_ARG_REQUIRED | MP_ARG_OBJ, {.u_obj = mp_const_none} },
        { MP_QSTR_out,   MP_ARG_REQUIRED | MP_ARG_OBJ, {.u_obj = mp_const_none} },
        { MP_QSTR_shift, MP_ARG_KW_ONLY | MP_ARG_INT,  {.u_int = 0} },
    };
    mp_arg_val_t v[MP_ARRAY_SIZE(allowed)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed), allowed, v);

    mp_buffer_info_t a, b, o;
    mp_get_buffer_raise(v[ARG_a].u_obj, &a, MP_BUFFER_READ);
    mp_get_buffer_raise(v[ARG_b].u_obj, &b, MP_BUFFER_READ);
    mp_get_buffer_raise(v[ARG_out].u_obj, &o, MP_BUFFER_WRITE);

    size_t n;
    kind_t k = check_pair(&a, &b, &n);
    if (o.typecode != a.typecode && !((o.typecode == 'i' || o.typecode == 'l') && k == K_S32)) {
        mp_raise_TypeError(MP_ERROR_TEXT("vector: out typecode differs"));
    }
    size_t esz;
    kind_of(o.typecode, &esz);
    if (o.len / esz != n) {
        mp_raise_ValueError(MP_ERROR_TEXT("vector: out length differs"));
    }
    if (v[ARG_shift].u_int < 0 || v[ARG_shift].u_int > 62) {
        mp_raise_ValueError(MP_ERROR_TEXT("vector: shift out of range"));
    }
    if (n) {
        op_table[k][op](a.buf, b.buf, o.buf, n, v[ARG_shift].u_int);
    }
    return mp_const_none;
}

static mp_obj_t vector_add(size_t n, const mp_obj_t *p, mp_map_t *kw) { return vector_binop(n, p, kw, OP_ADD); }
static mp_obj_t vector_sub(size_t n, const mp_obj_t *p, mp_map_t *kw) { return vector_binop(n, p, kw, OP_SUB); }
static mp_obj_t vector_mul(size_t n, const mp_obj_t *p, mp_map_t *kw) { return vector_binop(n, p, kw, OP_MUL); }
static MP_DEFINE_CONST_FUN_OBJ_KW(vector_add_obj, 3, vector_add);
static MP_DEFINE_CONST_FUN_OBJ_KW(vector_sub_obj, 3, vector_sub);
static MP_DEFINE_CONST_FUN_OBJ_KW(vector_mul_obj, 3, vector_mul);

/* ------------------------------------------------------------------ */
/* dot product                                                          */
/* ------------------------------------------------------------------ */

static mp_obj_t vector_dot(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    static const mp_arg_t allowed[] = {
        { MP_QSTR_a,     MP_ARG_REQUIRED | MP_ARG_OBJ, {.u_obj = mp_const_none} },
        { MP_QSTR_b,     MP_ARG_REQUIRED | MP_ARG_OBJ, {.u_obj = mp_const_none} },
        { MP_QSTR_shift, MP_ARG_KW_ONLY | MP_ARG_INT,  {.u_int = 0} },
    };
    mp_arg_val_t v[MP_ARRAY_SIZE(allowed)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed), allowed, v);

    mp_buffer_info_t a, b;
    mp_get_buffer_raise(v[0].u_obj, &a, MP_BUFFER_READ);
    mp_get_buffer_raise(v[1].u_obj, &b, MP_BUFFER_READ);
    int sh = v[2].u_int;
    if (sh < 0 || sh > 62) {
        mp_raise_ValueError(MP_ERROR_TEXT("vector: shift out of range"));
    }

    size_t n;
    kind_t k = check_pair(&a, &b, &n);

    switch (k) {
        case K_F32: {
            float acc = 0.0f;
            #if VECTOR_HAVE_DSP
            if (n) dsps_dotprod_f32(a.buf, b.buf, &acc, (int)n);
            #else
            const float *x = a.buf, *y = b.buf;
            for (size_t i = 0; i < n; i++) acc += x[i] * y[i];
            #endif
            return mp_obj_new_float(acc);
        }
        case K_S16: {
            /* Result is int16 (ESP-DSP semantics): pick `shift` so it fits. */
            #if VECTOR_HAVE_DSP
            int16_t r = 0;
            if (n) dsps_dotprod_s16(a.buf, b.buf, &r, (int)n, (int8_t)sh);
            return mp_obj_new_int(r);
            #else
            const int16_t *x = a.buf, *y = b.buf;
            int64_t acc = 0;
            for (size_t i = 0; i < n; i++) acc += (int32_t)x[i] * (int32_t)y[i];
            acc >>= sh;
            return mp_obj_new_int(CLAMP(acc, (int64_t)INT16_MIN, (int64_t)INT16_MAX));
            #endif
        }
        case K_S8: {
            const int8_t *x = a.buf, *y = b.buf;
            int64_t acc = 0;
            for (size_t i = 0; i < n; i++) acc += (int32_t)x[i] * (int32_t)y[i];
            return mp_obj_new_int_from_ll(acc >> sh);
        }
        case K_S32:
        default: {
            const int32_t *x = a.buf, *y = b.buf;
            int64_t acc = 0;
            for (size_t i = 0; i < n; i++) acc += (int64_t)x[i] * (int64_t)y[i];
            return mp_obj_new_int_from_ll(acc >> sh);
        }
    }
}
static MP_DEFINE_CONST_FUN_OBJ_KW(vector_dot_obj, 2, vector_dot);


/* ------------------------------------------------------------------ */
/* Matrix multiply, transpose, scale, offset, sum                       */
/* ------------------------------------------------------------------ */

typedef void (*matmul_fn_t)(const void *a, const void *b, void *o, size_t m, size_t n, size_t k, int sh);

#define DEF_MATMUL_INT(SFX, T, LO, HI)                                               \
    static void sc_matmul_##SFX(const void *pa, const void *pb, void *po,            \
                                size_t m, size_t n, size_t k, int sh) {              \
        const T *a = pa, *b = pb; T *o = po;                                         \
        for (size_t i = 0; i < m; i++) {                                             \
            for (size_t j = 0; j < k; j++) {                                         \
                int64_t acc = 0;                                                     \
                for (size_t t = 0; t < n; t++) {                                     \
                    acc += (int64_t)a[i * n + t] * (int64_t)b[t * k + j];            \
                }                                                                    \
                acc >>= sh;                                                          \
                o[i * k + j] = (T)CLAMP(acc, (int64_t)(LO), (int64_t)(HI));          \
            }                                                                        \
        }                                                                            \
    }

DEF_MATMUL_INT(s8,  int8_t,  INT8_MIN,  INT8_MAX)
DEF_MATMUL_INT(s16, int16_t, INT16_MIN, INT16_MAX)
DEF_MATMUL_INT(s32, int32_t, INT32_MIN, INT32_MAX)

static void sc_matmul_f32(const void *pa, const void *pb, void *po,
                          size_t m, size_t n, size_t k, int sh) {
    (void)sh;
    const float *a = pa, *b = pb; float *o = po;
    for (size_t i = 0; i < m; i++) {
        float *row = o + i * k;
        for (size_t j = 0; j < k; j++) row[j] = 0.0f;
        for (size_t t = 0; t < n; t++) {
            float av = a[i * n + t];
            const float *brow = b + t * k;
            for (size_t j = 0; j < k; j++) row[j] += av * brow[j];
        }
    }
}

#if VECTOR_HAVE_DSPM
static void fast_matmul_s16(const void *a, const void *b, void *o, size_t m, size_t n, size_t k, int sh) {
    dspm_mult_s16(a, b, o, (int)m, (int)n, (int)k, sh);
}
static void fast_matmul_f32(const void *a, const void *b, void *o, size_t m, size_t n, size_t k, int sh) {
    (void)sh;
    dspm_mult_f32(a, b, o, (int)m, (int)n, (int)k);
}
#define MM_S16 fast_matmul_s16
#define MM_F32 fast_matmul_f32
#else
#define MM_S16 sc_matmul_s16
#define MM_F32 sc_matmul_f32
#endif

static const matmul_fn_t matmul_table[4] = {
    [K_S8] = sc_matmul_s8, [K_S16] = MM_S16, [K_S32] = sc_matmul_s32, [K_F32] = MM_F32,
};

static bool bufs_overlap(const mp_buffer_info_t *x, const mp_buffer_info_t *y) {
    uintptr_t xs = (uintptr_t)x->buf, ys = (uintptr_t)y->buf;
    return xs < ys + y->len && ys < xs + x->len;
}

static mp_obj_t vector_matmul(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    static const mp_arg_t allowed[] = {
        { MP_QSTR_a,     MP_ARG_REQUIRED | MP_ARG_OBJ, {.u_obj = mp_const_none} },
        { MP_QSTR_b,     MP_ARG_REQUIRED | MP_ARG_OBJ, {.u_obj = mp_const_none} },
        { MP_QSTR_out,   MP_ARG_REQUIRED | MP_ARG_OBJ, {.u_obj = mp_const_none} },
        { MP_QSTR_m,     MP_ARG_REQUIRED | MP_ARG_INT, {.u_int = 0} },
        { MP_QSTR_n,     MP_ARG_REQUIRED | MP_ARG_INT, {.u_int = 0} },
        { MP_QSTR_k,     MP_ARG_REQUIRED | MP_ARG_INT, {.u_int = 0} },
        { MP_QSTR_shift, MP_ARG_KW_ONLY | MP_ARG_INT,  {.u_int = 0} },
    };
    mp_arg_val_t v[MP_ARRAY_SIZE(allowed)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed), allowed, v);

    mp_buffer_info_t a, b, o;
    mp_get_buffer_raise(v[0].u_obj, &a, MP_BUFFER_READ);
    mp_get_buffer_raise(v[1].u_obj, &b, MP_BUFFER_READ);
    mp_get_buffer_raise(v[2].u_obj, &o, MP_BUFFER_WRITE);
    int mi = v[3].u_int, ni = v[4].u_int, ki = v[5].u_int, sh = v[6].u_int;
    if (mi <= 0 || ni <= 0 || ki <= 0) {
        mp_raise_ValueError(MP_ERROR_TEXT("vector: m, n, k must be > 0"));
    }
    if (sh < 0 || sh > 62) {
        mp_raise_ValueError(MP_ERROR_TEXT("vector: shift out of range"));
    }
    size_t m = mi, n = ni, k = ki;
    size_t ea, eb, eo;
    kind_t kind = kind_of(a.typecode, &ea);
    if (kind != kind_of(b.typecode, &eb) || kind != kind_of(o.typecode, &eo)) {
        mp_raise_TypeError(MP_ERROR_TEXT("vector: typecodes differ"));
    }
    if (a.len / ea != m * n || b.len / eb != n * k || o.len / eo != m * k) {
        mp_raise_ValueError(MP_ERROR_TEXT("vector: buffer sizes do not match m, n, k"));
    }
    if (bufs_overlap(&o, &a) || bufs_overlap(&o, &b)) {
        mp_raise_ValueError(MP_ERROR_TEXT("vector: out must not overlap inputs"));
    }
    matmul_table[kind](a.buf, b.buf, o.buf, m, n, k, sh);
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_KW(vector_matmul_obj, 3, vector_matmul);

/* transpose: a is m x n, out is n x m */
static mp_obj_t vector_transpose(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    static const mp_arg_t allowed[] = {
        { MP_QSTR_a,   MP_ARG_REQUIRED | MP_ARG_OBJ, {.u_obj = mp_const_none} },
        { MP_QSTR_out, MP_ARG_REQUIRED | MP_ARG_OBJ, {.u_obj = mp_const_none} },
        { MP_QSTR_m,   MP_ARG_REQUIRED | MP_ARG_INT, {.u_int = 0} },
        { MP_QSTR_n,   MP_ARG_REQUIRED | MP_ARG_INT, {.u_int = 0} },
    };
    mp_arg_val_t v[MP_ARRAY_SIZE(allowed)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed), allowed, v);

    mp_buffer_info_t a, o;
    mp_get_buffer_raise(v[0].u_obj, &a, MP_BUFFER_READ);
    mp_get_buffer_raise(v[1].u_obj, &o, MP_BUFFER_WRITE);
    if (v[2].u_int <= 0 || v[3].u_int <= 0) {
        mp_raise_ValueError(MP_ERROR_TEXT("vector: m and n must be > 0"));
    }
    size_t m = v[2].u_int, n = v[3].u_int;
    size_t ea, eo;
    kind_t ka = kind_of(a.typecode, &ea), ko = kind_of(o.typecode, &eo);
    if (ka != ko) {
        mp_raise_TypeError(MP_ERROR_TEXT("vector: typecodes differ"));
    }
    if (a.len / ea != m * n || o.len / eo != m * n) {
        mp_raise_ValueError(MP_ERROR_TEXT("vector: buffer sizes do not match m, n"));
    }
    if (bufs_overlap(&o, &a)) {
        mp_raise_ValueError(MP_ERROR_TEXT("vector: out must not overlap input"));
    }
    const uint8_t *src = a.buf;
    uint8_t *dst = o.buf;
    for (size_t i = 0; i < m; i++) {
        for (size_t j = 0; j < n; j++) {
            memcpy(dst + (j * m + i) * ea, src + (i * n + j) * ea, ea);
        }
    }
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_KW(vector_transpose_obj, 2, vector_transpose);

/* scale / offset by a constant */
#define DEF_CONST_OPS(SFX, T, LO, HI)                                                \
    static void sc_scale_##SFX(const void *pa, void *po, size_t n, int64_t c, int sh) { \
        const T *a = pa; T *o = po;                                                  \
        for (size_t i = 0; i < n; i++) {                                             \
            int64_t r = (int64_t)a[i] * c;                                           \
            r >>= sh;                                                                \
            o[i] = (T)CLAMP(r, (int64_t)(LO), (int64_t)(HI));                        \
        }                                                                            \
    }                                                                                \
    static void sc_offset_##SFX(const void *pa, void *po, size_t n, int64_t c) {     \
        const T *a = pa; T *o = po;                                                  \
        for (size_t i = 0; i < n; i++) {                                             \
            int64_t r = (int64_t)a[i] + c;                                           \
            o[i] = (T)CLAMP(r, (int64_t)(LO), (int64_t)(HI));                        \
        }                                                                            \
    }

DEF_CONST_OPS(s8,  int8_t,  INT8_MIN,  INT8_MAX)
DEF_CONST_OPS(s16, int16_t, INT16_MIN, INT16_MAX)
DEF_CONST_OPS(s32, int32_t, INT32_MIN, INT32_MAX)

static mp_obj_t vector_const_op(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args, bool is_scale) {
    static const mp_arg_t allowed[] = {
        { MP_QSTR_a,     MP_ARG_REQUIRED | MP_ARG_OBJ, {.u_obj = mp_const_none} },
        { MP_QSTR_c,     MP_ARG_REQUIRED | MP_ARG_OBJ, {.u_obj = mp_const_none} },
        { MP_QSTR_out,   MP_ARG_REQUIRED | MP_ARG_OBJ, {.u_obj = mp_const_none} },
        { MP_QSTR_shift, MP_ARG_KW_ONLY | MP_ARG_INT,  {.u_int = 0} },
    };
    mp_arg_val_t v[MP_ARRAY_SIZE(allowed)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed), allowed, v);

    mp_buffer_info_t a, o;
    mp_get_buffer_raise(v[0].u_obj, &a, MP_BUFFER_READ);
    mp_get_buffer_raise(v[2].u_obj, &o, MP_BUFFER_WRITE);
    int sh = v[3].u_int;
    if (sh < 0 || sh > 62) {
        mp_raise_ValueError(MP_ERROR_TEXT("vector: shift out of range"));
    }
    size_t ea, eo;
    kind_t k = kind_of(a.typecode, &ea);
    if (k != kind_of(o.typecode, &eo)) {
        mp_raise_TypeError(MP_ERROR_TEXT("vector: typecodes differ"));
    }
    size_t n = a.len / ea;
    if (o.len / eo != n) {
        mp_raise_ValueError(MP_ERROR_TEXT("vector: out length differs"));
    }
    if (k == K_F32) {
        float c = (float)mp_obj_get_float(v[1].u_obj);
        const float *x = a.buf; float *y = o.buf;
        for (size_t i = 0; i < n; i++) y[i] = is_scale ? x[i] * c : x[i] + c;
        return mp_const_none;
    }
    int64_t c = mp_obj_get_int(v[1].u_obj);
    switch (k) {
        case K_S8:  is_scale ? sc_scale_s8(a.buf, o.buf, n, c, sh)  : sc_offset_s8(a.buf, o.buf, n, c);  break;
        case K_S16: is_scale ? sc_scale_s16(a.buf, o.buf, n, c, sh) : sc_offset_s16(a.buf, o.buf, n, c); break;
        default:    is_scale ? sc_scale_s32(a.buf, o.buf, n, c, sh) : sc_offset_s32(a.buf, o.buf, n, c); break;
    }
    return mp_const_none;
}
static mp_obj_t vector_scale(size_t n, const mp_obj_t *p, mp_map_t *kw)  { return vector_const_op(n, p, kw, true); }
static mp_obj_t vector_offset(size_t n, const mp_obj_t *p, mp_map_t *kw) { return vector_const_op(n, p, kw, false); }
static MP_DEFINE_CONST_FUN_OBJ_KW(vector_scale_obj, 3, vector_scale);
static MP_DEFINE_CONST_FUN_OBJ_KW(vector_offset_obj, 3, vector_offset);

/* sum of all elements */
static mp_obj_t vector_sum(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    static const mp_arg_t allowed[] = {
        { MP_QSTR_a,     MP_ARG_REQUIRED | MP_ARG_OBJ, {.u_obj = mp_const_none} },
        { MP_QSTR_shift, MP_ARG_KW_ONLY | MP_ARG_INT,  {.u_int = 0} },
    };
    mp_arg_val_t v[MP_ARRAY_SIZE(allowed)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed), allowed, v);

    mp_buffer_info_t a;
    mp_get_buffer_raise(v[0].u_obj, &a, MP_BUFFER_READ);
    int sh = v[1].u_int;
    if (sh < 0 || sh > 62) {
        mp_raise_ValueError(MP_ERROR_TEXT("vector: shift out of range"));
    }
    size_t ea;
    kind_t k = kind_of(a.typecode, &ea);
    size_t n = a.len / ea;
    int64_t acc = 0;
    switch (k) {
        case K_F32: {
            const float *x = a.buf; float f = 0.0f;
            for (size_t i = 0; i < n; i++) f += x[i];
            return mp_obj_new_float(f);
        }
        case K_S8:  { const int8_t *x = a.buf;  for (size_t i = 0; i < n; i++) acc += x[i]; break; }
        case K_S16: { const int16_t *x = a.buf; for (size_t i = 0; i < n; i++) acc += x[i]; break; }
        default:    { const int32_t *x = a.buf; for (size_t i = 0; i < n; i++) acc += x[i]; break; }
    }
    return mp_obj_new_int_from_ll(acc >> sh);
}
static MP_DEFINE_CONST_FUN_OBJ_KW(vector_sum_obj, 1, vector_sum);

/* ------------------------------------------------------------------ */
/* info                                                                 */
/* ------------------------------------------------------------------ */

static mp_obj_t vector_info(void) {
    #if VECTOR_HAVE_DSP
    const char *m = "esp-dsp (PIE accelerated: add/mul/matmul s16 f32, dot s16 f32); scalar: others";
    return mp_obj_new_str(m, strlen(m));
    #else
    const char *m = "scalar C fallback (esp-dsp not found at build time)";
    return mp_obj_new_str(m, strlen(m));
    #endif
}
static MP_DEFINE_CONST_FUN_OBJ_0(vector_info_obj, vector_info);

/* ------------------------------------------------------------------ */
/* module                                                               */
/* ------------------------------------------------------------------ */

static const mp_rom_map_elem_t vector_module_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_vector) },
    { MP_ROM_QSTR(MP_QSTR_add),  MP_ROM_PTR(&vector_add_obj) },
    { MP_ROM_QSTR(MP_QSTR_sub),  MP_ROM_PTR(&vector_sub_obj) },
    { MP_ROM_QSTR(MP_QSTR_mul),  MP_ROM_PTR(&vector_mul_obj) },
    { MP_ROM_QSTR(MP_QSTR_dot),  MP_ROM_PTR(&vector_dot_obj) },
    { MP_ROM_QSTR(MP_QSTR_matmul),    MP_ROM_PTR(&vector_matmul_obj) },
    { MP_ROM_QSTR(MP_QSTR_transpose), MP_ROM_PTR(&vector_transpose_obj) },
    { MP_ROM_QSTR(MP_QSTR_scale),     MP_ROM_PTR(&vector_scale_obj) },
    { MP_ROM_QSTR(MP_QSTR_offset),    MP_ROM_PTR(&vector_offset_obj) },
    { MP_ROM_QSTR(MP_QSTR_sum),       MP_ROM_PTR(&vector_sum_obj) },
    { MP_ROM_QSTR(MP_QSTR_info), MP_ROM_PTR(&vector_info_obj) },
};
static MP_DEFINE_CONST_DICT(vector_module_globals, vector_module_globals_table);

const mp_obj_module_t vector_user_cmodule = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t *)&vector_module_globals,
};

MP_REGISTER_MODULE(MP_QSTR_vector, vector_user_cmodule);
