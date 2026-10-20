"""
storage.py -- Storage management utilities for Zeno OS.
Provides functions for disk usage, device listing, and basic filesystem management.
"""
import os
import uos  # MicroPython os module

def df():
    """Return disk usage statistics for the filesystem.

    Returns:
        tuple: (message, dict) - Status message and dictionary with keys:
            'total', 'used', 'free', 'percent'
    """
    try:
        # Get filesystem stats for root '/'
        s = uos.statvfs('/')
        block_size = s[0]  # f_bsize
        total_blocks = s[2]  # f_blocks
        free_blocks = s[3]   # f_bfree
        used_blocks = total_blocks - free_blocks

        total = block_size * total_blocks
        used = block_size * used_blocks
        free = block_size * free_blocks
        percent = (used / total * 100) if total > 0 else 0

        stats = {
            'total': total,
            'used': used,
            'free': free,
            'percent': round(percent, 2)
        }
        return ("Storage stats retrieved successfully", stats)
    except Exception as e:
        return (f"Failed to get storage stats: {e}", None)

def du(path='/'):
    """Return disk usage for a given path.

    Args:
        path (str): Path to check (default root)

    Returns:
        tuple: (message, dict) - Status message and dictionary with 'size' and 'files'
    """
    try:
        total_size = 0
        file_count = 0
        if uos.path.isdir(path):
            for entry in uos.ilistdir(path):
                # entry is tuple (name, type, inode, size) - size may be 0 for dirs
                full_path = uos.path.join(path, entry[0])
                try:
                    if entry[1] == 0x8000:  # FILE
                        stat = uos.stat(full_path)
                        total_size += stat[6]  # st_size
                        file_count += 1
                    elif entry[1] == 0x4000:  # DIR
                        sub_size, sub_count = du(full_path)
                        total_size += sub_size
                        file_count += sub_count
                except:
                    # Skip unreadable files
                    pass
        else:
            # It's a file
            stat = uos.stat(path)
            total_size = stat[6]
            file_count = 1
        return (f"Disk usage for {path} calculated", {'size': total_size, 'files': file_count})
    except Exception as e:
        return (f"Failed to calculate disk usage for {path}: {e}", None)

def list_devices():
    """List available block devices.

    Returns:
        tuple: (message, list) - Status message and list of device dicts
    """
    # In MicroPython, block devices may be limited; we check common paths
    devices = []
    try:
        # Check for SD card if available
        if uos.path.exists('/sd'):
            stat = uos.statvfs('/sd')
            devices.append({
                'name': 'sd',
                'path': '/sd',
                'type': 'SD Card',
                'size': stat[0] * stat[2]  # block size * total blocks
            })
        # Flash filesystem
        stat = uos.statvfs('/')
        devices.append({
            'name': 'flash',
            'path': '/',
            'type': 'Internal Flash',
            'size': stat[0] * stat[2]
        })
    except Exception as e:
        return (f"Error listing devices: {e}", [])
    return (f"Found {len(devices)} block devices", devices)

def mount_info():
    """Return mount point information.

    Returns:
        tuple: (message, list) - Status message and list of mount points
    """
    mounts = []
    try:
        # Root is always mounted
        stat = uos.statvfs('/')
        mounts.append({
            'device': 'internal flash',
            'mount_point': '/',
            'filesystem': 'unknown',
            'total': stat[0] * stat[2],
            'used': (stat[2] - stat[3]) * stat[0],
            'available': stat[3] * stat[0]
        })
        # Check for SD card
        if uos.path.exists('/sd'):
            stat = uos.statvfs('/sd')
            mounts.append({
                'device': 'sd card',
                'mount_point': '/sd',
                'filesystem': 'unknown',
                'total': stat[0] * stat[2],
                'used': (stat[2] - stat[3]) * stat[0],
                'available': stat[3] * stat[0]
            })
    except Exception as e:
        return (f"Error getting mount info: {e}", [])
    return (f"Found {len(mounts)} mount points", mounts)

def help():
    return ("Storage -- Storage management utilities for Zeno OS\n"
            "Functions: df(), du(path='/'), list_devices(), mount_info()\n"
            "Provides disk usage statistics and device information.")

def doc():
    return ("Storage Module -- Storage management utilities for Zeno OS\n"
            "=====================================================\n"
            "Provides functions for monitoring storage usage, listing block devices,\n"
            "and retrieving filesystem information.\n"
            "\n"
            "Functions:\n"
            "  df()\n"
            "    Returns disk usage statistics for the filesystem.\n"
            "    Returns: (message, dict) with keys: total, used, free, percent\n"
            "\n"
            "  du(path='/')\n"
            "    Returns disk usage for a given path (recursive for directories).\n"
            "    Args:\n"
            "      path (str): Path to check (default root)\n"
            "    Returns: (message, dict) with keys: size (bytes), files (count)\n"
            "\n"
            "  list_devices()\n"
            "    Lists available block devices (internal flash, SD card if present).\n"
            "    Returns: (message, list) of device dictionaries\n"
            "\n"
            "  mount_info()\n"
            "    Returns information about mounted filesystems.\n"
            "    Returns: (message, list) of mount point dictionaries\n"
            "\n"
            "Usage Example:\n"
            "  > msg, stats = df()\n"
            "  > print(f\"Disk usage: {stats['percent']}%\")\n"
            "  > msg, usage = du('/apps')\n"
            "  > print(f\"Directory size: {usage['size']} bytes\")\n"
            "\n"
            "Note: On MicroPython, detailed device information may be limited.\n"
            "      This module provides the best available information via os.statvfs()."
            )