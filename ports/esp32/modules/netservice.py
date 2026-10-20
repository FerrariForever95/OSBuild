"""
Network service with profile management
Enhanced for easy profile management and connection handling
All functions return (message, value) tuples
"""

# Network state
_saved_profiles = {}
_current_connection = None
_autoconnect_enabled = False

def connect(ssid, password=None, save=False, priority=0):
    """
    Connect to a Wi-Fi network.

    Args:
        ssid (str): Network SSID
        password (str, optional): Network password
        save (bool): Whether to save this network for future use
        priority (int): Priority of the network profile

    Returns:
        tuple: (message, bool) - Status message and connection success
    """
    global _current_connection

    # If password not provided and we have a saved profile, use it
    if password is None and ssid in _saved_profiles:
        profile = _saved_profiles[ssid]
        password = profile.get('password')
        security_type = profile.get('security_type', "WPA2")

    # Validate inputs
    if not ssid:
        return ("Error: SSID cannot be empty", False)

    if password is None and ssid not in _saved_profiles:
        return (f"Error: No password provided for SSID '{ssid}' and no saved profile found", False)

    # In a real implementation, this would call the underlying network stack
    # For now, we'll simulate the connection attempt

    # Simulate connection (in real implementation, this would interface with hardware)
    _current_connection = {
        'ssid': ssid,
        'security_type': security_type if 'security_type' in locals() else "WPA2",
        'connected_at': None,  # Would be set when actually connected
        'password_provided': password is not None
    }

    # Save profile if requested
    if save and password:
        save_result = save_network_profile(ssid, password,
                                         security_type if 'security_type' in locals() else "WPA2",
                                         priority)
        if not save_result[1]:  # If save failed
            return (f"Connected but failed to save profile: {save_result[0]}", True)

    auth_method = "using saved profile" if (password is None and ssid in _saved_profiles) else "with provided password"
    message = f"Connecting to network '{ssid}' {auth_method}..."
    if password:
        message += f"\n  Security: {security_type if 'security_type' in locals() else 'WPA2'}"
    message += f"\n  Save profile: {'yes' if save else 'no'}"
    if save:
        message += f"\n  Priority: {priority}"

    # In a real implementation, we would return the actual connection result
    # For now, we assume the connection attempt was initiated successfully
    return (message, True)

def disconnect():
    """
    Disconnect from current network.

    Returns:
        tuple: (message, bool) - Status message and disconnection success
    """
    global _current_connection

    if _current_connection is None:
        return ("Not connected to any network", False)

    ssid = _current_connection.get('ssid', 'unknown')
    _current_connection = None

    return (f"Disconnected from network '{ssid}'", True)

def save_network_profile(ssid, password, security_type="WPA2", priority=0):
    """
    Save a network profile.

    Args:
        ssid (str): Network SSID
        password (str): Network password
        security_type (str): Security type (WEP, WPA, WPA2, etc.)
        priority (int): Priority of the network profile

    Returns:
        tuple: (message, bool) - Status message and save success
    """
    global _saved_profiles

    if not ssid or not password:
        return ("Error: SSID and password required", False)

    _saved_profiles[ssid] = {
        'password': password,
        'security_type': security_type,
        'priority': priority
    }

    # In a real implementation, this would save to persistent storage
    # For now, we'll just update the in-memory dictionary

    message = f"Saved WiFi profile for '{ssid}' (security={security_type}, priority={priority})"
    return (message, True)

def remove_profile(ssid):
    """
    Remove a saved network profile.

    Args:
        ssid (str): Network SSID to remove

    Returns:
        tuple: (message, bool) - Status message and removal success
    """
    global _saved_profiles

    if ssid not in _saved_profiles:
        return (f"Error: No profile found for '{ssid}'", False)

    del _saved_profiles[ssid]
    return (f"Removed profile for '{ssid}'", True)

def list_profiles():
    """
    List all saved network profiles.

    Returns:
        tuple: (message, list) - Status message and list of profiles
    """
    global _saved_profiles

    if not _saved_profiles:
        return ("No saved profiles", [])

    profiles_list = []
    for ssid, profile in _saved_profiles.items():
        profiles_list.append({
            'ssid': ssid,
            'security_type': profile.get('security_type', 'Unknown'),
            'priority': profile.get('priority', 0)
        })

    message = f"Found {len(profiles_list)} saved profile(s)"
    return (message, profiles_list)

def get_profile(ssid):
    """
    Get a specific network profile.

    Args:
        ssid (str): Network SSID to retrieve

    Returns:
        tuple: (message, dict) - Status message and profile data (or None if not found)
    """
    global _saved_profiles

    if ssid not in _saved_profiles:
        return (f"Error: No profile found for '{ssid}'", None)

    profile_data = _saved_profiles[ssid].copy()
    profile_data['ssid'] = ssid

    return (f"Retrieved profile for '{ssid}'", profile_data)

