###############################################################################
# Makefile for the project basis
###############################################################################
# Source
# Name your Project
PROJECT = basis

# Sourcefiles
SOURCE =main.c
# Files from basis
SOURCE += adc.c timer.c pwm.c uart.c servo.c
# Libraries
LIBS = -lm 

# Optimization
# O0 - no optimization
# O1 and 02 - optimize for speed
# O3 - faster but not always correct
# Os - optimize for programmsize (default)
OPT=Os

#keep the next line
all:
 
# Programmer
# Port for the programmer
SERPORT = /dev/ttyS0
DIAMEXPORT = /dev/ttyACM0 #diamex usb

#select programmer
#prog: prog-usb
#prog: prog-ser
#prog: prog-funk
prog: prog-diamexusb


# Hardware
MCU   = atmega168
F_OSC = 16000000L

# How to use this Makefile:
# 'make all' Compiles everything
# 'make prog' downloads programm to the MC
# 'make clean' deletes objectfiles
# 'make file.lss' dissambles file.o


# NO CHANGES BELOW THIS LINE!
###############################################################################

## General Flags
TARGET = $(PROJECT)
#CC = avr-gcc.exe
CC = avr-gcc

#
# Paths
#

OBJDIR = obj
BITDIR = bit
OBJ = $(patsubst %.c, $(OBJDIR)/%.o, $(SOURCE))


## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU) -DF_CPU=$(F_OSC)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON) $(INC)
CFLAGS += -Wall -$(OPT) -fsigned-char
CFLAGS += -MD -MP -MT $@ -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS += 


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0



## Objects that must be built in order to link
OBJECTS = $(OBJ) 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(OBJDIR) $(TARGET).elf $(TARGET).hex $(TARGET).eep 
#size

## Compile
$(OBJDIR):
	@mkdir $(OBJDIR)

$(OBJDIR)/%.o: %.c
	@echo Compiling $< ...
	@$(CC) $(INCLUDES) $(CFLAGS) -c  $< -o $@

##Link
$(TARGET).elf: $(OBJECTS)
	 @echo Linking $@ ...
	 @$(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET).elf

%.hex: $(TARGET).elf
	@echo Creating $@ ...
	@avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: $(TARGET).elf
	@echo Creating $@ ...
	@avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@

%.lss: $(TARGET).elf
	@echo Creating $@ ...
	@avr-objdump -h -S $< > $@

size: $(TARGET).elf
	@echo size ${TARGET}.elf
	@avr-size --mcu=${MCU} ${TARGET}.elf

## Clean target
.PHONY: clean prog 
clean:
	rm -rf $(OBJECTS) dep/ obj/ $(TARGET).eep $(TARGET).hex $(TARGET).elf

## Other dependencies
-include $(shell mkdir dep 2>/dev/null) $(wildcard dep/*)


prog-usb: all #original avr programmer
	avrdude -p $(MCU) -c avrispmkII -P usb -U flash:w:$(TARGET).hex:i
prog-ser: all #pony serial programmer cable (low-budget)
	avrdude -p $(MCU) -c ponyser -P $(SERPORT) -U flash:w:$(TARGET).hex:i
prog-funk: all #funk by jschleic
	avrdude -p $(MCU) -c stk500v2 -P $(SERPORT) -U flash:w:$(TARGET).hex:i
prog-diamexusb: all #usb programmer
	avrdude -p $(MCU) -c stk500v2 -P $(DIAMEXPORT) -U flash:w:$(TARGET).hex:i

.PHONY: fuse
fuse:
ifeq ($(MCU),atmega168)
#	avrdude -p $(MCU) -c stk500v2 -P $(DIAMEXPORT) -U lfuse:w:0xff:m -U hfuse:w:0xd7:m
	avrdude -p $(MCU) -c avrispmkII -P usb -U lfuse:w:0xff:m -U hfuse:w:0xd7:m
else
	@echo Controller $(MCU) for make fuse not supported, only atmega168
endif
