/*
 * moclcd.c — higher-level 8080 8-bit parallel LCD module for MicroPython,
 * built on top of ESP-IDF's esp_lcd i80 driver.
 *
 * Same known-good pin mapping and init sequence as lcd_min.c:
 * - RST: GPIO 12
 * - RS (DC): GPIO 13
 * - WR: GPIO 14
 * - RD: GPIO 41
 * - BL (Backlight): GPIO 38
 * - D0-D7: GPIOs 16, 15, 11, 10, 9, 4, 18, 17
 *
 * What's new vs lcd_min.c:
 * - panel_init() runs the exact working command sequence once (no more
 *   doing it by hand in Python).
 * - fill_rect() / fill_screen() / blit() replace the manual per-line
 *   data() calls from Python.
 * - Fills stream through a small DMA-capable buffer (heap_caps_malloc
 *   with MALLOC_CAP_DMA) that's resent in chunks. Because
 *   trans_queue_depth is 10, several chunks can be in flight on the DMA
 *   engine at once instead of the CPU/Python loop stalling on each line
 *   like the original demo script did.
 *
 * API:
 *   moclcd.init(pclk=10_000_000, width=480, height=320, madctl=0x28)
 *                                             -- defaults to landscape;
 *                                                pass width=320, height=480,
 *                                                madctl=0x48 for portrait
 *   moclcd.reset()
 *   moclcd.panel_init()
 *   moclcd.backlight(on)                     -- digital on/off; drives PWM duty
 *                                                to max/0 instead if backlight_init()
 *                                                was called
 *   moclcd.backlight_init(freq_hz=5000, resolution_bits=8)
 *                                             -- sets up LEDC PWM on the BL pin
 *   moclcd.backlight_set(level)              -- level is 0.0-1.0 brightness fraction,
 *                                                requires backlight_init() first
 *   moclcd.cmd(cmd, params=None)     -- raw passthrough, still available
 *   moclcd.data(buf)                 -- raw passthrough, still available
 *   moclcd.fill_rect(x, y, w, h, color)      -- raises ValueError if out of bounds
 *   moclcd.fill_screen(color)
 *   moclcd.blit(x, y, w, h, buf)             -- buf is raw RGB565 bytes, MSB first
 *   moclcd.draw_pixel(x, y, color)           -- clipped silently if off-panel
 *   moclcd.draw_line(x0, y0, x1, y1, color)  -- clipped silently if off-panel
 *   moclcd.draw_rect(x, y, w, h, color)      -- outline; clipped silently if off-panel
 *   moclcd.draw_circle(x0, y0, r, color)     -- outline; clipped silently if off-panel
 *   moclcd.fill_circle(x0, y0, r, color)     -- filled; clipped silently if off-panel
 *
 *   moclcd.draw_text(x, y, text, fg, bg=None, font=0)
 *                                             -- like draw_text8x8 but with a `font` id;
 *                                                font=0 is the built-in 8x8 font
 *   moclcd.register_font(font_id, glyph_data, char_w, char_h, first_char, last_char)
 *                                             -- register an additional font (1..7);
 *                                                glyph_data is a column-major bitmap
 *                                                table, same byte layout as
 *                                                font_petme128_8x8
 *   moclcd.unregister_font(font_id)          -- free a previously registered font
 *   moclcd.font_metrics(font_id)             -- (char_w, char_h, first_char, last_char)
 *
 *   moclcd.mirror_enable()                   -- start mirroring every draw into an
 *                                                internal shadow framebuffer (~300KB
 *                                                RAM at 480x320); required before
 *                                                read_framebuffer() will work, since
 *                                                the panel itself has no readback bus
 *   moclcd.mirror_disable()                  -- stop mirroring (keeps the buffer)
 *   moclcd.mirror_free()                     -- stop mirroring and free the buffer
 *   moclcd.read_framebuffer(dest, x=0, y=0, w=None, h=None)
 *                                             -- copies the current (mirrored) frame,
 *                                                or a sub-rect of it, into your own
 *                                                pre-allocated `dest` buffer (e.g.
 *                                                bytearray); RGB565 MSB-first, same
 *                                                layout blit() expects; returns bytes
 *                                                written. Raises OSError if
 *                                                mirror_enable() wasn't called first.
 *
 *   moclcd.blit_fast(xs, ys, ws, hs, colors, count)
 *                                             -- batched fill_rect stream driven from
 *                                                one C call instead of `count` separate
 *                                                Python calls; xs/ys/ws/hs/colors are
 *                                                int32 arrays (e.g. array.array('i', ...))
 *                                                of length >= count. Used internally by
 *                                                the faster screen-open animation path.
 *
 * NOTE on draw_bmp() and the filesystem:
 *   draw_bmp() uses the C library's fopen(), which is routed through
 *   MicroPython's VFS -- exactly like Python's own open(). If it's
 *   called before anything has been mounted (os.mount(), or before
 *   the board's default flash mount has run -- e.g. from code that
 *   runs very early in boot.py, or a custom C init path that calls
 *   panel_init()/draw_bmp() before main.py), every path will fail to
 *   open even if the file will exist a moment later. draw_bmp() now
 *   detects this case and raises a clear OSError explaining it instead
 *   of a bare ENOENT that looks like a typo'd filename. Fix: call
 *   draw_bmp() (and anything that draws logos/backgrounds) only after
 *   the filesystem is mounted -- normally that just means "not from
 *   boot.py before the default mount, and not before main.py has had
 *   a chance to run".
 */

#include "py/obj.h"
#include "py/runtime.h"
#include "py/mphal.h"
#include "mphalport.h"

#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_heap_caps.h"
#include "driver/ledc.h"
#include "extmod/font_petme128_8x8.h"   /* same 8x8 font MicroPython's framebuf.text() uses */
#include "py/mperrno.h"
#include "extmod/vfs.h"

#include <stdio.h>

#include <string.h>

#define LCD_CMD_CASET  0x2A
#define LCD_CMD_PASET  0x2B
#define LCD_CMD_RAMWR  0x2C
#define LCD_CMD_RAMWRC 0x3C   /* continuation write, used for pixel streaming */

/* how many pixels we buffer per DMA chunk (2 bytes/pixel -> 4KB chunks) */
#define FILL_CHUNK_PIXELS 32768

/* ---- module state ---- */
static esp_lcd_i80_bus_handle_t  s_bus       = NULL;
static esp_lcd_panel_io_handle_t s_io        = NULL;
static mp_hal_pin_obj_t          s_reset_pin = 12;
static mp_hal_pin_obj_t          s_bl_pin    = 38;
static mp_hal_pin_obj_t          s_rd_pin    = 41;
static bool                      s_has_reset = false;
static uint16_t                  s_width     = 480;
static uint16_t                  s_height    = 320;
static uint8_t                   s_madctl    = 0x28; /* landscape (MV set); 0x48=portrait, 0x88/0xE8=other rotations */
static uint8_t                  *s_fill_buf  = NULL; /* FILL_CHUNK_PIXELS*2 bytes, DMA capable */
static bool                      s_bl_pwm_inited = false;
static uint32_t                  s_bl_duty_max   = 255; /* set by backlight_init() from resolution_bits */
static uint8_t                  *s_glyph_buf = NULL;    /* 8*8*2 bytes, DMA capable, reused per glyph */

/* ---- shadow framebuffer (for read_framebuffer()) ----
 * The panel itself is write-only over the 8080 bus (no readback path),
 * so "reading the current frame" means mirroring every pixel we send
 * into a RAM copy as we send it, then handing that copy out on
 * request. This is opt-in (mirror_enable()) since it costs
 * width*height*2 bytes (~300KB at 480x320) and a bit of CPU per draw
 * call to keep in sync -- most apps that never call read_framebuffer()
 * shouldn't pay for it. */
static uint16_t                 *s_shadow_fb      = NULL; /* w*h uint16 RGB565, native endianness */
static bool                      s_shadow_enabled  = false;

#define FONT_CHAR_W     8
#define FONT_CHAR_H     8
#define FONT_FIRST_CHAR 32
#define FONT_LAST_CHAR  127

/* ---- font registry (multi-font support) ----
 * font_petme128_8x8 remains font id 0 / the default. Additional fonts
 * can be registered from Python via register_font(id, glyph_bytes,
 * char_w, char_h, first_char, last_char) -- glyph_bytes must be a
 * column-major bitmap table exactly like font_petme128_8x8 (char_w
 * bytes per glyph, bit j of byte i is row j of column i), so any font
 * exported in that layout (e.g. converted offline from a .bdf/.ttf)
 * can be dropped in without rebuilding firmware. */
#define MAX_FONTS 8
typedef struct {
    const uint8_t *data;     /* owned copy, column-major glyph table */
    uint8_t        char_w;
    uint8_t        char_h;
    uint8_t        first_char;
    uint8_t        last_char;
    bool           in_use;
} font_entry_t;

static font_entry_t s_fonts[MAX_FONTS];
static uint8_t *s_font_glyph_buf = NULL;   /* DMA scratch, grown on demand */
static size_t   s_font_glyph_buf_cap = 0;

