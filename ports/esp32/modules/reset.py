from firmware import HUIModule
ui=HUIModule()
ui.begin()
import zfs

zfs.format() 
zfs.mount()
with open("/kernel.py", "rb") as f:
    content = f.read()
zfs.write("/kernel.py", content)
print("stored:", zfs.exists("/kernel.py"))
print(zfs.info())
os.remove("/kernel.py")
zfs.umount()
try:
    ui.tft.cleanup()
    ui.begin()
    del ui
    del HUIModule
    import boot
except Exception as e:
    print(e)
    
