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
#   §08  Git & Firmware Release Subsystem
#   §09  Build firmware
#   §10  Flash firmware
#   §11  Serial monitor
#   §12  Build information
#   §13  Log viewer
#   §14  Clean build
#   §15  Full automated workflow
#   §16  Settings menu
#   §17  Firmware Identity & Branding
#   §18  Partition Manager
#   §19  Build Automation Menu
#   §20  Main menu + entry point
# =============================================================================

set -uo pipefail
IFS=$'\n\t'

# =============================================================================
# §00  CONSTANTS & PATHS
# =============================================================================

readonly SCRIPT_VERSION="2.2.0"
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
readonly RELEASES_DIR="$REPO/firmware releases"

# ── Known source file locations (auto-detected; fall back to these) ──────────
readonly BOARD_DIR="$PORT_DIR/boards/${BOARD}"
readonly MPCONFIGBOARD_H="$BOARD_DIR/mpconfigboard.h"   # MICROPY_HW_BOARD_NAME etc.
readonly MPCONFIG_H="$REPO/py/mpconfig.h"               # MICROPY_BANNER_NAME macro
readonly MAKEVERSIONHDR="$REPO/py/makeversionhdr.py"    # version generator

# ── Partition table candidates ───────────────────────────────────────────────
readonly PARTITION_4MIB="$PORT_DIR/partitions-4MiBplus.csv"
readonly PARTITION_8MIB_OTA="$PORT_DIR/partitions-8MiBplus-ota.csv"

# ── Firmware Binary Paths (standard MicroPython ESP32 layout) ────────────────
readonly FW_BOOTLOADER="$BUILD_DIR/bootloader/bootloader.bin"
readonly FW_PARTITION="$BUILD_DIR/partition_table/partition-table.bin"
readonly FW_MICROPYTHON="$BUILD_DIR/micropython.bin"
readonly BUILD_SDKCONFIG="$BUILD_DIR/sdkconfig"

# ── Default Settings (overridden by settings file) ──────────────────────────
DEFAULT_BOARD="ESP32_GENERIC_S3"
DEFAULT_PORT=""
DEFAULT_BAUD="921600"
DEFAULT_AUTO_MONITOR="no"
DEFAULT_LOG_RETENTION="20"
DEFAULT_AUTO_GIT_PUSH="yes"

# ── Runtime state ────────────────────────────────────────────────────────────
SELECTED_PORT=""
SETTINGS_LOADED=0
CURRENT_BUILD_LABEL=""
CURRENT_COMMIT_MSG=""

# ── Log file references (set by init_log_session) ───────────────────────────
LOG_BUILD=""
LOG_FLASH=""
LOG_REPAIR=""
LOG_AUTO=""
LOG_SESSION=""
LOG_BRAND=""
LOG_PART=""
LOG_GIT=""

# =============================================================================
# §01  SAFETY & ERROR HANDLING
# =============================================================================

_cleanup() {
    local exit_code=$?
    rm -f /tmp/zfm_gauge_$$ /tmp/zfm_tmp_$$ 2>/dev/null || true
    tput cnorm 2>/dev/null || true
    tput rmcup 2>/dev/null || true
    [[ $exit_code -ne 0 ]] && echo ""
    exit $exit_code
}
trap _cleanup EXIT
trap 'exit 130' INT TERM

die() {
    tput cnorm 2>/dev/null || true
    echo -e "\n  \033[1;31m✘  FATAL: $*\033[0m\n" >&2
    exit 1
}

require_repo() {
    [[ -d "$PORT_DIR" ]] || die "ports/esp32 not found under REPO=$REPO\n  Set REPO= to your OSBuild path."
}

# =============================================================================
# §02  TERMINAL / UI ENGINE
# =============================================================================

UI_BACKEND=""
UI_W=0
UI_H=0

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

_wt() {
    case "$UI_BACKEND" in
        whiptail) whiptail "$@" 3>&1 1>&2 2>&3 ;;
        dialog)   dialog   "$@" 3>&1 1>&2 2>&3 ;;
    esac
}

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

ui_infobox() {
    local title="$1" msg="$2"
    case "$UI_BACKEND" in
        whiptail) whiptail --title " $title " --infobox "$msg" 8 "$UI_W" ;;
        dialog)   dialog   --title " $title " --infobox "$msg" 8 "$UI_W" ;;
        text)     echo -e "\n  ── $title ──\n$msg\n" | sed 's/^/  /' ;;
    esac
}

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

ui_gauge() {
    local title="$1" msg="${2:-Working…}"
    local h=7

    case "$UI_BACKEND" in
        whiptail) whiptail --title " $title " --gauge "$msg" "$h" "$UI_W" 0 ;;
        dialog)   dialog   --title " $title " --gauge "$msg" "$h" "$UI_W" 0 ;;
        text)
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

ui_textbox() {
    local title="$1" file="$2"
    [[ ! -f "$file" ]] && ui_msgbox "$title" "File not found:\n$file" && return

    case "$UI_BACKEND" in
        whiptail) whiptail --title " $title " --textbox "$file" "$UI_H" "$UI_W" ;;
        dialog)   dialog   --title " $title " --textbox "$file" "$UI_H" "$UI_W" ;;
        text)     less -F "$file" || true ;;
    esac
}

ui_wait_screen() {
    local title="$1" msg="$2"
    case "$UI_BACKEND" in
        whiptail) whiptail --title " $title " --infobox "\n$msg\n\n  Please wait…" 9 "$UI_W" ;;
        dialog)   dialog   --title " $title " --infobox "\n$msg\n\n  Please wait…" 9 "$UI_W" ;;
        text)     echo -e "\n  $title\n  $msg\n  Please wait…\n" ;;
    esac
}

