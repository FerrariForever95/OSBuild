import time
import os
import machine
import hashlib
import ucryptolib
import network
import gc
import pystone_lowmem
import json
import urequests as requests
import urequests
import usocket
import ntptime
import ssl
import micropython
import ubluetooth as bt
from machine import Pin, SPI,I2C,PWM, SoftSPI ,RTC
from firmware import DS3231
from firmware import SDCard
import _thread
import sys
import json
 # SD (Hardware SPI)
SD_SCK  = 12
SD_MOSI = 11
SD_MISO = 13
SD_CS   = 10

LOGS_DIR = "/LOGS"
EXT = ".sh"
CONST_TEXT = b"ZENO MICRO PC v4.3 beta"

# -------------------------
# Helpers
# -------------------------
def _exists(path):
    try:
        os.stat(path)
        return True
    except OSError:
        return False

def _makedirs(path):
    # simple make dirs (single-level or nested)
    if not path:
        return
    parts = path.split('/')
    cur = ""
    for p in parts:
        if not p:
            continue
        cur = cur + "/" + p if cur else p
        if not _exists(cur):
            try:
                os.mkdir(cur)
            except Exception:
                pass

def _random_bytes(n):
    # prefer os.urandom if present
    try:
        return os.urandom(n)
    except Exception:
        # fallback deterministic-ish: hash machine id repeatedly
        uid = machine.unique_id()
        out = bytearray()
        i = 0
        while len(out) < n:
            h = hashlib.sha256(uid + bytes([i & 0xFF])).digest()
            out.extend(h)
            i += 1
        return bytes(out[:n])

def _hmac_sha256(key, data):
    # simple HMAC-SHA256 implementation
    block = 64
    if len(key) > block:
        key = hashlib.sha256(key).digest()
    if len(key) < block:
        key = key + b'\x00' * (block - len(key))
    o_key = bytes([b ^ 0x5c for b in key])
    i_key = bytes([b ^ 0x36 for b in key])
    inner = hashlib.sha256(i_key + data).digest()
    return hashlib.sha256(o_key + inner).digest()

def _pkcs7_pad(b):
    pad = 16 - (len(b) % 16)
    return b + bytes([pad]) * pad

def _pkcs7_unpad(b):
    if not b:
        raise ValueError("Invalid padding")
    pad = b[-1]
    if pad < 1 or pad > 16:
        raise ValueError("Invalid padding value")
    if b[-pad:] != bytes([pad]) * pad:
        raise ValueError("Invalid padding bytes")
    return b[:-pad]

def _aes_ecb(key):
    # create AES-ECB using ucryptolib
    return ucryptolib.aes(key, 1)

def _aes_cbc_encrypt(aes_key, iv, plaintext):
    # implement CBC using ECB primitive
    cipher = _aes_ecb(aes_key)
    prev = iv
    out = bytearray()
    for i in range(0, len(plaintext), 16):
        block = plaintext[i:i+16]
        # xor with prev
        xb = bytes([block[j] ^ prev[j] for j in range(16)])
        eb = cipher.encrypt(xb)
        out.extend(eb)
        prev = eb
    return bytes(out)

def _aes_cbc_decrypt(aes_key, iv, ciphertext):
    cipher = _aes_ecb(aes_key)
    prev = iv
    out = bytearray()
    for i in range(0, len(ciphertext), 16):
        block = ciphertext[i:i+16]
        db = cipher.decrypt(block)
        pt = bytes([db[j] ^ prev[j] for j in range(16)])
        out.extend(pt)
        prev = block
    return bytes(out)

# -------------------------
# MemFile - in-memory file-like object that zeroes on close
# -------------------------
class MemFile:
    def __init__(self, data_bytes):
        self._buf = bytearray(data_bytes)
        self._pos = 0
        self._closed = False

    def read(self, n=-1):
        if self._closed:
            raise ValueError("I/O on closed file")
        if n is None or n < 0:
            data = bytes(self._buf[self._pos:])
            self._pos = len(self._buf)
            return data
        end = min(self._pos + n, len(self._buf))
        data = bytes(self._buf[self._pos:end])
        self._pos = end
        return data

    def readline(self):
        if self._closed:
            raise ValueError("I/O on closed file")
        buf = self._buf
        ln = len(buf)
        if self._pos >= ln:
            return b""
        for i in range(self._pos, ln):
            if buf[i] == 0x0A:
                i += 1
                data = bytes(buf[self._pos:i])
                self._pos = i
                return data
        data = bytes(buf[self._pos:])
        self._pos = ln
        return data

    def seek(self, offset, whence=0):
        if self._closed:
            raise ValueError("I/O on closed file")
        if whence == 0:
            pos = offset
        elif whence == 1:
            pos = self._pos + offset
        elif whence == 2:
            pos = len(self._buf) + offset
        else:
            raise ValueError("Invalid whence")
        if pos < 0:
            raise ValueError("Negative seek")
        self._pos = min(pos, len(self._buf))
        return self._pos

    def tell(self):
        return self._pos

    def close(self):
        if not self._closed and self._buf is not None:
            try:
                for i in range(len(self._buf)):
                    self._buf[i] = 0
            except Exception:
                pass
        self._buf = None
        self._closed = True

# -------------------------
# ZenZip class
# -------------------------
class ZenZip:
    def __init__(self, user_key=b"", logs_dir=LOGS_DIR):
        """
        user_key: short bytes you supply (recommended 4 bytes). Will be concatenated.
        logs_dir: directory where encrypted files are stored (default /LOGS).
        """
        
        if isinstance(user_key, str):
            user_key = user_key.encode()
        self.user_key = user_key
        self.logs_dir = logs_dir
        _makedirs(self.logs_dir)
        
        # derive AES key and MAC key (from sha256)
        self._derive_keys()

    def _derive_keys(self):
        uid = machine.unique_id() or b""
        data = uid + self.user_key + CONST_TEXT
        h = hashlib.sha256(data).digest()  # 32 bytes
        # split into AES key (16) and MAC key (16)
        self.aes_key = h[:16]
        self.mac_key = h[16:]

    def _encrypted_path(self, name):
        # ensure ext .sh
        if not name.endswith(EXT):
            name = name + EXT
        return self.logs_dir.rstrip("/") + "/" + name

    def list(self):
        try:
            return [f for f in os.listdir(self.logs_dir) if f.endswith(EXT)]
        except Exception:
            return []

    def encrypt_file(self, src_path, dest_name=None):
        """
        Encrypt src_path and store as /LOGS/<dest_name or basename>.sh
        Deletes the original file after success.
        Returns dest filename (basename with .sh)
        """
        if not _exists(src_path):
            raise OSError("Source not found: " + src_path)
        base = dest_name if dest_name else src_path.split("/")[-1]
        if base.endswith(EXT):
            base = base[:-len(EXT)]
        out_name = base + EXT
        out_path = self._encrypted_path(base)

        with open(src_path, "rb") as f:
            plain = f.read()

        # pad
        padded = _pkcs7_pad(plain)

        iv = _random_bytes(16)
        ct = _aes_cbc_encrypt(self.aes_key, iv, padded)
        mac = _hmac_sha256(self.mac_key, iv + ct)

        # write file as: iv || ct || mac
        try:
            with open(out_path, "wb") as fo:
                fo.write(iv)
                fo.write(ct)
                fo.write(mac)
        except Exception as e:
            raise OSError("Failed to write encrypted file: " + str(e))

        # delete original file if possible
        try:
            os.remove(src_path)
        except Exception:
            pass

        return out_name

    def _read_blob(self, name):
        path = self._encrypted_path(name if name.endswith(EXT) else name)
        if not _exists(path):
            raise OSError("Encrypted file not found: " + path)
        with open(path, "rb") as f:
            data = f.read()
        if len(data) < 16 + 32:
            raise ValueError("Invalid encrypted file format")
        iv = data[:16]
        mac = data[-32:]
        ct = data[16:-32]
        return iv, ct, mac

    def get_bytes(self, name):
        """
        Decrypt and return bytes (no temp file). name can be basename or with .sh
        """
        iv, ct, mac = self._read_blob(name)
        # verify mac
        if _hmac_sha256(self.mac_key, iv + ct) != mac:
            raise ValueError("MAC mismatch - wrong key or tampered")
        pt_padded = _aes_cbc_decrypt(self.aes_key, iv, ct)
        pt = _pkcs7_unpad(pt_padded)
        # attempt to zero sensitive buffers (best-effort)
        try:
            if isinstance(pt_padded, bytearray):
                for i in range(len(pt_padded)): pt_padded[i] = 0
        except Exception:
            pass
        return pt

    def get_text(self, name):
        """
        Decrypt and return UTF-8 decoded string.
        """
        b = self.get_bytes(name)
        try:
            s = b.decode("utf-8")
        except Exception:
            # fall back to latin1 if needed
            s = b.decode("latin1")
        # zero bytes buffer reference if possible
        try:
            if isinstance(b, bytearray):
                for i in range(len(b)): b[i] = 0
        except Exception:
            pass
        return s

    def get_fileobj(self, name):
        """
        Decrypt and return an in-memory file-like object (MemFile).
        Caller MUST call .close() when done to zero memory.
        """
        b = self.get_bytes(name)
        return MemFile(b)

    def remove(self, name):
        """
        Delete the encrypted file from /LOGS (pass basename or name.sh).
        """
        path = self._encrypted_path(name if name.endswith(EXT) else name)
        try:
            if _exists(path):
                os.remove(path)
                return True
        except Exception:
            pass
        return False
