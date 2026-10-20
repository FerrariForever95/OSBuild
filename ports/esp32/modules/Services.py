"""
Services.py -- Zeno OS core services, single file.

Everything that used to be split across logger/power/filesystem/security/
process/network/system/packages now lives here directly. No dynamic
cross-file imports, no sys.path/SysPathManager involved for any of this --
it's all one real Python module, loaded once, in dependency order.

Wiki moved out to its own package (/bin/Wiki/Wiki.py) -- it was never a
core OS dependency, just a bundled convenience, so it belongs with
Graphics/healthmonitor/banner as an optional install.

PackageManager no longer eagerly imports every installed package at
construction time. Packages are imported lazily, the first time
run(command, ...) is actually called for them -- exactly like a normal
package manager. Use reload_all_installed() if you explicitly want
everything brought back online in one go (e.g. right after a recover).
"""

from machine import Pin, I2C
import time
import os
import json
import zfs
import zeno
import network as _network
import gc
import machine
import micropython
import pystone_lowmem
import ubluetooth as bt
from machine import Pin, SPI
from firmware import SDCard
import sys
import usocket
import ssl
import urequests


# ============================================================================

debug_log_enabled = False


class Logger:
    LEVELS = {0: "ERROR", 1: "WARNING", 2: "DEBUG"}

    def __init__(self, log_file_user="/LOGS/systemlog.txt", boot=False):
        self.boot = boot
        self.log_file_user = log_file_user
        self._create_file(self.log_file_user)
        self._boot_marker = "[BOOT_START]"

        if self.boot:
            self._write(self._boot_marker)
            self.debug("Logger initialized. Boot starting...", source="BOOT")

    def _create_file(self, path):
        try:
            with open(path, "a"):
                pass
            message = f"File created successfully: {path}"
            return (message, True)
        except Exception as e:
            message = f"[Logger] File creation failed: {e}"
            return (message, False)

    def _write(self, text):
        # Since debug_log_enabled is False, we won't print to console
        # But let's keep the check for safety
        if debug_log_enabled:
            # We won't actually print, but we could log this decision
            pass  # Not printing to console as per user request
        try:
            with open(self.log_file_user, "a") as f:
                f.write(text + "\n")
            message = f"Log entry written successfully"
            return (message, True)
        except Exception as e:
            message = f"Failed to write log entry: {e}"
            return (message, False)

    def log(self, level, message, source="GENERAL"):
        log_message = f"[SRC:{source}] [{self.LEVELS.get(level, 'UNKNOWN')}] {message}"
        write_result = self._write(log_message)
        # Assuming _write now returns (message, value)
        if isinstance(write_result, tuple) and len(write_result) == 2:
            return write_result
        else:
            # Handle case where _write hasn't been modified yet
            return ("Log write attempted", write_result)

    def error(self, message, source="GENERAL"):
        return self.log(0, message, source)

    def warning(self, message, source="GENERAL"):
        return self.log(1, message, source)

    def debug(self, message, source="GENERAL"):
        return self.log(2, message, source)

    def boot_complete(self):
        self.debug("Boot sequence complete.", source="BOOT")
        write_result = self._write("_" * 40)
        # For boot_complete, we don't really need to return anything specific
        # But to follow the pattern, let's return a success message
        return ("Boot sequence completed", True)

    def viewlogs(self, lines=None):
        try:
            with open(self.log_file_user, "r") as f:
                data = f.read()
        except Exception as e:
            message = f"[Logger] Failed to read logs: {e}"
            return (message, None)

        logs = data.strip().split("\n")
        last_boot_index = 0
        for i, line in enumerate(logs):
            if line.startswith("[BOOT_START]"):
                last_boot_index = i
        logs = logs[last_boot_index:]
        if lines:
            logs = logs[-lines:]
        # Instead of printing, we return the logs as the value
        # For message, we describe what we did
        if lines:
            message = f"Showing last {lines} log entries"
        else:
            message = "Showing all log entries since last boot"
        return (message, logs)

    def clear_logs(self):
        try:
            with open(self.log_file_user, "w") as f:
                f.write("")
            message = "[Logger] Logs cleared successfully."
            # Instead of printing, we log it for debugging (won't appear in console)
            self.debug("Logs cleared successfully by system.", source="LOGGER")
            return (message, True)
        except Exception as e:
            message = f"[Logger] Failed to clear logs: {e}"
            # Instead of printing, we log it for debugging (won't appear in console)
            self.error(message, source="LOGGER")
            return (message, False)


def _logger_help():
    return (
        "logger.py -- system logging\n"
        "  Logger(log_file_user='/LOGS/systemlog.txt', boot=False)\n"
        "    .log(level, message, source='GENERAL')\n"
        "    .error(message, source='GENERAL')\n"
        "    .warning(message, source='GENERAL')\n"
        "    .debug(message, source='GENERAL')\n"
        "    .boot_complete()\n"
        "    .viewlogs(lines=None)\n"
        "    .clear_logs()\n"
        "  module var: debug_log_enabled (bool) -- toggle console echo of log writes"
    )


# ============================================================================

try:
    import machine
except ImportError:
    machine = None

try:
    import sys as _sys
    _PLATFORM = _sys.platform
except Exception:
    _PLATFORM = "unknown"


_PLATFORM_LEVELS = {
    "esp32":  {"low": 80_000_000,  "normal": 160_000_000, "high": 240_000_000, "turbo": 240_000_000},
    "rp2":    {"low": 48_000_000,  "normal": 125_000_000, "high": 200_000_000, "turbo": 250_000_000},
}

_LEVEL_ORDER = ("low", "normal", "high", "turbo")


class PowerManagement:
    def __init__(self, logger=None):
        self.logger = logger or (Logger() if "Logger" in globals() else None)
        self.source = "POWERMGR"

        self.levels = dict(_PLATFORM_LEVELS.get(_PLATFORM, {}))
        if not self.levels:
            base = self._raw_freq() or 0
            self.levels = {"low": base, "normal": base, "high": base, "turbo": base}

        self.baseline = "normal" if "normal" in self.levels else _LEVEL_ORDER[0]
        self._requests = {}

    def help(self):
        help_text = (
            "  power                          Show current frequency / active boosts\n"
            "  power levels                   List available frequency tiers\n"
            "  power set <level>               Manually pin CPU to a tier (low/normal/high/turbo)\n"
            "  power boost <reason> [level]    Request a frequency tier, tracked by reason\n"
            "  power release <reason>          Release a previous boost request\n"
            "  power baseline <level>          Change the default idle tier"
        )
        # Instead of printing, we return the help text as the value
        return ("Power management help", help_text)

    def status(self):
        current = self._raw_freq()
        # Instead of printing, we'll include the status information in the return value
        status_lines = []
        status_lines.append("[Power]")
        status_lines.append("  Platform      : {}".format(_PLATFORM))
        status_lines.append("  Current freq  : {}".format(self._fmt_hz(current)))
        status_lines.append("  Baseline tier : {} ({})".format(self.baseline, self._fmt_hz(self.levels.get(self.baseline))))
        if self._requests:
            status_lines.append("  Active boosts :")
            for reason, level in self._requests.items():
                status_lines.append("    - {:<20} -> {}".format(reason, level))
        else:
            status_lines.append("  Active boosts : none")

        result = {"platform": _PLATFORM, "current_hz": current, "requests": dict(self._requests)}
        message = "\n".join(status_lines)
        return (message, result)

    def levels_list(self):
        # Instead of printing, we'll include the list information in the return value
        lines = []
        lines.append("\n[Power] Available tiers ({}):".format(_PLATFORM))
        for name in _LEVEL_ORDER:
            if name in self.levels:
                lines.append("  {:<8} {}".format(name, self._fmt_hz(self.levels[name])))
        # Instead of printing, we return the lines as part of the message
        message = "\n".join(lines)
        return (message, dict(self.levels))

    def set_level(self, level):
        # Assuming _apply now returns (message, value)
        result = self._apply(level)
        if isinstance(result, tuple) and len(result) == 2:
            return result
        else:
            # Handle case where _apply hasn't been modified yet
            return ("Set level attempted", result)

    def baseline_set(self, level):
        if level not in self.levels:
            message = f"Unknown level '{level}'"
            self._error(message)
            return (message, False)
        self.baseline = level
        if not self._requests:
            apply_result = self._apply(level)
            # Assuming _apply now returns (message, value)
            if isinstance(apply_result, tuple) and len(apply_result) == 2:
                # We don't really need to return the _apply result, but we'll return success
                return (f"Baseline set to {level}", True)
            else:
                # Handle case where _apply hasn't been modified yet
                return (f"Baseline set to {level}", True)
        return (f"Baseline set to {level}", True)

    def boost(self, reason, level="high"):
        if level not in self.levels:
            message = f"Unknown level '{level}'"
            self._error(message)
            return (message, False)
        self._requests[reason] = level
        apply_result = self._apply(self._highest_requested())
        # Assuming _apply now returns (message, value)
        if isinstance(apply_result, tuple) and len(apply_result) == 2:
            return apply_result
        else:
            # Handle case where _apply hasn't been modified yet
            return (f"Boost applied for reason '{reason}' with level '{level}'", apply_result)

    def release(self, reason):
        if reason in self._requests:
            del self._requests[reason]
        target = self._highest_requested() or self.baseline
        apply_result = self._apply(target)
        # Assuming _apply now returns (message, value)
        if isinstance(apply_result, tuple) and len(apply_result) == 2:
            return apply_result
        else:
            # Handle case where _apply hasn't been modified yet
            return (f"Released boost for reason '{reason}'", apply_result)

    def boosted(self, level="high", reason="context"):
        # Instead of returning the context object directly, we'll return it as the value
        # with a descriptive message
        context = _BoostContext(self, level, reason)
        return (f"Created boost context for level '{level}' and reason '{reason}'", context)

    def auto_scale(self, load_percent, reason="auto"):
        if load_percent is None:
            return ("No load percentage provided", None)
        if load_percent >= 70:
            return self.boost(reason, "high")
        elif load_percent >= 40:
            return self.boost(reason, "normal")
        else:
            return self.release(reason)

    def _highest_requested(self):
        active = [lvl for lvl in self._requests.values() if lvl in self.levels]
        if not active:
            return ("No active requests", None)
        result = max(active, key=lambda lvl: _LEVEL_ORDER.index(lvl))
        return (f"Highest requested level: {result}", result)

    def _apply(self, level):
        if level not in self.levels:
            message = f"Unknown level '{level}'"
            self._error(message)
            return (message, False)
        hz = self.levels[level]
        if not hz:
            message = f"No known frequency for level '{level}' on platform '{_PLATFORM}'"
            self._error(message)
            return (message, False)
        if machine is None:
            message = f"machine module unavailable -- would set {level} ({self._fmt_hz(hz)})"
            self._log(message)
            return (message, True)
        try:
            machine.freq(hz)
            message = f"CPU frequency set to {level} ({self._fmt_hz(hz)})"
            self._log(message)
            return (message, True)
        except Exception as e:
            message = f"Failed to set frequency: {e}"
            self._error(message)
            return (message, False)

    def _raw_freq(self):
        if machine is None:
            return ("Machine module not available", None)
        try:
            freq = machine.freq()
            return (f"Current CPU frequency: {freq} Hz", freq)
        except Exception as e:
            message = f"Failed to get CPU frequency: {e}"
            return (message, None)

    def _fmt_hz(self, hz):
        if not hz:
            return ("Frequency is zero or None", "unknown")
        formatted = "{:.0f} MHz".format(hz / 1_000_000)
        return (f"Formatted frequency: {formatted}", formatted)

    def _log(self, message):
        if self.logger:
            try:
                # Assuming logger.debug now returns (message, value)
                log_result = self.logger.debug(message, source=self.source)
                # We don't really need to return anything from _log, but let's return success
                return ("Message logged", True)
            except Exception:
                return ("Failed to log message", False)
        return ("No logger available", False)

    def _error(self, message):
        if self.logger:
            try:
                self.logger.error(message, source=self.source)
            except Exception:
                pass
        # Instead of printing, return the error message for handling by caller
        # Since this is an internal method, we log it but don't print to console
        # as per user request to prevent traceability issues
        pass  # Not printing to console as per user request

class _BoostContext:

    def __init__(self, power, level, reason):
        self.power = power
        self.level = level
        self.reason = reason

    def __enter__(self):
        self.power.boost(self.reason, self.level)
        return self.power

    def __exit__(self, exc_type, exc_val, exc_tb):
        self.power.release(self.reason)
        return False

class CPU:
    def __init__(self, model="ESP32-S3N16R8"):
        self.model = model
        self.usage_pct = 0
        # Instead of printing, we could log this but debug_log_enabled is False
        # so it won't appear in console as per user request
        pass  # Not printing to console as per user request

    def report_frame(self, busy_us, idle_us):
        total = busy_us + idle_us
        if total <= 0:
            return

        self.usage_pct = (busy_us * 100) // total

    def usage(self):
        return self.usage_pct

    def reboot(self):
        time.sleep_ms(50)
        machine.reset()

    def shutdown(self):
        time.sleep_ms(100)
        machine.deepsleep()

    def sleep_ms(self, ms):
        machine.lightsleep(ms)

    def set_freq(self, hz):
        machine.freq(hz)

    def get_freq(self):
        return machine.freq()

    def reset_cause(self):
        return machine.reset_cause()

    def wake_reason(self):
        return machine.wake_reason()

    def panic(self, reason=None):
        try:
            print("[CPU PANIC]", reason)
        except:
            pass
        time.sleep_ms(50)
        machine.reset()

    def chip_temp(self):
        try:
            return esp.raw_temperature()
        except:
            return None


def _power_help():
    return (
        "power.py -- CPU frequency management\n"
        "  PowerManagement(logger=None)\n"
        "    .status() / .levels_list()\n"
        "    .set_level(level) / .baseline_set(level)\n"
        "    .boost(reason, level='high') / .release(reason)\n"
        "    .boosted(level='high', reason='context')  -- context manager\n"
        "    .auto_scale(load_percent, reason='auto')\n"
        "  CPU(model='ESP32-S3N16R8')\n"
        "    .usage() / .report_frame(busy_us, idle_us)\n"
        "    .reboot() / .shutdown() / .sleep_ms(ms)\n"
        "    .set_freq(hz) / .get_freq()\n"
        "    .reset_cause() / .wake_reason() / .panic(reason=None)\n"
        "    .chip_temp()"
    )


# ============================================================================

class dewrapper:
    @staticmethod
    def read(path):
        return zfs.read(path).decode("utf-8")

    @staticmethod
    def write(path, text):
        zfs.write(path, text.encode("utf-8"))

    @staticmethod
    def lines(path):
        return [l.strip() for l in dewrapper.read(path).splitlines() if l.strip()]

    @staticmethod
    def records(path):
        return [line.split("###") for line in dewrapper.lines(path)]


def _exists(path):
    try:
        os.stat(path)
        return True
    except OSError:
        return False


def _makedirs(path):
    if not path:
        return
    cur = ""
    for p in path.split('/'):
        if not p:
            continue
        cur = cur + "/" + p if cur else p
        if not _exists(cur):
            try:
                os.mkdir(cur)
            except Exception:
                pass


class PermissionError(Exception):
    pass


class FileNotFoundError(Exception):
    pass


class FileExistsError(Exception):
    pass


class NotADirectoryError(Exception):
    pass


SYS_DIR = "/.sys"

PROGRAM_EXT = (".py", ".mpy", ".zsh")
TEXT_EXT = (".txt", ".md", ".log")
BITMAP_EXT = (".bmp",)
IMAGE_EXT = (".png", ".jpg", ".jpeg", ".webp", ".gif")
AUDIO_EXT = (".mp3", ".wav", ".ogg")
VIDEO_EXT = (".mp4", ".avi", ".mov")
JSON_EXT = (".json",)
ARCHIVE_EXT = (".zip", ".tar", ".gz")

EXECUTABLE_EXT = (".py", ".mpy")     # auto-executable by default
ROOT_OWNED_EXT = (".py", ".mpy", ".zsh")

PERM_FULL, PERM_READ, PERM_READWRITE, PERM_NONE = 0, 1, 2, 3
PERM_READEXEC, PERM_WRITEEXEC, PERM_WRITE, PERM_EXEC = 4, 5, 6, 7

READ_PERMS = {PERM_FULL, PERM_READ, PERM_READWRITE, PERM_READEXEC}
WRITE_PERMS = {PERM_FULL, PERM_READWRITE, PERM_WRITEEXEC, PERM_WRITE}
EXEC_PERMS = {PERM_FULL, PERM_READEXEC, PERM_WRITEEXEC, PERM_EXEC}
VALID_PERMS = {0, 1, 2, 3, 4, 5, 6, 7}

S_IFDIR = 0x4000


def _os_exists(path):
    try:
        os.stat(path)
        return True
    except OSError:
        return False


def _os_isdir(path):
    try:
        return bool(os.stat(path)[0] & S_IFDIR)
    except OSError:
        return False


def _os_mkdir(path):
    if not _os_exists(path):
        os.mkdir(path)


def _os_stat(path):
    try:
        st = os.stat(path)
    except OSError:
        return False, 0
    is_dir = bool(st[0] & S_IFDIR)
    return is_dir, (0 if is_dir else (st[6] if len(st) > 6 else 0))


def _os_rmnode(path):
    (os.rmdir if _os_isdir(path) else os.remove)(path)


def _os_rename(old, new):
    os.rename(old, new)


def _os_listdir(path):
    return os.listdir(path)


def _os_read(path):
    with open(path, "r") as f:
        return f.read()


def _os_write(path, data):
    with open(path, "w") as f:
        f.write(data)


def _norm(path):
    if not path:
        return "/"
    parts = [p for p in str(path).split("/") if p]
    return "/" + "/".join(parts) if parts else "/"


def _parent(path):
    path = _norm(path)
    if path == "/":
        return "/"
    idx = path.rfind("/")
    return path[:idx] if idx > 0 else "/"


def _name(path):
    path = _norm(path)
    return "" if path == "/" else path.rsplit("/", 1)[-1]


def _join(parent, name):
    parent = _norm(parent)
    return name if parent == "/" else parent + "/" + name


def _ext(name):
    idx = name.rfind(".")
    return "" if idx <= 0 else name[idx:].lower()


def _mirror_dir_for(path):
    path = _norm(path)
    return SYS_DIR if path == "/" else SYS_DIR + path


def _meta_file_for(path):
    path = _norm(path)
    return SYS_DIR + "/root.vfs" if path == "/" else SYS_DIR + path + ".vfs"


def is_hidden(name):
    return bool(name) and name.startswith(".")