/* -------------------------------------------------------------------
 * helpers
 * ---------------------------------------------------------------- */
static void io_check(esp_err_t ret, const char *what)
{
    if (ret != ESP_OK) {
        mp_raise_msg_varg(&mp_type_OSError, MP_ERROR_TEXT("%s failed: %d"), what, ret);
    }
}

static void require_init(void)
{
    if (s_io == NULL) {
        mp_raise_msg(&mp_type_OSError, MP_ERROR_TEXT("moclcd.init() must be called first"));
    }
}

/* Best-effort check for "has anything been mounted into MicroPython's
 * VFS yet". MP_STATE_VM(vfs_cur) is NULL/unset until the first mount
 * (os.mount(), or the board's default flash mount during boot.py)
 * happens; ports that mount internal flash automatically before
 * main.py runs will already show ready=true by the time user code
 * gets a chance to call draw_bmp(), so this only actually fires for
 * code that runs *before* that point (e.g. inside boot.py itself, or
 * a custom C init path that calls panel_init()/draw_bmp() too early). */
static bool mp_vfs_mount_is_ready(void)
{
#if MICROPY_VFS
    return MP_STATE_VM(vfs_cur) != NULL || MP_STATE_VM(vfs_mount_table) != NULL;
#else
    return true; /* no VFS layer compiled in: nothing to check, let fopen()'s own error stand */
#endif
}

static void lcd_cmd_raw(uint8_t cmd, const void *buf, size_t len)
{
    io_check(esp_lcd_panel_io_tx_param(s_io, cmd, buf, len), "cmd");
}

static void ensure_fill_buf(void)
{
    if (s_fill_buf == NULL) {
        s_fill_buf = heap_caps_malloc(FILL_CHUNK_PIXELS * 2, MALLOC_CAP_DMA);
        if (s_fill_buf == NULL) {
            mp_raise_msg(&mp_type_MemoryError, MP_ERROR_TEXT("no DMA memory for fill buffer"));
        }
    }
}

/* CASET / PASET / RAMWR — sets the address window and arms the panel
 * for a pixel stream, exactly like begin_write() did in the Python demo. */
static void set_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    uint8_t caset[4] = { (uint8_t)(x0 >> 8), (uint8_t)(x0 & 0xFF),
                         (uint8_t)(x1 >> 8), (uint8_t)(x1 & 0xFF) };
    uint8_t paset[4] = { (uint8_t)(y0 >> 8), (uint8_t)(y0 & 0xFF),
                         (uint8_t)(y1 >> 8), (uint8_t)(y1 & 0xFF) };
    lcd_cmd_raw(LCD_CMD_CASET, caset, sizeof(caset));
    lcd_cmd_raw(LCD_CMD_PASET, paset, sizeof(paset));
    lcd_cmd_raw(LCD_CMD_RAMWR, NULL, 0);
}

/* stream `total_pixels` copies of `color` right after the address
 * window has been armed via set_window(). Shared by fill_rect() and by
 * the line/rect/circle primitives below so they all get the same
 * chunked, DMA-pipelined path. */
static void stream_solid(uint32_t total_pixels, uint16_t color)
{
    ensure_fill_buf();

    uint32_t chunk = total_pixels < FILL_CHUNK_PIXELS ? total_pixels : FILL_CHUNK_PIXELS;
    uint8_t hi = (uint8_t)(color >> 8);
    uint8_t lo = (uint8_t)(color & 0xFF);
    for (uint32_t i = 0; i < chunk; i++) {
        s_fill_buf[2 * i]     = hi;
        s_fill_buf[2 * i + 1] = lo;
    }

    uint32_t remaining = total_pixels;
    while (remaining > 0) {
        uint32_t n = remaining < FILL_CHUNK_PIXELS ? remaining : FILL_CHUNK_PIXELS;
        io_check(esp_lcd_panel_io_tx_color(s_io, LCD_CMD_RAMWRC, s_fill_buf, n * 2), "fill");
        remaining -= n;
    }
}

/* ---- shadow framebuffer mirroring helpers ----
 * Called right alongside the real DMA sends so the RAM copy always
 * matches what's on-panel (for whichever ops opt into mirroring; every
 * primitive that goes through do_fill_rect_clip/do_draw_pixel/blit/
 * draw_bmp/draw_glyph is covered). No-ops entirely (and cheaply) when
 * mirroring hasn't been turned on. */
static void ensure_shadow_fb(void)
{
    if (s_shadow_fb == NULL) {
        s_shadow_fb = heap_caps_malloc((size_t)s_width * (size_t)s_height * 2, MALLOC_CAP_DEFAULT);
        if (s_shadow_fb == NULL) {
            mp_raise_msg(&mp_type_MemoryError, MP_ERROR_TEXT("no memory for shadow framebuffer"));
        }
        memset(s_shadow_fb, 0, (size_t)s_width * (size_t)s_height * 2);
    }
}

static inline void shadow_set_px(int x, int y, uint16_t color)
{
    if (!s_shadow_enabled || s_shadow_fb == NULL) return;
    if (x < 0 || y < 0 || x >= s_width || y >= s_height) return;
    s_shadow_fb[(uint32_t)y * s_width + (uint32_t)x] = color;
}

static void shadow_fill_rect(int x, int y, int w, int h, uint16_t color)
{
    if (!s_shadow_enabled || s_shadow_fb == NULL) return;
    for (int row = y; row < y + h; row++) {
        for (int col = x; col < x + w; col++) {
            shadow_set_px(col, row, color);
        }
    }
}

/* buf is RGB565 MSB-first bytes, exactly what blit()/draw_bmp() send
 * to the panel -- convert to native uint16 as we copy into the shadow. */
static void shadow_blit_bytes(int x, int y, int w, int h, const uint8_t *buf)
{
    if (!s_shadow_enabled || s_shadow_fb == NULL) return;
    for (int row = 0; row < h; row++) {
        for (int col = 0; col < w; col++) {
            size_t p = ((size_t)row * w + col) * 2;
            uint16_t c = ((uint16_t)buf[p] << 8) | buf[p + 1];
            shadow_set_px(x + col, y + row, c);
        }
    }
}

/* Clip a rectangle to the panel bounds in place. Returns false if the
 * result is empty (nothing to draw), unlike the strict fill_rect()
 * below which raises on out-of-bounds. Shapes like circles and lines
 * routinely have parts that fall off the edge, so the primitives that
 * build on this clip silently instead of erroring. */
static bool clip_rect(int *x, int *y, int *w, int *h)
{
    if (*x < 0) { *w += *x; *x = 0; }
    if (*y < 0) { *h += *y; *y = 0; }
    if (*x + *w > s_width)  *w = (int)s_width  - *x;
    if (*y + *h > s_height) *h = (int)s_height - *y;
    return (*w > 0 && *h > 0 && *x < s_width && *y < s_height);
}

static void do_fill_rect_clip(int x, int y, int w, int h, uint16_t color)
{
    if (!clip_rect(&x, &y, &w, &h)) return;
    set_window((uint16_t)x, (uint16_t)y, (uint16_t)(x + w - 1), (uint16_t)(y + h - 1));
    stream_solid((uint32_t)w * (uint32_t)h, color);
    shadow_fill_rect(x, y, w, h, color);
}

static void do_draw_pixel(int x, int y, uint16_t color)
{
    if (x < 0 || y < 0 || x >= s_width || y >= s_height) return;
    set_window((uint16_t)x, (uint16_t)y, (uint16_t)x, (uint16_t)y);
    stream_solid(1, color);
    shadow_set_px(x, y, color);
}

/* -------------------------------------------------------------------
 * text rendering -- font_petme128_8x8 is column-major: 8 bytes/char,
 * byte i is column i, bit j of that byte is row j. Same table and
 * bit layout MicroPython's framebuf.text() uses internally.
 * ---------------------------------------------------------------- */
static void ensure_glyph_buf(size_t needed_bytes)
{
    if (s_glyph_buf == NULL || s_font_glyph_buf_cap < needed_bytes) {
        if (s_glyph_buf) heap_caps_free(s_glyph_buf);
        s_glyph_buf = heap_caps_malloc(needed_bytes, MALLOC_CAP_DMA);
        if (s_glyph_buf == NULL) {
            s_font_glyph_buf_cap = 0;
            mp_raise_msg(&mp_type_MemoryError, MP_ERROR_TEXT("no DMA memory for glyph buffer"));
        }
        s_font_glyph_buf_cap = needed_bytes;
    }
}

/* Resolve a font id (0 = built-in font_petme128_8x8, 1..MAX_FONTS-1 =
 * user-registered via register_font()) to its glyph table + metrics.
 * Falls back to font 0 for an unregistered id rather than raising, so
 * a typo'd font id degrades gracefully instead of crashing a draw
 * loop mid-frame. */