class Logger:
    LEVELS = {0: "ERROR", 1: "WARNING", 2: "DEBUG"}

    def __init__(self, log_file_user="/LOGS/systemlog.txt", boot=False):
        # Initialize RTC
        self.i2c = I2C(0, scl=Pin(4), sda=Pin(5))
        self.rtc = DS3231(self.i2c)

        self.boot = boot
        self.log_file_user = log_file_user
        self._create_file(self.log_file_user)

        self._boot_marker = f"[BOOT_START_{self._time_str()}]"

        if self.boot:
            self._write(self._boot_marker)
            self.debug("Logger initialized. Boot starting...", source="BOOT")

    def _create_file(self, path):
        try:
            with open(path, "a"):
                pass
            return True
        except Exception as e:
            print("[Logger] File creation failed:", e)
            return False

    def _time_str(self):
        try:
            t = self.rtc.datetime()
            return "{:04}-{:02}-{:02} {:02}:{:02}:{:02}".format(*t[:3], *t[4:7])
        except:
            return "0000-00-00 00:00:00"

    def _write(self, text):
        if debug_log_enabled:
            print(text)
        try:
            with open(self.log_file_user, "a") as f:
                f.write(text + "\n")
        except:
            pass

    def log(self, level, message, source="GENERAL"):
        level_name = self.LEVELS.get(level, "UNKNOWN")
        ts = self._time_str()
        entry = f"[{ts}] [SRC:{source}] [{level_name}] {message}"
        self._write(entry)

    def error(self, message, source="GENERAL"):
        self.log(0, message, source)

    def warning(self, message, source="GENERAL"):
        self.log(1, message, source)

    def debug(self, message, source="GENERAL"):
        self.log(2, message, source)

    def boot_complete(self):
        self.debug("Boot sequence complete.", source="BOOT")
        self._write("_" * 40)

    def viewlogs(self, lines=None):
        try:
            with open(self.log_file_user, "r") as f:
                data = f.read()
        except Exception as e:
            print("[Logger] Failed to read logs:", e)
            return

        logs = data.strip().split("\n")

        # Find last boot marker
        last_boot_index = 0
        for i, line in enumerate(logs):
            if line.startswith("[BOOT_START_"):
                last_boot_index = i

        logs = logs[last_boot_index:]

        if lines:
            logs = logs[-lines:]

        print("\n".join(logs))

    def clear_logs(self):
        try:
            with open(self.log_file_user, "w") as f:
                f.write("")
            print("[Logger] Logs cleared successfully.")
            self.debug("Logs cleared successfully by system.", source="LOGGER")
        except Exception as e:
            print("[Logger] Failed to clear logs:", e)
            self.error(f"Failed to clear logs: {e}", source="LOGGER")
    def help(self):
        print("\n[Logger Module]")
        print("Description:")
        print("  Provides persistent logging facilities with RTC-based timestamps.")
        print("  Supports boot tracking, severity levels, and log file management.\n")

        print("Methods:\n")

        print("  log(level, message, source='GENERAL')")
        print("    Writes a log entry with specified severity level and source.\n")

        print("  error(message, source='GENERAL')")
        print("    Logs an error-level message.\n")

        print("  warning(message, source='GENERAL')")
        print("    Logs a warning-level message.\n")

        print("  debug(message, source='GENERAL')")
        print("    Logs a debug-level message.\n")

        print("  boot_complete()")
        print("    Marks the end of the boot sequence in the log.\n")

        print("  viewlogs(lines=None)")
        print("    Displays log entries from the most recent boot.")
        print("    If lines is provided, limits output to the last N entries.\n")

        print("  clear_logs()")
        print("    Clears the log file and records the operation.")
class Disk:
    """
    Disk class for handling SD card operations.

    Attributes:
        mount_point (str): Path where the SD card is mounted.
        spi (SPI): SPI interface used for SD card communication.
        cs (Pin): Chip select pin for the SD card.
        sd (SDCard): SD card object after initialization.
    """

    def __init__(self, mount_point="/SYSTEM32"):
        self.mount_point = mount_point
        self.spi = SPI(
            1,
            baudrate=20_000_000,
            polarity=0,
            phase=0,
            sck=Pin(SD_SCK, Pin.OUT),
            mosi=Pin(SD_MOSI, Pin.OUT),
            miso=Pin(SD_MISO, Pin.OUT),
        )
        self.sd = None
        self.target_path = None
        self.cs = Pin(SD_CS, Pin.OUT)
        self.log = Logger()

    # -------------------------------------------------- #
    # CHECK MOUNT
    # -------------------------------------------------- #
    def check(self, retries=5, delay=0.2):
        """Check if SD card is mounted at mount_point."""
        for i in range(retries):
            try:
                os.listdir(self.mount_point)
                if i > 0:
                    self.log.debug(
                        "SD mount became available after {} retries".format(i + 1),
                        source="DISK",
                    )
                return True
            except OSError:
                time.sleep(delay)
        self.log.error(
            "SD card not accessible at '{}' after {} retries".format(
                self.mount_point, retries
            ),
            source="DISK",
        )
        return False

    # -------------------------------------------------- #
    # INIT / MOUNT
    # -------------------------------------------------- #
    def begin(self):
        """Initialize and mount the SD card."""
        try:
            self.sd = SDCard(self.spi, self.cs)
            os.mount(self.sd, self.mount_point)
            self.log.debug(
                "SD card mounted at '{}'".format(self.mount_point), source="DISK"
            )
            return True
        except Exception as e:
            self.sd = None
            self.log.error(
                "SD init/mount failed for '{}': {}".format(self.mount_point, e),
                source="DISK",
            )

        # If mount failed, check if it's somehow already mounted
        if self.check():
            self.log.warning(
                "SD init failed, but '{}' appears to be accessible".format(
                    self.mount_point
                ),
                source="DISK",
            )
            return True

        return False

    # -------------------------------------------------- #
    # LIST FILES
    # -------------------------------------------------- #
    def listfiles(self, target_path="/SYSTEM32"):
        """List all files in the specified path."""
        try:
            files = os.listdir(target_path)
            if not files:
                self.log.warning("No files in {}".format(target_path), source="DISK")
                return []
            self.log.debug("Files in {}: {}".format(target_path, files), source="DISK")
            for f in files:
                print(" -", f)
            return files
        except OSError as e:
            self.log.error(
                "Error accessing '{}': {}".format(target_path, e), source="DISK"
            )
            return []

    # -------------------------------------------------- #
    # MKDIR
    # -------------------------------------------------- #
    def mkdir(self, path):
        """Create a directory at the specified path."""
        try:
            path = path.rstrip("/") or "/"  # Normalize path
            os.mkdir(path)
            self.log.debug("Directory created: {}".format(path), source="DISK")
            return True
        except OSError as e:
            if len(e.args) > 0 and e.args[0] == 17:  # Directory exists
                self.log.warning(
                    "Directory already exists: {}".format(path), source="DISK"
                )
            else:
                self.log.error(
                    "Failed to create directory '{}': {}".format(path, e),
                    source="DISK",
                )
            return False
    def rmdir(self, path):
        try:
            path = path.rstrip("/") or "/"
            os.rmdir(path)
            self.log.debug("Directory deleted: {}".format(path), source="DISK")
            return True

        except OSError as e:
            self.log.error(
            "Failed to delete directory '{}': {}".format(path, e),
            source="DISK",
            )
            return False
    # -------------------------------------------------- #
    # INFO
    # -------------------------------------------------- #
    def info(self, path=None):
        """Display SD card information with auto MB/GB conversion."""
        if path is None:
            path = self.mount_point

        try:
            stats = os.statvfs(path)
            block_size = stats[0]
            total_blocks = stats[2]
            free_blocks = stats[3]

            total_bytes = total_blocks * block_size
            free_bytes = free_blocks * block_size

            def convert(bytes_value):
                if bytes_value >= 1024**3:
                    return "{:.2f} GB".format(bytes_value / (1024**3))
                else:
                    return "{:.2f} MB".format(bytes_value / (1024**2))

            print("Path       :", path)
            print("Volume Name:", path.split("/")[-1])
            print("Total Size :", convert(total_bytes))
            print("Free Space :", convert(free_bytes))

            self.log.debug(
                "Disk info for '{}': total={}, free={}".format(
                    path, convert(total_bytes), convert(free_bytes)
                ),
                source="DISK",
            )

        except Exception as e:
            self.log.error("Cannot access '{}': {}".format(path, e), source="DISK")

    # -------------------------------------------------- #
    # DELETE FILE
    # -------------------------------------------------- #
    def delete(self, p):
        """Delete a file if it exists (MicroPython compatible)."""
        try:
            os.remove(p)
            self.log.debug("File deleted: {}".format(p), source="DISK")
            return True
        except OSError as e:
            self.log.error("Could not delete file '{}': {}".format(p, e), source="DISK")
            return False

    # -------------------------------------------------- #
    # DELETE FOLDER (RECURSIVE)
    # -------------------------------------------------- #
    def del_folder(self, p):
        """Recursively delete a folder and all its contents."""
        try:
            # Ensure directory exists
            entries = os.listdir(p)
        except OSError as e:
            self.log.error(
                "Could not list folder '{}': {}".format(p, e), source="DISK"
            )
            return False

        # Walk entries
        for entry in entries:
            full_path = p.rstrip("/") + "/" + entry
            try:
                st = os.stat(full_path)[0]
                is_dir = bool(st & 0x4000)  # directory flag
            except OSError as e:
                self.log.error(
                    "Could not stat '{}': {}".format(full_path, e), source="DISK"
                )
                continue

            if is_dir:
                # Recurse
                if not self.del_folder(full_path):
                    # Already logged inside
                    continue
            else:
                # File
                try:
                    os.remove(full_path)
                    self.log.debug(
                        "Deleted file in folder: {}".format(full_path), source="DISK"
                    )
                except OSError as e:
                    self.log.error(
                        "Could not delete file '{}': {}".format(full_path, e),
                        source="DISK",
                    )

        # Finally remove folder itself
        try:
            os.rmdir(p)
            self.log.debug("Folder deleted: {}".format(p), source="DISK")
            return True
        except OSError as e:
            self.log.error(
                "Could not delete folder '{}': {}".format(p, e), source="DISK"
            )
            return False

    # -------------------------------------------------- #
    # HELP
    # -------------------------------------------------- #
    def help(self):
        print("\n[Disk Module]")
        print("Description:")
        print("  Provides SD card initialization, mounting, and filesystem operations")
        print("  using SPI-based SD card access.\n")

        print("Methods:\n")

        print("  begin()")
        print("    Initializes the SD card and mounts it at the configured mount point.\n")

        print("  check(retries=5, delay=0.2)")
        print("    Checks whether the SD card is accessible at the mount point.\n")

        print("  listfiles(target_path='/SYSTEM32')")
        print("    Lists files and directories at the specified path.\n")

        print("  mkdir(path)")
        print("    Creates a directory at the specified filesystem path.\n")

        print("  info(path=None)")
        print("    Displays filesystem size and free space information.\n")

        print("  delete(path)")
        print("    Deletes a single file if it exists.\n")

        print("  del_folder(path)")
        print("    Recursively deletes a directory and all its contents.\n")

