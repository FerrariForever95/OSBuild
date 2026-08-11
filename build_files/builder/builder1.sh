#!/usr/bin/env bash
# =============================================================================
# ZENO OS FIRMWARE MANAGERV2
# =============================================================================
# Professional firmware management TUI for MicroPython ESP32-S3 development.
# UI modelled on Ubuntu Recovery Mode: whiptail-based nested menus,
# confirmation dialogs, progress gauges, and status screens.
#
# Architecture:
#   §00  Constants & Paths
#   §01  Safety & Error Handling
#   §02  Terminal / UI Engine  (whiptail → dialog → text fallback)
#   §03  Settings (load/save)
#   §04  Logging subsystem
#   §05  Environment verification
#   §06  Serial port detection
#   §07  Board configuration management
#   §08  Build firmware
#   §09  Flash firmware
#   §10  Serial monitor
#   §11  Build information
#   §12  Log viewer
#   §13  Clean build
#   §14  Full automated workflow
#   §15  Settings menu
#   §16  Main menu + entry point
# =============================================================================

set -uo pipefail
IFS=$'\n\t'

# =============================================================================
# §00  CONSTANTS & PATHS
# =============================================================================

readonly SCRIPT_VERSION="2.0.0"
readonly SCRIPT_NAME="ZENO OS Firmware Manager"
readonly SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# ── Repository Layout ────────────────────────────────────────────────────────
# Script lives at OSBuild/builder/zeno_firmware_manager.sh  →  REPO = OSBuild/
REPO="${REPO:-$(dirname "$SCRIPT_DIR")}"
readonly PORT_DIR="$REPO/ports/esp32"
readonly BOARD="ESP32_GENERIC_S3"
readonly BUILD_DIR="$PORT_DIR/build-${BOARD}"
readonly SDKCONFIG_SPIRAM="$PORT_DIR/boards/sdkconfig.spiram_sx"
readonly LOG_DIR="$REPO/logs"
readonly SETTINGS_FILE="$REPO/.config/firmware_manager.conf"
readonly BACKUP_DIR="$REPO/.config/backups"

# ── Known source file locations (auto-detected; fall back to these) ──────────
readonly BOARD_DIR="$PORT_DIR/boards/${BOARD}"
readonly MPCONFIGBOARD_H="$BOARD_DIR/mpconfigboard.h"   # MICROPY_HW_BOARD_NAME etc.
readonly MPCONFIG_H="$REPO/py/mpconfig.h"               # MICROPY_BANNER_NAME macro
readonly MAKEVERSIONHDR="$REPO/py/makeversionhdr.py"    # version generator

# ── Partition table candidates ───────────────────────────────────────────────
# Listed in preference order; active one is read from build sdkconfig.
readonly PARTITION_4MIB="$PORT_DIR/partitions-4MiBplus.csv"
readonly PARTITION_8MIB_OTA="$PORT_DIR/partitions-8MiBplus-ota.csv"

# ── Firmware Binary Paths (standard MicroPython ESP32 layout) ────────────────
readonly FW_BOOTLOADER="$BUILD_DIR/bootloader/bootloader.bin"
readonly FW_PARTITION="$BUILD_DIR/partition_table/partition-table.bin"
readonly FW_MICROPYTHON="$BUILD_DIR/micropython.bin"
readonly BUILD_SDKCONFIG="$BUILD_DIR/sdkconfig"   # generated after first build

# ── Default Settings (overridden by settings file) ──────────────────────────
DEFAULT_BOARD="ESP32_GENERIC_S3"
DEFAULT_PORT=""                  # empty = auto-detect
DEFAULT_BAUD="921600"
DEFAULT_AUTO_MONITOR="no"
DEFAULT_LOG_RETENTION="20"

# ── Runtime state ────────────────────────────────────────────────────────────
SELECTED_PORT=""
SETTINGS_LOADED=0

# ── Log file references (set by init_log_session) ───────────────────────────
LOG_BUILD=""
LOG_FLASH=""
LOG_REPAIR=""
LOG_AUTO=""
LOG_SESSION=""
LOG_BRAND=""
LOG_PART=""

# =============================================================================
# §01  SAFETY & ERROR HANDLING
# =============================================================================

# Cleanup handler — called on EXIT, INT, TERM
_cleanup() {
    local exit_code=$?
    # Remove any whiptail temp files
    rm -f /tmp/zfm_gauge_$$ /tmp/zfm_tmp_$$ 2>/dev/null || true
    # Restore terminal state
    tput cnorm 2>/dev/null || true    # show cursor
    tput rmcup 2>/dev/null || true    # leave alt screen if used
    [[ $exit_code -ne 0 ]] && echo "" # newline after abrupt exit
    exit $exit_code
}
trap _cleanup EXIT
trap 'exit 130' INT TERM

# die MESSAGE — print error and exit
die() {
    tput cnorm 2>/dev/null || true
    echo -e "\n  \033[1;31m✘  FATAL: $*\033[0m\n" >&2
    exit 1
}

# require_repo — verify we are in a recognisable repository
require_repo() {
    [[ -d "$PORT_DIR" ]] || die "ports/esp32 not found under REPO=$REPO\n  Set REPO= to your OSBuild path."
}

# =============================================================================
# §02  TERMINAL / UI ENGINE
# =============================================================================
# All user-visible output goes through these primitives.
# Backend is auto-detected once at startup; caller code never branches on it.
# =============================================================================

UI_BACKEND=""    # "whiptail" | "dialog" | "text"
UI_W=0           # usable dialog width
UI_H=0           # usable dialog height

# ── Detect terminal dimensions ───────────────────────────────────────────────
_ui_measure_term() {
    local rows cols
    rows=$(tput lines  2>/dev/null || echo 48)
    cols=$(tput cols   2>/dev/null || echo 160)
    UI_H=$(( rows * 95 / 100 ))
    UI_W=$(( cols * 95 / 100 ))
    [[ $UI_H -lt 12 ]] && UI_H=12
    [[ $UI_W -lt 60 ]] && UI_W=60
    [[ $UI_W -gt 100 ]] && UI_W=100
}

# ── Detect UI backend ────────────────────────────────────────────────────────
ui_init() {
    _ui_measure_term
    if command -v whiptail >/dev/null 2>&1; then
        UI_BACKEND="whiptail"
    elif command -v dialog >/dev/null 2>&1; then
        UI_BACKEND="dialog"
    else
        UI_BACKEND="text"
    fi
}

# ── Internal: run whiptail or dialog transparently ──────────────────────────
# All output from the TUI widget goes to stderr by the tool;
# we capture it by redirecting 3>&1 1>&2 2>&3 (classic trick).
_wt() {
    case "$UI_BACKEND" in
        whiptail) whiptail "$@" 3>&1 1>&2 2>&3 ;;
        dialog)   dialog   "$@" 3>&1 1>&2 2>&3 ;;
    esac
}

# ── ui_msgbox TITLE MESSAGE ──────────────────────────────────────────────────
ui_msgbox() {
    local title="$1" msg="$2"
    local h=$(( UI_H * 60 / 100 )); [[ $h -lt 8 ]] && h=8
    case "$UI_BACKEND" in
        whiptail|dialog) _wt --title " $title " --msgbox "$msg" "$h" "$UI_W" ;;
        text)
            echo -e "\n  ── $title ──"
            echo -e "$msg" | sed 's/^/  /'
            echo; read -rp "  [Press Enter to continue] " _ ;;
    esac
}

# ── ui_infobox TITLE MESSAGE (no button — used for transient notices) ────────
ui_infobox() {
    local title="$1" msg="$2"
    case "$UI_BACKEND" in
        whiptail) whiptail --title " $title " --infobox "$msg" 8 "$UI_W" ;;
        dialog)   dialog   --title " $title " --infobox "$msg" 8 "$UI_W" ;;
        text)     echo -e "\n  ── $title ──\n$msg\n" | sed 's/^/  /' ;;
    esac
}

# ── ui_yesno TITLE MESSAGE  →  0=yes  1=no ──────────────────────────────────
ui_yesno() {
    local title="$1" msg="$2"
    local h=$(( UI_H * 50 / 100 )); [[ $h -lt 8 ]] && h=8
    case "$UI_BACKEND" in
        whiptail|dialog)
            _wt --title " $title " --yesno "$msg" "$h" "$UI_W"
            return $?
            ;;
        text)
            echo -e "\n  ── $title ──\n$msg" | sed 's/^/  /'
            local ans
            read -rp "  Confirm? [y/N]: " ans
            [[ "${ans,,}" == "y" ]] && return 0 || return 1
            ;;
    esac
}

# ── ui_menu TITLE ITEMS_ARRAY  →  echoes selected tag ───────────────────────
# Items array: alternating TAG DESCRIPTION pairs
ui_menu() {
    local title="$1"; shift
    local -a items=("$@")
    local list_h=$(( UI_H - 8 )); [[ $list_h -lt 5 ]] && list_h=5
    local result

    case "$UI_BACKEND" in
        whiptail|dialog)
            result=$(_wt --title " $title " \
                --menu "" "$UI_H" "$UI_W" "$list_h" \
                "${items[@]}")
            echo "$result"
            ;;
        text)
            echo -e "\n  ── $title ──\n"
            local i=0 n=1
            local -a tags=()
            while (( i < ${#items[@]} )); do
                printf "    %2d)  %s\n" "$n" "${items[$((i+1))]}"
                tags+=("${items[$i]}")
                (( i+=2 )); (( n++ ))
            done
            echo
            local choice
            read -rp "  Select [1-$((n-1))]: " choice
            if [[ "$choice" =~ ^[0-9]+$ ]] && \
               (( choice >= 1 && choice < n )); then
                echo "${tags[$((choice-1))]}"
            else
                echo ""
            fi
            ;;
    esac
}

# ── ui_radiolist TITLE ITEMS_ARRAY  →  echoes selected tag ──────────────────
# Items array: TAG DESCRIPTION STATUS triples (STATUS = "on"|"off")
ui_radiolist() {
    local title="$1"; shift
    local -a items=("$@")
    local list_h=$(( UI_H - 8 )); [[ $list_h -lt 5 ]] && list_h=5
    local result

    case "$UI_BACKEND" in
        whiptail|dialog)
            result=$(_wt --title " $title " \
                --radiolist "" "$UI_H" "$UI_W" "$list_h" \
                "${items[@]}")
            echo "$result"
            ;;
        text)
            # Fallback: show as numbered menu
            local -a tags=()
            local -a labels=()
            local i=0
            while (( i < ${#items[@]} )); do
                tags+=("${items[$i]}")
                labels+=("${items[$((i+1))]}")
                (( i+=3 ))
            done
            echo -e "\n  ── $title ──\n"
            for n in "${!tags[@]}"; do
                printf "    %2d)  %s\n" "$((n+1))" "${labels[$n]}"
            done
            echo
            local choice
            read -rp "  Select [1-${#tags[@]}]: " choice
            if [[ "$choice" =~ ^[0-9]+$ ]] && \
               (( choice >= 1 && choice <= ${#tags[@]} )); then
                echo "${tags[$((choice-1))]}"
            fi
            ;;
    esac
}

# ── ui_inputbox TITLE PROMPT DEFAULT  →  echoes entered text ────────────────
ui_inputbox() {
    local title="$1" prompt="$2" default="${3:-}"
    local result

    case "$UI_BACKEND" in
        whiptail|dialog)
            result=$(_wt --title " $title " \
                --inputbox "$prompt" 10 "$UI_W" "$default")
            echo "$result"
            ;;
        text)
            echo -e "\n  ── $title ──\n  $prompt"
            read -rp "  [$default]: " result
            echo "${result:-$default}"
            ;;
    esac
}

# ── ui_gauge TITLE FIFO  ─────────────────────────────────────────────────────
# Reads integers 0-100 from stdin (one per line) and updates a progress bar.
# Caller pipes into this function.
ui_gauge() {
    local title="$1" msg="${2:-Working…}"
    local h=7

    case "$UI_BACKEND" in
        whiptail) whiptail --title " $title " --gauge "$msg" "$h" "$UI_W" 0 ;;
        dialog)   dialog   --title " $title " --gauge "$msg" "$h" "$UI_W" 0 ;;
        text)
            # Text fallback: print dots as percentages arrive
            local pct
            printf "  %s: [" "$title"
            while IFS= read -r pct; do
                [[ "$pct" == "100" ]] && printf "█] done\n" && return
                printf "."
            done
            printf "] done\n"
            ;;
    esac
}

# ── ui_tailbox TITLE FILE ────────────────────────────────────────────────────
# Shows a scrollable view of a log file.
ui_tailbox() {
    local title="$1" file="$2"
    [[ ! -f "$file" ]] && ui_msgbox "$title" "File not found:\n$file" && return

    case "$UI_BACKEND" in
        whiptail)
            # whiptail has no tailbox — use textbox
            whiptail --title " $title " --textbox "$file" "$UI_H" "$UI_W"
            ;;
        dialog)
            dialog --title " $title " --tailbox "$file" "$UI_H" "$UI_W"
            ;;
        text)
            less -F "$file" || true
            ;;
    esac
}

# ── ui_textbox TITLE FILE ────────────────────────────────────────────────────
ui_textbox() {
    local title="$1" file="$2"
    [[ ! -f "$file" ]] && ui_msgbox "$title" "File not found:\n$file" && return

    case "$UI_BACKEND" in
        whiptail) whiptail --title " $title " --textbox "$file" "$UI_H" "$UI_W" ;;
        dialog)   dialog   --title " $title " --textbox "$file" "$UI_H" "$UI_W" ;;
        text)     less -F "$file" || true ;;
    esac
}

