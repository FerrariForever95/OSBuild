---
name: esp32-psram-memory-settings
description: Recommendations for increasing usable memory and performance on ESP32-S3 with 8MB PSRAM
metadata: 
  node_type: memory
  type: reference
  originSessionId: 9b852a51-7cc8-4f32-a4cd-dad3f3527297
  modified: 2026-09-01T05:44:19.548Z
---

To get more memory for your MicroPython programs on ESP32-S3 with 8MB PSRAM:

1. Increase initial Python heap size in `mpconfigport.h`:
   - Raise `MICROPY_GC_INITIAL_HEAP_SIZE` (e.g., to 1MB).

2. Adjust PSRAM malloc preference to favor PSRAM for larger allocations:
   - Lower `CONFIG_SPIRAM_MALLOC_ALWAYSINTERNAL` in board sdkconfig (e.g., to 4096 or 0).
   - Keep `CONFIG_SPIRAM_MALLOC_RESERVE_INTERNAL` at a reasonable value (16KB–32KB).

3. Performance settings are already optimized (`CONFIG_COMPILER_OPTIMIZATION_PERF=y`, assertions disabled).

4. Rebuild after changes: `make clean && make`.

Monitor memory via `esp32.heap_info()` or the `idf_heap_info()` function.

Related: [[esp32-build-config]], [[micropython-heap-split]]