static void resolve_font(int font_id, const uint8_t **data, uint8_t *cw, uint8_t *ch,
                          uint8_t *first, uint8_t *last)
{
    if (font_id == 0 || font_id < 0 || font_id >= MAX_FONTS || !s_fonts[font_id].in_use) {
        *data = font_petme128_8x8;
        *cw = FONT_CHAR_W; *ch = FONT_CHAR_H;
        *first = FONT_FIRST_CHAR; *last = FONT_LAST_CHAR;
        return;
    }
    font_entry_t *f = &s_fonts[font_id];
    *data = f->data; *cw = f->char_w; *ch = f->char_h;
    *first = f->first_char; *last = f->last_char;
}

/* Draws one glyph (from font `font_id`) at (x,y). If bg_transparent,
 * only foreground pixels are plotted (one address-window per lit
 * pixel -- slower, but leaves whatever's already behind the glyph
 * untouched). Otherwise the whole cell (fg+bg) is built in a small
 * buffer and sent as a single DMA transfer when it fully fits
 * on-panel. Mirrors into the shadow framebuffer either way. */
static void draw_glyph_font(int x, int y, char c, uint16_t fg, uint16_t bg, bool bg_transparent, int font_id)
{
    const uint8_t *font_data; uint8_t cw, ch, first, last;
    resolve_font(font_id, &font_data, &cw, &ch, &first, &last);

    if (c < (char)first || c > (char)last) c = ' ';
    if (c < (char)first) c = (char)first; /* space itself out of range: clamp instead of UB */
    const uint8_t *glyph = &font_data[((uint8_t)c - first) * cw];

    if (bg_transparent) {
        for (int col = 0; col < cw; col++) {
            uint8_t line = glyph[col];
            for (int row = 0; row < ch; row++) {
                if ((line >> row) & 1) {
                    do_draw_pixel(x + col, y + row, fg);
                }
            }
        }
        return;
    }

    int cx = x, cy = y, cclw = cw, cclh = ch;
    if (!clip_rect(&cx, &cy, &cclw, &cclh)) return;

    if (cclw != cw || cclh != ch) {
        /* clipped by a screen edge: fall back to per-pixel so we don't
           send pixels that belong to a different part of the screen */
        for (int col = 0; col < cw; col++) {
            uint8_t line = glyph[col];
            for (int row = 0; row < ch; row++) {
                do_draw_pixel(x + col, y + row, ((line >> row) & 1) ? fg : bg);
            }
        }
        return;
    }

    size_t need = (size_t)cw * (size_t)ch * 2;
    ensure_glyph_buf(need);
    uint8_t fg_hi = (uint8_t)(fg >> 8), fg_lo = (uint8_t)(fg & 0xFF);
    uint8_t bg_hi = (uint8_t)(bg >> 8), bg_lo = (uint8_t)(bg & 0xFF);

    for (int row = 0; row < ch; row++) {
        for (int col = 0; col < cw; col++) {
            bool on = (glyph[col] >> row) & 1;
            int p = (row * cw + col) * 2;
            s_glyph_buf[p]     = on ? fg_hi : bg_hi;
            s_glyph_buf[p + 1] = on ? fg_lo : bg_lo;
        }
    }

    set_window((uint16_t)x, (uint16_t)y, (uint16_t)(x + cw - 1), (uint16_t)(y + ch - 1));
    io_check(esp_lcd_panel_io_tx_color(s_io, LCD_CMD_RAMWRC, s_glyph_buf, need), "text");
    shadow_blit_bytes(x, y, cw, ch, s_glyph_buf);
}

/* Back-compat wrapper: original 8x8-only call sites (draw_text8x8)
 * keep working unchanged, always using font 0. */
static void draw_glyph(int x, int y, char c, uint16_t fg, uint16_t bg, bool bg_transparent)
{
    draw_glyph_font(x, y, c, fg, bg, bg_transparent, 0);
}

/* -------------------------------------------------------------------
 * moclcd.draw_text8x8(x, y, text, fg, bg=None)
 * bg omitted/None -> transparent background (only fg pixels drawn).
 * ---------------------------------------------------------------- */
