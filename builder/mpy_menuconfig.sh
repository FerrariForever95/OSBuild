#!/usr/bin/env bash
# =============================================================================
# mpy_menuconfig.sh — MicroPython Feature Configuration Manager
# ZENO OS Build System
# =============================================================================
#
# PURPOSE:
#   Interactive menu-driven configuration of MicroPython build options.
#   Generates .config/mpyconfig and .config/mpyconfig.mk consumed by
#   the existing Makefile build workflow. Does NOT build firmware.
#
# USAGE:
#   ./mpy_menuconfig.sh [--board <BOARD>] [--preset <PRESET_ID>] [--batch]
#
# OUTPUT:
#   .config/mpyconfig      — Kconfig-style key=value store
#   .config/mpyconfig.mk   — Makefile-include fragment (MICROPY_PY_* flags)
#
# INTEGRATION:
#   zeno_build.sh calls this script instead of 'make menuconfig BOARD=...'
#   Then reads .config/mpyconfig.mk via: FROZEN_MANIFEST or CFLAGS_EXTRA
#
# ARCHITECTURE:
#   Menu pages are registered in MENU_PAGES[] and dispatched by show_menu().
#   Each page is a function: page_<name>().
#   Config state lives in CONFIG[] associative array (key → "y"/"n"/value).
#   Presets are associative arrays merged into CONFIG[] at load time.
#   save_config() / load_config() persist CONFIG[] to .config/mpyconfig.
#   generate_makefile() translates CONFIG[] → MICROPY_PY_* Makefile flags.
#   scan_modules() discovers lib/ modules/ extmod/ and registers them.
#
# =============================================================================

set -euo pipefail
# shellcheck disable=SC2034  # Preset arrays accessed via nameref — SC2034 false positive

# =============================================================================
# 0. CONSTANTS & PATHS
# =============================================================================

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO="${REPO:-$SCRIPT_DIR}"

CONFIG_DIR="$REPO/.config"
CONFIG_FILE="$CONFIG_DIR/mpyconfig"
CONFIG_MK="$CONFIG_DIR/mpyconfig.mk"
CONFIG_BACKUP="$CONFIG_DIR/mpyconfig.bak"
SCAN_CACHE="$CONFIG_DIR/module_scan.cache"

# Paths to scan for discoverable modules
LIB_DIR="$REPO/lib"
MODULES_DIR="$REPO/modules"
EXTMOD_DIR="$REPO/extmod"

# UI backend: auto-detected
UI_BACKEND=""   # "dialog" | "whiptail" | "text"

# Terminal dimensions (updated by detect_term_size)
TERM_ROWS=24
TERM_COLS=80

# Dialog/whiptail menu box sizing
BOX_H=20
BOX_W=72
LIST_H=14

# Colour codes (text-mode fallback)
RED='\033[0;31m'; YELLOW='\033[1;33m'; GREEN='\033[0;32m'
CYAN='\033[0;36m'; BLUE='\033[0;34m'; BOLD='\033[1m'
DIM='\033[2m'; REVERSE='\033[7m'; RESET='\033[0m'

# Exit codes
EXIT_OK=0
EXIT_CANCEL=1
EXIT_ERROR=2

# =============================================================================
# 1. CONFIG STATE — single source of truth
# =============================================================================
# All configuration lives here.  Keys use MICROPY_PY_* or CONFIG_* prefixes
# matching MicroPython's own Makefile variable names.
# Values: "y" (enabled), "n" (disabled), or a literal string for non-boolean.
# =============================================================================

declare -A CONFIG=()

# Metadata: human-readable labels and help text for each key
declare -A CFG_LABEL=()
declare -A CFG_HELP=()
declare -A CFG_TYPE=()   # "bool" | "string" | "choice"

# Discovered module keys (populated by scan_modules)
declare -a DISCOVERED_MODULES=()

# Menu page registry
declare -a MENU_PAGES=()       # ordered list of page IDs
declare -A MENU_PAGE_LABEL=()  # page ID → display label

# =============================================================================
# 2. DEFAULT CONFIG SCHEMA
# =============================================================================
# Defines every known config key with its default, label, help, and type.
# register_key KEY DEFAULT LABEL TYPE "HELP TEXT"
# =============================================================================

register_key() {
    local key="$1" default="$2" label="$3" type="$4" help="$5"
    [[ -z "${CONFIG[$key]+x}" ]] && CONFIG[$key]="$default"
    CFG_LABEL[$key]="$label"
    CFG_TYPE[$key]="$type"
    CFG_HELP[$key]="$help"
}

