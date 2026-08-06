"""
RecoveryShell.py -- Zeno OS minimal recovery shell.

Meant to be frozen into ESP32 firmware alongside Recovery.py (see
bin/Recovery/Recovery.py). This is deliberately a small subset of
ZenCMD.py's command set -- just enough to look around, fix, and
re-populate a broken filesystem -- not a full shell replacement.

Hard rule: this file must import cleanly and RecoveryShell().run() must
start cleanly even if '/' is completely empty, Services.py doesn't
exist, zeno.py doesn't exist, and nothing has ever been installed.
Every filesystem call goes through plain `os`/`open()` -- never
Services.FileManager -- because FileManager assumes a metadata tree
that plain 'os' access doesn't need and recovery can't depend on.

Anything that touches an optional subsystem (Services, PackageManager,
Recovery itself) is probed lazily, inside the command that needs it,
never at import time or shell-start time. A missing subsystem prints
one clear line and the shell keeps going -- it never crashes and never
raises an exception the user can see.

Usage on-device:

    >>> import RecoveryShell
    >>> RecoveryShell.run()

or, if frozen and you want it auto-available:

    >>> from RecoveryShell import RecoveryShell
    >>> RecoveryShell().run()
"""

import os
import sys
import time


# =====================================================================
# path helpers -- raw os only, no FileManager, no metadata tree
# =====================================================================

def _normalize_path(path):
    parts = []
    for p in (path or "").split("/"):
        if p in ("", "."):
            continue
        if p == "..":
            if parts:
                parts.pop()
            continue
        parts.append(p)
    return "/" + "/".join(parts) if parts else "/"


def _join(parent, name):
    parent = (parent or "/").rstrip("/") or "/"
    return name if parent == "/" else parent + "/" + name


def _exists(path):
    try:
        os.stat(path)
        return True
    except OSError:
        return False


def _is_dir(path):
    try:
        return bool(os.stat(path)[0] & 0x4000)
    except OSError:
        return False


def _size(path):
    try:
        st = os.stat(path)
        return 0 if bool(st[0] & 0x4000) else (st[6] if len(st) > 6 else 0)
    except OSError:
        return 0


def _human_size(n):
    if n < 1024:
        return "{} B".format(n)
    if n < 1024 * 1024:
        return "{:.2f} KB".format(n / 1024)
    return "{:.2f} MB".format(n / (1024 * 1024))


def _mkdirs(path):
    """mkdir -p, raw os only."""
    cur = ""
    for p in [p for p in (path or "").split("/") if p]:
        cur += "/" + p
        if not _exists(cur):
            try:
                os.mkdir(cur)
            except OSError:
                pass


def _rm_recursive(path):
    if _is_dir(path):
        for child in (_listdir_safe(path) or []):
            _rm_recursive(_join(path, child))
        try:
            os.rmdir(path)
        except OSError as e:
            print("[rm] Could not remove directory '{}': {}".format(path, e))
    else:
        try:
            os.remove(path)
        except OSError as e:
            print("[rm] Could not remove '{}': {}".format(path, e))


def _listdir_safe(path):
    try:
        return os.listdir(path)
    except OSError as e:
        print("[ls] Cannot list '{}': {}".format(path, e))
        return None


# =====================================================================
# lazy, optional subsystem access -- probed on demand, never at import
# =====================================================================

def _try_import_recovery():
    try:
        from Recovery import Recovery
        return Recovery
    except ImportError as e:
        print("[recover] Recovery module unavailable: {}".format(e))
        return None


def _try_import_services():
    """Best-effort import of Services.py. Returns None (with a clear
    message) instead of raising if it's missing or broken -- recovery
    must keep working with Services entirely absent."""
    try:
        for stale in [m for m in list(sys.modules.keys()) if m == "Services"]:
            del sys.modules[stale]
        import Services
        return Services
    except Exception as e:
        print("[pkg] Services.py unavailable ({}). 'pkg' needs Services to work; "
              "try 'recover' first.".format(e))
        return None


