#!/usr/bin/env bash
# =============================================================================
# ZENO OS BUILD MANAGER
# MicroPython ESP32 Makefile-based automated configuration manager
# =============================================================================
# Architecture:
#   1. Board selection      (existing)
#   2. Hardware variant     (NEW - preset selector)
#   3. Feature selection    (NEW - interactive feature flags)
#   4. sdkconfig generation (NEW - safe config writer)
#   5. Optional menuconfig  (NEW - launches make menuconfig)
#   6. Submodule update     (existing)
#   7. Firmware build       (existing - UNCHANGED)
#   8. Flash stage          (NEW - device detection + esptool flash)
# =============================================================================

set -euo pipefail
# shellcheck disable=SC2034  # Arrays used via nameref (_load_preset/_merge_block) — false positive

# ---------------------------------------------------------------------------
# PATHS
# ---------------------------------------------------------------------------
# Resolve REPO as the parent of the directory this script lives in.
# Layout: OSBuild/builder/zeno_build.sh  =>  REPO = OSBuild/
# Override with: REPO=/path/to/OSBuild ./zeno_build.sh
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO="${REPO:-$(dirname "$SCRIPT_DIR")}"

PORT_DIR="$REPO/ports/esp32"
BUILDER_DIR="$SCRIPT_DIR"           # OSBuild/builder/
MPY_MENUCONFIG="$BUILDER_DIR/mpy_menuconfig.sh"
DEV_BRANCH="dev"

# Temporary sdkconfig written by this script; cleaned up on exit
SDKCONFIG_OVERLAY=""
SDKCONFIG_BACKUP=""

# ---------------------------------------------------------------------------
# COLOUR & UI HELPERS
# ---------------------------------------------------------------------------
RED='\033[0;31m'; YELLOW='\033[1;33m'; GREEN='\033[0;32m'
CYAN='\033[0;36m'; BOLD='\033[1m'; DIM='\033[2m'; RESET='\033[0m'

banner() {
    clear
    echo -e "${CYAN}${BOLD}"
    echo "=============================================="
    echo "         ZENO OS BUILD MANAGER"
    echo "=============================================="
    echo -e "${RESET}"
}

section() { echo -e "\n${CYAN}${BOLD}── $1 ──${RESET}"; }
info()    { echo -e "  ${GREEN}✔${RESET}  $*"; }
warn()    { echo -e "  ${YELLOW}⚠${RESET}  $*"; }
die()     { echo -e "\n  ${RED}✘  ERROR: $*${RESET}\n"; exit 1; }
ask()     { read -rp "$(echo -e "  ${BOLD}$1${RESET} ")" "$2"; }

# ---------------------------------------------------------------------------
# CLEANUP TRAP — ensure temp sdkconfig is removed on any exit
# ---------------------------------------------------------------------------
cleanup() {
    local exit_code=$?
    if [[ -n "$SDKCONFIG_OVERLAY" && -f "$SDKCONFIG_OVERLAY" ]]; then
        rm -f "$SDKCONFIG_OVERLAY"
    fi
    if [[ -n "$SDKCONFIG_BACKUP" && -f "$SDKCONFIG_BACKUP" ]]; then
        # Restore original sdkconfig if we interrupted mid-build
        local sdk_target="$PORT_DIR/sdkconfig"
        if [[ -f "$SDKCONFIG_BACKUP" ]]; then
            mv "$SDKCONFIG_BACKUP" "$sdk_target" 2>/dev/null || true
        fi
    fi
    exit $exit_code
}
trap cleanup EXIT INT TERM

# =============================================================================
# SECTION 1 — BOARD PRESETS
# =============================================================================
# Storage format: associative arrays keyed by PRESET_ID.
# Each array maps CONFIG_KEY → value (raw Kconfig syntax).
# Presets are merged with user feature selections; user choices take priority.
# =============================================================================

declare -A PRESET_ESP32S3_N16R8=(
    # Flash
    [CONFIG_ESPTOOLPY_FLASHSIZE]="\"16MB\""
    [CONFIG_ESPTOOLPY_FLASHSIZE_16MB]="y"
    [CONFIG_ESPTOOLPY_FLASHFREQ_80M]="y"
    [CONFIG_ESPTOOLPY_FLASHFREQ]="\"80m\""
    [CONFIG_ESPTOOLPY_FLASHMODE_DIO]="y"
    [CONFIG_ESPTOOLPY_FLASHMODE]="\"dio\""
    # Partition table
    [CONFIG_PARTITION_TABLE_CUSTOM]="y"
    # SPIRAM — mirrors ports/esp32/boards/sdkconfig.spiram_sx exactly
    [CONFIG_SPIRAM]="y"
    [CONFIG_SPIRAM_MODE_OCT]="y"
    [CONFIG_SPIRAM_TYPE_AUTO]="y"
    [CONFIG_SPIRAM_CLK_IO]="30"
    [CONFIG_SPIRAM_CS_IO]="26"
    [CONFIG_SPIRAM_SPEED_80M]="y"
    [CONFIG_SPIRAM_BOOT_INIT]="y"
    [CONFIG_SPIRAM_IGNORE_NOTFOUND]="y"
    [CONFIG_SPIRAM_USE_MALLOC]="y"
    [CONFIG_SPIRAM_MALLOC_ALWAYSINTERNAL]="8192"
    [CONFIG_ESP32S3_SPIRAM_SUPPORT]="y"
    [CONFIG_SPIRAM_SIZE]="8388608"
    # IDF target
    [CONFIG_IDF_TARGET]="\"esp32s3\""
    # USB OTG / TinyUSB
    [CONFIG_USB_OTG_SUPPORTED]="y"
    [CONFIG_TINYUSB_ENABLED]="y"
)

declare -A PRESET_ESP32S3_N16R16=(
    [CONFIG_ESPTOOLPY_FLASHSIZE]="\"16MB\""
    [CONFIG_ESPTOOLPY_FLASHSIZE_16MB]="y"
    [CONFIG_ESPTOOLPY_FLASHFREQ_80M]="y"
    [CONFIG_ESPTOOLPY_FLASHFREQ]="\"80m\""
    [CONFIG_ESPTOOLPY_FLASHMODE_DIO]="y"
    [CONFIG_ESPTOOLPY_FLASHMODE]="\"dio\""
    [CONFIG_PARTITION_TABLE_CUSTOM]="y"
    [CONFIG_SPIRAM]="y"
    [CONFIG_SPIRAM_MODE_OCT]="y"
    [CONFIG_SPIRAM_TYPE_AUTO]="y"
    [CONFIG_SPIRAM_CLK_IO]="30"
    [CONFIG_SPIRAM_CS_IO]="26"
    [CONFIG_SPIRAM_SPEED_80M]="y"
    [CONFIG_SPIRAM_BOOT_INIT]="y"
    [CONFIG_SPIRAM_IGNORE_NOTFOUND]="y"
    [CONFIG_SPIRAM_USE_MALLOC]="y"
    [CONFIG_SPIRAM_MALLOC_ALWAYSINTERNAL]="8192"
    [CONFIG_ESP32S3_SPIRAM_SUPPORT]="y"
    [CONFIG_SPIRAM_SIZE]="16777216"
    [CONFIG_IDF_TARGET]="\"esp32s3\""
    [CONFIG_USB_OTG_SUPPORTED]="y"
    [CONFIG_TINYUSB_ENABLED]="y"
)