define_schema() {
    # ── Core Features ─────────────────────────────────────────────────────────
    register_key "CONFIG_BLUETOOTH"       "n" "Bluetooth"              "bool"   "Enable Bluetooth (BLE/Classic) support"
    register_key "CONFIG_WIFI"            "y" "WiFi"                   "bool"   "Enable WiFi networking support"
    register_key "CONFIG_REPL"            "y" "REPL"                   "bool"   "Enable interactive REPL over UART/USB"
    register_key "CONFIG_USB_CDC"         "n" "USB CDC"                "bool"   "Enable USB CDC serial (requires USB OTG hardware)"
    register_key "CONFIG_USB_SERIAL_JTAG" "n" "USB Serial/JTAG"       "bool"   "Enable USB Serial/JTAG console (C3/C6/S3)"
    register_key "CONFIG_MDNS"            "n" "mDNS"                   "bool"   "Enable mDNS/DNS-SD service discovery"
    register_key "CONFIG_WEBREPL"         "n" "WebREPL"                "bool"   "Enable WebREPL (REPL over WebSocket)"
    register_key "CONFIG_FILESYSTEM"      "y" "Filesystem Support"     "bool"   "Enable VFS / LittleFS / FAT filesystem"
    register_key "CONFIG_NETWORK_STACK"   "y" "Network Stack"          "bool"   "Enable lwIP network stack"
    register_key "CONFIG_SSL_TLS"         "n" "SSL/TLS"                "bool"   "Enable mbedTLS SSL/TLS support"

    # ── Python Libraries ──────────────────────────────────────────────────────
    register_key "MICROPY_PY_UJSON"       "y" "ujson"                  "bool"   "JSON encode/decode (micropython-lib)"
    register_key "MICROPY_PY_UREQUESTS"   "n" "urequests"              "bool"   "HTTP client library"
    register_key "MICROPY_PY_UZLIB"       "n" "uzlib"                  "bool"   "zlib/deflate decompression"
    register_key "MICROPY_PY_UASYNCIO"    "y" "uasyncio"               "bool"   "Async/await event loop (uasyncio v3)"
    register_key "MICROPY_PY_SSL"         "n" "ssl"                    "bool"   "ssl module (wraps mbedTLS)"
    register_key "MICROPY_PY_HASHLIB"     "y" "hashlib"                "bool"   "SHA256/MD5 hash functions"
    register_key "MICROPY_PY_FRAMEBUF"    "n" "framebuf"               "bool"   "Framebuffer for pixel display drivers"
    register_key "MICROPY_PY_MACHINE"     "y" "machine"                "bool"   "Hardware abstraction (GPIO, I2C, SPI, …)"
    register_key "MICROPY_PY_NETWORK"     "y" "network"                "bool"   "network module (WLAN, Ethernet)"
    register_key "MICROPY_PY_SOCKET"      "y" "socket"                 "bool"   "socket module (BSD sockets via lwIP)"
    register_key "MICROPY_PY_SELECT"      "y" "select"                 "bool"   "select/poll for I/O multiplexing"
    register_key "MICROPY_PY_THREADING"   "n" "threading"              "bool"   "_thread / threading primitives"

    # ── Built-in Modules (machine sub-features) ───────────────────────────────
    register_key "MICROPY_PY_MACHINE_ADC" "y" "ADC"                    "bool"   "Analogue-to-digital converter"
    register_key "MICROPY_PY_MACHINE_DAC" "n" "DAC"                    "bool"   "Digital-to-analogue converter (ESP32 only)"
    register_key "MICROPY_PY_MACHINE_SPI" "y" "SPI"                    "bool"   "SPI bus master"
    register_key "MICROPY_PY_MACHINE_I2C" "y" "I2C"                    "bool"   "I2C bus master"
    register_key "MICROPY_PY_MACHINE_UART" "y" "UART"                  "bool"   "UART serial port"
    register_key "MICROPY_PY_MACHINE_PWM" "y" "PWM"                    "bool"   "PWM output (LEDC peripheral)"
    register_key "MICROPY_PY_MACHINE_RMT" "n" "RMT"                    "bool"   "RMT (Remote Control / IR) peripheral"
    register_key "MICROPY_PY_MACHINE_TOUCH" "n" "Touch"                "bool"   "Capacitive touch sensor (ESP32/S2/S3)"
    register_key "MICROPY_PY_MACHINE_CAN"  "n" "CAN / TWAI"           "bool"   "CAN bus / TWAI controller"
    register_key "MICROPY_PY_MACHINE_USB"  "n" "USB (machine.USB)"    "bool"   "machine.USB device API"
    register_key "MICROPY_PY_MACHINE_SDCARD" "n" "SD Card"            "bool"   "machine.SDCard block device"

    # ── ESP32-Specific ────────────────────────────────────────────────────────
    register_key "CONFIG_PSRAM"             "n"      "PSRAM"                "bool"   "Enable external PSRAM"
    register_key "CONFIG_FLASH_SIZE"        "4MB"    "Flash Size"           "choice" "Flash memory size: 4MB 8MB 16MB"
    register_key "CONFIG_FLASH_FREQ"        "80m"    "Flash Frequency"      "choice" "Flash SPI frequency: 40m 80m"
    register_key "CONFIG_CPU_FREQ"          "240"    "CPU Frequency (MHz)"  "choice" "CPU speed: 80 160 240"
    register_key "CONFIG_PARTITION_TABLE"   "default" "Partition Table"     "choice" "default custom ota"
    register_key "CONFIG_BT_MEMORY_KB"      "64"     "Bluetooth Memory (KB)" "string" "Heap allocated to BT controller"
    register_key "CONFIG_WIFI_RX_BUF"       "10"     "WiFi RX Buffer Count"  "string" "Static RX buffer count for WiFi"
    register_key "CONFIG_WIFI_TX_BUF"       "10"     "WiFi TX Buffer Count"  "string" "Dynamic TX buffer count for WiFi"

    # ── Optimisation ──────────────────────────────────────────────────────────
    register_key "CONFIG_BUILD_TYPE"        "release" "Build Type"          "choice" "debug release size performance"
    register_key "CONFIG_MICROPY_DEBUG_PRNT" "n"      "Debug Printf"        "bool"   "Enable mp_printf debug output"
    register_key "CONFIG_NATIVE_CODE"        "n"      "Native Emitters"     "bool"   "Enable @micropython.native decorator"
    register_key "CONFIG_VIPER_CODE"         "n"      "Viper Emitters"      "bool"   "Enable @micropython.viper decorator"
    register_key "CONFIG_LTO"                "n"      "Link-Time Optimise"  "bool"   "Enable LTO during link (slower build)"
}

# =============================================================================
# 3. PRESETS
# =============================================================================

declare -A PRESET_ESP32S3_N16R8=(
    [CONFIG_PSRAM]="y"
    [CONFIG_FLASH_SIZE]="16MB"
    [CONFIG_FLASH_FREQ]="80m"
    [CONFIG_CPU_FREQ]="240"
    [CONFIG_USB_CDC]="y"
    [CONFIG_WIFI]="y"
    [CONFIG_NETWORK_STACK]="y"
    [CONFIG_FILESYSTEM]="y"
    [CONFIG_SSL_TLS]="y"
    [CONFIG_BUILD_TYPE]="release"
    [MICROPY_PY_UASYNCIO]="y"
    [MICROPY_PY_UJSON]="y"
    [MICROPY_PY_HASHLIB]="y"
    [MICROPY_PY_SSL]="y"
    [MICROPY_PY_MACHINE]="y"
    [MICROPY_PY_MACHINE_SPI]="y"
    [MICROPY_PY_MACHINE_I2C]="y"
    [MICROPY_PY_MACHINE_UART]="y"
    [MICROPY_PY_MACHINE_ADC]="y"
    [MICROPY_PY_MACHINE_PWM]="y"
    [MICROPY_PY_NETWORK]="y"
    [MICROPY_PY_SOCKET]="y"
    [MICROPY_PY_SELECT]="y"
)

declare -A PRESET_ESP32S3_N16R16=(
    [CONFIG_PSRAM]="y"
    [CONFIG_FLASH_SIZE]="16MB"
    [CONFIG_FLASH_FREQ]="80m"
    [CONFIG_CPU_FREQ]="240"
    [CONFIG_USB_CDC]="y"
    [CONFIG_WIFI]="y"
    [CONFIG_NETWORK_STACK]="y"
    [CONFIG_FILESYSTEM]="y"
    [CONFIG_SSL_TLS]="y"
    [CONFIG_BUILD_TYPE]="release"
    [MICROPY_PY_UASYNCIO]="y"
    [MICROPY_PY_UJSON]="y"
    [MICROPY_PY_HASHLIB]="y"
    [MICROPY_PY_SSL]="y"
    [MICROPY_PY_MACHINE]="y"
    [MICROPY_PY_MACHINE_SPI]="y"
    [MICROPY_PY_MACHINE_I2C]="y"
    [MICROPY_PY_MACHINE_UART]="y"
    [MICROPY_PY_MACHINE_ADC]="y"
    [MICROPY_PY_MACHINE_PWM]="y"
    [MICROPY_PY_NETWORK]="y"
    [MICROPY_PY_SOCKET]="y"
    [MICROPY_PY_SELECT]="y"
    [MICROPY_PY_THREADING]="y"
)

declare -A PRESET_ESP32S3_N8R8=(
    [CONFIG_PSRAM]="y"
    [CONFIG_FLASH_SIZE]="8MB"
    [CONFIG_FLASH_FREQ]="80m"
    [CONFIG_CPU_FREQ]="240"
    [CONFIG_USB_CDC]="y"
    [CONFIG_WIFI]="y"
    [CONFIG_NETWORK_STACK]="y"
    [CONFIG_FILESYSTEM]="y"
    [CONFIG_BUILD_TYPE]="size"
    [MICROPY_PY_UASYNCIO]="y"
    [MICROPY_PY_UJSON]="y"
    [MICROPY_PY_MACHINE]="y"
    [MICROPY_PY_MACHINE_SPI]="y"
    [MICROPY_PY_MACHINE_I2C]="y"
    [MICROPY_PY_MACHINE_UART]="y"
    [MICROPY_PY_NETWORK]="y"
    [MICROPY_PY_SOCKET]="y"
)

