# Silicon Vitals Monitor

A professional bare-metal embedded systems project for the ATmega328P (Arduino Uno) that monitors internal chip health using pure C and avr-gcc.

## Project Overview

This project implements a system that monitors the ATmega328P's internal temperature sensor and calculates VCC voltage using the internal 1.1V bandgap reference. All sensor data is filtered using a Kalman filter to reduce noise, and the system provides a UART-based command-line interface for status reporting.

## Features

- **Bare Metal Implementation**: No Arduino libraries - pure AVR C code
- **Internal Temperature Monitoring**: Uses the ATmega328P's internal temperature sensor (ADC channel 8)
- **VCC Measurement**: Calculates supply voltage using the internal 1.1V bandgap reference
- **Kalman Filtering**: Implements a 1D Kalman filter for optimal noise reduction
- **UART CLI**: Command-line interface via serial communication (9600 baud)
- **Professional Code Structure**: Modular design with separate drivers and math modules

## Hardware Requirements

- ATmega328P microcontroller (Arduino Uno)
- USB connection for programming and serial communication
- No external wiring required - uses only internal sensors

## Project Structure

```
.
├── src/
│   ├── main.c              # Main application logic
│   ├── drivers/
│   │   ├── uart.h          # UART driver header
│   │   ├── uart.c          # UART driver implementation
│   │   ├── adc.h           # ADC driver header
│   │   └── adc.c           # ADC driver implementation
│   └── math/
│       ├── kalman.h        # Kalman filter header
│       └── kalman.c        # Kalman filter implementation
├── Makefile                # Build system
└── README.md              # This file
```

## Building the Project

### Prerequisites

- `avr-gcc` - AVR GCC compiler
- `avr-binutils` - AVR binary utilities
- `avrdude` - AVR programmer utility
- `make` - Build automation tool

### Compilation

```bash
make
```

This will:
1. Compile all source files
2. Link them into an ELF file
3. Generate a HEX file for flashing
4. Display memory usage statistics

### Flashing to Microcontroller

1. Connect your Arduino Uno via USB
2. Identify the COM port (e.g., COM3 on Windows, /dev/ttyUSB0 on Linux)
3. Update `AVRDUDE_PORT` in the Makefile if needed
4. Run:

```bash
make flash
```

## Usage

1. After flashing, open a serial terminal (e.g., PuTTY, Arduino Serial Monitor, or `screen`)
2. Configure terminal for:
   - Baud rate: 9600
   - Data bits: 8
   - Stop bits: 1
   - Parity: None
3. Available commands:
   - `status` - Display current sensor readings (raw temp, filtered temp, VCC)
   - `help` - Show available commands

## Technical Details

### UART Configuration
- Baud rate: 9600
- Frame format: 8N1 (8 data bits, no parity, 1 stop bit)
- RX interrupt enabled for command processing

### ADC Configuration
- Prescaler: 128 (ADC clock = 125kHz)
- Internal 1.1V reference for temperature sensor
- AVCC reference for VCC measurement via bandgap

### Kalman Filter
- Process noise (Q): 0.01 (temperature changes slowly)
- Measurement noise (R): 4.0 (ADC has inherent noise)
- Implements standard predict-update cycle

### Temperature Conversion
The internal temperature sensor uses the formula:
```
T(°C) = (ADC - 324.31) / 1.22
```

## Code Style

- Uses standard `stdint.h` types (`uint8_t`, `uint16_t`, etc.)
- Bitwise register manipulation with defined macros
- No magic numbers - all constants are defined
- Professional commenting and documentation
- Modular, maintainable structure

## Makefile Targets

- `make` or `make all` - Build the project
- `make flash` - Build and flash to microcontroller
- `make erase` - Erase the microcontroller
- `make fuses` - Read fuse bits
- `make clean` - Remove build artifacts
- `make help` - Show help message

## Notes

- The internal temperature sensor is not factory-calibrated and may have significant offset
- For accurate temperature readings, calibration against a known reference is recommended
- The Kalman filter parameters may need tuning based on your specific application requirements
- Update the `AVRDUDE_PORT` variable in the Makefile to match your system's COM port

## License

This project is provided as-is for educational and professional development purposes.