ui_status_screen() {
    local title="$1"; shift
    local -a lines=("$@")
    local tmpfile
    tmpfile=$(mktemp /tmp/zfm_tmp_XXXX)
    printf '%s\n' "${lines[@]}" > "$tmpfile"
    ui_textbox "$title" "$tmpfile"
    rm -f "$tmpfile"
}

# =============================================================================
# §03  SETTINGS — load / save
# =============================================================================

cfg_load() {
    CFG_BOARD="$DEFAULT_BOARD"
    CFG_PORT="$DEFAULT_PORT"
    CFG_BAUD="$DEFAULT_BAUD"
    CFG_AUTO_MONITOR="$DEFAULT_AUTO_MONITOR"
    CFG_LOG_RETENTION="$DEFAULT_LOG_RETENTION"
    CFG_PARTITION_CSV=""
    CFG_IDF_EXPORT=""
    CFG_AUTO_GIT_PUSH="$DEFAULT_AUTO_GIT_PUSH"

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
                CFG_AUTO_GIT_PUSH)  CFG_AUTO_GIT_PUSH="$val" ;;
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
CFG_AUTO_GIT_PUSH=$CFG_AUTO_GIT_PUSH
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
    LOG_GIT="$LOG_DIR/git_${ts}.log"

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
    local level="$1"; shift
    echo "[$(date '+%H:%M:%S')] [$level] $*" >> "$log_file"
}

