/*
 * zeno_mp_err.h -- raise a MicroPython OSError carrying the real ESP-IDF
 * error name, instead of mp_raise_OSError(-e). esp_err_t values are NOT
 * POSIX errno codes (e.g. ESP_ERR_NO_MEM == 0x101 == 257), so passing one
 * straight into mp_raise_OSError() produces a meaningless "[Errno -257]"
 * instead of a message anyone can act on.
 */
#ifndef ZENO_MP_ERR_H
#define ZENO_MP_ERR_H

#include "py/runtime.h"
#include "esp_err.h"

/* A macro, not a static inline function: MicroPython's QSTR/text-compression
 * scanner (py/makeqstrdefs.py) attributes MP_ERROR_TEXT() strings to the .c
 * file being preprocessed. Left inside a header it can be the first thing in
 * the preprocessed stream with no source file yet, which crashes the scanner. */
#define zl_raise_esp_err(e) \
    mp_raise_msg_varg(&mp_type_OSError, MP_ERROR_TEXT("%s"), esp_err_to_name(e))

#endif
