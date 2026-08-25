# OSBuild ESP32 Port - Complete Reference Guide

This file provides a complete map of the OSBuild ESP32 port structure and explains what exists where and what to modify to achieve expected results such as banner text in REPL, system default settings, memory management, and other configurations.

## Table of Contents
1. [Project Structure Overview](#project-structure-overview)
2. [Banner Text Configuration](#banner-text-configuration)
3. [System Default Settings](#system-default-settings)
4. [Memory Management](#memory-management)
5. [Board Configuration](#board-configuration)
6. [Build System](#build-system)
7. [Key Files and Their Purposes](#key-files-and-their-purposes)
8. [How to Modify Specific Features](#how-to-modify-specific-features)

---

## Project Structure Overview

The ESP32 port is located at `/home/shanmukh/OSBuild/ports/esp32/` and contains the following key directories and files:

```
ports/esp32/
├── main.c                 - Entry point for the application
├── main.h                 - Main header (if exists)
├── mpconfigport.h         - Port-specific configuration overrides
├── boards/                - Board-specific configurations
├── build/                 - Build output directories
├── modules/               - Additional modules
├── managed_components/    - Managed IDF components
├── drivers/               - Hardware driver implementations
├── help.c                 - Help text shown when typing help()
├── README.md              - Port documentation
└── ...                    - Various source files for peripherals, networking, etc.
```

## Banner Text Configuration

The welcome banner that appears when starting the REPL is configured through several layers:

### Where the Banner is Printed
The banner is printed in `/home/shanmukh/OSBuild/shared/runtime/pyexec.c` in the `pyexec_friendly_repl()` function (lines 624-628):

```c
mp_hal_stdout_tx_str(MICROPY_BANNER_NAME_AND_VERSION);
mp_hal_stdout_tx_str("; " MICROPY_BANNER_MACHINE);
mp_hal_stdout_tx_str("\r\n");
#if MICROPY_PY_BUILTINS_HELP
mp_hal_stdout_tx_str("Type \"help()\" for more information.\r\n");
#endif
```

### Banner Components

1. **MICROPY_BANNER_NAME_AND_VERSION** - Defined in `/home/shanmukh/OSBuild/py/mpconfig.h` (lines 2266-2272):
   ```c
   #ifndef MICROPY_BANNER_NAME_AND_VERSION
   #if MICROPY_PREVIEW_VERSION_2
   #define MICROPY_BANNER_NAME_AND_VERSION "MicroPython (with v2.0 preview) " MICROPY_GIT_TAG " on " MICROPY_BUILD_DATE
   #else
   #define MICROPY_BANNER_NAME_AND_VERSION "MicroPython " MICROPY_GIT_TAG " on " MICROPY_BUILD_DATE
   #endif
   #endif
   ```

2. **MICROPY_BANNER_MACHINE** - Also defined in `/home/shanmukh/OSBuild/py/mpconfig.h` (lines 2274-2281):
   ```c
   #ifndef MICROPY_BANNER_MACHINE
   #ifdef MICROPY_HW_BOARD_NAME
   #define MICROPY_BANNER_MACHINE MICROPY_HW_BOARD_NAME " with " MICROPY_HW_MCU_NAME
   #else
   #define MICROPY_BANNER_MACHINE MICROPY_PY_SYS_PLATFORM " [" MICROPY_PLATFORM_COMPILER "] version"
   #endif
   #endif
   ```

### Current Values (from build)
Based on the latest build in `/home/shanmukh/OSBuild/ports/esp32/build-ESP32_GENERIC_S3/genhdr/mpversion.h`:
- `MICROPY_GIT_TAG` = "4987e76f9f-dirty"
- `MICROPY_BUILD_DATE` = "2026-08-22"

From board configuration in `/home/shanmukh/OSBuild/ports/esp32/boards/ESP32_GENERIC/mpconfigboard.h`:
- `MICROPY_HW_BOARD_NAME` = "Generic ESP32 module" (default)
- `MICROPY_HW_MCU_NAME` = "ESP32" (default)

**Resulting Banner:**
```
MicroPython 4987e76f9f-dirty on 2026-08-22
Generic ESP32 module with ESP32
Type "help()" for more information.
```

### How to Modify the Banner
To change the banner text, you can modify:

1. **Board-specific name**: Edit `/home/shanmukh/OSBuild/ports/esp32/boards/ESP32_GENERIC/mpconfigboard.h`
   ```c
   #define MICROPY_HW_BOARD_NAME "Your Custom Board Name"
   #define MICROPY_HW_MCU_NAME "ESP32-S3"  // or appropriate MCU
   ```

2. **Build date/GIT tag**: These are automatically generated during build. To override:
   - Set environment variables before building: `MICROPY_GIT_TAG="custom-tag" MICROPY_BUILD_DATE="2026-01-01" make`
   - Or modify the generation script in `py/makeversionhdr.py`

3. **Disable help hint**: Set `MICROPY_PY_BUILTINS_HELP=0` in `mpconfigport.h`

4. **Complete banner override**: Define `MICROPY_BANNER_NAME_AND_VERSION` and `MICROPY_BANNER_MACHINE` in `mpconfigport.h` to override the defaults.

## System Default Settings

System default settings are primarily configured in `/home/shanmukh/OSBuild/ports/esp32/mpconfigport.h`. This file overrides defaults from `/home/shanmukh/OSBuild/py/mpconfig.h`.

### Key System Settings in mpconfigport.h

1. **Object Representation** (lines 18-22):
   ```c
   #define MICROPY_OBJ_REPR (MICROPY_OBJ_REPR_A)
   #if CONFIG_IDF_TARGET_ARCH_XTENSA
   #define MICROPY_NLR_SETJMP (1)
   #endif
   ```

2. **Memory Allocation Policies** (lines 24-38):
   - Initial heap size varies by chip type:
     - ESP32: 56KB
     - ESP32-C2/ESP32-S2 without SPIRAM: 36KB
     - Others: 64KB

3. **Feature Enables** (lines 41-112):
   - Persistent code load: Enabled
   - Bluetooth: Enabled by default
   - Threading: Enabled
   - GC features: Enabled
   - Network modules: Enabled
   - And many others...

4. **Board-specific Overrides** (line 5):
   ```c
   #include "mpconfigboard.h"  // Board-specific settings come first
   ```

### Board Configuration Files
Board-specific settings are found in:
- `/home/shanmukh/OSBuild/ports/esp32/boards/ESP32_GENERIC/mpconfigboard.h`
- `/home/shanmukh/OSBuild/ports/esp32/boards/ESP32_GENERIC/mpconfigboard.cmake`

Example from ESP32_GENERIC/mpconfigboard.h:
```c
#define MICROPY_HW_BOARD_NAME "Generic ESP32 module"
#define MICROPY_HW_MCU_NAME "ESP32"
```

## Memory Management

Memory management configuration is found in several places:

### Heap Size Configuration
Located in `/home/shanmukh/OSBuild/ports/esp32/mpconfigport.h` lines 26-38:
```c
// Initial Python heap size. This starts small but adds new heap areas on demand due to
// the settings MICROPY_GC_SPLIT_HEAP and MICROPY_GC_SPLIT_HEAP_AUTO.
#ifndef MICROPY_GC_INITIAL_HEAP_SIZE
#if CONFIG_IDF_TARGET_ESP32
#define MICROPY_GC_INITIAL_HEAP_SIZE        (56 * 1024)
#elif CONFIG_IDF_TARGET_ESP32C2 || (CONFIG_IDF_TARGET_ESP32S2 && !CONFIG_SPIRAM)
#define MICROPY_GC_INITIAL_HEAP_SIZE        (36 * 1024)
#else
#define MICROPY_GC_INITIAL_HEAP_SIZE        (64 * 1024)
#endif
#endif
```

### Garbage Collection Settings
Also in `mpconfigport.h`:
- `MICROPY_ENABLE_GC` (line 62) - Enables garbage collection
- `MICROPY_GC_SPLIT_HEAP` (line 89) - Allows heap to be split over multiple memory areas
- `MICROPY_GC_SPLIT_HEAP_AUTO` (line 90) - Automatically manages split heap
- `MICROPY_STACK_CHECK_MARGIN` (line 63) - Stack checking margin (1024 bytes)
- `MICROPY_ENABLE_EMERGENCY_EXCEPTION_BUF` (line 64) - Emergency exception buffer

### SMSPIRAM Support
SPIRAM (external RAM) support is configured through:
- Board-specific `sdkconfig` files in `/home/shanmukh/OSBuild/ports/esp32/boards/`
- Examples: `sdkconfig.spiram`, `sdkconfig.spiram_oct`

To enable SPIRAM:
1. Ensure your board has external SPIRAM
2. Use a board variant that enables SPIRAM (e.g., `BOARD_VARIANT=SPIRAM`)
3. The heap configuration will automatically adjust for chips with SPIRAM

## Board Configuration

The ESP32 port supports many different boards through the `boards/` directory.

### Board Directory Structure
Each board has its own directory under `/home/shanmukh/OSBuild/ports/esp32/boards/` containing:
- `mpconfigboard.h` - MicroPython-specific settings
- `mpconfigboard.cmake` - CMake/IDF configuration
- `sdkconfig.*` - ESP-IDF configuration files
- `deploy.md`, `deploy_nativeusb.md` - Deployment instructions
- Optional: Custom `main.c`, `boot.py`, etc.

### Example: ESP32_GENERIC Board
Located at `/home/shanmukh/OSBuild/ports/esp32/boards/ESP32_GENERIC/`:
- `mpconfigboard.h`:
  ```c
  #define MICROPY_HW_BOARD_NAME "Generic ESP32 module"
  #define MICROPY_HW_MCU_NAME "ESP32"
  ```
- `mpconfigboard.cmake`:
  ```cmake
  include(boards/mpconfigboard_esp32_common.cmake)
  ```
- References common configuration in `boards/mpconfigboard_esp32_common.cmake`:
  ```cmake
  set(IDF_TARGET esp32)
  set(SDKCONFIG_DEFAULTS
      boards/sdkconfig.base
      boards/sdkconfig.ble
  )
  ```

### How to Add a New Board
1. Copy an existing board directory (e.g., `ESP32_GENERIC`)
2. Rename it to your board name
3. Modify `mpconfigboard.h` for your board's specifics
4. Adjust `mpconfigboard.cmake` if needed
5. Add any necessary `sdkconfig.*` files for your board's hardware
6. Update deployment instructions if needed

## Build System

The build system uses the ESP-IDF framework with MicroPython-specific wrappers.

### Key Build Files
- `Makefile` - Main makefile (thin wrapper around idf.py)
- `CMakeLists.txt` - CMake configuration
- `makeimg.py` - Tool to create final firmware image
- `modules/` - Additional components to include

### Build Process
1. Build mpy-cross: `make -C mpy-cross`
2. Build MicroPython: `make submodules` then `make`
3. This creates firmware in `build-BOARD_NAME/` directory
4. Flash with: `make deploy` (or `idf.py flash`)

### Build Targets
- `make` or `make build` - Build firmware
- `make clean` - Clean build objects
- `make erase` - Erase flash before flashing
- `make deploy` - Build and flash firmware
- `make idfmenuconfig` - Open ESP-IDF configuration menu

### Board Selection
Build for different boards using:
```bash
make BOARD=ESP32_GENERIC_S3
make BOARD=ESP32_GENERIC_S3 BOARD_VARIANT=SPIRAM_OCT
```

## Key Files and Their Purposes

### Core Initialization
- `main.c` - Contains `MICROPY_ESP_IDF_ENTRY()` and `mp_task()` - the main MicroPython task
- `mphalport.c`, `mphalport.h` - MicroPython Hardware Abstraction Layer port

### Peripheral Drivers
Located in the main port directory:
- `machine_*.c` - Machine module implementations (ADC, DAC, I2C, I2S, Pin, PWM, RTC, SDCard, Timer, TouchPad, UART, WDT)
- `modnetwork.c` - Network module
- `modesp32.c` - ESP32-specific module
- `usb.c`, `usb_serial_jtag.c` - USB functionality
- `esp32_*.c` - ESP32-specific peripherals (LDo, NVS, partition, PCNT, RMT, ULP)

### Filesystem
- `fatfs_port.c` - FAT filesystem port
- Network filesystem components in `network_*.c`

### Configuration
- `mpconfigport.h` - Main port configuration (overrides py/mpconfig.h defaults)
- `qstrdefsport.h` - Port-specific quoted strings
- `boards/` - Board-specific configurations

### Help and Documentation
- `help.c` - Text displayed when user types `help()` in REPL
- `README.md` - Port documentation
- `Changes.md` - Change log

## How to Modify Specific Features

### 1. Changing the REPL Banner Text
As described above, modify:
- Board name: `boards/BOARD_NAME/mpconfigboard.h` -> `MICROPY_HW_BOARD_NAME`
- MCU name: Same file -> `MICROPY_HW_MCU_NAME`
- Or override completely in `mpconfigport.h`:
  ```c
  #undef MICROPY_BANNER_NAME_AND_VERSION
  #define MICROPY_BANNER_NAME_AND_VERSION "My Custom Banner "
  
  #undef MICROPY_BANNER_MACHINE  
  #define MICROPY_BANNER_MACHINE "My Custom Machine"
  ```

### 2. Adjusting Memory Heap Size
Edit `/home/shanmukh/OSBuild/ports/esp32/mpconfigport.h`:
```c
#ifndef MICROPY_GC_INITIAL_HEAP_SIZE
#define MICROPY_GC_INITIAL_HEAP_SIZE    (128 * 1024)  // 128KB instead of default
#endif
```

### 3. Enabling/Disabling Features
In `mpconfigport.h`, find the relevant `#define` and change 0 to 1 or vice versa:
- To disable Bluetooth: `#undef MICROPY_PY_BLUETOOTH` or `#define MICROPY_PY_BLUETOOTH 0`
- To enable additional features: Look for the corresponding `#ifndef` and define it

### 4. Changing CPU Frequency
This is typically done through ESP-IDF sdkconfig:
```bash
make idfmenuconfig
```
Then navigate to:
- Component config → ESP32-specific → CPU frequency

Or override in board's `sdkconfig.*` file:
```c
CONFIG_ESP32_DEFAULT_CPU_FREQ_MHZ=240
```

### 5. Adding Custom Modules
1. Place your module `.c` and `.h` files in the port directory or a subdirectory
2. Add them to the build in `CMakeLists.txt` or `Makefile`
3. Export the module in `modesp32.c` or create a new mod*.c file
4. Add initialization in `main.c` or `mp_task()` if needed

### 6. Modifying Pin Definitions
Pin definitions are handled by:
- `pins_*.c` files generated by `make-pins.py`
- Board-specific pin configuration in `boards/BOARD_NAME/`
- To change pin mappings, modify the board's CSV pin definition and regenerate

### 7. Adjusting Task Stack Size
In `mpconfigport.h`:
```c
#ifndef MICROPY_TASK_STACK_SIZE
#define MICROPY_TASK_STACK_SIZE    (32 * 1024)  // 32KB instead of 16KB default
#endif
```

### 8. Enabling Debug Features
Various debug flags in `mpconfigport.h`:
- `MICROPY_DEBUG_PRINTERS` - Enable debug printers
- `MICROPY_MEM_STATS` - Enable memory allocation stats
- `MICROPY_REPL_INFO` - Show REPL timing and diagnostics
- `MICROPY_ENABLE_GC` - Already enabled, but controls GC availability

## ESP-IDF Configuration

The ESP-IDF configuration is managed through:
1. Board-specific `sdkconfig.*` files in `/home/shanmukh/OSBuild/ports/esp32/boards/`
2. The `sdkconfig.base` and `sdkconfig.ble` files included by default
3. Overrides in board's `mpconfigboard.cmake` via `set(SDKCONFIG_DEFAULTS ...)`

To modify ESP-IDF settings:
1. Edit the appropriate `sdkconfig.*` file for your board
2. Or run `make idfmenuconfig` to configure interactively
3. Common settings to adjust:
   - Partition table
   - CPU frequency
   - Flash mode/size
   - PSRAM configuration
   - WiFi/Bluetooth settings
   - Console/UART settings
   - Log level

## Summary

To get expected results like custom banner text, memory settings, or system defaults:

1. **For banner changes**: Modify board config in `boards/BOARD_NAME/mpconfigboard.h` or override macros in `mpconfigport.h`
2. **For memory/heap adjustments**: Edit `MICROPY_GC_INITIAL_HEAP_SIZE` in `mpconfigport.h`
3. **For feature enables/disables**: Modify the corresponding `#define` in `mpconfigport.h`
4. **For board-specific hardware**: Create/modify board directory in `boards/`
5. **For ESP-IDF low-level settings**: Use `idfmenuconfig` or edit board's `sdkconfig.*` files
6. **For peripheral drivers**: Look in the main port directory for `machine_*.c` and `mod*.c` files
7. **For help text**: Edit `/home/shanmukh/OSBuild/ports/esp32/help.c`

This structure allows for extensive customization while maintaining compatibility with the ESP-IDF framework and MicroPython core.