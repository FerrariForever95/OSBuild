#!/usr/bin/env bash
# Build the SPIFFS image for the "assets" partition and flash it.
# usage: tools/make_assets.sh /dev/ttyACM0     (needs IDF_PATH set and esptool installed)
set -euo pipefail
PORT="${1:?serial port}"
HERE="$(cd "$(dirname "$0")/.." && pwd)"
STAGE="$(mktemp -d)"
cp "$HERE"/fonts/*.FNT "$STAGE"/
[ -d "$HERE/assets_extra" ] && cp "$HERE"/assets_extra/* "$STAGE"/ || true   # put your PNG/JPEG files here
python "$IDF_PATH/components/spiffs/spiffsgen.py" 0x200000 "$STAGE" "$HERE/assets.bin"
python -m esptool --chip esp32s3 -p "$PORT" write_flash 0x310000 "$HERE/assets.bin"
