"""
diagnostics.py -- Diagnostic and debugging tools for Zeno OS.
Provides system information, memory usage, logs, and health checks.
"""
import os
import uos
import time
import json
import gc
import sys
import machine

def meminfo():
    """Return memory usage statistics.

    Returns:
        tuple: (message, dict) - Status and memory info
    """
    try:
        # MicroPython gc.mem_free() and gc.mem_alloc()
        free = gc.mem_free()
        allocated = gc.mem_alloc()
        total = free + allocated
        # Also get heap info if available
        heap_info = {}
        if hasattr(gc, 'heap_info'):
            heap_info = gc.heap_info()
        info = {
            'total': total,
            'free': free,
            'used': allocated,
            'percent': round((allocated / total * 100) if total > 0 else 0, 2)
        }
        # Add heap info if available
        if heap_info:
            info.update(heap_info)
        return (f"Memory usage retrieved", info)
    except Exception as e:
        return (f"Failed to get memory info: {e}", None)

def diskusage(path='/'):
    """Return disk usage for a given path.

    Args:
        path (str): Path to check (default root)

    Returns:
        tuple: (message, dict) - Status and disk usage
    """
    try:
        s = uos.statvfs(path)
        block_size = s[0]
        total_blocks = s[2]
        free_blocks = s[3]
        used_blocks = total_blocks - free_blocks

        total = block_size * total_blocks
        used = block_size * used_blocks
        free = block_size * free_blocks
        percent = (used / total * 100) if total > 0 else 0

        info = {
            'total': total,
            'used': used,
            'free': free,
            'percent': round(percent, 2)
        }
        return (f"Disk usage for {path}", info)
    except Exception as e:
        return (f"Failed to get disk usage for {path}: {e}", None)

def cpuinfo():
    """Return CPU information.

    Returns:
        tuple: (message, dict) - Status and CPU info
    """
    try:
        info = {
            'platform': sys.platform,
            'machine': machine.__class__.__name__ if hasattr(machine, '__class__') else 'unknown',
            'frequency': machine.freq() if hasattr(machine, 'freq') else None,
            'unique_id': machine.unique_id() if hasattr(machine, 'unique_id') else None
        }
        # Try to get temperature if available
        try:
            if hasattr(machine, 'temperature'):
                info['temperature'] = machine.temperature()
            elif hasattr(esp, 'raw_temperature'):
                info['temperature'] = esp.raw_temperature()
        except:
            pass
        return (f"CPU information retrieved", info)
    except Exception as e:
        return (f"Failed to get CPU info: {e}", None)

def version():
    """Return system version information.

    Returns:
        tuple: (message, dict) - Status and version info
    """
    try:
        info = {
            'python': sys.version,
            'version': sys.version_info,
            'implementation': (sys.implementation.name if hasattr(sys, 'implementation') else 'unknown',
                              getattr(sys.implementation, 'version', (0,0,0)) if hasattr(sys, 'implementation') else None),
            'boot_time': time.time() if hasattr(time, 'time') else None
        }
        return (f"System version", info)
    except Exception as e:
        return (f"Failed to get version info: {e}", None)

def logs(lines=None):
    """Return system logs (if available).

    Args:
        lines (int): Number of lines to return (None for all)

    Returns:
        tuple: (message, list) - Status and log lines
    """
    # Try to read from a log file if exists
    log_path = "/LOGS/system.log"
    try:
        if uos.path.exists(log_path):
            with open(log_path, 'r') as f:
                data = f.read().strip()
                if data:
                    log_lines = data.split('\n')
                    if lines is not None:
                        log_lines = log_lines[-lines:]
                    return (f"Retrieved {len(log_lines)} log lines", log_lines)
                else:
                    return ("Log file empty", [])
        else:
            return ("Log file not found", [])
    except Exception as e:
        return (f"Failed to read logs: {e}", [])

def dmesg(lines=None):
    """Return kernel ring buffer messages (simulated).

    Args:
        lines (int): Number of lines to return (None for all)

    Returns:
        tuple: (message, list) - Status and messages
    """
    # In MicroPython, we might not have direct access to kernel logs.
    # We'll simulate by reading from a file if available, or return empty.
    kmsg_path = "/dev/kmsg"
    try:
        if uos.path.exists(kmsg_path):
            with open(kmsg_path, 'r') as f:
                data = f.read().strip()
                if data:
                    kmsg_lines = data.split('\n')
                    if lines is not None:
                        kmsg_lines = kmsg_lines[-lines:]
                    return (f"Retrieved {len(kmsg_lines)} dmesg lines", kmsg_lines)
                else:
                    return ("dmesg empty", [])
        else:
            # Fallback: return some system info
            info = [
                f"Zeno OS diagnostic info",
                f"Platform: {sys.platform}",
                f"CPU freq: {machine.freq() if hasattr(machine, 'freq') else 'unknown'} Hz",
                f"Memory free: {gc.mem_free()} bytes",
                f"Time: {time.time()}"
            ]
            if lines is not None:
                info = info[-lines:]
            return (f"Simulated dmesg ({len(info)} lines)", info)
    except Exception as e:
        return (f"Failed to read dmesg: {e}", [])