declare -A PRESET_ESP32S3_N8=(
    [CONFIG_PSRAM]="n"
    [CONFIG_FLASH_SIZE]="8MB"
    [CONFIG_FLASH_FREQ]="80m"
    [CONFIG_CPU_FREQ]="240"
    [CONFIG_USB_CDC]="y"
    [CONFIG_WIFI]="y"
    [CONFIG_NETWORK_STACK]="y"
    [CONFIG_FILESYSTEM]="y"
    [CONFIG_BUILD_TYPE]="size"
    [MICROPY_PY_UASYNCIO]="y"
    [MICROPY_PY_UJSON]="y"
    [MICROPY_PY_MACHINE]="y"
    [MICROPY_PY_NETWORK]="y"
    [MICROPY_PY_SOCKET]="y"
)

declare -A PRESET_ESP32C3=(
    [CONFIG_PSRAM]="n"
    [CONFIG_FLASH_SIZE]="4MB"
    [CONFIG_FLASH_FREQ]="80m"
    [CONFIG_CPU_FREQ]="160"
    [CONFIG_USB_SERIAL_JTAG]="y"
    [CONFIG_BLUETOOTH]="y"
    [CONFIG_WIFI]="y"
    [CONFIG_NETWORK_STACK]="y"
    [CONFIG_FILESYSTEM]="y"
    [CONFIG_BUILD_TYPE]="size"
    [MICROPY_PY_UASYNCIO]="y"
    [MICROPY_PY_UJSON]="y"
    [MICROPY_PY_MACHINE]="y"
    [MICROPY_PY_MACHINE_SPI]="y"
    [MICROPY_PY_MACHINE_I2C]="y"
    [MICROPY_PY_MACHINE_UART]="y"
    [MICROPY_PY_NETWORK]="y"
    [MICROPY_PY_SOCKET]="y"
)

declare -A PRESET_ESP32C6=(
    [CONFIG_PSRAM]="n"
    [CONFIG_FLASH_SIZE]="4MB"
    [CONFIG_FLASH_FREQ]="80m"
    [CONFIG_CPU_FREQ]="160"
    [CONFIG_USB_SERIAL_JTAG]="y"
    [CONFIG_BLUETOOTH]="y"
    [CONFIG_WIFI]="y"
    [CONFIG_NETWORK_STACK]="y"
    [CONFIG_FILESYSTEM]="y"
    [CONFIG_BUILD_TYPE]="size"
    [MICROPY_PY_UASYNCIO]="y"
    [MICROPY_PY_UJSON]="y"
    [MICROPY_PY_MACHINE]="y"
    [MICROPY_PY_MACHINE_SPI]="y"
    [MICROPY_PY_MACHINE_I2C]="y"
    [MICROPY_PY_MACHINE_UART]="y"
    [MICROPY_PY_NETWORK]="y"
    [MICROPY_PY_SOCKET]="y"
)

# Map preset ID → array name
declare -A PRESET_MAP=(
    [ESP32S3_N16R8]="PRESET_ESP32S3_N16R8"
    [ESP32S3_N16R16]="PRESET_ESP32S3_N16R16"
    [ESP32S3_N8R8]="PRESET_ESP32S3_N8R8"
    [ESP32S3_N8]="PRESET_ESP32S3_N8"
    [ESP32C3]="PRESET_ESP32C3"
    [ESP32C6]="PRESET_ESP32C6"
)

apply_preset() {
    local preset_id="$1"
    local array_name="${PRESET_MAP[$preset_id]:-}"
    if [[ -z "$array_name" ]]; then
        ui_warn "Unknown preset: $preset_id"
        return 1
    fi
    local -n src="$array_name"
    for key in "${!src[@]}"; do
        CONFIG[$key]="${src[$key]}"
    done
    return 0
}

# =============================================================================
# 4. UI BACKEND — dialog / whiptail / text fallback
# =============================================================================

detect_ui_backend() {
    if command -v dialog >/dev/null 2>&1; then
        UI_BACKEND="dialog"
    elif command -v whiptail >/dev/null 2>&1; then
        UI_BACKEND="whiptail"
    else
        UI_BACKEND="text"
    fi
}

detect_term_size() {
    if command -v tput >/dev/null 2>&1; then
        TERM_ROWS=$(tput lines  2>/dev/null || echo 24)
        TERM_COLS=$(tput cols   2>/dev/null || echo 80)
    fi
    BOX_H=$(( TERM_ROWS - 4 ))
    BOX_W=$(( TERM_COLS - 8 ))
    LIST_H=$(( BOX_H - 7 ))
    [[ $BOX_H  -lt 10 ]] && BOX_H=10
    [[ $BOX_W  -lt 50 ]] && BOX_W=50
    [[ $LIST_H -lt 4  ]] && LIST_H=4
}

# ---------------------------------------------------------------------------
# UI primitives — abstract over dialog/whiptail/text
# ---------------------------------------------------------------------------

ui_clear() { [[ "$UI_BACKEND" == "text" ]] && clear || true; }

ui_info() {
    local msg="$1"
    case "$UI_BACKEND" in
        dialog)   dialog --msgbox "$msg" 8 60 ;;
        whiptail) whiptail --msgbox "$msg" 8 60 ;;
        text)     echo -e "  ${GREEN}✔${RESET}  $msg" ;;
    esac
}

ui_warn() {
    local msg="$1"
    case "$UI_BACKEND" in
        dialog)   dialog --msgbox "WARNING: $msg" 8 60 ;;
        whiptail) whiptail --msgbox "WARNING: $msg" 8 60 ;;
        text)     echo -e "  ${YELLOW}⚠${RESET}  $msg" ;;
    esac
}

ui_error() {
    local msg="$1"
    case "$UI_BACKEND" in
        dialog)   dialog --msgbox "ERROR: $msg" 8 60 ;;
        whiptail) whiptail --msgbox "ERROR: $msg" 8 60 ;;
        text)     echo -e "\n  ${RED}✘  ERROR: $msg${RESET}\n" ;;
    esac
}

# Returns 0 for Yes, 1 for No
ui_yesno() {
    local msg="$1" title="${2:-Confirm}"
    case "$UI_BACKEND" in
        dialog)
            dialog --title "$title" --yesno "$msg" 8 60
            return $?
            ;;
        whiptail)
            whiptail --title "$title" --yesno "$msg" 8 60
            return $?
            ;;
        text)
            local ans
            read -rp "$(echo -e "  ${BOLD}$msg (y/n):${RESET} ")" ans
            [[ "${ans,,}" =~ ^y ]] && return 0 || return 1
            ;;
    esac
}

# Show a simple menu; echoes selected item tag to stdout
# ui_menu TITLE ITEMS_ARRAY_NAME
ui_menu() {
    local title="$1"
    shift
    local -a items=("$@")
    local result

    case "$UI_BACKEND" in
        dialog)
            result=$(dialog --stdout --title "$title" \
                --menu "$title" "$BOX_H" "$BOX_W" "$LIST_H" \
                "${items[@]}" 2>&1)
            echo "$result"
            ;;
        whiptail)
            result=$(whiptail --title "$title" \
                --menu "$title" "$BOX_H" "$BOX_W" "$LIST_H" \
                "${items[@]}" 3>&1 1>&2 2>&3)
            echo "$result"
            ;;
        text)
            _text_menu "$title" "${items[@]}"
            ;;
    esac
}