log_cmd() {
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
    for prefix in build flash repair auto session brand partition git; do
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

env_verify() {
    local show_ui="${1:-1}"
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

    if python3 --version >/dev/null 2>&1; then
        local pyver; pyver=$(python3 --version 2>&1)
        _chk "Python3" "pass" "$pyver"
    else
        _chk "Python3" "fail" "not found — install python3"
    fi

    if command -v make >/dev/null 2>&1; then
        _chk "make" "pass" "$(make --version 2>&1 | head -1)"
    else
        _chk "make" "fail" "not found — install build-essential"
    fi

    if command -v git >/dev/null 2>&1; then
        _chk "git" "pass" "$(git --version 2>&1)"
    else
        _chk "git" "warn" "not found (needed for git release tracking)"
    fi

    report+=("")
    report+=("  ESP Toolchain")
    report+=("  ─────────────")

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

    if python3 -m mpremote version >/dev/null 2>&1; then
        _chk "mpremote" "pass" "$(python3 -m mpremote version 2>&1 | head -1)"
    elif command -v mpremote >/dev/null 2>&1; then
        _chk "mpremote" "pass" "$(mpremote version 2>&1 | head -1)"
    else
        _chk "mpremote" "warn" "not found — pip install mpremote (needed for monitor)"
    fi

    report+=("")
    report+=("  Repository Structure")
    report+=("  ────────────────────")

    if [[ -d "$REPO" ]]; then
        _chk "Repository root" "pass" "$REPO"
    else
        _chk "Repository root" "fail" "not found: $REPO"
        all_ok=0
    fi

    if [[ -d "$PORT_DIR" ]]; then
        _chk "ports/esp32" "pass" ""
    else
        _chk "ports/esp32" "fail" "not found: $PORT_DIR"
        all_ok=0
    fi

    local board_dir="$PORT_DIR/boards/${CFG_BOARD}"
    if [[ -d "$board_dir" ]]; then
        _chk "Board directory" "pass" "$board_dir"
    else
        _chk "Board directory" "fail" "not found: $board_dir"
        all_ok=0
    fi

    if [[ -f "$PORT_DIR/Makefile" ]]; then
        _chk "ESP32 Makefile" "pass" ""
    else
        _chk "ESP32 Makefile" "fail" "not found: $PORT_DIR/Makefile"
        all_ok=0
    fi

    report+=("")
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

# shellcheck disable=SC2120
serial_detect() {
    local auto="${1:-0}"
    local -a ports=()

    for p in /dev/ttyACM* /dev/ttyUSB*; do
        [[ -e "$p" ]] && ports+=("$p")
    done

    if [[ ${#ports[@]} -eq 0 ]]; then
        ui_msgbox "Serial Port" \
            "No serial devices detected.\n\nChecked:\n  /dev/ttyACM*\n  /dev/ttyUSB*\n\nConnect your ESP32-S3 board and try again."
        SELECTED_PORT=""
        return 1
    fi

    if [[ -n "$CFG_PORT" && -e "$CFG_PORT" ]]; then
        SELECTED_PORT="$CFG_PORT"
        return 0
    fi

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

    local -a items=()
    for p in "${ports[@]}"; do
        local detail="USB Serial Device"
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

board_config_check() {
    if [[ ! -f "$SDKCONFIG_SPIRAM" ]]; then
        return 2
    fi

    local oct_ok=0 quad_disabled=0
    grep -q 'CONFIG_SPIRAM_MODE_OCT=y' "$SDKCONFIG_SPIRAM" 2>/dev/null && oct_ok=1
    if ! grep -q 'CONFIG_SPIRAM_MODE_QUAD=y' "$SDKCONFIG_SPIRAM" 2>/dev/null; then
        quad_disabled=1
    fi

    [[ $oct_ok -eq 1 && $quad_disabled -eq 1 ]] && return 0 || return 1
}

board_config_repair() {
    local ts; ts=$(date '+%Y%m%d_%H%M%S')
    local backup="${SDKCONFIG_SPIRAM}.backup_${ts}"

    log_section "$LOG_REPAIR" "Board Config Repair"
    log_write "$LOG_REPAIR" INFO "Repairing $SDKCONFIG_SPIRAM"
    log_write "$LOG_REPAIR" INFO "Backup → $backup"

    cp "$SDKCONFIG_SPIRAM" "$backup"

    if grep -q 'CONFIG_SPIRAM_MODE_OCT' "$SDKCONFIG_SPIRAM"; then
        sed -i 's/^.*CONFIG_SPIRAM_MODE_OCT.*$/CONFIG_SPIRAM_MODE_OCT=y/' "$SDKCONFIG_SPIRAM"
    else
        echo "CONFIG_SPIRAM_MODE_OCT=y" >> "$SDKCONFIG_SPIRAM"
    fi

    if grep -q 'CONFIG_SPIRAM_MODE_QUAD=y' "$SDKCONFIG_SPIRAM"; then
        sed -i 's/^CONFIG_SPIRAM_MODE_QUAD=y$/# CONFIG_SPIRAM_MODE_QUAD is not set/' "$SDKCONFIG_SPIRAM"
    fi

    log_write "$LOG_REPAIR" INFO "Repair complete"
    return 0
}

board_config_ui() {
    if [[ ! -f "$SDKCONFIG_SPIRAM" ]]; then
        ui_msgbox "Board Configuration" \
            "sdkconfig.spiram_sx not found:\n\n$SDKCONFIG_SPIRAM\n\nCannot verify or repair without it."
        return 1
    fi

    if board_config_check; then
        local -a lines=()
        lines+=("")
        lines+=("  Board configuration is CORRECT.")
        lines+=("")
        lines+=("  File: $SDKCONFIG_SPIRAM")
        lines+=("  CONFIG_SPIRAM_MODE_OCT=y                [OK]")
        lines+=("  CONFIG_SPIRAM_MODE_QUAD not set          [OK]")
        lines+=("")
        ui_status_screen "Board Configuration" "${lines[@]}"
        return 0
    fi

    if ui_yesno "Repair Board Configuration?" \
        "Configuration issue detected in sdkconfig.spiram_sx.\n\nRepair now? A backup will be created."; then
        ui_wait_screen "Repairing Configuration" "Writing corrected settings…"
        if board_config_repair; then
            ui_msgbox "Repair Complete" "Configuration repaired successfully.\n\nBackup created at:\n$SDKCONFIG_SPIRAM.backup_*"
        else
            ui_msgbox "Repair Failed" "Could not repair configuration.\n\nCheck $LOG_REPAIR."
            return 1
        fi
    fi
}

# =============================================================================
# §08  GIT & FIRMWARE RELEASE SUBSYSTEM
# =============================================================================

# Prompt for build tag and commit message before compilation starts
git_prompt_metadata() {
    local default_label="release"
    local now_str; now_str=$(date '+%Y-%m-%d %H:%M')
    local default_commit="Build: $CFG_BOARD ($now_str)"

    CURRENT_BUILD_LABEL=$(ui_inputbox "Build Label" \
        "Enter a custom label for this build (used for release folder naming):" "$default_label")
    [[ -z "$CURRENT_BUILD_LABEL" ]] && CURRENT_BUILD_LABEL="release"
    # Sanitize label (alphanumeric and dashes only)
    CURRENT_BUILD_LABEL=$(echo "$CURRENT_BUILD_LABEL" | tr -cd '[:alnum:]_-')

    CURRENT_COMMIT_MSG=$(ui_inputbox "Git Commit Message" \
        "Enter commit message for git add & push after build:" "$default_commit")
    [[ -z "$CURRENT_COMMIT_MSG" ]] && CURRENT_COMMIT_MSG="$default_commit"
}

# Create timestamped release directory, copy binaries, stage, commit, and push
git_release_workflow() {
    local ts; ts=$(date '+%Y%m%d_%H%M%S')
    local log_file="$LOG_DIR/git_${ts}.log"
    LOG_GIT="$log_file"
    mkdir -p "$LOG_DIR" "$RELEASES_DIR"

    # Dedicated folder for this specific release
    local release_folder_name="${ts}_${CURRENT_BUILD_LABEL}_${CFG_BOARD}"
    local target_release_dir="$RELEASES_DIR/$release_folder_name"
    mkdir -p "$target_release_dir"

    {
        echo "# ZENO OS Firmware Manager — Release & Git Log"
        echo "# Started : $(date -u '+%Y-%m-%d %H:%M:%S UTC')"
        echo "# Label   : $CURRENT_BUILD_LABEL"
        echo "# Folder  : $target_release_dir"
        echo "# Commit  : $CURRENT_COMMIT_MSG"
        echo "# ────────────────────────────────────────────"
    } > "$log_file"

    ui_wait_screen "Firmware Release" "Archiving binaries to:\n$release_folder_name…"

    # Copy files directly into their dedicated release subfolder
    cp -v "$FW_BOOTLOADER" "$target_release_dir/bootloader.bin" >> "$log_file" 2>&1 || true
    cp -v "$FW_PARTITION" "$target_release_dir/partition-table.bin" >> "$log_file" 2>&1 || true
    cp -v "$FW_MICROPYTHON" "$target_release_dir/micropython.bin" >> "$log_file" 2>&1 || true

    log_write "$log_file" INFO "Firmware binaries copied to: $target_release_dir"

    # Check git repository
    if ! command -v git >/dev/null 2>&1 || [[ ! -d "$REPO/.git" ]]; then
        ui_msgbox "Git Notice" "Git is not initialized in $REPO.\nBinaries safely archived in:\n$target_release_dir"
        return 0
    fi

    ui_wait_screen "Git Operations" "Staging files (git add .)…"
    cd "$REPO"

    git add . >> "$log_file" 2>&1
    log_write "$log_file" CMD "git add ."

    ui_wait_screen "Git Operations" "Committing changes…"
    local commit_output
    commit_output=$(git commit -m "$CURRENT_COMMIT_MSG" 2>&1) || true
    echo "$commit_output" >> "$log_file"
    log_write "$log_file" INFO "Committed: $CURRENT_COMMIT_MSG"

    if [[ "$CFG_AUTO_GIT_PUSH" == "yes" ]]; then
        ui_wait_screen "Git Operations" "Pushing to remote origin (main)…"
        local push_rc=0
        
        # Ensure we push directly to main branch
        local current_branch
        current_branch=$(git branch --show-current 2>/dev/null || echo "main")
        git push origin "$current_branch" >> "$log_file" 2>&1 || push_rc=$?

        if [[ $push_rc -eq 0 ]]; then
            log_write "$log_file" INFO "Git push succeeded."
            ui_msgbox "Release Complete" \
                "✔ Firmware built & released!\n\n1. Binaries folder:\n   firmware releases/$release_folder_name/\n\n2. Git commit:\n   '$CURRENT_COMMIT_MSG'\n\n3. Pushed successfully to origin/$current_branch!"
        else
            log_write "$log_file" ERROR "Git push failed (RC: $push_rc)"
            ui_msgbox "Git Push Failed" \
                "Files committed locally, but push failed.\n\nCheck log: $log_file\nRun 'git push' manually."
        fi
    else
        ui_msgbox "Release Archived" \
            "Binaries saved in:\nfirmware releases/$release_folder_name/\nand committed locally."
    fi
}

# =============================================================================
# §09  BUILD FIRMWARE
# =============================================================================

build_firmware() {
    local silent="${1:-0}"

    if [[ ! -d "$PORT_DIR" ]]; then
        ui_msgbox "Build Error" "ports/esp32 not found:\n$PORT_DIR"
        return 1
    fi

    if [[ -z "${IDF_PATH:-}" ]]; then
        _idf_ensure_loaded || return 1
    fi

    if [[ "$silent" == "0" ]]; then
        git_prompt_metadata
        ui_yesno "Build Firmware" \
            "Ready to build firmware.\n\nBoard: $CFG_BOARD\nLabel: $CURRENT_BUILD_LABEL\nCommit: $CURRENT_COMMIT_MSG\n\nBegin build?" \
            || return 0
    else
        [[ -z "$CURRENT_BUILD_LABEL" ]] && CURRENT_BUILD_LABEL="auto"
        [[ -z "$CURRENT_COMMIT_MSG" ]] && CURRENT_COMMIT_MSG="Automated build $(date '+%Y-%m-%d %H:%M')"
    fi

    local ts; ts=$(date '+%Y%m%d_%H%M%S')
    local log_file="$LOG_DIR/build_${ts}.log"
    LOG_BUILD="$log_file"
    mkdir -p "$LOG_DIR"

    {
        echo "# ZENO OS Firmware Manager — Build Log"
        echo "# Started : $(date -u '+%Y-%m-%d %H:%M:%S UTC')"
        echo "# Board   : $CFG_BOARD"
        echo "# Label   : $CURRENT_BUILD_LABEL"
        echo "# IDF     : ${IDF_PATH:-unset}"
        echo "# ────────────────────────────────────────────"
    } > "$log_file"

    local build_start; build_start=$(date +%s)
    local rc=0

    cd "$PORT_DIR"
    make BOARD="$CFG_BOARD" >> "$log_file" 2>&1 &
    local make_pid=$!

    (
        local elapsed=0
        local pct=0
        while kill -0 "$make_pid" 2>/dev/null; do
            (( elapsed++ )) || true
            pct=$(echo "scale=0; 95 - (95 / (1 + $elapsed / 30))" | bc 2>/dev/null || \
                  echo $(( 95 < elapsed*2 ? 95 : elapsed*2 )) )
            echo "$pct"
            sleep 1
        done
        echo 100
    ) | ui_gauge "Building Firmware" "make BOARD=$CFG_BOARD"

    wait "$make_pid" && rc=0 || rc=$?

    local build_end; build_end=$(date +%s)
    local duration=$(( build_end - build_start ))

    echo "" >> "$log_file"
    echo "# Build finished: $(date -u '+%Y-%m-%d %H:%M:%S UTC')  Duration: ${duration}s  RC: $rc" >> "$log_file"

    if [[ $rc -eq 0 ]]; then
        _build_show_result "$rc" "$duration" "$log_file" "Build"
        git_release_workflow
        return 0
    else
        _build_show_result "$rc" "$duration" "$log_file" "Build"
        return 1
    fi
}

# =============================================================================
# §10  FLASH FIRMWARE
# =============================================================================

_flash_summary_screen() {
    local port="$1"
    local fw_ts="not built"
    [[ -f "$FW_MICROPYTHON" ]] && \
        fw_ts=$(stat -c '%y' "$FW_MICROPYTHON" 2>/dev/null | cut -d. -f1 || echo "unknown")

    local -a lines=()
    lines+=("")
    lines+=("  Flash Firmware — Summary")
    lines+=("  ────────────────────────────────────────────────")
    lines+=("  Target Board  : $CFG_BOARD")
    lines+=("  Serial Port   : $port")
    lines+=("  Baud Rate     : $CFG_BAUD")
    lines+=("  Flash mode    : dio / 16MB / 80m")
    lines+=("")
    lines+=("  Firmware Images:")
    lines+=("  0x00000  bootloader.bin")
    lines+=("  0x08000  partition-table.bin")
    lines+=("  0x10000  micropython.bin")
    lines+=("")
    lines+=("  Build date    : $fw_ts")
    lines+=("")
    ui_status_screen "Flash Summary" "${lines[@]}"
}

_flash_validate() {
    if [[ ! -f "$FW_BOOTLOADER" || ! -f "$FW_PARTITION" || ! -f "$FW_MICROPYTHON" ]]; then
        ui_msgbox "Flash Error" "One or more firmware images are missing in:\n$BUILD_DIR\n\nPlease build first."
        return 1
    fi
    return 0
}

flash_firmware() {
    local silent="${1:-0}"
    local port="${SELECTED_PORT:-}"

    _flash_validate || return 1

    if [[ -z "$port" ]]; then
        serial_detect || return 1
        port="$SELECTED_PORT"
    fi

    [[ ! -e "$port" ]] && ui_msgbox "Flash Error" "Selected port does not exist:\n$port" && return 1

    _flash_summary_screen "$port"

    if ! ui_yesno "Confirm Flash" "Flash firmware to $port?\n\nBoard must be connected."; then
        return 0
    fi

    local ts; ts=$(date '+%Y%m%d_%H%M%S')
    local log_file="$LOG_DIR/flash_${ts}.log"
    LOG_FLASH="$log_file"
    mkdir -p "$LOG_DIR"

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

    ui_wait_screen "Flashing Firmware" "Writing to $port at ${CFG_BAUD} baud…"
    local rc=0
    "${cmd[@]}" >> "$log_file" 2>&1 || rc=$?

    if [[ $rc -eq 0 ]]; then
        ui_msgbox "Flash Complete" "✔ Firmware flashed successfully!\n\nPort : $port\nLog  : $log_file"
        if [[ "$CFG_AUTO_MONITOR" == "yes" ]]; then
            if ui_yesno "Open Monitor?" "Open serial monitor on $port now?"; then
                serial_monitor_launch "$port"
            fi
        fi
        return 0
    else
        ui_msgbox "Flash Failed" "✘ Flashing failed (RC: $rc).\nCheck log: $log_file"
        return 1
    fi
}

# =============================================================================
# §11  SERIAL MONITOR
# =============================================================================

serial_monitor_launch() {
    local port="${1:-$SELECTED_PORT}"

    if [[ -z "$port" ]]; then
        serial_detect || return 1
        port="$SELECTED_PORT"
    fi

    local -a items=(
        "mpremote"  "mpremote connect $port (recommended)"
        "picocom"   "picocom -b 115200 $port"
        "screen"    "screen $port 115200"
        "BACK"      "← Return to menu"
    )

    local choice
    choice=$(ui_menu "Serial Monitor — $port" "${items[@]}")

    case "$choice" in
        mpremote)
            clear
            echo -e "\033[1;36m  ZENO OS — Serial Monitor (mpremote)\033[0m"
            echo -e "  Port: $port  |  Press Ctrl+] to exit\n"
            python3 -m mpremote connect "$port" || true
            ;;
        picocom)
            clear
            echo -e "\033[1;36m  ZENO OS — Serial Monitor (picocom)\033[0m"
            echo -e "  Port: $port 115200  |  Press Ctrl+A Ctrl+X to exit\n"
            picocom -b 115200 "$port" || true
            ;;
        screen)
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
# §12  BUILD INFORMATION
# =============================================================================

build_info_ui() {
    local -a lines=()
    local branch="unknown" last_commit="unknown"
    if command -v git >/dev/null 2>&1 && [[ -d "$REPO/.git" ]]; then
        branch=$(git -C "$REPO" branch --show-current 2>/dev/null || echo "unknown")
        last_commit=$(git -C "$REPO" log -1 --format='%h %s' 2>/dev/null || echo "unknown")
    fi

    local build_ts="never"
    [[ -f "$FW_MICROPYTHON" ]] && build_ts=$(stat -c '%y' "$FW_MICROPYTHON" 2>/dev/null | cut -d. -f1)

    lines+=("")
    lines+=("  Build & Release Information")
    lines+=("  ─────────────────────────────────────────────────────")
    lines+=("  Repository Path : $REPO")
    lines+=("  Branch          : $branch")
    lines+=("  Latest Commit   : $last_commit")
    lines+=("  Releases Root   : $RELEASES_DIR")
    lines+=("")
    lines+=("  Board           : $CFG_BOARD")
    lines+=("  Build Dir       : $BUILD_DIR")
    lines+=("  Latest Build    : $build_ts")
    lines+=("")
    ui_status_screen "Build Information" "${lines[@]}"
}

# =============================================================================
# §13  LOG VIEWER
# =============================================================================

log_viewer_ui() {
    while true; do
        local -a items=()
        for prefix in build flash repair auto session brand partition git; do
            local -a logs
            mapfile -t logs < <(
                find "$LOG_DIR" -maxdepth 1 -name "${prefix}_*.log" \
                    -printf '%T@ %p\n' 2>/dev/null | sort -rn | \
                    head -5 | awk '{print $2}')
            for f in "${logs[@]}"; do
                local ts_label; ts_label=$(basename "$f" .log | sed "s/${prefix}_//")
                items+=("$f" "[$prefix] $ts_label")
            done
        done

        if [[ ${#items[@]} -eq 0 ]]; then
            ui_msgbox "Log Viewer" "No log files found in:\n$LOG_DIR"
            return 0
        fi

        items+=("BACK" "← Return to main menu")
        local choice; choice=$(ui_menu "Log Viewer" "${items[@]}")

        case "$choice" in
            BACK|"") return 0 ;;
            *) [[ -f "$choice" ]] && ui_textbox "Log: $(basename "$choice")" "$choice" ;;
        esac
    done
}

# =============================================================================
# §14  CLEAN BUILD DIRECTORY
# =============================================================================

clean_build_ui() {
    local -a items=(
        "CLEAN"   "Clean — run make clean (preserves source)"
        "REMOVE"  "Remove — delete entire build-$BOARD directory"
        "BACK"    "← Cancel — return to main menu"
    )

    local choice; choice=$(ui_menu "Clean Build Directory" "${items[@]}")

    case "$choice" in
        CLEAN)
            if ui_yesno "Confirm Clean" "Run 'make BOARD=$CFG_BOARD clean'?"; then
                ui_wait_screen "Cleaning" "Running make clean…"
                local log="$LOG_DIR/clean_$(date '+%Y%m%d_%H%M%S').log"
                mkdir -p "$LOG_DIR"
                cd "$PORT_DIR"
                make BOARD="$CFG_BOARD" clean > "$log" 2>&1 && \
                    ui_msgbox "Clean Complete" "make clean finished.\nLog: $log" || \
                    ui_msgbox "Clean Failed" "make clean failed.\nLog: $log"
            fi
            ;;
        REMOVE)
            if ui_yesno "Confirm Delete" "DELETE build directory?\n\n$BUILD_DIR"; then
                rm -rf "$BUILD_DIR"
                ui_msgbox "Removed" "Build directory deleted."
            fi
            ;;
        BACK|"") return 0 ;;
    esac
}

# =============================================================================
# §15  FULL AUTOMATED WORKFLOW
# =============================================================================

workflow_auto() {
    git_prompt_metadata

    local ts; ts=$(date '+%Y%m%d_%H%M%S')
    local log_file="$LOG_DIR/auto_${ts}.log"
    LOG_AUTO="$log_file"
    mkdir -p "$LOG_DIR"

    {
        echo "# ZENO OS Firmware Manager — Automated Workflow Log"
        echo "# Started : $(date -u '+%Y-%m-%d %H:%M:%S UTC')"
        echo "# Board   : $CFG_BOARD"
    } > "$log_file"

    ui_msgbox "Automated Workflow" "Beginning complete workflow: Verify → Build → Release Archive → Push → Flash"

    if ! env_verify 0; then
        ui_msgbox "Workflow Aborted" "Environment verification failed."
        return 1
    fi

    if ! board_config_check; then
        board_config_repair
    fi

    if ! build_firmware 1; then
        ui_msgbox "Workflow Aborted" "Build failed."
        return 1
    fi

    if serial_detect; then
        flash_firmware 1
    fi

    ui_msgbox "Workflow Complete" "Full automated workflow finished.\nAutomation log: $log_file"
}

# =============================================================================
# §16  SETTINGS MENU
# =============================================================================

settings_ui() {
    while true; do
        local -a items=(
            "BOARD"    "Default board          [$CFG_BOARD]"
            "PORT"     "Default serial port    [${CFG_PORT:-auto}]"
            "BAUD"     "Flash baud rate        [$CFG_BAUD]"
            "GIT"      "Auto Git Push on Build [$CFG_AUTO_GIT_PUSH]"
            "MONITOR"  "Auto-open monitor      [$CFG_AUTO_MONITOR]"
            "LOGS"     "Log retention count    [$CFG_LOG_RETENTION files]"
            "IDF"      "IDF export.sh path     [${CFG_IDF_EXPORT:-auto-detect}]"
            "SAVE"     "Save settings"
            "BACK"     "← Return to main menu"
        )

        local choice; choice=$(ui_menu "Settings" "${items[@]}")

        case "$choice" in
            BOARD)
                local val; val=$(ui_inputbox "Default Board" "Enter default board name:" "$CFG_BOARD")
                [[ -n "$val" ]] && CFG_BOARD="$val"
                ;;
            PORT)
                local -a port_items=("" "Auto-detect (recommended)")
                for p in /dev/ttyACM* /dev/ttyUSB*; do
                    [[ -e "$p" ]] && port_items+=("$p" "$p")
                done
                port_items+=("CUSTOM" "Enter manually…")
                local pval; pval=$(ui_menu "Default Serial Port" "${port_items[@]}")
                if [[ "$pval" == "CUSTOM" ]]; then
                    pval=$(ui_inputbox "Serial Port" "Enter port path:" "${CFG_PORT:-/dev/ttyUSB0}")
                fi
                CFG_PORT="${pval:-}"
                ;;
            BAUD)
                local -a baud_items=(
                    "921600" "921600 — maximum speed (default)"
                    "460800" "460800 — recommended"
                    "115200" "115200 — safe fallback"
                )
                local bval; bval=$(ui_menu "Flash Baud Rate" "${baud_items[@]}")
                [[ -n "$bval" ]] && CFG_BAUD="$bval"
                ;;
            GIT)
                local gval
                gval=$(ui_menu "Auto Git Push After Build" \
                    "yes" "Yes — Stage, commit, and push automatically" \
                    "no"  "No  — Stage and commit locally only")
                [[ -n "$gval" ]] && CFG_AUTO_GIT_PUSH="$gval"
                ;;
            MONITOR)
                local mval
                mval=$(ui_menu "Auto-open Monitor After Flash" \
                    "yes" "Yes — open monitor automatically after flash" \
                    "no"  "No  — return to menu after flash")
                [[ -n "$mval" ]] && CFG_AUTO_MONITOR="$mval"
                ;;
            LOGS)
                local lval
                lval=$(ui_inputbox "Log Retention" "Number of log files to keep (1-100):" "$CFG_LOG_RETENTION")
                [[ "$lval" =~ ^[0-9]+$ ]] && CFG_LOG_RETENTION="$lval"
                ;;
            IDF)
                local ival
                ival=$(ui_inputbox "ESP-IDF Export Path" "Path to ESP-IDF export.sh:" "${CFG_IDF_EXPORT:-}")
                CFG_IDF_EXPORT="$ival"
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
# §17  FIRMWARE IDENTITY & BRANDING
# =============================================================================