# ── ui_checklist TITLE ITEMS_ARRAY  →  space-separated enabled tags ─────────
# Items: TAG DESCRIPTION STATUS triples
ui_checklist() {
    local title="$1"; shift
    local -a items=("$@")
    local list_h=$(( UI_H - 8 )); [[ $list_h -lt 5 ]] && list_h=5

    case "$UI_BACKEND" in
        whiptail|dialog)
            _wt --title " $title " \
                --checklist "" "$UI_H" "$UI_W" "$list_h" \
                "${items[@]}"
            ;;
        text)
            # Simple toggle loop
            local -a tags=() labels=() states=()
            local i=0
            while (( i < ${#items[@]} )); do
                tags+=("${items[$i]}")
                labels+=("${items[$((i+1))]}")
                states+=("${items[$((i+2))]}")
                (( i+=3 ))
            done
            echo -e "\n  ── $title ──\n"
            for n in "${!tags[@]}"; do
                local mark=" "
                [[ "${states[$n]}" == "on" ]] && mark="*"
                printf "    %2d) [%s] %s\n" "$((n+1))" "$mark" "${labels[$n]}"
            done
            echo; read -rp "  Toggle numbers (space-sep): " input
            for tok in $input; do
                if [[ "$tok" =~ ^[0-9]+$ ]] && \
                   (( tok >= 1 && tok <= ${#tags[@]} )); then
                    local idx=$((tok-1))
                    [[ "${states[$idx]}" == "on" ]] && \
                        states[$idx]="off" || states[$idx]="on"
                fi
            done
            for n in "${!tags[@]}"; do
                [[ "${states[$n]}" == "on" ]] && echo -n "\"${tags[$n]}\" "
            done
            echo
            ;;
    esac
}

# ── ui_progressfile TITLE LOG_FILE ──────────────────────────────────────────
# Shows a non-interactive "please wait" box while a background job runs.
# The caller must manage the background job separately.
ui_wait_screen() {
    local title="$1" msg="$2"
    case "$UI_BACKEND" in
        whiptail) whiptail --title " $title " --infobox "\n$msg\n\n  Please wait…" 9 "$UI_W" ;;
        dialog)   dialog   --title " $title " --infobox "\n$msg\n\n  Please wait…" 9 "$UI_W" ;;
        text)     echo -e "\n  $title\n  $msg\n  Please wait…\n" ;;
    esac
}

# ── ui_status_screen TITLE ITEMS_ARRAY ──────────────────────────────────────
# Shows a scrollable report. items = plain text lines (not tag/label pairs).
ui_status_screen() {
    local title="$1"; shift
    local -a lines=("$@")
    local tmpfile
    tmpfile=$(mktemp /tmp/zfm_tmp_XXXX)
    printf '%s\n' "${lines[@]}" > "$tmpfile"
    ui_textbox "$title" "$tmpfile"
    rm -f "$tmpfile"
}

# ── Colour helpers (text mode only) ─────────────────────────────────────────
_c()  { [[ "$UI_BACKEND" == "text" ]] && echo -ne "\033[$1m" || true; }
_cr() { _c "0"; }
_bold()  { _c "1"; }
_green() { _c "32"; }
_red()   { _c "31"; }
_yellow(){ _c "33"; }
_cyan()  { _c "36"; }

# =============================================================================
# §03  SETTINGS — load / save
# =============================================================================

cfg_load() {
    CFG_BOARD="$DEFAULT_BOARD"
    CFG_PORT="$DEFAULT_PORT"
    CFG_BAUD="$DEFAULT_BAUD"
    CFG_AUTO_MONITOR="$DEFAULT_AUTO_MONITOR"
    CFG_LOG_RETENTION="$DEFAULT_LOG_RETENTION"
    CFG_PARTITION_CSV=""          # empty = auto-detect from build sdkconfig
    CFG_IDF_EXPORT=""             # path to ESP-IDF export.sh, empty = assume loaded

    [[ -f "$SETTINGS_FILE" ]] || return 0

    local line key val
    while IFS= read -r line || [[ -n "$line" ]]; do
        [[ -z "$line" || "$line" =~ ^# ]] && continue
        if [[ "$line" =~ ^([A-Za-z_]+)=(.*)$ ]]; then
            key="${BASH_REMATCH[1]}"
            val="${BASH_REMATCH[2]}"
            case "$key" in
                CFG_BOARD)          CFG_BOARD="$val" ;;
                CFG_PORT)           CFG_PORT="$val" ;;
                CFG_BAUD)           CFG_BAUD="$val" ;;
                CFG_AUTO_MONITOR)   CFG_AUTO_MONITOR="$val" ;;
                CFG_LOG_RETENTION)  CFG_LOG_RETENTION="$val" ;;
                CFG_PARTITION_CSV)  CFG_PARTITION_CSV="$val" ;;
                CFG_IDF_EXPORT)     CFG_IDF_EXPORT="$val" ;;
            esac
        fi
    done < "$SETTINGS_FILE"

    SETTINGS_LOADED=1
}

cfg_save() {
    mkdir -p "$(dirname "$SETTINGS_FILE")"
    cat > "$SETTINGS_FILE" <<EOF
# ZENO OS Firmware Manager — Settings
# Generated: $(date -u '+%Y-%m-%dT%H:%M:%SZ')
CFG_BOARD=$CFG_BOARD
CFG_PORT=$CFG_PORT
CFG_BAUD=$CFG_BAUD
CFG_AUTO_MONITOR=$CFG_AUTO_MONITOR
CFG_LOG_RETENTION=$CFG_LOG_RETENTION
CFG_PARTITION_CSV=$CFG_PARTITION_CSV
CFG_IDF_EXPORT=$CFG_IDF_EXPORT
EOF
}

# =============================================================================
# §04  LOGGING SUBSYSTEM
# =============================================================================

init_log_session() {
    mkdir -p "$LOG_DIR"
    local ts
    ts=$(date '+%Y%m%d_%H%M%S')

    LOG_BUILD="$LOG_DIR/build_${ts}.log"
    LOG_FLASH="$LOG_DIR/flash_${ts}.log"
    LOG_REPAIR="$LOG_DIR/repair_${ts}.log"
    LOG_AUTO="$LOG_DIR/auto_${ts}.log"
    LOG_SESSION="$LOG_DIR/session_${ts}.log"
    LOG_BRAND="$LOG_DIR/brand_${ts}.log"
    LOG_PART="$LOG_DIR/partition_${ts}.log"

    # Session header written to all logs via a symlink approach:
    # Each log gets its own file but shares the session timestamp.
    {
        echo "# ZENO OS Firmware Manager — Session Log"
        echo "# Started : $(date -u '+%Y-%m-%d %H:%M:%S UTC')"
        echo "# User    : ${USER:-unknown}"
        echo "# Host    : $(hostname -s 2>/dev/null || echo unknown)"
        echo "# REPO    : $REPO"
        echo "# Board   : $CFG_BOARD"
        echo "# ──────────────────────────────────────────────────────────"
    } > "$LOG_SESSION"

    _prune_logs
}

log_write() {
    local log_file="$1"; shift
    local level="$1"; shift  # INFO WARN ERROR CMD OUT
    echo "[$(date '+%H:%M:%S')] [$level] $*" >> "$log_file"
}

log_cmd() {
    # log_cmd LOG_FILE CMD [ARGS…]
    local log_file="$1"; shift
    log_write "$log_file" CMD "$*"
}

log_section() {
    local log_file="$1" section="$2"
    {
        echo ""
        echo "# ── $section ── $(date '+%Y-%m-%d %H:%M:%S')"
        echo ""
    } >> "$log_file"
}

_prune_logs() {
    local retention="${CFG_LOG_RETENTION:-20}"
    local -a old_logs
    # Prune each category independently
    for prefix in build flash repair auto session brand partition; do
        mapfile -t old_logs < <(
            find "$LOG_DIR" -maxdepth 1 -name "${prefix}_*.log" \
                -printf '%T@ %p\n' 2>/dev/null | sort -n | \
                head -n "-${retention}" | awk '{print $2}')
        for f in "${old_logs[@]}"; do
            [[ -f "$f" ]] && rm -f "$f"
        done
    done
}

# =============================================================================
# §05  ENVIRONMENT VERIFICATION
# =============================================================================

# Returns 0 if all critical checks pass, 1 otherwise.
# Writes a formatted report to a temp file and displays it.
env_verify() {
    local show_ui="${1:-1}"   # 1 = show UI, 0 = silent (returns pass/fail)
    local -a report=()
    local all_ok=1

    local pass="  [PASS]"
    local fail="  [FAIL]"
    local warn="  [WARN]"
    local skip="  [SKIP]"

    _chk() {
        local label="$1" ok="$2" detail="${3:-}"
        local marker
        if [[ "$ok" == "pass" ]]; then
            marker="$pass"
        elif [[ "$ok" == "warn" ]]; then
            marker="$warn"
        elif [[ "$ok" == "skip" ]]; then
            marker="$skip"
        else
            marker="$fail"
            all_ok=0
        fi
        local line
        printf -v line "  %-38s %s" "$label" "$marker"
        report+=("$line")
        [[ -n "$detail" ]] && report+=("        → $detail")
    }

    report+=("")
    report+=("  ZENO OS Firmware Manager — Environment Report")
    report+=("  $(date '+%Y-%m-%d %H:%M:%S')")
    report+=("  ──────────────────────────────────────────────")
    report+=("")
    report+=("  System Tools")
    report+=("  ────────────")

    # Python
    if python3 --version >/dev/null 2>&1; then
        local pyver; pyver=$(python3 --version 2>&1)
        _chk "Python3" "pass" "$pyver"
    else
        _chk "Python3" "fail" "not found — install python3"
    fi

    # make
    if command -v make >/dev/null 2>&1; then
        _chk "make" "pass" "$(make --version 2>&1 | head -1)"
    else
        _chk "make" "fail" "not found — install build-essential"
    fi

    # git
    if command -v git >/dev/null 2>&1; then
        _chk "git" "pass" "$(git --version 2>&1)"
    else
        _chk "git" "warn" "not found (needed for branch info)"
    fi

    report+=("")
    report+=("  ESP Toolchain")
    report+=("  ─────────────")

    # ESP-IDF
    if [[ -n "${IDF_PATH:-}" && -d "$IDF_PATH" ]]; then
        local idf_ver="unknown"
        if [[ -f "$IDF_PATH/version.txt" ]]; then
            idf_ver=$(cat "$IDF_PATH/version.txt" 2>/dev/null | head -1)
        elif [[ -f "$IDF_PATH/tools/idf_tools.py" ]]; then
            idf_ver=$(python3 "$IDF_PATH/tools/idf_tools.py" version 2>/dev/null | head -1 || echo "unknown")
        fi
        _chk "ESP-IDF environment" "pass" "v$idf_ver at $IDF_PATH"
    else
        _chk "ESP-IDF environment" "fail" "IDF_PATH not set — source \$IDF_PATH/export.sh"
        all_ok=0
    fi

    # esptool
    if python3 -m esptool version >/dev/null 2>&1; then
        local esptool_ver
        esptool_ver=$(python3 -m esptool version 2>&1 | head -1)
        _chk "esptool (python -m esptool)" "pass" "$esptool_ver"
    elif command -v esptool.py >/dev/null 2>&1; then
        _chk "esptool (esptool.py)" "warn" "found as esptool.py — python -m esptool preferred"
    else
        _chk "esptool" "fail" "not found — pip install esptool"
        all_ok=0
    fi

    # mpremote
    if python3 -m mpremote version >/dev/null 2>&1; then
        _chk "mpremote" "pass" "$(python3 -m mpremote version 2>&1 | head -1)"
    elif command -v mpremote >/dev/null 2>&1; then
        _chk "mpremote" "pass" "$(mpremote version 2>&1 | head -1)"
    else
        _chk "mpremote" "warn" "not found — pip install mpremote (needed for monitor)"
    fi

    # picocom
    if command -v picocom >/dev/null 2>&1; then
        _chk "picocom" "pass" ""
    else
        _chk "picocom" "skip" "optional monitor fallback"
    fi

    # screen
    if command -v screen >/dev/null 2>&1; then
        _chk "screen" "pass" ""
    else
        _chk "screen" "skip" "optional monitor fallback"
    fi

    report+=("")
    report+=("  Repository Structure")
    report+=("  ────────────────────")

    # REPO
    if [[ -d "$REPO" ]]; then
        _chk "Repository root" "pass" "$REPO"
    else
        _chk "Repository root" "fail" "not found: $REPO"
        all_ok=0
    fi

    # ports/esp32
    if [[ -d "$PORT_DIR" ]]; then
        _chk "ports/esp32" "pass" ""
    else
        _chk "ports/esp32" "fail" "not found: $PORT_DIR"
        all_ok=0
    fi

    # Board directory
    local board_dir="$PORT_DIR/boards/${CFG_BOARD}"
    if [[ -d "$board_dir" ]]; then
        _chk "Board directory" "pass" "$board_dir"
    else
        _chk "Board directory" "fail" "not found: $board_dir"
        all_ok=0
    fi

    # sdkconfig.spiram_sx
    if [[ -f "$SDKCONFIG_SPIRAM" ]]; then
        _chk "sdkconfig.spiram_sx" "pass" ""
    else
        _chk "sdkconfig.spiram_sx" "warn" "not found: $SDKCONFIG_SPIRAM"
    fi

    # Makefile
    if [[ -f "$PORT_DIR/Makefile" ]]; then
        _chk "ESP32 Makefile" "pass" ""
    else
        _chk "ESP32 Makefile" "fail" "not found: $PORT_DIR/Makefile"
        all_ok=0
    fi

    report+=("")
    report+=("  Build Artifacts")
    report+=("  ───────────────")

    if [[ -d "$BUILD_DIR" ]]; then
        _chk "Build directory" "pass" "$BUILD_DIR"
    else
        _chk "Build directory" "skip" "not built yet"
    fi

    local -a _fw_labels=("bootloader.bin" "partition-table.bin" "micropython.bin")
    local -a _fw_paths=("$FW_BOOTLOADER" "$FW_PARTITION" "$FW_MICROPYTHON")
    local _fwi
    for _fwi in 0 1 2; do
        local _fw_label="${_fw_labels[$_fwi]}" _fw_path="${_fw_paths[$_fwi]}"
        if [[ -f "$_fw_path" ]]; then
            local sz
            sz=$(du -h "$_fw_path" 2>/dev/null | cut -f1)
            _chk "$_fw_label" "pass" "$sz"
        else
            _chk "$_fw_label" "skip" "not built"
        fi
    done

    report+=("")
    # Summary line
    if [[ $all_ok -eq 1 ]]; then
        report+=("  ✔  All critical checks passed.")
    else
        report+=("  ✘  One or more critical checks FAILED.")
        report+=("     Fix the issues above before building.")
    fi
    report+=("")

    if [[ "$show_ui" == "1" ]]; then
        ui_status_screen "Environment Verification" "${report[@]}"
    fi

    [[ $all_ok -eq 1 ]] && return 0 || return 1
}