static mp_obj_t moclcd_draw_text8x8(size_t n_args, const mp_obj_t *args_in)
{
    require_init();

    int x = mp_obj_get_int(args_in[0]);
    int y = mp_obj_get_int(args_in[1]);

    size_t len;
    const char *text = mp_obj_str_get_data(args_in[2], &len);

    uint16_t fg = (uint16_t)mp_obj_get_int(args_in[3]);

    bool bg_transparent = (n_args < 5) || (args_in[4] == mp_const_none);
    uint16_t bg = bg_transparent ? 0 : (uint16_t)mp_obj_get_int(args_in[4]);

    for (size_t i = 0; i < len; i++) {
        draw_glyph(x + (int)i * FONT_CHAR_W, y, text[i], fg, bg, bg_transparent);
    }
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(moclcd_draw_text8x8_obj, 4, 5, moclcd_draw_text8x8);

/* -------------------------------------------------------------------
 * moclcd.register_font(font_id, glyph_data, char_w, char_h, first_char, last_char)
 *
 * Registers an additional font (1..MAX_FONTS-1; font id 0 is always
 * the built-in font_petme128_8x8 and can't be overwritten). glyph_data
 * is a bytes-like object: a column-major bitmap table just like
 * font_petme128_8x8 -- char_w bytes per glyph, bit j of byte i is row
 * j of column i -- covering (last_char - first_char + 1) characters.
 * The data is copied into internal storage, so the Python-side buffer
 * doesn't need to stay alive afterward.
 *
 * Once registered, use draw_text(x, y, text, fg, bg=None, font=font_id)
 * to draw with it.
 * ---------------------------------------------------------------- */
static mp_obj_t moclcd_register_font(size_t n_args, const mp_obj_t *args_in)
{
    int font_id = mp_obj_get_int(args_in[0]);
    if (font_id <= 0 || font_id >= MAX_FONTS) {
        mp_raise_ValueError(MP_ERROR_TEXT("font_id must be 1..MAX_FONTS-1 (0 is the built-in font)"));
    }

    mp_buffer_info_t bufinfo;
    mp_get_buffer_raise(args_in[1], &bufinfo, MP_BUFFER_READ);

    int char_w = mp_obj_get_int(args_in[2]);
    int char_h = mp_obj_get_int(args_in[3]);
    int first  = mp_obj_get_int(args_in[4]);
    int last   = mp_obj_get_int(args_in[5]);

    if (char_w <= 0 || char_w > 32 || char_h <= 0 || char_h > 32 ||
        first < 0 || last > 255 || last < first) {
        mp_raise_ValueError(MP_ERROR_TEXT("invalid font metrics"));
    }

    size_t n_chars = (size_t)(last - first + 1);
    size_t expected_len = n_chars * (size_t)char_w;
    if (bufinfo.len != expected_len) {
        mp_raise_msg_varg(&mp_type_ValueError, MP_ERROR_TEXT(
            "glyph_data length %d does not match char_w*num_chars (%d)"),
            (int)bufinfo.len, (int)expected_len);
    }

    font_entry_t *f = &s_fonts[font_id];
    if (f->in_use && f->data) {
        heap_caps_free((void *)f->data);
        f->data = NULL;
        f->in_use = false;
    }

    uint8_t *copy = heap_caps_malloc(expected_len, MALLOC_CAP_DEFAULT);
    if (!copy) {
        mp_raise_msg(&mp_type_MemoryError, MP_ERROR_TEXT("no memory to register font"));
    }
    memcpy(copy, bufinfo.buf, expected_len);

    f->data       = copy;
    f->char_w     = (uint8_t)char_w;
    f->char_h     = (uint8_t)char_h;
    f->first_char = (uint8_t)first;
    f->last_char  = (uint8_t)last;
    f->in_use     = true;

    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(moclcd_register_font_obj, 6, 6, moclcd_register_font);

/* -------------------------------------------------------------------
 * moclcd.unregister_font(font_id)
 * Frees a previously registered font. Font 0 (built-in) can't be
 * unregistered -- this is a no-op for font_id 0.
 * ---------------------------------------------------------------- */
static mp_obj_t moclcd_unregister_font(mp_obj_t font_id_in)
{
    int font_id = mp_obj_get_int(font_id_in);
    if (font_id <= 0 || font_id >= MAX_FONTS) return mp_const_none;
    font_entry_t *f = &s_fonts[font_id];
    if (f->in_use && f->data) {
        heap_caps_free((void *)f->data);
    }
    memset(f, 0, sizeof(*f));
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_1(moclcd_unregister_font_obj, moclcd_unregister_font);

/* -------------------------------------------------------------------
 * moclcd.draw_text(x, y, text, fg, bg=None, font=0)
 * Same semantics as draw_text8x8(), but with an extra `font` id
 * selecting which registered font table to use (0 = built-in 8x8).
 * Character advance uses that font's own char_w, so mixed-width fonts
 * lay out correctly without any Python-side width bookkeeping.
 * ---------------------------------------------------------------- */
static mp_obj_t moclcd_draw_text(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args)
{
    require_init();

    enum { ARG_x, ARG_y, ARG_text, ARG_fg, ARG_bg, ARG_font };
    static const mp_arg_t allowed[] = {
        { MP_QSTR_x,    MP_ARG_REQUIRED | MP_ARG_INT, {.u_int = 0} },
        { MP_QSTR_y,    MP_ARG_REQUIRED | MP_ARG_INT, {.u_int = 0} },
        { MP_QSTR_text, MP_ARG_REQUIRED | MP_ARG_OBJ, {.u_rom_obj = MP_ROM_NONE} },
        { MP_QSTR_fg,   MP_ARG_REQUIRED | MP_ARG_INT, {.u_int = 0} },
        { MP_QSTR_bg,   MP_ARG_KW_ONLY   | MP_ARG_OBJ, {.u_rom_obj = MP_ROM_NONE} },
        { MP_QSTR_font, MP_ARG_KW_ONLY   | MP_ARG_INT, {.u_int = 0} },
    };
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed), allowed, args);

    int x = args[ARG_x].u_int;
    int y = args[ARG_y].u_int;
    size_t len;
    const char *text = mp_obj_str_get_data(args[ARG_text].u_obj, &len);
    uint16_t fg = (uint16_t)args[ARG_fg].u_int;
    bool bg_transparent = (args[ARG_bg].u_obj == mp_const_none);
    uint16_t bg = bg_transparent ? 0 : (uint16_t)mp_obj_get_int(args[ARG_bg].u_obj);
    int font_id = args[ARG_font].u_int;

    const uint8_t *font_data; uint8_t cw, ch, first, last;
    resolve_font(font_id, &font_data, &cw, &ch, &first, &last);
    (void)font_data; (void)ch; (void)first; (void)last;

    for (size_t i = 0; i < len; i++) {
        draw_glyph_font(x + (int)i * cw, y, text[i], fg, bg, bg_transparent, font_id);
    }
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_KW(moclcd_draw_text_obj, 4, moclcd_draw_text);

/* -------------------------------------------------------------------
 * moclcd.font_metrics(font_id)
 * Returns (char_w, char_h, first_char, last_char) for a font id, so
 * Python-side layout code (centering, wrapping) doesn't need to
 * hardcode 8x8 assumptions when a custom font is in use.
 * ---------------------------------------------------------------- */
static mp_obj_t moclcd_font_metrics(mp_obj_t font_id_in)
{
    int font_id = mp_obj_get_int(font_id_in);
    const uint8_t *font_data; uint8_t cw, ch, first, last;
    resolve_font(font_id, &font_data, &cw, &ch, &first, &last);
    (void)font_data;
    mp_obj_t tuple[4] = {
        mp_obj_new_int(cw), mp_obj_new_int(ch),
        mp_obj_new_int(first), mp_obj_new_int(last),
    };
    return mp_obj_new_tuple(4, tuple);
}
static MP_DEFINE_CONST_FUN_OBJ_1(moclcd_font_metrics_obj, moclcd_font_metrics);

/* -------------------------------------------------------------------
 * moclcd.draw_bmp(path, x, y, w=None, h=None, max_w=None, max_h=None)
 * Minimal loader: uncompressed 24-bit BMP only (no palette, no RLE).
 * w/h/max_w/max_h of 0 (the default) are treated as "unset", same as
 * the Python version's None. The whole converted image is built in
 * one DMA-capable buffer and sent as a single transfer, same approach
 * as blit().
 *
 * Note: this uses the C library's fopen()/fread(), so `path` must be
 * reachable through the ESP-IDF VFS (e.g. internal flash or SD mounted
 * via esp_vfs) -- the same filesystem MicroPython's own open() sees.
 * ---------------------------------------------------------------- */
static mp_obj_t moclcd_draw_bmp(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args)
{
    require_init();

    enum { ARG_path, ARG_x, ARG_y, ARG_w, ARG_h, ARG_max_w, ARG_max_h };
    static const mp_arg_t allowed[] = {
        { MP_QSTR_path,  MP_ARG_REQUIRED | MP_ARG_OBJ, {.u_rom_obj = MP_ROM_NONE} },
        { MP_QSTR_x,     MP_ARG_REQUIRED | MP_ARG_INT, {.u_int = 0} },
        { MP_QSTR_y,     MP_ARG_REQUIRED | MP_ARG_INT, {.u_int = 0} },
        { MP_QSTR_w,     MP_ARG_KW_ONLY  | MP_ARG_INT, {.u_int = 0} },
        { MP_QSTR_h,     MP_ARG_KW_ONLY  | MP_ARG_INT, {.u_int = 0} },
        { MP_QSTR_max_w, MP_ARG_KW_ONLY  | MP_ARG_INT, {.u_int = 0} },
        { MP_QSTR_max_h, MP_ARG_KW_ONLY  | MP_ARG_INT, {.u_int = 0} },
    };
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed), allowed, args);

    const char *path = mp_obj_str_get_str(args[ARG_path].u_obj);
    int x = args[ARG_x].u_int;
    int y = args[ARG_y].u_int;
    int want_w = args[ARG_w].u_int;
    int want_h = args[ARG_h].u_int;
    int max_w  = args[ARG_max_w].u_int;
    int max_h  = args[ARG_max_h].u_int;

    FILE *f = fopen(path, "rb");
    if (!f) {
        /* This is very commonly *not* a missing file -- it's this
           function being called before MicroPython's VFS is mounted
           yet (e.g. from panel_init()/boot code that runs before
           os.mount()/the filesystem is set up). fopen()'s C stdio
           layer routes through the VFS the same way MicroPython's own
           open() does, so if nothing is mounted there's no root to
           resolve `path` against and every path looks like ENOENT,
           even a path that will exist a moment later once the FS is
           mounted. Surface that distinction instead of a bare ENOENT
           so it's obvious what to fix, rather than looking like a
           typo'd filename. */
        if (!mp_vfs_mount_is_ready()) {
            mp_raise_msg(&mp_type_OSError, MP_ERROR_TEXT(
                "draw_bmp: filesystem not mounted yet -- call draw_bmp() "
                "after os.mount()/VFS init completes, not from early boot code"));
        }
        mp_raise_OSError(MP_ENOENT);
    }

    uint8_t header[54];
    if (fread(header, 1, 54, f) != 54 || header[0] != 'B' || header[1] != 'M') {
        fclose(f);
        mp_raise_ValueError(MP_ERROR_TEXT("not a BMP file"));
    }

    uint32_t data_offset = (uint32_t)header[10] | ((uint32_t)header[11] << 8) |
                           ((uint32_t)header[12] << 16) | ((uint32_t)header[13] << 24);
    int32_t bmp_w = (int32_t)((uint32_t)header[18] | ((uint32_t)header[19] << 8) |
                              ((uint32_t)header[20] << 16) | ((uint32_t)header[21] << 24));
    int32_t bmp_h_raw = (int32_t)((uint32_t)header[22] | ((uint32_t)header[23] << 8) |
                                  ((uint32_t)header[24] << 16) | ((uint32_t)header[25] << 24));
    uint16_t bpp = (uint16_t)header[28] | ((uint16_t)header[29] << 8);
    uint32_t compression = (uint32_t)header[30] | ((uint32_t)header[31] << 8) |
                           ((uint32_t)header[32] << 16) | ((uint32_t)header[33] << 24);

    if (bpp != 24 || compression != 0) {
        fclose(f);
        mp_raise_ValueError(MP_ERROR_TEXT("only uncompressed 24-bit BMP is supported"));
    }

    bool top_down = bmp_h_raw < 0;
    int32_t bmp_h = top_down ? -bmp_h_raw : bmp_h_raw;
    int row_size = ((bmp_w * 3 + 3) / 4) * 4; /* rows padded to 4 bytes */

    int out_w = want_w > 0 ? want_w : (int)bmp_w;
    int out_h = want_h > 0 ? want_h : (int)bmp_h;
    if (max_w > 0 && out_w > max_w) out_w = max_w;
    if (max_h > 0 && out_h > max_h) out_h = max_h;

    if (out_w <= 0 || out_h <= 0) {
        fclose(f);
        return mp_const_none;
    }

    int dx = x, dy = y, dw = out_w, dh = out_h;
    if (!clip_rect(&dx, &dy, &dw, &dh)) {
        fclose(f);
        return mp_const_none;
    }

    int skip_rows = dy - y; /* how many source rows/cols the top/left clip ate */
    int skip_cols = dx - x;

    uint8_t *row_buf = heap_caps_malloc(row_size, MALLOC_CAP_DEFAULT);
    uint8_t *img = heap_caps_malloc((size_t)dw * (size_t)dh * 2, MALLOC_CAP_DMA);
    if (!row_buf || !img) {
        if (row_buf) heap_caps_free(row_buf);
        if (img) heap_caps_free(img);
        fclose(f);
        mp_raise_msg(&mp_type_MemoryError, MP_ERROR_TEXT("no memory for bmp load"));
    }

    for (int row = 0; row < dh; row++) {
        int dest_row = row + skip_rows;
        int src_row = top_down ? dest_row : ((int)bmp_h - 1 - dest_row);

        fseek(f, (long)(data_offset + (uint32_t)src_row * (uint32_t)row_size), SEEK_SET);
        if (fread(row_buf, 1, row_size, f) != (size_t)row_size) {
            break; /* short read / EOF: stop rather than send garbage rows */
        }

        int p = row * dw * 2;
        for (int col = 0; col < dw; col++) {
            int src_col = col + skip_cols;
            uint8_t b = row_buf[src_col * 3 + 0];
            uint8_t g = row_buf[src_col * 3 + 1];
            uint8_t r = row_buf[src_col * 3 + 2];
            uint16_t c = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
            img[p++] = (uint8_t)(c >> 8);
            img[p++] = (uint8_t)(c & 0xFF);
        }
    }

    set_window((uint16_t)dx, (uint16_t)dy, (uint16_t)(dx + dw - 1), (uint16_t)(dy + dh - 1));
    io_check(esp_lcd_panel_io_tx_color(s_io, LCD_CMD_RAMWRC, img, (size_t)dw * (size_t)dh * 2), "bmp");
    shadow_blit_bytes(dx, dy, dw, dh, img);

    heap_caps_free(row_buf);
    heap_caps_free(img);
    fclose(f);

    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_KW(moclcd_draw_bmp_obj, 3, moclcd_draw_bmp);

/* -------------------------------------------------------------------
 * moclcd.init(pclk=10_000_000, width=320, height=480)
 * ---------------------------------------------------------------- */
static mp_obj_t moclcd_init(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args)
{
    enum { ARG_pclk, ARG_width, ARG_height, ARG_madctl };
    static const mp_arg_t allowed[] = {
        { MP_QSTR_pclk,   MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = 10000000} },
        { MP_QSTR_width,  MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = 480} },
        { MP_QSTR_height, MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = 320} },
        /* 0x28 = landscape (MV set). 0x48 = portrait (original orientation).
           0x88 / 0xE8 = the other two 90-degree rotations. If the image
           comes up mirrored or upside down in landscape, try 0xE8. */
        { MP_QSTR_madctl, MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = 0x28} },
    };
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed), allowed, args);

    if (s_shadow_fb && (s_width != (uint16_t)args[ARG_width].u_int ||
                         s_height != (uint16_t)args[ARG_height].u_int)) {
        /* dimensions changing on a re-init: old shadow buffer is the
           wrong size, drop it -- ensure_shadow_fb() will reallocate
           lazily at the new size next time mirroring is needed */
        heap_caps_free(s_shadow_fb);
        s_shadow_fb = NULL;
        s_shadow_enabled = false;
    }

    s_width  = (uint16_t)args[ARG_width].u_int;
    s_height = (uint16_t)args[ARG_height].u_int;
    s_madctl = (uint8_t)args[ARG_madctl].u_int;

    /* --- Your exact data pins (D0 through D7) --- */
    int data_gpios[8] = { 16, 15, 11, 10, 9, 4, 18, 17 };

    esp_lcd_i80_bus_config_t bus_cfg = {
        .dc_gpio_num = 13, /* RS */
        .wr_gpio_num = 14, /* WR */
        .clk_src     = LCD_CLK_SRC_PLL160M,
        .data_gpio_nums = {
            data_gpios[0], data_gpios[1], data_gpios[2], data_gpios[3],
            data_gpios[4], data_gpios[5], data_gpios[6], data_gpios[7],
        },
        .bus_width          = 8,
        /* generous ceiling so a full-frame blit() can go out in one shot;
           fill_rect() still chunks itself for pipelining regardless */
        .max_transfer_bytes = (size_t)s_width * (size_t)s_height * 2,
    };
    io_check(esp_lcd_new_i80_bus(&bus_cfg, &s_bus), "esp_lcd_new_i80_bus");

    esp_lcd_panel_io_i80_config_t io_cfg = {
        .cs_gpio_num       = -1, /* CS tied LOW in hardware */
        .pclk_hz           = (uint32_t)args[ARG_pclk].u_int,
        .trans_queue_depth = 10,
        .dc_levels = {
            .dc_idle_level  = 0,
            .dc_cmd_level   = 0,
            .dc_dummy_level = 0,
            .dc_data_level  = 1,
        },
        .lcd_cmd_bits   = 8,
        .lcd_param_bits = 8,
    };
    io_check(esp_lcd_new_panel_io_i80(s_bus, &io_cfg, &s_io), "esp_lcd_new_panel_io_i80");

    /* --- RD pin, idle HIGH --- */
    mp_hal_pin_output(s_rd_pin);
    mp_hal_pin_write(s_rd_pin, 1);

    /* --- Backlight, ON --- */
    mp_hal_pin_output(s_bl_pin);
    mp_hal_pin_write(s_bl_pin, 1);

    /* --- Reset pin, idle HIGH --- */
    mp_hal_pin_output(s_reset_pin);
    mp_hal_pin_write(s_reset_pin, 1);
    s_has_reset = true;

    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_KW(moclcd_init_obj, 0, moclcd_init);