_read_define() {
    local file="$1" macro="$2"
    if [[ ! -f "$file" ]]; then echo "unknown"; return; fi
    local val
    val=$(grep -m1 "#define[[:space:]]\+${macro}" "$file" 2>/dev/null \
          | sed 's/.*#define[[:space:]]\+[A-Za-z_0-9]\+[[:space:]]\+//' \
          | sed 's/"//g' | sed 's/^[[:space:]]*//' | sed 's/[[:space:]]*$//')
    echo "${val:-unknown}"
}

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

_patch_define() {
    local file="$1" macro="$2" newval="$3"
    [[ ! -f "$file" ]] && return 1
    local original_line
    original_line=$(grep -m1 "#define[[:space:]]\+${macro}" "$file" 2>/dev/null || echo "")

    if [[ "$original_line" =~ '"' ]]; then
        sed -i "s|#define[[:space:]]\+${macro}[[:space:]].*|#define ${macro} \"${newval}\"|" "$file"
    else
        sed -i "s|#define[[:space:]]\+${macro}[[:space:]].*|#define ${macro} ${newval}|" "$file"
    fi
}

branding_ui() {
    while true; do
        local board_name mcu_name
        board_name=$(_read_define "$MPCONFIGBOARD_H" "MICROPY_HW_BOARD_NAME")
        mcu_name=$(_read_define   "$MPCONFIGBOARD_H" "MICROPY_HW_MCU_NAME")

        local -a items=(
            "BOARD"     "Edit Board Name  [$board_name]"
            "MCU"       "Edit MCU Name    [$mcu_name]"
            "BACK"      "← Return to main menu"
        )

        local choice; choice=$(ui_menu "Branding & Identity" "${items[@]}")

        case "$choice" in
            BOARD)
                local newval; newval=$(ui_inputbox "Edit Board Name" "New MICROPY_HW_BOARD_NAME:" "$board_name")
                if [[ -n "$newval" && "$newval" != "$board_name" ]]; then
                    _backup_file "$MPCONFIGBOARD_H"
                    _patch_define "$MPCONFIGBOARD_H" "MICROPY_HW_BOARD_NAME" "$newval"
                    ui_msgbox "Updated" "BOARD_NAME set to: $newval"
                fi
                ;;
            MCU)
                local newval; newval=$(ui_inputbox "Edit MCU Name" "New MICROPY_HW_MCU_NAME:" "$mcu_name")
                if [[ -n "$newval" && "$newval" != "$mcu_name" ]]; then
                    _backup_file "$MPCONFIGBOARD_H"
                    _patch_define "$MPCONFIGBOARD_H" "MICROPY_HW_MCU_NAME" "$newval"
                    ui_msgbox "Updated" "MCU_NAME set to: $newval"
                fi
                ;;
            BACK|"") return 0 ;;
        esac
    done
}

