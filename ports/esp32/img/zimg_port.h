/*
 * zimg_port.h -- the only place where platform specifics live.
 *
 *  - allocator: on MicroPython everything comes from the MicroPython heap
 *    (which on a PSRAM board is the big one). Allocation failure returns NULL;
 *    the module turns that into MemoryError.
 *  - inflate: on the ESP32-S3 the ROM miniz "tinfl" is used (no code size,
 *    same one the Nopnop pngle decoder uses). The host unit tests supply a
 *    zlib-based implementation of the same 3 functions instead.
 */
#ifndef ZIMG_PORT_H
#define ZIMG_PORT_H

#include <stddef.h>
#include <stdint.h>

#ifdef ZIMG_HOST
  #include <stdlib.h>
  #define zi_alloc(n)       calloc(1, (n))
  #define zi_free(p, n)     free(p)
#else
  #include <string.h>
  #include "py/misc.h"
  static inline void *zi_alloc(size_t n) {
      void *p = m_malloc_maybe(n);
      if (p) {
          memset(p, 0, n);
      }
      return p;
  }
  #if defined(MICROPY_MALLOC_USES_ALLOCATED_SIZE) && MICROPY_MALLOC_USES_ALLOCATED_SIZE
    #define zi_free(p, n)   m_free((p), (n))
  #else
    #define zi_free(p, n)   m_free(p)
  #endif
#endif

/* ---- inflate backend ------------------------------------------------------
 * Streaming zlib inflate into a 32 KiB ring ("dictionary") buffer.
 *   zi_inflate_new()   allocate+init a decompressor (NULL on OOM)
 *   zi_inflate_free()
 *   zi_inflate_run()   consume *in_n bytes from `in`, write at `out_pos`
 *                      (which is inside the 32 KiB ring starting at `ring`),
 *                      at most *out_n bytes. Updates both counts.
 * Returns: 0 = stream finished, 1 = needs more input, 2 = has more output
 *          (call again, ring position already advanced by the caller),
 *          <0 = corrupt data.
 */
#define ZI_RING_SIZE 32768
typedef struct zi_inflate zi_inflate_t;
zi_inflate_t *zi_inflate_new(void);
void zi_inflate_free(zi_inflate_t *z);
int zi_inflate_run(zi_inflate_t *z, uint8_t *ring, const uint8_t *in, size_t *in_n,
                   uint8_t *out_pos, size_t *out_n, int has_more_input);

#endif