# =============================================================================
# §06  SERIAL PORT DETECTION
# =============================================================================

# Scans for ESP32-capable serial ports.
# Sets SELECTED_PORT or presents a selection menu.
# Returns 0 on success, 1 if no port found or user cancelled.
# shellcheck disable=SC2120
serial_detect() {
    local auto="${1:-0}"    # 1 = skip menu if only one port found
    local -a ports=()

    for p in /dev/ttyACM* /dev/ttyUSB*; do
        [[ -e "$p" ]] && ports+=("$p")
    done

    # ── No ports found ────────────────────────────────────────────────────────
    if [[ ${#ports[@]} -eq 0 ]]; then
        ui_msgbox "Serial Port" \
            "No serial devices detected.\n\n\
Checked:\n  /dev/ttyACM*\n  /dev/ttyUSB*\n\n\
Connect your ESP32-S3 board and try again.\n\
Make sure the USB cable supports data transfer."
        SELECTED_PORT=""
        return 1
    fi

    # ── Saved preference ──────────────────────────────────────────────────────
    if [[ -n "$CFG_PORT" && -e "$CFG_PORT" ]]; then
        SELECTED_PORT="$CFG_PORT"
        return 0
    fi

    # ── Exactly one port ──────────────────────────────────────────────────────
    if [[ ${#ports[@]} -eq 1 && "$auto" == "1" ]]; then
        SELECTED_PORT="${ports[0]}"
        return 0
    fi

    if [[ ${#ports[@]} -eq 1 ]]; then
        if ui_yesno "Serial Port Detected" \
            "Found one serial device:\n\n  ${ports[0]}\n\nUse this port?"; then
            SELECTED_PORT="${ports[0]}"
            return 0
        else
            SELECTED_PORT=""
            return 1
        fi
    fi

    # ── Multiple ports — present selection menu ────────────────────────────────
    local -a items=()
    for p in "${ports[@]}"; do
        local detail="USB Serial Device"
        # Try to get the USB device description
        local sysfs_path
        sysfs_path=$(udevadm info --name="$p" --query=property 2>/dev/null | \
                     grep 'ID_MODEL=' | cut -d= -f2 || echo "")
        [[ -n "$sysfs_path" ]] && detail="$sysfs_path"
        items+=("$p" "$p — $detail")
    done
    items+=("CANCEL" "← Cancel — return to menu")

    local choice
    choice=$(ui_menu "Select Serial Port" "${items[@]}")

    if [[ -z "$choice" || "$choice" == "CANCEL" ]]; then
        SELECTED_PORT=""
        return 1
    fi

    SELECTED_PORT="$choice"
    return 0
}

# =============================================================================
# §07  BOARD CONFIGURATION MANAGEMENT
# =============================================================================

# Check if sdkconfig.spiram_sx has correct Octal PSRAM settings.
# Returns 0 = correct, 1 = problem found
board_config_check() {
    if [[ ! -f "$SDKCONFIG_SPIRAM" ]]; then
        return 2   # file missing
    fi

    local oct_ok=0 quad_disabled=0

    grep -q 'CONFIG_SPIRAM_MODE_OCT=y'   "$SDKCONFIG_SPIRAM" 2>/dev/null && oct_ok=1
    # CONFIG_SPIRAM_MODE_QUAD should either be absent or explicitly set to n
    if ! grep -q 'CONFIG_SPIRAM_MODE_QUAD=y' "$SDKCONFIG_SPIRAM" 2>/dev/null; then
        quad_disabled=1
    fi

    [[ $oct_ok -eq 1 && $quad_disabled -eq 1 ]] && return 0 || return 1
}

# Repair sdkconfig.spiram_sx — called after user approval
board_config_repair() {
    local ts
    ts=$(date '+%Y%m%d_%H%M%S')
    local backup="${SDKCONFIG_SPIRAM}.backup_${ts}"

    log_section "$LOG_REPAIR" "Board Config Repair"
    log_write "$LOG_REPAIR" INFO "Repairing $SDKCONFIG_SPIRAM"
    log_write "$LOG_REPAIR" INFO "Backup → $backup"

    # Create timestamped backup
    cp "$SDKCONFIG_SPIRAM" "$backup"

    # Apply corrections using sed — never rewrites the entire file
    # 1. Ensure CONFIG_SPIRAM_MODE_OCT=y is present
    if grep -q 'CONFIG_SPIRAM_MODE_OCT' "$SDKCONFIG_SPIRAM"; then
        sed -i 's/^.*CONFIG_SPIRAM_MODE_OCT.*$/CONFIG_SPIRAM_MODE_OCT=y/' \
            "$SDKCONFIG_SPIRAM"
    else
        echo "CONFIG_SPIRAM_MODE_OCT=y" >> "$SDKCONFIG_SPIRAM"
    fi

    # 2. Disable CONFIG_SPIRAM_MODE_QUAD
    if grep -q 'CONFIG_SPIRAM_MODE_QUAD=y' "$SDKCONFIG_SPIRAM"; then
        sed -i 's/^CONFIG_SPIRAM_MODE_QUAD=y$/# CONFIG_SPIRAM_MODE_QUAD is not set/' \
            "$SDKCONFIG_SPIRAM"
    fi

    log_write "$LOG_REPAIR" INFO "Repair complete"
    return 0
}

# Interactive board configuration workflow
board_config_ui() {
    if [[ ! -f "$SDKCONFIG_SPIRAM" ]]; then
        ui_msgbox "Board Configuration" \
            "sdkconfig.spiram_sx not found:\n\n$SDKCONFIG_SPIRAM\n\n\
This file should exist in your MicroPython ESP32 board definitions.\n\
Cannot verify or repair without it."
        return 1
    fi

    if board_config_check; then
        local -a lines=()
        lines+=("")
        lines+=("  Board configuration is CORRECT.")
        lines+=("")
        lines+=("  File: $SDKCONFIG_SPIRAM")
        lines+=("")
        lines+=("  CONFIG_SPIRAM_MODE_OCT=y                [OK]")
        lines+=("  CONFIG_SPIRAM_MODE_QUAD not set          [OK]")
        lines+=("")
        lines+=("  Your N16R8 Octal PSRAM configuration is valid.")
        lines+=("  No repair required.")
        lines+=("")
        ui_status_screen "Board Configuration" "${lines[@]}"
        return 0
    fi

    # Problem found — show diagnostic and ask for approval
    local oct_status="MISSING" quad_status="OK"
    grep -q 'CONFIG_SPIRAM_MODE_OCT=y' "$SDKCONFIG_SPIRAM" 2>/dev/null && \
        oct_status="OK"
    grep -q 'CONFIG_SPIRAM_MODE_QUAD=y' "$SDKCONFIG_SPIRAM" 2>/dev/null && \
        quad_status="SET (WRONG)"

    local -a diag=()
    diag+=("")
    diag+=("  ⚠  Board Configuration Issue Detected")
    diag+=("")
    diag+=("  File: $SDKCONFIG_SPIRAM")
    diag+=("")
    diag+=("  Diagnosis:")
    diag+=("  ──────────")
    diag+=("  CONFIG_SPIRAM_MODE_OCT      → $oct_status")
    diag+=("  CONFIG_SPIRAM_MODE_QUAD     → $quad_status")
    diag+=("")
    diag+=("  Expected for ESP32-S3 N16R8 (Octal PSRAM):")
    diag+=("    CONFIG_SPIRAM_MODE_OCT=y")
    diag+=("    CONFIG_SPIRAM_MODE_QUAD must NOT be set")
    diag+=("")
    diag+=("  Impact:")
    diag+=("  Using incorrect PSRAM mode will cause a panic or")
    diag+=("  silent memory corruption at boot. Octal (OPI) PSRAM")
    diag+=("  cannot be driven with Quad mode settings.")
    diag+=("")
    diag+=("  Repair will:")
    diag+=("  1. Create a timestamped backup of the original file")
    diag+=("  2. Set CONFIG_SPIRAM_MODE_OCT=y")
    diag+=("  3. Disable CONFIG_SPIRAM_MODE_QUAD")
    diag+=("")

    ui_status_screen "Board Configuration — Issue Found" "${diag[@]}"

    if ui_yesno "Repair Board Configuration?" \
        "Configuration issue detected.\n\nRepair sdkconfig.spiram_sx now?\n\nA timestamped backup will be created first."; then

        ui_wait_screen "Repairing Configuration" \
            "Writing corrected sdkconfig.spiram_sx…"

        if board_config_repair; then
            ui_msgbox "Repair Complete" \
                "Configuration repaired successfully.\n\n\
Backup created at:\n$SDKCONFIG_SPIRAM.backup_*\n\n\
Repair log:\n$LOG_REPAIR"
        else
            ui_msgbox "Repair Failed" \
                "Could not repair configuration.\n\nCheck $LOG_REPAIR for details."
            return 1
        fi
    fi
}

# =============================================================================
# §08  BUILD FIRMWARE
# =============================================================================

build_firmware() {
    local silent="${1:-0}"   # 1 = no prompts (called from automation)

    # ── Pre-flight ─────────────────────────────────────────────────────────────
    if [[ ! -d "$PORT_DIR" ]]; then
        ui_msgbox "Build Error" "ports/esp32 not found:\n$PORT_DIR"
        return 1
    fi

    if [[ ! -f "$PORT_DIR/Makefile" ]]; then
        ui_msgbox "Build Error" "Makefile not found in:\n$PORT_DIR"
        return 1
    fi

    if [[ -z "${IDF_PATH:-}" ]]; then
        ui_msgbox "Build Error" \
            "ESP-IDF environment not loaded.\n\n\
Run:  source \$IDF_PATH/export.sh\n\
then re-launch the firmware manager."
        return 1
    fi

    if [[ "$silent" == "0" ]]; then
        ui_yesno "Build Firmware" \
            "Ready to build firmware.\n\nBoard: $CFG_BOARD\nPort: $PORT_DIR\n\nBegin build?" \
            || return 0
    fi

    local ts; ts=$(date '+%Y%m%d_%H%M%S')
    local log_file="$LOG_DIR/build_${ts}.log"
    LOG_BUILD="$log_file"
    mkdir -p "$LOG_DIR"

    # Write build header
    {
        echo "# ZENO OS Firmware Manager — Build Log"
        echo "# Started : $(date -u '+%Y-%m-%d %H:%M:%S UTC')"
        echo "# Board   : $CFG_BOARD"
        echo "# IDF     : ${IDF_PATH:-unset}"
        echo "# ────────────────────────────────────────────"
        echo ""
    } > "$log_file"

    # ── Run build with progress display ───────────────────────────────────────
    local build_start; build_start=$(date +%s)
    local rc=0

    # We run make in background and tail the log for the progress display.
    # Gauge input: we fake progress by polling elapsed time.
    cd "$PORT_DIR"

    # Launch make in background, redirect all output to log
    make BOARD="$CFG_BOARD" >> "$log_file" 2>&1 &
    local make_pid=$!

    # Show progress gauge fed by a background time-based faker
    # Real progress cannot be measured from make output directly,
    # so we show a pulsing time-based gauge up to 95%, then jump to 100 on done.
    (
        local elapsed=0
        local pct=0
        # Rough estimate: ESP32 builds take ~60-180s
        while kill -0 "$make_pid" 2>/dev/null; do
            (( elapsed++ )) || true
            # Asymptotic approach to 95 — never reaches it while building
            pct=$(echo "scale=0; 95 - (95 / (1 + $elapsed / 30))" | bc 2>/dev/null || \
                  echo $(( 95 < elapsed*2 ? 95 : elapsed*2 )) )
            echo "$pct"
            sleep 1
        done
        echo 100
    ) | ui_gauge "Building Firmware" "make BOARD=$CFG_BOARD — see $log_file"

    wait "$make_pid" && rc=0 || rc=$?

    local build_end; build_end=$(date +%s)
    local duration=$(( build_end - build_start ))

    echo "" >> "$log_file"
    echo "# Build finished: $(date -u '+%Y-%m-%d %H:%M:%S UTC')" >> "$log_file"
    echo "# Duration: ${duration}s  Exit code: $rc" >> "$log_file"

    # ── Result ────────────────────────────────────────────────────────────────
    if [[ $rc -eq 0 ]]; then
        # Collect output file sizes
        local boot_sz pt_sz fw_sz
        boot_sz=$(du -h "$FW_BOOTLOADER"  2>/dev/null | cut -f1 || echo "?")
        pt_sz=$(du -h   "$FW_PARTITION"   2>/dev/null | cut -f1 || echo "?")
        fw_sz=$(du -h   "$FW_MICROPYTHON" 2>/dev/null | cut -f1 || echo "?")

        local -a summary=()
        summary+=("")
        summary+=("  ✔  Build succeeded!")
        summary+=("")
        summary+=("  Board      : $CFG_BOARD")
        summary+=("  Duration   : ${duration}s")
        summary+=("  Completed  : $(date '+%Y-%m-%d %H:%M:%S')")
        summary+=("")
        summary+=("  Firmware images:")
        summary+=("    bootloader.bin        $boot_sz")
        summary+=("    partition-table.bin   $pt_sz")
        summary+=("    micropython.bin       $fw_sz")
        summary+=("")
        summary+=("  Build log: $log_file")
        summary+=("")
        ui_status_screen "Build Complete" "${summary[@]}"
        return 0
    else
        # Extract last error lines from log
        local -a err_lines=()
        err_lines+=("")
        err_lines+=("  ✘  Build FAILED  (exit code $rc)")
        err_lines+=("")
        err_lines+=("  Duration  : ${duration}s")
        err_lines+=("  Log file  : $log_file")
        err_lines+=("")
        err_lines+=("  Last 20 lines of build output:")
        err_lines+=("  ────────────────────────────────")
        while IFS= read -r line; do
            err_lines+=("  $line")
        done < <(grep -v '^#' "$log_file" | tail -20)
        err_lines+=("")
        ui_status_screen "Build Failed" "${err_lines[@]}"

        if ui_yesno "View Full Build Log?" \
            "Build failed.\n\nView the complete build log now?"; then
            ui_textbox "Build Log — $log_file" "$log_file"
        fi
        return 1
    fi
}

# =============================================================================
# §09  FLASH FIRMWARE
# =============================================================================

# Show a pre-flash summary screen.  Returns 0 if user confirms, 1 if cancelled.
_flash_summary_screen() {
    local port="$1"

    # Collect build timestamps from file mtimes
    local fw_ts="not built"
    [[ -f "$FW_MICROPYTHON" ]] && \
        fw_ts=$(stat -c '%y' "$FW_MICROPYTHON" 2>/dev/null | cut -d. -f1 || echo "unknown")

    local -a lines=()
    lines+=("")
    lines+=("  Flash Firmware — Summary")
    lines+=("  ────────────────────────────────────────────────")
    lines+=("")
    lines+=("  Target Board  : $CFG_BOARD")
    lines+=("  Serial Port   : $port")
    lines+=("  Baud Rate     : $CFG_BAUD")
    lines+=("")
    lines+=("  Flash Parameters")
    lines+=("  ────────────────")
    lines+=("  Chip          : esp32s3")
    lines+=("  Flash mode    : dio")
    lines+=("  Flash size    : 16MB")
    lines+=("  Flash freq    : 0m")
    lines+=("  Before        : default_reset")
    lines+=("  After         : hard_reset")
    lines+=("")
    lines+=("  Firmware Images")
    lines+=("  ───────────────")

    local boot_ok=" [MISSING]" pt_ok=" [MISSING]" fw_ok=" [MISSING]"
    local boot_sz="" pt_sz="" fw_sz=""

    if [[ -f "$FW_BOOTLOADER" ]]; then
        boot_sz=$(du -h "$FW_BOOTLOADER" | cut -f1)
        boot_ok=" [$boot_sz]"
    fi
    if [[ -f "$FW_PARTITION" ]]; then
        pt_sz=$(du -h "$FW_PARTITION" | cut -f1)
        pt_ok=" [$pt_sz]"
    fi
    if [[ -f "$FW_MICROPYTHON" ]]; then
        fw_sz=$(du -h "$FW_MICROPYTHON" | cut -f1)
        fw_ok=" [$fw_sz]"
    fi

    lines+=("  0x00000  bootloader.bin       $boot_ok")
    lines+=("  0x08000  partition-table.bin  $pt_ok")
    lines+=("  0x10000  micropython.bin      $fw_ok")
    lines+=("")
    lines+=("  Build date    : $fw_ts")
    lines+=("")

    ui_status_screen "Flash Summary" "${lines[@]}"
}

# Validate that all three firmware binaries exist.
_flash_validate() {
    local missing=0
    [[ ! -f "$FW_BOOTLOADER"  ]] && missing=1
    [[ ! -f "$FW_PARTITION"   ]] && missing=1
    [[ ! -f "$FW_MICROPYTHON" ]] && missing=1

    if [[ $missing -eq 1 ]]; then
        ui_msgbox "Flash Error" \
            "One or more firmware images are missing.\n\n\
Expected:\n\
  $FW_BOOTLOADER\n\
  $FW_PARTITION\n\
  $FW_MICROPYTHON\n\n\
Please build the firmware first (option 1 from main menu)."
        return 1
    fi
    return 0
}

flash_firmware() {
    local silent="${1:-0}"
    local port="${SELECTED_PORT:-}"

    # Validate binaries
    _flash_validate || return 1

    # Detect port if not already set
    if [[ -z "$port" ]]; then
        serial_detect || return 1
        port="$SELECTED_PORT"
    fi

    [[ ! -e "$port" ]] && \
        ui_msgbox "Flash Error" "Selected port does not exist:\n$port" && return 1

    # Show summary and ask for confirmation
    _flash_summary_screen "$port"

    if ! ui_yesno "Confirm Flash" \
        "Flash firmware to $port?\n\n\nBoard must be connected."; then
        return 0
    fi

    local ts; ts=$(date '+%Y%m%d_%H%M%S')
    local log_file="$LOG_DIR/flash_${ts}.log"
    LOG_FLASH="$log_file"
    mkdir -p "$LOG_DIR"

    {
        echo "# ZENO OS Firmware Manager — Flash Log"
        echo "# Started  : $(date -u '+%Y-%m-%d %H:%M:%S UTC')"
        echo "# Port     : $port"
        echo "# Baud     : $CFG_BAUD"
        echo "# Board    : $CFG_BOARD"
        echo "# ────────────────────────────────────────────"
        echo ""
    } > "$log_file"

    # ── Build esptool command (exactly as specified) ───────────────────────────
    local -a cmd=(
        python3 -m esptool
        --chip esp32s3
        -b "$CFG_BAUD"
        --port "$port"
        write_flash
        --flash_mode dio
        --flash_size 16MB
        --flash_freq 80m
        0x0    "$FW_BOOTLOADER"
        0x8000 "$FW_PARTITION"
        0x10000 "$FW_MICROPYTHON"
    )

    echo "# Command: ${cmd[*]}" >> "$log_file"
    echo "" >> "$log_file"

    ui_wait_screen "Flashing Firmware" \
        "Writing to $port at ${CFG_BAUD} baud…\n\nDo not disconnect the board."

    local rc=0
    "${cmd[@]}" >> "$log_file" 2>&1 || rc=$?

    echo "" >> "$log_file"
    echo "# Flash finished: $(date -u '+%Y-%m-%d %H:%M:%S UTC')" >> "$log_file"
    echo "# Exit code: $rc" >> "$log_file"

    if [[ $rc -eq 0 ]]; then
        ui_msgbox "Flash Complete" \
            "✔  Firmware flashed successfully!\n\nPort  : $port\nBoard : $CFG_BOARD\nLog   : $log_file"

        # Auto-open monitor if configured
        if [[ "$CFG_AUTO_MONITOR" == "yes" ]]; then
            if ui_yesno "Open Monitor?" "Open serial monitor on $port now?"; then
                serial_monitor_launch "$port"
            fi
        fi
        return 0
    else
        local -a err=()
        err+=("")
        err+=("  ✘  Flash FAILED (exit code $rc)")
        err+=("")
        err+=("  Port    : $port")
        err+=("  Log     : $log_file")
        err+=("")
        err+=("  Common causes:")
        err+=("  • Board not in download mode (hold BOOT, tap RESET)")
        err+=("  • Wrong port selected")
        err+=("  • USB data cable (not charge-only)")
        err+=("  • Port already open by another program")
        err+=("")
        err+=("  Last output:")
        err+=("  ────────────")
        while IFS= read -r line; do
            err+=("  $line")
        done < <(tail -15 "$log_file")
        err+=("")
        ui_status_screen "Flash Failed" "${err[@]}"
        return 1
    fi
}

# =============================================================================
# §10  SERIAL MONITOR
# =============================================================================

serial_monitor_launch() {
    local port="${1:-$SELECTED_PORT}"

    if [[ -z "$port" ]]; then
        serial_detect || return 1
        port="$SELECTED_PORT"
    fi

    [[ ! -e "$port" ]] && \
        ui_msgbox "Monitor Error" "Port not found: $port" && return 1

    # Choose monitor tool
    local -a items=(
        "mpremote"  "mpremote connect $port  (recommended)"
        "picocom"   "picocom -b 115200 $port"
        "screen"    "screen $port 115200"
        "BACK"      "← Return to menu"
    )

    # Auto-prefer mpremote
    local default_tool=""
    python3 -m mpremote version >/dev/null 2>&1 && default_tool="mpremote"
    command -v picocom >/dev/null 2>&1 && [[ -z "$default_tool" ]] && default_tool="picocom"
    command -v screen  >/dev/null 2>&1 && [[ -z "$default_tool" ]] && default_tool="screen"

    local choice
    choice=$(ui_menu "Serial Monitor — $port" "${items[@]}")

    case "$choice" in
        mpremote)
            if ! python3 -m mpremote version >/dev/null 2>&1; then
                ui_msgbox "mpremote Not Found" \
                    "mpremote is not installed.\n\nInstall with:\n  pip install mpremote"
                return 1
            fi
            clear
            echo -e "\033[1;36m  ZENO OS — Serial Monitor (mpremote)\033[0m"
            echo -e "  Port: $port  |  Press Ctrl+] to exit\n"
            python3 -m mpremote connect "$port" || true
            ;;
        picocom)
            if ! command -v picocom >/dev/null 2>&1; then
                ui_msgbox "picocom Not Found" \
                    "picocom is not installed.\n\nInstall with:\n  sudo apt install picocom"
                return 1
            fi
            clear
            echo -e "\033[1;36m  ZENO OS — Serial Monitor (picocom)\033[0m"
            echo -e "  Port: $port 115200  |  Press Ctrl+A Ctrl+X to exit\n"
            picocom -b 115200 "$port" || true
            ;;
        screen)
            if ! command -v screen >/dev/null 2>&1; then
                ui_msgbox "screen Not Found" \
                    "screen is not installed.\n\nInstall with:\n  sudo apt install screen"
                return 1
            fi
            clear
            echo -e "\033[1;36m  ZENO OS — Serial Monitor (screen)\033[0m"
            echo -e "  Port: $port 115200  |  Press Ctrl+A K to exit\n"
            screen "$port" 115200 || true
            ;;
        BACK|"") return 0 ;;
    esac
}

serial_monitor_ui() {
    serial_detect
    [[ -n "$SELECTED_PORT" ]] && serial_monitor_launch "$SELECTED_PORT"
}

# =============================================================================
# §11  BUILD INFORMATION
# =============================================================================

build_info_ui() {
    local -a lines=()

    # Git info
    local branch="unknown"
    local last_commit="unknown"
    if command -v git >/dev/null 2>&1 && [[ -d "$REPO/.git" ]]; then
        branch=$(git -C "$REPO" branch --show-current 2>/dev/null || echo "unknown")
        last_commit=$(git -C "$REPO" log -1 --format='%h %s' 2>/dev/null || echo "unknown")
    fi

    # Build timestamps from file mtimes
    local build_ts="never"
    [[ -f "$FW_MICROPYTHON" ]] && \
        build_ts=$(stat -c '%y' "$FW_MICROPYTHON" 2>/dev/null | cut -d. -f1)

    # Latest flash log
    local flash_ts="never"
    local latest_flash
    latest_flash=$(find "$LOG_DIR" -maxdepth 1 -name "flash_*.log" \
        -printf '%T@ %p\n' 2>/dev/null | sort -n | tail -1 | awk '{print $2}')
    if [[ -n "$latest_flash" ]]; then
        flash_ts=$(stat -c '%y' "$latest_flash" 2>/dev/null | cut -d. -f1)
    fi

    # ESP-IDF version
    local idf_ver="loaded"
    if [[ -n "${IDF_PATH:-}" && -f "$IDF_PATH/version.txt" ]]; then
        idf_ver="v$(cat "$IDF_PATH/version.txt" | head -1)"
    elif [[ -n "${IDF_VERSION:-}" ]]; then
        idf_ver="v$IDF_VERSION"
    fi

    # PSRAM config summary
    local psram_mode="unknown"
    if [[ -f "$SDKCONFIG_SPIRAM" ]]; then
        if grep -q 'CONFIG_SPIRAM_MODE_OCT=y' "$SDKCONFIG_SPIRAM"; then
            psram_mode="Octal (OPI) ✔"
        elif grep -q 'CONFIG_SPIRAM_MODE_QUAD=y' "$SDKCONFIG_SPIRAM"; then
            psram_mode="Quad (WRONG for N16R8)"
        else
            psram_mode="unknown (check $SDKCONFIG_SPIRAM)"
        fi
    fi

    # Flash config
    local flash_mode="dio / 80MHz / 16MB (hardcoded)"

    lines+=("")
    lines+=("  Build Information")
    lines+=("  ─────────────────────────────────────────────────────")
    lines+=("")
    lines+=("  Repository")
    lines+=("  ──────────")
    lines+=("  Path            : $REPO")
    lines+=("  Branch          : $branch")
    lines+=("  Latest commit   : $last_commit")
    lines+=("")
    lines+=("  Build")
    lines+=("  ─────")
    lines+=("  Board           : $CFG_BOARD")
    lines+=("  Build dir       : $BUILD_DIR")
    lines+=("  Latest build    : $build_ts")
    lines+=("  Latest flash    : $flash_ts")
    lines+=("")
    lines+=("  Toolchain")
    lines+=("  ─────────")
    lines+=("  ESP-IDF version : $idf_ver")m
    lines+=("  Python          : $(python3 --version 2>&1)")
    lines+=("")
    lines+=("  Hardware Configuration")
    lines+=("  ──────────────────────")
    lines+=("  Target hardware : ESP32-S3 N16R8")
    lines+=("  PSRAM mode      : $psram_mode")
    lines+=("  Flash mode      : $flash_mode")
    lines+=("")
    lines+=("  Firmware Images")
    lines+=("  ───────────────")

    local -a _bi_labels=("bootloader.bin" "partition-table.bin" "micropython.bin")
    local -a _bi_paths=("$FW_BOOTLOADER" "$FW_PARTITION" "$FW_MICROPYTHON")
    local _bii
    for _bii in 0 1 2; do
        local _bi_label="${_bi_labels[$_bii]}" _bi_path="${_bi_paths[$_bii]}"
        if [[ -f "$_bi_path" ]]; then
            local sz ts_fw
            sz=$(du -h "$_bi_path" | cut -f1)
            ts_fw=$(stat -c '%y' "$_bi_path" | cut -d. -f1)
            lines+=("  $(printf '%-24s' "$_bi_label")  $sz   $ts_fw")
        else
            lines+=("  $(printf '%-24s' "$_bi_label")  [not built]")
        fi
    done
    lines+=("")

    ui_status_screen "Build Information" "${lines[@]}"
}

# =============================================================================
# §12  LOG VIEWER
# =============================================================================

log_viewer_ui() {
    while true; do
        local -a items=()

        # Dynamically list available log files grouped by type
        for prefix in build flash repair auto session; do
            local -a logs
            mapfile -t logs < <(
                find "$LOG_DIR" -maxdepth 1 -name "${prefix}_*.log" \
                    -printf '%T@ %p\n' 2>/dev/null | sort -rn | \
                    head -5 | awk '{print $2}')
            for f in "${logs[@]}"; do
                local ts_label
                ts_label=$(basename "$f" .log | sed "s/${prefix}_//")
                items+=("$f" "[$prefix] $ts_label")
            done
        done

        if [[ ${#items[@]} -eq 0 ]]; then
            ui_msgbox "Log Viewer" \
                "No log files found.\n\nLogs are stored in:\n$LOG_DIR\n\nRun a build or flash first."
            return 0
        fi

        items+=("BACK" "← Return to main menu")

        local choice
        choice=$(ui_menu "Log Viewer — Select a log file" "${items[@]}")

        case "$choice" in
            BACK|"") return 0 ;;
            *)
                if [[ -f "$choice" ]]; then
                    ui_textbox "Log: $(basename "$choice")" "$choice"
                fi
                ;;
        esac
    done
}

# =============================================================================
# §13  CLEAN BUILD DIRECTORY
# =============================================================================

clean_build_ui() {
    local -a items=(
        "CLEAN"   "Clean — run make clean (preserves source)"
        "REMOVE"  "Remove — delete entire build-$BOARD directory"
        "BACK"    "← Cancel — return to main menu"
    )

    local choice
    choice=$(ui_menu "Clean Build Directory" "${items[@]}")

    case "$choice" in
        CLEAN)
            if ui_yesno "Confirm Clean" \
                "Run 'make BOARD=$CFG_BOARD clean'?\n\nThis removes compiled objects but keeps configuration."; then
                ui_wait_screen "Cleaning" "Running make clean…"
                local log="$LOG_DIR/clean_$(date '+%Y%m%d_%H%M%S').log"
                mkdir -p "$LOG_DIR"
                cd "$PORT_DIR"
                make BOARD="$CFG_BOARD" clean > "$log" 2>&1 && \
                    ui_msgbox "Clean Complete" "make clean finished.\n\nLog: $log" || \
                    ui_msgbox "Clean Failed" "make clean failed.\n\nLog: $log"
            fi
            ;;
        REMOVE)
            if ui_yesno "Confirm Delete" \
                "DELETE the entire build directory?\n\n$BUILD_DIR\n\nThis cannot be undone."; then
                if [[ -d "$BUILD_DIR" ]]; then
                    rm -rf "$BUILD_DIR"
                    ui_msgbox "Removed" "Build directory deleted:\n$BUILD_DIR"
                else
                    ui_msgbox "Nothing to Remove" "Build directory does not exist:\n$BUILD_DIR"
                fi
            fi
            ;;
        BACK|"") return 0 ;;
    esac
}

# =============================================================================
# §14  FULL AUTOMATED WORKFLOW
# =============================================================================

workflow_auto() {
    local ts; ts=$(date '+%Y%m%d_%H%M%S')
    local log_file="$LOG_DIR/auto_${ts}.log"
    LOG_AUTO="$log_file"
    mkdir -p "$LOG_DIR"

    {
        echo "# ZENO OS Firmware Manager — Automated Workflow Log"
        echo "# Started : $(date -u '+%Y-%m-%d %H:%M:%S UTC')"
        echo "# Board   : $CFG_BOARD"
        echo "# ──────────────────────────────────────────────"
        echo ""
    } > "$log_file"

    local -a steps=(
        "1" "Verify environment"
        "2" "Verify board configuration"
        "3" "Repair board config (if needed)"
        "4" "Build firmware"
        "5" "Detect serial port"
        "6" "Review flash summary"
        "7" "Flash firmware"
        "8" "Open serial monitor"
    )

    ui_msgbox "Full Automated Workflow" \
        "This will run the complete build+flash pipeline:\n\n\
1. Verify environment\n\
2. Verify board configuration\n\
3. Repair if needed\n\
4. Build firmware\n\
5. Detect serial port\n\
6. Review flash summary\n\
7. Flash firmware\n\
8. Optionally open monitor\n\n\
You will be asked to confirm before flashing."

    # ── Step 1: Environment ───────────────────────────────────────────────────
    log_section "$log_file" "Step 1: Environment Verification"
    ui_wait_screen "Automated Workflow" "Step 1/8: Verifying environment…"

    if ! env_verify 0; then
        ui_msgbox "Workflow Aborted" \
            "Environment verification failed.\n\n\
Fix the issues shown in Verify Environment (menu option 5)\nthen try again."
        echo "# ABORTED at step 1 — environment verification failed" >> "$log_file"
        return 1
    fi
    echo "# Step 1 PASSED" >> "$log_file"

    # ── Step 2 & 3: Board config ──────────────────────────────────────────────
    log_section "$log_file" "Step 2: Board Configuration"
    ui_wait_screen "Automated Workflow" "Step 2/8: Checking board configuration…"

    if ! board_config_check; then
        echo "# Board config issue detected — repairing" >> "$log_file"
        if ui_yesno "Board Configuration Issue" \
            "Board PSRAM configuration issue detected.\n\nRepair automatically and continue?"; then
            board_config_repair
            echo "# Step 3 REPAIRED" >> "$log_file"
        else
            echo "# ABORTED at step 3 — user declined repair" >> "$log_file"
            ui_msgbox "Workflow Aborted" "Board configuration not repaired.\nFix manually and retry."
            return 1
        fi
    else
        echo "# Step 2 PASSED — config correct" >> "$log_file"
    fi

    # ── Step 4: Build ─────────────────────────────────────────────────────────
    log_section "$log_file" "Step 4: Build"
    echo "# Starting build…" >> "$log_file"

    if ! build_firmware 1; then
        echo "# ABORTED at step 4 — build failed" >> "$log_file"
        ui_msgbox "Workflow Aborted" "Build failed.\nCheck build log and fix errors."
        return 1
    fi
    echo "# Step 4 PASSED" >> "$log_file"

    # ── Step 5: Serial detect ─────────────────────────────────────────────────
    log_section "$log_file" "Step 5: Serial Port"
    if ! serial_detect; then
        echo "# ABORTED at step 5 — no serial port" >> "$log_file"
        ui_msgbox "Workflow Aborted" "No serial port detected.\nConnect the board and try again."
        return 1
    fi
    echo "# Step 5 PASSED — port: $SELECTED_PORT" >> "$log_file"

    # ── Step 6 & 7: Flash ─────────────────────────────────────────────────────
    log_section "$log_file" "Step 6-7: Flash"
    if ! flash_firmware 1; then
        echo "# Step 7 FAILED or cancelled" >> "$log_file"
        # Not fatal — firmware was built; user can flash manually
    else
        echo "# Step 7 PASSED" >> "$log_file"
    fi

    # ── Step 8: Monitor ───────────────────────────────────────────────────────
    if ui_yesno "Open Serial Monitor?" \
        "Workflow complete!\n\nOpen serial monitor on $SELECTED_PORT now?"; then
        serial_monitor_launch "$SELECTED_PORT"
    fi

    echo "" >> "$log_file"
    echo "# Workflow finished: $(date -u '+%Y-%m-%d %H:%M:%S UTC')" >> "$log_file"

    ui_msgbox "Workflow Complete" \
        "Full automated workflow finished.\n\nAutomation log: $log_file"
}

# =============================================================================
# §15  SETTINGS MENU
# =============================================================================

settings_ui() {
    while true; do
        local -a items=(
            "BOARD"    "Default board       [$CFG_BOARD]"
            "PORT"     "Default serial port [${CFG_PORT:-auto}]"
            "BAUD"     "Flash baud rate     [$CFG_BAUD]"
            "MONITOR"  "Auto-open monitor   [$CFG_AUTO_MONITOR]"
            "LOGS"     "Log retention count [$CFG_LOG_RETENTION files]"
            "IDF"      "IDF export.sh path  [${CFG_IDF_EXPORT:-auto-detect}]"
            "PART"     "Partition CSV       [${CFG_PARTITION_CSV:-auto-detect}]"
            "SAVE"     "Save settings"
            "BACK"     "← Return to main menu"
        )

        local choice
        choice=$(ui_menu "Settings" "${items[@]}")

        case "$choice" in
            BOARD)
                local val
                val=$(ui_inputbox "Default Board" \
                    "Enter default board name:" "$CFG_BOARD")
                [[ -n "$val" ]] && CFG_BOARD="$val"
                ;;
            PORT)
                local -a port_items=("" "Auto-detect (recommended)")
                for p in /dev/ttyACM* /dev/ttyUSB*; do
                    [[ -e "$p" ]] && port_items+=("$p" "$p")
                done
                port_items+=("CUSTOM" "Enter manually…")
                local pval
                pval=$(ui_menu "Default Serial Port" "${port_items[@]}")
                if [[ "$pval" == "CUSTOM" ]]; then
                    pval=$(ui_inputbox "Serial Port" \
                        "Enter port path:" "${CFG_PORT:-/dev/ttyUSB0}")
                fi
                CFG_PORT="${pval:-}"
                ;;
            BAUD)
                local -a baud_items=(
                    "921600" "921600 — maximum speed(default)"
                    "460800" "460800 — recommended"
                    "230400" "230400 — conservative"
                    "115200" "115200 — safe fallback"
                )
                local bval
                bval=$(ui_menu "Flash Baud Rate" "${baud_items[@]}")
                [[ -n "$bval" ]] && CFG_BAUD="$bval"
                ;;
            MONITOR)
                local mval
                mval=$(ui_menu "Auto-open Monitor After Flash" \
                    "yes" "Yes — open monitor automatically after flash" \
                    "no"  "No  — return to menu after flash (default)")
                [[ -n "$mval" ]] && CFG_AUTO_MONITOR="$mval"
                ;;
            LOGS)
                local lval
                lval=$(ui_inputbox "Log Retention" \
                    "Number of log files to keep per category (1-100):" \
                    "$CFG_LOG_RETENTION")
                if [[ "$lval" =~ ^[0-9]+$ ]] && \
                   (( lval >= 1 && lval <= 100 )); then
                    CFG_LOG_RETENTION="$lval"
                else
                    ui_msgbox "Invalid Input" "Please enter a number between 1 and 100."
                fi
                ;;
            IDF)
                local ival
                ival=$(ui_inputbox "ESP-IDF Export Path"                     "Path to ESP-IDF export.sh\n(leave empty to use auto-detect):"                     "${CFG_IDF_EXPORT:-}")
                CFG_IDF_EXPORT="$ival"
                ;;
            PART)
                local -a pcsv_items=("" "Auto-detect from build sdkconfig")
                while IFS= read -r csv; do
                    pcsv_items+=("$csv" "$csv")
                done < <(partition_scan_csvs)
                local pval
                pval=$(ui_menu "Default Partition CSV" "${pcsv_items[@]}")
                CFG_PARTITION_CSV="${pval:-}"
                ;;
            SAVE)
                cfg_save
                ui_msgbox "Settings Saved" "Settings written to:\n$SETTINGS_FILE"
                ;;
            BACK|"") return 0 ;;
        esac
    done
}

