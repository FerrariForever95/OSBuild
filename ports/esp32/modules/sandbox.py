"""
sandbox.py -- Simple application sandboxing for Zeno OS.
Provides limited execution environments for untrusted code.
"""
import time
import gc

class SandboxViolation(Exception):
    """Exception raised when a sandbox limit is exceeded."""
    pass

class Sandbox:
    """Sandbox for limiting resource usage of a callable."""
    def __init__(self, max_time_ms=None, max_mem_bytes=None, max_instructions=None):
        self.max_time_ms = max_time_ms
        self.max_mem_bytes = max_mem_bytes
        self.max_instructions = max_instructions
        self._start_time = None
        self._start_mem = None

    def _check_limits(self):
        """Check if any limits have been exceeded."""
        now = time.ticks_ms()
        if self.max_time_ms is not None:
            elapsed = time.ticks_diff(now, self._start_time)
            if elapsed > self.max_time_ms:
                raise SandboxViolation(f"Time limit exceeded: {elapsed}ms > {self.max_time_ms}ms")
        if self.max_mem_bytes is not None:
            used = gc.mem_alloc()
            if used > self._start_mem + self.max_mem_bytes:
                raise SandboxViolation(f"Memory limit exceeded: {used} bytes > {self._start_mem + self.max_mem_bytes} bytes")
        # Instruction limit would require cycle counting; not implemented here

    def run(self, func, *args, **kwargs):
        """Execute a function within the sandbox limits.

        Args:
            func (callable): Function to execute
            *args, **kwargs: Arguments to pass to func

        Returns:
            tuple: (message, result) - Status and function result or exception
        """
        self._start_time = time.ticks_ms()
        self._start_mem = gc.mem_alloc()
        try:
            self._check_limits()  # Initial check
            result = func(*args, **kwargs)
            self._check_limits()  # Final check
            return (f"Sandbox execution completed successfully", result)
        except SandboxViolation as e:
            return (f"Sandbox violation: {e}", None)
        except Exception as e:
            return (f"Sandbox execution failed: {e}", None)

    def run_with_timeout(self, func, timeout_ms, *args, **kwargs):
        """Convenience method to set time limit and run."""
        orig_max_time = self.max_time_ms
        self.max_time_ms = timeout_ms
        try:
            return self.run(func, *args, **kwargs)
        finally:
            self.max_time_ms = orig_max_time

    def run_with_memory_limit(self, func, max_mem_bytes, *args, **kwargs):
        """Convenience method to set memory limit and run."""
        orig_max_mem = self.max_mem_bytes
        self.max_mem_bytes = max_mem_bytes
        try:
            return self.run(func, *args, **kwargs)
        finally:
            self.max_mem_bytes = orig_max_mem

def help():
    return ("Sandbox -- Simple application sandboxing for Zeno OS\n"
            "Class: Sandbox(max_time_ms=None, max_mem_bytes=None, max_instructions=None)\n"
            "Methods: run(func, *args, **kwargs), run_with_timeout(...), run_with_memory_limit(...)\n"
            "Exception: SandboxViolation")

def doc():
    return ("Sandbox Module -- Simple application sandboxing for Zeno OS\n"
            "=============================================================\n"
            "Provides a simple sandbox mechanism to limit resource usage (time, memory)\n"
            "of arbitrary callable functions. Useful for running untrusted scripts\n"
            "or plugins with restricted resources.\n"
            "\n"
            "Class:\n"
            "  Sandbox(max_time_ms=None, max_mem_bytes=None, max_instructions=None)\n"
            "    Create a sandbox with optional limits.\n"
            "    Args:\n"
            "      max_time_ms (int): Maximum execution time in milliseconds\n"
            "      max_mem_bytes (int): Maximum additional memory allocation in bytes\n"
            "      max_instructions (int): Maximum instruction count (not implemented)\n"
            "\n"
            "  run(func, *args, **kwargs)\n"
            "    Execute a function within the sandbox limits.\n"
            "    Args:\n"
            "      func (callable): Function to execute\n"
            "      *args, **kwargs: Arguments to pass to func\n"
            "    Returns: (message, result)\n"
            "      On success: message describes success, result is function return\n"
            "      On SandboxViolation: message describes violation, result is None\n"
            "      On other exception: message describes error, result is None\n"
            "\n"
            "  run_with_timeout(func, timeout_ms, *args, **kwargs)\n"
            "    Shortcut for setting a time limit and running.\n"
            "\n"
            "  run_with_memory_limit(func, max_mem_bytes, *args, **kwargs)\n"
            "    Shortcut for setting a memory limit and running.\n"
            "\n"
            "Usage Example:\n"
            "  from sandbox import Sandbox\n"
            "  import time\n"
            "  \n"
            "  def infinite_loop():\n"
            "      while True:\n"
            "          pass\n"
            "  \n"
            "  sb = Sandbox(max_time_ms=1000)  # 1 second limit\n"
            "  msg, result = sb.run(infinite_loop)\n"
            "  print(msg)  # Should indicate time limit exceeded\n"
            "  \n"
            "  def memory_hog():\n"
            "      data = []\n"
            "      for i in range(10000):\n"
            "          data.append(bytearray(1000))  # Allocate ~10MB\n"
            "  \n"
            "  sb2 = Sandbox(max_mem_bytes=50000)  # 50KB limit\n"
            "  msg, result = sb2.run(memory_hog)\n"
            "  print(msg)  # Should indicate memory limit exceeded\n"
            "\n"
            "Note: Memory limit checks allocated memory at start and end; it does not\n"
            "      prevent temporary spikes that are freed before the end. For strict\n"
            "      memory limits, consider periodic checks during execution.\n"
            "      Time limit is checked at start and end; long-running calls may\n"
            "      exceed limit before check. For better accuracy, consider dividing\n"
            "      work into smaller chunks.\n"
            )