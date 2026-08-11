#!/usr/bin/env bash
# =============================================================================
# ZENO OS FIRMWARE MANAGER
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

readonly SCRIPT_VERSION="1.0.0"
readonly SCRIPT_NAME="ZENO OS Firmware Manager"
readonly SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# ── Repository Layout ────────────────────────────────────────────────────────
# Script lives at OSBuild/builder/zeno_firmware_manager.sh
# REPO  = OSBuild/
REPO="${REPO:-$(dirname "$SCRIPT_DIR")}"
readonly PORT_DIR="$REPO/ports/esp32"
readonly BOARD="ESP32_GENERIC_S3"
readonly BUILD_DIR="$PORT_DIR/build-${BOARD}"
readonly SDKCONFIG_SPIRAM="$PORT_DIR/boards/sdkconfig.spiram_sx"
readonly LOG_DIR="$REPO/logs"
readonly SETTINGS_FILE="$REPO/.config/firmware_manager.conf"

# ── Firmware Binary Paths (standard MicroPython ESP32 layout) ────────────────
readonly FW_BOOTLOADER="$BUILD_DIR/bootloader/bootloader.bin"
readonly FW_PARTITION="$BUILD_DIR/partition_table/partition-table.bin"
readonly FW_MICROPYTHON="$BUILD_DIR/micropython.bin"

# ── Default Settings (overridden by settings file) ──────────────────────────
DEFAULT_BOARD="ESP32_GENERIC_S3"
DEFAULT_PORT=""                  # empty = auto-detect
DEFAULT_BAUD="460800"
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
    rows=$(tput lines  2>/dev/null || echo 24)
    cols=$(tput cols   2>/dev/null || echo 80)
    UI_H=$(( rows - 2 ))
    UI_W=$(( cols - 4 ))
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
    # Populate with defaults first
    CFG_BOARD="$DEFAULT_BOARD"
    CFG_PORT="$DEFAULT_PORT"
    CFG_BAUD="$DEFAULT_BAUD"
    CFG_AUTO_MONITOR="$DEFAULT_AUTO_MONITOR"
    CFG_LOG_RETENTION="$DEFAULT_LOG_RETENTION"

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
    for prefix in build flash repair auto session; do
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
    lines+=("  Flash size    : 4MB")
    lines+=("  Flash freq    : 80m")
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
        "Flash firmware to $port?\n\nThis will erase and reprogram the board.\nBoard must be connected."; then
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
        --before default_reset
        --after hard_reset
        write_flash
        --flash_mode dio
        --flash_size 4MB
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
    local idf_ver="not loaded"
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
    local flash_mode="dio / 80MHz / 4MB (hardcoded)"

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
    lines+=("  ESP-IDF version : $idf_ver")
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
            "PORT"     "Default serial port [$CFG_PORT]"
            "BAUD"     "Flash baud rate     [$CFG_BAUD]"
            "MONITOR"  "Auto-open monitor   [$CFG_AUTO_MONITOR]"
            "LOGS"     "Log retention count [$CFG_LOG_RETENTION files]"
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
                    "921600" "921600 — maximum speed"
                    "460800" "460800 — recommended (default)"
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
            SAVE)
                cfg_save
                ui_msgbox "Settings Saved" "Settings written to:\n$SETTINGS_FILE"
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
    msg+="  Target    : ESP32-S3 N16R8 (Octal PSRAM)\n"

    ui_msgbox "ZENO OS Firmware Manager v$SCRIPT_VERSION" "$msg"
}

# ── Main menu ─────────────────────────────────────────────────────────────────
main_menu() {
    while true; do
        # Build dynamic status tags for certain items
        local port_label="${SELECTED_PORT:-auto-detect}"
        local build_label="Build Firmware"
        [[ -f "$FW_MICROPYTHON" ]] && build_label="Build Firmware  [built]"

        local -a items=(
            "1"  "Build Firmware"
            "2"  "Flash Firmware"
            "3"  "Build + Flash Firmware"
            "4"  "Serial Monitor"
            "5"  "Verify Environment"
            "6"  "Repair Board Configuration"
            "7"  "View Build Information"
            "8"  "View Logs"
            "9"  "Clean Build Directory"
            "10" "Full Automated Workflow"
            "11" "Settings"
            "12" "Exit"
        )

        local choice
        choice=$(ui_menu "ZENO OS Firmware Manager — Main Menu" "${items[@]}")

        case "$choice" in
            1)
                build_firmware
                ;;
            2)
                serial_detect
                [[ -n "$SELECTED_PORT" ]] && flash_firmware
                ;;
            3)
                build_firmware || continue
                serial_detect  || continue
                flash_firmware
                ;;
            4)
                serial_monitor_ui
                ;;
            5)
                env_verify 1
                ;;
            6)
                board_config_ui
                ;;
            7)
                build_info_ui
                ;;
            8)
                log_viewer_ui
                ;;
            9)
                clean_build_ui
                ;;
            10)
                workflow_auto
                ;;
            11)
                settings_ui
                ;;
            12|"")
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
    IDF_EXPORT="$HOME/esp/esp-idf/export.sh"

    if [[ -f "$IDF_EXPORT" ]]; then
    	source "$IDF_EXPORT"
    fi
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