/* -------------------------------------------------------------------
 * moclcd.reset()
 * ---------------------------------------------------------------- */
static mp_obj_t moclcd_reset(void)
{
    if (!s_has_reset) {
        mp_raise_msg(&mp_type_OSError, MP_ERROR_TEXT("no reset pin configured"));
    }
    mp_hal_pin_write(s_reset_pin, 1);
    mp_hal_delay_us(1000 * 1000);
    mp_hal_pin_write(s_reset_pin, 0);
    mp_hal_delay_us(1000 * 1000);
    mp_hal_pin_write(s_reset_pin, 1);
    mp_hal_delay_us(150 * 1000);
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_0(moclcd_reset_obj, moclcd_reset);

/* -------------------------------------------------------------------
 * moclcd.panel_init()
 * Runs the exact working 0x01 / 0x11 / 0x3A / 0x36 / 0x2A / 0x2B / 0x29
 * sequence from the Python script, sized to width/height from init().
 * ---------------------------------------------------------------- */
static mp_obj_t moclcd_panel_init(void)
{
    require_init();

    /* matches time.sleep_ms(20) between reset() and the 0x01 command in
       the working Python script -- in raw C there's no interpreter
       overhead to give you this gap for free, so it's made explicit */
    mp_hal_delay_us(20 * 1000);

    lcd_cmd_raw(0x01, NULL, 0);              /* software reset */
    mp_hal_delay_us(150 * 1000);

    lcd_cmd_raw(0x11, NULL, 0);              /* sleep out */
    mp_hal_delay_us(150 * 1000);

    uint8_t colmod = 0x55;
    lcd_cmd_raw(0x3A, &colmod, 1);           /* 16bpp */
    mp_hal_delay_us(10 * 1000);

    uint8_t madctl = s_madctl;
    lcd_cmd_raw(0x36, &madctl, 1);
    mp_hal_delay_us(10 * 1000);

    uint16_t x1 = s_width - 1;
    uint16_t y1 = s_height - 1;
    uint8_t caset[4] = { 0x00, 0x00, (uint8_t)(x1 >> 8), (uint8_t)(x1 & 0xFF) };
    lcd_cmd_raw(0x2A, caset, sizeof(caset));

    uint8_t paset[4] = { 0x00, 0x00, (uint8_t)(y1 >> 8), (uint8_t)(y1 & 0xFF) };
    lcd_cmd_raw(0x2B, paset, sizeof(paset));

    lcd_cmd_raw(0x29, NULL, 0);              /* display on */
    mp_hal_delay_us(50 * 1000);

    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_0(moclcd_panel_init_obj, moclcd_panel_init);

/* -------------------------------------------------------------------
 * moclcd.backlight_init(freq_hz=5000, resolution_bits=8)
 * Sets up an LEDC PWM channel on the backlight pin. Call once, before
 * using backlight_set() or expecting backlight() to dim rather than
 * just switch on/off.
 * ---------------------------------------------------------------- */
static mp_obj_t moclcd_backlight_init(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args)
{
    enum { ARG_freq, ARG_res_bits };
    static const mp_arg_t allowed[] = {
        { MP_QSTR_freq_hz,          MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = 5000} },
        { MP_QSTR_resolution_bits,  MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = 8} },
    };
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed), allowed, args);

    int res_bits = args[ARG_res_bits].u_int;

    ledc_timer_config_t timer_cfg = {
        .speed_mode      = LEDC_LOW_SPEED_MODE,
        .duty_resolution = (ledc_timer_bit_t)res_bits,
        .timer_num       = LEDC_TIMER_0,
        .freq_hz         = (uint32_t)args[ARG_freq].u_int,
        .clk_cfg         = LEDC_AUTO_CLK,
    };
    io_check(ledc_timer_config(&timer_cfg), "ledc_timer_config");

    ledc_channel_config_t ch_cfg = {
        .gpio_num   = s_bl_pin,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel    = LEDC_CHANNEL_0,
        .intr_type  = LEDC_INTR_DISABLE,
        .timer_sel  = LEDC_TIMER_0,
        .duty       = 0,
        .hpoint     = 0,
    };
    io_check(ledc_channel_config(&ch_cfg), "ledc_channel_config");

    s_bl_duty_max   = (1u << res_bits) - 1;
    s_bl_pwm_inited = true;

    /* start fully on, matching the plain-GPIO backlight()'s prior default */
    io_check(ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, s_bl_duty_max), "ledc_set_duty");
    io_check(ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0), "ledc_update_duty");

    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_KW(moclcd_backlight_init_obj, 0, moclcd_backlight_init);