class BootConfig:
    def __init__(self, user_key=b"2006"):
        self.vault = ZenZip(user_key=user_key)
        self.default = {
            "BOOT_MODE": "NORMAL",
            "OPT_LEVEL": 0,
            "WIFI_AUTOCONNECT": True,
            "SHOW_UI": True,
            "KERNEL_PATH": "/SYSTEM32/Admin/ROM/kernel.py",
            "LOGGER_STATUS": "ENABLED",
            "LOG_REPL":"ENABLED",
            "MODE":"PERFORMANCE"
        }

        self.cfg_name = "bootcfg.cfg"
        self.enc_name = "bootcfg.cfg.sh"
        self.cfg_dir = "/LOGS"
        self.config = {}

        # --- Ensure /LOGS exists ---
        try:
            files = os.listdir(self.cfg_dir)
        except Exception:
            os.mkdir(self.cfg_dir)
            files = []

        # --- Load or create config ---
        if self.enc_name in files:
            print("[BOOTCFG] Found encrypted config. Decrypting...")
            try:
                data = self.vault.get_text(self.cfg_name)
                self.config = json.loads(data)
                print("[BOOTCFG] Configuration loaded successfully.")
            except Exception as e:
                print("[BOOTCFG] Decrypt or parse failed:", e)
                self.config = dict(self.default)
                self.save()
        else:
            print("[BOOTCFG] No existing config. Creating default...")
            self.config = dict(self.default)
            self.save()

    # -----------------------------------------------------------------
    def _file_exists(self, filename):
        """MicroPython-safe file existence check."""
        try:
            return filename in os.listdir(self.cfg_dir)
        except Exception:
            return False

    # -----------------------------------------------------------------
    def save(self):
        """Encrypt and store updated configuration."""
        try:
            enc_path = self.cfg_dir + "/" + self.enc_name
            # delete existing encrypted file if it exists
            if self._file_exists(self.enc_name):
                os.remove(enc_path)

            # write plaintext version
            with open(self.cfg_name, "w") as file:
                json.dump(self.config, file)

            # encrypt (ZenZip moves to /LOGS and deletes original)
            self.vault.encrypt_file(self.cfg_name)
            print("[BOOTCFG] Configuration saved and encrypted.")
        except Exception as e:
            print("[BOOTCFG] Save failed:", e)

    # -----------------------------------------------------------------
    def get(self, key, default=None):
        """Return value for key."""
        return self.config.get(key, default)

    def set(self, key, value):
        """Update config key and re-encrypt."""
        self.config[key] = value
        self.save()

    # -----------------------------------------------------------------
    def show(self):
        """Display current configuration."""
        print("\n[Boot Configuration]")
        for k, v in self.config.items():
            print(" ", k, ":", v)
        print()
debug_log_enabled=False
def cfg_get(cfg, *keys):
    for k in keys:
        if k in cfg:
            return cfg[k]
