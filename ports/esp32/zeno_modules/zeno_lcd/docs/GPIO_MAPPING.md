# GPIO / configuration mapping

Single source of truth: `zeno_lcd_config.h`. This table is a human-readable
view of it; if the two ever disagree, the header wins.

## LCD bus (`CONFIG_CUSTOM_GPIO_CONFIG=y`, `CONFIG_INTERFACE_REG=y`)

| Signal | GPIO | sdkconfig symbol | Notes |
|---|---|---|---|
| D0 | 8  | `CONFIG_D0_GPIO` | shared with touch XP |
| D1 | 15 | `CONFIG_D1_GPIO` | shared with touch YM |
| D2 | 11 | `CONFIG_D2_GPIO` | |
| D3 | 10 | `CONFIG_D3_GPIO` | |
| D4 | 9  | `CONFIG_D4_GPIO` | |
| D5 | 4  | `CONFIG_D5_GPIO` | |
| D6 | 18 | `CONFIG_D6_GPIO` | |
| D7 | 17 | `CONFIG_D7_GPIO` | |
| RD | 41 | `CONFIG_RD_GPIO` | held HIGH; this driver is write-only, like the original |
| WR | 14 | `CONFIG_WR_GPIO` | |
| RS/DC | 2 | `CONFIG_RS_GPIO` | shared with touch XM |
| CS | 1 | `CONFIG_CS_GPIO` | shared with touch YP |
| RESET | 12 | `CONFIG_RESET_GPIO` | |
| Backlight | 38 | *(not in sdkconfig)* | **not verified** -- carried over from the pre-existing `moclcd` module's header comment only; confirm or correct before relying on `lcd.display()`-style brightness control. The driver never drives this pin; it exists only as a placeholder define in case Zeno OS wants it later. |

All eight data pins plus WR, RS, CS must be GPIO < 32 (bank 0) for the
fast `GPIO_OUT_W1TS_REG`/`GPIO_OUT_W1TC_REG` path in `lcd_bus.c` to work;
`_Static_assert` in that file enforces this at compile time. Every pin
above is < 32, so this holds for the supplied hardware as-is.

## Panel geometry

| Item | Value | sdkconfig symbol |
|---|---|---|
| Native width | 320 | `CONFIG_WIDTH` |
| Native height | 480 | `CONFIG_HEIGHT` |
| X offset | 0 | `CONFIG_OFFSETX` |
| Y offset | 0 | `CONFIG_OFFSETY` |
| Color inversion at init | off | `# CONFIG_INVERSION is not set` |
| Rotation 0 MADCTL | `0x98` | hard-coded in `ili9488.c`'s `ili9488_regValues[]`, **verified** |
| Rotations 1-3 MADCTL | `0x38` / `0x58` / `0xF8` | **derived, not hardware-verified** -- see `zl_madctl_table[]` in `zeno_pure.h` and Stage A of `VALIDATION.md` |

## Touch electrodes (`CONFIG_ENABLE_TOUCH=y`)

| Electrode | GPIO | Shares LCD signal | sdkconfig symbol |
|---|---|---|---|
| XP | 8  | D0 | `CONFIG_XP_GPIO_D0=y` |
| XM | 2  | RS | `CONFIG_XM_GPIO_RS=y` |
| YP | 1  | CS | `CONFIG_YP_GPIO_CS=y` |
| YM | 15 | D1 | `CONFIG_YM_GPIO_D1=y` |

| ADC channel | GPIO it reads | Electrode | sdkconfig symbol |
|---|---|---|---|
| ADC1 channel 0 | 1 (= CS) | YP | `CONFIG_ADC_CHANNEL_YP=0` |
| ADC1 channel 1 | 2 (= RS) | XM | `CONFIG_ADC_CHANNEL_XM=1` |

See `docs/ASSESSMENT.md` section 5 for why only YP and XM have ADC
channels (XP and YM are digital-drive-only in this algorithm), and for the
one place this disagrees with the task brief's own prose pin list
(GPIO8/D0/XP is not actually sampled by any configured ADC channel).

## Acquisition constants (`4-line_resistance_touch_screen.txt` / `lcd_com.h`)

| Constant | Value | Meaning |
|---|---|---|
| `NUM_SAMPLES` | 2 | independent dual-read per axis, must agree within tolerance |
| `COMP_TOLERANCE` | 2 | max ADC-count disagreement between the two reads |
| `AVERAGE_TIME` | 4 | raw samples per read; min+max dropped, remaining 2 averaged |

## Values this port added that are NOT in the sdkconfig

These have no original-hardware value to carry over and need confirming
on real hardware (Stage A/B of `VALIDATION.md`):

| Item | Default | Why |
|---|---|---|
| `ZLCD_WR_LOW_NOPS_DEFAULT` / `..._HIGH_NOPS_DEFAULT` | 6 / 2 | the original had no explicit WR hold; its timing margin came from `-Og` call overhead around `gpio_set_level()` (see `ASSESSMENT.md` section 10) |
| `ZTOUCH_DEFAULT_MIN_RAW` | 100 | floor below which a reading is treated as "nothing pressed"; the original had no such floor (only the dual-sample-agreement check) |
| `ZTOUCH_DEFAULT_Z_MAX` | 4095 (disabled) | replaces the original's non-functional `z==1023` 10-bit-era check, see `ASSESSMENT.md` |
| `ZTOUCH_DEFAULT_SETTLE_US` | 20 | the original had no settle delay between switching electrodes and sampling |
| `ZTOUCH_DEFAULT_DOWN_CONFIRM` / `..._UP_CONFIRM` | 2 / 3 | debouncing; the original had none |
