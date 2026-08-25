# MicroPython ESP32 Port API Reference

This document provides an overview of the APIs available in the MicroPython ESP32 port. It covers the core modules, classes, functions, and constants that you can use to interact with the ESP32 hardware and system features.

## Table of Contents
1. [machine module](#machine-module)
2. [network module](#network-module)
3. [esp32 module](#esp32-module)
4. [os module](#os-module)
5. [Additional Modules](#additional-modules)
6. [ESP-IDF Integration](#esp-idf-integration)

---

## machine module

The `machine` module provides access to low-level hardware functionalities.

### Functions

| Function | Description |
|----------|-------------|
| `freq([value])` | Get or set the CPU frequency in Hz. If `value` is provided, sets the CPU frequency (supported values depend on the chip). |
| `sleep(t=0)` | Enter light sleep. If `t` is given (in milliseconds), sleep for that time; otherwise, sleep indefinitely until a wake event. |
| `deepsleep(t=0)` | Enter deep sleep. If `t` is given (in milliseconds), sleep for that time; otherwise, sleep indefinitely until a wake event. Note: deep sleep requires a wake source to be configured. |
| `reset()` | Perform a soft reset of the chip (restarts MicroPython). |
| `reset_cause()` | Get the reset cause as an integer constant. See [Reset Causes](#reset-causes). |
| `bootloader()` | Enter the bootloader mode to allow reflashing via UART. |
| `wake_reason()` | Get the wake reason as an integer constant. See [Wake Reasons](#wake-reasons). |
| `wake_pins()` | Get the wake pin status as a tuple of (pin_mask, level). |

### Constants

#### Reset Causes
Returned by `machine.reset_cause()`:
- `machine.HARD_RESET`
- `machine.PWRON_RESET`
- `machine.WDT_RESET`
- `machine.DEEPSLEEP_RESET`
- `machine.SOFT_RESET`

#### Wake Reasons
Returned by `machine.wake_reason()` (bitmask, can combine):
- `machine.PIN_WAKE` (GPIO0-39)
- `machine.EXT0_WAKE` (RTC IO)
- `machine.EXT1_WAKE` (RTC IO)
- `machine.TIMER_WAKE`
- `machine.TOUCHPAD_WAKE`
- `machine.ULP_WAKE`

#### Sleep Types (used internally)
- `machine.SLEEP` (light sleep)
- `machine.DEEPSLEEP` (deep sleep)

### Classes

#### Pin
Control individual GPIO pins.

**Constructors:**
- `machine.Pin(id, mode=None, pull=None, *, value, drive, hold)`  
  - `id`: pin number (0-39) or a tuple for bus pins.
  - `mode`: `Pin.IN`, `Pin.OUT`, `Pin.OPEN_DRAIN`, `Pin.ALT`, `Pin.ALT_OPEN_DRAIN`.
  - `pull`: `Pin.PULL_UP`, `Pin.PULL_DOWN`, `None`.
  - `value`: initial output value (if mode is OUT).
  - `drive`: drive strength (`Pin.DRIVE_0` to `Pin.DRIVE_3`).
  - `hold`: enable hold feature (if supported).

**Methods:**
- `init(mode, pull=None, *, value, drive, hold)` – re-initialize the pin.
- `value([x])` – get or set the pin value.
- `on()` – set pin to 1.
- `off()` – set pin to 0.
- `irq(trigger, priority=1, wake=None, hard=False)` – configure interrupt.

**Constants:**
- Pin modes: `Pin.IN`, `Pin.OUT`, `Pin.OPEN_DRAIN`, `Pin.ALT`, `Pin.ALT_OPEN_DRAIN`.
- Pull: `Pin.PULL_UP`, `Pin.PULL_DOWN`, `None`.
- Interrupt trigger: `Pin.IRQ_FALLING`, `Pin.IRQ_RISING`, `Pin.IRQ_LOW_LEVEL`, `Pin.IRQ_HIGH_LEVEL`.

#### Signal
A Pin with optional inversion (useful for active-low signals).

Same API as Pin, plus inversion handling.

#### Timer
Hardware timers (based on ESP32 timer groups).

**Constructor:**
- `machine.Timer(id, ...)` – timer ID depends on chip (usually 0-3).

**Methods:**
- `init(mode, period, callback=None, *, freq, unit)` – configure the timer.
  - `mode`: `Timer.ONE_SHOT` or `Timer.PERIODIC`.
  - `period`: timeout period in the given unit (or use `freq` for frequency).
  - `callback`: function to call on timeout.
  - `unit`: `Timer.MS` or `Timer.US` (default MS).
- `deinit()` – disable the timer.

**Constants:**
- `Timer.ONE_SHOT`, `Timer.PERIODIC`.
- `Timer.MS`, `Timer.US`.

#### RTC
Real-time clock and memory.

**Constructor:**
- `machine.RTC()`

**Methods:**
- `datetime([datetimetuple])` – get or set date/time as (year, month, day, weekday, hours, minutes, seconds, subseconds).
- `memory([data])` – get or set bytes of the RTC memory (512 bytes on ESP32).
- `sync()` – synchronize RTC with external clock (if supported).

#### TouchPad
Capacitive touch sensing (on supported pins).

**Constructor:**
- `machine.TouchPad(pin)` – pin must be a touch-capable GPIO.

**Methods:**
- `read()` – return touch reading (smaller values indicate touch).
- `config([value])` – configure the touch threshold or voltage.

#### ADC
Analog-to-digital converter (ADC1 channels on most ESP32).

**Constructor:**
- `machine.ADC(pin)` – pin must be an ADC-capable GPIO.

**Methods:**
- `read()` – read raw ADC value (0-4095 for 12-bit).
- `read_u16()` – read scaled to 16-bit (0-65535).
- `atten(atten)` – set input attenuation (`ADC.ATTN_0DB`, `ADC.ATTN_2_5DB`, `ADC.ATTN_6DB`, `ADC.ATTN_11DB`).
- `width(width)` – set bit width (`ADC.WIDTH_9BIT`, `ADC.WIDTH_10BIT`, `ADC.WIDTH_11BIT`, `ADC.WIDTH_12BIT`).

#### DAC
Digital-to-analog converter (on GPIO25 and GPIO26).

**Constructor:**
- `machine.DAC(pin)` – pin must be 25 or 26.

**Methods:**
- `write(value)` – write 8-bit value (0-255) to produce voltage.

#### I2C
I2C bus interface (software and hardware).

**Constructor:**
- `machine.I2C(id, scl, sda, freq=400000, timeout=50000)`  
  - `id`: 0 or 1 (selects hardware I2C peripheral, else software).
  - `scl`, `sda`: Pin objects or pin numbers for SCL/SDA.
  - `freq`: clock frequency in Hz.
  - `timeout`: timeout in ticks.

**Methods:**
- `scan()` – scan for slave devices, returning list of addresses.
- `writeto(addr, buf, stop=True)` – write bytes to slave.
- `readfrom(addr, nbytes, stop=True)` – read bytes from slave.
- `mem_read(nbytes, addr, memaddr, *, addrsize=8)` – read from slave memory.
- `mem_write(buf, addr, memaddr, *, addrsize=8)` – write to slave memory.
- `deinit()` – disable the I2C bus.

#### I2S
I2S (Inter-IC Sound) bus for digital audio.

**Constructor:**
- `machine.I2S(id, sck, ws, sd, mode, bits, format, rate, ibuf)`  
  - Refer to ESP-IDF I2S driver for details.

**Methods:**
- `read(buf, nbytes)` – read samples into buffer.
- `write(buf, nbytes)` – write samples from buffer.
- `deinit()` – disable I2S peripheral.

#### SPI
SPI bus interface.

**Constructor:**
- `machine.SPI(id, sck, mosi, miso, ...)` – pins for SCK, MOSI, MISO; optional CS pin.

**Methods:**
- `init(baudrate, polarity, phase, bits, firstbit, sck, mosi, miso, cs)` – configure SPI.
- `read(nbytes, write=0x00)` – read bytes, optionally writing a byte first.
- `write(buf)` – write bytes.
- `write_readinto(write_buf, read_buf)` – simultaneous write and read.
- `deinit()` – disable SPI peripheral.

#### UART
Universal Asynchronous Receiver/Transmitter.

**Constructor:**
- `machine.UART(id, tx, rx, ...)` – pins for TX and RX; optional RTS/CTS.

**Methods:**
- `init(baudrate, bits, parity, stop, timeout, ...)` – configure UART.
- `any()` – return number of bytes waiting in receive buffer.
- `read([nbytes])` – read bytes.
- `readline()` – read line until newline.
- `write(buf)` – write bytes.
- `deinit()` – disable UART peripheral.

#### WDT
Watchdog timer.

**Constructor:**
- `machine.WDT(timeout=5000)` – timeout in milliseconds.

**Methods:**
- `feed()` – feed the watchdog to prevent reset.
- `suspend()` – suspend the WDT.
- `restore()` – restore after suspend.

#### SDCard
SD card interface (via SPI or SDMMC).

**Constructor:**
- `machine.SDCard(slot=2, width=1, ...)` – depends on board.

**Methods:**
- Same as a block device; can be mounted as filesystem.

#### RMT
Remote Control Transceiver (for infrared, etc.).

**Constructor:**
- `machine.RMT(channel, gpio, ...)` – channel 0-7.

**Methods:**
- `config(...)` – configure clock frequency, idle level, etc.
- `write_pulse_items(items)` – send pulse/space items.
- `wait_done()` – wait for transmission to finish.
- `deinit()` – disable RMT channel.

#### LDO
GPIO LDO voltage regulator (if supported).

**Constructor:**
- `machine.LDO(gpio, ...)` – control a GPIO that powers an external LDO.

#### NVS
Non-volatile storage (key-value store).

**Constructor:**
- `machine.NVS(namespace)` – open a namespace.

**Methods:**
- `get_str(key)`, `get_i32(key)`, `get_u64(key)`, etc.
- `set_str(key, value)`, `set_i32(key, value)`, etc.
- `commit()` – save changes.
- `erase(key)`, `erase_all()`.

#### Partition
Access to ESP32 partition table.

**Constructor:**
- `machine.Partition()` – or `machine.Partition(type, subtype)` to find a specific partition.

**Methods:**
- `info()` – get partition info (offset, size, flags, label).
- `read(offset, size)` – read data from partition.
- `write(offset, data)` – write data to partition.

#### PCNT
Pulse Counter unit (for counting edges).

**Constructor:**
- `machine.PCNT(unit, pin, ...)` – unit 0-7.

**Methods:**
- `init(...)` – configure mode, filter, etc.
- `counter()` – get current count.
- `clear()` – reset counter to zero.
- `deinit()` – disable unit.

#### ULP
Ultra Low Power co-processor (for simple tasks during deep sleep).

**Constructor:**
- `machine.ULP()` – load and run ULP code.

**Methods:**
- `load_binary(data, entry_point)` – load ULP binary.
- `start()` – start ULP execution.
- `halt()` – stop ULP.
- `mem_write(addr, data)` – write to ULP memory.
- `mem_read(addr, size)` – read from ULP memory.

---

## network module

Provides network interface capabilities, primarily Wi-Fi.

### WLAN
Wi-Fi station and access-point interface.

**Constructor:**
- `network.WLAN(interface)` – where `interface` is `network.STA_IF` (station) or `network.AP_IF` (access point).

**Methods:**
- `active([is_active])` – enable or disable the interface.
- `connect(ssid, password=None, bssid=None)` – connect to an AP (station mode).
- `disconnect()` – disconnect from AP.
- `isconnected()` – return True if connected.
- `ifconfig([(ip, subnet, gateway, dns)])` – get or set IP configuration.
- `config('param')` – get or set various configuration parameters (e.g., essid, password, channel, hidden, max_clients).
- `status()` – return connection status.
- `scan()` – list available APs (returns list of tuples: (ssid, bssid, channel, RSSI, authmode, hidden)).

### Bluetooth
Bluetooth Low Energy (BLE) support (if enabled via `MICROPY_PY_BLUETOOTH`).

See `bluetooth` module for BLE API.

---

## esp32 module

ESP32-specific features beyond the generic `machine` module.

### Functions

| Function | Description |
|----------|-------------|
| `wake_on_touch(enable)` | Enable or disable wake from touchpad. |
| `wake_on_ext0(pin, level)` | Configure wake from external trigger 0 (RTC pin). |
| `wake_on_ext1(pins, level)` | Configure wake from external trigger 1 (RTC pins, bitmask). |
| `wake_on_ulp(enable)` | Enable or disable wake from ULP program. |
| `wake_on_gpio(pins, level)` | Configure wake from any GPIO (bitmask, level). |
| `gpio_deep_sleep_hold(enable)` | Enable or disable holding GPIO state during deep sleep (if supported). |
| `raw_temperature()` | (ESP32 only) Raw temperature sensor reading. |
| `mcu_temperature()` | (ESP32-S2/S3/C3, etc.) Die temperature in Celsius. |
| `idf_heap_info(capabilities)` | Return heap usage per capabilities (e.g., `esp32.HEAP_DATA`). |
| `idf_task_info()` | Return information about FreeRTOS tasks. |

### Types (Classes)

- `esp32.NVS` – Non-volatile storage (same as `machine.NVS`).
- `esp32.Partition` – Partition table access.
- `esp32.PCNT` – Pulse counter.
- `esp32.RMT` – Remote control transceiver.
- `esp32.ULP` – ULP co-processor.
- `esp32.LDO` – GPIO LDO regulator (if supported).

### Constants

- `esp32.WAKEUP_ANY_HIGH`, `esp32.WAKEUP_ALL_LOW` – for wake level configuration.
- `esp32.HEAP_DATA`, `esp32.HEAP_EXEC` – heap capabilities for `idf_heap_info`.

---

## os module

Standard MicroPython operating system interface.

### Functions

- `os.chdir(path)` – change current directory.
- `os.getcwd()` – get current directory.
- `os.listdir([dir])` – list directory contents.
- `os.mkdir(path)` – create directory.
- `os.remove(path)` – remove file.
- `os.rename(old, new)` – rename file or directory.
- `os.rmdir(path)` – remove empty directory.
- `os.stat(path)` – get file/directory status.
- `os.sync()` – sync all filesystems.
- `os.unlink(path)` – same as `remove`.
- `os.urandom(n)` – return `n` random bytes.

### Filesystem Classes

- `os.VfsFat` – FAT filesystem builder.
- `os.VfsLfs1` – LittleFS v1 builder.
- `os.VfsLfs2` – LittleFS v2 builder.
- Use with `os.mount()` to mount a filesystem.

Example:
```python
import os
bdev = machine.SDCard()
vfs = os.VfsFat(bdev)
os.mount(vfs, '/sd')
```

---

## Additional Modules

Below is a non-exhaustive list of other modules available in this build. Refer to their source or use `help()` in the REPL for details.

- `bluetooth` – BLE stack (`Bluetooth` class).
- `espnow` – ESP-NOW peer-to-peer communication.
- `freemem` – memory free information.
- `gc` – garbage collector (`collect()`, `mem_free()`, `mem_alloc()`, `threshold()`).
- `json` – JSON parsing (`loads()`, `dumps()`).
- `micropython` – low-level utilities (`mem_info()`, `qstr_info()`, `stack_use()`, `opt_level()`).
- `network` – as described above.
- `ntime` – time functions (nanosecond precision).
- `onewire` – 1-Wire bus (`OneWire`, `DS18X20`).
- `sdcard` – SD card interface (alternative to `machine.SDCard`).
- `select` – `select()` for polling streams.
- `socket` – socket API (`socket`, `getaddrinfo`, etc.).
- `ssl` – SSL/TLS support (`wrap_socket()`).
- `struct` – pack/unpack binary data (`pack()`, `unpack()`).
- `sys` – system-specific (`stdin`, `stdout`, `stderr`, `version`, `implementation`, `path`, `modules`, `exit()`).
- `threading` – thread support (`Lock`, `RLock`, `Semaphore`, `Timer`).
- `time` – time functions (`time()`, `sleep()`, `sleep_ms()`, `sleep_us()`, `ticks_ms()`, etc.).
- `uzlib` – decompression (`decompress()`).
- `ujson` – micro JSON (`loads()`, `dumps()`).
- `ure` – regular expressions (`compile()`, `match()`).
- `usocket` – socket alias.
- `ussl` – SSL alias.
- `utime` – time aliases (`time()`, `sleep()`, etc.).
- `utimeq` – time equality helpers.

---

## ESP-IDF Integration

While the MicroPython abstractions cover most ESP32 features, you can access ESP-IDF directly via:

- The `esp32.idf_*` functions (heap info, task info).
- Registering callbacks for interrupts via `machine.Pin.irq()`.
- Using the `machine` module classes that map to ESP-IDF drivers (I2C, SPI, UART, etc.).
- For deeper integration, you can write a custom C module or modify the port.

To inspect ESP-IDF configuration, use `make idfmenuconfig` in the port directory.

---

## Example Usage

```python
import machine
import network
import time

# Connect to Wi-Fi
sta = network.WLAN(network.STA_IF)
sta.active(True)
sta.connect('your-ssid', 'your-password')
while not sta.isconnected():
    time.sleep_ms(500)
print('Connected:', sta.ifconfig())

# Blink an LED
led = machine.Pin(2, machine.Pin.OUT)
while True:
    led.toggle()
    time.sleep_ms(500)
```

---

## Further Reading

- Source code: `/home/shanmukh/OSBuild/ports/esp32/`
- MicroPython docs: http://docs.micropython.org/
- ESP-IDF Programming Guide: https://docs.espressif.com/projects/esp-idf/en/latest/