_text_menu() {
    local title="$1"; shift
    local -a items=("$@")
    local -a tags=() labels=()
    local i=0
    while (( i < ${#items[@]} )); do
        tags+=("${items[$i]}")
        labels+=("${items[$((i+1))]}")
        (( i+=2 ))
    done

    echo
    echo -e "${CYAN}${BOLD}  ── $title ──${RESET}"
    echo
    local n
    for n in "${!tags[@]}"; do
        printf "    ${BOLD}%2d)${RESET}  %s\n" "$((n+1))" "${labels[$n]}"
    done
    echo
    local choice
    read -rp "$(echo -e "  ${BOLD}Select [1-${#tags[@]}]:${RESET} ")" choice
    if [[ "$choice" =~ ^[0-9]+$ ]] && (( choice >= 1 && choice <= ${#tags[@]} )); then
        echo "${tags[$((choice-1))]}"
    else
        echo ""
    fi
}

# Checklist: items are TAG LABEL STATUS triples; returns space-separated enabled tags
ui_checklist() {
    local title="$1"; shift
    local -a items=("$@")
    local result

    case "$UI_BACKEND" in
        dialog)
            result=$(dialog --stdout --title "$title" \
                --checklist "$title" "$BOX_H" "$BOX_W" "$LIST_H" \
                "${items[@]}" 2>&1)
            echo "$result"
            ;;
        whiptail)
            result=$(whiptail --title "$title" \
                --checklist "$title" "$BOX_H" "$BOX_W" "$LIST_H" \
                "${items[@]}" 3>&1 1>&2 2>&3)
            echo "$result"
            ;;
        text)
            _text_checklist "$title" "${items[@]}"
            ;;
    esac
}

_text_checklist() {
    local title="$1"; shift
    local -a items=("$@")
    local -a tags=() labels=() states=()
    local i=0
    while (( i < ${#items[@]} )); do
        tags+=("${items[$i]}")
        labels+=("${items[$((i+1))]}")
        states+=("${items[$((i+2))]}")
        (( i+=3 ))
    done

    echo
    echo -e "${CYAN}${BOLD}  ── $title ──${RESET}"
    echo -e "  ${DIM}(Space to toggle, Enter to confirm. Current state shown as [*]/[ ])${RESET}"
    echo

    # Copy states into editable array
    local -a cur=("${states[@]}")
    local n
    for n in "${!tags[@]}"; do
        local mark=" "
        [[ "${cur[$n]}" == "on" ]] && mark="*"
        printf "    %2d) [%s]  %s\n" "$((n+1))" "$mark" "${labels[$n]}"
    done
    echo
    echo -e "  ${DIM}Enter numbers to toggle (space-separated), then press Enter to confirm:${RESET}"
    local input
    read -rp "  Toggle: " input
    for tok in $input; do
        if [[ "$tok" =~ ^[0-9]+$ ]] && (( tok >= 1 && tok <= ${#tags[@]} )); then
            local idx=$((tok-1))
            [[ "${cur[$idx]}" == "on" ]] && cur[$idx]="off" || cur[$idx]="on"
        fi
    done

    local -a enabled=()
    for n in "${!tags[@]}"; do
        [[ "${cur[$n]}" == "on" ]] && enabled+=("\"${tags[$n]}\"")
    done
    echo "${enabled[*]}"
}

# Input box — returns entered string
ui_inputbox() {
    local title="$1" prompt="$2" default="${3:-}"
    local result

    case "$UI_BACKEND" in
        dialog)
            result=$(dialog --stdout --title "$title" \
                --inputbox "$prompt" 8 60 "$default")
            echo "$result"
            ;;
        whiptail)
            result=$(whiptail --title "$title" \
                --inputbox "$prompt" 8 60 "$default" 3>&1 1>&2 2>&3)
            echo "$result"
            ;;
        text)
            local ans
            read -rp "$(echo -e "  ${BOLD}$prompt [$default]:${RESET} ")" ans
            echo "${ans:-$default}"
            ;;
    esac
}

# =============================================================================
# 5. CONFIG SAVE / LOAD
# =============================================================================

save_config() {
    mkdir -p "$CONFIG_DIR"

    # Backup previous config
    [[ -f "$CONFIG_FILE" ]] && cp "$CONFIG_FILE" "$CONFIG_BACKUP"

    {
        echo "# mpy_menuconfig — MicroPython Feature Configuration"
        echo "# Generated: $(date -u '+%Y-%m-%dT%H:%M:%SZ')"
        echo "# Board: ${ACTIVE_BOARD:-unset}"
        echo "# Preset: ${ACTIVE_PRESET_ID:-custom}"
        echo "#"
        echo "# DO NOT EDIT — regenerated by mpy_menuconfig.sh"
        echo ""
        for key in $(printf '%s\n' "${!CONFIG[@]}" | sort); do
            local val="${CONFIG[$key]}"
            if [[ "$val" == "n" && "${CFG_TYPE[$key]:-bool}" == "bool" ]]; then
                echo "# ${key} is not set"
            else
                echo "${key}=${val}"
            fi
        done
    } > "$CONFIG_FILE"
}

load_config() {
    [[ ! -f "$CONFIG_FILE" ]] && return 0

    local lineno=0
    while IFS= read -r line || [[ -n "$line" ]]; do
        (( lineno++ )) || true
        # Skip blank lines and pure comments (but not "# KEY is not set")
        [[ -z "$line" ]] && continue
        if [[ "$line" =~ ^#\ (.+)\ is\ not\ set$ ]]; then
            local key="${BASH_REMATCH[1]}"
            CONFIG[$key]="n"
            continue
        fi
        [[ "$line" =~ ^# ]] && continue
        if [[ "$line" =~ ^([A-Za-z0-9_]+)=(.*)$ ]]; then
            CONFIG["${BASH_REMATCH[1]}"]="${BASH_REMATCH[2]}"
        fi
    done < "$CONFIG_FILE"
}

# =============================================================================
# 6. MAKEFILE GENERATION
# =============================================================================
# Translates CONFIG[] → two output files:
#   .config/mpyconfig      — Kconfig-style (already written by save_config)
#   .config/mpyconfig.mk   — Makefile include fragment
#
# Mapping rules:
#   MICROPY_PY_* keys → MICROPY_PY_*=1 / MICROPY_PY_*=0
#   CONFIG_BUILD_TYPE → CFLAGS_EXTRA / compiler flags
#   CONFIG_FLASH_*    → FLASH_SIZE, FLASH_FREQ vars
#   CONFIG_CPU_FREQ   → MICROPY_HW_MCU_FREQ
# =============================================================================

generate_makefile() {
    mkdir -p "$CONFIG_DIR"

    {
        echo "# mpy_menuconfig — Makefile Fragment"
        echo "# Generated: $(date -u '+%Y-%m-%dT%H:%M:%SZ')"
        echo "# Source: $CONFIG_FILE"
        echo "#"
        echo "# Include in your Makefile with:"
        echo "#   -include \$(BUILD)/../.config/mpyconfig.mk"
        echo "# or pass as: make BOARD=... CFLAGS_EXTRA=\"\$(shell cat .config/mpyconfig.mk | grep CFLAGS)\""
        echo ""

        # ── Python library flags ───────────────────────────────────────────
        echo "# Python libraries"
        local key
        for key in $(printf '%s\n' "${!CONFIG[@]}" | grep '^MICROPY_PY_' | sort); do
            local val="${CONFIG[$key]}"
            if [[ "$val" == "y" ]]; then
                echo "${key}=1"
            elif [[ "$val" == "n" ]]; then
                echo "# ${key}=0"
            fi
        done
        echo ""

        # ── Build type → CFLAGS_EXTRA ─────────────────────────────────────
        echo "# Build optimisation"
        local build_type="${CONFIG[CONFIG_BUILD_TYPE]:-release}"
        case "$build_type" in
            debug)
                echo "CFLAGS_EXTRA += -Og -g3 -DMICROPY_DEBUG_PRNT=1"
                echo "MICROPY_DEBUG_PRNT=1"
                ;;
            release)
                echo "CFLAGS_EXTRA += -O2"
                ;;
            size)
                echo "CFLAGS_EXTRA += -Os -DNDEBUG"
                echo "MICROPY_OPT_COMPUTED_GOTO=1"
                ;;
            performance)
                echo "CFLAGS_EXTRA += -O3 -fomit-frame-pointer"
                echo "MICROPY_OPT_COMPUTED_GOTO=1"
                echo "MICROPY_ENABLE_COMPILER=1"
                ;;
        esac
        echo ""

        # ── Native / Viper emitters ────────────────────────────────────────
        echo "# Code emitters"
        _mk_bool "CONFIG_NATIVE_CODE" "MICROPY_EMIT_XTENSAWIN"
        _mk_bool "CONFIG_VIPER_CODE"  "MICROPY_EMIT_VIPER"
        _mk_bool "CONFIG_LTO"         "LTO=1"
        echo ""

        # ── ESP32 hardware flags ───────────────────────────────────────────
        echo "# ESP32 hardware"
        local flash_size="${CONFIG[CONFIG_FLASH_SIZE]:-4MB}"
        local flash_freq="${CONFIG[CONFIG_FLASH_FREQ]:-80m}"
        local cpu_freq="${CONFIG[CONFIG_CPU_FREQ]:-240}"
        echo "FLASH_SIZE ?= $flash_size"
        echo "FLASH_FREQ ?= $flash_freq"
        echo "MICROPY_HW_MCU_FREQ ?= ${cpu_freq}000000"
        echo ""

        # ── Feature presence flags ─────────────────────────────────────────
        echo "# Feature flags"
        _mk_bool "CONFIG_WIFI"            "MICROPY_PY_NETWORK_WLAN=1"
        _mk_bool "CONFIG_BLUETOOTH"       "MICROPY_BLUETOOTH_NIMBLE=1"
        _mk_bool "CONFIG_WEBREPL"         "MICROPY_PY_WEBREPL=1"
        _mk_bool "CONFIG_MDNS"            "MICROPY_PY_ESPNOW=0"  # placeholder
        _mk_bool "CONFIG_SSL_TLS"         "MICROPY_SSL_MBEDTLS=1"
        _mk_bool "CONFIG_FILESYSTEM"      "MICROPY_VFS=1"
        _mk_bool "CONFIG_USB_CDC"         "MICROPY_HW_USB_CDC=1"
        _mk_bool "CONFIG_USB_SERIAL_JTAG" "MICROPY_HW_USB_SERIAL_JTAG=1"
        echo ""

        # ── Discovered / scanned modules ──────────────────────────────────
        if [[ ${#DISCOVERED_MODULES[@]} -gt 0 ]]; then
            echo "# Discovered external modules"
            for mod in "${DISCOVERED_MODULES[@]}"; do
                local mk_key="MICROPY_MODULE_${mod^^}"
                local cfg_key="MODULE_${mod^^}"
                if [[ "${CONFIG[$cfg_key]:-n}" == "y" ]]; then
                    echo "${mk_key}=1"
                fi
            done
            echo ""
        fi

    } > "$CONFIG_MK"
}

# Helper: emit a Makefile line only when CONFIG key is "y"
_mk_bool() {
    local cfg_key="$1" mk_line="$2"
    local val="${CONFIG[$cfg_key]:-n}"
    [[ "$val" == "y" ]] && echo "$mk_line"
    return 0
}

# =============================================================================
# 7. MODULE SCANNER
# =============================================================================
# Scans lib/ modules/ extmod/ for directories/files that look like optional
# MicroPython modules, registers them in CONFIG and DISCOVERED_MODULES.
# Results cached in .config/module_scan.cache (invalidated by mtime check).
# =============================================================================

scan_modules() {
    DISCOVERED_MODULES=()

    local dirs=()
    [[ -d "$LIB_DIR"     ]] && dirs+=("$LIB_DIR")
    [[ -d "$MODULES_DIR" ]] && dirs+=("$MODULES_DIR")
    [[ -d "$EXTMOD_DIR"  ]] && dirs+=("$EXTMOD_DIR")

    if [[ ${#dirs[@]} -eq 0 ]]; then
        return 0
    fi

    # Check if cache is still valid (newer than any dir)
    local use_cache=0
    if [[ -f "$SCAN_CACHE" ]]; then
        use_cache=1
        for d in "${dirs[@]}"; do
            if [[ "$d" -nt "$SCAN_CACHE" ]]; then
                use_cache=0
                break
            fi
        done
    fi

    if (( use_cache )); then
        mapfile -t DISCOVERED_MODULES < "$SCAN_CACHE"
        # Register schema entries for cached modules
        for mod in "${DISCOVERED_MODULES[@]}"; do
            local cfg_key="MODULE_${mod^^}"
            register_key "$cfg_key" "n" "Module: $mod" "bool" "External module found in lib/modules/extmod"
        done
        return 0
    fi

    # Scan: collect candidate module names
    local -a found=()
    local d item name

    for d in "${dirs[@]}"; do
        # Top-level .py files
        while IFS= read -r item; do
            name="$(basename "$item" .py)"
            # Filter: skip __init__, test files, micropython-lib internals
            [[ "$name" =~ ^__ ]]      && continue
            [[ "$name" =~ ^test ]]    && continue
            [[ "$name" =~ _test$ ]]   && continue
            [[ "$name" == "setup" ]]  && continue
            found+=("$name")
        done < <(find "$d" -maxdepth 1 -name '*.py' -not -name '__*' 2>/dev/null | sort)

        # Top-level directories that contain __init__.py or module.c
        while IFS= read -r item; do
            name="$(basename "$item")"
            [[ "$name" =~ ^__ ]]   && continue
            [[ "$name" =~ ^test ]] && continue
            [[ "$name" == "."  ]]  && continue
            if [[ -f "$item/__init__.py" || -f "$item/module.c" || -f "$item/${name}.py" ]]; then
                found+=("$name")
            fi
        done < <(find "$d" -maxdepth 1 -mindepth 1 -type d 2>/dev/null | sort)
    done

    # Deduplicate
    local -A seen=()
    local -a deduped=()
    for name in "${found[@]}"; do
        if [[ -z "${seen[$name]+x}" ]]; then
            seen[$name]=1
            deduped+=("$name")
        fi
    done

    DISCOVERED_MODULES=("${deduped[@]}")

    # Write cache
    mkdir -p "$CONFIG_DIR"
    printf '%s\n' "${DISCOVERED_MODULES[@]}" > "$SCAN_CACHE"

    # Register schema entries
    for mod in "${DISCOVERED_MODULES[@]}"; do
        local cfg_key="MODULE_${mod^^}"
        register_key "$cfg_key" "n" "Module: $mod" "bool" "Discovered in lib/modules/extmod"
    done
}

# =============================================================================
# 8. MENU PAGES
# =============================================================================

# ---------------------------------------------------------------------------
# Register menu pages
# ---------------------------------------------------------------------------
register_pages() {
    MENU_PAGES=()
    MENU_PAGE_LABEL=()

    MENU_PAGES+=("preset")        MENU_PAGE_LABEL[preset]="Load Preset"
    MENU_PAGES+=("core")          MENU_PAGE_LABEL[core]="Core Features"
    MENU_PAGES+=("python_libs")   MENU_PAGE_LABEL[python_libs]="Python Libraries"
    MENU_PAGES+=("builtin_mods")  MENU_PAGE_LABEL[builtin_mods]="Built-in Modules (machine.*)"
    MENU_PAGES+=("esp32")         MENU_PAGE_LABEL[esp32]="ESP32-Specific Settings"
    MENU_PAGES+=("optimisation")  MENU_PAGE_LABEL[optimisation]="Optimisation & Build Type"

    if [[ ${#DISCOVERED_MODULES[@]} -gt 0 ]]; then
        MENU_PAGES+=("discovered")  MENU_PAGE_LABEL[discovered]="Discovered External Modules"
    fi

    MENU_PAGES+=("summary")       MENU_PAGE_LABEL[summary]="View Current Configuration"
    MENU_PAGES+=("save_exit")     MENU_PAGE_LABEL[save_exit]="Save & Exit"
    MENU_PAGES+=("exit_nosave")   MENU_PAGE_LABEL[exit_nosave]="Exit Without Saving"
}

# ---------------------------------------------------------------------------
# Top-level menu
# ---------------------------------------------------------------------------
show_main_menu() {
    while true; do
        local -a items=()
        for page in "${MENU_PAGES[@]}"; do
            local label="${MENU_PAGE_LABEL[$page]}"
            # Append dirty marker if config has been modified
            items+=("$page" "$label")
        done

        local choice
        choice=$(ui_menu "MicroPython Configuration Manager — ZENO OS" "${items[@]}")

        case "$choice" in
            "")             continue ;;
            preset)         page_preset ;;
            core)           page_core ;;
            python_libs)    page_python_libs ;;
            builtin_mods)   page_builtin_mods ;;
            esp32)          page_esp32 ;;
            optimisation)   page_optimisation ;;
            discovered)     page_discovered ;;
            summary)        page_summary ;;
            save_exit)
                save_config
                generate_makefile
                ui_info "Configuration saved to:\n  $CONFIG_FILE\n  $CONFIG_MK"
                exit $EXIT_OK
                ;;
            exit_nosave)
                if ui_yesno "Exit without saving changes?" "Confirm Exit"; then
                    exit $EXIT_CANCEL
                fi
                ;;
        esac
    done
}

# ---------------------------------------------------------------------------
# Page: Preset loader
# ---------------------------------------------------------------------------
page_preset() {
    local -a items=(
        "ESP32S3_N16R8"  "ESP32-S3-N16R8  (16MB Flash, 8MB Octal PSRAM)"
        "ESP32S3_N16R16" "ESP32-S3-N16R16 (16MB Flash, 16MB Octal PSRAM)"
        "ESP32S3_N8R8"   "ESP32-S3-N8R8   (8MB Flash,  8MB Octal PSRAM)"
        "ESP32S3_N8"     "ESP32-S3-N8     (8MB Flash,  no PSRAM)"
        "ESP32C3"        "ESP32-C3        (4MB Flash, BLE, USB Serial/JTAG)"
        "ESP32C6"        "ESP32-C6        (4MB Flash, BLE+802.15.4, USB Serial/JTAG)"
        "CUSTOM"         "Custom          (keep current settings)"
    )

    local choice
    choice=$(ui_menu "Select Hardware Preset" "${items[@]}")
    [[ -z "$choice" || "$choice" == "CUSTOM" ]] && return 0

    if ui_yesno "Apply preset '$choice'?\nThis will overwrite current settings." "Load Preset"; then
        apply_preset "$choice"
        ACTIVE_PRESET_ID="$choice"
        ui_info "Preset '$choice' applied."
    fi
}

# ---------------------------------------------------------------------------
# Page: Core Features — boolean checklist
# ---------------------------------------------------------------------------
page_core() {
    local keys=(
        CONFIG_BLUETOOTH CONFIG_WIFI CONFIG_REPL
        CONFIG_USB_CDC CONFIG_USB_SERIAL_JTAG CONFIG_MDNS
        CONFIG_WEBREPL CONFIG_FILESYSTEM CONFIG_NETWORK_STACK CONFIG_SSL_TLS
    )
    _page_checklist "Core Features" "${keys[@]}"
}

# ---------------------------------------------------------------------------
# Page: Python Libraries
# ---------------------------------------------------------------------------
page_python_libs() {
    local keys=(
        MICROPY_PY_UJSON MICROPY_PY_UREQUESTS MICROPY_PY_UZLIB
        MICROPY_PY_UASYNCIO MICROPY_PY_SSL MICROPY_PY_HASHLIB
        MICROPY_PY_FRAMEBUF MICROPY_PY_MACHINE MICROPY_PY_NETWORK
        MICROPY_PY_SOCKET MICROPY_PY_SELECT MICROPY_PY_THREADING
    )
    _page_checklist "Python Libraries" "${keys[@]}"
}

# ---------------------------------------------------------------------------
# Page: Built-in Modules
# ---------------------------------------------------------------------------
page_builtin_mods() {
    local keys=(
        MICROPY_PY_MACHINE_ADC MICROPY_PY_MACHINE_DAC
        MICROPY_PY_MACHINE_SPI MICROPY_PY_MACHINE_I2C
        MICROPY_PY_MACHINE_UART MICROPY_PY_MACHINE_PWM
        MICROPY_PY_MACHINE_RMT MICROPY_PY_MACHINE_TOUCH
        MICROPY_PY_MACHINE_CAN MICROPY_PY_MACHINE_USB
        MICROPY_PY_MACHINE_SDCARD
    )
    _page_checklist "Built-in Modules (machine.*)" "${keys[@]}"
}

# ---------------------------------------------------------------------------
# Page: ESP32-Specific Settings — mix of bool, choice, and string
# ---------------------------------------------------------------------------
page_esp32() {
    while true; do
        local -a items=(
            "psram"     "PSRAM              [${CONFIG[CONFIG_PSRAM]:-n}]"
            "flash_sz"  "Flash Size         [${CONFIG[CONFIG_FLASH_SIZE]:-4MB}]"
            "flash_fq"  "Flash Frequency    [${CONFIG[CONFIG_FLASH_FREQ]:-80m}]"
            "cpu_freq"  "CPU Frequency      [${CONFIG[CONFIG_CPU_FREQ]:-240} MHz]"
            "partition" "Partition Table    [${CONFIG[CONFIG_PARTITION_TABLE]:-default}]"
            "bt_mem"    "BT Memory (KB)     [${CONFIG[CONFIG_BT_MEMORY_KB]:-64}]"
            "wifi_rx"   "WiFi RX Bufs       [${CONFIG[CONFIG_WIFI_RX_BUF]:-10}]"
            "wifi_tx"   "WiFi TX Bufs       [${CONFIG[CONFIG_WIFI_TX_BUF]:-10}]"
            "back"      "← Back"
        )

        local choice
        choice=$(ui_menu "ESP32-Specific Settings" "${items[@]}")

        case "$choice" in
            psram)
                _toggle_bool "CONFIG_PSRAM"
                ;;
            flash_sz)
                local val
                val=$(ui_menu "Flash Size" \
                    "4MB" "4 MB" "8MB" "8 MB" "16MB" "16 MB" "32MB" "32 MB")
                [[ -n "$val" ]] && CONFIG[CONFIG_FLASH_SIZE]="$val"
                ;;
            flash_fq)
                local val
                val=$(ui_menu "Flash Frequency" \
                    "40m" "40 MHz" "80m" "80 MHz")
                [[ -n "$val" ]] && CONFIG[CONFIG_FLASH_FREQ]="$val"
                ;;
            cpu_freq)
                local val
                val=$(ui_menu "CPU Frequency" \
                    "80"  "80  MHz" \
                    "160" "160 MHz" \
                    "240" "240 MHz")
                [[ -n "$val" ]] && CONFIG[CONFIG_CPU_FREQ]="$val"
                ;;
            partition)
                local val
                val=$(ui_menu "Partition Table" \
                    "default" "Default (2MB app + OTA)" \
                    "custom"  "Custom  (uses partitions.csv)" \
                    "ota"     "OTA     (dual app partitions)")
                [[ -n "$val" ]] && CONFIG[CONFIG_PARTITION_TABLE]="$val"
                ;;
            bt_mem)
                local val
                val=$(ui_inputbox "Bluetooth Memory" \
                    "Heap KB allocated to BT controller:" \
                    "${CONFIG[CONFIG_BT_MEMORY_KB]:-64}")
                [[ -n "$val" ]] && CONFIG[CONFIG_BT_MEMORY_KB]="$val"
                ;;
            wifi_rx)
                local val
                val=$(ui_inputbox "WiFi RX Buffers" \
                    "Static RX buffer count (default 10):" \
                    "${CONFIG[CONFIG_WIFI_RX_BUF]:-10}")
                [[ -n "$val" ]] && CONFIG[CONFIG_WIFI_RX_BUF]="$val"
                ;;
            wifi_tx)
                local val
                val=$(ui_inputbox "WiFi TX Buffers" \
                    "Dynamic TX buffer count (default 10):" \
                    "${CONFIG[CONFIG_WIFI_TX_BUF]:-10}")
                [[ -n "$val" ]] && CONFIG[CONFIG_WIFI_TX_BUF]="$val"
                ;;
            back|"") return 0 ;;
        esac
    done
}

