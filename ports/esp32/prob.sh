#!/bin/bash

OUT="/home/shanmukh/problem"
mkdir -p "$OUT"

for DIR in "working build" "fked up build"; do
    echo "Processing: $DIR"

    TMP="$DIR/firmware_compare"
    rm -rf "$TMP"
    mkdir -p "$TMP"

    # Root configuration
    cp "$DIR/sdkconfig" "$TMP/" 2>/dev/null
    cp "$DIR/sdkconfig.board" "$TMP/" 2>/dev/null
    cp "$DIR/CMakeLists.txt" "$TMP/" 2>/dev/null
    cp "$DIR/Makefile" "$TMP/" 2>/dev/null
    cp "$DIR/mpconfigport.h" "$TMP/" 2>/dev/null

    # Partition tables
    find "$DIR" -maxdepth 1 -name "partitions*.csv" -exec cp {} "$TMP/" \; 2>/dev/null

    # Board definitions
    if [ -d "$DIR/boards" ]; then
        cp -r "$DIR/boards" "$TMP/"
    fi

    # Build configuration
    cp "$DIR/build/CMakeCache.txt" "$TMP/" 2>/dev/null
    cp "$DIR/build/project_description.json" "$TMP/" 2>/dev/null
    cp "$DIR/build/flasher_args.json" "$TMP/" 2>/dev/null
    cp "$DIR/build/flash_args" "$TMP/" 2>/dev/null

    # Bootloader
    mkdir -p "$TMP/bootloader"
    cp "$DIR/build/bootloader/"{bootloader.bin,bootloader.elf,bootloader.map,sdkconfig} "$TMP/bootloader/" 2>/dev/null

    # Partition table
    mkdir -p "$TMP/partition_table"
    cp "$DIR/build/partition_table/"* "$TMP/partition_table/" 2>/dev/null

    # Application
    cp "$DIR/build/"*.bin "$TMP/" 2>/dev/null
    cp "$DIR/build/"*.elf "$TMP/" 2>/dev/null
    cp "$DIR/build/"*.map "$TMP/" 2>/dev/null

    # sdkconfig header
    mkdir -p "$TMP/config"
    cp "$DIR/build/config/sdkconfig.h" "$TMP/config/" 2>/dev/null

    # Zip it
    ZIPNAME="$(echo "$DIR" | tr ' ' '_').zip"
    (
        cd "$DIR"
        zip -qr "$OUT/$ZIPNAME" firmware_compare
    )

    rm -rf "$TMP"

    echo "Created: $OUT/$ZIPNAME"
done

echo
echo "Done!"
echo "Output:"
echo "  /home/shanmukh/problem/working_build.zip"
echo "  /home/shanmukh/problem/fked_up_build.zip"