bootcfg = BootConfig()
cfg = getattr(bootcfg, "config", {}) or {}
class system:
    def __init__(self, opt_level=0, debug=False):
        self.opt_level_value = opt_level
        self.debug = debug
        self.log = Logger()
        self.path = "/SYSTEM32"
        self.cfg=BootConfig()

    def restart(self):
        try:
            self.log.debug(message="System restart requested", source="SYSTEM")
            machine.reset()
        except Exception as e:
            self.log.error(
                message="System restart failed: {}".format(e),
                source="SYSTEM"
            )

    def optlevel(self, level):
        try:
            level_int = int(level)
            micropython.opt_level(level_int)
            self.opt_level_value = level_int
            self.log.debug(
                message="System optimization level set to {}".format(level_int),
                source="SYSTEM"
            )
        except Exception as e:
            self.log.error(
                message="Error configuring optimization level '{}': {}".format(level, e),
                source="SYSTEM"
            )

    def info(self):
        try:
            cpu_freq_mhz = machine.freq() / 1_000_000
            cpu_cores = 2
            heap_total_mb = (gc.mem_free() + gc.mem_alloc()) / (1024 * 1024)
            heap_free_mb = gc.mem_free() / (1024 * 1024)
            unique_id = int.from_bytes(machine.unique_id(), 'big')
            chip_model = "ESP32-S3"
            print("Zeno Micro PC Version: V4.X alpha")
            print("CPU:", chip_model)
            print("CPU Frequency:", cpu_freq_mhz, "MHz")
            print("CPU Cores:", cpu_cores)
            print("Installed RAM:", heap_total_mb, "MB")
            print("Unique ID:", unique_id)
            print("Installed internal ROM: 16 MB")
            print("Disk Info:")
            self.log.debug(message="System info displayed", source="SYSTEM")

        except Exception as e:
            self.log.error(
                message="Failed to gather base system info: {}".format(e),
                source="SYSTEM"
            )
            return

        # Disk / filesystem info
        path = "/SYSTEM32"
        try:
            stats = os.statvfs(path)
            block_size = stats[0]
            total_blocks = stats[2]
            free_blocks = stats[3]

            total_bytes = total_blocks * block_size
            free_bytes = free_blocks * block_size

            def convert(bytes_value):
                if bytes_value >= 1024 ** 3:
                    return "{:.2f} GB".format(bytes_value / (1024 ** 3))
                else:
                    return "{:.2f} MB".format(bytes_value / (1024 ** 2))

            print("Path       :", path)
            print("Volume Name:", path.split('/')[-1])
            print("Total Size :", convert(total_bytes))
            print("Free Space :", convert(free_bytes))

        except Exception as e:
            self.log.error(
                message="Error accessing filesystem stats for '{}': {}".format(path, e),
                source="SYSTEM"
            )

    def memconfig(self, percent=25):
        try:
            gc.collect()
            free = gc.mem_free()
            alloc = gc.mem_alloc()
            threshold = alloc + free * percent // 100
            gc.threshold(threshold)
            micropython.alloc_emergency_exception_buf(100)
            self.log.debug(
                message=(
                    "Memory configuration updated: free={} bytes, alloc={} bytes, "
                    "threshold_percent={} (threshold={})"
                ).format(free, alloc, percent, threshold),
                source="SYSTEM"
            )
        except Exception as e:
            self.log.error(
                message="Failed to configure memory/GC with percent {}: {}".format(percent, e),
                source="SYSTEM"
            )

    def force_mem(self):
        try:
            before = gc.mem_free()
            gc.collect()
            after = gc.mem_free()
            delta = after - before
            self.log.debug(
                message="Forced GC executed: free memory {} → {} bytes (delta {})."
                        .format(before, after, delta),
                source="SYSTEM"
            )
        except Exception as e:
            self.log.error(
                message="Forced garbage collection failed: {}".format(e),
                source="SYSTEM"
            )

    def mem_usage(self):
        try:
            free = gc.mem_free()
            alloc = gc.mem_alloc()
            total = free + alloc

            if total <= 0:
                self.log.error(
                    message="Cannot compute memory usage: total memory reported as 0.",
                    source="SYSTEM"
                )
                print("[System] Memory usage: total memory is 0, cannot compute percentages.")
                return

            used_pct = (alloc / total) * 100
            free_pct = (free / total) * 100

            def format_mem(val):
                if val >= 1024 * 1024:
                    return "{:.2f} MB".format(val / (1024 * 1024))
                elif val >= 1024:
                    return "{:.2f} KB".format(val / 1024)
                else:
                    return "{} B".format(val)

            print("[System] Memory usage:")
            print("  Total: {}".format(format_mem(total)))
            print("  Used:  {} ({:.2f}%)".format(format_mem(alloc), used_pct))
            print("  Free:  {} ({:.2f}%)".format(format_mem(free), free_pct))

            self.log.debug(message="System memory information displayed", source="SYSTEM")

        except Exception as e:
            self.log.error(
                message="Failed to read memory usage: {}".format(e),
                source="SYSTEM"
            )

    def perf_test(self):
        self.log.debug(message="Performing system hardware test", source="SYSTEM")

        # CPU benchmark
        try:
            pystone_lowmem.main(1000)
            self.log.debug(
                message="CPU benchmark (pystone_lowmem) completed.",
                source="SYSTEM"
            )
        except Exception as e:
            self.log.error(
                message="CPU benchmark (pystone_lowmem) failed: {}".format(e),
                source="SYSTEM"
            )

        # RAM benchmark: allocate and free a large list
        try:
            start_ram = gc.mem_free() / (1024 * 1024)
            l = [0] * 100000
            mid_ram = gc.mem_free() / (1024 * 1024)
            del l
            gc.collect()
            end_ram = gc.mem_free() / (1024 * 1024)
            self.log.debug(
                message="RAM test: start={:.3f} MB, during alloc={:.3f} MB, after free={:.3f} MB"
                        .format(start_ram, mid_ram, end_ram),
                source="SYSTEM"
            )
        except Exception as e:
            self.log.error(
                message="RAM performance test failed: {}".format(e),
                source="SYSTEM"
            )

        # Flash benchmark: write/read temporary file
        try:
            start_flash = time.ticks_ms()
            tmp_path = "/tmp_test.bin"

            with open(tmp_path, "wb") as f:
                f.write(bytearray(1024 * 50))  # 50 KB

            with open(tmp_path, "rb") as f:
                _ = f.read()

            try:
                os.remove(tmp_path)
            except Exception as e_rm:
                self.log.error(
                    message="Flash test cleanup failed (could not remove '{}'): {}"
                            .format(tmp_path, e_rm),
                    source="SYSTEM"
                )

            flash_time = time.ticks_diff(time.ticks_ms(), start_flash)
            self.log.debug(
                message="Flash test complete: 50KB write/read in {} ms".format(flash_time),
                source="SYSTEM"
            )
        except Exception as e:
            self.log.error(
                message="Flash performance test failed: {}".format(e),
                source="SYSTEM"
            )
    def mode(self, m):
        if not isinstance(m, str):
            m = str(m)

        s = m.strip().upper()

        # map user input → (MODE, opt_level)
        mode_map = {
            "PERF": ("PERFORMANCE", 0),
            "PERFORMANCE": ("PERFORMANCE", 0),

            "BAL": ("BALANCED", 3),
            "BALANCED": ("BALANCED", 3),

            "SAVE": ("POWERSAVING", 3),
            "POWERSAVE": ("POWERSAVING", 3),
            "POWERSAVING": ("POWERSAVING", 3),
        }

        selected = None

        for key, tup in mode_map.items():
            if key in s:
                selected = tup
                break

        if not selected:
            self.log.error("Unknown mode requested: {}".format(m), "SYSTEM")
            return

        mode_name, optlevel_val = selected

        # save mode to bootcfg
        self.cfg.set("MODE", mode_name)

        # set MicroPython optimization now
        self.optlevel(optlevel_val)

        self.log.debug(
            "Mode set → {} (optlevel {}) – rebooting".format(mode_name, optlevel_val),
            "SYSTEM"
        )

        machine.reset()
        
    def help(self):
        print("\n[System Module]")
        print("Description:")
        print("  Provides low-level system control including runtime configuration,")
        print("  memory management, performance diagnostics, and system information.\n")

        print("Methods:\n")

        print("  restart()")
        print("    Requests a soft system reboot using the hardware reset interface.\n")

        print("  optlevel(level)")
        print("    Sets the MicroPython optimization level for runtime execution.\n")

        print("  info()")
        print("    Displays hardware, memory, filesystem, and network identification data.\n")

        print("  memconfig(percent=25)")
        print("    Configures garbage collection thresholds based on available memory.\n")

        print("  force_mem()")
        print("    Forces immediate garbage collection and reports memory changes.\n")

        print("  mem_usage()")
        print("    Outputs current memory usage statistics with percentages.\n")

        print("  perf_test()")
        print("    Executes CPU, RAM, and flash storage performance tests.\n")

        print("  mode(m)")
        print("    Sets the system operating mode and applies corresponding optimization levels.")

        

    def firmware_update(self):
        self._safe_update("firmware.py", "/LOGS/firmwarecopy.py", "firmwarestable.py")

    def boot_update(self):
        self._safe_update("boot.py", "/LOGS/bootcopy.py")

    def _safe_update(self, src_file, log_dest, stable_file=None):
        # Step 1: Read source
        try:
            with open(src_file, "rb") as fsrc:
                data = fsrc.read()
        except Exception as e:
            self.log.error(
                message="Safe update failed while reading '{}': {}".format(src_file, e),
                source="SYSTEM"
            )
            return

        # Step 2: Ensure /LOGS exists
        try:
            try:
                os.mkdir("/LOGS")
            except OSError as e:
                # Ignore 'directory exists' (errno 17 on many systems)
                if len(e.args) > 0 and e.args[0] != 17:
                    raise
        except Exception as e:
            self.log.error(
                message="Safe update failed while ensuring '/LOGS' directory: {}".format(e),
                source="SYSTEM"
            )
            return

        # Step 3: Write to /LOGS destination
        try:
            with open(log_dest, "wb") as fdest:
                fdest.write(data)
        except Exception as e:
            self.log.error(
                message="Safe update failed while writing backup '{}' → '{}': {}"
                        .format(src_file, log_dest, e),
                source="SYSTEM"
            )
            return

        # Step 4: Also write to stable file if provided
        if stable_file:
            try:
                with open(stable_file, "wb") as fstable:
                    fstable.write(data)
            except Exception as e:
                self.log.error(
                    message="Safe update failed while writing stable copy '{}' → '{}': {}"
                            .format(src_file, stable_file, e),
                    source="SYSTEM"
                )
                return

        # Step 5: Log success and attempt restart
        self.log.debug(
            message="{} backup complete (log='{}', stable='{}'). Restarting..."
                    .format(src_file, log_dest, stable_file),
            source="SYSTEM"
        )

        try:
            for i in range(5, 0, -1):
                print(i)
                time.sleep(1)
            self.log.debug(
                message="System expecting restart after backup of '{}'.".format(src_file),
                source="SYSTEM"
            )
            machine.reset()
        except Exception as e:
            self.log.error(
                message="Backup completed for '{}', but restart failed: {}".format(src_file, e),
                source="SYSTEM"
            )