declare -A PRESET_ESP32S3_N8R8=(
    [CONFIG_ESPTOOLPY_FLASHSIZE]="\"8MB\""
    [CONFIG_ESPTOOLPY_FLASHSIZE_8MB]="y"
    [CONFIG_ESPTOOLPY_FLASHFREQ_80M]="y"
    [CONFIG_ESPTOOLPY_FLASHFREQ]="\"80m\""
    [CONFIG_ESPTOOLPY_FLASHMODE_DIO]="y"
    [CONFIG_ESPTOOLPY_FLASHMODE]="\"dio\""
    [CONFIG_SPIRAM]="y"
    [CONFIG_SPIRAM_MODE_OCT]="y"
    [CONFIG_SPIRAM_TYPE_AUTO]="y"
    [CONFIG_SPIRAM_CLK_IO]="30"
    [CONFIG_SPIRAM_CS_IO]="26"
    [CONFIG_SPIRAM_SPEED_80M]="y"
    [CONFIG_SPIRAM_BOOT_INIT]="y"
    [CONFIG_SPIRAM_IGNORE_NOTFOUND]="y"
    [CONFIG_SPIRAM_USE_MALLOC]="y"
    [CONFIG_SPIRAM_MALLOC_ALWAYSINTERNAL]="8192"
    [CONFIG_ESP32S3_SPIRAM_SUPPORT]="y"
    [CONFIG_SPIRAM_SIZE]="8388608"
    [CONFIG_IDF_TARGET]="\"esp32s3\""
    [CONFIG_USB_OTG_SUPPORTED]="y"
    [CONFIG_TINYUSB_ENABLED]="y"
)

declare -A PRESET_ESP32S3_N8=(
    [CONFIG_ESPTOOLPY_FLASHSIZE]="\"8MB\""
    [CONFIG_ESPTOOLPY_FLASHSIZE_8MB]="y"
    [CONFIG_IDF_TARGET]="\"esp32s3\""
    [CONFIG_USB_OTG_SUPPORTED]="y"
    [CONFIG_TINYUSB_ENABLED]="y"
)

declare -A PRESET_ESP32C3=(
    [CONFIG_ESPTOOLPY_FLASHSIZE]="\"4MB\""
    [CONFIG_ESPTOOLPY_FLASHSIZE_4MB]="y"
    [CONFIG_IDF_TARGET]="\"esp32c3\""
    [CONFIG_USB_ENABLED]="y"
    [CONFIG_ESP_CONSOLE_USB_SERIAL_JTAG]="y"
    [CONFIG_BTDM_CTRL_MODE_BLE_ONLY]="y"
    [CONFIG_BT_ENABLED]="y"
)

declare -A PRESET_ESP32C6=(
    [CONFIG_ESPTOOLPY_FLASHSIZE]="\"4MB\""
    [CONFIG_ESPTOOLPY_FLASHSIZE_4MB]="y"
    [CONFIG_IDF_TARGET]="\"esp32c6\""
    [CONFIG_USB_ENABLED]="y"
    [CONFIG_ESP_CONSOLE_USB_SERIAL_JTAG]="y"
    [CONFIG_BT_ENABLED]="y"
    [CONFIG_IEEE802154_ENABLED]="y"
)

# PSRAM feature block — merged when FEATURE_PSRAM=y
declare -A FEATURE_PSRAM_BLOCK=(
    [CONFIG_SPIRAM]="y"
    [CONFIG_SPIRAM_MODE_OCT]="y"
    [CONFIG_SPIRAM_BOOT_INIT]="y"
    [CONFIG_SPIRAM_USE_MALLOC]="y"
    [CONFIG_SPIRAM_IGNORE_NOTFOUND]="y"
    [CONFIG_SPIRAM_SPEED_80M]="y"
    [CONFIG_ESP32S3_SPIRAM_SUPPORT]="y"
)

# USB CDC feature block
declare -A FEATURE_USB_CDC_BLOCK=(
    [CONFIG_TINYUSB_ENABLED]="y"
    [CONFIG_TINYUSB_CDC_ENABLED]="y"
    [CONFIG_USB_OTG_SUPPORTED]="y"
    [CONFIG_TINYUSB_CDC_RX_BUFSIZE]="512"
    [CONFIG_TINYUSB_CDC_TX_BUFSIZE]="512"
)

# USB Serial/JTAG feature block
declare -A FEATURE_USB_JTAG_BLOCK=(
    [CONFIG_ESP_CONSOLE_USB_SERIAL_JTAG]="y"
    [CONFIG_ESP_CONSOLE_USB_SERIAL_JTAG_ENABLED]="y"
)

# Bluetooth feature block
declare -A FEATURE_BT_BLOCK=(
    [CONFIG_BT_ENABLED]="y"
    [CONFIG_BTDM_CTRL_MODE_BLE_ONLY]="y"
    [CONFIG_BT_BLE_ENABLED]="y"
)

# WiFi feature block
declare -A FEATURE_WIFI_BLOCK=(
    [CONFIG_ESP_WIFI_ENABLED]="y"
    [CONFIG_ESP_WIFI_STATIC_RX_BUFFER_NUM]="10"
    [CONFIG_ESP_WIFI_DYNAMIC_RX_BUFFER_NUM]="32"
    [CONFIG_ESP_WIFI_AMPDU_TX_ENABLED]="y"
    [CONFIG_ESP_WIFI_AMPDU_RX_ENABLED]="y"
)

# Optimization feature block
declare -A FEATURE_OPT_BLOCK=(
    [CONFIG_COMPILER_OPTIMIZATION_SIZE]="y"
    [CONFIG_COMPILER_OPTIMIZATION_ASSERTIONS_SILENT]="y"
    [CONFIG_FREERTOS_ASSERT_FAIL_ABORT]="n"
    [CONFIG_BOOTLOADER_COMPILER_OPTIMIZATION_SIZE]="y"
)

# =============================================================================
# SECTION 2 — BOARD SELECTION (existing, preserved exactly)
# =============================================================================

BOARD=""
TARGET=""

choose_board() {
    section "Select Target Board"
    echo "    1) ESP32_GENERIC"
    echo "    2) ESP32_GENERIC_S2"
    echo "    3) ESP32_GENERIC_S3"
    echo "    4) ESP32_GENERIC_C3"
    echo "    5) ESP32_GENERIC_C5"
    echo "    6) ESP32_GENERIC_C6"
    echo
    ask "Select Board [1-6]:" CHOICE
    case "$CHOICE" in
        1) BOARD="ESP32_GENERIC";    TARGET="ESP32"    ;;
        2) BOARD="ESP32_GENERIC_S2"; TARGET="ESP32-S2" ;;
        3) BOARD="ESP32_GENERIC_S3"; TARGET="ESP32-S3" ;;
        4) BOARD="ESP32_GENERIC_C3"; TARGET="ESP32-C3" ;;
        5) BOARD="ESP32_GENERIC_C5"; TARGET="ESP32-C5" ;;
        6) BOARD="ESP32_GENERIC_C6"; TARGET="ESP32-C6" ;;
        *) die "Invalid board selection: $CHOICE" ;;
    esac
}

