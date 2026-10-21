# zeno_img -- PNG / JPEG / BMP decoding for the zeno MicroPython build

Native C user module. Gives you four modules: `png`, `jpeg` (also importable as `jpg`),
`bmp`, and `img` (detects the format from the file's first bytes).

```python
import png, lcd
p = png.open("/sd/logo.png")
lcd.blit(10, 20, p.width, p.height, p.buf)      # p.buf is already in lcd.blit's format
```

`p.buf` is a `bytearray`, **RGB565 big-endian, exactly `width*height*2` bytes** (what
`lcd.blit` requires). The Image object also supports the buffer protocol, so
`lcd.blit(x, y, p.width, p.height, p)` works too.

## API (identical in png / jpeg / bmp / img)

| call | result |
|---|---|
| `open(src, *, fit=None, div=1, bg=0x000000)` | `Image` |
| `decode(...)` | same as `open` |
| `info(src)` | `(width, height)` reading only the header |

* `src`: a path string, an open binary file (anything with `.read`), or bytes / bytearray / memoryview.
* `fit=(w, h)`: shrink by the smallest integer divider that makes the image fit inside w x h
  (never enlarges). `div=n`: force a minimum divider. Typical: `fit=(lcd.width(), lcd.height())`.
  PNG/BMP: any divider 1..255, box-filtered (smooth). JPEG: 1, 2, 4 or 8 (native TJpgDec scaling,
  also the fastest way to decode a big photo); beyond 8x raises `ValueError`.
* `bg`: `0xRRGGBB` colour that transparent PNG pixels are blended onto (RGB888, not RGB565).
* `Image` attributes: `.width .height .buf .size .src_size .div`.

Errors: `MemoryError` (not enough RAM), `EOFError` (truncated file), `NotImplementedError`
(valid but unsupported variant), `ValueError` (not an image / corrupt), `OSError` (file error).

## Supported

* **PNG**: gray, RGB, palette, gray+alpha, RGBA; depth 1/2/4/8/16; tRNS; Adam7 interlaced.
  Streams from the file: RAM = 32 KB inflate window + ~11 KB state + one scanline + the result.
  Interlaced + shrink uses nearest-neighbour (pixels arrive out of order); everything else box-filters.
* **BMP**: 1/4/8-bit palette, 16-bit (555 and bitfields), 24, 32; bottom-up and top-down; headers
  12/40/52/56/108/124. Not supported: RLE4/RLE8, embedded JPEG/PNG. 32-bit alpha is ignored.
* **JPEG**: baseline (sequential) only, via the TJpgDec decoder **in the ESP32-S3 ROM** (no flash cost).
  Progressive JPEG raises `NotImplementedError`; re-save it as baseline.

## Build

Copy the folder next to `zeno_lcd` and add its cmake file to the list:

```
idf.py -D MICROPY_BOARD=<board> \
       -D USER_C_MODULES="/abs/zeno_lcd/micropython.cmake;/abs/zeno_img/micropython.cmake" \
       build flash
```
Needs MicroPython >= 1.22 (new-style `MP_DEFINE_CONST_OBJ_TYPE`) and ESP-IDF headers `rom/tjpgd.h`,
`rom/miniz.h` (both in `esp_rom`, present in every IDF build). If your chip had no ROM TJpgDec, put
`tjpgd.c/.h` in this folder and compile with `-DZIMG_JPEG_EXTERNAL_TJPGD` (S3 does not need it).

## RAM

The result is allocated from the MicroPython heap: a full 480x320 image is 300 KB, so a PSRAM
build is needed for full-screen images; on a no-PSRAM heap use `fit=` / `div=`. Temporary buffers
are freed before `open()` returns.

## What was and was not verified

Verified on a desktop (`tests/host/build.sh`, built with AddressSanitizer + UBSan, 527 checks): PNG
of every colour type / bit depth / filter / interlace / tRNS / multi-chunk IDAT, box-filter and
`fit` maths, BMP of every supported layout, truncated + corrupt + fuzzed inputs, and the real
PNG/BMP files from your repo (bit-exact against Pillow).

**Not verified (no hardware or toolchain here):** compiling against your real MicroPython/ESP-IDF
tree (`modzimg.c` was only syntax-checked against stub headers), the ROM tinfl and ROM TJpgDec
calls (host tests use zlib and a mock TJpgDec that exercises the callbacks, scaling, clipping and
repacking but does not decode real JPEG data), and speed/RAM on the device. Run `py/img_demo.py`
with one file of each type first.