/* -------------------------------------------------------------------
 * moclcd.backlight_set(level)
 * level is a 0.0-1.0 brightness fraction. Requires backlight_init().
 * ---------------------------------------------------------------- */
static mp_obj_t moclcd_backlight_set(mp_obj_t level_in)
{
    if (!s_bl_pwm_inited) {
        mp_raise_msg(&mp_type_OSError, MP_ERROR_TEXT("moclcd.backlight_init() must be called first"));
    }
    mp_float_t level = mp_obj_get_float(level_in);
    if (level < 0) level = 0;
    if (level > 1) level = 1;

    uint32_t duty = (uint32_t)(level * s_bl_duty_max + 0.5f);
    io_check(ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, duty), "ledc_set_duty");
    io_check(ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0), "ledc_update_duty");

    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_1(moclcd_backlight_set_obj, moclcd_backlight_set);

/* -------------------------------------------------------------------
 * moclcd.backlight(on)
 * Plain on/off. If backlight_init() has been called, this drives the
 * PWM duty to max/0 instead of touching the pin directly (the pin is
 * now owned by the LEDC peripheral, not plain GPIO).
 * ---------------------------------------------------------------- */
static mp_obj_t moclcd_backlight(mp_obj_t on_in)
{
    bool on = mp_obj_is_true(on_in);

    if (s_bl_pwm_inited) {
        uint32_t duty = on ? s_bl_duty_max : 0;
        io_check(ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, duty), "ledc_set_duty");
        io_check(ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0), "ledc_update_duty");
    } else {
        mp_hal_pin_write(s_bl_pin, on ? 1 : 0);
    }
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_1(moclcd_backlight_obj, moclcd_backlight);

/* -------------------------------------------------------------------
 * moclcd.cmd(cmd, params=None) -- raw passthrough, kept for flexibility
 * ---------------------------------------------------------------- */