# =============================================================================
# SECTION 3 — HARDWARE VARIANT / PRESET SELECTION
# =============================================================================

SELECTED_PRESET_ID=""   # e.g. "ESP32S3_N16R8"
declare -A ACTIVE_PRESET=()

choose_variant() {
    local board="$1"
    section "Select Hardware Variant"

    case "$board" in
        ESP32_GENERIC_S3)
            echo "    1) ESP32-S3-N16R8   (16MB Flash, 8MB Octal PSRAM)"
            echo "    2) ESP32-S3-N16R16  (16MB Flash, 16MB Octal PSRAM)"
            echo "    3) ESP32-S3-N8R8    (8MB Flash,  8MB Octal PSRAM)"
            echo "    4) ESP32-S3-N8      (8MB Flash,  no PSRAM)"
            echo "    5) Custom           (manual feature selection only)"
            echo
            ask "Select Variant [1-5]:" VCHOICE
            case "$VCHOICE" in
                1) SELECTED_PRESET_ID="ESP32S3_N16R8";  _load_preset PRESET_ESP32S3_N16R8 ;;
                2) SELECTED_PRESET_ID="ESP32S3_N16R16"; _load_preset PRESET_ESP32S3_N16R16 ;;
                3) SELECTED_PRESET_ID="ESP32S3_N8R8";   _load_preset PRESET_ESP32S3_N8R8 ;;
                4) SELECTED_PRESET_ID="ESP32S3_N8";     _load_preset PRESET_ESP32S3_N8 ;;
                5) SELECTED_PRESET_ID="CUSTOM" ;;
                *) die "Invalid variant selection" ;;
            esac
            ;;
        ESP32_GENERIC_C3)
            echo "    1) ESP32-C3  (Standard 4MB preset)"
            echo "    2) Custom    (manual feature selection only)"
            echo
            ask "Select Variant [1-2]:" VCHOICE
            case "$VCHOICE" in
                1) SELECTED_PRESET_ID="ESP32C3"; _load_preset PRESET_ESP32C3 ;;
                2) SELECTED_PRESET_ID="CUSTOM" ;;
                *) die "Invalid variant selection" ;;
            esac
            ;;
        ESP32_GENERIC_C6)
            echo "    1) ESP32-C6  (Standard 4MB preset)"
            echo "    2) Custom    (manual feature selection only)"
            echo
            ask "Select Variant [1-2]:" VCHOICE
            case "$VCHOICE" in
                1) SELECTED_PRESET_ID="ESP32C6"; _load_preset PRESET_ESP32C6 ;;
                2) SELECTED_PRESET_ID="CUSTOM" ;;
                *) die "Invalid variant selection" ;;
            esac
            ;;
        *)
            warn "No hardware variants defined for $board — using Custom (feature selection only)."
            SELECTED_PRESET_ID="CUSTOM"
            ;;
    esac

    if [[ "$SELECTED_PRESET_ID" != "CUSTOM" ]]; then
        info "Preset loaded: $SELECTED_PRESET_ID (${#ACTIVE_PRESET[@]} config keys)"
    fi
}

# Internal: copy a named preset associative array into ACTIVE_PRESET
_load_preset() {
    local preset_name="$1"
    local -n src_ref="$preset_name"
    ACTIVE_PRESET=()
    for key in "${!src_ref[@]}"; do
        ACTIVE_PRESET[$key]="${src_ref[$key]}"
    done
}

# =============================================================================
# SECTION 4 — FEATURE SELECTION
# =============================================================================

# Feature flags — populated by choose_features()
FEATURE_PSRAM="n"
FEATURE_USB_CDC="n"
FEATURE_USB_JTAG="n"
FEATURE_BT="n"
FEATURE_WIFI="n"
FEATURE_OPT="n"
FEATURE_MENUCONFIG="n"

choose_features() {
    section "Configure Features"
    echo -e "  ${DIM}Preset defaults shown in [brackets]. Override with y/n, or press Enter to keep.${RESET}"
    echo

    _feature_prompt "Enable PSRAM?"           FEATURE_PSRAM    "CONFIG_SPIRAM"
    _feature_prompt "Enable USB CDC?"         FEATURE_USB_CDC  "CONFIG_TINYUSB_CDC_ENABLED"
    _feature_prompt "Enable USB Serial/JTAG?" FEATURE_USB_JTAG "CONFIG_ESP_CONSOLE_USB_SERIAL_JTAG"
    _feature_prompt "Enable Bluetooth?"       FEATURE_BT       "CONFIG_BT_ENABLED"
    _feature_prompt "Enable WiFi?"            FEATURE_WIFI     "CONFIG_ESP_WIFI_ENABLED"
    _feature_prompt "Enable Optimizations?"   FEATURE_OPT      "CONFIG_COMPILER_OPTIMIZATION_SIZE"
    echo
    ask "Open menuconfig after sdkconfig generation? (y/n):" FEATURE_MENUCONFIG
    FEATURE_MENUCONFIG="${FEATURE_MENUCONFIG,,}"
    [[ "$FEATURE_MENUCONFIG" =~ ^[yn]$ ]] || FEATURE_MENUCONFIG="n"
}

# Ask user for a feature, show preset default in brackets
_feature_prompt() {
    local label="$1"
    local -n flag_ref="$2"
    local config_key="$3"

    local preset_default="n"
    if [[ -v "ACTIVE_PRESET[$config_key]" && "${ACTIVE_PRESET[$config_key]}" == "y" ]]; then
        preset_default="y"
    fi

    ask "$label [preset: $preset_default] (y/n/Enter=keep):" RAW_INPUT
    RAW_INPUT="${RAW_INPUT,,}"
    case "$RAW_INPUT" in
        y) flag_ref="y" ;;
        n) flag_ref="n" ;;
        "") flag_ref="$preset_default" ;;
        *) warn "Unrecognised input '$RAW_INPUT', using preset default ($preset_default)."
           flag_ref="$preset_default" ;;
    esac
}

# =============================================================================
# SECTION 5 — SDKCONFIG GENERATION
# =============================================================================
# Strategy:
#   1. Start from board's base sdkconfig (if present) in build directory.
#   2. Merge ACTIVE_PRESET keys on top.
#   3. Merge feature block keys on top (user features override preset).
#   4. Write merged config to PORT_DIR/sdkconfig (temp file, removed on exit).
#   5. MicroPython's Makefile reads sdkconfig from the port directory during build.
# =============================================================================

declare -A MERGED_CONFIG=()   # final merged Kconfig map