# =============================================================================
# §18  PARTITION MANAGER
# =============================================================================

partition_scan_csvs() {
    find "$PORT_DIR" -maxdepth 1 -name '*.csv' -printf '%f\n' 2>/dev/null | sort
}

partition_manager_ui() {
    local -a items=(
        "4MIB"    "partitions-4MiBplus.csv"
        "8MIB"    "partitions-8MiBplus-ota.csv"
        "BACK"    "← Return to main menu"
    )
    local choice; choice=$(ui_menu "Partition Manager" "${items[@]}")
    case "$choice" in
        4MIB) ui_msgbox "Partition Table" "Selected partitions-4MiBplus.csv" ;;
        8MIB) ui_msgbox "Partition Table" "Selected partitions-8MiBplus-ota.csv" ;;
        BACK|"") return 0 ;;
    esac
}

# =============================================================================
# §19  BUILD AUTOMATION
# =============================================================================

_idf_ensure_loaded() {
    if [[ -n "${IDF_PATH:-}" && -d "$IDF_PATH" ]]; then
        return 0
    fi

    local export_sh="${CFG_IDF_EXPORT:-}"
    if [[ -z "$export_sh" ]]; then
        local -a search_paths=(
            "$HOME/esp/esp-idf/export.sh"
            "$HOME/esp-idf/export.sh"
            "/opt/esp/idf/export.sh"
            "/opt/esp-idf/export.sh"
        )
        for p in "${search_paths[@]}"; do
            [[ -f "$p" ]] && export_sh="$p" && break
        done
    fi

    if [[ -z "$export_sh" || ! -f "$export_sh" ]]; then
        ui_msgbox "ESP-IDF Error" "ESP-IDF export.sh not found. Set path in Settings."
        return 1
    fi

    # shellcheck disable=SC1090
    source "$export_sh" >/dev/null 2>&1
    return 0
}

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
        err+=("") err+=("  ✘  $label FAILED (exit $rc)") err+=("")
        err+=("  Duration  : ${duration}s")
        err+=("  Log       : $log_file") err+=("")
        ui_status_screen "$label — Failed" "${err[@]}"
    fi
}