# ---------------------------------------------------------------------------
# Page: Optimisation
# ---------------------------------------------------------------------------
page_optimisation() {
    while true; do
        local -a items=(
            "build_type" "Build Type          [${CONFIG[CONFIG_BUILD_TYPE]:-release}]"
            "debug_prnt" "Debug Printf        [${CONFIG[CONFIG_MICROPY_DEBUG_PRNT]:-n}]"
            "native"     "Native Emitters     [${CONFIG[CONFIG_NATIVE_CODE]:-n}]"
            "viper"      "Viper Emitters      [${CONFIG[CONFIG_VIPER_CODE]:-n}]"
            "lto"        "Link-Time Optimise  [${CONFIG[CONFIG_LTO]:-n}]"
            "back"       "← Back"
        )

        local choice
        choice=$(ui_menu "Optimisation & Build Type" "${items[@]}")

        case "$choice" in
            build_type)
                local val
                val=$(ui_menu "Build Type" \
                    "debug"       "Debug       (-Og -g3, asserts on)" \
                    "release"     "Release     (-O2, standard)" \
                    "size"        "Size        (-Os, minimal binary)" \
                    "performance" "Performance (-O3, fastest runtime)")
                [[ -n "$val" ]] && CONFIG[CONFIG_BUILD_TYPE]="$val"
                ;;
            debug_prnt) _toggle_bool "CONFIG_MICROPY_DEBUG_PRNT" ;;
            native)     _toggle_bool "CONFIG_NATIVE_CODE" ;;
            viper)      _toggle_bool "CONFIG_VIPER_CODE" ;;
            lto)        _toggle_bool "CONFIG_LTO" ;;
            back|"")    return 0 ;;
        esac
    done
}