def ps():
    """Return list of processes (if process management available).

    Returns:
        tuple: (message, list) - Status and process list
    """
    try:
        # Try to import scheduler or process module
        from Services import Scheduler
        # We cannot easily get the scheduler instance; this is a placeholder
        return (f"Process listing not directly available", [])
    except Exception as e:
        return (f"Failed to get process list: {e}", [])

def netinfo():
    """Return network interface information.

    Returns:
        tuple: (message, dict) - Status and network info
    """
    try:
        import network
        wlan = network.WLAN(network.STA_IF)
        ap = network.WLAN(network.AP_IF)
        info = {
            'station': {
                'active': wlan.active(),
                'connected': wlan.isconnected() if wlan.active() else False,
                'ifconfig': wlan.ifconfig() if wlan.isconnected() else None,
                'rssi': wlan.status('rssi') if wlan.isconnected() else None
            },
            'access_point': {
                'active': ap.active(),
                'ifconfig': ap.ifconfig() if ap.active() else None
            }
        }
        return (f"Network information retrieved", info)
    except Exception as e:
        return (f"Failed to get network info: {e}", None)

def help():
    return ("Diagnostics -- System diagnostic tools for Zeno OS\n"
            "Functions: meminfo(), diskusage(path='/'), cpuinfo(), version(),\n"
            "logs(lines=None), dmesg(lines=None), ps(), netinfo()\n"
            "Provides memory, disk, CPU, version, logs, processes, and network info.")

def doc():
    return ("Diagnostics Module -- Diagnostic and debugging tools for Zeno OS\n"
            "=================================================================\n"
            "Provides functions for monitoring system health, memory usage, disk usage,\n"
            "CPU information, version, logs, kernel messages, process listing, and\n"
            "network interfaces.\n"
            "\n"
            "Functions:\n"
            "  meminfo()\n"
            "    Returns memory usage statistics.\n"
            "    Returns: (message, dict) with keys: total, free, used, percent\n"
            "    (and additional heap info if available)\n"
            "\n"
            "  diskusage(path='/')\n"
            "    Returns disk usage for a given path.\n"
            "    Args:\n"
            "      path (str): Path to check (default root)\n"
            "    Returns: (message, dict) with keys: total, used, free, percent\n"
            "\n"
            "  cpuinfo()\n"
            "    Returns CPU and system information.\n"
            "    Returns: (message, dict) with keys: platform, machine, frequency,\n"
            "              unique_id, optional temperature\n"
            "\n"
            "  version()\n"
            "    Returns system version information.\n"
            "    Returns: (message, dict) with keys: python, version, implementation,\n"
            "              boot_time\n"
            "\n"
            "  logs(lines=None)\n"
            "    Retrieves lines from the system log file (/LOGS/system.log).\n"
            "    Args:\n"
            "      lines (int): Number of lines to return (None for all)\n"
            "    Returns: (message, list) - Status and list of log lines\n"
            "\n"
            "  dmesg(lines=None)\n"
            "    Retrieves kernel ring buffer messages (simulated if /dev/kmsg not available).\n"
            "    Args:\n"
            "      lines (int): Number of lines to return (None for all)\n"
            "    Returns: (message, list) - Status and list of messages\n"
            "\n"
            "  ps()\n"
            "    Attempts to list active processes (requires process management).\n"
            "    Returns: (message, list) - Status and list of process info\n"
            "\n"
            "  netinfo()\n"
            "    Returns network interface information.\n"
            "    Returns: (message, dict) with station and access point info\n"
            "\n"
            "Usage Example:\n"
            "  > msg, mem = meminfo()\n"
            "  > print(f\"RAM usage: {mem['percent']}%\")\n"
            "  > msg, disk = diskusage('/apps')\n"
            "  > print(f\"Disk usage: {disk['percent']}%\")\n"
            "  > msg, cpu = cpuinfo()\n"
            "  > print(f\"CPU freq: {cpu['frequency']} Hz\")\n"
            "  > msg, log_lines = logs(10)\n"
            "  > for line in log_lines:\n"
            "  >     print(line)\n"
            "\n"
            "Note: Some functions may return simulated or limited information based\n"
            "      on hardware and available modules.\n"
            )