build_clean() {
    _idf_ensure_loaded || return 1
    git_prompt_metadata

    if ! ui_yesno "Clean Build" "Perform a CLEAN build for $CFG_BOARD?"; then
        return 0
    fi

    local ts; ts=$(date '+%Y%m%d_%H%M%S')
    local log_file="$LOG_DIR/build_${ts}.log"
    LOG_BUILD="$log_file"
    mkdir -p "$LOG_DIR"

    rm -rf "$BUILD_DIR"
    cd "$PORT_DIR"

    make BOARD="$CFG_BOARD" submodules >> "$log_file" 2>&1

    local build_start; build_start=$(date +%s)
    local rc=0
    make BOARD="$CFG_BOARD" >> "$log_file" 2>&1 &
    local make_pid=$!

    (
        local elapsed=0
        while kill -0 "$make_pid" 2>/dev/null; do
            (( elapsed++ )) || true
            local pct
            pct=$(echo "scale=0; 95 - (95 / (1 + $elapsed / 45))" | bc 2>/dev/null || echo 50)
            echo "$pct"
            sleep 1
        done
        echo 100
    ) | ui_gauge "Clean Build in Progress" "Building for $CFG_BOARD"

    wait "$make_pid" && rc=0 || rc=$?
    local duration=$(( $(date +%s) - build_start ))

    _build_show_result "$rc" "$duration" "$log_file" "Clean Build"
    if [[ $rc -eq 0 ]]; then
        git_release_workflow
    fi
}