class Network:

    def __init__(self, wifi_path="/wifi.json", tz_offset_sec=5*3600 + 30*60, key=b"2006"):
        self.wifi_path = wifi_path
        self.ip = None
        self.tz_offset_sec = tz_offset_sec
        self.vault = ZenZip(user_key=key)
        self.i2c = I2C(0, scl=Pin(4), sda=Pin(5))
        self.rtc = DS3231(self.i2c)
        self.log = Logger()
        self.wlan = network.WLAN(network.STA_IF)
        self.connected = self.wlan.isconnected()
        self.on = False
    # --------------------------------------------------------------- #
    # NON-BLOCKING CONNECT
    def connect(self, ssid=None, password=None):

        if self.wlan.isconnected():
            self.connected = True
            return True

        wlan = self.wlan
        wlan.active(True)

        if not ssid or not password:
            try:
                creds_text = self.vault.get_text("wifi.json.sh")
                creds = json.loads(creds_text)
                ssid = creds.get("ssid")
                password = creds.get("password")
            except Exception as e:
                print("[WiFi] Credential load failed:", e)
                return False

        print("[WiFi] Attempting connect to ",ssid)

        try:
            wlan.connect(ssid, password)
        except Exception as e:
            print("[WiFi] Connect error:", e)
            return False

        # Do NOT wait.
        # Just trigger and return.

        return True
    def update(self, timeout=8):

        if not self.on:
            return

        wlan = self.wlan

        if wlan.isconnected():
            self.ip = wlan.ifconfig()[0]
            self.connected = True
            self.on = False
            print("[WiFi] Connected:", self.ip)
            self.sync_time()
            return

        # Timeout handling
        if time.time() - self._connect_start > timeout:
            print("[WiFi] Timeout.")
            self.disconnect()
    # --------------------------------------------------------------- #
    # SCAN
    # --------------------------------------------------------------- #
    def scan(self):
        try:
            self.wlan.active(True)
            results = self.wlan.scan()
        except Exception as e:
            self.log.error("Wi-Fi scan failed: {}".format(e), source="NETWORK")
            return []

        networks = []
        for entry in results:
            try:
                ssid, bssid, channel, rssi, authmode, hidden = entry
                networks.append({
                    "ssid": ssid.decode() if ssid else "",
                    "rssi": rssi,
                    "auth": authmode,
                    "channel": channel,
                    "hidden": hidden
                })
            except Exception:
                pass

        networks.sort(key=lambda n: n["rssi"], reverse=True)
        return networks

    def scan_names(self):
        return [n["ssid"] or "<hidden>" for n in self.scan()]

    # --------------------------------------------------------------- #
    # DISCONNECT
    # --------------------------------------------------------------- #
    def disconnect(self):
        try:
            self.wlan.active(False)
        except:
            pass
        self.ip = None
        self.connected = False
        self.on = False

    # --------------------------------------------------------------- #
    # RTC SYNC
    # --------------------------------------------------------------- #
    def sync_time(self):
        if not self.connected:
            return
        try:
            ntptime.settime()
            utc = time.localtime()
            ts = time.mktime(utc) + self.tz_offset_sec
            lt = time.localtime(ts)
            self.rtc.datetime((lt[0], lt[1], lt[2], lt[3], lt[4], lt[5], lt[6]))
            self.print_rtc_time()
        except Exception as e:
            print("[Time] RTC sync failed:", e)

    # --------------------------------------------------------------- #
    # IP
    # --------------------------------------------------------------- #
    def ip_addr(self):
        try:
            if self.wlan.isconnected():
                return self.wlan.ifconfig()[0]
        except:
            pass
        return None

    # --------------------------------------------------------------- #
    # RTC STRING
    # --------------------------------------------------------------- #
    def get_rtc_time(self):
        try:
            t = self.rtc.datetime()
            return "{:04}-{:02}-{:02} {:02}:{:02}:{:02}".format(*t[:3], *t[4:7])
        except:
            return "0000-00-00 00:00:00"

    def print_rtc_time(self):
        self.log.debug("RTC Time: {}".format(self.get_rtc_time()), source="RTC")

    # --------------------------------------------------------------- #
    # PING
    # --------------------------------------------------------------- #
    def ping(self, url="http://example.com", timeout=5):
        try:
            if not url.startswith(("http://", "https://")):
                url = "http://" + url

            proto, _, hostport, *rest = url.split("/", 3)
            host = hostport.split(":")[0]
            port = 443 if proto == "https:" else 80
            path = "/" + (rest[0] if rest else "")

            addr = usocket.getaddrinfo(host, port)[0][-1]
            s = usocket.socket()
            s.settimeout(timeout)
            s.connect(addr)

            if proto == "https:":
                s = ssl.wrap_socket(s)

            s.send("HEAD {} HTTP/1.0\r\nHost: {}\r\n\r\n".format(path, host))
            resp = s.readline()
            s.close()

            return bool(resp and b"200" in resp)
        except:
            return False


# ===== DOWNLOADHELPER CLASS =====
class downloadhelper:
    def __init__(self):
        self.log = Logger()

    def download_file(self, url, save_dir="/", save_file=None):
        """
        Download a file from a given HTTP/HTTPS URL.
        Returns full_path on success, or None on any error.
        """
        if not url:
            self.log.error("Download failed: empty URL provided", source="DOWNLOADSERV")
            return None

        url = url.strip()
        self.log.debug("Starting download from URL: '{}'".format(url), source="DOWNLOADSERV")

        # Normalize URL
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

        # Parse URL
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

        print("[DownloadHelper] Host:", host)
        print("[DownloadHelper] Port:", port)
        print("[DownloadHelper] Path:", path)
        print("[DownloadHelper] Saving to: {}/{}".format(save_dir, save_file))

        # Resolve DNS
        try:
            addr_info = usocket.getaddrinfo(host, port)
            if not addr_info:
                self.log.error("DNS resolution returned no results for host '{}'".format(host),
                               source="DOWNLOADSERV")
                return None
            addr = addr_info[0][-1]
        except Exception as e:
            self.log.error("DNS resolution failed for host '{}': {}".format(host, e),
                           source="DOWNLOADSERV")
            return None

        # Open socket and perform HTTP/HTTPS GET
        try:
            s = usocket.socket()
        except Exception as e:
            self.log.error("Failed to create socket: {}".format(e), source="DOWNLOADSERV")
            return None

        try:
            try:
                s.connect(addr)
            except Exception as e:
                self.log.error("Failed to connect to {}:{} → {}".format(host, port, e),
                               source="DOWNLOADSERV")
                return None

            if proto == "https:":
                try:
                    s = ssl.wrap_socket(s)
                except Exception as e:
                    self.log.error("SSL wrap failed for '{}': {}".format(url, e),
                                   source="DOWNLOADSERV")
                    return None

            req = "GET {} HTTP/1.0\r\nHost: {}\r\n\r\n".format(path, host)
            try:
                s.send(req.encode())
            except Exception as e:
                self.log.error("Failed to send HTTP request to '{}': {}".format(url, e),
                               source="DOWNLOADSERV")
                return None

            # Read status line
            try:
                status_line = s.readline()
            except Exception as e:
                self.log.error("Failed to read HTTP status from '{}': {}".format(url, e),
                               source="DOWNLOADSERV")
                return None

            if not status_line:
                self.log.warning("No response from server '{}'".format(url),
                                 source="DOWNLOADSERV")
                return None

            try:
                parts = status_line.decode().split()
            except Exception as e:
                self.log.error("Failed to decode HTTP status line from '{}': {}"
                               .format(url, e), source="DOWNLOADSERV")
                return None

            if len(parts) < 2 or parts[1] != "200":
                self.log.error("HTTP error from '{}': {}".format(
                    url, status_line.decode().strip()),
                    source="DOWNLOADSERV"
                )
                return None

            # Skip headers
            try:
                while True:
                    line = s.readline()
                    if not line or line == b"\r\n":
                        break
            except Exception as e:
                self.log.error("Failed while reading HTTP headers from '{}': {}".format(url, e),
                               source="DOWNLOADSERV")
                return None

            # Ensure save directory exists (best-effort, only top-level)
            try:
                if save_dir not in os.listdir("/"):
                    try:
                        os.mkdir(save_dir)
                        self.log.debug("Created directory '{}' for download".format(save_dir),
                                       source="DOWNLOADSERV")
                    except Exception as e_mk:
                        self.log.error("Failed to create directory '{}': {}".format(save_dir, e_mk),
                                       source="DOWNLOADSERV")
                        return None
            except Exception as e:
                self.log.error("Failed to list root directories while checking '{}': {}"
                               .format(save_dir, e), source="DOWNLOADSERV")
                return None

            # Write file
            full_path = save_dir + "/" + save_file
            try:
                with open(full_path, "wb") as f:
                    while True:
                        try:
                            data = s.recv(512)
                        except Exception as e:
                            self.log.error("Socket recv failed from '{}': {}".format(url, e),
                                           source="DOWNLOADSERV")
                            return None
                        if not data:
                            break
                        try:
                            f.write(data)
                        except Exception as e:
                            self.log.error("Failed writing to '{}': {}".format(full_path, e),
                                           source="DOWNLOADSERV")
                            return None
            except Exception as e:
                self.log.error("Failed to open/write file '{}': {}".format(full_path, e),
                               source="DOWNLOADSERV")
                return None

            self.log.debug("File saved successfully → {}".format(full_path),
                           source="DOWNLOADSERV")
            return full_path

        except Exception as e:
            self.log.error("Error during download from '{}': {}".format(url, e),
                           source="DOWNLOADSERV")
            return None
        finally:
            try:
                s.close()
            except Exception:
                # If close fails, we just ignore it; already logged enough.
                pass     
    def help(self):
        print("\n[DownloadHelper Module]")
        print("Description:")
        print("  Provides basic HTTP and HTTPS file download capability using")
        print("  raw sockets, suitable for constrained MicroPython systems.\n")

        print("Methods:\n")

        print("  download_file(url, save_dir='/', save_file=None)")
        print("    Downloads a file from the specified URL and stores it locally.")
        print("    Returns the full file path on success or None on failure.\n")

        print("Notes:")
        print("  - Automatically normalizes malformed URLs.")
        print("  - Supports HTTP and HTTPS connections.")
        print("  - Creates the target directory if it does not exist.")