# ---------------------------------------------------------------------------
# Page: Discovered External Modules
# ---------------------------------------------------------------------------
page_discovered() {
    if [[ ${#DISCOVERED_MODULES[@]} -eq 0 ]]; then
        ui_info "No external modules found in lib/ modules/ extmod/"
        return 0
    fi

    local -a items=()
    for mod in "${DISCOVERED_MODULES[@]}"; do
        local cfg_key="MODULE_${mod^^}"
        local state="off"
        [[ "${CONFIG[$cfg_key]:-n}" == "y" ]] && state="on"
        items+=("$cfg_key" "$mod" "$state")
    done

    local result
    result=$(ui_checklist "External Modules (discovered)" "${items[@]}")
    _apply_checklist_result "$result" "${items[@]}"
}

# ---------------------------------------------------------------------------
# Page: Summary — display all non-default settings
# ---------------------------------------------------------------------------
page_summary() {
    local -a lines=()
    lines+=("── Active Configuration ──" "")
    lines+=("Board preset : ${ACTIVE_PRESET_ID:-custom}" "")
    lines+=("" "")
    lines+=("Key" "Value")
    lines+=("---" "-----")

    for key in $(printf '%s\n' "${!CONFIG[@]}" | sort); do
        local val="${CONFIG[$key]}"
        lines+=("$key" "$val")
    done

    case "$UI_BACKEND" in
        dialog)
            local content
            content=$(printf "%-45s %s\n" "Key" "Value"; \
                      printf "%-45s %s\n" "---" "-----"; \
                      for key in $(printf '%s\n' "${!CONFIG[@]}" | sort); do
                          printf "%-45s %s\n" "$key" "${CONFIG[$key]}"
                      done)
            dialog --title "Current Configuration" \
                --scrolltext \
                --textbox <(echo "$content") "$BOX_H" "$BOX_W"
            ;;
        whiptail)
            local content
            content=$(for key in $(printf '%s\n' "${!CONFIG[@]}" | sort); do
                          printf "%-45s = %s\n" "$key" "${CONFIG[$key]}"
                      done)
            whiptail --title "Current Configuration" \
                --scrolltext \
                --textbox <(echo "$content") "$BOX_H" "$BOX_W"
            ;;
        text)
            echo
            echo -e "${CYAN}${BOLD}  ── Current Configuration ──${RESET}"
            echo
            printf "    ${BOLD}%-45s  %s${RESET}\n" "Key" "Value"
            printf "    %-45s  %s\n" "$(printf '%0.s─' {1..45})" "$(printf '%0.s─' {1..20})"
            for key in $(printf '%s\n' "${!CONFIG[@]}" | sort); do
                local val="${CONFIG[$key]}"
                local colour="$RESET"
                [[ "$val" == "y" ]] && colour="$GREEN"
                [[ "$val" == "n" ]] && colour="$DIM"
                printf "    %-45s  ${colour}%s${RESET}\n" "$key" "$val"
            done
            echo
            read -rp "  Press Enter to continue…" _
            ;;
    esac
}

