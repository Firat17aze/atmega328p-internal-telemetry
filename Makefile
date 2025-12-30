###############################################################################
# Makefile for Silicon Vitals Monitor - ATmega328P Bare Metal Project
# Compiler: avr-gcc
# Programmer: avrdude
###############################################################################

# Project Configuration
PROJECT_NAME = silicon_vitals_monitor
MCU = atmega328p
F_CPU = 16000000UL
BAUD = 9600

# Directories
SRC_DIR = src
DRIVERS_DIR = $(SRC_DIR)/drivers
MATH_DIR = $(SRC_DIR)/math
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj

# Source Files
SOURCES = $(SRC_DIR)/main.c \
          $(DRIVERS_DIR)/uart.c \
          $(DRIVERS_DIR)/adc.c \
          $(MATH_DIR)/kalman.c

# Object Files
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Include Directories
INCLUDES = -I$(SRC_DIR) -I$(DRIVERS_DIR) -I$(MATH_DIR)

# Compiler and Tools
CC = avr-gcc
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump
SIZE = avr-size
AVRDUDE = avrdude

# Compiler Flags
CFLAGS = -mmcu=$(MCU) \
         -DF_CPU=$(F_CPU) \
         -DBAUD=$(BAUD) \
         -Os \
         -Wall \
         -Wextra \
         -Wpedantic \
         -std=c99 \
         -ffunction-sections \
         -fdata-sections \
         $(INCLUDES)

# Linker Flags
LDFLAGS = -mmcu=$(MCU) \
          -Wl,-Map,$(BUILD_DIR)/$(PROJECT_NAME).map \
          -Wl,--gc-sections

# AVRDUDE Configuration (Arduino Uno)
AVRDUDE_PROGRAMMER = arduino
AVRDUDE_PORT = COM3
AVRDUDE_BAUDRATE = 115200
AVRDUDE_FLAGS = -p $(MCU) -c $(AVRDUDE_PROGRAMMER) -P $(AVRDUDE_PORT) -b $(AVRDUDE_BAUDRATE)

# Default target
all: $(BUILD_DIR)/$(PROJECT_NAME).hex

# Create build directories
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)/drivers
	@mkdir -p $(OBJ_DIR)/math

# Compile source files to object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@echo "Compiling $<"
	$(CC) $(CFLAGS) -c $< -o $@

# Link object files to create ELF
$(BUILD_DIR)/$(PROJECT_NAME).elf: $(OBJECTS) | $(OBJ_DIR)
	@echo "Linking $@"
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@
	@echo "Size information:"
	$(SIZE) -C --mcu=$(MCU) $@

# Create Intel HEX file from ELF
$(BUILD_DIR)/$(PROJECT_NAME).hex: $(BUILD_DIR)/$(PROJECT_NAME).elf
	@echo "Creating HEX file"
	$(OBJCOPY) -O ihex -R .eeprom $< $@

# Create assembly listing
$(BUILD_DIR)/$(PROJECT_NAME).lst: $(BUILD_DIR)/$(PROJECT_NAME).elf
	$(OBJDUMP) -d -S $< > $@

# Flash to microcontroller
flash: $(BUILD_DIR)/$(PROJECT_NAME).hex
	@echo "Flashing to $(MCU) on $(AVRDUDE_PORT)..."
	$(AVRDUDE) $(AVRDUDE_FLAGS) -U flash:w:$<:i

# Erase chip
erase:
	@echo "Erasing $(MCU)..."
	$(AVRDUDE) $(AVRDUDE_FLAGS) -e

# Read fuse bits
fuses:
	@echo "Reading fuse bits..."
	$(AVRDUDE) $(AVRDUDE_FLAGS) -v

# Clean build artifacts
clean:
	@echo "Cleaning build artifacts..."
	rm -rf $(BUILD_DIR)

# Show help
help:
	@echo "Silicon Vitals Monitor - Build System"
	@echo "======================================"
	@echo "Targets:"
	@echo "  all      - Build project (default)"
	@echo "  flash    - Build and flash to microcontroller"
	@echo "  erase    - Erase microcontroller"
	@echo "  fuses    - Read fuse bits"
	@echo "  clean    - Remove build artifacts"
	@echo "  help     - Show this help message"
	@echo ""
	@echo "Configuration:"
	@echo "  MCU:      $(MCU)"
	@echo "  F_CPU:    $(F_CPU)"
	@echo "  BAUD:     $(BAUD)"
	@echo "  PORT:     $(AVRDUDE_PORT)"
	@echo ""
	@echo "Note: Update AVRDUDE_PORT in Makefile if your COM port differs"

.PHONY: all flash erase fuses clean help