merge_configs() {
    MERGED_CONFIG=()

    # Layer 1: preset
    for k in "${!ACTIVE_PRESET[@]}"; do
        MERGED_CONFIG[$k]="${ACTIVE_PRESET[$k]}"
    done

    # Layer 2: feature blocks (override preset)
    [[ "$FEATURE_PSRAM"    == "y" ]] && _merge_block FEATURE_PSRAM_BLOCK
    [[ "$FEATURE_USB_CDC"  == "y" ]] && _merge_block FEATURE_USB_CDC_BLOCK
    [[ "$FEATURE_USB_JTAG" == "y" ]] && _merge_block FEATURE_USB_JTAG_BLOCK
    [[ "$FEATURE_BT"       == "y" ]] && _merge_block FEATURE_BT_BLOCK
    [[ "$FEATURE_WIFI"     == "y" ]] && _merge_block FEATURE_WIFI_BLOCK
    [[ "$FEATURE_OPT"      == "y" ]] && _merge_block FEATURE_OPT_BLOCK

    # Conflict resolution: USB CDC and USB Serial/JTAG use different USB hardware paths
    if [[ "$FEATURE_USB_CDC" == "y" && "$FEATURE_USB_JTAG" == "y" ]]; then
        warn "USB CDC and USB Serial/JTAG are mutually exclusive on most ESP32-S3 variants."
        warn "USB Serial/JTAG will be disabled to avoid config conflict."
        unset 'MERGED_CONFIG[CONFIG_ESP_CONSOLE_USB_SERIAL_JTAG]'
        unset 'MERGED_CONFIG[CONFIG_ESP_CONSOLE_USB_SERIAL_JTAG_ENABLED]'
        FEATURE_USB_JTAG="n"
    fi
}

_merge_block() {
    local block_name="$1"
    local -n block_ref="$block_name"
    for k in "${!block_ref[@]}"; do
        MERGED_CONFIG[$k]="${block_ref[$k]}"
    done
}