static mp_obj_t moclcd_cmd(size_t n_args, const mp_obj_t *args_in)
{
    require_init();
    int cmd = mp_obj_get_int(args_in[0]);

    const void *buf = NULL;
    size_t len = 0;
    mp_buffer_info_t bufinfo;
    if (n_args == 2 && args_in[1] != mp_const_none) {
        mp_get_buffer_raise(args_in[1], &bufinfo, MP_BUFFER_READ);
        buf = bufinfo.buf;
        len = bufinfo.len;
    }
    lcd_cmd_raw((uint8_t)cmd, buf, len);
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(moclcd_cmd_obj, 1, 2, moclcd_cmd);

/* -------------------------------------------------------------------
 * moclcd.data(buf) -- raw passthrough, still available for one-off writes
 * ---------------------------------------------------------------- */
static mp_obj_t moclcd_data(mp_obj_t buf_in)
{
    require_init();
    mp_buffer_info_t bufinfo;
    mp_get_buffer_raise(buf_in, &bufinfo, MP_BUFFER_READ);
    io_check(esp_lcd_panel_io_tx_color(s_io, LCD_CMD_RAMWRC, bufinfo.buf, bufinfo.len), "data write");
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_1(moclcd_data_obj, moclcd_data);

/* -------------------------------------------------------------------
 * moclcd.fill_rect(x, y, w, h, color)
 *
 * Sets the address window once, fills a small DMA-capable scratch
 * buffer with the target color, then resends that same buffer in
 * chunks via esp_lcd_panel_io_tx_color(). Because the content never
 * changes, the buffer can be safely queued again even while an earlier
 * chunk is still draining out over DMA, so up to trans_queue_depth
 * chunks stay in flight at once instead of the CPU waiting on each one.
 * ---------------------------------------------------------------- */
static mp_obj_t moclcd_fill_rect(size_t n_args, const mp_obj_t *args_in)
{
    require_init();

    int x = mp_obj_get_int(args_in[0]);
    int y = mp_obj_get_int(args_in[1]);
    int w = mp_obj_get_int(args_in[2]);
    int h = mp_obj_get_int(args_in[3]);
    uint16_t color = (uint16_t)mp_obj_get_int(args_in[4]);

    if (x < 0 || y < 0 || w <= 0 || h <= 0 ||
        x + w > s_width || y + h > s_height) {
        mp_raise_ValueError(MP_ERROR_TEXT("fill_rect out of bounds"));
    }

    set_window((uint16_t)x, (uint16_t)y, (uint16_t)(x + w - 1), (uint16_t)(y + h - 1));
    stream_solid((uint32_t)w * (uint32_t)h, color);

    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(moclcd_fill_rect_obj, 5, 5, moclcd_fill_rect);

/* -------------------------------------------------------------------
 * moclcd.fill_screen(color)
 * ---------------------------------------------------------------- */
static mp_obj_t moclcd_fill_screen(mp_obj_t color_in)
{
    mp_obj_t args[5] = {
        mp_obj_new_int(0), mp_obj_new_int(0),
        mp_obj_new_int(s_width), mp_obj_new_int(s_height),
        color_in
    };
    return moclcd_fill_rect(5, args);
}
static MP_DEFINE_CONST_FUN_OBJ_1(moclcd_fill_screen_obj, moclcd_fill_screen);

/* -------------------------------------------------------------------
 * moclcd.blit(x, y, w, h, buf)
 * Pushes an arbitrary RGB565 pixel buffer (w*h*2 bytes, MSB first per
 * pixel) into the window in one DMA-backed transfer. Useful for
 * sprites, images, or a full framebuffer flush.
 * ---------------------------------------------------------------- */
static mp_obj_t moclcd_blit(size_t n_args, const mp_obj_t *args_in)
{
    require_init();

    int x = mp_obj_get_int(args_in[0]);
    int y = mp_obj_get_int(args_in[1]);
    int w = mp_obj_get_int(args_in[2]);
    int h = mp_obj_get_int(args_in[3]);

    if (x < 0 || y < 0 || w <= 0 || h <= 0 ||
        x + w > s_width || y + h > s_height) {
        mp_raise_ValueError(MP_ERROR_TEXT("blit out of bounds"));
    }

    mp_buffer_info_t bufinfo;
    mp_get_buffer_raise(args_in[4], &bufinfo, MP_BUFFER_READ);

    size_t expected = (size_t)w * (size_t)h * 2;
    if (bufinfo.len != expected) {
        mp_raise_ValueError(MP_ERROR_TEXT("buffer size does not match w*h*2"));
    }

    set_window((uint16_t)x, (uint16_t)y, (uint16_t)(x + w - 1), (uint16_t)(y + h - 1));
    io_check(esp_lcd_panel_io_tx_color(s_io, LCD_CMD_RAMWRC, bufinfo.buf, bufinfo.len), "blit");
    shadow_blit_bytes(x, y, w, h, (const uint8_t *)bufinfo.buf);

    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(moclcd_blit_obj, 5, 5, moclcd_blit);

/* -------------------------------------------------------------------
 * moclcd.draw_pixel(x, y, color)
 * Silently clipped if off-panel (consistent with the primitives below,
 * unlike the strict fill_rect()/blit() calls above).
 * ---------------------------------------------------------------- */
static mp_obj_t moclcd_draw_pixel(size_t n_args, const mp_obj_t *args_in)
{
    require_init();
    int x = mp_obj_get_int(args_in[0]);
    int y = mp_obj_get_int(args_in[1]);
    uint16_t color = (uint16_t)mp_obj_get_int(args_in[2]);
    do_draw_pixel(x, y, color);
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(moclcd_draw_pixel_obj, 3, 3, moclcd_draw_pixel);

/* -------------------------------------------------------------------
 * moclcd.draw_line(x0, y0, x1, y1, color)
 * Horizontal/vertical lines take a fast path through fill_rect's
 * chunked DMA stream (a "line" one pixel thick). Diagonals fall back
 * to a pixel-by-pixel Bresenham walk, since each pixel needs its own
 * address window on this bus.
 * ---------------------------------------------------------------- */
static mp_obj_t moclcd_draw_line(size_t n_args, const mp_obj_t *args_in)
{
    require_init();
    int x0 = mp_obj_get_int(args_in[0]);
    int y0 = mp_obj_get_int(args_in[1]);
    int x1 = mp_obj_get_int(args_in[2]);
    int y1 = mp_obj_get_int(args_in[3]);
    uint16_t color = (uint16_t)mp_obj_get_int(args_in[4]);

    if (y0 == y1) {
        int x = x0 < x1 ? x0 : x1;
        int w = (x0 < x1 ? x1 - x0 : x0 - x1) + 1;
        do_fill_rect_clip(x, y0, w, 1, color);
        return mp_const_none;
    }
    if (x0 == x1) {
        int y = y0 < y1 ? y0 : y1;
        int h = (y0 < y1 ? y1 - y0 : y0 - y1) + 1;
        do_fill_rect_clip(x0, y, 1, h, color);
        return mp_const_none;
    }

    int dx = x1 > x0 ? x1 - x0 : x0 - x1;
    int sx = x0 < x1 ? 1 : -1;
    int dy = y1 > y0 ? -(y1 - y0) : (y0 - y1);
    int sy = y0 < y1 ? 1 : -1;
    int err = dx + dy;

    int x = x0, y = y0;
    for (;;) {
        do_draw_pixel(x, y, color);
        if (x == x1 && y == y1) break;
        int e2 = 2 * err;
        if (e2 >= dy) { err += dy; x += sx; }
        if (e2 <= dx) { err += dx; y += sy; }
    }
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(moclcd_draw_line_obj, 5, 5, moclcd_draw_line);

/* -------------------------------------------------------------------
 * moclcd.draw_rect(x, y, w, h, color)
 * Outline only (four 1px-thick edges via the DMA fill path). Use
 * fill_rect() for a solid rectangle.
 * ---------------------------------------------------------------- */
static mp_obj_t moclcd_draw_rect(size_t n_args, const mp_obj_t *args_in)
{
    require_init();
    int x = mp_obj_get_int(args_in[0]);
    int y = mp_obj_get_int(args_in[1]);
    int w = mp_obj_get_int(args_in[2]);
    int h = mp_obj_get_int(args_in[3]);
    uint16_t color = (uint16_t)mp_obj_get_int(args_in[4]);

    if (w <= 0 || h <= 0) return mp_const_none;

    do_fill_rect_clip(x, y, w, 1, color);          /* top */
    do_fill_rect_clip(x, y + h - 1, w, 1, color);   /* bottom */
    do_fill_rect_clip(x, y, 1, h, color);           /* left */
    do_fill_rect_clip(x + w - 1, y, 1, h, color);   /* right */
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(moclcd_draw_rect_obj, 5, 5, moclcd_draw_rect);

/* -------------------------------------------------------------------
 * moclcd.draw_circle(x0, y0, r, color)
 * Midpoint circle algorithm, 8-way symmetry, pixel-by-pixel (each
 * pixel needs its own address window on this bus, same as draw_line's
 * diagonal case).
 * ---------------------------------------------------------------- */
static mp_obj_t moclcd_draw_circle(size_t n_args, const mp_obj_t *args_in)
{
    require_init();
    int x0 = mp_obj_get_int(args_in[0]);
    int y0 = mp_obj_get_int(args_in[1]);
    int r  = mp_obj_get_int(args_in[2]);
    uint16_t color = (uint16_t)mp_obj_get_int(args_in[3]);

    if (r < 0) return mp_const_none;

    int f = 1 - r;
    int ddF_x = 1;
    int ddF_y = -2 * r;
    int x = 0;
    int y = r;

    do_draw_pixel(x0, y0 + r, color);
    do_draw_pixel(x0, y0 - r, color);
    do_draw_pixel(x0 + r, y0, color);
    do_draw_pixel(x0 - r, y0, color);

    while (x < y) {
        if (f >= 0) { y--; ddF_y += 2; f += ddF_y; }
        x++;
        ddF_x += 2;
        f += ddF_x;

        do_draw_pixel(x0 + x, y0 + y, color);
        do_draw_pixel(x0 - x, y0 + y, color);
        do_draw_pixel(x0 + x, y0 - y, color);
        do_draw_pixel(x0 - x, y0 - y, color);
        do_draw_pixel(x0 + y, y0 + x, color);
        do_draw_pixel(x0 - y, y0 + x, color);
        do_draw_pixel(x0 + y, y0 - x, color);
        do_draw_pixel(x0 - y, y0 - x, color);
    }
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(moclcd_draw_circle_obj, 4, 4, moclcd_draw_circle);

/* -------------------------------------------------------------------
 * moclcd.mirror_enable()
 * Turns on shadow-framebuffer mirroring: every fill_rect/draw_pixel/
 * blit/draw_bmp/draw_text call from this point on also writes into an
 * internal width*height*2-byte RAM copy, so read_framebuffer() can
 * hand back "what's currently on screen" even though the panel itself
 * has no readback path. Costs ~300KB RAM at 480x320 -- call this once
 * near startup if you'll need frame capture at all; leave it off (the
 * default) otherwise.
 * ---------------------------------------------------------------- */
static mp_obj_t moclcd_mirror_enable(void)
{
    require_init();
    ensure_shadow_fb();
    s_shadow_enabled = true;
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_0(moclcd_mirror_enable_obj, moclcd_mirror_enable);

/* -------------------------------------------------------------------
 * moclcd.mirror_disable()
 * Stops mirroring (draw calls stop paying the RAM-copy cost). The
 * shadow buffer memory itself is kept around (not freed) so a later
 * mirror_enable() doesn't need to re-allocate; call mirror_free() if
 * you actually want the memory back.
 * ---------------------------------------------------------------- */
static mp_obj_t moclcd_mirror_disable(void)
{
    s_shadow_enabled = false;
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_0(moclcd_mirror_disable_obj, moclcd_mirror_disable);

/* -------------------------------------------------------------------
 * moclcd.mirror_free()
 * Frees the shadow framebuffer's memory and turns mirroring off.
 * ---------------------------------------------------------------- */
static mp_obj_t moclcd_mirror_free(void)
{
    s_shadow_enabled = false;
    if (s_shadow_fb) {
        heap_caps_free(s_shadow_fb);
        s_shadow_fb = NULL;
    }
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_0(moclcd_mirror_free_obj, moclcd_mirror_free);

/* -------------------------------------------------------------------
 * moclcd.read_framebuffer(dest, x=0, y=0, w=None, h=None)
 *
 * Copies the current shadow framebuffer contents (or a sub-rectangle
 * of it) into `dest`, forwarding it to the caller's own buffer/
 * variable instead of allocating a new one each call. `dest` must be
 * a pre-allocated writable buffer (e.g. bytearray) of at least
 * w*h*2 bytes -- RGB565, MSB first per pixel, same layout blit()
 * expects, so the result can be fed straight back into blit() (e.g.
 * to restore a region, or save a "screenshot" to a file).
 *
 * Requires mirror_enable() to have been called first (raises OSError
 * otherwise, since there's nothing to read back -- the panel itself
 * can't be read from). Returns the number of bytes written.
 * ---------------------------------------------------------------- */
static mp_obj_t moclcd_read_framebuffer(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args)
{
    require_init();

    enum { ARG_dest, ARG_x, ARG_y, ARG_w, ARG_h };
    static const mp_arg_t allowed[] = {
        { MP_QSTR_dest, MP_ARG_REQUIRED | MP_ARG_OBJ, {.u_rom_obj = MP_ROM_NONE} },
        { MP_QSTR_x,    MP_ARG_KW_ONLY  | MP_ARG_INT, {.u_int = 0} },
        { MP_QSTR_y,    MP_ARG_KW_ONLY  | MP_ARG_INT, {.u_int = 0} },
        { MP_QSTR_w,    MP_ARG_KW_ONLY  | MP_ARG_INT, {.u_int = 0} }, /* 0 = full width */
        { MP_QSTR_h,    MP_ARG_KW_ONLY  | MP_ARG_INT, {.u_int = 0} }, /* 0 = full height */
    };
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed), allowed, args);

    if (!s_shadow_enabled || s_shadow_fb == NULL) {
        mp_raise_msg(&mp_type_OSError, MP_ERROR_TEXT(
            "read_framebuffer: call mirror_enable() first -- the panel has no readback path, "
            "only the shadow copy can be read"));
    }

    int x = args[ARG_x].u_int;
    int y = args[ARG_y].u_int;
    int w = args[ARG_w].u_int > 0 ? args[ARG_w].u_int : s_width;
    int h = args[ARG_h].u_int > 0 ? args[ARG_h].u_int : s_height;

    if (x < 0 || y < 0 || w <= 0 || h <= 0 || x + w > s_width || y + h > s_height) {
        mp_raise_ValueError(MP_ERROR_TEXT("read_framebuffer region out of bounds"));
    }

    mp_buffer_info_t bufinfo;
    mp_get_buffer_raise(args[ARG_dest].u_obj, &bufinfo, MP_BUFFER_WRITE);

    size_t needed = (size_t)w * (size_t)h * 2;
    if (bufinfo.len < needed) {
        mp_raise_msg_varg(&mp_type_ValueError, MP_ERROR_TEXT(
            "dest buffer too small: need %d bytes, got %d"),
            (int)needed, (int)bufinfo.len);
    }

    uint8_t *out = (uint8_t *)bufinfo.buf;
    for (int row = 0; row < h; row++) {
        const uint16_t *src_row = &s_shadow_fb[(size_t)(y + row) * s_width + x];
        uint8_t *dst_row = out + (size_t)row * w * 2;
        for (int col = 0; col < w; col++) {
            uint16_t c = src_row[col];
            dst_row[col * 2]     = (uint8_t)(c >> 8);
            dst_row[col * 2 + 1] = (uint8_t)(c & 0xFF);
        }
    }

    return mp_obj_new_int((mp_int_t)needed);
}
static MP_DEFINE_CONST_FUN_OBJ_KW(moclcd_read_framebuffer_obj, 1, moclcd_read_framebuffer);

/* -------------------------------------------------------------------
 * moclcd.blit_fast(ops_x, ops_y, ops_w, ops_h, ops_color, count, mode)
 *
 * Batched rectangle stream for driving animations from C instead of
 * looping fill_rect() calls one at a time from Python. Each Python
 * fill_rect() call pays MicroPython call overhead (arg unpacking,
 * bounds validation, a full esp_lcd_panel_io round trip) even for
 * tiny 1px animation-frame slivers; an open/close "genie" animation
 * can issue hundreds of these per second. blit_fast() takes four
 * equal-length int arrays (already-computed x/y/w/h for a batch of
 * rects, e.g. one whole animation frame's worth) plus a matching color
 * array, and streams them all in a single C-side loop with one
 * mp_arg parse instead of one per rect -- the DMA pipelining
 * (trans_queue_depth) does the rest.
 *
 * Arrays are any object supporting the buffer protocol interpreted as
 * int32 (e.g. array.array('i', [...])); all five must be the same
 * length >= count.
 * ---------------------------------------------------------------- */
static mp_obj_t moclcd_blit_fast(size_t n_args, const mp_obj_t *args_in)
{
    require_init();

    mp_buffer_info_t bx, by, bw, bh, bc;
    mp_get_buffer_raise(args_in[0], &bx, MP_BUFFER_READ);
    mp_get_buffer_raise(args_in[1], &by, MP_BUFFER_READ);
    mp_get_buffer_raise(args_in[2], &bw, MP_BUFFER_READ);
    mp_get_buffer_raise(args_in[3], &bh, MP_BUFFER_READ);
    mp_get_buffer_raise(args_in[4], &bc, MP_BUFFER_READ);
    int count = mp_obj_get_int(args_in[5]);

    const int32_t *xs = (const int32_t *)bx.buf;
    const int32_t *ys = (const int32_t *)by.buf;
    const int32_t *ws = (const int32_t *)bw.buf;
    const int32_t *hs = (const int32_t *)bh.buf;
    const int32_t *cs = (const int32_t *)bc.buf;

    size_t need = (size_t)count * sizeof(int32_t);
    if (bx.len < need || by.len < need || bw.len < need || bh.len < need || bc.len < need) {
        mp_raise_ValueError(MP_ERROR_TEXT("blit_fast: arrays shorter than count"));
    }

    for (int i = 0; i < count; i++) {
        do_fill_rect_clip((int)xs[i], (int)ys[i], (int)ws[i], (int)hs[i], (uint16_t)cs[i]);
    }
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(moclcd_blit_fast_obj, 6, 6, moclcd_blit_fast);

/* -------------------------------------------------------------------
 * moclcd.fill_circle(x0, y0, r, color)
 * Midpoint circle algorithm filled via vertical spans (same approach
 * Adafruit_GFX uses) -- each span goes through the DMA fill path
 * instead of being plotted pixel by pixel, so a filled circle is much
 * cheaper than the same shape built out of draw_pixel() calls.
 * ---------------------------------------------------------------- */
static mp_obj_t moclcd_fill_circle(size_t n_args, const mp_obj_t *args_in)
{
    require_init();
    int x0 = mp_obj_get_int(args_in[0]);
    int y0 = mp_obj_get_int(args_in[1]);
    int r  = mp_obj_get_int(args_in[2]);
    uint16_t color = (uint16_t)mp_obj_get_int(args_in[3]);

    if (r < 0) return mp_const_none;

    do_fill_rect_clip(x0, y0 - r, 1, 2 * r + 1, color); /* central vertical span */

    int f = 1 - r;
    int ddF_x = 1;
    int ddF_y = -2 * r;
    int x = 0;
    int y = r;

    while (x < y) {
        if (f >= 0) { y--; ddF_y += 2; f += ddF_y; }
        x++;
        ddF_x += 2;
        f += ddF_x;

        do_fill_rect_clip(x0 + x, y0 - y, 1, 2 * y + 1, color);
        do_fill_rect_clip(x0 - x, y0 - y, 1, 2 * y + 1, color);
        do_fill_rect_clip(x0 + y, y0 - x, 1, 2 * x + 1, color);
        do_fill_rect_clip(x0 - y, y0 - x, 1, 2 * x + 1, color);
    }
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(moclcd_fill_circle_obj, 4, 4, moclcd_fill_circle);

/* ---- module table ---- */
static const mp_rom_map_elem_t moclcd_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__),   MP_ROM_QSTR(MP_QSTR_moclcd)          },
    { MP_ROM_QSTR(MP_QSTR_init),        MP_ROM_PTR(&moclcd_init_obj)        },
    { MP_ROM_QSTR(MP_QSTR_reset),       MP_ROM_PTR(&moclcd_reset_obj)       },
    { MP_ROM_QSTR(MP_QSTR_panel_init),  MP_ROM_PTR(&moclcd_panel_init_obj)  },
    { MP_ROM_QSTR(MP_QSTR_backlight),   MP_ROM_PTR(&moclcd_backlight_obj)   },
    { MP_ROM_QSTR(MP_QSTR_backlight_init), MP_ROM_PTR(&moclcd_backlight_init_obj) },
    { MP_ROM_QSTR(MP_QSTR_backlight_set),  MP_ROM_PTR(&moclcd_backlight_set_obj)  },
    { MP_ROM_QSTR(MP_QSTR_cmd),         MP_ROM_PTR(&moclcd_cmd_obj)         },
    { MP_ROM_QSTR(MP_QSTR_data),        MP_ROM_PTR(&moclcd_data_obj)        },
    { MP_ROM_QSTR(MP_QSTR_fill_rect),   MP_ROM_PTR(&moclcd_fill_rect_obj)   },
    { MP_ROM_QSTR(MP_QSTR_fill_screen), MP_ROM_PTR(&moclcd_fill_screen_obj) },
    { MP_ROM_QSTR(MP_QSTR_blit),        MP_ROM_PTR(&moclcd_blit_obj)        },
    { MP_ROM_QSTR(MP_QSTR_draw_pixel),  MP_ROM_PTR(&moclcd_draw_pixel_obj)  },
    { MP_ROM_QSTR(MP_QSTR_draw_line),   MP_ROM_PTR(&moclcd_draw_line_obj)   },
    { MP_ROM_QSTR(MP_QSTR_draw_rect),   MP_ROM_PTR(&moclcd_draw_rect_obj)   },
    { MP_ROM_QSTR(MP_QSTR_draw_circle), MP_ROM_PTR(&moclcd_draw_circle_obj) },
    { MP_ROM_QSTR(MP_QSTR_fill_circle), MP_ROM_PTR(&moclcd_fill_circle_obj) },
    { MP_ROM_QSTR(MP_QSTR_draw_text8x8),MP_ROM_PTR(&moclcd_draw_text8x8_obj)},   // ADD THIS
    { MP_ROM_QSTR(MP_QSTR_draw_bmp),    MP_ROM_PTR(&moclcd_draw_bmp_obj)    },   // ADD THIS

    /* multi-font support */
    { MP_ROM_QSTR(MP_QSTR_draw_text),        MP_ROM_PTR(&moclcd_draw_text_obj)        },
    { MP_ROM_QSTR(MP_QSTR_register_font),    MP_ROM_PTR(&moclcd_register_font_obj)    },
    { MP_ROM_QSTR(MP_QSTR_unregister_font),  MP_ROM_PTR(&moclcd_unregister_font_obj)  },
    { MP_ROM_QSTR(MP_QSTR_font_metrics),     MP_ROM_PTR(&moclcd_font_metrics_obj)     },

    /* shadow framebuffer / readback */
    { MP_ROM_QSTR(MP_QSTR_mirror_enable),    MP_ROM_PTR(&moclcd_mirror_enable_obj)    },
    { MP_ROM_QSTR(MP_QSTR_mirror_disable),   MP_ROM_PTR(&moclcd_mirror_disable_obj)   },
    { MP_ROM_QSTR(MP_QSTR_mirror_free),      MP_ROM_PTR(&moclcd_mirror_free_obj)      },
    { MP_ROM_QSTR(MP_QSTR_read_framebuffer), MP_ROM_PTR(&moclcd_read_framebuffer_obj) },

    /* fast batched animation path */
    { MP_ROM_QSTR(MP_QSTR_blit_fast),        MP_ROM_PTR(&moclcd_blit_fast_obj)        },
};
static MP_DEFINE_CONST_DICT(moclcd_globals, moclcd_globals_table);

const mp_obj_module_t mp_module_moclcd = {
    .base    = { &mp_type_module },
    .globals = (mp_obj_dict_t *)&moclcd_globals,
};

MP_REGISTER_MODULE(MP_QSTR_moclcd, mp_module_moclcd);