class Git:
    """
    Git Module
    ------------------
    Lightweight GitHub file fetcher for ZenCMD.

    • Downloads *single raw files* from public GitHub repos
    • Handles spaces and special chars in filenames (URL-encoded)
    • Saves to /SYSTEM32/Admin/ROM/Downloads if available, else falls back to '/'
    • Logs everything via Logger()
    """

    def __init__(self, base_raw=None, default_branch="main"):
        self.logger = Logger()
        # Base URL for raw GitHub content
        self.base_raw = base_raw or "https://raw.githubusercontent.com"
        self.default_branch = default_branch
        # Preferred download directory
        self.default_download_dir = "/SYSTEM32/Downloads"
        self.source = "GITSERV"
        self.token = "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"


    # ------------------------------------------------------------------
    # Public API
    # ------------------------------------------------------------------
    def help(self):
        print("\n[Git Module]")
        print("Description:")
        print("  Lightweight GitHub integration for downloading and uploading")
        print("  individual files using GitHub raw content and Contents API.\n")

        print("Methods:\n")

        print("  download(user, repo, filename, branch=None)")
        print("    Downloads a single file from a public GitHub repository.")
        print("    Supports spaces and special characters in file paths.\n")

        print("  upload(user, repo, local_path, repo_path=None, branch=None, message='Upload from ESP32')")
        print("    Uploads or updates a file in a GitHub repository using the")
        print("    GitHub Contents API with authentication.\n")

        print("Usage Notes:")
        print("  - Download targets raw.githubusercontent.com endpoints.")
        print("  - Upload requires a valid GitHub access token.")
        print("  - Files are saved to '/SYSTEM32/Downloads' if available,")
        print("    otherwise fallback to '/'.\n")

    def download(self, user, repo, filename, branch=None):
        """
        Download a single file from a public GitHub repository.

        Args:
          user     : GitHub username/org
          repo     : Repository name
          filename : File path inside repo (may contain spaces)
          branch   : Branch name, default = self.default_branch

        Returns:
          True on success, False on failure.
        """
        branch = branch or self.default_branch

        # Normalize filename: remove any leading '/'
        filename = (filename or "").lstrip("/")

        # Keep original filename for saving
        original_filename = filename
        encoded_path = self._encode_path(filename)

        url = self._build_url(user, repo, encoded_path, branch)
        self.logger.debug(
            "Git download requested: {} / {} / {} (branch={}) -> {}".format(
                user, repo, filename, branch, url
            ),
            source=self.source,
        )
        print("[GIT] URL:", url)

        # Decide where to save
        target_dir = self._resolve_download_dir(self.default_download_dir)
        fname = original_filename.split("/")[-1]
        full_path = "{}/{}".format(target_dir.rstrip("/"), fname)
        print("[GIT] Save path:", full_path)

        # HTTP GET using requests or urequests
        try:
            try:
                import requests as _req
                print("[GIT] Using 'requests' module")
            except ImportError:
                import urequests as _req   # type: ignore
                print("[GIT] Using 'urequests' module")

            resp = _req.get(url)
        except Exception as e:
            # This is where your '-202' is coming from (OSError(-202))
            self.logger.error(
                "HTTP request failed for URL {}: {!r}".format(url, e),
                source=self.source,
            )
            print("Download failed: HTTP error:", repr(e))
            # If it's an OSError with code -202, give a hint:
            try:
                code = e.args[0]
                if code == -202:
                    print("Hint: OSError(-202) -> check Wi-Fi, DNS, or SSL handshake.")
            except Exception:
                pass
            return False

        # Check HTTP status
        try:
            status = resp.status_code
        except Exception:
            status = 0

        print("[GIT] HTTP status:", status)

        if status != 200:
            self.logger.error(
                "HTTP {} while downloading {} from {}".format(
                    status, filename, url
                ),
                source=self.source,
            )
            print("Download failed: HTTP", status)
            try:
                # Optionally show a tiny snippet of body for debugging
                try:
                    body_preview = resp.text[:120]
                    print("[GIT] Response preview:", body_preview)
                except Exception:
                    pass
                resp.close()
            except Exception:
                pass
            return False

        # Write to file
        try:
            data = resp.content
        except Exception:
            try:
                data = resp.text
                if isinstance(data, str):
                    data = data.encode()
            except Exception as e:
                self.logger.error(
                    "Failed to read HTTP response body: {}".format(e),
                    source=self.source,
                )
                print("Download failed: cannot read response body")
                try:
                    resp.close()
                except Exception:
                    pass
                return False

        try:
            with open(full_path, "wb") as f:
                f.write(data)
        except Exception as e:
            self.logger.error(
                "Failed to write file '{}': {}".format(full_path, e),
                source=self.source,
            )
            print("Download failed: could not save file:", e)
            try:
                resp.close()
            except Exception:
                pass
            return False

        # Clean up response object
        try:
            resp.close()
        except Exception:
            pass

        self.logger.debug(
            "Download completed. Saved as '{}'".format(full_path),
            source=self.source,
        )
        print("File saved as:", full_path)
        return True

    # ------------------------------------------------------------------
    # Internal helpers
    # ------------------------------------------------------------------
    def _encode_path(self, path):
        """
        Percent-encode a GitHub path segment.
        Keeps: A-Z a-z 0-9 - _ . ~ and /.
        Everything else is converted to %HH.

        Example:
          '01. LinearSearch.c' -> '01.%20LinearSearch.c'
        """
        safe = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_.~/"
        out = []
        for ch in path:
            if ch in safe:
                out.append(ch)
            else:
                out.append("%{:02X}".format(ord(ch)))
        return "".join(out)

    def _build_url(self, user, repo, encoded_path, branch):
        """
        Build the full raw.githubusercontent URL.
        """
        # Ensure no leading / in encoded_path
        if encoded_path.startswith("/"):
            encoded_path = encoded_path[1:]

        # base_raw already like "https://raw.githubusercontent.com"
        # Final:
        #   base_raw / user / repo / branch / encoded_path
        parts = [
            self.base_raw.rstrip("/"),
            user,
            repo,
            branch,
            encoded_path,
        ]
        return "/".join(parts)

    def _resolve_download_dir(self, preferred_dir):
        """
        Ensure preferred_dir exists; if not, fall back to '/'.

        Creates missing folders where possible.
        """
        import os

        # Try to create the path chain if needed
        path = preferred_dir
        try:
            parts = path.strip("/").split("/")
            curr = "/"
            for p in parts:
                if not p:
                    continue
                # Does dir exist here?
                try:
                    items = os.listdir(curr)
                except Exception:
                    # Cannot list current path → fallback
                    self.logger.warning(
                        "Cannot access '{}', falling back to '/'".format(curr),
                        source=self.source,
                    )
                    return "/"

                if p not in items:
                    # try to create
                    new_path = curr.rstrip("/") + "/" + p
                    try:
                        os.mkdir(new_path)
                        self.logger.debug(
                            "Created directory '{}' for Git downloads".format(
                                new_path
                            ),
                            source=self.source,
                        )
                    except Exception:
                        # Can't create → fallback
                        self.logger.warning(
                            "Failed to create '{}', falling back to '/'".format(
                                new_path
                            ),
                            source=self.source,
                        )
                        return "/"
                curr = curr.rstrip("/") + "/" + p
            # If we get here, path is valid
            return curr.rstrip("/") or "/"
        except Exception as e:
            self.logger.warning(
                "Error resolving download dir '{}': {}. Falling back to '/'".format(
                    preferred_dir, e
                ),
                source=self.source,
            )
            return "/"

    # ------------------------------------------------------------------
    # NEW: GitHub upload (create/update file via Contents API)
    # ------------------------------------------------------------------
    def upload(self, user, repo, local_path, repo_path=None,
               branch=None, message="Upload from ESP32"):
        """
        Upload (create or update) a file to a GitHub repo using the Contents API.

        Args:
          user       : GitHub username/org (e.g. '11249a165')
          repo       : Repository name (e.g. 'DSA-lab')
          local_path : Path to file on device (e.g. '/SYSTEM32/logs/system_log.txt')
          repo_path  : Path *inside* repo (e.g. 'logs/system_log.txt').
                       Defaults to just the filename from local_path.
          branch     : Branch name, default = self.default_branch
          message    : Commit message for this change

        Returns:
          True on success (HTTP 200/201), False on failure.
        """
        import ubinascii
        token=self.token
        branch = branch or self.default_branch
        if repo_path is None:
            repo_path = local_path.split("/")[-1]

        # Read local file
        try:
            with open(local_path, "rb") as f:
                data = f.read()
        except Exception as e:
            self.logger.error(
                "Git upload: cannot read local file '{}': {}".format(local_path, e),
                source=self.source,
            )
            print("Upload failed: cannot read local file:", e)
            return False

        # Base64 encode (GitHub API requires this)
        try:
            b64 = ubinascii.b2a_base64(data).decode().strip()
        except Exception as e:
            self.logger.error(
                "Git upload: base64 encode failed: {}".format(e),
                source=self.source,
            )
            print("Upload failed: base64 encode error:", e)
            return False

        api_url = "https://api.github.com/repos/{}/{}/contents/{}".format(
            user, repo, repo_path
        )

        # Choose HTTP client
        try:
            try:
                import requests as _req
            except ImportError:
                import urequests as _req   # type: ignore
        except Exception as e:
            self.logger.error(
                "Git upload: cannot import requests/urequests: {}".format(e),
                source=self.source,
            )
            print("Upload failed: HTTP lib missing:", e)
            return False

        headers = {
            "Authorization": "token {}".format(token),
            "User-Agent": "ZenoMicroPC",
            "Accept": "application/vnd.github+json",
        }

        # Optional: check if file already exists to get 'sha'
        sha = None
        try:
            r = _req.get(api_url + "?ref={}".format(branch), headers=headers)
            if r.status_code == 200:
                try:
                    j = r.json()
                    sha = j.get("sha")
                except Exception:
                    sha = None
            r.close()
        except Exception as e:
            # It's okay if GET fails with 404 → new file
            self.logger.debug(
                "Git upload: GET existing file failed/404 (new file maybe): {}".format(e),
                source=self.source,
            )

        body = {
            "message": message,
            "content": b64,
            "branch": branch,
        }
        if sha:
            body["sha"] = sha  # required for updates

        # Actual upload
        try:
            r = _req.put(api_url, headers=headers, json=body)
        except Exception as e:
            self.logger.error(
                "Git upload: PUT request failed: {}".format(e),
                source=self.source,
            )
            print("Upload failed: HTTP PUT error:", e)
            return False

        try:
            status = r.status_code
        except Exception:
            status = 0

        if status not in (200, 201):
            # 201 = created, 200 = updated
            try:
                err_txt = r.text
            except Exception:
                err_txt = "no body"
            self.logger.error(
                "Git upload: HTTP {} from GitHub: {}".format(status, err_txt),
                source=self.source,
            )
            print("Upload failed: HTTP", status)
            try:
                r.close()
            except Exception:
                pass
            return False

        try:
            r.close()
        except Exception:
            pass

        self.logger.debug(
            "Git upload completed: {} -> {}/{} ({})".format(
                local_path, user, repo, repo_path
            ),
            source=self.source,
        )
        print("Upload successful:", repo_path)
        return True