# =============================================================================
# 9. PAGE HELPER PRIMITIVES
# =============================================================================

# Generic bool checklist page
_page_checklist() {
    local title="$1"; shift
    local -a keys=("$@")
    local -a items=()

    for key in "${keys[@]}"; do
        local label="${CFG_LABEL[$key]:-$key}"
        local state="off"
        [[ "${CONFIG[$key]:-n}" == "y" ]] && state="on"
        items+=("$key" "$label" "$state")
    done

    local result
    result=$(ui_checklist "$title" "${items[@]}")
    _apply_checklist_result "$result" "${items[@]}"
}

# Apply checklist return value back into CONFIG
# result is space-separated list of QUOTED enabled tags: "TAG1" "TAG2"
_apply_checklist_result() {
    local raw_result="$1"; shift
    local -a items=("$@")

    # Collect all tag keys from items (every 3rd element starting at 0)
    local -a all_keys=()
    local i=0
    while (( i < ${#items[@]} )); do
        all_keys+=("${items[$i]}")
        (( i+=3 ))
    done

    # Parse enabled tags from result (strip quotes)
    local -A enabled_map=()
    local tok
    for tok in $raw_result; do
        tok="${tok//\"/}"
        [[ -n "$tok" ]] && enabled_map[$tok]=1
    done

    # Apply: enabled → y, absent → n
    for key in "${all_keys[@]}"; do
        if [[ -n "${enabled_map[$key]+x}" ]]; then
            CONFIG[$key]="y"
        else
            CONFIG[$key]="n"
        fi
    done
}

# Toggle a single bool CONFIG key
_toggle_bool() {
    local key="$1"
    local bool_cur="${CONFIG[$key]:-n}"
    if [[ "$bool_cur" == "y" ]]; then
        CONFIG[$key]="n"
    else
        CONFIG[$key]="y"
    fi
}

# =============================================================================
# 10. VALIDATION
# =============================================================================

validate_config() {
    local errors=0 warnings=0

    # USB CDC and USB Serial/JTAG are mutually exclusive
    if [[ "${CONFIG[CONFIG_USB_CDC]:-n}" == "y" && \
          "${CONFIG[CONFIG_USB_SERIAL_JTAG]:-n}" == "y" ]]; then
        ui_warn "USB CDC and USB Serial/JTAG are mutually exclusive.\nDisabling USB Serial/JTAG."
        CONFIG[CONFIG_USB_SERIAL_JTAG]="n"
        (( warnings++ )) || true
    fi

    # SSL requires Network Stack
    if [[ "${CONFIG[CONFIG_SSL_TLS]:-n}" == "y" && \
          "${CONFIG[CONFIG_NETWORK_STACK]:-n}" != "y" ]]; then
        ui_warn "SSL/TLS requires Network Stack. Enabling Network Stack."
        CONFIG[CONFIG_NETWORK_STACK]="y"
        (( warnings++ )) || true
    fi

    # ssl module requires SSL_TLS
    if [[ "${CONFIG[MICROPY_PY_SSL]:-n}" == "y" && \
          "${CONFIG[CONFIG_SSL_TLS]:-n}" != "y" ]]; then
        ui_warn "ssl module requires SSL/TLS to be enabled. Enabling SSL/TLS."
        CONFIG[CONFIG_SSL_TLS]="y"
        (( warnings++ )) || true
    fi

    # WebREPL requires WiFi + Network
    if [[ "${CONFIG[CONFIG_WEBREPL]:-n}" == "y" ]]; then
        if [[ "${CONFIG[CONFIG_WIFI]:-n}" != "y" || \
              "${CONFIG[CONFIG_NETWORK_STACK]:-n}" != "y" ]]; then
            ui_warn "WebREPL requires WiFi and Network Stack. Enabling both."
            CONFIG[CONFIG_WIFI]="y"
            CONFIG[CONFIG_NETWORK_STACK]="y"
            (( warnings++ )) || true
        fi
    fi

    # urequests requires socket + network
    if [[ "${CONFIG[MICROPY_PY_UREQUESTS]:-n}" == "y" ]]; then
        CONFIG[MICROPY_PY_SOCKET]="y"
        CONFIG[MICROPY_PY_NETWORK]="y"
    fi

    if [[ $errors -gt 0 ]]; then
        ui_error "$errors configuration error(s) found. Please review."
        return 1
    fi

    return 0
}

# =============================================================================
# 11. BATCH / NON-INTERACTIVE MODE
# =============================================================================
# Called when --batch flag is passed (e.g. from zeno_build.sh).
# Loads existing config (or applies a preset), validates, saves, and exits.

run_batch() {
    local preset="${1:-}"

    load_config

    if [[ -n "$preset" ]]; then
        apply_preset "$preset" || true
        ACTIVE_PRESET_ID="$preset"
    fi

    validate_config

    save_config
    generate_makefile

    echo "  Config saved → $CONFIG_FILE"
    echo "  Makefile fragment → $CONFIG_MK"
}

# =============================================================================
# 12. ARGUMENT PARSING
# =============================================================================

ACTIVE_BOARD=""
ACTIVE_PRESET_ID="custom"
BATCH_MODE=0
BATCH_PRESET=""

parse_args() {
    while [[ $# -gt 0 ]]; do
        case "$1" in
            --board)   ACTIVE_BOARD="$2";    shift 2 ;;
            --preset)  BATCH_PRESET="$2";    shift 2 ;;
            --batch)   BATCH_MODE=1;         shift   ;;
            --help|-h) _print_usage; exit 0           ;;
            *)         echo "Unknown option: $1"; _print_usage; exit $EXIT_ERROR ;;
        esac
    done
}

