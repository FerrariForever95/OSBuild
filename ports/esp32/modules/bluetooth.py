"""
bluetooth.py -- Bluetooth Low Energy (BLE) functionality for Zeno OS.
Provides scanning, advertising, and basic GATT client/server capabilities.
"""
import ubluetooth as bt
import time
import ujson as json
from machine import Timer

# BLE event constants
_IRQ_CENTRAL_CONNECT = 1
_IRQ_CENTRAL_DISCONNECT = 2
_IRQ_GATTS_WRITE = 3
_IRQ_GATTS_READ_REQUEST = 4
_IRQ_SCAN_RESULT = 5
_IRQ_SCAN_DONE = 6
_IRQ_PERIPHERAL_CONNECT = 7
_IRQ_PERIPHERAL_DISCONNECT = 8
_IRQ_GATTC_SERVICE_RESULT = 9
_IRQ_GATTC_SERVICE_DONE = 10
_IRQ_GATTC_CHARACTERISTIC_RESULT = 11
_IRQ_GATTC_CHARACTERISTIC_DONE = 12
_IRQ_GATTC_DESCRIPTOR_RESULT = 13
_IRQ_GATTC_DESCRIPTOR_DONE = 14
_IRQ_GATTC_READ_RESULT = 15
_IRQ_GATTC_READ_DONE = 16
_IRQ_GATTC_WRITE_DONE = 17
_IRQ_GATTC_NOTIFY = 18
_IRQ_GATTC_INDICATE = 19