build_incremental() {
    _idf_ensure_loaded || return 1
    git_prompt_metadata

    if ! ui_yesno "Incremental Build" "Perform an INCREMENTAL build?"; then
        return 0
    fi

    local ts; ts=$(date '+%Y%m%d_%H%M%S')
    local log_file="$LOG_DIR/build_${ts}.log"
    LOG_BUILD="$log_file"
    mkdir -p "$LOG_DIR"

    cd "$PORT_DIR"
    local build_start; build_start=$(date +%s)
    local rc=0
    make BOARD="$CFG_BOARD" >> "$log_file" 2>&1 &
    local make_pid=$!

    (
        local elapsed=0
        while kill -0 "$make_pid" 2>/dev/null; do
            (( elapsed++ )) || true
            pct=$(echo "scale=0; 95 - (95 / (1 + $elapsed / 20))" | bc 2>/dev/null || echo 50)
            echo "$pct"
            sleep 1
        done
        echo 100
    ) | ui_gauge "Building (Incremental)" "make BOARD=$CFG_BOARD"

    wait "$make_pid" && rc=0 || rc=$?
    local duration=$(( $(date +%s) - build_start ))

    _build_show_result "$rc" "$duration" "$log_file" "Incremental Build"
    if [[ $rc -eq 0 ]]; then
        git_release_workflow
    fi
}