class BluetoothManager:
    def __init__(self, device_name="Zeno Micro PC"):
        self.device_name = device_name
        self.ble = bt.BLE()
        self.ble.active(False)
        self.connected = False
        self.rx_buffer = bytearray()
        self.conn_handle = None
        print("[BT] BluetoothManager initialized for:", self.device_name)

    # --- Internal IRQ handler ---
    def _irq(self, event, data):
        if event == 1:  # Central connected
            self.conn_handle, _, _ = data
            self.connected = True
            print("[BT] Device connected. Handle:", self.conn_handle)
        elif event == 2:  # Central disconnected
            conn_handle, _, _ = data
            self.connected = False
            self.conn_handle = None
            print("[BT] Device disconnected. Advertising restarted...")
            self._advertise()
        elif event == 3:  # GATTS write
            conn_handle, value_handle = data
            value = self.ble.gatts_read(value_handle)
            self.rx_buffer.extend(value)
            print("[BT] Received:", value)

    # --- Enable BLE ---
    def on(self):
        if not self.ble.active():
            self.ble.active(True)
            self.ble.irq(self._irq)
            self._advertise()
            print("[BT] Bluetooth ON. Advertising as:", self.device_name)
        else:
            print("[BT] Already ON.")

    # --- Disable BLE ---
    def off(self):
        try:
            self.ble.active(False)
            self.connected = False
            print("[BT] Bluetooth OFF.")
        except Exception as e:
            print("[BT] Error turning off Bluetooth:", e)

    # --- Start advertising ---
    def _advertise(self, interval_us=500000):
        name = bytes(self.device_name, "utf-8")
        adv_data = bytearray(b"\x02\x01\x06") + bytes((len(name) + 1, 0x09)) + name
        try:
            self.ble.gap_advertise(interval_us, adv_data)
        except Exception as e:
            print("[BT] Advertisement error:", e)

    # --- Scan for devices ---
    def search(self, duration=5):
        print("[BT] Scanning for nearby BLE devices...")
        found = []
        scan_done = False

        def _scan_irq(event, data):
            nonlocal found, scan_done
            if event == bt._IRQ_SCAN_RESULT:
                addr_type, addr, adv_type, rssi, adv_data = data
                # Convert memoryview to bytes safely
                addr_b = bytes(addr)
                adv_b = bytes(adv_data)
                found.append((addr_type, addr_b, adv_type, rssi, adv_b))
                print("  •", addr_b.hex(":"), "RSSI:", rssi)
            elif event == bt._IRQ_SCAN_DONE:
                scan_done = True
                print("[BT] Scan complete. Found:", len(found))

        self.ble.irq(_scan_irq)
        self.ble.active(True)
        self.ble.gap_scan(duration * 1000, 30000, 30000)

        t0 = time.ticks_ms()
        while not scan_done and time.ticks_diff(time.ticks_ms(), t0) < (duration + 2) * 1000:
            time.sleep_ms(100)

        self.ble.gap_scan(None)
        return found

    # --- Connect to a device ---
    def connect(self, addr_type, addr):
        try:
            print("[BT] Connecting to:", addr)
            self.ble.gap_connect(addr_type, addr)
        except Exception as e:
            print("[BT] Connection error:", e)

    # --- Disconnect ---
    def disconnect(self):
        try:
            if self.conn_handle is not None:
                self.ble.gap_disconnect(self.conn_handle)
                self.connected = False
                self.conn_handle = None
                print("[BT] Disconnected.")
            else:
                print("[BT] No active connection.")
        except Exception as e:
            print("[BT] Disconnect error:", e)

    # --- Send data ---
    def send_data(self, data):
        try:
            if not self.connected or self.conn_handle is None:
                print("[BT] No connected device.")
                return
            if isinstance(data, str):
                data = data.encode()
            self.ble.gatts_notify(self.conn_handle, 0, data)
            print("[BT] Data sent:", data)
        except Exception as e:
            print("[BT] Send error:", e)

    # --- Get received data ---
    def get_data(self):
        if self.rx_buffer:
            data = bytes(self.rx_buffer)
            self.rx_buffer = bytearray()
            return data
        return None
    def help(self):
        print("\n[BluetoothManager Module]")
        print("Description:")
        print("  Provides basic Bluetooth Low Energy (BLE) functionality including")
        print("  advertising, scanning, connecting, data transmission, and reception.\n")

        print("Methods:\n")

        print("  on()")
        print("    Enables BLE, registers IRQ handler, and starts advertising.\n")

        print("  off()")
        print("    Disables BLE and clears connection state.\n")

        print("  search(duration=5)")
        print("    Scans for nearby BLE devices for the specified duration (seconds).")
        print("    Returns a list of discovered devices.\n")

        print("  connect(addr_type, addr)")
        print("    Initiates a connection to a BLE device using address type and address.\n")

        print("  disconnect()")
        print("    Disconnects the currently connected BLE device.\n")

        print("  send_data(data)")
        print("    Sends bytes or string data to the connected BLE device.\n")

        print("  get_data()")
        print("    Returns received BLE data from the internal buffer and clears it.\n")
    