# =============================================================================
# §A  FIRMWARE IDENTITY HELPERS
# =============================================================================
# Read firmware identity from source files and build artefacts.
# All readers are pure grep/awk — never modify files.
# =============================================================================

# ── Read a #define value from a C header ────────────────────────────────────
# _read_define FILE MACRO_NAME  →  echoes the value or "unknown"
_read_define() {
    local file="$1" macro="$2"
    if [[ ! -f "$file" ]]; then echo "unknown"; return; fi
    local val
    val=$(grep -m1 "#define[[:space:]]\+${macro}" "$file" 2>/dev/null \
          | sed 's/.*#define[[:space:]]\+[A-Za-z_0-9]\+[[:space:]]\+//' \
          | sed 's/"//g' | sed 's/^[[:space:]]*//' | sed 's/[[:space:]]*$//')
    echo "${val:-unknown}"
}

# ── Read active partition CSV from build sdkconfig ───────────────────────────
# Returns filename only (e.g. "partitions-8MiBplus-ota.csv") or "unknown"
# shellcheck disable=SC2120
_read_active_partition() {
    local sdkcfg="${1:-$BUILD_SDKCONFIG}"
    [[ ! -f "$sdkcfg" ]] && echo "unknown (build first)" && return
    local val
    val=$(grep -m1 'CONFIG_PARTITION_TABLE_FILENAME=' "$sdkcfg" 2>/dev/null \
          | cut -d= -f2 | sed 's/"//g' | sed 's/^[[:space:]]*//')
    # Fallback: custom CSV path
    if [[ -z "$val" ]]; then
        val=$(grep -m1 'CONFIG_PARTITION_TABLE_CUSTOM_FILENAME=' "$sdkcfg" 2>/dev/null \
              | cut -d= -f2 | sed 's/"//g')
    fi
    echo "${val:-default}"
}

