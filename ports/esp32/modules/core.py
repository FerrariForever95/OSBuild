"""
syspathmanager.py
=================================================
Shared low-level utility for Zeno OS.

SysPathManager is the ONLY thing exported from here that ZenCMD/Services
are expected to depend on for dynamic package imports. It is intentionally
tiny and dependency-free (stdlib os/sys only) so that a broken
syspathmanager.py is easy to diagnose and easy for 'recover' to replace --
but note that recovery.py deliberately does NOT import this module at all
(see the standalone note in recovery.py). syspathmanager.py is a
convenience for the rest of ZenOS (Services, ZenCMD module loading, etc.),
not a dependency of last resort -- 'recover' and basic shell builtins
(cd/ls/pwd/...) must keep working even if this file is missing, empty, or
corrupted.
=================================================
"""
import sys
import os


class NotADirectoryError(OSError):
    """MicroPython does not define this as a builtin (unlike CPython),
    so it's declared locally here rather than assumed to exist --
    referencing a name that isn't actually a builtin on-device raises a
    NameError instead of the intended exception, which is exactly the
    kind of "missing declaration" failure this class exists to avoid."""
    pass


class SysPathManager:
    """Locates a package folder under /bin, adds it to sys.path if needed,
    and imports the named module from it.

    Every installed package lives at /bin/<folder_name>/<module_name>.py
    (or a package directory of the same name). This class is used by
    Services / ZenCMD to import those on demand -- it does not itself
    fix corrupted or missing packages; 'recover' handles that.
    """

    @staticmethod
    def import_from(base_path, folder_name, module_name):
        """
        Validates the path, updates sys.path, and imports the module.

        Args:
            base_path:   e.g. "/bin"
            folder_name: e.g. "Services" (the package's own folder name)
            module_name: e.g. "Services" (the module to import from that folder)

        Returns:
            The imported module object.

        Raises:
            NotADirectoryError: if base_path/folder_name doesn't exist or
                                 isn't a directory.
            ImportError: if the module can't be found/imported from that
                         folder.
        """
        # 1. Construct the full path (e.g. /bin/Services), collapsing any
        #    accidental double slashes without corrupting a leading "//".
        full_path = "/".join(p for p in "{}/{}".format(base_path, folder_name).split("/") if p)
        if base_path.startswith("/"):
            full_path = "/" + full_path

        # 2. Check the directory actually exists on the filesystem, and
        #    that it IS a directory (not e.g. a stray file of that name).
        try:
            st = os.stat(full_path)
        except OSError:
            raise NotADirectoryError("Directory not found: {}".format(full_path))
        is_dir = (st[0] & 0o170000) == 0o040000  # S_ISDIR, stat.S_IFDIR
        if not is_dir:
            raise NotADirectoryError("Not a directory: {}".format(full_path))

        # 3. Add to sys.path if it is not already there.
        if full_path not in sys.path:
            sys.path.append(full_path)

        # 4. Import and return the module dynamically. Drop any stale
        #    cached copy first so a package that was just repaired by
        #    'recover' (or updated by PackageManager) is re-read from
        #    disk rather than served from sys.modules.
        for cached in [k for k in list(sys.modules.keys())
                       if k == module_name or k.startswith(module_name + ".")]:
            del sys.modules[cached]

        try:
            return __import__(module_name)
        except ImportError as e:
            raise ImportError(
                "Could not find '{}' inside '{}': {}".format(module_name, full_path, e)
            )
