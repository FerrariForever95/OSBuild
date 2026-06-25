print("ZenCMD initializing...")

from Services import Network, Disk, downloadhelper, system, Logger, Git, BluetoothManager,ZenZip, BootConfig
import os
import sys
import zeno
# =================================================
# INIT
# =================================================

logger = Logger()
current_path = "/"

MODULES = {
    "net": Network,
    "disk": Disk,
    "downserv": downloadhelper,
    "system": system,
    "log": Logger,
    "git": Git,
    "encrypter": ZenZip,
    "bootmgr": BootConfig,
    "bluetoothmgr": BluetoothManager
}

active_module = None
module_instance = None

# =================================================
# FILE HELPERS (NO os.path)
# =================================================

def _is_file(path):
    try:
        return (os.stat(path)[0] & 0x4000) == 0
    except:
        return False


def _is_dir(path):
    try:
        return (os.stat(path)[0] & 0x4000) != 0
    except:
        return False


def _is_root_py(path):
    # path like "/something.py" but NOT "/dir/something.py"
    return path.startswith("/") and path.count("/") == 1 and path.endswith(".py")


def _is_explicit_admin(path):
    return path in ("/boot.py", "/firmware.py")


# =================================================
# FULL FILESYSTEM SEARCH (ROOT-PY SAFE)
# =================================================

def resolve_program(name, cwd):
    targets = [name]
    if not name.endswith(".py"):
        targets.append(name + ".py")

    # absolute
    if name.startswith("/"):
        for t in targets:
            if _is_file(t):
                if _is_explicit_admin(t) or _is_root_py(t):
                    print("[SYSRUN] Permission denied (admin/root script)")
                    return None
                print("[SYSRUN] Found:", t)
                return t
        return None

    # cwd fast-path
    for t in targets:
        p = cwd + "/" + t if not cwd.endswith("/") else cwd + t
        if _is_file(p):
            if _is_explicit_admin(p) or _is_root_py(p):
                print("[SYSRUN] Permission denied (admin/root script)")
                return None
            print("[SYSRUN] Found:", p)
            return p

    # full filesystem search
    print("[SYSRUN] Searching filesystem...")
    stack = ["/"]
    seen = set()

    while stack:
        base = stack.pop()
        if base in seen:
            continue
        seen.add(base)

        try:
            entries = os.listdir(base)
        except:
            continue

        for e in entries:
            full = base + "/" + e if not base.endswith("/") else base + e

            # block root .py and admin scripts
            if _is_file(full):
                if e in targets:
                    if _is_root_py(full) or _is_explicit_admin(full):
                        continue
                    print("[SYSRUN] Found:", full)
                    return full

            if _is_dir(full):
                stack.append(full)

    return None


# =================================================
# EXECUTION BOUNDARY
# =================================================

def run_python_file(path):
    with open(path, "r") as f:
        code = f.read()

    exec(code, {
        "__name__": "__main__",
        "__file__": path
    })


# =================================================
# UTILS
# =================================================

def convert_arg(arg):
    try:
        if "." in arg:
            return float(arg)
        return int(arg)
    except:
        return arg


def human_size(size):
    if size < 1024:
        return f"{size} B"
    if size < 1024 * 1024:
        return f"{size / 1024:.2f} KB"
    return f"{size / (1024 * 1024):.2f} MB"


def list_dir(path):
    try:
        print("\nDirectory listing for", path)
        for f in os.listdir(path):
            full = path + "/" + f if not path.endswith("/") else path + f

            # hide ONLY root-level .py
            if path == "/" and f.endswith(".py"):
                continue

            try:
                st = os.stat(full)
                is_dir = (st[0] & 0x4000) != 0
                size = st[6] if len(st) > 6 else 0
                print(f"  {f:24} {'<DIR>' if is_dir else human_size(size)}")
            except:
                print(f"  {f:24} <unknown>")
        print()
    except Exception as e:
        print("[LS] Error:", e)


def tree_dir(path, prefix=""):
    try:
        entries = os.listdir(path)
    except:
        print(prefix + "[unreadable]")
        return

    for e in entries:
        full = path + "/" + e if not path.endswith("/") else path + e
        if _is_dir(full):
            print(prefix + "|-- " + e + "/")
            tree_dir(full, prefix + "    ")
        else:
            # hide root-level .py
            if path == "/" and e.endswith(".py"):
                continue
            print(prefix + "|-- " + e)