# ── Read git short hash ──────────────────────────────────────────────────────
_read_git_hash() {
    if command -v git >/dev/null 2>&1 && [[ -d "$REPO/.git" ]]; then
        git -C "$REPO" rev-parse --short HEAD 2>/dev/null || echo "unknown"
    else
        echo "unknown"
    fi
}

# ── Read git branch ──────────────────────────────────────────────────────────
_read_git_branch() {
    if command -v git >/dev/null 2>&1 && [[ -d "$REPO/.git" ]]; then
        git -C "$REPO" branch --show-current 2>/dev/null || echo "unknown"
    else
        echo "unknown"
    fi
}

# ── Read build date from micropython.bin mtime ───────────────────────────────
_read_build_date() {
    [[ -f "$FW_MICROPYTHON" ]] && \
        stat -c '%y' "$FW_MICROPYTHON" 2>/dev/null | cut -d. -f1 || echo "not built"
}

# ── Render the MicroPython boot banner as it will appear on device ────────────
# Reads MICROPY_BANNER_NAME from py/mpconfig.h, MICROPY_HW_BOARD_NAME from
# mpconfigboard.h, and version from makeversionhdr.py.
_render_banner() {
    local board_name mcu_name banner_name version git_hash
    board_name=$(_read_define "$MPCONFIGBOARD_H" "MICROPY_HW_BOARD_NAME")
    mcu_name=$(_read_define   "$MPCONFIGBOARD_H" "MICROPY_HW_MCU_NAME")
    banner_name=$(_read_define "$MPCONFIG_H"     "MICROPY_BANNER_NAME")
    git_hash=$(_read_git_hash)
    version="unknown"

    # Try to get version from makeversionhdr.py output
    if [[ -f "$MAKEVERSIONHDR" ]]; then
        version=$(python3 "$MAKEVERSIONHDR" /dev/null 2>/dev/null \
                  | grep 'MICROPY_VERSION_STRING' \
                  | sed 's/.*"\(.*\)".*/\1/' || echo "unknown")
    fi
    # Fallback: read from py/mpconfig.h
    if [[ "$version" == "unknown" && -f "$MPCONFIG_H" ]]; then
        version=$(_read_define "$MPCONFIG_H" "MICROPY_VERSION_STRING")
    fi

    # Compose banner exactly as MicroPython does:
    #   MicroPython <version>; <banner_name> with <board_name>
    local banner_str
    if [[ "$banner_name" != "unknown" && "$banner_name" != "" ]]; then
        banner_str="MicroPython ${version}; ${banner_name} with ${board_name}"
    else
        banner_str="MicroPython ${version}; ${board_name} with ${mcu_name}"
    fi
    echo "$banner_str"
}