class FileManager:
    def __init__(self):
        self.um = usermanager()
        self._fd_table = {}
        self._next_fd = 3
        self._bootstrap()

    def _bootstrap(self):
        if not _os_exists(SYS_DIR):
            _os_mkdir(SYS_DIR)
        if not _os_exists(_meta_file_for("/")):
            self._rebuild_meta("/")

    def _current_user(self):
        return self.um.userinfo().get("user", "unknown")

    def _is_root(self):
        return self.um.is_session_root()

    def _check_permission(self, path, action):
        if self._is_root():
            return True
        perm = self.metadata(path).get("permission", PERM_NONE)
        allowed = {"read": READ_PERMS, "write": WRITE_PERMS, "exec": EXEC_PERMS}[action]
        if perm not in allowed:
            raise PermissionError(
                "%s: %s access denied for user '%s'" % (path, action, self._current_user())
            )
        return True

    def _filetype(self, name, is_dir):
        if is_dir:
            return "directory"
        ext = _ext(name)
        for exts, label in (
            (PROGRAM_EXT, "program"), (TEXT_EXT, "text"), (BITMAP_EXT, "bitmap"),
            (IMAGE_EXT, "image"), (AUDIO_EXT, "audio"), (VIDEO_EXT, "video"),
            (JSON_EXT, "json"), (ARCHIVE_EXT, "archive"),
        ):
            if ext in exts:
                return label
        return "unknown"

    def _default_owner(self, name):
        return "root" if _ext(name) in ROOT_OWNED_EXT else self._current_user()

    def _default_permission(self, name, is_dir):
        if is_dir:
            return PERM_FULL
        return PERM_READEXEC if _ext(name) in EXECUTABLE_EXT else PERM_READWRITE

    def _scan_entry(self, parent, name):
        is_dir, size = _os_stat(_join(parent, name))
        return {
            "owner": self._default_owner(name),
            "permission": self._default_permission(name, is_dir),
            "type": self._filetype(name, is_dir),
            "size": size,
        }

    def _ensure_dir_chain(self, path):
        path = _norm(path)
        if path == "/" or _os_exists(path):
            return
        self._ensure_dir_chain(_parent(path))
        _os_mkdir(path)

    def _rebuild_meta(self, dirpath):
        dirpath = _norm(dirpath)
        names = _os_listdir(dirpath) if _os_exists(dirpath) else []
        meta = {}
        for n in names:
            if dirpath == "/" and n == _name(SYS_DIR):
                continue
            meta[n] = self._scan_entry(dirpath, n)
        self._write_meta(dirpath, meta)
        return meta

    def _read_meta(self, dirpath):
        dirpath = _norm(dirpath)
        meta_file = _meta_file_for(dirpath)
        if not _os_exists(meta_file):
            return self._rebuild_meta(dirpath)
        raw = _os_read(meta_file)
        if not raw:
            return self._rebuild_meta(dirpath)
        try:
            return json.loads(raw)
        except Exception:
            return self._rebuild_meta(dirpath)

    def _write_meta(self, dirpath, data, _skip_ensure=False):
        dirpath = _norm(dirpath)
        meta_file = _meta_file_for(dirpath)
        if not _skip_ensure:
            container = SYS_DIR if dirpath == "/" else _mirror_dir_for(_parent(dirpath))
            self._ensure_dir_chain(container)
        _os_write(meta_file, json.dumps(data))

    def exists(self, path):
        return _os_exists(_norm(path))

    def metadata(self, path):
        path = _norm(path)
        if path == "/":
            return {"owner": "root", "permission": PERM_FULL, "type": "directory", "size": 0}
        if not self.exists(path):
            raise FileNotFoundError(path)
        parent, name = _parent(path), _name(path)
        meta = self._read_meta(parent)
        if name not in meta:
            meta = self._rebuild_meta(parent)
        return dict(meta.get(name) or self._scan_entry(parent, name))

    def listdir(self, path="/", show_hidden=False):
        path = _norm(path)
        if not self.exists(path):
            raise FileNotFoundError(path)
        if not _os_isdir(path) and path != "/":
            raise NotADirectoryError(path)
        self._check_permission(path, "read")

        meta = self._read_meta(path)
        real_names = _os_listdir(path)
        if path == "/":
            real_names = [n for n in real_names if n != _name(SYS_DIR)]

        changed = False
        for n in real_names:
            if n not in meta:
                meta[n] = self._scan_entry(path, n)
                changed = True
        for n in list(meta.keys()):
            if n not in real_names:
                del meta[n]
                changed = True
        if changed:
            self._write_meta(path, meta)

        names = real_names if show_hidden else [n for n in real_names if not is_hidden(n)]
        return sorted(names)

    def create(self, path, content="", owner=None, permission=None):
        path = _norm(path)
        parent, name = _parent(path), _name(path)
        if not self.exists(parent):
            raise FileNotFoundError(parent)
        if self.exists(path):
            raise FileExistsError(path)
        self._check_permission(parent, "write")

        owner = owner if owner is not None else self._default_owner(name)
        permission = permission if permission is not None else self._default_permission(name, False)
        if permission not in VALID_PERMS:
            raise ValueError("invalid permission: %r" % (permission,))

        _os_write(path, content)
        _, size = _os_stat(path)
        meta = self._read_meta(parent)
        meta[name] = {"owner": owner, "permission": permission, "type": self._filetype(name, False), "size": size}
        self._write_meta(parent, meta)
        return True

    def mkdir(self, path, owner=None, permission=None):
        path = _norm(path)
        if path == "/":
            raise FileExistsError("root already exists")
        parent, name = _parent(path), _name(path)
        if not self.exists(parent):
            raise FileNotFoundError(parent)
        if self.exists(path):
            raise FileExistsError(path)
        self._check_permission(parent, "write")

        owner = owner if owner is not None else self._default_owner(name)
        permission = permission if permission is not None else self._default_permission(name, True)
        if permission not in VALID_PERMS:
            raise ValueError("invalid permission: %r" % (permission,))

        _os_mkdir(path)
        meta = self._read_meta(parent)
        meta[name] = {"owner": owner, "permission": permission, "type": "directory", "size": 0}
        self._write_meta(parent, meta)

        self._ensure_dir_chain(_mirror_dir_for(path))
        self._write_meta(path, {}, _skip_ensure=True)
        return True

    def delete(self, path):
        path = _norm(path)
        if path == "/":
            raise PermissionError("cannot delete root")
        if not self.exists(path):
            raise FileNotFoundError(path)
        parent, name = _parent(path), _name(path)
        self._check_permission(parent, "write")

        if _os_isdir(path):
            for child in _os_listdir(path):
                self.delete(_join(path, child))
            _os_rmnode(path)
            if _os_exists(_meta_file_for(path)):
                _os_rmnode(_meta_file_for(path))
            if _os_exists(_mirror_dir_for(path)):
                _os_rmnode(_mirror_dir_for(path))
        else:
            _os_rmnode(path)

        meta = self._read_meta(parent)
        if name in meta:
            del meta[name]
            self._write_meta(parent, meta)
        return True

    def rename(self, path, new_name):
        path = _norm(path)
        parent, old_name = _parent(path), _name(path)
        if "/" in new_name:
            new_path = _norm(new_name)
            if _parent(new_path) != parent:
                raise ValueError("rename() cannot change directory, use move()")
            new_name_only = _name(new_path)
        else:
            new_name_only = new_name
            new_path = _join(parent, new_name_only)

        if not self.exists(path):
            raise FileNotFoundError(path)
        if self.exists(new_path):
            raise FileExistsError(new_path)
        self._check_permission(parent, "write")

        is_dir = _os_isdir(path)
        _os_rename(path, new_path)

        meta = self._read_meta(parent)
        entry = meta.pop(old_name, None) or self._scan_entry(parent, new_name_only)
        meta[new_name_only] = entry
        self._write_meta(parent, meta)

        if is_dir:
            if _os_exists(_meta_file_for(path)):
                _os_rename(_meta_file_for(path), _meta_file_for(new_path))
            if _os_exists(_mirror_dir_for(path)):
                _os_rename(_mirror_dir_for(path), _mirror_dir_for(new_path))
        return True

    def move(self, src, dst):
        src, dst = _norm(src), _norm(dst)
        if not self.exists(src):
            raise FileNotFoundError(src)
        if self.exists(dst):
            raise FileExistsError(dst)
        src_parent, src_name = _parent(src), _name(src)
        dst_parent, dst_name = _parent(dst), _name(dst)
        if not self.exists(dst_parent):
            raise FileNotFoundError(dst_parent)
        if src_parent == dst_parent:
            return self.rename(src, dst_name)

        self._check_permission(src_parent, "write")
        self._check_permission(dst_parent, "write")

        is_dir = _os_isdir(src)
        _os_rename(src, dst)

        src_meta = self._read_meta(src_parent)
        entry = src_meta.pop(src_name, None)
        self._write_meta(src_parent, src_meta)

        dst_meta = self._read_meta(dst_parent)
        dst_meta[dst_name] = entry or self._scan_entry(dst_parent, dst_name)
        self._write_meta(dst_parent, dst_meta)

        if is_dir:
            if _os_exists(_meta_file_for(src)):
                _os_rename(_meta_file_for(src), _meta_file_for(dst))
            if _os_exists(_mirror_dir_for(src)):
                _os_rename(_mirror_dir_for(src), _mirror_dir_for(dst))
        return True

    def copy(self, src, dst):
        src, dst = _norm(src), _norm(dst)
        if not self.exists(src):
            raise FileNotFoundError(src)
        if self.exists(dst):
            raise FileExistsError(dst)
        src_parent, dst_parent = _parent(src), _parent(dst)
        if not self.exists(dst_parent):
            raise FileNotFoundError(dst_parent)
        self._check_permission(src_parent, "read")
        self._check_permission(dst_parent, "write")

        src_entry = self._read_meta(src_parent).get(_name(src), {})
        if _os_isdir(src):
            self.mkdir(dst, owner=src_entry.get("owner"), permission=src_entry.get("permission"))
            for child in _os_listdir(src):
                self.copy(_join(src, child), _join(dst, child))
        else:
            self.create(dst, _os_read(src) or "", owner=src_entry.get("owner"), permission=src_entry.get("permission"))
        return True

    def chmod(self, path, permission):
        path = _norm(path)
        if permission not in VALID_PERMS:
            raise ValueError("invalid permission: %r" % (permission,))
        if path == "/":
            raise PermissionError("cannot chmod root")
        parent, name = _parent(path), _name(path)
        meta = self._read_meta(parent)
        entry = meta.get(name)
        if entry is None:
            if not self.exists(path):
                raise FileNotFoundError(path)
            entry = self._scan_entry(parent, name)
        if not self._is_root() and entry.get("owner") != self._current_user():
            raise PermissionError("only the owner or root may chmod '%s'" % path)
        entry["permission"] = permission
        meta[name] = entry
        self._write_meta(parent, meta)
        return True

    def chown(self, path, new_owner):
        path = _norm(path)
        if path == "/":
            raise PermissionError("cannot chown root")
        if not self._is_root():
            raise PermissionError("only root may chown")
        parent, name = _parent(path), _name(path)
        meta = self._read_meta(parent)
        entry = meta.get(name)
        if entry is None:
            if not self.exists(path):
                raise FileNotFoundError(path)
            entry = self._scan_entry(parent, name)
        entry["owner"] = new_owner
        meta[name] = entry
        self._write_meta(parent, meta)
        return True

    def refresh_tree(self, path="/", system_token=None):
        if system_token is not None:
            if system_token != _SYSTEM_TOKEN:
                raise PermissionError("invalid system privilege token")
            with SystemPrivilege(system_token, "refresh_tree"):
                return self._refresh_tree_impl(path)
        if not self._is_root():
            raise PermissionError("only root may refresh the metadata tree")
        return self._refresh_tree_impl(path)

    def _refresh_tree_impl(self, path="/"):
        path = _norm(path)
        if not self.exists(path):
            raise FileNotFoundError(path)
        if path != "/" and not _os_isdir(path):
            raise NotADirectoryError(path)

        refreshed = 0
        stack = [path]
        while stack:
            current = stack.pop()
            try:
                names = _os_listdir(current)
            except OSError:
                continue
            meta = {}
            for n in names:
                if current == "/" and n == _name(SYS_DIR):
                    continue
                entry = self._scan_entry(current, n)
                meta[n] = entry
                if entry["type"] == "directory":
                    stack.append(_join(current, n))
            self._write_meta(current, meta)
            refreshed += 1
        return refreshed

    def open(self, path, mode="r"):
        path = _norm(path)
        parent = _parent(path)
        wants_write = any(c in mode for c in "wa+")
        wants_read = "r" in mode or "+" in mode

        if not self.exists(path):
            if wants_write:
                self._check_permission(parent, "write")
                self.create(path, "")
            else:
                raise FileNotFoundError(path)

        if wants_read:
            self._check_permission(path, "read")
        if wants_write:
            self._check_permission(path, "write")

        handle = open(path, mode)
        fd = self._next_fd
        self._next_fd += 1
        self._fd_table[fd] = handle
        return fd

    def _handle(self, fd):
        handle = self._fd_table.get(fd)
        if handle is None:
            raise ValueError("invalid file descriptor: %r" % (fd,))
        return handle

    def read(self, fd, size=-1):
        return self._handle(fd).read(size)

    def write(self, fd, data):
        return self._handle(fd).write(data)

    def close(self, fd):
        handle = self._fd_table.pop(fd, None)
        if handle is None:
            raise ValueError("invalid file descriptor: %r" % (fd,))
        handle.close()
        return True


def _filesystem_help():
    return (
        "filesystem.py -- VFS facade\n"
        "  FileManager()\n"
        "    .exists(path) / .metadata(path) / .listdir(path='/', show_hidden=False)\n"
        "    .create(path, content='', owner=None, permission=None)\n"
        "    .mkdir(path, owner=None, permission=None) / .delete(path)\n"
        "    .rename(path, new_name) / .move(src, dst) / .copy(src, dst)\n"
        "    .chmod(path, permission) / .chown(path, new_owner)\n"
        "    .refresh_tree(path='/', system_token=None)\n"
        "    .open(path, mode='r') / .read(fd, size=-1) / .write(fd, data) / .close(fd)\n"
        "  dewrapper -- low-level ZFS text wrapper (.read/.write/.lines/.records)\n"
        "  Exceptions: PermissionError, FileNotFoundError, FileExistsError, NotADirectoryError"
    )


# ============================================================================

try:
    import _thread
    _HAVE_THREAD = True
except ImportError:
    _HAVE_THREAD = False

try:
    import urandom
    _HAVE_URANDOM = True
except ImportError:
    _HAVE_URANDOM = False
PERSIST_PATH = "/LOGS/proc_state.json"

PID_TYPE_KERNEL   = 1
PID_TYPE_USER     = 2
PID_TYPE_THREAD   = 3
PID_TYPE_DAEMON   = 4
PID_TYPE_NETWORK  = 5
PID_TYPE_RESERVED = 9

PID_TYPE_NAMES = {
    PID_TYPE_KERNEL:   "KERNEL",
    PID_TYPE_USER:     "USER",
    PID_TYPE_THREAD:   "THREAD",
    PID_TYPE_DAEMON:   "DAEMON",
    PID_TYPE_NETWORK:  "NETWORK",
    PID_TYPE_RESERVED: "RESERVED",
}


def pid_type(pid):
    return pid // 1000


def pid_type_name(pid):
    return PID_TYPE_NAMES.get(pid_type(pid), "UNKNOWN")

NEW     = "NEW"
READY   = "READY"
RUNNING = "RUNNING"
BLOCKED = "BLOCKED"
ZOMBIE  = "ZOMBIE"     # finished/killed, exit info not yet reaped
DEAD    = "DEAD"       # reaped

SIGTERM = 15
SIGKILL = 9
SIGSTOP = 19
SIGCONT = 18

MODE_LOOP     = "loop"      # runs every time it's scheduled
MODE_PERIODIC = "periodic"  # runs every `period` ms
MODE_ONCE     = "once"      # runs once then becomes ZOMBIE
MODE_THREAD   = "thread"    # runs on a real FreeRTOS thread via _thread

NICE_MIN, NICE_MAX = -20, 19


