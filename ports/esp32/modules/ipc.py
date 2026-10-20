"""
ipc.py -- Inter-Process Communication (IPC) mechanisms for Zeno OS.
Provides message queues, shared memory basics, and signaling.
"""
import _thread
import time
import heapq

class MessageQueue:
    """Thread-safe message queue."""
    def __init__(self, maxsize=0):
        self._queue = []
        self._maxsize = maxsize
        self._lock = _thread.allocate_lock()
        self._not_empty = _thread.allocate_lock()
        self._not_full = _thread.allocate_lock()
        # Initially, not_empty is acquired (queue empty)
        self._not_empty.acquire()
        if maxsize > 0:
            self._not_full.acquire()  # Initially not full if maxsize>0? Actually we want to allow puts until full.
            # We'll manage with condition variables; simpler: use semaphores but we'll do basic locking.

    def put(self, item, block=True, timeout=None):
        """Put an item into the queue."""
        with self._lock:
            if self._maxsize > 0 and len(self._queue) >= self._maxsize:
                if not block:
                    raise QueueFull
                # TODO: implement blocking with timeout - simplified for now
                # For simplicity, we'll just wait until space available (busy wait not ideal)
                while len(self._queue) >= self._maxsize:
                    time.sleep_ms(10)
            self._queue.append(item)
            self._not_empty.release()  # Signal that queue is not empty

    def get(self, block=True, timeout=None):
        """Get an item from the queue."""
        with self._lock:
            if not self._queue:
                if not block:
                    raise QueueEmpty
                # Wait until item available
                while not self._queue:
                    time.sleep_ms(10)
            item = self._queue.pop(0)
            if self._maxsize > 0:
                self._not_full.release()  # Signal that queue is not full
            return item

    def qsize(self):
        """Return approximate queue size."""
        with self._lock:
            return len(self._queue)

    def empty(self):
        """Return True if queue is empty."""
        with self._lock:
            return not self._queue

    def full(self):
        """Return True if queue is full."""
        with self._lock:
            return self._maxsize > 0 and len(self._queue) >= self._maxsize

class QueueEmpty(Exception):
    pass

class QueueFull(Exception):
    pass

class SharedMemory:
    """Simple shared memory block (simulated)."""
    def __init__(self, size=1024):
        self._buf = bytearray(size)
        self._size = size
        self._lock = _thread.allocate_lock()

    def read(self, offset, nbytes):
        """Read nbytes from offset."""
        with self._lock:
            if offset < 0 or offset + nbytes > self._size:
                raise ValueError("Invalid offset or size")
            return bytes(self._buf[offset:offset+nbytes])

    def write(self, offset, data):
        """Write data at offset."""
        with self._lock:
            if offset < 0 or offset + len(data) > self._size:
                raise ValueError("Invalid offset or size")
            self._buf[offset:offset+len(data)] = data

    def size(self):
        """Return size of shared memory block."""
        return self._size

class Semaphore:
    """Simple semaphore."""
    def __init__(self, value=1):
        self._value = value
        self._lock = _thread.allocate_lock()

    def acquire(self, block=True, timeout=None):
        """Acquire semaphore."""
        with self._lock:
            if self._value > 0:
                self._value -= 1
                return True
            if not block:
                return False
            # Simplified blocking: busy wait (not ideal)
            while self._value <= 0:
                time.sleep_ms(10)
            self._value -= 1
            return True

    def release(self):
        """Release semaphore."""
        with self._lock:
            self._value += 1

class Event:
    """Simple event flag."""
    def __init__(self):
        self._flag = False
        self._lock = _thread.allocate_lock()

    def set(self):
        """Set the event flag."""
        with self._lock:
            self._flag = True

    def clear(self):
        """Clear the event flag."""
        with self._lock:
            self._flag = False

    def is_set(self):
        """Return True if event is set."""
        with self._lock:
            return self._flag

    def wait(self, timeout=None):
        """Wait for event to be set."""
        start = time.time()
        while not self._flag:
            if timeout is not None and (time.time() - start) > timeout:
                return False
            time.sleep_ms(10)
        return True

def help():
    return ("IPC -- Inter-Process Communication mechanisms for Zeno OS\n"
            "Classes: MessageQueue, SharedMemory, Semaphore, Event\n"
            "Exceptions: QueueEmpty, QueueFull")

def doc():
    return ("IPC Module -- Inter-Process Communication mechanisms for Zeno OS\n"
            "=================================================================\n"
            "Provides basic IPC primitives for communication between processes or\n"
            "threads in Zeno OS. Built on top of _thread locking primitives.\n"
            "\n"
            "Classes:\n"
            "  MessageQueue(maxsize=0)\n"
            "    Thread-safe FIFO queue.\n"
            "    Methods:\n"
            "      put(item, block=True, timeout=None)\n"
            "      get(block=True, timeout=None)\n"
            "      qsize(), empty(), full()\n"
            "    Exceptions: QueueEmpty, QueueFull\n"
            "\n"
            "  SharedMemory(size=1024)\n"
            "    Simulated shared memory block.\n"
            "    Methods:\n"
            "      read(offset, nbytes) -> bytes\n"
            "      write(offset, data)\n"
            "      size() -> int\n"
            "\n"
            "  Semaphore(value=1)\n"
            "    Counting semaphore.\n"
            "    Methods:\n"
            "      acquire(block=True, timeout=None) -> bool\n"
            "      release()\n"
            "\n"
            "  Event()\n"
            "    Simple event flag.\n"
            "    Methods:\n"
            "      set()\n"
            "      clear()\n"
            "      is_set() -> bool\n"
            "      wait(timeout=None) -> bool\n"
            "\n"
            "Usage Example:\n"
            "  # Producer-consumer with MessageQueue\n"
            "  from ipc import MessageQueue\n"
            "  q = MessageQueue(maxsize=10)\n"
            "  \n"
            "  def producer():\n"
            "      for i in range(5):\n"
            "          q.put(f'item-{i}')\n"
            "          print(f'Produced item-{i}')\n"
            "          time.sleep(0.5)\n"
            "  \n"
            "  def consumer():\n"
            "      for _ in range(5):\n"
            "          item = q.get()\n"
            "          print(f'Consumed {item}')\n"
            "          time.sleep(0.7)\n"
            "  \n"
            "  # Start threads (requires _thread)\n"
            "  import _thread\n"
            "  _thread.start_new_thread(producer, ())\n"
            "  _thread.start_new_thread(consumer, ())\n"
            "\n"
            "  # Shared memory example\n"
            "  shm = SharedMemory(256)\n"
            "  shm.write(0, b'Hello World')\n"
            "  data = shm.read(0, 11)\n"
            "  print(data)  # b'Hello World'\n"
            "\n"
            "Note: Blocking operations with timeout use busy waiting; for production\n"
            "      consider using proper condition variables or hardware primitives.\n"
            )