# =================================================
# START
# =================================================

print("ZenCMD ready. Type 'help' or 'exit'.")
logger.debug("ZenCMD initialized.", source="ZenCMD")

# =================================================
# MAIN LOOP
# =================================================

while True:
    try:
        d = current_path.strip("/") or ""
        prompt = (
            f"ZenCMD/{d}>{active_module}> "
            if active_module else
            f"{zeno.user}/{d}> "
        )

        cmd = input(prompt).strip()
        if not cmd:
            continue

        # ---------------------------------------------
        # SYSRUN
        # ---------------------------------------------
        if cmd.startswith("sysrun "):
            name = cmd.split(" ", 1)[1].strip()
            path = resolve_program(name, current_path)

            if not path:
                continue

            if path.endswith(".py"):
                print("[SYSRUN] Running", path)
                try:
                    run_python_file(path)
                except KeyboardInterrupt:
                    print("\n[SYSRUN] Interrupted")
                except Exception as e:
                    print("[SYSRUN] Program error:", e)
                continue

            try:
                print("[SYSRUN] Opening", path)
                with open(path, "r") as f:
                    print(f.read())
            except KeyboardInterrupt:
                print("\n[SYSRUN] Interrupted")
            except Exception as e:
                print("[SYSRUN] Cannot open file:", e)

            continue

        # ---------------------------------------------
        # EXIT
        # ---------------------------------------------
        if cmd in ("exit", "quit"):
            if active_module:
                active_module = None
                module_instance = None
                print("Exited module.")
                continue
            print("Exiting ZenCMD.")
            break

        # ---------------------------------------------
        # HELP
        # ---------------------------------------------
        if cmd == "help":
            if active_module and module_instance and hasattr(module_instance, "help"):
                module_instance.help()
                continue
            print("\nModules:")
            for m in MODULES:
                print(" ", m)
            continue

        # ---------------------------------------------
        # LS / DIR
        # ---------------------------------------------
        if cmd in ("ls", "dir"):
            list_dir(current_path)
            continue

        # ---------------------------------------------
        # TREE
        # ---------------------------------------------
        if cmd.startswith("tree"):
            parts = cmd.split()
            path = current_path if len(parts) == 1 else (
                parts[1] if parts[1].startswith("/") else current_path + "/" + parts[1]
            )
            if not _is_dir(path):
                print("[TREE] Not a directory")
                continue
            print(path + "/")
            tree_dir(path)
            continue

        # ---------------------------------------------
        # CD
        # ---------------------------------------------
        if cmd.startswith("cd "):
            target = cmd.split(" ", 1)[1]
            new = target if target.startswith("/") else current_path + "/" + target
            os.chdir(new)
            current_path = os.getcwd()
            continue

        # ---------------------------------------------
        # ENTER MODULE
        # ---------------------------------------------
        if cmd.startswith("enter ") or cmd.lower() in MODULES:
            name = cmd.split(" ")[1] if cmd.startswith("enter ") else cmd
            if name.lower() in MODULES:
                module_instance = MODULES[name.lower()]()
                active_module = name.lower()
                print(">>", name)
            else:
                print("No such module")
            continue

        # ---------------------------------------------
        # INSIDE MODULE
        # ---------------------------------------------
        if active_module:
            parts = cmd.split()
            fn = parts[0]
            args = [convert_arg(a) for a in parts[1:]]
            if hasattr(module_instance, fn):
                r = getattr(module_instance, fn)(*args)
                if r is not None:
                    print(r)
            else:
                print("No method:", fn)
            continue

        # ---------------------------------------------
        # GLOBAL MODULE CALL
        # ---------------------------------------------
        parts = cmd.split()
        mod = parts[0].lower()
        if mod not in MODULES:
            print("Unknown command")
            continue

        m = MODULES[mod]()
        if len(parts) == 1:
            print("Module loaded:", mod)
            continue

        fn = parts[1]
        args = [convert_arg(a) for a in parts[2:]]
        if hasattr(m, fn):
            r = getattr(m, fn)(*args)
            if r is not None:
                print(r)
        else:
            print("No method:", fn)

    except KeyboardInterrupt:
        print("\n[ZenCMD] ^C")
        continue

    except Exception as e:
        print("[ZenCMD] Error:", e)
        logger.error(str(e), source="ZenCMD")

