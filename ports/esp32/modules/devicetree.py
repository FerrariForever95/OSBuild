"""
devicetree.py -- Device Tree parser for Zeno OS.
Provides a simple way to describe hardware configuration and pin assignments.
Supports loading from JSON files and querying node properties.
"""
import json
import os

class DeviceTree:
    """Simple Device Tree representation."""
    def __init__(self):
        self.nodes = {}
        self.loaded = False

    def load_from_json(self, filepath):
        """Load device tree from a JSON file.

        Args:
            filepath (str): Path to JSON file

        Returns:
            tuple: (message, bool) - Status and success flag
        """
        try:
            with open(filepath, 'r') as f:
                data = json.load(f)
            self.nodes = self._parse_nodes(data)
            self.loaded = True
            return (f"Device tree loaded from {filepath}", True)
        except Exception as e:
            return (f"Failed to load device tree: {e}", False)

    def _parse_nodes(self, data, parent_path=''):
        """Recursively parse nodes from dictionary."""
        nodes = {}
        for key, value in data.items():
            path = f"{parent_path}.{key}" if parent_path else key
            if isinstance(value, dict):
                nodes[path] = {
                    'properties': value.get('properties', {}),
                    'children': self._parse_nodes(value, path)
                }
            else:
                # Leaf node
                nodes[path] = {
                    'value': value,
                    'properties': {},
                    'children': {}
                }
        return nodes

    def get_node(self, path):
        """Get a node by path (e.g., 'board.leds.red').

        Args:
            path (str): Dot-separated path to node

        Returns:
            tuple: (message, dict) - Status and node dict or None
        """
        if not self.loaded:
            return ("Device tree not loaded", None)
        # Simple lookup; could be improved
        node = self.nodes.get(path)
        if node:
            return (f"Node {path} found", node)
        else:
            return (f"Node {path} not found", None)

    def get_property(self, path, prop_name, default=None):
        """Get a property from a node.

        Args:
            path (str): Node path
            prop_name (str): Property name
            default: Default value if not found

        Returns:
            tuple: (message, value) - Status and property value
        """
        msg, node = self.get_node(path)
        if node is None:
            return (msg, default)
        props = node.get('properties', {})
        value = props.get(prop_name, default)
        return (f"Property {prop_name} retrieved", value)

    def list_nodes(self, prefix=''):
        """List all node paths.

        Args:
            prefix (str): Filter nodes by prefix

        Returns:
            tuple: (message, list) - Status and list of paths
        """
        if not self.loaded:
            return ("Device tree not loaded", [])
        paths = [p for p in self.nodes.keys() if p.startswith(prefix)]
        return (f"Found {len(paths)} nodes", paths)

# Global instance for convenience
_dt = DeviceTree()

def load_devicetree(filepath):
    """Load device tree from JSON file.

    Args:
        filepath (str): Path to JSON file

    Returns:
        tuple: (message, bool) - Status and success flag
    """
    return _dt.load_from_json(filepath)

def get_node(path):
    """Get node by path."""
    return _dt.get_node(path)

def get_property(path, prop_name, default=None):
    """Get property from node."""
    return _dt.get_property(path, prop_name, default)

def list_nodes(prefix=''):
    """List node paths."""
    return _dt.list_nodes(prefix)

def help():
    return ("DeviceTree -- Simple device tree parser for Zeno OS\n"
            "Functions: load_devicetree(filepath), get_node(path),\n"
            "get_property(path, prop_name, default=None), list_nodes(prefix='')\n"
            "Loads hardware configuration from JSON and queries node properties.")

def doc():
    return ("DeviceTree Module -- Device Tree parser for Zeno OS\n"
            "===================================================\n"
            "Provides a simple way to describe hardware configuration (pin assignments,\n"
            "device parameters, etc.) using a JSON-based device tree format.\n"
            "Inspired by Linux device tree but simplified for MicroPython.\n"
            "\n"
            "Format:\n"
            "  JSON object where each key is a node name.\n"
            "  Nodes can contain:\n"
            "    - 'properties': dict of key-value pairs\n"
            "    - 'children': nested nodes\n"
            "  Example:\n"
            "  {\n"
            "    \"board\": {\n"
            "      \"properties\": {\n"
            "        \"model\": \"ESP32-S3\"\n"
            "      },\n"
            "      \"leds\": {\n"
            "        \"properties\": {\n"
            "          \"count\": 2\n"
            "        },\n"
            "        \"red\": {\n"
            "          \"properties\": {\n"
            "            \"pin\": 21,\n"
            "            \"active_low\": false\n"
            "          }\n"
            "        },\n"
            "        \"green\": {\n"
            "          \"properties\": {\n"
            "            \"pin\": 22,\n"
            "            \"active_low\": true\n"
            "          }\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "  }\n"
            "\n"
            "Functions:\n"
            "  load_devicetree(filepath)\n"
            "    Load device tree from JSON file.\n"
            "    Args:\n"
            "      filepath (str): Path to JSON file\n"
            "    Returns: (message, bool) - Status and success flag\n"
            "\n"
            "  get_node(path)\n"
            "    Retrieve a node by dot-separated path (e.g., 'board.leds.red').\n"
            "    Args:\n"
            "      path (str): Node path\n"
            "    Returns: (message, dict) - Status and node dict with 'properties' and 'children'\n"
            "\n"
            "  get_property(path, prop_name, default=None)\n"
            "    Get a property value from a node.\n"
            "    Args:\n"
            "      path (str): Node path\n"
            "      prop_name (str): Property name\n"
            "      default: Value to return if not found\n"
            "    Returns: (message, value) - Status and property value\n"
            "\n"
            "  list_nodes(prefix='')\n"
            "    List all node paths, optionally filtered by prefix.\n"
            "    Args:\n"
            "      prefix (str): Filter nodes whose path starts with this string\n"
            "    Returns: (message, list) - Status and list of node paths\n"
            "\n"
            "Usage Example:\n"
            "  > msg, ok = load_devicetree('/etc/devicetree.json')\n"
            "  > if ok:\n"
            "  >     msg, pin = get_property('board.leds.red', 'pin', -1)\n"
            "  >     print(f\"Red LED pin: {pin}\")\n"
            "\n"
            "Note: This is a simplified implementation. For production use,\n"
            "      consider a more robust device tree compiler and runtime.\n"
            )