build_automation_ui() {
    while true; do
        local -a items=(
            "CLEAN"       "Clean Build (rm build + make + git release)"
            "INCREMENTAL" "Incremental Build (make + git release)"
            "FLASH"       "Flash Firmware"
            "MONITOR"     "Serial Monitor"
            "BACK"        "← Return to main menu"
        )
        local choice; choice=$(ui_menu "Build Automation" "${items[@]}")

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
            BACK|"") return 0 ;;
        esac
    done
}

# =============================================================================
# §20  WELCOME SCREEN + MAIN MENU + ENTRY POINT
# =============================================================================

show_welcome() {
    local branch="unknown"
    command -v git >/dev/null 2>&1 && [[ -d "$REPO/.git" ]] && \
        branch=$(git -C "$REPO" branch --show-current 2>/dev/null || echo "unknown")

    local msg="Welcome to the ZENO OS Firmware Manager\nVersion $SCRIPT_VERSION\n\n"
    msg+="  Repo       : $REPO\n"
    msg+="  Branch     : $branch\n"
    msg+="  Board      : $CFG_BOARD\n"
    msg+="  Releases   : firmware releases/\n"
    ui_msgbox "ZENO OS Firmware Manager v$SCRIPT_VERSION" "$msg"
}

main_menu() {
    while true; do
        local -a items=(
            "1"  "Build Automation (Clean / Incremental / Git Release / Flash)"
            "2"  "Branding & Identity (Board Name / MCU / Version)"
            "3"  "Partition Manager"
            "4"  "Flash Firmware"
            "5"  "Serial Monitor"
            "6"  "Verify Environment"
            "7"  "Repair Board Configuration (PSRAM)"
            "8"  "View Build Information"
            "9"  "View Logs"
            "10" "Clean Build Directory"
            "11" "Full Automated Workflow"
            "12" "Settings"
            "13" "Exit"
        )

        local choice; choice=$(ui_menu "ZENO OS Firmware Manager v${SCRIPT_VERSION} — Main Menu" "${items[@]}")

        case "$choice" in
            1)  build_automation_ui ;;
            2)  branding_ui ;;
            3)  partition_manager_ui ;;
            4)  serial_detect && flash_firmware ;;
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

main() {
    require_repo
    ui_init
    cfg_load
    init_log_session
    show_welcome
    main_menu
}

main "$@"