# ── Firmware Identity — assemble full report ─────────────────────────────────
firmware_identity_ui() {
    local board_name mcu_name git_hash git_branch build_date active_pt banner

    board_name=$(_read_define "$MPCONFIGBOARD_H" "MICROPY_HW_BOARD_NAME")
    mcu_name=$(_read_define   "$MPCONFIGBOARD_H" "MICROPY_HW_MCU_NAME")
    git_hash=$(_read_git_hash)
    git_branch=$(_read_git_branch)
    build_date=$(_read_build_date)
    active_pt=$(_read_active_partition)
    banner=$(_render_banner)

    local -a lines=()
    lines+=("")
    lines+=("  Firmware Identity")
    lines+=("  ══════════════════════════════════════════════════════")
    lines+=("")
    lines+=("  Board Name     :  $board_name")
    lines+=("  MCU Name       :  $mcu_name")
    lines+=("")
    lines+=("  Git Hash       :  $git_hash")
    lines+=("  Git Branch     :  $git_branch")
    lines+=("")
    lines+=("  Build Date     :  $build_date")
    lines+=("  Partition Table:  $active_pt")
    lines+=("")
    lines+=("  ── Boot Banner Preview ────────────────────────────────")
    lines+=("")
    lines+=("  $banner")
    lines+=("  Type \"help()\" for more information.")
    lines+=("")
    lines+=("  ── Source Files ───────────────────────────────────────")
    lines+=("")
    lines+=("  mpconfigboard.h  :  $MPCONFIGBOARD_H")

    if [[ -f "$MPCONFIGBOARD_H" ]]; then
        lines+=("                     [EXISTS]")
    else
        lines+=("                     [NOT FOUND]")
    fi

    lines+=("  mpconfig.h       :  $MPCONFIG_H")
    if [[ -f "$MPCONFIG_H" ]]; then
        lines+=("                     [EXISTS]")
    else
        lines+=("                     [NOT FOUND]")
    fi
    lines+=("")

    ui_status_screen "Firmware Identity" "${lines[@]}"
}

# =============================================================================
# §B  BRANDING MENU
# =============================================================================
# Edits MICROPY_HW_BOARD_NAME, MICROPY_HW_MCU_NAME, and optionally
# MICROPY_BANNER_NAME.  Always creates a timestamped backup before any write.
# =============================================================================

# ── Create timestamped backup of a file ──────────────────────────────────────
_backup_file() {
    local file="$1"
    [[ ! -f "$file" ]] && return 1
    mkdir -p "$BACKUP_DIR"
    local ts; ts=$(date '+%Y%m%d_%H%M%S')
    local basename; basename=$(basename "$file")
    local backup="$BACKUP_DIR/${basename}.backup_${ts}"
    cp "$file" "$backup"
    echo "$backup"
}

# ── Patch a #define in a C header in-place ───────────────────────────────────
# _patch_define FILE MACRO NEW_VALUE
# Handles both:  #define MACRO "value"
# and:           #define MACRO value  (no quotes for numeric)
_patch_define() {
    local file="$1" macro="$2" newval="$3"
    [[ ! -f "$file" ]] && return 1

    # Detect if original uses quotes
    local original_line
    original_line=$(grep -m1 "#define[[:space:]]\+${macro}" "$file" 2>/dev/null || echo "")

    if [[ "$original_line" =~ '"' ]]; then
        # Quoted string value
        sed -i "s|#define[[:space:]]\+${macro}[[:space:]].*|#define ${macro} \"${newval}\"|" "$file"
    else
        # Unquoted / numeric
        sed -i "s|#define[[:space:]]\+${macro}[[:space:]].*|#define ${macro} ${newval}|" "$file"
    fi
}

# ── Branding: edit MICROPY_HW_BOARD_NAME ─────────────────────────────────────
_brand_edit_board_name() {
    if [[ ! -f "$MPCONFIGBOARD_H" ]]; then
        ui_msgbox "File Not Found" \
            "mpconfigboard.h not found:\n$MPCONFIGBOARD_H\n\nCannot edit board name."
        return 1
    fi

    local current; current=$(_read_define "$MPCONFIGBOARD_H" "MICROPY_HW_BOARD_NAME")
    local newval
    newval=$(ui_inputbox "Edit Board Name" \
        "Current: $current\n\nEnter new MICROPY_HW_BOARD_NAME:" "$current")

    [[ -z "$newval" || "$newval" == "$current" ]] && return 0

    if ui_yesno "Confirm Edit" \
        "Change MICROPY_HW_BOARD_NAME?\n\nFrom: $current\nTo  : $newval\n\nA backup will be created first."; then

        local backup; backup=$(_backup_file "$MPCONFIGBOARD_H")
        _patch_define "$MPCONFIGBOARD_H" "MICROPY_HW_BOARD_NAME" "$newval"

        local ts; ts=$(date '+%Y%m%d_%H%M%S')
        LOG_BRAND="${LOG_BRAND:-$LOG_DIR/brand_${ts}.log}"
        mkdir -p "$LOG_DIR"
        log_write "$LOG_BRAND" INFO "BOARD_NAME: '$current' → '$newval'"
        log_write "$LOG_BRAND" INFO "Backup: $backup"

        ui_msgbox "Board Name Updated" \
            "MICROPY_HW_BOARD_NAME updated.\n\nFrom: $current\nTo  : $newval\n\nBackup: $backup"
    fi
}

# ── Branding: edit MICROPY_HW_MCU_NAME ───────────────────────────────────────
_brand_edit_mcu_name() {
    if [[ ! -f "$MPCONFIGBOARD_H" ]]; then
        ui_msgbox "File Not Found" \
            "mpconfigboard.h not found:\n$MPCONFIGBOARD_H\n\nCannot edit MCU name."
        return 1
    fi

    local current; current=$(_read_define "$MPCONFIGBOARD_H" "MICROPY_HW_MCU_NAME")
    local newval
    newval=$(ui_inputbox "Edit MCU Name" \
        "Current: $current\n\nEnter new MICROPY_HW_MCU_NAME:" "$current")

    [[ -z "$newval" || "$newval" == "$current" ]] && return 0

    if ui_yesno "Confirm Edit" \
        "Change MICROPY_HW_MCU_NAME?\n\nFrom: $current\nTo  : $newval\n\nA backup will be created first."; then

        local backup; backup=$(_backup_file "$MPCONFIGBOARD_H")
        _patch_define "$MPCONFIGBOARD_H" "MICROPY_HW_MCU_NAME" "$newval"

        local ts; ts=$(date '+%Y%m%d_%H%M%S')
        LOG_BRAND="${LOG_BRAND:-$LOG_DIR/brand_${ts}.log}"
        mkdir -p "$LOG_DIR"
        log_write "$LOG_BRAND" INFO "MCU_NAME: '$current' → '$newval'"
        log_write "$LOG_BRAND" INFO "Backup: $backup"

        ui_msgbox "MCU Name Updated" \
            "MICROPY_HW_MCU_NAME updated.\n\nFrom: $current\nTo  : $newval\n\nBackup: $backup"
    fi
}

# ── Branding: set Zeno OS version string ─────────────────────────────────────
# Writes a MICROPY_ZENO_VERSION define to mpconfigboard.h (or mpconfig.h
# if a ZENO_VERSION define already exists there).
_brand_set_zeno_version() {
    local target_file="$MPCONFIGBOARD_H"
    [[ ! -f "$target_file" ]] && target_file="$MPCONFIG_H"
    if [[ ! -f "$target_file" ]]; then
        ui_msgbox "File Not Found" "Neither mpconfigboard.h nor mpconfig.h found.\nCannot set Zeno version."
        return 1
    fi

    local current="unknown"
    current=$(_read_define "$target_file" "MICROPY_ZENO_VERSION" 2>/dev/null || echo "unknown")
    # Also check if it is a comment line
    if grep -q 'MICROPY_ZENO_VERSION' "$target_file" 2>/dev/null; then
        current=$(grep -m1 'MICROPY_ZENO_VERSION' "$target_file" | sed 's/.*"\(.*\)".*/\1/')
    fi

    local newval
    newval=$(ui_inputbox "Set Zeno Version" \
        "Current MICROPY_ZENO_VERSION: $current\n\nEnter version string (e.g. 1.0.0):" \
        "${current/unknown/1.0.0}")

    [[ -z "$newval" ]] && return 0

    if ui_yesno "Confirm Version" \
        "Set MICROPY_ZENO_VERSION to \"$newval\"?\n\nFile: $target_file\n\nA backup will be created first."; then

        local backup; backup=$(_backup_file "$target_file")

        if grep -q 'MICROPY_ZENO_VERSION' "$target_file" 2>/dev/null; then
            _patch_define "$target_file" "MICROPY_ZENO_VERSION" "$newval"
        else
            # Append new define after last #define block
            printf '\n// ZENO OS Version\n#define MICROPY_ZENO_VERSION "%s"\n' "$newval" \
                >> "$target_file"
        fi

        local ts; ts=$(date '+%Y%m%d_%H%M%S')
        LOG_BRAND="${LOG_BRAND:-$LOG_DIR/brand_${ts}.log}"
        mkdir -p "$LOG_DIR"
        log_write "$LOG_BRAND" INFO "ZENO_VERSION: '$current' → '$newval'"
        log_write "$LOG_BRAND" INFO "Backup: $backup"

        ui_msgbox "Zeno Version Set" \
            "MICROPY_ZENO_VERSION set to: $newval\n\nBackup: $backup"
    fi
}

# ── Branding: preview rendered banner ────────────────────────────────────────
_brand_preview_banner() {
    local banner; banner=$(_render_banner)
    local board_name; board_name=$(_read_define "$MPCONFIGBOARD_H" "MICROPY_HW_BOARD_NAME")
    local mcu_name;   mcu_name=$(_read_define   "$MPCONFIGBOARD_H" "MICROPY_HW_MCU_NAME")
    local zeno_ver
    if [[ -f "$MPCONFIGBOARD_H" ]]; then
        zeno_ver=$(_read_define "$MPCONFIGBOARD_H" "MICROPY_ZENO_VERSION")
    fi
    [[ "${zeno_ver:-unknown}" == "unknown" && -f "$MPCONFIG_H" ]] && \
        zeno_ver=$(_read_define "$MPCONFIG_H" "MICROPY_ZENO_VERSION")

    local -a lines=()
    lines+=("")
    lines+=("  Boot Banner Preview")
    lines+=("  ══════════════════════════════════════════════════════")
    lines+=("")
    lines+=("  As it will appear on the serial console at boot:")
    lines+=("")
    lines+=("  ┌─────────────────────────────────────────────────┐")
    lines+=("  │  $banner")
    lines+=("  │  Type \"help()\" for more information.")
    lines+=("  │  >>> _")
    lines+=("  └─────────────────────────────────────────────────┘")
    lines+=("")
    lines+=("  Component Values")
    lines+=("  ─────────────────")
    lines+=("  Board Name    : $board_name")
    lines+=("  MCU Name      : $mcu_name")
    lines+=("  Zeno Version  : ${zeno_ver:-not set}")
    lines+=("")
    lines+=("  Source: $MPCONFIGBOARD_H")
    lines+=("")

    ui_status_screen "Banner Preview" "${lines[@]}"
}

