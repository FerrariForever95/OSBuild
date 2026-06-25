import gc
import vfs
from flashbdev import bdev

try:
    if bdev:
        vfs.mount(bdev, "/")
	import zeno_boot
except OSError:
    import inisetup

    inisetup.setup()

gc.collect()