class AppInstaller:
    """
    Zeno OS App Installer

    • Uses fixed GitHub user/repo/branch
    • Asks only for app name (without .py)
    • Downloads <app_name>.py from repo
    • Saves into /SYSTEM32/Admin/ROM/APPS
    """

    def __init__(self):
        self.user   = "FerrariForever95"
        self.repo   = "Zeno-Micro-PC"
        self.branch = "main"

        # Folder in GitHub repo where apps live.
        # Example: "APPS"  -> APPS/MyApp.py
        # DO NOT start with '/' here.
        self.remote_base = "APPS"

        # Use your existing Git module, branch fixed to main
        self.git = Git(default_branch=self.branch)

        # Target directory on device
        self.apps_dir = "/SYSTEM32/APPS"

    # -------------------------------------------------
    # Public APIs
    # -------------------------------------------------
    def prompt_and_install(self):
        """
        Ask for app name (no .py), then install.
        """
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
        """
        Build the path inside the GitHub repo:
        remote_base=""      -> "<app_name>.py"
        remote_base="APPS"  -> "APPS/<app_name>.py"
        """
        base = (self.remote_base or "").strip().strip("/")
        if base:
            return f"{base}/{app_name}.py"
        else:
            return f"{app_name}.py"

    def install(self, app_name):
        """
        Install given app_name (string) from your fixed repo/main.
        Returns True on success, False on failure.
        """
        remote_path = self._build_remote_path(app_name)

        print("[APPINST] Installing app:", app_name)

        # Initialize Disk
        try:
            d = Disk()
            d.begin()
        except Exception as e:
            print("[APPINST] WARNING: Disk init failed:", e)

        # Force Git downloads into APPS dir on device
        old_dir = self.git.default_download_dir
        self.git.default_download_dir = self.apps_dir

        try:
            ok = self.git.download(
                self.user,
                self.repo,
                remote_path,
                branch=self.branch,   # always "main"
            )
        finally:
            # restore original dir so we don't surprise other calls
            self.git.default_download_dir = old_dir

        if ok:
            print(
                "[APPINST] App '{}' installed to {}/{}.py".format(
                    app_name, self.apps_dir, app_name
                )
            )
            try:
                time.sleep(1)
            except:
                pass
            print("[APPINST] Restarting system")
        else:
            print("[APPINST] Failed to install app '{}'".format(app_name))

        return ok
    def uninstall(self, name):
        d = Disk()
        d.begin()  # ensure filesystem mounted RW

        path = f"/SYSTEM32/APPS/{name}.py"

        try:
            os.remove(path)
            print("[APPINST] uninstalled", name)
        except OSError as e:
            print("[APPINST] uninstall failed:", e)

    
    def listapps(self):
        APP_PATH = "/SYSTEM32/APPS"
        files = os.listdir(APP_PATH)

# filter .py files only
        apps = [f[:-3] for f in files if f.endswith(".py")]

        print("Detected apps:", apps)
    def help(self):
        print("\n[AppInstaller Module]")
        print("Description:")
        print("  Handles installation and removal of applications for Zeno OS")
        print("  by downloading Python app files from a fixed GitHub repository.\n")

        print("Methods:\n")

        print("  prompt_and_install()")
        print("    Prompts the user for an app name and installs it from the repository.\n")

        print("  install(app_name)")
        print("    Downloads '<app_name>.py' from the configured GitHub repository")
        print("    and installs it into the local APPS directory.\n")

        print("  uninstall(name)")
        print("    Removes the specified application file from the APPS directory.\n")

        print("  listapps()")
        print("    Lists all installed applications detected in the APPS directory.\n")


_HEADERS = {
    "User-Agent": "ZenoOS/1.0",
    "Accept": "application/json",
}

class Wiki:
    def __init__(self, lang="en", width=60, lines=10, out=print):
        self.lang = lang
        self.width = width
        self.lines = lines
        self.out = out

        self.buf = []
        self.pos = 0

    # ------------------------
    # Internal helpers
    # ------------------------
    def _wrap(self, text):
        out = []
        for raw in text.split("\n"):
            s = raw.strip()
            if not s:
                out.append("")
                continue
            while len(s) > self.width:
                cut = s.rfind(" ", 0, self.width)
                if cut < 0:
                    cut = self.width
                out.append(s[:cut])
                s = s[cut:].lstrip()
            out.append(s)
        return out

    # ------------------------
    # Public API
    # ------------------------
    def fetch(self, title, preview_dots=3):
        """
        Fetch article and AUTO-PRINT first preview.
        preview_dots = how many '.' sentences before stopping
        """
        self.buf = []
        self.pos = 0

        url = "https://{}.wikipedia.org/api/rest_v1/page/summary/{}".format(
            self.lang, title.replace(" ", "%20")
        )

        try:
            r = urequests.get(url, headers=_HEADERS)
            if r.status_code != 200:
                self.out("[wiki] http", r.status_code)
                return None

            text = r.json().get("extract", "")
            r.close()

            if not text:
                self.out("[wiki] empty article")
                return None

            self.buf = self._wrap(text)
            gc.collect()

            # ---- AUTO PREVIEW PRINT ----
            dots = 0
            printed = 0

            for line in self.buf:
                self.out(line)
                printed += 1
                dots += line.count(".")
                if dots >= preview_dots or printed >= self.lines:
                    break

            self.pos = printed
            return None   # IMPORTANT: silence ZenCMD

        except Exception as e:
            self.out("[wiki] error:", e)
            return None

    def next(self):
        """
        Print next page.
        """
        if self.pos >= len(self.buf):
            self.out("[wiki] end of article")
            return None

        end = self.pos + self.lines
        for line in self.buf[self.pos:end]:
            self.out(line)

        self.pos = end
        return None

    def search(self, query, n=5):
        """
        Search Wikipedia.
        """
        url = (
            "https://{}.wikipedia.org/w/api.php"
            "?action=query&list=search&format=json&srsearch={}"
        ).format(self.lang, query.replace(" ", "%20"))

        try:
            r = urequests.get(url, headers=_HEADERS)
            data = r.json()
            r.close()

            res = data["query"]["search"][:n]
            for i, item in enumerate(res):
                self.out(i + 1, item["title"])

            return None

        except Exception as e:
            self.out("[wiki] search error:", e)
            return None
    def help(self):
        print("\n[Wiki Module]")
        print("Description:")
        print("  Provides lightweight access to Wikipedia articles using the")
        print("  REST and search APIs, optimized for text-only environments.\n")

        print("Methods:\n")

        print("  fetch(title, preview_dots=3)")
        print("    Fetches a Wikipedia article summary and automatically prints")
        print("    an initial preview based on sentence count or line limit.\n")

        print("  next()")
        print("    Prints the next page of the currently fetched article.\n")

        print("  search(query, n=5)")
        print("    Searches Wikipedia and prints the top matching article titles.\n")

_DB_DIR  = "/SYSTEM32/APPS/Data"
_DB_FILE = _DB_DIR + "/appdb.json"
class AppDB:
    def __init__(self):
        self._data = {}
        self._load()

    # -------------------------
    # Internal
    # -------------------------
    def _load(self):
        try:
            if _DB_DIR not in os.listdir("/SYSTEM32/APPS"):
                os.mkdir(_DB_DIR)
        except:
            pass

        try:
            with open(_DB_FILE, "r") as f:
                self._data = json.load(f)
        except:
            self._data = {}

    def _save(self):
        try:
            with open(_DB_FILE, "w") as f:
                json.dump(self._data, f)
        except Exception as e:
            print("[TinyAppDB] save failed:", e)

    # -------------------------
    # Public API
    # -------------------------
    def set(self, app, key, value):
        app = str(app)
        key = str(key)

        if app not in self._data:
            self._data[app] = {}

        self._data[app][key] = value
        self._save()

    def get(self, app, key, default=None):
        try:
            return self._data.get(app, {}).get(key, default)
        except:
            return default

    def delete(self, app, key):
        try:
            del self._data[app][key]     
            if not self._data[app]:
                del self._data[app]
            self._save()
        except:
            pass

    def clear(self, app):
        if app in self._data:
            del self._data[app]
            self._save()

    def dump(self):
        """Debug: return full DB"""
        return self._data