def status():
    """
    Show current network status.

    Returns:
        tuple: (message, dict) - Status message and status information
    """
    global _current_connection, _autoconnect_enabled

    if _current_connection is None:
        status_info = {
            "connected": False,
            "ssid": None,
            "connecting": False,
            "autoconnect": _autoconnect_enabled
        }
        message = f"Network status: Disconnected (autoconnect: {'enabled' if _autoconnect_enabled else 'disabled'})"
        return (message, status_info)
    else:
        # Check if we're actually connected or just attempting
        connected = _current_connection.get('connected_at') is not None
        status_info = {
            "connected": connected,
            "ssid": _current_connection.get('ssid'),
            "connecting": not connected,
            "autoconnect": _autoconnect_enabled,
            "security_type": _current_connection.get('security_type', 'Unknown')
        }

        if connected:
            import time
            connected_duration = time.time() - _current_connection.get('connected_at', time.time())
            status_info['connected_duration'] = connected_duration
            message = f"Network status: Connected to '{_current_connection.get('ssid')}' "
            message += f"(security: {status_info['security_type']}, "
            message += f"duration: {int(connected_duration)}s, "
            message += f"autoconnect: {'enabled' if _autoconnect_enabled else 'disabled'})"
        else:
            message = f"Network status: Connecting to '{_current_connection.get('ssid')}' "
            message += f"(autoconnect: {'enabled' if _autoconnect_enabled else 'disabled'})"

        return (message, status_info)

def set_autoconnect(enabled):
    """
    Set auto-connect behavior.

    Args:
        enabled (bool): Whether to enable auto-connect

    Returns:
        tuple: (message, bool) - Status message and success
    """
    global _autoconnect_enabled
    _autoconnect_enabled = bool(enabled)

    status = "enabled" if _autoconnect_enabled else "disabled"
    return (f"Auto-connect {status}", True)

def get_autoconnect():
    """
    Get current auto-connect setting.

    Returns:
        tuple: (message, bool) - Status message and current autoconnect setting
    """
    global _autoconnect_enabled
    return (f"Auto-connect is {'enabled' if _autoconnect_enabled else 'disabled'}", _autoconnect_enabled)

# Version info
__version__ = "3.0.0"
__author__ = "FerrariForever95"
def help():
    return ("Network Service -- Wi-Fi networking with profile management\n"
            "Functions: connect(ssid, password=None, save=False, priority=0), disconnect(),\n"
            "save_network_profile(ssid, password, security_type=\"WPA2\", priority=0),\n"
            "remove_profile(ssid), list_profiles(), get_profile(ssid), status(),\n"
            "set_autoconnect(enabled), get_autoconnect()\n"
            "All functions return (message, value) tuples")


def doc():
    return ("Network Service Module -- Wi-Fi networking with profile management\n"
            "=============================================================\n"
            "Provides Wi-Fi connectivity and profile management for Zeno OS.\n"
            "All functions return (message, value) tuples for consistent API.\n"
            "\n"
            "Functions:\n"
            "  connect(ssid, password=None, save=False, priority=0)\n"
            "    Connect to a Wi-Fi network\n"
            "    Args:\n"
            "      ssid (str): Network SSID\n"
            "      password (str, optional): Network password (uses saved profile if not provided)\n"
            "      save (bool): Whether to save this network for future use\n"
            "      priority (int): Priority of the network profile (default 0)\n"
            "    Returns:\n"
            "      tuple: (message, bool) - Status message and connection success\n"
            "\n"
            "  disconnect()\n"
            "    Disconnect from current network\n"
            "    Returns:\n"
            "      tuple: (message, bool) - Status message and disconnection success\n"
            "\n"
            "  save_network_profile(ssid, password, security_type=\"WPA2\", priority=0)\n"
            "    Save a Wi-Fi network profile\n"
            "    Args:\n"
            "      ssid (str): Network SSID\n"
            "      password (str): Network password\n"
            "      security_type (str): Security type (default \"WPA2\")\n"
            "      priority (int): Priority of the network profile (default 0)\n"
            "    Returns:\n"
            "      tuple: (message, bool) - Status message and save success\n"
            "\n"
            "  remove_profile(ssid)\n"
            "    Remove a saved network profile\n"
            "    Args:\n"
            "      ssid (str): Network SSID to remove\n"
            "    Returns:\n"
            "      tuple: (message, bool) - Status message and removal success\n"
            "\n"
            "  list_profiles()\n"
            "    List all saved network profiles\n"
            "    Returns:\n"
            "      tuple: (message, list) - Status message and list of profile dictionaries\n"
            "\n"
            "  get_profile(ssid)\n"
            "    Retrieve a specific network profile\n"
            "    Args:\n"
            "      ssid (str): Network SSID to retrieve\n"
            "    Returns:\n"
            "      tuple: (message, dict) - Status message and profile dictionary (or None if not found)\n"
            "\n"
            "  status()\n"
            "    Get current network connection status\n"
            "    Returns:\n"
            "      tuple: (message, dict) - Status message and status information dictionary\n"
            "\n"
            "  set_autoconnect(enabled)\n"
            "    Enable or disable automatic connection to known networks\n"
            "    Args:\n"
            "      enabled (bool): Whether to enable auto-connect\n"
            "    Returns:\n"
            "      tuple: (message, bool) - Status message and previous setting\n"
            "\n"
            "  get_autoconnect()\n"
            "    Get the current auto-connect setting\n"
            "    Returns:\n"
            "      tuple: (message, bool) - Status message and current auto-connect setting\n"
            "\n"
            "Usage Example:\n"
            "  > msg, success = connect(\"MyWiFi\", \"password123\", save=True)\n"
            "  > print(msg)  # \"Connected to MyWiFi\" or error message\n"
            "  > msg, profiles = list_profiles()\n"
            "  > for profile in profiles:\n"
            "      print(f\"{profile['ssid']} (priority {profile['priority']})\")\n"
            "\n"
            "See also: help() for concise function reference."
            )