# ── Branding menu ─────────────────────────────────────────────────────────────
branding_ui() {
    while true; do
        local board_name mcu_name zeno_ver
        board_name=$(_read_define "$MPCONFIGBOARD_H" "MICROPY_HW_BOARD_NAME")
        mcu_name=$(_read_define   "$MPCONFIGBOARD_H" "MICROPY_HW_MCU_NAME")
        zeno_ver="not set"
        if [[ -f "$MPCONFIGBOARD_H" ]]; then
            zeno_ver=$(_read_define "$MPCONFIGBOARD_H" "MICROPY_ZENO_VERSION")
        fi

        local -a items=(
            "IDENTITY"  "View Firmware Identity"
            "BOARD"     "Edit Board Name  [$board_name]"
            "MCU"       "Edit MCU Name    [$mcu_name]"
            "VERSION"   "Set Zeno Version [$zeno_ver]"
            "BANNER"    "Preview Boot Banner"
            "BACK"      "← Return to main menu"
        )

        local choice
        choice=$(ui_menu "Branding & Identity" "${items[@]}")

        case "$choice" in
            IDENTITY)  firmware_identity_ui ;;
            BOARD)     _brand_edit_board_name ;;
            MCU)       _brand_edit_mcu_name ;;
            VERSION)   _brand_set_zeno_version ;;
            BANNER)    _brand_preview_banner ;;
            BACK|"")   return 0 ;;
        esac
    done
}

# =============================================================================
# §C  PARTITION MANAGER
# =============================================================================
# Detects active partition table from build sdkconfig (read-only).
# Allows switching between known partition CSV files by patching
# mpconfigboard.h or the board-level Makefile fragment — never the
# MicroPython source Makefiles.
# Validates CSV exists before applying.  Always backs up before writing.
# =============================================================================

# ── Read currently active CSV from build sdkconfig (best source) ─────────────
# Falls back to scanning mpconfigboard.h / board Makefile for PARTITION_TABLE
partition_detect_active() {
    # 1. Best source: generated build sdkconfig
    if [[ -f "$BUILD_SDKCONFIG" ]]; then
        local val
        val=$(grep -m1 'CONFIG_PARTITION_TABLE_CUSTOM_FILENAME=' "$BUILD_SDKCONFIG" \
              2>/dev/null | cut -d= -f2 | sed 's/"//g')
        [[ -n "$val" ]] && echo "$val" && return
        val=$(grep -m1 'CONFIG_PARTITION_TABLE_FILENAME=' "$BUILD_SDKCONFIG" \
              2>/dev/null | cut -d= -f2 | sed 's/"//g')
        [[ -n "$val" ]] && echo "$val" && return
    fi

    # 2. Fallback: scan board Makefile fragments
    for f in "$BOARD_DIR/mpconfigboard.mk" "$BOARD_DIR/board.cmake" \
             "$BOARD_DIR/Makefile"; do
        if [[ -f "$f" ]]; then
            local val
            val=$(grep -m1 'PARTITION_TABLE\|PARTITIONS' "$f" 2>/dev/null \
                  | grep -o '[a-zA-Z0-9._-]*\.csv' | head -1)
            [[ -n "$val" ]] && echo "$val" && return
        fi
    done

    # 3. Settings override
    if [[ -n "$CFG_PARTITION_CSV" ]]; then
        echo "$CFG_PARTITION_CSV"
        return
    fi

    echo "unknown (build first)"
}

# ── Scan for partition CSVs in ports/esp32 ───────────────────────────────────
partition_scan_csvs() {
    find "$PORT_DIR" -maxdepth 1 -name '*.csv' -printf '%f\n' 2>/dev/null | sort
}

# ── Render a human-readable partition layout from a CSV ──────────────────────
_partition_render_csv() {
    local csv_path="$1"
    [[ ! -f "$csv_path" ]] && echo "  File not found: $csv_path" && return

    local -a lines=()
    lines+=("  Partition Table: $(basename "$csv_path")")
    lines+=("  ──────────────────────────────────────────────────────")
    lines+=("")
    lines+=("  $(printf '%-18s %-8s %-10s %-10s %s' 'Name' 'Type' 'SubType' 'Offset' 'Size')")
    lines+=("  $(printf '%s' '─────────────────────────────────────────────────────────────')")

    while IFS=',' read -r name type subtype offset size flags; do
        # Skip comment and blank lines
        [[ "$name" =~ ^[[:space:]]*# || -z "$name" ]] && continue
        name="${name// /}"
        type="${type// /}"
        subtype="${subtype// /}"
        offset="${offset// /}"
        size="${size// /}"
        lines+=("  $(printf '%-18s %-8s %-10s %-10s %s' "$name" "$type" "$subtype" "$offset" "$size")")
    done < "$csv_path"

    lines+=("")
    printf '%s\n' "${lines[@]}"
}

# ── Apply a partition CSV: write setting and persist ─────────────────────────
# Strategy: write into board mpconfigboard.mk (create if absent),
# as BOARD_PARTITION_TABLE_CSV = <name>.  The MicroPython build picks this up
# via the board's Makefile include chain without touching core source files.
_partition_apply() {
    local csv_name="$1"   # just the filename, e.g. partitions-8MiBplus-ota.csv
    local csv_path="$PORT_DIR/$csv_name"

    if [[ ! -f "$csv_path" ]]; then
        ui_msgbox "File Not Found" \
            "Partition CSV not found:\n$csv_path\n\nThe file must exist in ports/esp32/"
        return 1
    fi

    # Backup + patch mpconfigboard.mk
    local mk_file="$BOARD_DIR/mpconfigboard.mk"
    local backup=""

    # Create mpconfigboard.mk if it does not exist
    if [[ ! -f "$mk_file" ]]; then
        mkdir -p "$BOARD_DIR"
        {
            echo "# Board Makefile fragment — auto-managed by ZENO OS Firmware Manager"
            echo "# $(date -u '+%Y-%m-%d %H:%M:%S UTC')"
        } > "$mk_file"
    else
        backup=$(_backup_file "$mk_file")
    fi

    # Patch or append BOARD_PARTITION_TABLE_CSV
    if grep -q 'BOARD_PARTITION_TABLE_CSV\|PARTITION_TABLE_CSV' "$mk_file" 2>/dev/null; then
        sed -i "s|BOARD_PARTITION_TABLE_CSV.*|BOARD_PARTITION_TABLE_CSV = $csv_name|" "$mk_file"
        sed -i "s|PARTITION_TABLE_CSV.*|BOARD_PARTITION_TABLE_CSV = $csv_name|" "$mk_file"
    else
        echo "BOARD_PARTITION_TABLE_CSV = $csv_name" >> "$mk_file"
    fi

    # Persist choice to settings
    CFG_PARTITION_CSV="$csv_name"
    cfg_save

    local ts; ts=$(date '+%Y%m%d_%H%M%S')
    LOG_PART="${LOG_PART:-$LOG_DIR/partition_${ts}.log}"
    mkdir -p "$LOG_DIR"
    log_write "$LOG_PART" INFO "Partition CSV set to: $csv_name"
    log_write "$LOG_PART" INFO "mpconfigboard.mk: $mk_file"
    [[ -n "$backup" ]] && log_write "$LOG_PART" INFO "Backup: $backup"

    ui_msgbox "Partition Table Applied" \
        "Partition table set to:\n  $csv_name\n\nWritten to:\n  $mk_file\n\n\
A clean build is required for this change to take effect.\nUse: Build Automation → Clean Build."
}

# ── Partition manager UI ──────────────────────────────────────────────────────
partition_manager_ui() {
    while true; do
        local active; active=$(partition_detect_active)

        local -a items=(
            "STATUS"    "Current Status  [Active: $active]"
            "SHOW4M"    "Show 4MiB layout  (partitions-4MiBplus.csv)"
            "SHOW8M"    "Show 8MiB OTA layout  (partitions-8MiBplus-ota.csv)"
            "SWITCH"    "Switch Partition Table"
            "CUSTOM"    "Select from all detected CSVs"
            "BACK"      "← Return to main menu"
        )

        local choice
        choice=$(ui_menu "Partition Manager" "${items[@]}")

        case "$choice" in
            STATUS)
                local -a status_lines=()
                status_lines+=("")
                status_lines+=("  Partition Manager — Status")
                status_lines+=("  ════════════════════════════════════════════")
                status_lines+=("")
                status_lines+=("  Active partition table : $active")
                status_lines+=("  Settings override      : ${CFG_PARTITION_CSV:-auto-detect}")
                status_lines+=("")
                status_lines+=("  Known partition files:")
                for csv in $(partition_scan_csvs); do
                    local marker="  "
                    [[ "$csv" == "$active" ]] && marker="▶ "
                    status_lines+=("    ${marker}$csv")
                done
                status_lines+=("")
                status_lines+=("  Build sdkconfig        : $BUILD_SDKCONFIG")
                [[ -f "$BUILD_SDKCONFIG" ]] && \
                    status_lines+=("                           [EXISTS — source of active value]") || \
                    status_lines+=("                           [NOT BUILT YET]")
                status_lines+=("")
                ui_status_screen "Partition Status" "${status_lines[@]}"
                ;;

            SHOW4M)
                if [[ ! -f "$PARTITION_4MIB" ]]; then
                    ui_msgbox "File Not Found" \
                        "partitions-4MiBplus.csv not found:\n$PARTITION_4MIB"
                else
                    local -a plines=()
                    while IFS= read -r l; do plines+=("$l"); done \
                        < <(_partition_render_csv "$PARTITION_4MIB")
                    ui_status_screen "Partition Layout — 4MiB" "${plines[@]}"
                fi
                ;;

            SHOW8M)
                if [[ ! -f "$PARTITION_8MIB_OTA" ]]; then
                    ui_msgbox "File Not Found" \
                        "partitions-8MiBplus-ota.csv not found:\n$PARTITION_8MIB_OTA"
                else
                    local -a plines=()
                    while IFS= read -r l; do plines+=("$l"); done \
                        < <(_partition_render_csv "$PARTITION_8MIB_OTA")
                    ui_status_screen "Partition Layout — 8MiB OTA" "${plines[@]}"
                fi
                ;;

            SWITCH)
                local -a sw_items=(
                    "4MIB"    "partitions-4MiBplus.csv    (4MB flash, no OTA)"
                    "8MIB"    "partitions-8MiBplus-ota.csv (8MB flash + OTA)  [recommended for N16R8]"
                    "BACK"    "← Cancel"
                )
                local sw_choice
                sw_choice=$(ui_menu "Switch Partition Table" "${sw_items[@]}")
                case "$sw_choice" in
                    4MIB)
                        if ui_yesno "Confirm Switch" \
                            "Switch to partitions-4MiBplus.csv?\n\nActive: $active\n\nA clean build will be required."; then
                            _partition_apply "partitions-4MiBplus.csv"
                        fi
                        ;;
                    8MIB)
                        if ui_yesno "Confirm Switch" \
                            "Switch to partitions-8MiBplus-ota.csv?\n\nActive: $active\n\nA clean build will be required."; then
                            _partition_apply "partitions-8MiBplus-ota.csv"
                        fi
                        ;;
                    BACK|"") : ;;
                esac
                ;;

            CUSTOM)
                # Dynamic list from scanned CSVs
                local -a csv_items=()
                local csv_file
                while IFS= read -r csv_file; do
                    local marker=""
                    [[ "$csv_file" == "$active" ]] && marker=" [ACTIVE]"
                    csv_items+=("$csv_file" "${csv_file}${marker}")
                done < <(partition_scan_csvs)

                if [[ ${#csv_items[@]} -eq 0 ]]; then
                    ui_msgbox "No CSVs Found" \
                        "No .csv files found in:\n$PORT_DIR\n\nAdd partition CSV files to the ports/esp32 directory."
                    continue
                fi
                csv_items+=("BACK" "← Cancel")

                local csv_choice
                csv_choice=$(ui_menu "Select Partition CSV" "${csv_items[@]}")
                if [[ -n "$csv_choice" && "$csv_choice" != "BACK" ]]; then
                    # Show layout before applying
                    local -a preview_lines=()
                    while IFS= read -r l; do preview_lines+=("$l"); done \
                        < <(_partition_render_csv "$PORT_DIR/$csv_choice")
                    ui_status_screen "Partition Layout Preview — $csv_choice" "${preview_lines[@]}"

                    if ui_yesno "Apply Partition Table?" \
                        "Apply $csv_choice?\n\nActive: $active\n\nA clean build will be required."; then
                        _partition_apply "$csv_choice"
                    fi
                fi
                ;;

            BACK|"") return 0 ;;
        esac
    done
}

# =============================================================================
# §D  BUILD AUTOMATION
# =============================================================================
# Enhanced build/flash/monitor with:
#   • Auto ESP-IDF export.sh sourcing
#   • Clean build vs incremental build choice
#   • IDF export path auto-detection
#   • Return to TUI after every command
# =============================================================================

# ── Ensure ESP-IDF is loaded ──────────────────────────────────────────────────
# Returns 0 if already loaded, or sources export.sh and returns 0 on success.
# Returns 1 if IDF cannot be found.
_idf_ensure_loaded() {
    # Already loaded?
    if [[ -n "${IDF_PATH:-}" && -d "$IDF_PATH" ]]; then
        return 0
    fi

    # User has configured the export path
    local export_sh=""
    if [[ -n "$CFG_IDF_EXPORT" && -f "$CFG_IDF_EXPORT" ]]; then
        export_sh="$CFG_IDF_EXPORT"
    fi

    # Auto-detect common locations
    if [[ -z "$export_sh" ]]; then
        local -a search_paths=(
            "$HOME/esp/esp-idf/export.sh"
            "$HOME/esp-idf/export.sh"
            "/opt/esp/idf/export.sh"
            "/opt/esp-idf/export.sh"
        )
        for p in "${search_paths[@]}"; do
            if [[ -f "$p" ]]; then
                export_sh="$p"
                break
            fi
        done
    fi

    if [[ -z "$export_sh" ]]; then
        ui_msgbox "ESP-IDF Not Found" \
            "ESP-IDF is not loaded and could not be auto-detected.\n\n\
Common locations checked:\n\
  ~/esp/esp-idf/export.sh\n\
  ~/esp-idf/export.sh\n\
  /opt/esp-idf/export.sh\n\n\
Options:\n\
1. Run:  source /path/to/esp-idf/export.sh\n\
   then re-launch the firmware manager.\n\
2. Set the export.sh path in Settings → IDF Export Path."
        return 1
    fi

    # Source in a subshell to test, then in current shell
    if ! bash -c "source '$export_sh' >/dev/null 2>&1 && echo ok" 2>/dev/null | grep -q ok; then
        ui_msgbox "IDF Export Failed" \
            "Failed to source ESP-IDF export.sh:\n$export_sh\n\nCheck the file is valid."
        return 1
    fi

    # Source in current shell (affects this process and all children)
    # shellcheck disable=SC1090
    source "$export_sh" >/dev/null 2>&1 || {
        ui_msgbox "IDF Export Failed" "Could not source:\n$export_sh"
        return 1
    }

    ui_infobox "ESP-IDF Loaded" "Sourced: $export_sh"
    return 0
}

# ── Clean build ───────────────────────────────────────────────────────────────
build_clean() {
    _idf_ensure_loaded || return 1

    if ! ui_yesno "Clean Build" \
        "Perform a CLEAN build?\n\n\
This will:\n\
  1. Delete build-$CFG_BOARD/ completely\n\
  2. Run: make BOARD=$CFG_BOARD submodules\n\
  3. Run: make BOARD=$CFG_BOARD\n\n\
Estimated time: 3–10 minutes."; then
        return 0
    fi

    local ts; ts=$(date '+%Y%m%d_%H%M%S')
    local log_file="$LOG_DIR/build_${ts}.log"
    LOG_BUILD="$log_file"
    mkdir -p "$LOG_DIR"

    {
        echo "# ZENO OS Firmware Manager — Clean Build Log"
        echo "# Started  : $(date -u '+%Y-%m-%d %H:%M:%S UTC')"
        echo "# Board    : $CFG_BOARD"
        echo "# IDF      : ${IDF_PATH:-unset}"
        echo "# Type     : CLEAN"
        echo "# ─────────────────────────────────────────────"
        echo ""
    } > "$log_file"

    # Step 1: remove build directory
    ui_infobox "Clean Build" "Removing $BUILD_DIR…"
    if [[ -d "$BUILD_DIR" ]]; then
        rm -rf "$BUILD_DIR"
        log_write "$log_file" INFO "Removed $BUILD_DIR"
    fi

    cd "$PORT_DIR"

    # Step 2: submodules
    log_write "$log_file" CMD "make BOARD=$CFG_BOARD submodules"
    {
        echo "# ── make submodules ──"
        make BOARD="$CFG_BOARD" submodules 2>&1
        echo "# ── submodules done ──"
    } >> "$log_file"

    # Step 3: main build with progress gauge
    log_write "$log_file" CMD "make BOARD=$CFG_BOARD"

    local build_start; build_start=$(date +%s)
    local rc=0

    make BOARD="$CFG_BOARD" >> "$log_file" 2>&1 &
    local make_pid=$!

    (
        local elapsed=0
        while kill -0 "$make_pid" 2>/dev/null; do
            (( elapsed++ )) || true
            local pct
            pct=$(echo "scale=0; 95 - (95 / (1 + $elapsed / 45))" | bc 2>/dev/null || \
                  echo $(( 95 < elapsed ? 95 : elapsed )) )
            echo "$pct"
            sleep 1
        done
        echo 100
    ) | ui_gauge "Clean Build in Progress" "Building for $CFG_BOARD"

    wait "$make_pid" && rc=0 || rc=$?

    local duration=$(( $(date +%s) - build_start ))
    echo "" >> "$log_file"
    echo "# Finished: $(date -u '+%Y-%m-%d %H:%M:%S UTC')  Duration: ${duration}s  RC: $rc" \
        >> "$log_file"

    _build_show_result "$rc" "$duration" "$log_file" "Clean Build"
}

# ── Incremental build ─────────────────────────────────────────────────────────
build_incremental() {
    _idf_ensure_loaded || return 1

    if ! ui_yesno "Incremental Build" \
        "Perform an INCREMENTAL build?\n\n\
Runs: make BOARD=$CFG_BOARD\n\
(Uses existing build artefacts — faster)\n\n\
Use this for day-to-day development."; then
        return 0
    fi

    local ts; ts=$(date '+%Y%m%d_%H%M%S')
    local log_file="$LOG_DIR/build_${ts}.log"
    LOG_BUILD="$log_file"
    mkdir -p "$LOG_DIR"

    {
        echo "# ZENO OS Firmware Manager — Incremental Build Log"
        echo "# Started  : $(date -u '+%Y-%m-%d %H:%M:%S UTC')"
        echo "# Board    : $CFG_BOARD"
        echo "# IDF      : ${IDF_PATH:-unset}"
        echo "# Type     : INCREMENTAL"
        echo "# ─────────────────────────────────────────────"
        echo ""
    } > "$log_file"

    cd "$PORT_DIR"
    log_write "$log_file" CMD "make BOARD=$CFG_BOARD"

    local build_start; build_start=$(date +%s)
    local rc=0

    make BOARD="$CFG_BOARD" >> "$log_file" 2>&1 &
    local make_pid=$!

    (
        local elapsed=0
        while kill -0 "$make_pid" 2>/dev/null; do
            (( elapsed++ )) || true
            local pct
            pct=$(echo "scale=0; 95 - (95 / (1 + $elapsed / 20))" | bc 2>/dev/null || \
                  echo $(( 95 < elapsed*3 ? 95 : elapsed*3 )) )
            echo "$pct"
            sleep 1
        done
        echo 100
    ) | ui_gauge "Building (Incremental)" "make BOARD=$CFG_BOARD"

    wait "$make_pid" && rc=0 || rc=$?

    local duration=$(( $(date +%s) - build_start ))
    echo "" >> "$log_file"
    echo "# Finished: $(date -u '+%Y-%m-%d %H:%M:%S UTC')  Duration: ${duration}s  RC: $rc" \
        >> "$log_file"

    _build_show_result "$rc" "$duration" "$log_file" "Incremental Build"
}

# ── Shared build result display ───────────────────────────────────────────────
_build_show_result() {
    local rc="$1" duration="$2" log_file="$3" label="$4"

    if [[ "$rc" -eq 0 ]]; then
        local boot_sz pt_sz fw_sz
        boot_sz=$(du -h "$FW_BOOTLOADER"  2>/dev/null | cut -f1 || echo "?")
        pt_sz=$(du -h   "$FW_PARTITION"   2>/dev/null | cut -f1 || echo "?")
        fw_sz=$(du -h   "$FW_MICROPYTHON" 2>/dev/null | cut -f1 || echo "?")

        local -a ok=()
        ok+=("") ok+=("  ✔  $label succeeded!")  ok+=("")
        ok+=("  Board      : $CFG_BOARD")
        ok+=("  Duration   : ${duration}s")
        ok+=("  Completed  : $(date '+%Y-%m-%d %H:%M:%S')")
        ok+=("")
        ok+=("  Firmware images:")
        ok+=("    bootloader.bin        $boot_sz")
        ok+=("    partition-table.bin   $pt_sz")
        ok+=("    micropython.bin       $fw_sz")
        ok+=("")
        ok+=("  Log: $log_file")  ok+=("")
        ui_status_screen "$label — Success" "${ok[@]}"
    else
        local -a err=()
        err+=("") err+=("  ✘  $label FAILED  (exit $rc)")  err+=("")
        err+=("  Duration  : ${duration}s")
        err+=("  Log       : $log_file")  err+=("")
        err+=("  Last 20 lines:")
        err+=("  ───────────────────────────────────")
        while IFS= read -r line; do
            err+=("  $line")
        done < <(grep -v '^#' "$log_file" | tail -20)
        err+=("")
        ui_status_screen "$label — Failed" "${err[@]}"

        if ui_yesno "View Full Log?" "View the complete build log?"; then
            ui_textbox "$label Log" "$log_file"
        fi
    fi
}

# ── Build Automation menu ─────────────────────────────────────────────────────
build_automation_ui() {
    while true; do
        local idf_status="not loaded"
        [[ -n "${IDF_PATH:-}" ]] && idf_status="v$(cat "$IDF_PATH/version.txt" 2>/dev/null | head -1) — loaded"

        local port_status="${SELECTED_PORT:-auto-detect}"
        local fw_status="not built"
        [[ -f "$FW_MICROPYTHON" ]] && fw_status="built $(stat -c '%y' "$FW_MICROPYTHON" 2>/dev/null | cut -d. -f1)"

        local -a items=(
            "CLEAN"      "Clean Build     (rm build + make)"
            "INCREMENTAL" "Incremental Build (make only)"
            "FLASH"      "Flash Firmware  [fw: $fw_status]"
            "MONITOR"    "Serial Monitor"
            "IDF"        "Load ESP-IDF    [$idf_status]"
            "BACK"       "← Return to main menu"
        )

        local choice
        choice=$(ui_menu "Build Automation" "${items[@]}")

        case "$choice" in
            CLEAN)       build_clean ;;
            INCREMENTAL) build_incremental ;;
            FLASH)
                _idf_ensure_loaded || continue
                serial_detect || continue
                flash_firmware
                ;;
            MONITOR)
                serial_detect || continue
                serial_monitor_launch "$SELECTED_PORT"
                ;;
            IDF)
                if [[ -n "${IDF_PATH:-}" ]]; then
                    ui_msgbox "ESP-IDF Status" \
                        "ESP-IDF is already loaded.\n\nIDF_PATH: $IDF_PATH\n\nVersion: $(cat "$IDF_PATH/version.txt" 2>/dev/null | head -1)"
                else
                    _idf_ensure_loaded
                fi
                ;;
            BACK|"") return 0 ;;
        esac
    done
}

