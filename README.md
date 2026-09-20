# Pico-Serial-Communication

A lightweight command-line shell written in C for the Raspberry Pi Pico W using the Pico SDK. The shell communicates through USB serial, accepts commands character by character, and controls the onboard green LED.

## Features

* USB serial communication using the Pico SDK
* Non-blocking character input
* Command echoing
* Backspace support
* Bounded 64-byte input buffer
* Buffer overflow protection
* Command parsing and execution
* Onboard LED control
* Help menu

## Available Commands

| Command   | Description                    |
| --------- | ------------------------------ |
| `help`    | Display available commands     |
| `led_on`  | Turn the onboard green LED on  |
| `led_off` | Turn the onboard green LED off |

## Requirements

* Raspberry Pi Pico W
* USB cable
* Raspberry Pi Pico SDK
* CMake
* C compiler
* Serial terminal

## Project Structure

```text
pico-shell/
├── CMakeLists.txt
├── shell.c
└── README.md
```

## Building

Make sure the Raspberry Pi Pico SDK is installed and configured.

From the project directory, run:

```bash
mkdir build
cd build
cmake ..
make -j4
```

This compiles the project and generates the firmware files in the `build` directory.

The UF2 file will be located at:

```text
build/shell.uf2
```

## Flashing the Firmware

1. Disconnect the Raspberry Pi Pico W from USB.
2. Hold the BOOTSEL button on the Pico W.
3. Connect the Pico W to your computer while holding BOOTSEL.
4. Release the BOOTSEL button.
5. The Pico W should appear as a USB mass storage device named `RPI-RP2`.

Copy the compiled UF2 file to the virtual storage:

```bash
cp build/shell.uf2 /media/$USER/RPI-RP2/
```

Replace the destination path if your system mounts the Pico W somewhere else.

Alternatively, copy `shell.uf2` to the `RPI-RP2` drive using your file manager.

After copying, the Pico W automatically reboots and runs the new firmware.

## Running

Once the Pico W reboots:

1. Open a serial terminal connected to the Pico W's USB serial interface.
2. Wait for the shell initialization message.
3. Type a command and press Enter.

### Example

```text
Shell Initialized. Type a command and press Enter.
> help
Available commands:
  help    - Show this menu
  led_on  - Turn the green onboard LED ON
  led_off - Turn the green onboard LED OFF
> led_on
LED status: ON
> led_off
LED status: OFF
>
```

## How It Works

### Initialization

The program initializes standard I/O using `stdio_init_all()` and initializes the CYW43 wireless chip driver.

It then waits for the USB connection before starting the shell.

### Input Handling

The shell reads characters using `getchar_timeout_us(0)`, a non-blocking input function.

Each received character is processed individually. The shell supports:

* Regular character input
* Enter and carriage return
* Backspace
* EOF
* Buffer overflow detection

### Command Buffer

The shell uses a 64-byte character buffer:

```c
char buffer[64];
size_t buffer_index = 0;
```

The buffer stores the command as the user types. When Enter is pressed, the command is null-terminated and checked against the available commands.

The buffer supports commands up to 63 characters long.

### Command Execution

Commands are compared using `strcmp()`.

```c
if (strcmp(buffer, "help") == 0) {
    // Display help menu
}
```

The shell currently supports three commands:

* `help`
* `led_on`
* `led_off`

### LED Control

The onboard LED is controlled through the CYW43 GPIO interface.

```c
cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
```

A value of `1` turns the LED on, while `0` turns it off.

### Buffer Overflow Protection

The shell checks whether there is enough space in the buffer before storing a character.

```c
if (buffer_index < sizeof(buffer) - 1) {
    buffer[buffer_index++] = (char)rx_byte;
}
```

If the command is too long, the shell displays an error, resets the buffer, and returns to the prompt.

## Technical Details

| Feature          | Implementation                      |
| ---------------- | ----------------------------------- |
| Language         | C                                   |
| Platform         | Raspberry Pi Pico W                 |
| SDK              | Raspberry Pi Pico SDK               |
| Input            | USB serial                          |
| Input method     | Non-blocking character-by-character |
| Buffer size      | 64 bytes                            |
| LED interface    | CYW43 GPIO                          |
| Command matching | `strcmp()`                          |
| String handling  | `string.h`                          |
| Standard I/O     | `stdio.h`                           |

## Future Improvements

* Add a command lookup table using function pointers.
* Support command arguments.
* Implement command history.
* Add tab completion.
* Add more hardware control commands.
* Support hardware UART communication.
* Separate command processing into dedicated functions.
* Add a reusable shell library for other Pico projects.

## License

This project is intended for learning and experimentation with embedded C, serial communication, and Raspberry Pi Pico W development.