# ---------------------------------------------------------------------------
# Write the merged config to PORT_DIR/sdkconfig
# Safely handles existing sdkconfig: backs it up, patches known keys,
# and appends any keys that were not already present.
# ---------------------------------------------------------------------------
generate_sdkconfig() {
    local sdk_path="$PORT_DIR/sdkconfig"
    SDKCONFIG_OVERLAY="$PORT_DIR/sdkconfig.zeno_overlay_$$"
    SDKCONFIG_BACKUP=""

    section "Generating sdkconfig"

    if [[ ${#MERGED_CONFIG[@]} -eq 0 ]]; then
        info "No configuration keys to write — skipping sdkconfig generation."
        return 0
    fi

    # Back up any pre-existing sdkconfig (may have been left from a previous build)
    if [[ -f "$sdk_path" ]]; then
        SDKCONFIG_BACKUP="${sdk_path}.zeno_bak_$$"
        cp "$sdk_path" "$SDKCONFIG_BACKUP"
        info "Existing sdkconfig backed up to $(basename "$SDKCONFIG_BACKUP")"
        _patch_sdkconfig "$sdk_path"
    else
        _write_fresh_sdkconfig "$sdk_path"
    fi

    SDKCONFIG_OVERLAY="$sdk_path"   # track for cleanup trap
    info "sdkconfig written: $sdk_path (${#MERGED_CONFIG[@]} keys)"
    echo
    echo -e "  ${DIM}Config summary:${RESET}"
    for k in $(echo "${!MERGED_CONFIG[@]}" | tr ' ' '\n' | sort); do
        printf "    %-50s = %s\n" "$k" "${MERGED_CONFIG[$k]}"
    done
}

# Write a fresh sdkconfig from merged config only
_write_fresh_sdkconfig() {
    local sdk_path="$1"
    {
        echo "# Auto-generated by ZENO OS Build Manager"
        echo "# Generated: $(date -u '+%Y-%m-%d %H:%M:%S UTC')"
        echo "# Board: $BOARD  Preset: $SELECTED_PRESET_ID"
        echo "#"
        echo "# DO NOT EDIT — regenerated on every build."
        echo "# Modify build.sh presets or feature selections instead."
        echo ""
        for k in $(echo "${!MERGED_CONFIG[@]}" | tr ' ' '\n' | sort); do
            local val="${MERGED_CONFIG[$k]}"
            if [[ "$val" == "y" || "$val" == "n" ]]; then
                if [[ "$val" == "y" ]]; then
                    echo "${k}=${val}"
                else
                    echo "# ${k} is not set"
                fi
            else
                echo "${k}=${val}"
            fi
        done
    } > "$sdk_path"
}

# Patch an existing sdkconfig: update keys that exist, append keys that don't
_patch_sdkconfig() {
    local sdk_path="$1"
    local tmp="${sdk_path}.zeno_tmp_$$"

    cp "$sdk_path" "$tmp"

    local appended_keys=()

    for k in "${!MERGED_CONFIG[@]}"; do
        local val="${MERGED_CONFIG[$k]}"
        local pattern_set="^${k}="
        local pattern_notset="^# ${k} is not set"

        if grep -qE "$pattern_set|$pattern_notset" "$tmp" 2>/dev/null; then
            # Key exists — replace it (handles both set and not-set forms)
            if [[ "$val" == "n" ]]; then
                # Use sed to replace any existing form with the "not set" comment
                sed -i "s|${pattern_set}.*|# ${k} is not set|g" "$tmp"
                sed -i "s|${pattern_notset}|# ${k} is not set|g" "$tmp"
            else
                sed -i "s|# ${k} is not set|${k}=${val}|g" "$tmp"
                sed -i "s|${k}=.*|${k}=${val}|g" "$tmp"
            fi
        else
            # Key not present — collect for append
            appended_keys+=("$k")
        fi
    done

    # Append new keys at end of file
    if [[ ${#appended_keys[@]} -gt 0 ]]; then
        {
            echo ""
            echo "# Keys appended by ZENO OS Build Manager — $(date -u '+%Y-%m-%dT%H:%M:%SZ')"
            for k in "${appended_keys[@]}"; do
                local val="${MERGED_CONFIG[$k]}"
                if [[ "$val" == "n" ]]; then
                    echo "# ${k} is not set"
                else
                    echo "${k}=${val}"
                fi
            done
        } >> "$tmp"
    fi

    mv "$tmp" "$sdk_path"
}

# =============================================================================
# SECTION 6 — VALIDATION
# =============================================================================

validate_config() {
    section "Validating Configuration"
    local errors=0

    # Validate PSRAM + board compatibility
    if [[ "$FEATURE_PSRAM" == "y" ]]; then
        case "$BOARD" in
            ESP32_GENERIC_S3) : ;;  # PSRAM supported
            ESP32_GENERIC)
                warn "ESP32 (original) PSRAM support is limited. Verify your module has PSRAM."
                ;;
            ESP32_GENERIC_C3|ESP32_GENERIC_C5|ESP32_GENERIC_C6)
                warn "ESP32-C3/C5/C6 do not support external PSRAM. Disabling PSRAM."
                FEATURE_PSRAM="n"
                unset 'MERGED_CONFIG[CONFIG_SPIRAM]'
                unset 'MERGED_CONFIG[CONFIG_SPIRAM_MODE_OCT]'
                unset 'MERGED_CONFIG[CONFIG_SPIRAM_BOOT_INIT]'
                ;;
        esac
    fi

    # Validate USB CDC + board compatibility
    if [[ "$FEATURE_USB_CDC" == "y" ]]; then
        case "$BOARD" in
            ESP32_GENERIC_S3|ESP32_GENERIC_S2) : ;;   # USB OTG capable
            ESP32_GENERIC_C3|ESP32_GENERIC_C6)
                warn "C3/C6 support USB Serial/JTAG only, not USB OTG CDC. Switching to Serial/JTAG."
                FEATURE_USB_CDC="n"
                FEATURE_USB_JTAG="y"
                _merge_block FEATURE_USB_JTAG_BLOCK
                ;;
            ESP32_GENERIC)
                warn "ESP32 (original) does not have native USB. Disabling USB CDC."
                FEATURE_USB_CDC="n"
                ;;
        esac
    fi

    # Validate Flash size consistency with preset
    if [[ -v "MERGED_CONFIG[CONFIG_ESPTOOLPY_FLASHSIZE]" ]]; then
        local flash_size="${MERGED_CONFIG[CONFIG_ESPTOOLPY_FLASHSIZE]}"
        info "Flash size: $flash_size"
    fi

    # Check IDF target consistency
    if [[ -v "MERGED_CONFIG[CONFIG_IDF_TARGET]" ]]; then
        local idf_target="${MERGED_CONFIG[CONFIG_IDF_TARGET]}"
        info "IDF target: $idf_target"
    fi

    # Warn if no keys were generated
    if [[ ${#MERGED_CONFIG[@]} -eq 0 && "$SELECTED_PRESET_ID" == "CUSTOM" ]]; then
        warn "Custom mode selected with no features enabled — no sdkconfig will be generated."
        warn "MicroPython will use its internal defaults."
    fi

    if [[ $errors -gt 0 ]]; then
        die "Configuration validation failed with $errors error(s). Aborting."
    fi

    info "Validation passed."
}

# =============================================================================
# SECTION 7 — MicroPython FEATURE CONFIGURATION (mpy_menuconfig.sh)
# =============================================================================
# Replaces the old "make menuconfig BOARD=..." call.
# Launches mpy_menuconfig.sh — the ZENO OS MicroPython configuration manager.
# That script writes .config/mpyconfig and .config/mpyconfig.mk which are
# read by the build system via CFLAGS_EXTRA / FROZEN_MANIFEST.
# After mpy_menuconfig exits the build continues normally.
# =============================================================================

run_menuconfig_if_requested() {
    [[ "$FEATURE_MENUCONFIG" != "y" ]] && return 0

    section "MicroPython Feature Configuration"

    # Verify mpy_menuconfig.sh exists next to this script in builder/
    if [[ ! -f "$MPY_MENUCONFIG" ]]; then
        warn "mpy_menuconfig.sh not found at: $MPY_MENUCONFIG"
        warn "Place mpy_menuconfig.sh in the same directory as zeno_build.sh"
        warn "  ($BUILDER_DIR/)"
        warn "Skipping MicroPython feature configuration."
        return 0
    fi

    if [[ ! -x "$MPY_MENUCONFIG" ]]; then
        chmod +x "$MPY_MENUCONFIG"
    fi

    info "Launching mpy_menuconfig.sh for board: $BOARD (preset: $SELECTED_PRESET_ID)"
    echo
    warn "Configure MicroPython features. Save & Exit when done — build will continue."
    echo
    read -rp "  Press Enter to open MicroPython configuration…" _

    # Pass board name and preset so mpy_menuconfig can pre-populate sensibly.
    # REPO is exported so mpy_menuconfig resolves lib/ modules/ extmod/ correctly.
    export REPO
    "$MPY_MENUCONFIG" --board "$BOARD" --preset "$SELECTED_PRESET_ID" || {
        local rc=$?
        # Exit code 1 = user chose "Exit Without Saving" — not a fatal error
        if [[ $rc -eq 1 ]]; then
            warn "mpy_menuconfig exited without saving. Continuing with sdkconfig only."
        else
            die "mpy_menuconfig.sh exited with error code $rc."
        fi
    }

    # If mpyconfig.mk was generated, surface its path for the user
    local mkcfg="$REPO/.config/mpyconfig.mk"
    if [[ -f "$mkcfg" ]]; then
        info "MicroPython config written: $mkcfg"
        info "Makefile fragment: $mkcfg"
    fi

    echo
    info "MicroPython configuration complete. Continuing build…"
}

# =============================================================================
# SECTION 8 — BUILD (existing commands, UNCHANGED)
# =============================================================================

run_build() {
    section "Building Firmware"
    echo "  Board   : $BOARD"
    echo "  Target  : $TARGET"
    echo "  Preset  : $SELECTED_PRESET_ID"
    echo

    cd "$PORT_DIR"

    echo "  → make BOARD=\"$BOARD\" submodules"
    make BOARD="$BOARD" submodules

    echo
    echo "  → make BOARD=\"$BOARD\""
    make BOARD="$BOARD"
}

# =============================================================================
# SECTION 9 — DISPLAY OUTPUTS (existing, preserved)
# =============================================================================

show_outputs() {
    local BUILD_DIR="$PORT_DIR/build-$BOARD"

    echo
    echo "=============================================="
    echo "BUILD OUTPUTS"
    echo "=============================================="
    find "$BUILD_DIR" -type f | grep -E '\.(bin|elf)$' || true
    echo
    echo "Expected firmware location:"
    echo "$BUILD_DIR"
    echo
    info "Build complete."
}

# =============================================================================
# SECTION 10 — FLASH STAGE
# =============================================================================
# Detection strategy:
#   1. Enumerate candidate ports: /dev/ttyUSB*, /dev/ttyACM*, /dev/cu.usbserial*,
#      /dev/cu.usbmodem*, /dev/ttyS* (filtered to only those present + accessible)
#   2. For each candidate, attempt a fast esptool.py chip_id probe (2 s timeout).
#      Record: port, chip type, MAC, flash size (if readable).
#   3. Present numbered list of confirmed ESP32 devices + any unprobed candidates.
#   4. User selects a port (or enters a custom path).
#   5. Script derives flash baud rate, partition table path, and binary offsets
#      from the build directory, then runs esptool.py write_flash.
# =============================================================================

# Populated by detect_esp_devices()
declare -a FLASH_PORTS=()        # confirmed or candidate port paths
declare -a FLASH_CHIP_INFO=()    # human-readable chip info per port (parallel array)

# ---------------------------------------------------------------------------
# enumerate_candidate_ports — list all serial-like devices on this host
# ---------------------------------------------------------------------------
enumerate_candidate_ports() {
    local -a candidates=()

    # Linux USB-serial bridges (CH340, CP210x, FTDI, …)
    for p in /dev/ttyUSB*; do [[ -e "$p" ]] && candidates+=("$p"); done
    # Linux USB CDC-ACM (native USB on S3/C3/C6 with USB Serial/JTAG)
    for p in /dev/ttyACM*; do [[ -e "$p" ]] && candidates+=("$p"); done
    # macOS USB-serial
    for p in /dev/cu.usbserial*; do [[ -e "$p" ]] && candidates+=("$p"); done
    for p in /dev/cu.usbmodem*;  do [[ -e "$p" ]] && candidates+=("$p"); done
    # WSL / legacy ttyS (only low-numbered ones to avoid noise)
    for p in /dev/ttyS{0..9}; do [[ -e "$p" ]] && candidates+=("$p"); done

    printf '%s\n' "${candidates[@]}"
}

# ---------------------------------------------------------------------------
# probe_port — run esptool.py chip_id on a single port; sets arrays
# Returns 0 if chip identified, 1 if probe failed or not an ESP device
# ---------------------------------------------------------------------------
probe_port() {
    local port="$1"
    local result chip_desc

    # Check read permission before attempting
    if [[ ! -r "$port" ]]; then
        FLASH_PORTS+=("$port")
        FLASH_CHIP_INFO+=("(no read permission — may need: sudo chmod a+rw $port)")
        return 1
    fi

    # Fast probe: 2 second timeout, 115200 baud initial connect baud
    # esptool.py outputs lines like:
    #   Chip is ESP32-S3 (revision v0.2)
    #   Features: WiFi, BLE
    #   Crystal is 40MHz
    #   MAC: aa:bb:cc:dd:ee:ff
    # Probe using python -m esptool (preferred) then fallbacks
    local _probe_cmd="esptool.py"
    if python -m esptool version >/dev/null 2>&1; then
        _probe_cmd="python -m esptool"
    elif python3 -m esptool version >/dev/null 2>&1; then
        _probe_cmd="python3 -m esptool"
    fi
    result=$(timeout 4 $_probe_cmd --port "$port" --baud 115200 \
                 --before default_reset --after no_reset chip_id 2>&1) || true

    # Check for known ESP chip strings in output
    if echo "$result" | grep -qiE 'Chip is ESP|chip type|esp32|esp8266'; then
        local chip_line mac_line flash_line
        chip_line=$(echo "$result"  | grep -m1 -i 'Chip is'   | sed 's/^[[:space:]]*//' || true)
        mac_line=$(echo "$result"   | grep -m1 -i '^MAC:'      | sed 's/^[[:space:]]*//' || true)
        flash_line=$(echo "$result" | grep -m1 -i 'flash size' | sed 's/^[[:space:]]*//' || true)

        chip_desc="${chip_line}"
        [[ -n "$mac_line"   ]] && chip_desc+=" | ${mac_line}"
        [[ -n "$flash_line" ]] && chip_desc+=" | ${flash_line}"

        FLASH_PORTS+=("$port")
        FLASH_CHIP_INFO+=("$chip_desc")
        return 0
    elif echo "$result" | grep -qiE 'failed to connect|no serial data|errno'; then
        # Port exists but nothing responding — include as unconfirmed candidate
        FLASH_PORTS+=("$port")
        FLASH_CHIP_INFO+=("(no response — device may need manual reset to bootloader)")
        return 1
    fi

    # Port exists but produced no ESP-related output — skip silently
    return 1
}

# ---------------------------------------------------------------------------
# detect_esp_devices — probe all candidates, populate arrays
# ---------------------------------------------------------------------------
detect_esp_devices() {
    FLASH_PORTS=()
    FLASH_CHIP_INFO=()

    local -a candidates
    mapfile -t candidates < <(enumerate_candidate_ports)

    if [[ ${#candidates[@]} -eq 0 ]]; then
        return 0
    fi

    echo -e "  ${DIM}Scanning ${#candidates[@]} candidate port(s)…${RESET}"

    local port
    for port in "${candidates[@]}"; do
        printf "    %-22s " "$port"
        if probe_port "$port"; then
            echo -e "${GREEN}✔ ESP detected${RESET}"
        else
            echo -e "${DIM}—${RESET}"
        fi
    done
}

# ---------------------------------------------------------------------------
# resolve_flash_args — derive esptool write_flash arguments from build dir
#
# MicroPython's ESP32 Makefile produces:
#   build-<BOARD>/bootloader/bootloader.bin    @ 0x0
#   build-<BOARD>/partition_table/partition-table.bin @ 0x8000
#   build-<BOARD>/micropython.bin              @ 0x10000
#
# It also writes build-<BOARD>/flash_args (or flash_project_args) containing
# the exact offsets and binary paths used by idf.py flash — we parse that
# first since it is the authoritative source for custom partition layouts.
# ---------------------------------------------------------------------------
resolve_flash_args() {
    local build_dir="$1"
    local -a args=()

    # Prefer flash_args file (written by MicroPython's Makefile via esptool)
    local flash_args_file=""
    for candidate in "$build_dir/flash_args" "$build_dir/flash_project_args"; do
        if [[ -f "$candidate" ]]; then
            flash_args_file="$candidate"
            break
        fi
    done

    if [[ -n "$flash_args_file" ]]; then
        # flash_args contains lines like: --flash_mode dio --flash_freq 80m ...
        # and offset+path pairs like: 0x0 bootloader/bootloader.bin
        # Read all tokens, resolve relative paths to absolute
        while IFS= read -r line || [[ -n "$line" ]]; do
            [[ -z "$line" || "$line" =~ ^# ]] && continue
            for token in $line; do
                if [[ "$token" =~ ^0x ]]; then
                    args+=("$token")
                elif [[ "$token" =~ \.bin$ ]]; then
                    # Resolve relative path against build_dir
                    if [[ -f "$build_dir/$token" ]]; then
                        args+=("$build_dir/$token")
                    elif [[ -f "$token" ]]; then
                        args+=("$token")
                    else
                        warn "flash_args references missing file: $token"
                        args+=("$token")
                    fi
                else
                    args+=("$token")
                fi
            done
        done < "$flash_args_file"
        printf '%s\n' "${args[@]}"
        return 0
    fi

    # Fallback: standard MicroPython ESP32 layout
    local boot="$build_dir/bootloader/bootloader.bin"
    local pt="$build_dir/partition_table/partition-table.bin"
    local fw="$build_dir/micropython.bin"
    local missing=0

    [[ ! -f "$boot" ]] && warn "Missing: $boot" && missing=1
    [[ ! -f "$pt"   ]] && warn "Missing: $pt"   && missing=1
    [[ ! -f "$fw"   ]] && warn "Missing: $fw"   && missing=1
    [[ $missing -eq 1 ]] && return 1

    printf '%s\n' \
        "0x0"    "$boot" \
        "0x8000" "$pt"   \
        "0x10000" "$fw"
}

# ---------------------------------------------------------------------------
# get_flash_mode_args — derive --flash_mode / --flash_freq / --flash_size
#
# Matches the exact flags used by MicroPython's ESP32 build system:
#   --flash_mode dio  (safe for all ESP32-S3 variants incl. Octal PSRAM)
#   --flash_freq 80m
#   --flash_size <from preset, default 4MB>
#
# ESP32-S3 Octal PSRAM boards (N16R8, N16R16, N8R8) still use DIO mode for
# the bootloader flash write. The octal PSRAM is initialised by the bootloader
# after flash programming; it is NOT a flash-mode setting.
# ---------------------------------------------------------------------------
get_flash_mode_args() {
    local mode="dio"
    local freq="80m"
    local size="4MB"   # conservative safe default; overridden by preset below

    # Pull flash size from merged sdkconfig (strip surrounding quotes)
    if [[ -v "MERGED_CONFIG[CONFIG_ESPTOOLPY_FLASHSIZE]" ]]; then
        size="${MERGED_CONFIG[CONFIG_ESPTOOLPY_FLASHSIZE]//\"/ }"
        size="${size// /}"   # strip spaces
        # (single-quote stripping omitted — not expected in flash size values)
    fi

    # Pull flash freq from merged sdkconfig (strip surrounding quotes)
    if [[ -v "MERGED_CONFIG[CONFIG_ESPTOOLPY_FLASHFREQ]" ]]; then
        local raw_freq="${MERGED_CONFIG[CONFIG_ESPTOOLPY_FLASHFREQ]//\"/}"
        raw_freq="${raw_freq// /}"
        [[ -n "$raw_freq" ]] && freq="$raw_freq"
    fi

    echo "--flash_mode $mode --flash_freq $freq --flash_size $size"
}

# ---------------------------------------------------------------------------
# run_flash_stage — orchestrates device detection, selection, and flashing
# ---------------------------------------------------------------------------
run_flash_stage() {
    local build_dir="$PORT_DIR/build-$BOARD"

    # Verify firmware exists before even asking
    if [[ ! -f "$build_dir/micropython.bin" && \
          ! -f "$build_dir/firmware.bin" ]]; then
        warn "No firmware binary found in $build_dir — skipping flash stage."
        return 0
    fi

    section "Flash Firmware"
    ask "Flash firmware to a connected device? (y/n):" FLASH_ASK
    [[ "${FLASH_ASK,,}" =~ ^y$ ]] || return 0

    # Resolve esptool invocation — prefer python -m esptool (most reliable),
    # fall back to esptool.py / esptool standalone if python module not found.
    local ESPTOOL_CMD=""
    if python -m esptool version >/dev/null 2>&1; then
        ESPTOOL_CMD="python -m esptool"
    elif python3 -m esptool version >/dev/null 2>&1; then
        ESPTOOL_CMD="python3 -m esptool"
    elif command -v esptool.py >/dev/null 2>&1; then
        ESPTOOL_CMD="esptool.py"
    elif command -v esptool >/dev/null 2>&1; then
        ESPTOOL_CMD="esptool"
    else
        warn "esptool not found. Install with: pip install esptool"
        warn "Cannot flash without esptool. Skipping."
        return 0
    fi
    info "Using esptool: $ESPTOOL_CMD"

    echo
    echo -e "  ${BOLD}Detecting connected ESP devices…${RESET}"
    echo
    detect_esp_devices
    echo

    # ---- Build device menu --------------------------------------------------
    local num_devices=${#FLASH_PORTS[@]}

    if [[ $num_devices -eq 0 ]]; then
        warn "No serial/USB devices detected on this system."
        warn "Connect your ESP32 and ensure the port is accessible."
        echo
        ask "Enter port path manually (or press Enter to skip):" MANUAL_PORT
        if [[ -z "$MANUAL_PORT" ]]; then
            info "Flash skipped."
            return 0
        fi
        FLASH_PORTS=("$MANUAL_PORT")
        FLASH_CHIP_INFO=("(manually entered — unprobed)")
        num_devices=1
    fi

    section "Select Flash Target"
    echo
    printf "    %-4s  %-22s  %s\n" "No." "Port" "Device Info"
    printf "    %-4s  %-22s  %s\n" "---" "----" "-----------"
    local i
    for (( i=0; i<num_devices; i++ )); do
        printf "    ${BOLD}%-4s${RESET}  ${CYAN}%-22s${RESET}  %s\n" \
            "$((i+1)))" \
            "${FLASH_PORTS[$i]}" \
            "${FLASH_CHIP_INFO[$i]}"
    done
    echo
    printf "    %-4s  %-22s  %s\n" "$((num_devices+1)))" "(custom path)" "Enter port manually"
    printf "    %-4s  %-22s  %s\n" "$((num_devices+2)))" "(skip)"        "Do not flash"
    echo

    local SELECTED_PORT=""
    while true; do
        ask "Select target [1-$((num_devices+2))]:" PORT_CHOICE
        if [[ "$PORT_CHOICE" =~ ^[0-9]+$ ]]; then
            if (( PORT_CHOICE >= 1 && PORT_CHOICE <= num_devices )); then
                SELECTED_PORT="${FLASH_PORTS[$((PORT_CHOICE-1))]}"
                break
            elif (( PORT_CHOICE == num_devices+1 )); then
                ask "Enter port path:" SELECTED_PORT
                [[ -n "$SELECTED_PORT" ]] && break
                warn "Port path cannot be empty."
            elif (( PORT_CHOICE == num_devices+2 )); then
                info "Flash skipped."
                return 0
            else
                warn "Invalid selection. Choose 1–$((num_devices+2))."
            fi
        else
            warn "Please enter a number."
        fi
    done

    # ---- Baud rate selection ------------------------------------------------
    section "Flash Settings"
    echo "  Port    : $SELECTED_PORT"
    echo
    echo "    1) 921600  (fast — recommended for most USB-serial adapters)"
    echo "    2) 460800"
    echo "    3) 230400"
    echo "    4) 115200  (safe fallback)"
    echo "    5) Custom"
    echo
    ask "Select baud rate [1-5, default=1]:" BAUD_CHOICE
    case "${BAUD_CHOICE:-1}" in
        1|"") FLASH_BAUD=921600  ;;
        2)    FLASH_BAUD=460800  ;;
        3)    FLASH_BAUD=230400  ;;
        4)    FLASH_BAUD=115200  ;;
        5)    ask "Enter baud rate:" FLASH_BAUD ;;
        *)    FLASH_BAUD=921600  ;;
    esac

    # ---- Erase before flash? -----------------------------------------------
    ask "Erase flash before writing? (y/n, default=n):" ERASE_CHOICE
    local ERASE_FLAG=""
    [[ "${ERASE_CHOICE,,}" == "y" ]] && ERASE_FLAG="--erase-all"

    # ---- Resolve binaries + offsets ----------------------------------------
    section "Resolving Firmware Binaries"
    local -a flash_file_args
    mapfile -t flash_file_args < <(resolve_flash_args "$build_dir")

    if [[ ${#flash_file_args[@]} -eq 0 ]]; then
        die "Could not resolve firmware binary paths from $build_dir. Aborting flash."
    fi

    # Derive flash mode/size flags — returned as space-separated string, split into array
    local mode_args_str
    mode_args_str=$(get_flash_mode_args)
    local -a mode_args_arr
    read -ra mode_args_arr <<< "$mode_args_str"

    # Erase flag as array (empty array when not set — expands to nothing)
    local -a erase_arr=()
    [[ -n "$ERASE_FLAG" ]] && erase_arr=("$ERASE_FLAG")

    # ---- Summary + confirmation ---------------------------------------------
    section "Flash Summary"
    echo
    echo "  Port      : $SELECTED_PORT"
    echo "  Baud      : $FLASH_BAUD"
    echo "  Board     : $BOARD"
    echo "  Mode args : $mode_args_str"
    [[ ${#erase_arr[@]} -gt 0 ]] && echo "  Erase     : YES (full chip erase before write)"
    echo
    echo "  Binaries to flash:"
    local j=0
    while (( j < ${#flash_file_args[@]} )); do
        local offset="${flash_file_args[$j]}"
        local binfile="${flash_file_args[$((j+1))]}"
        local size_kb=""
        if [[ -f "$binfile" ]]; then
            size_kb=$(( $(stat -c%s "$binfile" 2>/dev/null || stat -f%z "$binfile" 2>/dev/null || echo 0) / 1024 ))
            printf "    %-10s  %s  ${DIM}(%d KB)${RESET}\n" "$offset" "$(basename "$binfile")" "$size_kb"
        else
            printf "    %-10s  ${RED}%s (MISSING)${RESET}\n" "$offset" "$binfile"
        fi
        (( j+=2 ))
    done
    echo

    ask "Proceed with flash? (y/n):" FLASH_CONFIRM
    [[ "${FLASH_CONFIRM,,}" =~ ^y$ ]] || { info "Flash cancelled."; return 0; }

    # ---- Execute flash ------------------------------------------------------
    section "Flashing"
    echo
    echo -e "  ${DIM}Put device into bootloader mode if not already (hold BOOT/IO0, tap EN/RST)${RESET}"
    echo
    read -rp "  Press Enter when device is ready…" _

    # Derive chip ID from board name for --chip flag
    # python -m esptool accepts: esp32 esp32s2 esp32s3 esp32c3 esp32c6 esp32c5 auto
    local chip_id="auto"
    case "${BOARD^^}" in
        *S3*) chip_id="esp32s3" ;;
        *S2*) chip_id="esp32s2" ;;
        *C3*) chip_id="esp32c3" ;;
        *C5*) chip_id="esp32c5" ;;
        *C6*) chip_id="esp32c6" ;;
        *)    chip_id="esp32"   ;;
    esac

    # Build final esptool command array — exactly matching:
    # python -m esptool --chip esp32s3 -b 460800 --before default_reset
    #   --after hard_reset write_flash --flash_mode dio --flash_size 4MB
    #   --flash_freq 80m 0x0 bootloader.bin 0x8000 partition-table.bin
    #   0x10000 micropython.bin
    # Split ESPTOOL_CMD string into array for safe execution
    local -a esptool_base
    read -ra esptool_base <<< "$ESPTOOL_CMD"

    local -a esptool_cmd=(
        "${esptool_base[@]}"
        --chip   "$chip_id"
        -b       "$FLASH_BAUD"
        --before default_reset
        --after  hard_reset
        write_flash
        "${mode_args_arr[@]}"
        "${erase_arr[@]}"
        "${flash_file_args[@]}"
    )

    echo
    echo -e "  ${DIM}Running: ${esptool_cmd[*]}${RESET}"
    echo

    if "${esptool_cmd[@]}"; then
        echo
        info "Flash complete. Device is resetting…"
    else
        local rc=$?
        echo
        warn "esptool exited with code $rc."
        warn "Common causes:"
        warn "  • Device not in bootloader mode (hold BOOT/IO0 while pressing EN/RST)"
        warn "  • Wrong port selected"
        warn "  • USB cable is charge-only (no data lines)"
        warn "  • Port in use by another process (screen, minicom, IDE monitor)"
        warn "Firmware was built successfully. You can flash manually with:"
        echo
        echo "    ${esptool_cmd[*]}"
        echo
    fi
}

# =============================================================================
# MAIN FLOW
# =============================================================================

main() {
    # ---- Board selection (existing) ----------------------------------------
    banner
    choose_board

    # ---- Display selected board (existing) ----------------------------------
    banner
    if command -v figlet >/dev/null 2>&1; then
        figlet "$TARGET"
    fi
    echo
    echo "  Board : $BOARD"
    echo

    ask "Continue? (y/n):" OK
    [[ "$OK" =~ ^[Yy]$ ]] || exit 0

    # ---- Git branch check (existing) ----------------------------------------
    cd "$REPO"
    CURRENT_BRANCH=$(git branch --show-current)
    if [[ "$CURRENT_BRANCH" != "$DEV_BRANCH" ]]; then
        die "Not on dev branch. Current branch: $CURRENT_BRANCH. Switch with: git checkout $DEV_BRANCH"
    fi

    # ---- Repository tree check (existing) -----------------------------------
    section "Repository Tree"
    if command -v tree >/dev/null 2>&1; then
        tree -L 2 -d "$REPO"
    else
        find "$REPO" -maxdepth 2 -type d | sort
    fi
    echo
    ask "Is repository tree correct? (y/n):" TREE_OK
    [[ "$TREE_OK" =~ ^[Yy]$ ]] || exit 0

    # ---- NEW: Hardware variant selection ------------------------------------
    banner
    choose_variant "$BOARD"

    # ---- NEW: Feature selection ---------------------------------------------
    banner
    choose_features

    # ---- NEW: Config merge + validation ------------------------------------
    banner
    merge_configs
    validate_config

    # ---- NEW: sdkconfig generation -----------------------------------------
    generate_sdkconfig

    # ---- NEW: Optional menuconfig ------------------------------------------
    run_menuconfig_if_requested

    # ---- Clean build artifacts (existing) -----------------------------------
    section "Cleaning Build Environment"
    cd "$PORT_DIR"
    rm -rf build build-* managed_components
    find . -maxdepth 1 -name "sdkconfig.old" -delete
    # Note: we do NOT delete sdkconfig here — we just wrote it above
    info "Build artifacts cleaned."

    # ---- Final build confirmation -------------------------------------------
    echo
    echo "  Ready to build:"
    echo "  Board   : $BOARD"
    echo "  Preset  : $SELECTED_PRESET_ID"
    [[ "$FEATURE_PSRAM"    == "y" ]] && echo "  PSRAM   : enabled"
    [[ "$FEATURE_USB_CDC"  == "y" ]] && echo "  USB CDC : enabled"
    [[ "$FEATURE_USB_JTAG" == "y" ]] && echo "  USB JTAG: enabled"
    [[ "$FEATURE_BT"       == "y" ]] && echo "  BT      : enabled"
    [[ "$FEATURE_WIFI"     == "y" ]] && echo "  WiFi    : enabled"
    [[ "$FEATURE_OPT"      == "y" ]] && echo "  Opts    : enabled"
    echo
    ask "Build firmware now? (y/n):" BUILD_OK
    [[ "$BUILD_OK" =~ ^[Yy]$ ]] || exit 0

    # ---- Build (existing commands, UNCHANGED) --------------------------------
    run_build

    # ---- Display outputs (existing) -----------------------------------------
    show_outputs

    # ---- NEW: Optional flash stage ------------------------------------------
    run_flash_stage

    # ---- Cleanup (cleanup trap handles sdkconfig removal) -------------------
    # Remove our generated sdkconfig after successful build so the repo stays clean
    if [[ -n "$SDKCONFIG_OVERLAY" && -f "$SDKCONFIG_OVERLAY" ]]; then
        rm -f "$SDKCONFIG_OVERLAY"
        SDKCONFIG_OVERLAY=""   # prevent double-delete by trap
        info "Temporary sdkconfig removed."
    fi
    # Remove backup
    if [[ -n "$SDKCONFIG_BACKUP" && -f "$SDKCONFIG_BACKUP" ]]; then
        rm -f "$SDKCONFIG_BACKUP"
        SDKCONFIG_BACKUP=""
    fi
}

main "$@"