# =============================================================================
# §16  WELCOME SCREEN + MAIN MENU + ENTRY POINT
# =============================================================================

# ── Welcome / splash screen ──────────────────────────────────────────────────
show_welcome() {
    # Get dynamic values
    local branch="unknown"
    command -v git >/dev/null 2>&1 && [[ -d "$REPO/.git" ]] && \
        branch=$(git -C "$REPO" branch --show-current 2>/dev/null || echo "unknown")

    local idf_ver="not loaded"
    [[ -n "${IDF_PATH:-}" && -f "$IDF_PATH/version.txt" ]] && \
        idf_ver="v$(cat "$IDF_PATH/version.txt" | head -1)"
    [[ -n "${IDF_VERSION:-}" ]] && idf_ver="v$IDF_VERSION"

    local now; now=$(date '+%Y-%m-%d %H:%M:%S')

    # Build welcome message — whiptail renders \n as real newlines in msgbox
    local msg
    msg="Welcome to the ZENO OS Firmware Manager\n"
    msg+="Version $SCRIPT_VERSION\n"
    msg+="\n"
    msg+="  Project   : ZENO OS / MicroPython ESP32-S3\n"
    msg+="  Repo      : $REPO\n"
    msg+="  Branch    : $branch\n"
    msg+="  Date/Time : $now\n"
    msg+="  ESP-IDF   : $idf_ver\n"
    msg+="  User      : ${USER:-$(id -un)}\n"
    msg+="  UI        : $UI_BACKEND\n"
    msg+="\n"
    msg+="  Board     : $CFG_BOARD\n"
    msg+="  Target    : ESP32-S3 N16R8 (16MB / Octal PSRAM 8MB)\n"
    msg+="\n"
    msg+="  Capabilities:\n"
    msg+="  Build Automation • Branding • Partition Manager\n"
    msg+="  Flash • Monitor • Environment Verification\n"

    ui_msgbox "ZENO OS Firmware Manager v$SCRIPT_VERSION" "$msg"
}

# ── Main menu ─────────────────────────────────────────────────────────────────
main_menu() {
    while true; do
        # Dynamic status indicators
        local fw_tag="  "
        [[ -f "$FW_MICROPYTHON" ]] && fw_tag="✔ "
        local idf_tag="  "
        [[ -n "${IDF_PATH:-}" ]] && idf_tag="✔ "

        local -a items=(
            "1"  "${fw_tag}Build Automation  (Clean / Incremental / Flash / Monitor)"
            "2"  "Branding & Identity        (Board Name / MCU / Version / Banner)"
            "3"  "Partition Manager          (Layout / Switch / Validate)"
            "4"  "Flash Firmware             (flash only)"
            "5"  "Serial Monitor"
            "6"  "Verify IDF Environment"
            "7"  "Repair Board Configuration (PSRAM / SPIRAM check)"
            "8"  "View Build Information"
            "9"  "View Logs"
            "10" "Clean Build Directory"
            "11" "Full Automated Workflow"
            "12" "Settings"
            "13" "Exit"
        )

        local choice
        choice=$(ui_menu "ZENO OS Firmware Manager v${SCRIPT_VERSION} — Main Menu" "${items[@]}")

        case "$choice" in
            1)  build_automation_ui ;;
            2)  branding_ui ;;
            3)  partition_manager_ui ;;
            4)
                serial_detect || continue
                flash_firmware
                ;;
            5)  serial_monitor_ui ;;
            6)  env_verify 1 ;;
            7)  board_config_ui ;;
            8)  build_info_ui ;;
            9)  log_viewer_ui ;;
            10) clean_build_ui ;;
            11) workflow_auto ;;
            12) settings_ui ;;
            13|"")
                if ui_yesno "Exit" "Exit ZENO OS Firmware Manager?"; then
                    exit 0
                fi
                ;;
        esac
    done
}

# ── Entry point ───────────────────────────────────────────────────────────────
main() {
    # Safety check first
    require_repo

    # Init UI
    ui_init

    # Load settings (populates CFG_* vars)
    cfg_load

    # Init logging for this session
    init_log_session

    # Welcome screen
    show_welcome

    # Main menu loop (returns only on Exit or fatal error)
    main_menu
}

main "$@"