def _try_get_package_manager():
    Services = _try_import_services()
    if Services is None:
        return None
    PackageManager = getattr(Services, "PackageManager", None)
    if PackageManager is None:
        print("[pkg] Services.py loaded, but has no PackageManager -- can't run 'pkg'.")
        return None
    try:
        return PackageManager()
    except Exception as e:
        print("[pkg] Could not start PackageManager: {}".format(e))
        return None


# =====================================================================
# the shell
# =====================================================================

class RecoveryShell:

    PROMPT = "recovery"

    def __init__(self):
        self.current_path = "/"
        self.running = False
        self._pm = None  # lazily created PackageManager, cached per-session

    # -----------------------------------------------------------
    # path resolution
    # -----------------------------------------------------------
    def _abs(self, path):
        if not path:
            return self.current_path
        if path.startswith("/"):
            combined = path
        else:
            base = self.current_path if self.current_path.endswith("/") else self.current_path + "/"
            combined = base + path
        return _normalize_path(combined)

    # -----------------------------------------------------------
    # main loop
    # -----------------------------------------------------------
    def run(self):
        print("\n=== Zeno OS Recovery Shell ===")
        print("Minimal shell -- ls/cd/cat/pkg/recover and basic file ops only.")
        print("Type 'help' for the command list, 'exit' to quit.\n")

        self.running = True
        while self.running:
            try:
                raw = input("{}:{}> ".format(self.PROMPT, self.current_path)).strip()
            except (KeyboardInterrupt, EOFError):
                print("\n[recovery] Exiting.")
                break

            if not raw:
                continue

            try:
                self._dispatch(raw)
            except (KeyboardInterrupt, EOFError):
                print("\n[recovery] Interrupted.")
            except Exception as e:
                # last-resort net: a single broken command must never
                # kill the shell itself.
                print("[recovery] Command failed: {}".format(e))

    def _dispatch(self, raw):
        parts = raw.split()
        cmd, args = parts[0], parts[1:]

        table = {
            "help":    self._cmd_help,
            "ls":      self._cmd_ls,
            "cd":      self._cmd_cd,
            "pwd":     self._cmd_pwd,
            "cat":     self._cmd_cat,
            "mkdir":   self._cmd_mkdir,
            "rmdir":   self._cmd_rmdir,
            "rm":      self._cmd_rm,
            "cp":      self._cmd_cp,
            "mv":      self._cmd_mv,
            "touch":   self._cmd_touch,
            "stat":    self._cmd_stat,
            "df":      self._cmd_df,
            "free":    self._cmd_free,
            "pkg":     self._cmd_pkg,
            "recover": self._cmd_recover,
            "reboot":  self._cmd_reboot,
            "exit":    self._cmd_exit,
            "quit":    self._cmd_exit,
        }

        fn = table.get(cmd)
        if fn is None:
            print("Unknown command: '{}'. Type 'help' for the list.".format(cmd))
            return
        fn(args)

    # -----------------------------------------------------------
    # commands
    # -----------------------------------------------------------
    def _cmd_help(self, args):
        print("\nRecovery shell -- minimal command set:")
        print("  ls [-l] [path]        List directory contents")
        print("  cd [path]             Change directory (no args -> /)")
        print("  pwd                   Print current directory")
        print("  cat <file>            Print file contents")
        print("  mkdir <dir>           Create a directory (and parents)")
        print("  rmdir <dir>           Remove an empty directory")
        print("  rm <file>             Remove a file")
        print("  rm -r <path>          Remove a file or directory tree")
        print("  cp <src> <dst>        Copy a file")
        print("  mv <src> <dst>        Rename/move a file or directory")
        print("  touch <file>          Create an empty file")
        print("  stat <path>           Show size/type for a path")
        print("  df [path]             Show filesystem free/used space")
        print("  free                  Show RAM free/used (if available)")
        print("  pkg <args...>         Package manager (needs Services.py)")
        print("  recover               Rebuild core packages from pkgtable.json")
        print("  reboot                Reset the device")
        print("  exit / quit           Leave the recovery shell\n")

    def _cmd_ls(self, args):
        long_fmt = "-l" in args
        paths = [a for a in args if not a.startswith("-")]
        path = self._abs(paths[0]) if paths else self.current_path

        entries = _listdir_safe(path)
        if entries is None:
            return

        print("\n{}".format(path))
        print("-" * 40)
        for name in sorted(entries):
            full = _join(path, name)
            is_d = _is_dir(full)
            if long_fmt:
                kind = "<DIR> " if is_d else "      "
                sz = _human_size(_size(full))
                print("  {}{:28} {:>10}".format(kind, name, sz))
            else:
                print("  {}{}".format(name, "/" if is_d else ""))
        print()

    def _cmd_cd(self, args):
        if not args:
            self.current_path = "/"
            return
        target = self._abs(args[0])
        if not _exists(target):
            print("[cd] No such path: {}".format(target))
            return
        if not _is_dir(target):
            print("[cd] Not a directory: {}".format(target))
            return
        self.current_path = target

    def _cmd_pwd(self, args):
        print(self.current_path)

    def _cmd_cat(self, args):
        if not args:
            print("Usage: cat <file>")
            return
        path = self._abs(args[0])
        try:
            with open(path, "r") as f:
                print(f.read())
        except OSError as e:
            print("[cat] Could not read '{}': {}".format(path, e))

    def _cmd_mkdir(self, args):
        if not args:
            print("Usage: mkdir <dir>")
            return
        for d in args:
            path = self._abs(d)
            _mkdirs(path)
            print("Created:", path)

    def _cmd_rmdir(self, args):
        if not args:
            print("Usage: rmdir <dir>")
            return
        path = self._abs(args[0])
        if not _is_dir(path):
            print("[rmdir] Not a directory: {}".format(path))
            return
        contents = _listdir_safe(path)
        if contents:
            print("[rmdir] Directory not empty: {}".format(path))
            return
        try:
            os.rmdir(path)
            print("Removed:", path)
        except OSError as e:
            print("[rmdir] {}".format(e))

    def _cmd_rm(self, args):
        recursive = "-r" in args or "-rf" in args
        targets = [a for a in args if not a.startswith("-")]
        if not targets:
            print("Usage: rm [-r] <path>")
            return
        path = self._abs(targets[0])
        if not _exists(path):
            print("[rm] No such path: {}".format(path))
            return
        if _is_dir(path) and not recursive:
            print("[rm] Is a directory (use 'rm -r' or 'rmdir'): {}".format(path))
            return
        _rm_recursive(path)
        print("Removed:", path)

    def _cmd_cp(self, args):
        if len(args) < 2:
            print("Usage: cp <src> <dst>")
            return
        src, dst = self._abs(args[0]), self._abs(args[1])
        if _is_dir(src):
            print("[cp] Directory copy not supported in recovery shell (files only).")
            return
        try:
            with open(src, "rb") as f_in:
                data = f_in.read()
            with open(dst, "wb") as f_out:
                f_out.write(data)
            print("Copied {} -> {}".format(src, dst))
        except OSError as e:
            print("[cp] {}".format(e))

    def _cmd_mv(self, args):
        if len(args) < 2:
            print("Usage: mv <src> <dst>")
            return
        src, dst = self._abs(args[0]), self._abs(args[1])
        try:
            os.rename(src, dst)
            print("Moved {} -> {}".format(src, dst))
        except OSError as e:
            print("[mv] {}".format(e))

    def _cmd_touch(self, args):
        if not args:
            print("Usage: touch <file>")
            return
        path = self._abs(args[0])
        try:
            if _exists(path):
                with open(path, "a"):
                    pass
            else:
                with open(path, "w"):
                    pass
            print("Touched:", path)
        except OSError as e:
            print("[touch] {}".format(e))

    def _cmd_stat(self, args):
        if not args:
            print("Usage: stat <path>")
            return
        path = self._abs(args[0])
        if not _exists(path):
            print("[stat] No such path: {}".format(path))
            return
        is_d = _is_dir(path)
        print("Path : {}".format(path))
        print("Type : {}".format("directory" if is_d else "file"))
        if not is_d:
            print("Size : {}".format(_human_size(_size(path))))

    def _cmd_df(self, args):
        path = self._abs(args[0]) if args else "/"
        try:
            stats = os.statvfs(path)
            total = stats[2] * stats[0]
            free = stats[3] * stats[0]
            used = total - free
            print("Path  : {}".format(path))
            print("Total : {}".format(_human_size(total)))
            print("Used  : {}".format(_human_size(used)))
            print("Free  : {}".format(_human_size(free)))
        except OSError as e:
            print("[df] Could not stat '{}': {}".format(path, e))

    def _cmd_free(self, args):
        try:
            import gc
            gc.collect()
            free = gc.mem_free()
            alloc = gc.mem_alloc()
            total = free + alloc
            print("RAM total : {}".format(_human_size(total)))
            print("RAM used  : {}".format(_human_size(alloc)))
            print("RAM free  : {}".format(_human_size(free)))
        except Exception as e:
            print("[free] Memory info unavailable: {}".format(e))

    def _cmd_pkg(self, args):
        if not args:
            print("Usage: pkg <install|uninstall|update|list|info|verify> [name]")
            return

        sub, rest = args[0], args[1:]

        if self._pm is None:
            self._pm = _try_get_package_manager()

        if self._pm is not None:
            try:
                if sub == "install" and rest:
                    self._pm.install(rest[0])
                elif sub == "uninstall" and rest:
                    self._pm.uninstall(rest[0])
                elif sub == "update":
                    self._pm.update(rest[0] if rest else None)
                elif sub == "list":
                    self._pm.list()
                elif sub == "info" and rest:
                    self._pm.info(rest[0])
                elif sub == "verify":
                    self._pm.verify()
                else:
                    print("Usage: pkg <install|uninstall|update|list|info|verify> [name]")
            except Exception as e:
                print("[pkg] '{}' failed: {}".format(sub, e))
            return

        # Services.py/PackageManager unavailable -- fall back to a direct,
        # Services-free install straight from pkgtable.json for the one
        # thing that matters most in this state: getting a named package
        # (very likely Services.py itself) back onto the filesystem.
        if sub in ("install", "update") and rest:
            Recovery = _try_import_recovery()
            if Recovery is None:
                print("[pkg] Cannot install '{}' without Services.py or Recovery.py -- "
                      "neither is available.".format(rest[0]))
                return
            print("[pkg] Services.py unavailable -- installing '{}' directly from "
                  "pkgtable.json instead.".format(rest[0]))
            try:
                Recovery().install_package(rest[0])
            except Exception as e:
                print("[pkg] Direct install of '{}' failed: {}".format(rest[0], e))
        else:
            print("[pkg] '{}' needs Services.py, which is unavailable. Try "
                  "'pkg install <name>' (works standalone via Recovery), or "
                  "'recover' to restore core packages including Services.py "
                  "itself.".format(sub))

    def _cmd_recover(self, args):
        Recovery = _try_import_recovery()
        if Recovery is None:
            print("[recover] Nothing to run -- Recovery.py is not importable "
                  "(missing from filesystem and not frozen into firmware).")
            return
        try:
            rec = Recovery()
            rec.run()
        except KeyboardInterrupt:
            print("\n[recover] Interrupted.")
        except Exception as e:
            print("[recover] Recovery failed: {}".format(e))

    def _cmd_reboot(self, args):
        try:
            import machine
            print("[recovery] Rebooting...")
            time.sleep_ms(200)
            machine.reset()
        except Exception as e:
            print("[reboot] Could not reset: {}".format(e))

    def _cmd_exit(self, args):
        self.running = False
        print("[recovery] Bye.")


def run():
    RecoveryShell().run()


if __name__ == "__main__":
    run()