_print_usage() {
    cat <<'EOF'

  mpy_menuconfig.sh — MicroPython Feature Configuration Manager

  USAGE:
    ./mpy_menuconfig.sh [OPTIONS]

  OPTIONS:
    --board  <BOARD>     Set active board name (informational)
    --preset <PRESET_ID> Apply a preset non-interactively (use with --batch)
    --batch              Non-interactive: load/apply preset, validate, save, exit
    --help               Show this help

  PRESET IDs:
    ESP32S3_N16R8  ESP32S3_N16R16  ESP32S3_N8R8
    ESP32S3_N8     ESP32C3         ESP32C6

  OUTPUT FILES:
    .config/mpyconfig      Kconfig-style config store
    .config/mpyconfig.mk   Makefile include fragment

  EXAMPLES:
    ./mpy_menuconfig.sh                          # interactive TUI
    ./mpy_menuconfig.sh --batch --preset ESP32C3 # non-interactive preset apply
    ./mpy_menuconfig.sh --board ESP32_GENERIC_S3 # interactive, board label set

EOF
}

# =============================================================================
# 13. MAIN
# =============================================================================

main() {
    parse_args "$@"

    detect_ui_backend
    detect_term_size

    define_schema
    scan_modules
    register_pages

    load_config  # Overlay saved values on top of schema defaults

    if (( BATCH_MODE )); then
        run_batch "$BATCH_PRESET"
        exit $EXIT_OK
    fi

    # Interactive TUI
    ui_clear

    # Apply preset from command line if supplied
    if [[ -n "$BATCH_PRESET" ]]; then
        apply_preset "$BATCH_PRESET" || true
        ACTIVE_PRESET_ID="$BATCH_PRESET"
    fi

    show_main_menu
}

main "$@"
