#!/bin/sh
# Build + run the host tests (needs gcc, zlib headers, python3 with numpy + Pillow).
set -e
cd "$(dirname "$0")/../.."
gcc -std=c11 -Wall -Wextra -O1 -g -fsanitize=address,undefined -DZIMG_HOST -I. -Itests/host \
    zimg_sink.c zimg_png.c zimg_bmp.c zimg_jpeg.c \
    tests/host/mock_tjpgd.c tests/host/inflate_zlib.c tests/host/test_main.c \
    -o tests/host/test_zimg -lz
python3 tests/host/run_tests.py tests/host/test_zimg
