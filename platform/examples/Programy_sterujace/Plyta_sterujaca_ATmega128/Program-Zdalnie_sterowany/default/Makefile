###############################################################################
# Makefile for the project Explorer_joystik
###############################################################################

## General Flags
PROJECT = Explorer_joystik
MCU = atmega128
TARGET = Explorer_joystik.elf
CC = avr-gcc.exe

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -std=gnu99 -DF_CPU=16000000UL -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,-Map=Explorer_joystik.map


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Include Directories
INCLUDES = -I"C:\WinAVR\avr\include\avr" -I"C:\WinAVR\avr\include" 

## Objects that must be built in order to link
OBJECTS = Main.o Adc.o motor.o stepper.o timer.o Uart.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) Explorer_joystik.hex Explorer_joystik.eep Explorer_joystik.lss size

## Compile
Main.o: ../Main.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

Adc.o: ../Adc.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

motor.o: ../motor.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

stepper.o: ../stepper.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

timer.o: ../timer.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

Uart.o: ../Uart.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

##Link
$(TARGET): $(OBJECTS)
	 $(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET)

%.hex: $(TARGET)
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: $(TARGET)
	-avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0

%.lss: $(TARGET)
	avr-objdump -h -S $< > $@

size: ${TARGET}
	@echo
	@avr-size -C --mcu=${MCU} ${TARGET}

## Clean target
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) Explorer_joystik.elf dep/* Explorer_joystik.hex Explorer_joystik.eep Explorer_joystik.lss Explorer_joystik.map


## Other dependencies
-include $(shell mkdir dep 2>/dev/null) $(wildcard dep/*)