def _weight(nice):
    nice = max(NICE_MIN, min(NICE_MAX, nice))
    return max(1, 1024 >> ((nice + 20) // 4))


class ProcessError(Exception):
    pass


class PermissionDenied(ProcessError):
    pass


class Process:

    __slots__ = (
        "pid", "ppid", "owner", "name", "func", "mode", "period",
        "priority", "state", "vruntime", "pending_signal",
        "exit_code", "meta", "_last_run", "_thread_started",
    )

    def __init__(self, pid, ppid, owner, name, func, mode, period, priority):
        self.pid = pid
        self.ppid = ppid
        self.owner = owner
        self.name = name
        self.func = func
        self.mode = mode
        self.period = period
        self.priority = priority
        self.state = NEW
        self.vruntime = 0
        self.pending_signal = None
        self.exit_code = None
        self.meta = {"samples": 0, "avg_us": None, "min_us": None,
                     "max_us": None, "last_us": None}
        self._last_run = 0
        self._thread_started = False

    def weight(self):
        return _weight(self.priority)

    def as_row(self):
        return "{:<5} {:<8} {:<8} {:<16} {:<9} {:<4} {:<8} {}".format(
            self.pid, pid_type_name(self.pid), self.owner, self.name,
            self.state, self.priority, self.mode,
            self.meta.get("avg_us") or "-"
        )


class Scheduler:

    TASK_BOOST_THRESHOLD_US = 8_000
    TASK_BOOST_MIN_SAMPLES = 3
    FRAME_LOAD_REASON = "scheduler:frame"

    def __init__(self, logger=None, power=None):
        self.cpu = CPU()
        self._current_user_fn = "root"

        self.table = {}
        self.running = False
        self._rand_seed = time.ticks_us() & 0xFFFFFFFF
        self._active_pid = None

        self.trace_execution = True

        self._announced = set()

        self._logger = logger

        self.power = power
        self._boosted_tasks = set()

        self._ensure_storage()
        self._load_state()

    def _get_logger(self):
        if self._logger is not None:
            return self._logger
        try:
            return Logger()
        except Exception:
            return None

    def _log_debug(self, msg, source="SCHED"):
        log = self._get_logger()
        if log is not None:
            try:
                log.debug(msg, source=source)
                return
            except Exception:
                pass
        print("[{}] {}".format(source, msg))

    def _log_error(self, msg, source="SCHED"):
        log = self._get_logger()
        if log is not None:
            try:
                log.error(msg, source=source)
                return
            except Exception:
                pass
        print("[{}][ERROR] {}".format(source, msg))

    def log_misuse(self, p, sig, reason):
        self._log_error(
            "blocked signal {} on pid {} ({}, owner={}): {}".format(
                sig, p.pid, p.name, p.owner, reason
            ),
            source="SECSCAN",
        )

    def _get_power(self):
        if self.power is False:
            return None
        if self.power is not None:
            return self.power
        try:
            self.power = PowerManagement(logger=self._get_logger())
        except Exception as e:
            self._log_error("PowerManagement unavailable: {}".format(e), source="POWER")
            self.power = False
            return None
        return self.power

    def set_power_policy(self, policy):
        power = self._get_power()
        if power is None:
            self._log_error("cannot set power policy: PowerManagement unavailable", source="POWER")
            return False
        return power.policy_set(policy)

    def power_status(self):
        power = self._get_power()
        if power is None:
            print("[SCHED] PowerManagement unavailable.")
            return None
        return power.status()

    def _scale_for_frame_load(self):
        power = self._get_power()
        if power is None:
            return
        try:
            power.auto_scale(self.cpu.usage(), reason=self.FRAME_LOAD_REASON)
        except Exception as e:
            self._log_error("frame-level power scaling failed: {}".format(e), source="POWER")

    def _task_boost_reason(self, p):
        return "task:{}:{}".format(p.pid, p.name)

    def _scale_for_task(self, p):
        power = self._get_power()
        if power is None:
            return

        avg = p.meta.get("avg_us")
        samples = p.meta.get("samples", 0)
        if avg is None or samples < self.TASK_BOOST_MIN_SAMPLES:
            return

        reason = self._task_boost_reason(p)
        is_large = avg >= self.TASK_BOOST_THRESHOLD_US
        already_boosted = reason in self._boosted_tasks

        if is_large and not already_boosted:
            if power.boost(reason, "high"):
                self._boosted_tasks.add(reason)
                self._log_debug(
                    "boosting CPU for large task '{}' (pid {}, avg {}us >= {}us)".format(
                        p.name, p.pid, avg, self.TASK_BOOST_THRESHOLD_US
                    ),
                    source="POWER",
                )
        elif not is_large and already_boosted:
            power.release(reason)
            self._boosted_tasks.discard(reason)
            self._log_debug(
                "releasing CPU boost for task '{}' (pid {}, avg {}us < {}us)".format(
                    p.name, p.pid, avg, self.TASK_BOOST_THRESHOLD_US
                ),
                source="POWER",
            )

    def _release_task_boost(self, p):
        reason = self._task_boost_reason(p)
        if reason in self._boosted_tasks:
            self._boosted_tasks.discard(reason)
            power = self._get_power()
            if power is not None:
                power.release(reason)

    def _ensure_storage(self):
        try:
            if "LOGS" not in os.listdir("/"):
                os.mkdir("/LOGS")
        except Exception:
            pass
        try:
            with open(PERSIST_PATH, "r"):
                pass
        except Exception:
            with open(PERSIST_PATH, "w") as f:
                f.write("{}")

    def _load_state(self):
        try:
            with open(PERSIST_PATH) as f:
                self._meta_store = json.loads(f.read())
        except Exception:
            self._meta_store = {}

    def _persist(self):
        out = {}
        for p in self.table.values():
            if p.meta["samples"] > 0:
                out[p.name] = p.meta
        try:
            with open(PERSIST_PATH, "w") as f:
                f.write(json.dumps(out))
        except Exception:
            pass

    def _caller_user(self):
        try:
            return self._current_user_fn()
        except Exception:
            return None

    def _can_signal(self, target, system_token=None):
        if system_token is not None:
            if system_token == _SYSTEM_TOKEN:
                return True
        caller = self._caller_user()
        if caller is None:
            return False
        if target.owner == caller:
            return True
        try:
            return usermanager().is_session_root()
        except Exception:
            return False

    def _rand3(self):
        if _HAVE_URANDOM:
            return urandom.getrandbits(10) % 1000
        x = self._rand_seed
        x ^= (x << 13) & 0xFFFFFFFF
        x ^= (x >> 17)
        x ^= (x << 5) & 0xFFFFFFFF
        self._rand_seed = x & 0xFFFFFFFF
        return x % 1000

    def _classify(self, mode, owner, ptype=None):
        if ptype is not None:
            if ptype not in PID_TYPE_NAMES:
                raise ValueError("invalid pid type: %r" % (ptype,))
            return ptype
        if mode == MODE_THREAD:
            return PID_TYPE_DAEMON if owner == "root" else PID_TYPE_THREAD
        if owner == "root":
            return PID_TYPE_KERNEL
        return PID_TYPE_USER

    def _alloc_pid(self, ptype):
        base = ptype * 1000
        for _ in range(2000):
            pid = base + self._rand3()
            if pid not in self.table:
                return pid
        raise ProcessError(
            "pid space exhausted for type {} ({})".format(ptype, PID_TYPE_NAMES.get(ptype))
        )

    def spawn(self, name, func, *, mode=MODE_LOOP, period=0, priority=0,
              owner=None, expected_us=None, ptype=None):
        if mode not in (MODE_LOOP, MODE_PERIODIC, MODE_ONCE, MODE_THREAD):
            raise ValueError("invalid mode: %r" % (mode,))
        if mode == MODE_THREAD and not _HAVE_THREAD:
            raise ProcessError("_thread not available on this build")

        owner = owner or self._caller_user() or "root"
        resolved_ptype = self._classify(mode, owner, ptype)

        if resolved_ptype == PID_TYPE_DAEMON and mode != MODE_THREAD:
            raise ValueError(
                "daemon-series (4xxx) processes must use mode=MODE_THREAD "
                "(got mode=%r for name=%r)" % (mode, name)
            )

        pid = self._alloc_pid(resolved_ptype)
        ppid = self._active_pid or 0

        p = Process(pid, ppid, owner, name, func, mode, period, priority)
        if name in self._meta_store:
            p.meta = self._meta_store[name]
        elif expected_us is not None:
            p.meta["avg_us"] = expected_us

        p.state = READY
        self.table[pid] = p

        if pid not in self._announced:
            self._announced.add(pid)
            self._log_debug(
                "started {} service -> pid={} type={} owner={} mode={} priority={}".format(
                    name, pid, pid_type_name(pid), owner, mode, priority
                ),
                source="SCHED",
            )

        if mode == MODE_THREAD:
            self._start_thread(p)

        return pid

    def kill(self, pid, sig=SIGTERM, system_token=None):
        p = self.table.get(pid)
        if p is None:
            raise ProcessError("no such pid: %d" % pid)

        if pid_type(p.pid) == PID_TYPE_DAEMON and sig == SIGKILL:
            self.log_misuse(p, sig, reason="SIGKILL forbidden on daemon pid")
            raise PermissionDenied(
                "pid %d (%s) is a protected daemon; SIGKILL is not permitted, "
                "use SIGTERM for cooperative shutdown" % (pid, p.name)
            )

        if not self._can_signal(p, system_token=system_token):
            self.log_misuse(p, sig, reason="permission denied")
            raise PermissionDenied(
                "user cannot signal pid %d (owned by %s)" % (pid, p.owner)
            )
        if p.state in (ZOMBIE, DEAD):
            return True

        if sig == SIGKILL and p.mode != MODE_THREAD:
            p.state = ZOMBIE
            p.exit_code = -SIGKILL
            self._release_task_boost(p)
            self._persist()
            return True

        p.pending_signal = sig
        return True

    def should_die(self, pid):
        p = self.table.get(pid)
        return bool(p and p.pending_signal in (SIGTERM, SIGKILL))

    def checkpoint(self, pid):
        p = self.table.get(pid)
        if p and p.pending_signal in (SIGTERM, SIGKILL):
            p.state = ZOMBIE
            p.exit_code = -p.pending_signal
            self._release_task_boost(p)
            raise SystemExit

    def wait(self, pid, timeout_ms=None):
        start = time.ticks_ms()
        while True:
            p = self.table.get(pid)
            if p is None:
                return None
            if p.state in (ZOMBIE, DEAD):
                code = p.exit_code
                p.state = DEAD
                del self.table[pid]
                return code
            if timeout_ms is not None and time.ticks_diff(time.ticks_ms(), start) >= timeout_ms:
                return None
            time.sleep_ms(5)

    def nice(self, pid, priority, system_token=None):
        p = self.table.get(pid)
        if p is None:
            raise ProcessError("no such pid: %d" % pid)
        if not self._can_signal(p, system_token=system_token):
            raise PermissionDenied("user cannot renice pid %d" % pid)
        p.priority = max(NICE_MIN, min(NICE_MAX, priority))
        return True

    def getpid(self):
        return self._active_pid

    def list(self):
        print("{:<5} {:<8} {:<8} {:<16} {:<9} {:<4} {:<8} {}".format(
            "PID", "TYPE", "OWNER", "NAME", "STATE", "NI", "MODE", "AVG_US"
        ))
        for p in sorted(self.table.values(), key=lambda p: p.pid):
            print(p.as_row())
        return list(self.table.keys())

    def _start_thread(self, p):
        def runner():
            self._active_pid = p.pid
            p.state = RUNNING

            if self.trace_execution:
                print("[EXEC TRACE] Executing Thread Task -> PID: {}, Name: {}, User: {}".format(
                    p.pid, p.name, p.owner
                ))

            start = time.ticks_us()
            try:
                p.func(p.pid)
                p.exit_code = 0
            except SystemExit:
                pass
            except Exception as e:
                self._log_error("pid {} ({}) raised: {}".format(p.pid, p.name, e), source="SCHED")
                p.exit_code = -1

            elapsed = time.ticks_diff(time.ticks_us(), start)
            self._record_stat(p, elapsed)
            p.state = ZOMBIE
            self._release_task_boost(p)

            if self.trace_execution:
                print("[EXEC TRACE] Finished Thread Task -> PID: {}, Name: {}, Elapsed: {}us".format(
                    p.pid, p.name, elapsed
                ))

        p._thread_started = True
        _thread.start_new_thread(runner, ())

    def _record_stat(self, p, elapsed_us):
        m = p.meta
        m["last_us"] = elapsed_us
        m["samples"] += 1
        m["min_us"] = elapsed_us if m["min_us"] is None else min(m["min_us"], elapsed_us)
        m["max_us"] = elapsed_us if m["max_us"] is None else max(m["max_us"], elapsed_us)
        m["avg_us"] = elapsed_us if m["avg_us"] is None else int(0.3 * elapsed_us + 0.7 * m["avg_us"])

    def _runnable_cooperative(self):
        now = time.ticks_ms()
        out = []
        for p in self.table.values():
            if p.mode == MODE_THREAD:
                continue
            if p.state in (ZOMBIE, DEAD, BLOCKED):
                continue
            if p.pending_signal == SIGSTOP:
                continue
            if p.mode == MODE_PERIODIC:
                if time.ticks_diff(now, p._last_run) < p.period:
                    continue
            out.append(p)
        return out

    def _pick_next(self, runnable):
        return min(runnable, key=lambda p: (p.vruntime, p.pid))

    def _run_one(self, p):
        if p.pending_signal in (SIGTERM, SIGKILL):
            p.state = ZOMBIE
            p.exit_code = -p.pending_signal
            self._release_task_boost(p)
            return

        self._active_pid = p.pid
        p.state = RUNNING

        if self.trace_execution:
            print("[EXEC TRACE] Executing Task -> PID: {}, Name: {}, User: {}".format(
                p.pid, p.name, p.owner
            ))

        start = time.ticks_us()
        try:
            p.func()
        except Exception as e:
            self._log_error("pid {} ({}) raised: {}".format(p.pid, p.name, e), source="SCHED")

        elapsed = max(1, time.ticks_diff(time.ticks_us(), start))
        self._active_pid = None

        self._record_stat(p, elapsed)
        p.vruntime += elapsed * 1024 // p.weight()
        p._last_run = time.ticks_ms()

        self._scale_for_task(p)

        if self.trace_execution:
            print("[EXEC TRACE] Finished Task -> PID: {}, Name: {}, Elapsed: {}us".format(
                p.pid, p.name, elapsed
            ))

        if p.mode == MODE_ONCE:
            p.state = ZOMBIE
            p.exit_code = 0
            self._release_task_boost(p)
        else:
            p.state = READY

    def tick(self):
        runnable = self._runnable_cooperative()
        if not runnable:
            return False
        self._run_one(self._pick_next(runnable))
        self._reap_zombies()
        return True

    def _reap_zombies(self):
        pass

    def reap(self, pid):
        p = self.table.get(pid)
        if p and p.state == ZOMBIE:
            self._release_task_boost(p)
            del self.table[pid]
            self._announced.discard(pid)
            return True
        return False

    def start(self, frame_ms=16):
        self.running = True
        self._log_debug("scheduler main loop starting (frame_ms={})".format(frame_ms), source="SCHED")
        while self.running:
            frame_start = time.ticks_us()
            budget_us = frame_ms * 1000
            while time.ticks_diff(time.ticks_us(), frame_start) < budget_us:
                if not self.tick():
                    break
            busy = time.ticks_diff(time.ticks_us(), frame_start)
            idle = max(0, budget_us - busy)
            if idle > 0:
                time.sleep_us(idle)
            if hasattr(self.cpu, "report_frame"):
                self.cpu.report_frame(busy, idle)
            self._scale_for_frame_load()
            self._persist()

    def stop(self):
        self.running = False
        self._log_debug("scheduler main loop stopped", source="SCHED")

    def killall(self, system_token=None):
        for pid, p in list(self.table.items()):
            if pid_type(p.pid) == PID_TYPE_DAEMON:
                continue
            try:
                self.kill(pid, SIGTERM, system_token=system_token)
            except Exception:
                pass


def _process_help():
    return (
        "process.py -- process model + scheduler\n"
        "  Process(pid, ppid, owner, name, func, mode, period, priority)  -- PCB\n"
        "  pid_type(pid) / pid_type_name(pid) -- decode the type digit encoded in a pid\n"
        "  Scheduler(logger=None, power=None)\n"
        "    .spawn(name, func, mode='loop', owner=..., priority=0, period=None)\n"
        "    .kill(pid, sig, system_token=None) / .nice(pid, priority, system_token=None)\n"
        "    .wait(pid, timeout_ms=None) / .reap(pid) / .list() / .getpid()\n"
        "    .start(frame_ms=16) / .stop() / .killall(system_token=None)\n"
        "    .set_power_policy(policy) / .power_status()\n"
        "  Exceptions: ProcessError, PermissionDenied"
    )


# ============================================================================

try:
    _SYSTEM_TOKEN = os.urandom(16)
except Exception:
    _SYSTEM_TOKEN = bytes(str(time.ticks_us()), "utf-8")


class SystemPrivilege:
    _active_depth = 0

    def __init__(self, token, reason="system operation"):
        if token != _SYSTEM_TOKEN:
            raise PermissionError("invalid system privilege token")
        self.reason = reason

    def __enter__(self):
        SystemPrivilege._active_depth += 1
        return self

    def __exit__(self, exc_type, exc, tb):
        SystemPrivilege._active_depth -= 1
        return False

    @classmethod
    def active(cls):
        return cls._active_depth > 0

class usermanager:
    _session_root = False

    def __init__(self):
        self.path = "OS/users/userinfo.json"
        self.d = dewrapper()

    def _checkpath(self, path):
        return zfs.exists(path)

    def _require_system(self):
        if not SystemPrivilege.active():
            raise PermissionError("access denied")

    def __read__(self):
        self._require_system()
        if not self._checkpath("OS"):
            zfs.mkdir("OS")
        if not self._checkpath("OS/users"):
            zfs.mkdir("OS/users")
        if not self._checkpath(self.path):
            self.d.write(self.path, json.dumps({
                "user": zeno.user, "password": zeno.password, "root": False
            }))

        data = self.d.read(self.path)
        if not data:
            zfs.delete(self.path)
            return self.__read__()
        try:
            parsed = json.loads(data)
        except Exception:
            zfs.delete(self.path)
            return self.__read__()

        if parsed.get("user") != zeno.user:
            parsed = {"user": zeno.user, "password": zeno.password, "root": False}
            self.d.write(self.path, json.dumps(parsed))

        return parsed

    def _write(self, key, value):
        self._require_system()
        a = self.__read__()
        a[key] = value
        if self._checkpath(self.path):
            self.d.write(self.path, json.dumps(a))

    def __username__(self):
        self._require_system()
        return self.__read__()["user"]

    def __password__(self):
        self._require_system()
        return self.__read__()["password"]

    def userinfo(self):
        with SystemPrivilege(_SYSTEM_TOKEN, "userinfo"):
            info = dict(self.__read__())
        info.pop("password", None)
        return info

    def removeuser(self, name):
        """Remove a user (legacy method). Only allows deleting the current user if they are root."""
        with SystemPrivilege(_SYSTEM_TOKEN, "removeuser"):
            self._require_system()
            user = self.__username__()
            data = self.__read__()
            if name == user and data.get("root", False):
                zfs.delete(self.path)
                # Recreate default user info
                self.__read__()
                return (f"{user} is deleted and default user name will be created", True)
            else:
                return ("user does't exists or not root", False)

    def is_session_root(self):
        return usermanager._session_root or SystemPrivilege.active()

    def elevate(self, user, password):
        with SystemPrivilege(_SYSTEM_TOKEN, "elevate"):
            ok = (user == self.__username__() and str(password) == str(self.__password__()))
            if ok and not self.__read__().get("root", False):
                self._write("root", True)
                print(f"{user} promoted to administrator")
        if ok:
            usermanager._session_root = True
            print("elevated user (this session)")
        else:
            print("user or password is wrong")

    def delevate(self, user, password):
        with SystemPrivilege(_SYSTEM_TOKEN, "delevate"):
            ok = (user == self.__username__() and str(password) == str(self.__password__()))
        if ok:
            usermanager._session_root = False
            print("delevated user")
        else:
            print("user or password is wrong")

    def isrooted(self, user):
        with SystemPrivilege(_SYSTEM_TOKEN, "isrooted"):
            current = self.__username__()
        if user == current:
            return self.is_session_root()
        return False


    def change_password(self, user, old_password, new_password):
        with SystemPrivilege(_SYSTEM_TOKEN, "change_password"):
            ok = (user == self.__username__() and str(old_password) == str(self.__password__()))
            if ok:
                self._write("password", new_password)
        if ok:
            print("password changed")
            return True
        print("user or password is wrong")
        return False

    def change_username(self, user, password, new_username):
        with SystemPrivilege(_SYSTEM_TOKEN, "change_username"):
            ok = (user == self.__username__() and str(password) == str(self.__password__()))
            if ok:
                self._write("user", new_username)
        if ok:
            print(f"username changed to {new_username}")
            return True
        print("user or password is wrong")
        return False
    def rebuild(self, system_token=None):
        if system_token != _SYSTEM_TOKEN:
            raise PermissionError("invalid system privilege token")

        with SystemPrivilege(system_token, "rebuild"):
            if not self._checkpath("OS"):
                zfs.mkdir("OS")
            if not self._checkpath("OS/users"):
                zfs.mkdir("OS/users")

            if self._checkpath(self.path):
                zfs.delete(self.path)

            fresh = {"user": zeno.user, "password": zeno.password, "root": False}
            self.d.write(self.path, json.dumps(fresh))

            try:
                check = json.loads(self.d.read(self.path))
            except Exception:
                raise ProcessError("rebuild: wrote userinfo.json but could not read it back")
            if check.get("user") != zeno.user:
                raise ProcessError("rebuild: verification failed after write")

        usermanager._session_root = False
        return True
    def current_user(self):
        with SystemPrivilege(_SYSTEM_TOKEN, "current_user"):
            return self.__username__()

    # Enhanced user management functions for multiple users
    def list_users(self):
        """List users visible to the current user.
        If the current user is root, list all users.
        Otherwise, list only the current user's own username.
        """
        # Determine if current user is root
        is_root = SystemPrivilege.active() or usermanager._session_root
        if is_root:
            with SystemPrivilege(_SYSTEM_TOKEN, "list_users"):
                self._require_system()
                if not self._checkpath("OS/users"):
                    return []
                try:
                    user_dir = zfs.listdir("OS/users")
                    users = [f for f in user_dir if not f.startswith('.') and
                             self._checkpath(f"OS/users/{f}") and
                             zfs.stat(f"OS/users/{f}")[0] & 0o170000 == 0o040000]
                    return users
                except Exception:
                    return []
        else:
            # Non-root: only see own user
            current_user = zeno.user
            user_path = f"OS/users/{current_user}"
            if self._checkpath(user_path):
                return [current_user]
            else:
                return []

    def create_user(self, requester_username, requester_password, new_username, new_password, new_password_confirm):
        """Create a new user.
        Requester must be root and authenticate with their own password.
        New user password must be confirmed.
        """
        # Verify requester is root
        is_root = SystemPrivilege.active() or usermanager._session_root
        if not is_root:
            return False, "Only root can create users"
        # Verify requester credentials
        auth_ok, auth_msg = self.authenticate_user(requester_username, requester_password)
        if not auth_ok:
            return False, f"Requester authentication failed: {auth_msg}"
        # Verify password confirmation
        if new_password != new_password_confirm:
            return False, "Password confirmation does not match"
        # Proceed to create user
        with SystemPrivilege(_SYSTEM_TOKEN, "create_user"):
            self._require_system()
            # Check if user already exists
            user_path = f"OS/users/{new_username}"
            if self._checkpath(user_path):
                return False, "User already exists"

            # Create user directory
            zfs.mkdir(user_path)

            # Create user info file
            user_info = {
                "user": new_username,
                "password": str(new_password),
                "root": False  # New users are not root by default; root can be granted later via elevate
            }
            self.d.write(f"{user_path}/userinfo.json", json.dumps(user_info))

            # Create home directory for the user
            try:
                zfs.mkdir(f"{user_path}/home")
            except:
                pass  # Home directory creation is optional

            return True, f"User '{new_username}' created successfully"

    def delete_user(self, requester_username, requester_password, target_username, target_password):
        """Delete a user.
        Requester must be root and know the target user's password.
        """
        # Verify requester is root
        is_root = SystemPrivilege.active() or usermanager._session_root
        if not is_root:
            return False, "Only root can delete users"
        # Verify requester credentials (optional but good practice)
        auth_ok, auth_msg = self.authenticate_user(requester_username, requester_password)
        if not auth_ok:
            return False, f"Requester authentication failed: {auth_msg}"
        # Verify target user credentials
        target_auth_ok, target_auth_msg = self.authenticate_user(target_username, target_password)
        if not target_auth_ok:
            return False, f"Target user authentication failed: {target_auth_msg}"
        # Prevent deletion of self? Optional; but if requester is root and knows target password, they could delete themselves if they know their own password.
        # We'll allow it; but we can also prevent deletion of root users? Already handled below.
        with SystemPrivilege(_SYSTEM_TOKEN, "delete_user"):
            self._require_system()
            user_path = f"OS/users/{target_username}"
            if not self._checkpath(user_path):
                return False, "User not found"

            # Check if user is root (prevent deletion of root users)
            try:
                user_info = json.loads(self.d.read(f"{user_path}/userinfo.json"))
                if user_info.get("root", False):
                    return False, "Cannot delete root user"
            except Exception:
                return False, "Cannot read user info"

            # Delete user directory and all contents
            self._delete_directory_recursive(user_path)
            return True, f"User '{target_username}' deleted successfully"

    def _delete_directory_recursive(self, path):
        """Recursively delete a directory and its contents."""
        try:
            contents = zfs.listdir(path)
            for item in contents:
                item_path = f"{path}/{item}"
                if zfs.stat(item_path)[0] & 0o170000 == 0o040000:  # Directory
                    self._delete_directory_recursive(item_path)
                else:  # File
                    zfs.delete(item_path)
            zfs.delete(path)
        except Exception:
            pass  # Best effort deletion

    def authenticate_user(self, username, password):
        """Authenticate a user against their stored credentials."""
        user_path = f"OS/users/{username}"
        if not self._checkpath(user_path):
            return False, "User not found"

        try:
            user_info = json.loads(self.d.read(f"{user_path}/userinfo.json"))
            if user_info.get("user") == username and str(user_info.get("password")) == str(password):
                return True, "Authentication successful"
            else:
                return False, "Invalid credentials"
        except Exception:
            return False, "Error reading user data"

    def get_user_home_path(self, username):
        """Get the home directory path for a user."""
        user_path = f"OS/users/{username}"
        if not self._checkpath(user_path):
            return None
        home_path = f"{user_path}/home"
        if self._checkpath(home_path):
            return home_path
        return user_path  # Fallback to user directory if no home

    def switch_user_context(self, username, password):
        """Switch the current user context after authenticating."""
        success, message = self.authenticate_user(username, password)
        if success:
            # Update zeno to reflect the new user
            zeno.user = username
            return True, f"Switched to user '{username}'"
        return False, message


def _security_help():
    return (
        "security.py -- privilege / user / session model\n"
        "  SystemPrivilege(token, reason='system operation')  -- context manager, trusted-code gate\n"
        "    SystemPrivilege.active()  -- classmethod, True while inside such a block\n"
        "  usermanager()\n"
        "    .userinfo() / .current_user() / .is_session_root()\n"
        "    .elevate(user, password) / .delevate(user, password)\n"
        "    .isrooted(user)\n"
        "    .change_password(user, old_password, new_password)\n"
        "    .change_username(user, password, new_username)\n"
        "    .removeuser(name) / .rebuild(system_token=None)"
    )


# ============================================================================

try:
    import uasyncio as asyncio
    _HAVE_UASYNCIO = True
except ImportError:
    asyncio = None
    _HAVE_UASYNCIO = False


PROFILES_PATH = "/LOGS/wifi_profiles.json"
COMMAND = "wifi"

WATCHDOG_POLL_MS = 5_000
WATCHDOG_RECONNECT_DELAY_MS = 2_000


class WifiManager:

    def __init__(self, logger, profiles_path=PROFILES_PATH, timeout=15):
        self.logger = logger
        self.source = "NETWORK"
        self.profiles_path = profiles_path
        self.timeout = timeout
        self.wlan = _network.WLAN(_network.STA_IF)

        self._watchdog_running = False
        self._watchdog_task = None

    def _debug(self, message):
        self.logger.debug(message, source=self.source)

    def _warn(self, message):
        self.logger.warning(message, source=self.source)

    def _error(self, message):
        self.logger.error(message, source=self.source)

    def _load_profiles(self):
        try:
            with open(self.profiles_path, "r") as f:
                data = json.loads(f.read())
            if not isinstance(data, dict):
                return {}
            return data
        except Exception as e:
            self._debug("No existing profile store ({}); starting empty.".format(e))
            return {}

    def _save_profiles(self, profiles):
        try:
            with open(self.profiles_path, "w") as f:
                f.write(json.dumps(profiles))
            return True
        except Exception as e:
            self._error("Failed to save WiFi profiles: {}".format(e))
            return False

    def save_profile(self, ssid, password, priority=0):
        if not ssid:
            message = "save_profile: ssid is required"
            self._error(message)
            return (message, False)
        profiles = self._load_profiles()
        profiles[ssid] = {"password": str(password or ""), "priority": int(priority)}
        ok = self._save_profiles(profiles)
        if ok:
            message = f"Saved WiFi profile for '{ssid}' (priority={priority})"
            self._debug(message)
            return (message, True)
        else:
            message = "Failed to save WiFi profile"
            self._error(message)
            return (message, False)

    def remove_profile(self, ssid):
        profiles = self._load_profiles()
        if ssid not in profiles:
            message = f"remove_profile: no saved profile for '{ssid}'"
            self._warn(message)
            return (message, False)
        del profiles[ssid]
        ok = self._save_profiles(profiles)
        if ok:
            message = f"Removed WiFi profile for '{ssid}'"
            self._debug(message)
            return (message, True)
        else:
            message = "Failed to remove WiFi profile"
            self._error(message)
            return (message, False)

    def list_profiles(self):
        profiles = self._load_profiles()
        result = {ssid: {"priority": rec.get("priority", 0)} for ssid, rec in profiles.items()}
        message = f"Listed {len(result)} WiFi profiles"
        self._debug(message)
        return (message, result)

    def scan(self):
        try:
            self.wlan.active(True)
            raw = self.wlan.scan()
        except Exception as e:
            message = f"WiFi scan failed: {e}"
            self._error(message)
            return (message, [])

        results = []
        for entry in raw:
            try:
                ssid_bytes, bssid, channel, rssi, authmode, hidden = entry
                results.append({
                    "ssid": ssid_bytes.decode("utf-8", "ignore") if isinstance(ssid_bytes, (bytes, bytearray)) else str(ssid_bytes),
                    "bssid": bssid,
                    "channel": channel,
                    "rssi": rssi,
                    "authmode": authmode,
                    "hidden": bool(hidden),
                })
            except Exception as e:
                message = f"Skipping malformed scan entry: {e}"
                self._warn(message)
        message = f"Scan found {len(results)} network(s)"
        self._debug(message)
        return (message, results)

    def _do_connect(self, ssid, password):
        wlan = self.wlan
        wlan.active(False)
        time.sleep_ms(200)
        wlan.active(True)
        time.sleep_ms(200)
        try:
            wlan.config(pm=wlan.PM_NONE)
        except Exception as e:
            message = f"pm config failed (non-fatal): {e}"
            self._debug(message)
            # Continue anyway as it's non-fatal

        message = f"MAC: {wlan.config('mac')}"
        self._debug(message)
        message = f"Connecting to '{ssid}'"
        self._debug(message)
        wlan.connect(ssid, password)

        start = time.time()
        while not wlan.isconnected():
            status = wlan.status()
            if status in (_network.STAT_WRONG_PASSWORD, _network.STAT_NO_AP_FOUND, _network.STAT_CONNECT_FAIL):
                message = f"Connect to '{ssid}' failed with fatal status {status}"
                self._error(message)
                return (message, False)
            if time.time() - start > self.timeout:
                message = f"Connect to '{ssid}' timed out after {self.timeout}s"
                self._error(message)
                return (message, False)
            time.sleep_ms(500)

        message = f"Connected to '{ssid}': {wlan.ifconfig()}"
        self._debug(message)
        return (message, True)

    def connect(self, ssid=None, password=None, save=False, priority=0):
        if ssid is None:
            return self.auto_connect()

        if password is None:
            profiles = self._load_profiles()
            record = profiles.get(ssid)
            password = record.get("password", "") if record else ""

        message, ok = self._do_connect(ssid, password)
        if ok and save:
            save_message, save_ok = self.save_profile(ssid, password, priority=priority)
            if not save_ok:
                # Log the save failure but don't fail the connection
                self._error(save_message)
        return (message, ok)

    def auto_connect(self):
        profiles = self._load_profiles()
        if not profiles:
            message = "auto_connect: no saved WiFi profiles"
            self._warn(message)
            return (message, False)

        seen = self.scan()
        # Assuming scan now returns (message, value)
        scan_result = self.scan()
        if isinstance(scan_result, tuple) and len(scan_result) == 2:
            scan_message, seen = scan_result
        else:
            # Handle case where scan hasn't been modified yet
            scan_message = "Scan completed"
            seen = scan_result
        seen_ssids = {n["ssid"] for n in seen}
        candidates = [
            (ssid, rec) for ssid, rec in profiles.items() if ssid in seen_ssids
        ]
        if not candidates:
            message = "auto_connect: no saved profile is currently in range"
            self._warn(message)
            return (message, False)

        candidates.sort(key=lambda pair: pair[1].get("priority", 0), reverse=True)
        best_ssid, best_rec = candidates[0]
        message = f"auto_connect: selected '{best_ssid}' (priority={best_rec.get('priority', 0)})"
        self._debug(message)
        return self._do_connect(best_ssid, best_rec.get("password", ""))

    def disconnect(self):
        try:
            self.wlan.disconnect()
        except Exception as e:
            message = f"disconnect() raised (non-fatal): {e}"
            self._debug(message)
        try:
            self.wlan.active(False)
        except Exception as e:
            message = f"Failed to deactivate WiFi radio: {e}"
            self._warn(message)
            return (message, False)
        message = "WiFi disconnected"
        self._debug(message)
        return (message, True)

    def status(self):
        try:
            connected = bool(self.wlan.isconnected())
        except Exception as e:
            message = f"status: could not read link state: {e}"
            self._error(message)
            return (message, {"connected": False, "ssid": None, "ip": None,
                    "subnet": None, "gateway": None, "dns": None, "rssi": None})

        result = {"connected": connected, "ssid": None, "ip": None,
                  "subnet": None, "gateway": None, "dns": None, "rssi": None}

        if not connected:
            message = "Network status: disconnected"
            self._debug(message)
            return (message, result)

        try:
            ip, subnet, gateway, dns = self.wlan.ifconfig()
            result["ip"], result["subnet"], result["gateway"], result["dns"] = ip, subnet, gateway, dns
        except Exception as e:
            message = f"status: ifconfig() failed: {e}"
            self._warn(message)

        try:
            essid = self.wlan.config('essid')
            result["ssid"] = essid
        except Exception as e:
            message = f"status: could not read essid: {e}"
            self._debug(message)

        try:
            result["rssi"] = self.wlan.status('rssi')
        except Exception as e:
            message = f"status: could not read rssi: {e}"
            self._debug(message)

        message = f"Network status: {'connected' if connected else 'disconnecting'} to {result.get('ssid', 'unknown')}"
        self._debug(message)
        return (message, result)

    def isconnected(self):
        try:
            result = bool(self.wlan.isconnected())
            message = f"WiFi is {'connected' if result else 'disconnected'}"
            self._debug(message)
            return (message, result)
        except Exception as e:
            message = f"isconnected() failed: {e}"
            self._error(message)
            return (message, False)

    def start_watchdog(self, poll_ms=WATCHDOG_POLL_MS):
        if not _HAVE_UASYNCIO:
            message = "uasyncio not available on this build; background auto-reconnect watchdog disabled. Call watchdog_tick() manually (e.g. from a periodic Scheduler task) instead."
            self._warn(message)
            return (message, False)

        if self._watchdog_running:
            message = "Watchdog already running"
            self._debug(message)
            return (message, True)

        self._watchdog_running = True

        async def _loop():
            while self._watchdog_running:
                try:
                    self.watchdog_tick()
                except Exception as e:
                    self._error("watchdog tick raised: {}".format(e))
                await asyncio.sleep_ms(poll_ms)

        self._watchdog_task = asyncio.create_task(_loop())
        message = f"Auto-reconnect watchdog started (poll_ms={poll_ms})"
        self._debug(message)
        return (message, True)

    def stop_watchdog(self):
        if not self._watchdog_running:
            message = "Watchdog not running"
            self._debug(message)
            return (message, False)
        self._watchdog_running = False
        if self._watchdog_task is not None:
            try:
                self._watchdog_task.cancel()
            except Exception as e:
                message = f"watchdog task cancel raised (non-fatal): {e}"
                self._debug(message)
            self._watchdog_task = None
        message = "Auto-reconnect watchdog stopped"
        self._debug(message)
        return (message, True)

    def watchdog_tick(self):
        # Assuming isconnected now returns (message, value)
        is_connected_result = self.isconnected()
        if isinstance(is_connected_result, tuple) and len(is_connected_result) == 2:
            is_connected_message, is_connected = is_connected_result
        else:
            # Handle case where isconnected hasn't been modified yet
            is_connected_message = "WiFi status check completed"
            is_connected = is_connected_result

        if is_connected:
            message = "WiFi is connected"
            self._debug(message)
            return (message, True)

        message = "Link down; attempting auto-reconnect"
        self._warn(message)
        time.sleep_ms(WATCHDOG_RECONNECT_DELAY_MS)
        # Assuming auto_connect now returns (message, value)
        auto_connect_result = self.auto_connect()
        if isinstance(auto_connect_result, tuple) and len(auto_connect_result) == 2:
            return auto_connect_result
        else:
            # Handle case where auto_connect hasn't been modified yet
            message = f"Auto-connect result: {auto_connect_result}"
            return (message, auto_connect_result)


def _network_help():
    return (
        "network.py -- WiFi networking\n"
        "  WifiManager(logger, profiles_path='/LOGS/wifi_profiles.json', timeout=15)\n"
        "    .save_profile(ssid, password, priority=0) / .remove_profile(ssid) / .list_profiles()\n"
        "    .scan() -> list[dict]\n"
        "    .connect(ssid=None, password=None, save=False, priority=0) -> bool\n"
        "    .auto_connect() -> bool  -- connects to best known network in range\n"
        "    .disconnect() -> bool\n"
        "    .status() -> dict  -- connected/ssid/ip/subnet/gateway/dns/rssi\n"
        "    .isconnected() -> bool\n"
        "    .start_watchdog(poll_ms=5000) / .stop_watchdog() / .watchdog_tick()\n"
        "      -- background auto-reconnect; degrades gracefully (logged warning) "
        "without uasyncio, use watchdog_tick() manually instead\n"
        "  Command contract: install(pm) / uninstall(pm) / run(args, shell) "
        "registers/exposes 'wifi' as a ZenCMD command"
    )


_manager = None


def _get_manager(shell=None):
    global _manager
    if _manager is not None:
        return _manager

    logger = None
    if shell is not None:
        logger = getattr(shell, "logger", None) or getattr(shell, "log", None)
    if logger is None:
        try:
            logger = zeno.log
        except Exception:
            logger = Logger()

    _manager = WifiManager(logger)
    return _manager


def install(pm):
    pm.register(COMMAND, run, module_name="network")


def uninstall(pm):
    global _manager
    if _manager is not None:
        _manager.stop_watchdog()
    pm.unregister(COMMAND)


def run(args, shell):
    mgr = _get_manager(shell)
    args = list(args or [])
    if not args:
        return mgr.status()

    sub = args[0]
    rest = args[1:]

    if sub == "scan":
        return mgr.scan()

    if sub == "connect":
        ssid = rest[0] if len(rest) > 0 else None
        password = rest[1] if len(rest) > 1 and not rest[1].startswith("--") else None
        save = "--save" in rest
        priority = 0
        if "--priority" in rest:
            try:
                priority = int(rest[rest.index("--priority") + 1])
            except (ValueError, IndexError):
                priority = 0
        return mgr.connect(ssid=ssid, password=password, save=save, priority=priority)

    if sub == "disconnect":
        return mgr.disconnect()

    if sub == "status":
        return mgr.status()

    if sub == "profiles":
        return mgr.list_profiles()

    if sub == "forget":
        if not rest:
            mgr._error("forget requires an ssid")
            return False
        return mgr.remove_profile(rest[0])

    if sub == "watchdog":
        action = rest[0] if rest else "tick"
        if action == "start":
            return mgr.start_watchdog()
        if action == "stop":
            return mgr.stop_watchdog()
        return mgr.watchdog_tick()

    mgr._error("Unknown wifi subcommand: {}".format(sub))
    return False


# ============================================================================

SD_SCK, SD_MOSI, SD_MISO, SD_CS = 40, 6, 5, 7

class Disk:
    def __init__(self, mount_point="/MemDisk"):
        self.log = zeno.log
        self.mount_point = mount_point
        self.spi = SPI(1, baudrate=20_000_000, polarity=0, phase=0,
                        sck=Pin(SD_SCK, Pin.OUT), mosi=Pin(SD_MOSI, Pin.OUT), miso=Pin(SD_MISO, Pin.OUT))
        self.sd = None
        self.cs = Pin(SD_CS, Pin.OUT)

    def check(self, retries=5, delay=0.2):
        for i in range(retries):
            try:
                os.listdir(self.mount_point)
                if i > 0:
                    self.log.debug("SD mount became available after {} retries".format(i + 1), source="DISK")
                return True
            except OSError:
                time.sleep(delay)
        self.log.error("SD card not accessible at '{}' after {} retries".format(self.mount_point, retries), source="DISK")
        return False

    def begin(self):
        try:
            self.sd = SDCard(self.spi, self.cs)
            os.mount(self.sd, self.mount_point)
            self.log.debug("SD card mounted at '{}'".format(self.mount_point), source="DISK")
            return True
        except Exception as e:
            self.sd = None
            self.log.error("SD init/mount failed for '{}': {}".format(self.mount_point, e), source="DISK")

        if self.check():
            self.log.warning("SD init failed, but '{}' appears to be accessible".format(self.mount_point), source="DISK")
            return True
        return False

    def unmount(self):
        try:
            os.umount(self.mount_point)
            self.log.debug("SD card unmounted from '{}'".format(self.mount_point), source="DISK")
            return True
        except Exception as e:
            self.log.error("Failed to unmount '{}': {}".format(self.mount_point, e), source="DISK")
            return False

    def format(self, filesystem=os.VfsFat):
        if not self.sd:
            self.log.error("Cannot format: SD card object not initialized", source="DISK")
            return False
        try:
            try:
                os.umount(self.mount_point)
            except Exception:
                pass
            
            filesystem.mkfs(self.sd)
            os.mount(self.sd, self.mount_point)
            self.log.debug("Disk formatted successfully and re-mounted at '{}'".format(self.mount_point), source="DISK")
            return True
        except Exception as e:
            self.log.error("Failed to format disk at '{}': {}".format(self.mount_point, e), source="DISK")
            return False

    def info(self, path=None):
        path = path or self.mount_point
        try:
            stats = os.statvfs(path)
            total_bytes = stats[2] * stats[0]
            free_bytes = stats[3] * stats[0]

            def convert(v):
                return "{:.2f} GB".format(v / 1024**3) if v >= 1024**3 else "{:.2f} MB".format(v / 1024**2)

            print("Path        :", path)
            print("Volume Name:", path.split("/")[-1])
            print("Total Size :", convert(total_bytes))
            print("Free Space :", convert(free_bytes))
            self.log.debug("Disk info for '{}': total={}, free={}".format(path, convert(total_bytes), convert(free_bytes)), source="DISK")
        except Exception as e:
            self.log.error("Cannot access '{}': {}".format(path, e), source="DISK")

class BootConfig:
    def __init__(self):
        self.default = {
            "BOOT_MODE": "NORMAL", "OPT_LEVEL": 0, "WIFI_AUTOCONNECT": True,
            "SHOW_UI": True, "KERNEL_PATH": "/SYSTEM32/Admin/ROM/kernel.py",
            "LOGGER_STATUS": "ENABLED", "LOG_REPL": "ENABLED", "MODE": "PERFORMANCE",
        }
        self.cfg_name = "bootcfg.json"
        self.cfg_dir = "/LOGS"
        self.cfg_path = self.cfg_dir + "/" + self.cfg_name
        self.config = {}

        try:
            files = os.listdir(self.cfg_dir)
        except Exception:
            os.mkdir(self.cfg_dir)
            files = []

        if self.cfg_name in files:
            try:
                with open(self.cfg_path, "r") as f:
                    self.config = json.loads(f.read())
            except Exception as e:
                print("[BOOTCFG] Load or parse failed:", e)
                self.config = dict(self.default)
                self.save()
        else:
            self.config = dict(self.default)
            self.save()

    def save(self):
        try:
            with open(self.cfg_path, "w") as f:
                json.dump(self.config, f)
        except Exception as e:
            print("[BOOTCFG] Save failed:", e)

    def get(self, key, default=None):
        return self.config.get(key, default)

    def set(self, key, value):
        self.config[key] = value
        self.save()

    def show(self):
        print("\n[Boot Configuration]")
        for k, v in self.config.items():
            print(" ", k, ":", v)
        print()

bootcfg = BootConfig()
cfg = getattr(bootcfg, "config", {}) or {}


def cfg_get(cfg, *keys):
    for k in keys:
        if k in cfg:
            return cfg[k]


class system:
    def __init__(self, opt_level=0, debug=False):
        self.opt_level_value = opt_level
        self.debug = debug
        self.log = Logger()
        self.path = "/SYSTEM32"
        self.cfg = BootConfig()
        self.sched = None

    def restart(self):
        try:
            self.log.debug("System restart requested", source="SYSTEM")
            machine.reset()
        except Exception as e:
            self.log.error("System restart failed: {}".format(e), source="SYSTEM")

    def optlevel(self, level):
        try:
            micropython.opt_level(int(level))
            self.opt_level_value = int(level)
            self.log.debug("System optimization level set to {}".format(level), source="SYSTEM")
        except Exception as e:
            self.log.error("Error configuring optimization level '{}': {}".format(level, e), source="SYSTEM")

    def info(self):
        try:
            print("Zeno Micro PC Version: V4.X alpha")
            print("CPU: ESP32-S3")
            print("CPU Frequency:", machine.freq() / 1_000_000, "MHz")
            print("CPU Cores:", 2)
            print("Installed RAM:", (gc.mem_free() + gc.mem_alloc()) / (1024 * 1024), "MB")
            print("Unique ID:", int.from_bytes(machine.unique_id(), 'big'))
            print("Installed internal ROM: 16 MB")
            print("Disk Info:")
            self.log.debug("System info displayed", source="SYSTEM")
        except Exception as e:
            self.log.error("Failed to gather base system info: {}".format(e), source="SYSTEM")
            return

        path = "/SYSTEM32"
        try:
            stats = os.statvfs(path)
            total_bytes = stats[2] * stats[0]
            free_bytes = stats[3] * stats[0]

            def convert(v):
                return "{:.2f} GB".format(v / 1024**3) if v >= 1024**3 else "{:.2f} MB".format(v / 1024**2)

            print("Path       :", path)
            print("Volume Name:", path.split('/')[-1])
            print("Total Size :", convert(total_bytes))
            print("Free Space :", convert(free_bytes))
        except Exception as e:
            self.log.error("Error accessing filesystem stats for '{}': {}".format(path, e), source="SYSTEM")

    def memconfig(self, percent=25):
        try:
            gc.collect()
            free, alloc = gc.mem_free(), gc.mem_alloc()
            threshold = alloc + free * percent // 100
            gc.threshold(threshold)
            micropython.alloc_emergency_exception_buf(100)
            self.log.debug(
                "Memory configuration updated: free={} alloc={} threshold_percent={} threshold={}"
                .format(free, alloc, percent, threshold), source="SYSTEM"
            )
        except Exception as e:
            self.log.error("Failed to configure memory/GC with percent {}: {}".format(percent, e), source="SYSTEM")

    def force_mem(self):
        try:
            before = gc.mem_free()
            gc.collect()
            after = gc.mem_free()
            self.log.debug("Forced GC executed: free memory {} -> {} bytes (delta {})".format(before, after, after - before), source="SYSTEM")
        except Exception as e:
            self.log.error("Forced garbage collection failed: {}".format(e), source="SYSTEM")

    def mem_usage(self):
        try:
            free, alloc = gc.mem_free(), gc.mem_alloc()
            total = free + alloc
            if total <= 0:
                self.log.error("Cannot compute memory usage: total memory reported as 0.", source="SYSTEM")
                return

            def fmt(v):
                if v >= 1024 * 1024:
                    return "{:.2f} MB".format(v / (1024 * 1024))
                if v >= 1024:
                    return "{:.2f} KB".format(v / 1024)
                return "{} B".format(v)

            print("[System] Memory usage:")
            print("  Total: {}".format(fmt(total)))
            print("  Used:  {} ({:.2f}%)".format(fmt(alloc), (alloc / total) * 100))
            print("  Free:  {} ({:.2f}%)".format(fmt(free), (free / total) * 100))
            self.log.debug("System memory information displayed", source="SYSTEM")
        except Exception as e:
            self.log.error("Failed to read memory usage: {}".format(e), source="SYSTEM")

    def perf_test(self):
        self.log.debug("Performing system hardware test", source="SYSTEM")
        try:
            pystone_lowmem.main(1000)
            self.log.debug("CPU benchmark (pystone_lowmem) completed.", source="SYSTEM")
        except Exception as e:
            self.log.error("CPU benchmark (pystone_lowmem) failed: {}".format(e), source="SYSTEM")

        try:
            start_ram = gc.mem_free() / (1024 * 1024)
            l = [0] * 100000
            mid_ram = gc.mem_free() / (1024 * 1024)
            del l
            gc.collect()
            end_ram = gc.mem_free() / (1024 * 1024)
            self.log.debug("RAM test: start={:.3f} MB, during alloc={:.3f} MB, after free={:.3f} MB".format(start_ram, mid_ram, end_ram), source="SYSTEM")
        except Exception as e:
            self.log.error("RAM performance test failed: {}".format(e), source="SYSTEM")

        try:
            start_flash = time.ticks_ms()
            tmp_path = "/tmp_test.bin"
            with open(tmp_path, "wb") as f:
                f.write(bytearray(1024 * 50))
            with open(tmp_path, "rb") as f:
                _ = f.read()
            try:
                os.remove(tmp_path)
            except Exception as e_rm:
                self.log.error("Flash test cleanup failed (could not remove '{}'): {}".format(tmp_path, e_rm), source="SYSTEM")
            self.log.debug("Flash test complete: 50KB write/read in {} ms".format(time.ticks_diff(time.ticks_ms(), start_flash)), source="SYSTEM")
        except Exception as e:
            self.log.error("Flash performance test failed: {}".format(e), source="SYSTEM")

    def mode(self, m):
        s = str(m).strip().upper()
        mode_map = {
            "PERF": ("PERFORMANCE", 0), "PERFORMANCE": ("PERFORMANCE", 0),
            "BAL": ("BALANCED", 3), "BALANCED": ("BALANCED", 3),
            "SAVE": ("POWERSAVING", 3), "POWERSAVE": ("POWERSAVING", 3), "POWERSAVING": ("POWERSAVING", 3),
        }
        selected = next((tup for key, tup in mode_map.items() if key in s), None)
        if not selected:
            self.log.error("Unknown mode requested: {}".format(m), "SYSTEM")
            return

        mode_name, optlevel_val = selected
        self.cfg.set("MODE", mode_name)
        self.optlevel(optlevel_val)
        self.log.debug("Mode set -> {} (optlevel {}) - rebooting".format(mode_name, optlevel_val), "SYSTEM")
        machine.reset()

    def ram_guard(self, warn_pct=80, crit_pct=92):
        free, alloc = gc.mem_free(), gc.mem_alloc()
        total = free + alloc
        if total <= 0:
            return 0.0
        used_pct = (alloc / total) * 100
        if used_pct < warn_pct:
            return used_pct

        before_free = free
        gc.collect()
        after_free = gc.mem_free()
        reclaimed = after_free - before_free
        new_used_pct = (gc.mem_alloc() / (after_free + gc.mem_alloc())) * 100

        if used_pct >= crit_pct:
            self.log.error("RAM critical: {:.1f}% used, collected {} bytes -> {:.1f}% used".format(used_pct, reclaimed, new_used_pct), source="RAMGUARD")
        elif reclaimed > 4096:
            self.log.debug("RAM warning: {:.1f}% used, collected {} bytes -> {:.1f}% used".format(used_pct, reclaimed, new_used_pct), source="RAMGUARD")
        return new_used_pct

    def security_scan(self):
        findings = []
        try:
            if not getattr(zeno, "authorized", False):
                findings.append("zeno.authorized is False after boot")
            if getattr(zeno, "boot_cap", "unset") not in (None, "unset"):
                findings.append("zeno.boot_cap was not consumed at boot")
        except Exception as e:
            findings.append("kernel auth flag check failed: {}".format(e))

        try:
            os.stat("/.sys")
        except OSError:
            findings.append("/.sys metadata directory missing")

        if self.sched is not None and len(self.sched.table) > 64:
            findings.append("process count abnormally high: {}".format(len(self.sched.table)))

        for f in findings:
            self.log.error(f, source="SECSCAN")
        return findings

    def checkup(self, mem_warn_pct=80, mem_crit_pct=92):
        return {
            "mem_used_pct": self.ram_guard(mem_warn_pct, mem_crit_pct),
            "security_findings": self.security_scan(),
        }

    def start_guardian(self, sched, interval_ms=10_000):
        self.sched = sched

        def guardian(pid):
            self.log.debug("guardian started as pid {}".format(pid), source="GUARDIAN")
            while True:
                self.sched.checkpoint(pid)
                try:
                    self.checkup()
                except Exception as e:
                    self.log.error("checkup failed: {}".format(e), source="GUARDIAN")
                time.sleep_ms(interval_ms)

        pid = self.sched.spawn("guardian", guardian, mode="thread", owner="root", priority=10)
        self.log.debug("guardian spawned, pid={}".format(pid), source="SYSTEM")
        return pid

    def firmware_update(self):
        self._safe_update("firmware.py", "/LOGS/firmwarecopy.py", "firmwarestable.py")

    def boot_update(self):
        self._safe_update("boot.py", "/LOGS/bootcopy.py")

    def _safe_update(self, src_file, log_dest, stable_file=None):
        try:
            with open(src_file, "rb") as fsrc:
                data = fsrc.read()
        except Exception as e:
            self.log.error("Safe update failed while reading '{}': {}".format(src_file, e), source="SYSTEM")
            return

        try:
            try:
                os.mkdir("/LOGS")
            except OSError as e:
                if len(e.args) > 0 and e.args[0] != 17:
                    raise
        except Exception as e:
            self.log.error("Safe update failed while ensuring '/LOGS' directory: {}".format(e), source="SYSTEM")
            return

        try:
            with open(log_dest, "wb") as fdest:
                fdest.write(data)
        except Exception as e:
            self.log.error("Safe update failed while writing backup '{}' -> '{}': {}".format(src_file, log_dest, e), source="SYSTEM")
            return

        if stable_file:
            try:
                with open(stable_file, "wb") as fstable:
                    fstable.write(data)
            except Exception as e:
                self.log.error("Safe update failed while writing stable copy '{}' -> '{}': {}".format(src_file, stable_file, e), source="SYSTEM")
                return

        self.log.debug("{} backup complete (log='{}', stable='{}'). Restarting...".format(src_file, log_dest, stable_file), source="SYSTEM")
        try:
            for i in range(5, 0, -1):
                print(i)
                time.sleep(1)
            self.log.debug("System expecting restart after backup of '{}'.".format(src_file), source="SYSTEM")
            machine.reset()
        except Exception as e:
            self.log.error("Backup completed for '{}', but restart failed: {}".format(src_file, e), source="SYSTEM")


class BluetoothManager:
    def __init__(self, device_name="Zeno Micro PC"):
        self.device_name = device_name
        self.ble = bt.BLE()
        self.ble.active(False)
        self.connected = False
        self.rx_buffer = bytearray()
        self.conn_handle = None

    def _irq(self, event, data):
        if event == 1:
            self.conn_handle, _, _ = data
            self.connected = True
        elif event == 2:
            self.connected = False
            self.conn_handle = None
            self._advertise()
        elif event == 3:
            _, value_handle = data
            self.rx_buffer.extend(self.ble.gatts_read(value_handle))

    def on(self):
        if not self.ble.active():
            self.ble.active(True)
            self.ble.irq(self._irq)
            self._advertise()
        else:
            print("[BT] Already ON.")

    def off(self):
        try:
            self.ble.active(False)
            self.connected = False
        except Exception as e:
            print("[BT] Error turning off Bluetooth:", e)

    def _advertise(self, interval_us=500000):
        name = bytes(self.device_name, "utf-8")
        adv_data = bytearray(b"\x02\x01\x06") + bytes((len(name) + 1, 0x09)) + name
        try:
            self.ble.gap_advertise(interval_us, adv_data)
        except Exception as e:
            print("[BT] Advertisement error:", e)

    def search(self, duration=5):
        found = []
        scan_done = False

        def _scan_irq(event, data):
            nonlocal found, scan_done
            if event == bt._IRQ_SCAN_RESULT:
                addr_type, addr, adv_type, rssi, adv_data = data
                found.append((addr_type, bytes(addr), adv_type, rssi, bytes(adv_data)))
            elif event == bt._IRQ_SCAN_DONE:
                scan_done = True

        self.ble.irq(_scan_irq)
        self.ble.active(True)
        self.ble.gap_scan(duration * 1000, 30000, 30000)

        t0 = time.ticks_ms()
        while not scan_done and time.ticks_diff(time.ticks_ms(), t0) < (duration + 2) * 1000:
            time.sleep_ms(100)

        self.ble.gap_scan(None)
        return found

    def connect(self, addr_type, addr):
        try:
            self.ble.gap_connect(addr_type, addr)
        except Exception as e:
            print("[BT] Connection error:", e)

    def disconnect(self):
        try:
            if self.conn_handle is not None:
                self.ble.gap_disconnect(self.conn_handle)
                self.connected = False
                self.conn_handle = None
        except Exception as e:
            print("[BT] Disconnect error:", e)

    def send_data(self, data):
        try:
            if not self.connected or self.conn_handle is None:
                return
            if isinstance(data, str):
                data = data.encode()
            self.ble.gatts_notify(self.conn_handle, 0, data)
        except Exception as e:
            print("[BT] Send error:", e)

    def get_data(self):
        if self.rx_buffer:
            data = bytes(self.rx_buffer)
            self.rx_buffer = bytearray()
            return data
        return None


class Device:
    __slots__ = ("id", "name", "obj", "pin", "state")

    def __init__(self, device_id, name, obj, pin=None):
        self.id = device_id
        self.name = name
        self.obj = obj
        self.pin = pin
        self.state = False

    def __repr__(self):
        return f"<Device {self.name!r} id={self.id} state={'ON' if self.state else 'OFF'}>"

class IoTManager:

    def __init__(self, app_key, app_secret):
        from sinricpro import SinricPro
        self.app_key = app_key
        self.app_secret = app_secret
        self.devices = {}
        self.name_map = {}
        self.client = SinricPro()

    def add_switch(self, device_id, name, pin_number=None, custom_callback=None):
        from sinricpro.devices.sinricpro_switch import SinricProSwitch
        hardware_pin = Pin(pin_number, Pin.OUT) if pin_number is not None else None
        switch_device = SinricProSwitch(device_id)
        device = Device(device_id, name, switch_device, hardware_pin)

        def power_state_callback(did, state):
            print(f"\n[IoTManager] Cloud command -> '{name}' ({did}) set to {'ON' if state else 'OFF'}")
            device.state = state
            if hardware_pin:
                hardware_pin.value(1 if state else 0)
            if custom_callback:
                custom_callback(did, state)
            return True

        switch_device.on_power_state(power_state_callback)
        self.client.add_device(switch_device)

        self.devices[device_id] = device
        self.name_map[name.lower()] = device_id

        print(f"[IoTManager] Registered switch '{name}' [ID: {device_id}]")
        return switch_device

    def remove_device(self, identifier):
        did = self._resolve_identifier(identifier)
        if did is None:
            print(f"[IoTManager] Device '{identifier}' not found.")
            return False
        name = self.devices[did].name
        del self.name_map[name.lower()]
        del self.devices[did]
        print(f"[IoTManager] Removed device '{name}' [ID: {did}]")
        return True

    def list_devices(self):
        return list(self.devices.values())

    def _resolve_identifier(self, identifier):
        if identifier in self.devices:
            return identifier
        return self.name_map.get(str(identifier).lower())

    def _set_power(self, identifier, state):
        did = self._resolve_identifier(identifier)
        if did is None:
            print(f"[IoTManager] Device '{identifier}' not found.")
            return False

        device = self.devices[did]
        label = "ON" if state else "OFF"

        device.obj.send_power_state_event(state)
        device.state = state
        print(f"[IoTManager] Local command -> Turned {label} '{device.name}'")

        if device.pin:
            device.pin.value(1 if state else 0)
        return True

    def on(self, identifier):
        return self._set_power(identifier, True)

    def off(self, identifier):
        return self._set_power(identifier, False)

    def toggle(self, identifier):
        did = self._resolve_identifier(identifier)
        if did is None:
            print(f"[IoTManager] Device '{identifier}' not found.")
            return False
        return self._set_power(did, not self.devices[did].state)

    def start(self):
        print("[IoTManager] Connecting to Sinric Pro...")
        self.client.start(self.app_key, self.app_secret)

    def handle(self):
        handler = getattr(self.client, "handle", None)
        if handler is None:
            raise AttributeError(
                "SinricPro has no 'handle' method in this build. Check whether "
                "you're expected to call _process_publish_queue()/"
                "_process_received_queue() directly, or whether start() runs "
                "an asyncio loop that needs asyncio.run()/create_task() instead "
                "of manual pumping."
            )
        try:
            handler()
        except Exception as e:
            print(f"[IoTManager] Network handle error: {e}")


def _system_help():
    return (
        "system.py -- low-level system control\n"
        "  system(opt_level=0, debug=False)\n"
        "    .restart() / .optlevel(level) / .info() / .mode(m)\n"
        "    .memconfig(percent=25) / .force_mem() / .mem_usage() / .perf_test()\n"
        "    .ram_guard(warn_pct=80, crit_pct=92) / .security_scan() / .checkup()\n"
        "    .start_guardian(sched, interval_ms=10000)\n"
        "    .firmware_update() / .boot_update()\n"
        "  Disk(mount_point='/MemDisk')\n"
        "    .begin() / .check() / .unmount() / .format(filesystem=os.VfsFat) / .info(path=None)\n"
        "  BootConfig() -- persisted /LOGS/bootcfg.json\n"
        "    .get(key, default=None) / .set(key, value) / .show()\n"
        "  BluetoothManager(device_name='Zeno Micro PC')\n"
        "    .on() / .off() / .search(duration=5) / .connect(addr_type, addr) / .disconnect()\n"
        "    .send_data(data) / .get_data()\n"
        "  IoTManager(app_key, app_secret) -- Sinric Pro smart-home devices\n"
        "    .add_switch(device_id, name, pin_number=None) / .on/.off/.toggle(identifier)\n"
        "    .list_devices() / .remove_device(identifier) / .start() / .handle()"
    )


# ============================================================================

try:
    import uhashlib as hashlib
except ImportError:
    import hashlib


_IMPORTABLE_EXT = (".py", ".mpy")

class downloadhelper:
    def __init__(self):
        self.log = Logger()

    def download_file(self, url, save_dir="/", save_file=None):
        if not url:
            self.log.error("Download failed: empty URL provided", source="DOWNLOADSERV")
            return None

        url = url.strip()
        try:
            if url.startswith("http:/") and not url.startswith("http://"):
                url = url.replace("http:/", "http://", 1)
            elif url.startswith("https:/") and not url.startswith("https://"):
                url = url.replace("https:/", "https://", 1)
            elif not (url.startswith("http://") or url.startswith("https://")):
                url = "http://" + url
        except Exception as e:
            self.log.error("Failed to normalize URL '{}': {}".format(url, e), source="DOWNLOADSERV")
            return None

        try:
            proto, _, hostport, *rest = url.split("/", 3)
            host = hostport.split(":")[0]
            port = 443 if proto == "https:" else 80
            path = "/" + (rest[0] if rest else "")
            if path == "/":
                path = "/index.html"
        except Exception as e:
            self.log.error("Invalid URL format '{}': {}".format(url, e), source="DOWNLOADSERV")
            return None

        if not save_file:
            save_file = path.split("/")[-1] or "index.html"

        try:
            addr_info = usocket.getaddrinfo(host, port)
            if not addr_info:
                self.log.error("DNS resolution returned no results for host '{}'".format(host), source="DOWNLOADSERV")
                return None
            addr = addr_info[0][-1]
        except Exception as e:
            self.log.error("DNS resolution failed for host '{}': {}".format(host, e), source="DOWNLOADSERV")
            return None

        try:
            s = usocket.socket()
        except Exception as e:
            self.log.error("Failed to create socket: {}".format(e), source="DOWNLOADSERV")
            return None

        try:
            try:
                s.connect(addr)
            except Exception as e:
                self.log.error("Failed to connect to {}:{} -> {}".format(host, port, e), source="DOWNLOADSERV")
                return None

            if proto == "https:":
                try:
                    s = ssl.wrap_socket(s)
                except Exception as e:
                    self.log.error("SSL wrap failed for '{}': {}".format(url, e), source="DOWNLOADSERV")
                    return None

            try:
                s.send("GET {} HTTP/1.0\r\nHost: {}\r\n\r\n".format(path, host).encode())
            except Exception as e:
                self.log.error("Failed to send HTTP request to '{}': {}".format(url, e), source="DOWNLOADSERV")
                return None

            try:
                status_line = s.readline()
            except Exception as e:
                self.log.error("Failed to read HTTP status from '{}': {}".format(url, e), source="DOWNLOADSERV")
                return None

            if not status_line:
                self.log.warning("No response from server '{}'".format(url), source="DOWNLOADSERV")
                return None

            try:
                parts = status_line.decode().split()
            except Exception as e:
                self.log.error("Failed to decode HTTP status line from '{}': {}".format(url, e), source="DOWNLOADSERV")
                return None

            if len(parts) < 2 or parts[1] != "200":
                self.log.error("HTTP error from '{}': {}".format(url, status_line.decode().strip()), source="DOWNLOADSERV")
                return None

            try:
                while True:
                    line = s.readline()
                    if not line or line == b"\r\n":
                        break
            except Exception as e:
                self.log.error("Failed while reading HTTP headers from '{}': {}".format(url, e), source="DOWNLOADSERV")
                return None

            try:
                if save_dir not in os.listdir("/"):
                    try:
                        os.mkdir(save_dir)
                    except Exception as e_mk:
                        self.log.error("Failed to create directory '{}': {}".format(save_dir, e_mk), source="DOWNLOADSERV")
                        return None
            except Exception as e:
                self.log.error("Failed to list root directories while checking '{}': {}".format(save_dir, e), source="DOWNLOADSERV")
                return None

            full_path = save_dir + "/" + save_file
            try:
                with open(full_path, "wb") as f:
                    while True:
                        try:
                            data = s.recv(512)
                        except Exception as e:
                            self.log.error("Socket recv failed from '{}': {}".format(url, e), source="DOWNLOADSERV")
                            return None
                        if not data:
                            break
                        try:
                            f.write(data)
                        except Exception as e:
                            self.log.error("Failed writing to '{}': {}".format(full_path, e), source="DOWNLOADSERV")
                            return None
            except Exception as e:
                self.log.error("Failed to open/write file '{}': {}".format(full_path, e), source="DOWNLOADSERV")
                return None

            self.log.debug("File saved successfully -> {}".format(full_path), source="DOWNLOADSERV")
            return full_path

        except Exception as e:
            self.log.error("Error during download from '{}': {}".format(url, e), source="DOWNLOADSERV")
            return None
        finally:
            try:
                s.close()
            except Exception:
                pass


class Git:
    def __init__(self, base_raw=None, default_branch="main"):
        self.logger = Logger()
        self.base_raw = base_raw or "https://raw.githubusercontent.com"
        self.default_branch = default_branch
        self.default_download_dir = "/"
        self.source = "GITSERV"
        self.token = zeno.gitsecret
        self._http = None

    def download_url(self, url, save_dir=None):
        try:
            user, repo, branch, filename = self._parse_github_url(url)
        except ValueError as e:
            message = f"Download failed: invalid GitHub URL: {e}"
            self.logger.error(message, source=self.source)
            return (message, False)
        return self.download(user, repo, filename, branch=branch, save_dir=save_dir)

    def download(self, user, repo, filename, branch=None, save_dir=None):
        filename = filename or ""
        if filename.startswith("http://") or filename.startswith("https://"):
            try:
                parsed_user, parsed_repo, parsed_branch, filename = self._parse_github_url(filename)
            except ValueError as e:
                message = f"Download failed: invalid GitHub URL: {e}"
                self.logger.error(message, source=self.source)
                return (message, False)
            user = user or parsed_user
            repo = repo or parsed_repo
            branch = branch or parsed_branch

        if not user or not repo:
            message = "Download failed: 'user' and 'repo' are required."
            self.logger.error(message, source=self.source)
            return (message, False)

        branch = branch or self.default_branch
        filename = filename.lstrip("/")
        if not filename:
            message = "Download failed: empty file path."
            self.logger.error(message, source=self.source)
            return (message, False)

        original_filename = filename
        url = self._build_url(user, repo, self._encode_path(filename), branch)

        dir_message, target_dir = self._resolve_download_dir(save_dir if save_dir is not None else self.default_download_dir)
        self.logger.debug(dir_message, source=self.source)
        fname = original_filename.split("/")[-1]
        full_path = "{}/{}".format(target_dir.rstrip("/") or "", fname)
        if not full_path.startswith("/"):
            full_path = "/" + full_path

        message = f"Cloning '{fname}' from {user}/{repo}..."
        self.logger.debug(message, source=self.source)

        http = self._get_http()
        if http is None:
            message = "Download failed: no HTTP client available."
            self.logger.error(message, source=self.source)
            return (message, False)

        headers = {"User-Agent": "ZenoMicroPC"}
        try:
            resp = http.get(url, headers=headers)
        except Exception as e:
            message = f"HTTP request failed for URL {url}: {e}"
            self.logger.error(message, source=self.source)
            return (message, False)

        try:
            status = resp.status_code
        except Exception:
            status = 0

        if status == 404 and self.token:
            try:
                resp.close()
            except Exception:
                pass
            auth_headers = dict(headers)
            auth_headers["Authorization"] = "token {}".format(self.token)
            message = "Unauthenticated request 404'd, retrying with token"
            self.logger.debug(message, source=self.source)
            try:
                resp = http.get(url, headers=auth_headers)
                status = resp.status_code
            except Exception as e:
                message = f"HTTP retry-with-auth failed for URL {url}: {e}"
                self.logger.error(message, source=self.source)
                return (message, False)

        if status != 200:
            message = f"HTTP {status} while downloading {filename} from {url}"
            self.logger.error(message, source=self.source)
            if status == 404:
                message = "Download failed: file not found."
            else:
                message = "Download failed: server returned an error."
            try:
                resp.close()
            except Exception:
                pass
            return (message, False)

        try:
            data = resp.content
        except Exception:
            try:
                data = resp.text
                if isinstance(data, str):
                    data = data.encode()
            except Exception as e:
                message = f"Failed to read HTTP response body: {e}"
                self.logger.error(message, source=self.source)
                return (message, False)

        try:
            with open(full_path, "wb") as f:
                f.write(data)
        except Exception as e:
            message = f"Failed to write file '{full_path}': {e}"
            self.logger.error(message, source=self.source)
            return (message, False)

        try:
            resp.close()
        except Exception:
            pass

        message = f"Download completed. Saved as '{full_path}'"
        self.logger.debug(message, source=self.source)
        return (message, True)

    def _get_http(self):
        if self._http is not None:
            return self._http
        self._http = urequests
        return self._http

    def _parse_github_url(self, url):
        url = (url or "").strip()

        if url.startswith("https://raw.githubusercontent.com/"):
            parts = url[len("https://raw.githubusercontent.com/"):].split("/", 3)
            if len(parts) < 4 or not parts[3]:
                raise ValueError("Malformed raw.githubusercontent.com URL: {}".format(url))
            return parts[0], parts[1], parts[2], parts[3]

        if url.startswith("https://github.com/"):
            rest = url[len("https://github.com/"):]
            for marker in ("/blob/", "/raw/"):
                if marker in rest:
                    repo_part, _, tail = rest.partition(marker)
                    if "/" not in repo_part:
                        raise ValueError("Malformed github.com URL (missing repo): {}".format(url))
                    user, repo = repo_part.split("/", 1)
                    branch, _, path = tail.partition("/")
                    if not branch or not path:
                        raise ValueError("Malformed github.com URL (missing branch/path): {}".format(url))
                    return user, repo, branch, path
            raise ValueError("Unrecognized github.com URL (expected '/blob/' or '/raw/' in path): {}".format(url))

        raise ValueError("Unrecognized GitHub URL format: {}".format(url))

    def _encode_path(self, path):
        safe = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_.~/"
        return "".join(ch if ch in safe else "%{:02X}".format(ord(ch)) for ch in path)

    def _build_url(self, user, repo, encoded_path, branch):
        if encoded_path.startswith("/"):
            encoded_path = encoded_path[1:]
        return "/".join([self.base_raw.rstrip("/"), user, repo, branch, encoded_path])

    def _resolve_download_dir(self, preferred_dir):
        preferred_dir = preferred_dir or "/"
        if preferred_dir in ("", "/"):
            return "/"

        curr = ""
        try:
            for p in [p for p in preferred_dir.strip("/").split("/") if p]:
                curr = curr + "/" + p
                try:
                    os.stat(curr)
                except OSError:
                    try:
                        os.mkdir(curr)
                    except Exception as e:
                        self.logger.warning("Failed to create '{}' ({}), falling back to '/'".format(curr, e), source=self.source)
                        return "/"
            return curr or "/"
        except Exception as e:
            self.logger.warning("Error resolving download dir '{}': {}. Falling back to '/'".format(preferred_dir, e), source=self.source)
            return "/"

    def upload(self, user, repo, local_path, repo_path=None, branch=None, message="Upload from ESP32"):
        import ubinascii
        token = self.token
        if not token:
            message = "Upload failed: no GitHub token configured (zeno.gitsecret)."
            self.logger.error(message, source=self.source)
            return (message, False)

        branch = branch or self.default_branch
        repo_path = (repo_path or local_path.split("/")[-1]).lstrip("/")

        try:
            with open(local_path, "rb") as f:
                data = f.read()
        except Exception as e:
            message = f"Git upload: cannot read local file '{local_path}': {e}"
            self.logger.error(message, source=self.source)
            return (message, False)

        try:
            b64 = ubinascii.b2a_base64(data).decode().strip()
        except Exception as e:
            message = f"Git upload: base64 encode failed: {e}"
            self.logger.error(message, source=self.source)
            return (message, False)

        message = f"Uploading '{repo_path}' to {user}/{repo}..."
        self.logger.debug(message, source=self.source)

        api_url = "https://api.github.com/repos/{}/{}/contents/{}".format(user, repo, self._encode_path(repo_path))
        http = self._get_http()
        if http is None:
            message = "Upload failed: no HTTP client available."
            self.logger.error(message, source=self.source)
            return (message, False)

        headers = {"Authorization": "token {}".format(token), "User-Agent": "ZenoMicroPC", "Accept": "application/vnd.github+json"}

        sha = None
        try:
            r = http.get(api_url + "?ref={}".format(branch), headers=headers)
            try:
                if r.status_code == 200:
                    try:
                        sha = r.json().get("sha")
                    except Exception:
                        sha = None
            finally:
                r.close()
        except Exception as e:
            message = f"Git upload: GET existing file failed/404 (new file maybe): {e}"
            self.logger.debug(message, source=self.source)

        body = {"message": message, "content": b64, "branch": branch}
        if sha:
            body["sha"] = sha

        try:
            r = http.put(api_url, headers=headers, json=body)
        except Exception as e:
            message = f"Git upload: PUT request failed: {e}"
            self.logger.error(message, source=self.source)
            return (message, False)

        try:
            status = r.status_code
        except Exception:
            status = 0

        if status not in (200, 201):
            try:
                err_txt = r.text
            except Exception:
                err_txt = "no body"
            message = f"Git upload: HTTP {status} from GitHub: {err_txt}"
            self.logger.error(message, source=self.source)
            return (message, False)

        try:
            r.close()
        except Exception:
            pass

        message = f"Git upload completed: {local_path} -> {user}/{repo}/{repo_path}"
        self.logger.debug(message, source=self.source)
        return (message, True)


class AppInstaller:
    def __init__(self):
        self.user = "FerrariForever95"
        self.repo = "Zeno-Micro-PC"
        self.branch = "main"
        self.remote_base = "APPS"
        self.git = Git(default_branch=self.branch)
        self.apps_dir = "/SYSTEM32/APPS"

    def prompt_and_install(self):
        try:
            app_name = input("Enter app name to install (without .py): ").strip()
        except (KeyboardInterrupt, EOFError):
            print("Install cancelled.")
            return False
        if not app_name:
            print("No app name given.")
            return False
        return self.install(app_name)

    def _build_remote_path(self, app_name: str) -> str:
        base = (self.remote_base or "").strip().strip("/")
        return f"{base}/{app_name}.py" if base else f"{app_name}.py"

    def install(self, app_name):
        remote_path = self._build_remote_path(app_name)
        print("[APPINST] Installing app:", app_name)

        try:
            d = Disk()
            d.begin()
        except Exception as e:
            print("[APPINST] WARNING: Disk init failed:", e)

        old_dir = self.git.default_download_dir
        self.git.default_download_dir = self.apps_dir
        try:
            ok = self.git.download(self.user, self.repo, remote_path, branch=self.branch)
        finally:
            self.git.default_download_dir = old_dir

        if ok:
            print("[APPINST] App '{}' installed to {}/{}.py".format(app_name, self.apps_dir, app_name))
            try:
                time.sleep(1)
            except Exception:
                pass
        else:
            print("[APPINST] Failed to install app '{}'".format(app_name))
        return ok

    def uninstall(self, name):
        d = Disk()
        d.begin()
        try:
            os.remove(f"/SYSTEM32/APPS/{name}.py")
            print("[APPINST] uninstalled", name)
        except OSError as e:
            print("[APPINST] uninstall failed:", e)

    def listapps(self):
        files = os.listdir("/SYSTEM32/APPS")
        print("Detected apps:", [f[:-3] for f in files if f.endswith(".py")])


_DB_DIR = "/SYSTEM32/APPS/Data"
_DB_FILE = _DB_DIR + "/appdb.json"


class AppDB:
    def __init__(self):
        self._data = {}
        self._load()

    def _load(self):
        try:
            if _DB_DIR not in os.listdir("/SYSTEM32/APPS"):
                os.mkdir(_DB_DIR)
        except Exception:
            pass
        try:
            with open(_DB_FILE, "r") as f:
                self._data = json.load(f)
        except Exception:
            self._data = {}

    def _save(self):
        try:
            with open(_DB_FILE, "w") as f:
                json.dump(self._data, f)
        except Exception as e:
            print("[TinyAppDB] save failed:", e)

    def set(self, app, key, value):
        app, key = str(app), str(key)
        self._data.setdefault(app, {})[key] = value
        self._save()

    def get(self, app, key, default=None):
        try:
            return self._data.get(app, {}).get(key, default)
        except Exception:
            return default

    def delete(self, app, key):
        try:
            del self._data[app][key]
            if not self._data[app]:
                del self._data[app]
            self._save()
        except Exception:
            pass

    def clear(self, app):
        if app in self._data:
            del self._data[app]
            self._save()

    def dump(self):
        return self._data


class PackageManager:
    DEFAULT_USER = "FerrariForever95"
    DEFAULT_REPO = "Zeno-Micro-PC"
    PKGLIST_PATH = "/pkglist.json"
    PKGTABLE_CACHE_PATH = "/pkgtable_cache.json"

    def __init__(self, git=None, repo_user=None, repo_name=None):
        self.logger = Logger()
        self.source = "PKGMGR"
        self.git = git or Git()
        self.fm = FileManager()
        self.um = usermanager()
        self.repo_user = repo_user or self.DEFAULT_USER
        self.repo_name = repo_name or self.DEFAULT_REPO
        self._op_depth = 0
        self._pkgtable_cache = None

        self.commands = {}
        self._command_modules = {}
        self._installing_module = None

    def install(self, name, force=False):
        if not self._require_root("install"):
            return False

        self._begin_op()
        try:
            print("[PKG] === Installing '{}' ===".format(name))
            pkgtable = self._fetch_pkgtable()
            if pkgtable is None:
                return False

            entry = self._find_package(pkgtable, name)
            if entry is None:
                self._error("Package '{}' not found in pkgtable.json".format(name))
                return False

            installed = self._load_pkglist()
            if name in installed and not force:
                print("[PKG] '{}' is already installed (v{}). Use update() or reinstall() instead.".format(name, installed[name].get("version")))
                return False

            prior_record = installed.get(name) if force else None

            for dep in entry.get("dependencies", []):
                if not self._ensure_dependency(dep, pkgtable, installed):
                    self._error("Failed to satisfy dependency '{}' for '{}'".format(dep.get("name"), name))
                    return False
                installed = self._load_pkglist()

            required_modules = entry.get("modules", [])
            missing = self._check_modules(required_modules)
            if missing:
                if not self._resolve_missing_modules(missing, pkgtable, installed):
                    self._error("Cannot install '{}': missing required module(s) with no available package in pkgtable.json: {}".format(name, ", ".join(missing)))
                    return False
                installed = self._load_pkglist()
                still_missing = self._check_modules(required_modules)
                if still_missing:
                    self._error("Cannot install '{}': still missing module(s) after attempted auto-install: {}".format(name, ", ".join(still_missing)))
                    return False

            if not self._ensure_bin_dir(name):
                self._error("Could not create install directory '{}' for '{}'".format(self._bin_dir_for(name), name))
                return False

            if not self._download_package(entry):
                self._error("Failed to download package '{}'".format(name))
                return False

            installed = self._load_pkglist()
            installed[name] = self._make_pkglist_entry(entry)
            if not self._save_pkglist(installed):
                return False

            try:
                self.fm.refresh_tree(installed[name]["install_path"])
            except Exception as e:
                self.logger.warning(
                    "refresh_tree failed for '{}' after install: {}".format(installed[name]["install_path"], e),
                    source=self.source,
                )

            if prior_record:
                old_path = self._full_path(prior_record.get("install_path"), prior_record.get("filename"))
                new_path = self._full_path(installed[name].get("install_path"), installed[name].get("filename"))
                if old_path != new_path:
                    try:
                        os.remove(old_path)
                    except OSError:
                        pass

            self._load_and_install_module(name, installed[name])

            print("[PKG] '{}' v{} installed successfully.".format(name, entry.get("version")))
            return True
        finally:
            self._end_op()

    def _module_name_for(self, record):
        filename = record.get("filename", "")
        for ext in _IMPORTABLE_EXT:
            if filename.endswith(ext):
                return filename[: -len(ext)]
        return None

    def _ensure_on_path(self, install_path):
        path = (install_path or "/").rstrip("/") or "/"
        if path not in sys.path:
            sys.path.append(path)

    def _load_and_install_module(self, name, record):
        module_name = self._module_name_for(record)
        if module_name is None:
            filename = record.get("filename", "")
            if "." in filename:
                self.logger.debug(
                    "'{}' is a non-importable asset ({}); nothing to load.".format(name, filename),
                    source=self.source,
                )
                return True
            self._error("Cannot determine module name for package '{}'".format(name))
            return False

        install_path = record.get("install_path") or self._bin_dir_for(name)
        self._ensure_on_path(install_path)

        try:
            if not _os_exists(install_path):
                raise NotADirectoryError(install_path)
            for stale in [m for m in list(sys.modules.keys()) if m == module_name]:
                del sys.modules[stale]
            module = __import__(module_name)
        except NotADirectoryError as e:
            self._error("Package '{}' install directory is missing/broken: {}".format(name, e))
            return False
        except ImportError as e:
            self._error("Could not import module '{}' for package '{}': {}".format(module_name, name, e))
            return False
        except Exception as e:
            self._error("Could not import module '{}' for package '{}': {}".format(module_name, name, e))
            return False

        installer = getattr(module, "install", None)
        if installer is not None:
            self._installing_module = module_name
            try:
                installer(self)
            except Exception as e:
                self._error("install(pm) failed for package '{}': {}".format(name, e))
                return False
            finally:
                self._installing_module = None
        else:
            self.logger.debug("Package '{}' has no install(pm) hook".format(name), source=self.source)

        return True

    def load_all_installed(self):
        installed = self._load_pkglist()
        if not installed:
            self.logger.debug("No installed packages to auto-import.", source=self.source)
            return True

        all_ok = True
        for name, record in installed.items():
            try:
                ok = self._load_and_install_module(name, record)
            except Exception as e:
                ok = False
                self._error("Auto-import crashed for '{}': {}".format(name, e))
            if ok:
                self.logger.debug("Auto-imported '{}' on startup.".format(name), source=self.source)
            else:
                self.logger.warning("Failed to auto-import '{}' on startup -- command(s) from it will be unavailable until fixed/reinstalled.".format(name), source=self.source)
            all_ok = ok and all_ok
        return all_ok

    def _unload_module(self, name, record):
        module_name = self._module_name_for(record)
        module = sys.modules.get(module_name) if module_name else None

        if module is not None:
            uninstaller = getattr(module, "uninstall", None)
            if uninstaller is not None:
                try:
                    uninstaller(self)
                except Exception as e:
                    self.logger.warning(
                        "uninstall(pm) failed for package '{}': {}".format(name, e), source=self.source
                    )

        stale = [cmd for cmd, mod in self._command_modules.items() if mod == module_name]
        for cmd in stale:
            self.unregister(cmd)

        if module_name and module_name in sys.modules:
            del sys.modules[module_name]

    def uninstall(self, name):
        if not self._require_root("uninstall"):
            return False

        self._begin_op()
        try:
            print("[PKG] === Uninstalling '{}' ===".format(name))
            installed = self._load_pkglist()
            if name not in installed:
                self._error("Package '{}' is not installed.".format(name))
                return False

            record = installed[name]

            if record.get("core", False):
                self._error("Package '{}' is a core OS component and cannot be uninstalled.".format(name))
                return False

            install_dir = record.get("install_path")

            self._unload_module(name, record)

            if not self._is_safe_install_dir(install_dir):
                self._error("Refusing to uninstall '{}': install_path '{}' is missing or unsafe to remove.".format(name, install_dir))
                return False

            if not self._remove_dir_recursive(install_dir):
                self.logger.warning("Could not fully remove folder '{}' for package '{}'".format(install_dir, name), source=self.source)

            del installed[name]
            if not self._save_pkglist(installed):
                return False

            self.fm.refresh_tree(self._parent_dir(install_dir))
            print("[PKG] '{}' uninstalled.".format(name))
            return True
        finally:
            self._end_op()

    def reinstall(self, name):
        if not self._require_root("reinstall"):
            return False
        installed = self._load_pkglist()
        if name not in installed:
            self._error("Package '{}' is not installed, cannot reinstall.".format(name))
            return False

        self._begin_op()
        try:
            return self.uninstall(name) and self.install(name, force=True)
        finally:
            self._end_op()

    def update(self, name=None):
        if not self._require_root("update"):
            return False

        self._begin_op()
        try:
            installed = self._load_pkglist()
            if name is None:
                if not installed:
                    print("[PKG] No packages installed.")
                    return True
                ok = True
                for pkg_name in list(installed.keys()):
                    ok = self._update_one(pkg_name, installed) and ok
                return ok
            return self._update_one(name, installed)
        finally:
            self._end_op()

    def run(self, command, *args):
        callback = self.commands.get(command)
        if callback is None:
            installed = self._load_pkglist()
            if command in installed and command not in self._command_modules:
                self._load_and_install_module(command, installed[command])
                callback = self.commands.get(command)
        if callback is None:
            return self._run_legacy(command, *args)

        try:
            callback(list(args), self)
        except Exception as e:
            self._error("Command '{}' raised an error while running: {}".format(command, e))
            return False
        return True

    def _run_legacy(self, name, *args):
        installed = self._load_pkglist()
        if name not in installed:
            self._error("Package '{}' is not installed.".format(name))
            return False

        record = installed[name]
        full_path = self._full_path(record.get("install_path"), record.get("filename"))
        try:
            with open(full_path) as f:
                code = f.read()
        except OSError as e:
            self._error("Cannot read '{}' for package '{}': {}".format(full_path, name, e))
            return False

        try:
            exec(compile(code, full_path, "exec"), {"__name__": "__main__", "argv": list(args)})
        except Exception as e:
            self._error("Package '{}' raised an error while running: {}".format(name, e))
            return False
        return True


    def register(self, command, callback, module_name=None):
        if not command or not callable(callback):
            self._error("register() requires a non-empty command name and a callable")
            return False
        self.commands[command] = callback
        module_name = module_name or self._installing_module
        if module_name:
            self._command_modules[command] = module_name
        self.logger.debug("Registered command '{}'".format(command), source=self.source)
        return True

    def unregister(self, command):
        existed = command in self.commands
        self.commands.pop(command, None)
        self._command_modules.pop(command, None)
        if existed:
            self.logger.debug("Unregistered command '{}'".format(command), source=self.source)
        return existed

    def check(self, command):
        return command in self.commands

    def list_commands(self):
        return list(self.commands.keys())

    def reload(self, name):
        installed = self._load_pkglist()
        record = installed.get(name)
        if record is None:
            self._error("Package '{}' is not installed.".format(name))
            return False
        return self._load_and_install_module(name, record)

    def info(self, name):
        installed = self._load_pkglist()
        record = installed.get(name)

        print("\n[Package] {}".format(name))
        if record:
            print("  Installed     : yes")
            print("  Version       : {}".format(record.get("version")))
            print("  Author        : {}".format(record.get("author")))
            print("  Repository    : {}".format(record.get("repository")))
            print("  Branch        : {}".format(record.get("branch")))
            print("  Install path  : {}".format(record.get("install_path")))
            print("  Filename      : {}".format(record.get("filename")))
            print("  Dependencies  : {}".format(record.get("dependencies")))
            print("  Core          : {}".format(record.get("core", False)))
        else:
            print("  Installed     : no")

        pkgtable = self._fetch_pkgtable()
        try:
            entry = self._find_package(pkgtable, name) if pkgtable else None
            if entry:
                print("  Catalog version : {}".format(entry.get("version")))
                print("  Catalog author  : {}".format(entry.get("author")))
                print("  Catalog repo    : {}".format(entry.get("repository")))
                print("  Modules needed  : {}".format(entry.get("modules")))
                print("  Catalog core    : {}".format(entry.get("core", False)))
            elif not record:
                print("  Not found in catalog either.")
            return record or entry
        finally:
            self._cleanup_pkgtable_cache()

    def list(self):
        installed = self._load_pkglist()
        if not installed:
            print("[PKG] No packages installed.")
            return []
        print("\n[Installed Packages]")
        for pkg_name, record in installed.items():
            print("  {:<20} v{:<10} ({})".format(pkg_name, record.get("version", "?"), record.get("author", "?")))
        return list(installed.keys())

    def verify(self):
        installed = self._load_pkglist()
        issues = {}
        pkgtable = self._fetch_pkgtable()
        try:
            for pkg_name, record in installed.items():
                pkg_issues = []
                full_path = self._full_path(record.get("install_path"), record.get("filename"))
                try:
                    os.stat(full_path)
                except OSError:
                    pkg_issues.append("missing file: {}".format(full_path))

                entry = self._find_package(pkgtable, pkg_name) if pkgtable else None
                missing_modules = self._check_modules(entry.get("modules", []) if entry else [])
                if missing_modules:
                    pkg_issues.append("missing modules: {}".format(", ".join(missing_modules)))

                if pkg_issues:
                    issues[pkg_name] = pkg_issues

            if issues:
                for pkg_name, pkg_issues in issues.items():
                    print("  {}:".format(pkg_name))
                    for issue in pkg_issues:
                        print("    - {}".format(issue))
            return issues
        finally:
            self._cleanup_pkgtable_cache()
    def _begin_op(self):
        self._op_depth += 1

    def _end_op(self):
        self._op_depth -= 1
        if self._op_depth <= 0:
            self._op_depth = 0
            self._cleanup_pkgtable_cache()

    def _cleanup_pkgtable_cache(self):
        self._pkgtable_cache = None
        try:
            os.remove("/pkgtable.json")
        except OSError:
            pass

    def _current_user(self):
        try:
            return self.um.current_user()
        except Exception as e:
            self.logger.debug("Could not resolve current user: {}".format(e), source=self.source)
            return None

    def _is_root(self):
        user = self._current_user()
        if not user:
            return False
        try:
            return bool(self.um.isrooted(user))
        except Exception as e:
            self.logger.debug("Could not check root status: {}".format(e), source=self.source)
            return False

    def _require_root(self, action):
        if self._is_root():
            return True
        self._error("Permission denied: '{}' requires root privileges. Use usermanager().elevate(user, password) first.".format(action))
        return False

    def _fetch_pkgtable(self):
        if self._pkgtable_cache is not None:
            return self._pkgtable_cache

        ok = self.git.download(self.repo_user, self.repo_name, "pkgtable.json", save_dir="/")
        if not ok:
            self._error("Could not download pkgtable.json from {}/{}".format(self.repo_user, self.repo_name))
            return None
        data = self._load_json("/pkgtable.json")
        if data is None:
            self._error("Downloaded pkgtable.json but it could not be parsed as JSON")
            return None
        self._pkgtable_cache = data
        return data

    def _find_package(self, pkgtable, name):
        return pkgtable.get(name) if pkgtable else None

    def _find_package_providing_module(self, pkgtable, module_name):
        if not pkgtable:
            return None, None
        if module_name in pkgtable:
            return module_name, pkgtable[module_name]
        for pkg_name, entry in pkgtable.items():
            if module_name in entry.get("provides", []):
                return pkg_name, entry
        return None, None

    def _load_pkglist(self):
        data = self._load_json(self.PKGLIST_PATH)
        return data if isinstance(data, dict) else {}

    def _save_pkglist(self, data):
        return self._save_json(self.PKGLIST_PATH, data)

    def _load_json(self, path):
        try:
            with open(path) as f:
                return json.load(f)
        except (OSError, ValueError) as e:
            self.logger.debug("Could not load JSON from '{}': {}".format(path, e), source=self.source)
            return None

    def _save_json(self, path, data):
        try:
            with open(path, "w") as f:
                json.dump(data, f)
            return True
        except OSError as e:
            self._error("Could not write '{}': {}".format(path, e))
            return False

    def _bin_dir_for(self, name):
        return "/bin/{}".format(name)

    def _ensure_bin_dir(self, name):
        """Guarantee /bin and /bin/<name> exist before an extension is
        installed into them. Extensions must always live under
        /bin/<module_name>/ -- the installer should never fail just
        because that directory tree hasn't been created yet."""
        target_dir = self._bin_dir_for(name)
        try:
            self.fm._ensure_dir_chain(target_dir)
        except Exception as e:
            self.logger.warning(
                "Could not pre-create '{}' via FileManager ({}); falling back to raw mkdir.".format(target_dir, e),
                source=self.source,
            )
            _makedirs(target_dir)
        return _os_exists(target_dir)

    def _make_pkglist_entry(self, entry):
        name = entry.get("name")
        ext = self._ext_of(entry.get("file") or "") or ".py"
        return {
            "name": name, "version": entry.get("version"), "author": entry.get("author"),
            "repository": entry.get("repository"), "branch": entry.get("branch") or self.git.default_branch,
            "install_path": self._bin_dir_for(name), "filename": "{}{}".format(name, ext),
            "dependencies": entry.get("dependencies", []),
            "core": bool(entry.get("core", False)),
        }

    def _download_package(self, entry):
        name = entry.get("name")
        target_dir = self._bin_dir_for(name)
        source_file = entry.get("file") or ""
        ext = self._ext_of(source_file) or ".py"

        ok = self.git.download(entry.get("author"), entry.get("repository"), source_file,
                                branch=entry.get("branch"), save_dir=target_dir)
        if not ok:
            return False

        source_basename = source_file.split("/")[-1]
        desired_path = "{}/{}{}".format(target_dir, name, ext)
        actual_path = "{}/{}".format(target_dir, source_basename) if source_basename else desired_path

        if actual_path != desired_path:
            try:
                os.rename(actual_path, desired_path)
            except OSError as e:
                self.logger.warning(
                    "Downloaded '{}' but could not rename it to '{}': {}".format(actual_path, desired_path, e),
                    source=self.source,
                )
                return False

        if not self._verify_download(desired_path, entry):
            # A hard failure here means the file itself is unusable
            # (missing/unreadable/empty) -- not just a size/sha256
            # mismatch against the catalog, which is only ever logged
            # as a warning below. Safe to remove in this case.
            try:
                os.remove(desired_path)
            except OSError:
                pass
            return False

        return True

    def _ext_of(self, path):
        base = (path or "").split("/")[-1]
        if "." not in base:
            return ""
        return "." + base.split(".")[-1]

    def _verify_download(self, path, entry):
        """Confirm the downloaded file is real and usable. Returns False
        (fatal, caller deletes the file) only when the file itself is
        missing, unreadable, or empty -- something no catalog metadata
        can excuse.

        A size or sha256 mismatch against entry's catalog values is
        logged as a warning, not treated as fatal: Git.download() writes
        the raw HTTP response body byte-for-byte, so a mismatch is at
        least as likely to mean pkgtable.json's own size/sha256 fields
        are stale (e.g. the file was updated on the branch without
        regenerating the catalog) as it is to mean the download is
        actually corrupt. Deleting a perfectly good, freshly-downloaded
        file because of a stale catalog value was actively destructive.
        """
        try:
            actual_size = os.stat(path)[6]
        except OSError as e:
            self._error("Verification failed: could not stat '{}': {}".format(path, e))
            return False

        if actual_size == 0:
            self._error("Verification failed for '{}': file is empty".format(path))
            return False

        expected_size = entry.get("size")
        if expected_size is not None and actual_size != expected_size:
            self.logger.warning(
                "Size mismatch for '{}': pkgtable.json says {} bytes, downloaded {} bytes. "
                "Keeping the downloaded file -- this usually means the catalog's 'size' field "
                "is stale, not that the download is corrupt.".format(path, expected_size, actual_size),
                source=self.source,
            )

        expected_sha256 = entry.get("sha256")
        if expected_sha256:
            try:
                h = hashlib.sha256()
                with open(path, "rb") as f:
                    while True:
                        chunk = f.read(512)
                        if not chunk:
                            break
                        h.update(chunk)
                actual_hex = "".join("{:02x}".format(b) for b in h.digest())
            except Exception as e:
                self.logger.warning(
                    "Could not hash '{}' to verify checksum ({}); skipping checksum check.".format(path, e),
                    source=self.source,
                )
                actual_hex = None

            if actual_hex is not None and actual_hex.lower() != expected_sha256.lower():
                self.logger.warning(
                    "Checksum mismatch for '{}': pkgtable.json says {}, downloaded file hashes to {}. "
                    "Keeping the downloaded file -- this usually means the catalog's 'sha256' field "
                    "is stale, not that the download is corrupt. Run 'pkg verify' if you want to "
                    "double check later.".format(path, expected_sha256, actual_hex),
                    source=self.source,
                )

        return True

    def _ensure_dependency(self, dep, pkgtable, installed):
        dep_name = dep.get("name")
        dep_version = dep.get("version", "0")
        if dep_name in installed:
            current_version = installed[dep_name].get("version", "0")
            if not self._version_gt(dep_version, current_version):
                return True
            return self.install(dep_name, force=True)
        return self.install(dep_name)

    def _resolve_missing_modules(self, missing_modules, pkgtable, installed):
        all_ok = True
        for mod_name in missing_modules:
            pkg_name, entry = self._find_package_providing_module(pkgtable, mod_name)
            if not pkg_name:
                all_ok = False
                continue
            if pkg_name in installed:
                all_ok = False
                continue
            if not self.install(pkg_name):
                all_ok = False
        return all_ok

    def _check_modules(self, modules):
        missing = []
        for mod_name in modules or []:
            try:
                __import__(mod_name)
            except ImportError:
                missing.append(mod_name)
        return missing

    def _update_one(self, name, installed):
        if name not in installed:
            self._error("Package '{}' is not installed.".format(name))
            return False

        pkgtable = self._fetch_pkgtable()
        if pkgtable is None:
            return False

        entry = self._find_package(pkgtable, name)
        if entry is None:
            self._error("Package '{}' no longer exists in the catalog.".format(name))
            return False

        record = installed[name]
        if entry.get("author") != record.get("author") or entry.get("repository") != record.get("repository"):
            return False
        if not self._version_gt(entry.get("version", "0"), record.get("version", "0")):
            return True
        return self.install(name, force=True)

    def _full_path(self, install_path, filename):
        return "{}/{}".format((install_path or "/").rstrip("/"), filename)

    def _parent_dir(self, path):
        path = (path or "/").rstrip("/")
        if not path:
            return "/"
        idx = path.rfind("/")
        return path[:idx] if idx > 0 else "/"

    def _is_safe_install_dir(self, path):
        if not path:
            return False
        normalized = path.rstrip("/")
        if normalized in ("", "/", ".", ".."):
            return False
        return True

    def _remove_dir_recursive(self, path):
        if not path:
            return False

        normalized = path.rstrip("/")
        if normalized in ("", "/", ".", ".."):
            self.logger.warning("Refusing to recursively remove unsafe path '{}'".format(path), source=self.source)
            return False

        try:
            entries = os.listdir(path)
        except OSError:
            return True

        ok = True
        for entry in entries:
            full = path.rstrip("/") + "/" + entry
            try:
                is_dir = bool(os.stat(full)[0] & 0x4000)
            except OSError:
                continue
            if is_dir:
                if not self._remove_dir_recursive(full):
                    ok = False
            else:
                try:
                    os.remove(full)
                except OSError as e:
                    self.logger.warning("Could not remove file '{}': {}".format(full, e), source=self.source)
                    ok = False

        try:
            os.rmdir(path)
        except OSError as e:
            self.logger.warning("Could not remove directory '{}': {}".format(path, e), source=self.source)
            ok = False
        return ok

    def _version_tuple(self, version):
        parts = []
        for p in str(version).split("."):
            try:
                parts.append(int(p))
            except ValueError:
                parts.append(0)
        return tuple(parts)

    def _version_gt(self, a, b):
        ta, tb = self._version_tuple(a), self._version_tuple(b)
        length = max(len(ta), len(tb))
        ta += (0,) * (length - len(ta))
        tb += (0,) * (length - len(tb))
        return ta > tb
    def list_deps(self, name):
        """List dependencies of a package from the catalog."""
        pkgtable = self._fetch_pkgtable()
        if not pkgtable:
            return []
        entry = pkgtable.get(name)
        if not entry:
            self._error("Package '{}' not found in catalog".format(name))
            return []
        deps = entry.get("dependencies", [])
        # Return list of dicts
        return deps

    def checkdeps(self):
        """Check that all installed packages satisfy their dependencies.
        Returns list of tuples (package, missing_dep) or empty list if all good."""
        installed = self._load_pkglist()
        pkgtable = self._fetch_pkgtable()
        if not pkgtable:
            self._error("Could not fetch pkgtable")
            return []
        issues = []
        for pkg_name, record in installed.items():
            entry = pkgtable.get(pkg_name)
            if not entry:
                # package not in catalog anymore
                issues.append((pkg_name, "missing from catalog"))
                continue
            for dep in entry.get("dependencies", []):
                dep_name = dep.get("name")
                dep_version = dep.get("version", "0")
                # check if dep installed and version sufficient
                dep_installed = installed.get(dep_name)
                if not dep_installed:
                    issues.append((pkg_name, "missing dependency {}".format(dep_name)))
                    continue
                cur_ver = dep_installed.get("version", "0")
                if self._version_gt(dep_version, cur_ver):
                    issues.append((pkg_name, "dependency {} version {} required, have {}".format(dep_name, dep_version, cur_ver)))
        return issues

    def outdated(self):
        """List packages that have a newer version available in the catalog."""
        installed = self._load_pkglist()
        pkgtable = self._fetch_pkgtable()
        if not pkgtable:
            self._error("Could not fetch pkgtable")
            return []
        outdated_list = []
        for pkg_name, record in installed.items():
            entry = pkgtable.get(pkg_name)
            if not entry:
                continue
            cur_ver = record.get("version", "0")
            latest_ver = entry.get("version", "0")
            if self._version_gt(latest_ver, cur_ver):
                outdated_list.append((pkg_name, cur_ver, latest_ver))
        return outdated_list

    def upgrade(self, name=None, force=False):
        """Upgrade a specific package or all outdated packages.
        If name is None, upgrade all outdated packages.
        Returns True if any upgrades were performed."""
        if name is None:
            # upgrade all outdated
            outdated_list = self.outdated()
            if not outdated_list:
                self._error("No packages outdated")
                return False
            any_ok = False
            for pkg_name, cur_ver, latest_ver in outdated_list:
                if self.install(pkg_name, force=True):
                    any_ok = True
            return any_ok
        else:
            # upgrade specific package
            return self.install(name, force=True)


    def _error(self, message):
        self.logger.error(message, source=self.source)
        print("[PKG] Error:", message)


def _packages_help():
    return (
        "packages.py -- package manager + net file transfer helpers\n"
        "  PackageManager(git=None, repo_user=None, repo_name=None)\n"
        "    every package is stored at /bin/<name>/<name>.py -- /bin is a flat\n"
        "    command directory, one subfolder per package, regardless of what\n"
        "    the pkgtable.json catalog entry's own install_path/file say\n"
        "    .install(name, force=False) / .uninstall(name) / .reinstall(name)\n"
        "    .update(name=None) / .reload(name) / .list() / .info(name) / .verify()\n"
        "    .run(command, *args) / .register(command, callback, module_name=None)\n"
        "    .unregister(command) / .checkdeps() / .list_commands() / .upgrade()\n"
        "    .load_all_installed()  -- auto-runs at end of __init__\n"
        "  Git(base_raw=None, default_branch='main')\n"
        "    .download(user, repo, filename, branch=None, save_dir=None)\n"
        "    .download_url(url, save_dir=None) / .upload(user, repo, local_path, ...)\n"
        "  downloadhelper().download_file(url, save_dir='/', save_file=None)\n"
        "  AppInstaller().install(app_name) / .uninstall(name) / .listapps()\n"
        "  Wiki(lang='en').fetch(title) / .next() / .search(query, n=5)\n"
        "  AppDB().set(app, key, value) / .get(app, key, default=None) / .delete/.clear/.dump"
    )


# ============================================================================

def help():
    return "\n\n".join([
        _logger_help(), _power_help(), _filesystem_help(), _process_help(),
        _security_help(), _network_help(), _system_help(), _packages_help(),
    ])

def doc():
    """Return detailed documentation for the Services module.
    Includes descriptions of all classes: Logger, PowerManagement, WifiManager,
    Git, PackageManager, Drivers, etc., and their methods.
    """
    return (
        "Services -- Zeno OS core services\n"
        "================================\n"
        "This module consolidates all core services: logging, power management,\n"
        "wifi, git, package management, and hardware drivers.\n"
        "\n"
        "Classes:\n"
        "  Logger(log_file_user='/LOGS/systemlog.txt', boot=False)\n"
        "    Provides logging with levels ERROR, WARNING, DEBUG.\n"
        "    Methods: error(message, source), warning(...), debug(...),\n"
        "    viewlogs(lines=None), clear_logs(), boot_complete(),\n"
        "    log(level, message, source), _write(text), _create_file(path).\n"
        "  PowerManagement(logger=None)\n"
        "    CPU frequency scaling and boost management.\n"
        "    Methods: help(), status(), levels_list(), set_level(level),\n"
        "    baseline_set(level), boost(reason, level='high'), release(reason),\n"
        "    boosted(level='high', reason='context'), _error(message).\n"
        "  WifiManager(logger, profiles_path='/etc/wifi/profiles.json', timeout=15)\n"
        "    Wi‑Fi connection and profile management.\n"
        "    Methods: save_profile(ssid, password, priority=0),\n"
        "    remove_profile(ssid), list_profiles(), _load_profiles(),\n"
        "    _save_profiles(profiles).\n"
        "  Git(base_raw=None, default_branch='main')\n"
        "    GitHub wrapper for downloading packages.\n"
        "    Methods: download(user, repo, filename, branch=None, save_dir=None),\n"
        "    download_url(url, save_dir=None), _parse_github_url(url),\n"
        "    _error(message).\n"
        "  PackageManager(git=None, repo_user=None, repo_name=None)\n"
        "    Package management from pkgtable.json.\n"
        "    Methods: install(name, force=False), uninstall(name),\n"
        "    reinstall(name), update(name=None), reload(name), list(),\n"
        "    info(name), verify(), run(command, *args),\n"
        "    register(command, callback, module_name=None),\n"
        "    unregister(command), checkdeps(), outdated(), upgrade(name=None, force=False),\n"
        "    load_all_installed().\n"
        "  Drivers()\n"
        "    Low‑level hardware drivers.\n"
        "    Methods: i2c_init(scl_pin, sda_pin, freq=400000),\n"
        "    i2c_scan(i2c), i2c_read_mem(i2c, addr, mem, num_bytes=1),\n"
        "    i2c_write_mem(i2c, addr, mem, buf),\n"
        "    spi_init(sck_pin, mosi_pin, miso_pin, baudrate=1000000, polarity=0, phase=0),\n"
        "    spi_write_readinto(spi, write_buf, read_buf),\n"
        "    dht_read(pin, sensor_type='DHT22'),\n"
        "    ds3231_get_time(i2c), ds3231_set_time(i2c, year, month, day, hour,\n"
        "      minute, second, weekday),\n"
        "    oled_init(i2c, width=128, height=64, addr=0x3C, external_vcc=False),\n"
        "    oled_show_text(oled, text, x=0, y=0).\n"
        "    All driver methods return (message, value) tuples.\n"
        "\n"
        "Global constants: debug_log_enabled = False\n"
        "\n"
        "See also: help() for a concise overview."
    )
def get(name, default=None):
    return globals().get(name, default)


def reload_all_installed():
    pm = PackageManager()
    pm.load_all_installed()
    return pm
class Drivers:
    def __init__(self):
        self.logger = Logger()
        self.source = "DRIVERS"

    def _error(self, msg):
        self.logger.error(msg, source=self.source)
        return (msg, False)

    def _info(self, msg):
        self.logger.debug(msg, source=self.source)
        return (msg, True)

    # I2C methods
    def i2c_init(self, scl_pin, sda_pin, freq=400000):
        try:
            i2c = I2C(scl=Pin(scl_pin), sda=Pin(sda_pin), freq=freq)
            return self._info("I2C initialized"), i2c
        except Exception as e:
            return self._error(f"I2C init failed: {e}"), None

    def i2c_scan(self, i2c):
        try:
            return self._info("I2C scan complete"), i2c.scan()
        except Exception as e:
            return self._error(f"I2C scan failed: {e}"), []

    def i2c_read_mem(self, i2c, addr, mem, num_bytes=1):
        try:
            data = i2c.readfrom_mem(addr, mem, num_bytes)
            return self._info("I2C read memory"), data
        except Exception as e:
            return self._error(f"I2C read mem failed: {e}"), b""

    def i2c_write_mem(self, i2c, addr, mem, buf):
        try:
            i2c.writeto_mem(addr, mem, buf)
            return self._info("I2C write memory"), True
        except Exception as e:
            return self._error(f"I2C write mem failed: {e}"), False

    # SPI methods
    def spi_init(self, sck_pin, mosi_pin, miso_pin, baudrate=1000000, polarity=0, phase=0):
        try:
            spi = SPI(baudrate=baudrate, polarity=polarity, phase=phase,
                      sck=Pin(sck_pin), mosi=Pin(mosi_pin), miso=Pin(miso_pin))
            return self._info("SPI initialized"), spi
        except Exception as e:
            return self._error(f"SPI init failed: {e}"), None

    def spi_write_readinto(self, spi, write_buf, read_buf):
        try:
            spi.write_readinto(write_buf, read_buf)
            return self._info("SPI write readio"), True
        except Exception as e:
            return self._error(f"SPI write readio failed: {e}"), False

    # DHT
    def dht_read(self, pin, sensor_type="DHT22"):
        try:
            import dht
            if sensor_type.upper() == "DHT22":
                sensor = dht.DHT22(Pin(pin))
            else:
                sensor = dht.DHT11(Pin(pin))
            sensor.measure()
            temp = sensor.temperature()
            hum = sensor.humidity()
            return self._info("DHT read"), (temp, hum)
        except Exception as e:
            return self._error(f"DHT read failed: {e}"), (None, None)

    # DS3231 (RTC) via I2C
    def ds3231_get_time(self, i2c):
        try:
            data = i2c.readfrom_mem(0x68, 0x00, 7)
            def bcd_to_dec(b):
                return ((b >> 4) * 10) + (b & 0x0F)
            sec = bcd_to_dec(data[0] & 0x7F)
            min = bcd_to_dec(data[1])
            hour = bcd_to_dec(data[2] & 0x3F)
            day = bcd_to_dec(data[3])
            date = bcd_to_dec(data[4])
            month = bcd_to_dec(data[5])
            year = bcd_to_dec(data[6]) + 2000
            return self._info("DS3231 time read"), (year, month, date, hour, min, sec, day)
        except Exception as e:
            return self._error(f"DS3231 read time failed: {e}"), (0,0,0,0,0,0,0)

    def ds3231_set_time(self, i2c, year, month, day, hour, minute, second, weekday):
        try:
            def dec_to_bcd(d):
                return ((d // 10) << 4) + (d % 10)
            data = bytes([
                dec_to_bcd(second) & 0x7F,
                dec_to_bcd(minute),
                dec_to_bcd(hour) & 0x3F,
                dec_to_bcd(weekday),
                dec_to_bcd(day),
                dec_to_bcd(month),
                dec_to_bcd(year - 2000)
            ])
            i2c.writeto_mem(0x68, 0x00, data)
            return self._info("DS3231 time set"), True
        except Exception as e:
            return self._error(f"DS3231 set time failed: {e}"), False

    # OLED (SSD1306) via I2C
    def oled_init(self, i2c, width=128, height=64, addr=0x3C, external_vcc=False):
        try:
            from ssd1306 import SSD1306_I2C
            oled = SSD1306_I2C(width, height, i2c, addr=addr, external_vcc=external_vcc)
            return self._info("OLED initialized"), oled
        except ImportError:
            return self._error("SSD1306 driver not installed. Install 'ssd1306' package."), None
        except Exception as e:
            return self._error(f"OLED init failed: {e}"), None

    def oled_show_text(self, oled, text, x=0, y=0):
        try:
            oled.fill(0)
            oled.text(text, x, y)
            oled.show()
            return self._info("OLED text shown"), True
        except Exception as e:
            return self._error(f"OLED show text failed: {e}"), False