class BLE:
    """BLE controller for Zeno OS."""
    def __init__(self, name="ZenoOS"):
        self._ble = bt.BLE()
        self._ble.active(True)
        self._ble.irq(self._irq_handler)
        self._name = name
        self._connected = False
        self._addr = None
        self._services = {}
        self._callbacks = {
            'connect': [],
            'disconnect': [],
            'scan': [],
            'write': [],
            'notify': []
        }
        self._scan_results = []
        self._conn_handle = None
        self._value = None

    def _irq_handler(self, event, data):
        """Handle BLE events."""
        if event == _IRQ_SCAN_RESULT:
            addr_type, addr, adv_type, rssi, adv_data = data
            self._scan_results.append({
                'addr': ':'.join('%02x' % b for b in addr),
                'addr_type': addr_type,
                'adv_type': adv_type,
                'rssi': rssi,
                'adv_data': adv_data
            })
            for cb in self._callbacks['scan']:
                cb(self._scan_results[-1])
        elif event == _IRQ_SCAN_DONE:
            for cb in self._callbacks['scan']:
                cb(None)  # Signal scan done
        elif event == _IRQ_PERIPHERAL_CONNECT:
            conn_handle, addr_type, addr = data
            self._conn_handle = conn_handle
            self._addr = ':'.join('%02x' % b for b in addr)
            self._connected = True
            for cb in self._callbacks['connect']:
                cb(self._addr)
        elif event == _IRQ_PERIPHERAL_DISCONNECT:
            conn_handle, _, _ = data
            if conn_handle == self._conn_handle:
                self._conn_handle = None
                self._addr = None
                self._connected = False
                for cb in self._callbacks['disconnect']:
                    cb()
        elif event == _IRQ_GATTC_WRITE_DONE:
            conn_handle, value_handle, status = data
            for cb in self._callbacks['write']:
                cb(status == 0)  # True if success
        elif event == _IRQ_GATTC_NOTIFY:
            conn_handle, value_handle, notify_data = data
            for cb in self._callbacks['notify']:
                cb(value_handle, notify_data)

    def scan(self, duration=2000, callback=None):
        """Scan for BLE devices.

        Args:
            duration (int): Scan duration in milliseconds
            callback (function): Optional callback for each scan result

        Returns:
            tuple: (message, list) - Status and list of scan results
        """
        if callback:
            self._callbacks['scan'].append(callback)
        self._ble.gap_scan(duration, 30000, 30000)  # duration, interval, window
        # Results come via callback; we also return immediate status
        return (f"BLE scan started for {duration}ms", [])

    def stop_scan(self):
        """Stop scanning."""
        self._ble.gap_scan(None)
        return ("BLE scan stopped", True)

    def get_scan_results(self):
        """Get accumulated scan results.

        Returns:
            tuple: (message, list) - Status and list of scan results
        """
        return (f"Found {len(self._scan_results)} scan results", self._scan_results)

    def clear_scan_results(self):
        """Clear scan results."""
        self._scan_results = []
        return ("Scan results cleared", True)

    def connect(self, addr, timeout=5000):
        """Connect to a BLE device.

        Args:
            addr (str): MAC address in format 'xx:xx:xx:xx:xx:xx'
            timeout (int): Connection timeout in milliseconds

        Returns:
            tuple: (message, bool) - Status and connection success
        """
        # Convert address to bytes
        addr_bytes = bytes(int(b, 16) for b in addr.split(':'))
        self._ble.gap_connect(0, addr_bytes, timeout)
        # Connection result comes via callback
        return (f"Connecting to {addr}...", True)

    def disconnect(self):
        """Disconnect from current connection."""
        if self._conn_handle is not None:
            self._ble.gap_disconnect(self._conn_handle)
            return ("Disconnecting...", True)
        return ("Not connected", False)

    def is_connected(self):
        """Check if connected to a peripheral.

        Returns:
            tuple: (message, bool) - Status and connection state
        """
        return (f"Connection status: {'connected' if self._connected else 'disconnected'}", self._connected)

    # GATT Server methods
    def register_service(self, uuid, characteristics):
        """Register a GATT service.

        Args:
            uuid (str): Service UUID (16 or 32 bit hex string)
            characteristics (list): List of characteristic dicts with:
                - 'uuid': Characteristic UUID
                - 'props': Properties (read, write, notify, etc.)
                - 'value': Initial value (optional)

        Returns:
            tuple: (message, int) - Status and service handle
        """
        # Simplified implementation; full implementation would require
        # registering services and characteristics with the BLE stack
        # For now, we return a placeholder
        handle = len(self._services) + 1
        self._services[handle] = {
            'uuid': uuid,
            'characteristics': characteristics
        }
        return (f"Service registered with handle {handle}", handle)

    def start_advertising(self, name=None, services=None, interval=100000):
        """Start advertising as a peripheral.

        Args:
            name (str): Device name to advertise
            services (list): List of service UUIDs to advertise
            interval (int): Advertising interval in microseconds

        Returns:
            tuple: (message, bool) - Status and success
        """
        if name is None:
            name = self._name
        # Construct advertising payload
        # Simplified; in practice we'd build proper AD structures
        payload = bt.advertising_payload(
            name=name,
            services=services
        )
        self._ble.gap_advertise(interval, adv_data=payload)
        return (f"Advertising as '{name}'", True)

    def stop_advertising(self):
        """Stop advertising."""
        self._ble.gap_advertise(None)
        return ("Advertising stopped", True)

    # Callback registration
    def on_connect(self, callback):
        """Register a callback for connection events."""
        self._callbacks['connect'].append(callback)

    def on_disconnect(self, callback):
        """Register a callback for disconnection events."""
        self._callbacks['disconnect'].append(callback)

    def on_scan(self, callback):
        """Register a callback for scan results."""
        self._callbacks['scan'].append(callback)

    def on_write(self, callback):
        """Register a callback for write events."""
        self._callbacks['write'].append(callback)

    def on_notify(self, callback):
        """Register a callback for notify/indicate events."""
        self._callbacks['notify'].append(callback)

    def help(self):
        return ("Bluetooth -- BLE functionality for Zeno OS\n"
                "Functions: scan(duration, callback), stop_scan(), get_scan_results(),\n"
                "clear_scan_results(), connect(addr, timeout), disconnect(), is_connected(),\n"
                "register_service(uuid, characteristics), start_advertising(name, services, interval),\n"
                "stop_advertising(), on_connect(callback), on_disconnect(callback), on_scan(callback),\n"
                "on_write(callback), on_notify(callback)\n"
                "All operations use callbacks for asynchronous results.")

    def doc(self):
        return ("Bluetooth Module -- BLE functionality for Zeno OS\n"
                "=====================================================\n"
                "Provides Bluetooth Low Energy scanning, connection, and basic GATT\n"
                "client/server capabilities using the ubluetooth driver.\n"
                "\n"
                "Functions:\n"
                "  scan(duration=2000, callback=None)\n"
                "    Scan for BLE devices.\n"
                "    Args:\n"
                "      duration (int): Scan duration in milliseconds (default 2000)\n"
                "      callback (function): Optional function called for each scan result\n"
                "        with dict containing addr, addr_type, adv_type, rssi, adv_data\n"
                "    Returns: (message, list) - Status and empty list (results via callback)\n"
                "\n"
                "  stop_scan()\n"
                "    Stop an active scan.\n"
                "    Returns: (message, bool) - Status and success flag\n"
                "\n"
                "  get_scan_results()\n"
                "    Get accumulated scan results from the last scan.\n"
                "    Returns: (message, list) - Status and list of scan result dicts\n"
                "\n"
                "  clear_scan_results()\n"
                "    Clear stored scan results.\n"
                "    Returns: (message, bool) - Status and success flag\n"
                "\n"
                "  connect(addr, timeout=5000)\n"
                "    Connect to a BLE peripheral.\n"
                "    Args:\n"
                "      addr (str): MAC address in format 'xx:xx:xx:xx:xx:xx'\n"
                "      timeout (int): Connection timeout in milliseconds (default 5000)\n"
                "    Returns: (message, bool) - Status and immediate success flag\n"
                "      (actual connection result via on_connect callback)\n"
                "\n"
                "  disconnect()\n"
                "    Disconnect from current connection.\n"
                "    Returns: (message, bool) - Status and success flag\n"
                "\n"
                "  is_connected()\n"
                "    Check connection status.\n"
                "    Returns: (message, bool) - Status and connection boolean\n"
                "\n"
                "  register_service(uuid, characteristics)\n"
                "    Register a GATT service (simplified placeholder).\n"
                "    Args:\n"
                "      uuid (str): Service UUID (16 or 32 bit hex)\n"
                "      characteristics (list): List of characteristic dicts\n"
                "    Returns: (message, int) - Status and service handle\n"
                "\n"
                "  start_advertising(name=None, services=None, interval=100000)\n"
                "    Start advertising as a BLE peripheral.\n"
                "    Args:\n"
                "      name (str): Device name to advertise (default device name)\n"
                "      services (list): List of service UUIDs to advertise\n"
                "      interval (int): Advertising interval in microseconds (default 100000)\n"
                "    Returns: (message, bool) - Status and success flag\n"
                "\n"
                "  stop_advertising()\n"
                "    Stop advertising.\n"
                "    Returns: (message, bool) - Status and success flag\n"
                "\n"
                "  Callback Registration:\n"
                "    on_connect(callback)    -- Called when a connection is established\n"
                "    on_disconnect(callback) -- Called when a connection is terminated\n"
                "    on_scan(callback)       -- Called for each scan result during scanning\n"
                "    on_write(callback)      -- Called when a characteristic is written by a central\n"
                "    on_notify(callback)     -- Called when a notification/indication is received\n"
                "\n"
                "Usage Example:\n"
                "  def on_scan(result):\n"
                "      if result:\n"
                "          print(f\"Found device: {result['addr']} RSSI:{result['rssi']}\")\n"
                "  ble = BLE()\n"
                "  ble.on_scan(on_scan)\n"
                "  ble.scan(5000)  # Scan for 5 seconds\n"
                "  time.sleep(5)\n"
                "  ble.stop_scan()\n"
                "\n"
                "  # Connect to a device\n"
                "  def on_connect(addr):\n"
                "      print(f\"Connected to {addr}\")\n"
                "  ble.on_connect(on_connect)\n"
                "  ble.connect('aa:bb:cc:dd:ee:ff')\n"
                "\n"
                "Note: Full GATT client/server implementation requires more complex\n"
                "      interaction with the BLE stack; this module provides a foundation.\n"
                )