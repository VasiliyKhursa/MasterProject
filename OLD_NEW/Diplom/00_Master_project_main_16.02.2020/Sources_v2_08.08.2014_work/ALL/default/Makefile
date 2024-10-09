###############################################################################
# Makefile for the project main
###############################################################################

## General Flags
PROJECT = main
MCU = atmega2560
TARGET = main.elf
CC = avr-gcc

CPP = avr-g++

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -std=gnu99                                                                -DF_CPU=12000000UL -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,-Map=main.map


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Include Directories
INCLUDES = -I"E:\university\cnty\master_project\OLD_NEW\Diplom\00_Master_project_main_16.02.2020\Sources_v2_08.08.2014_work\ALL\freertos\Source\include" -I"E:\university\cnty\master_project\OLD_NEW\Diplom\00_Master_project_main_16.02.2020\Sources_v2_08.08.2014_work\ALL\freertos\Source\portable\GCC\ATMega323" -I"E:\university\cnty\master_project\OLD_NEW\Diplom\00_Master_project_main_16.02.2020\Sources_v2_08.08.2014_work\ALL\." -I"E:\university\cnty\master_project\OLD_NEW\Diplom\00_Master_project_main_16.02.2020\Sources_v2_08.08.2014_work\ALL\ff" -I"E:\university\cnty\master_project\OLD_NEW\Diplom\00_Master_project_main_16.02.2020\Sources_v2_08.08.2014_work\ALL\eth" -I"E:\university\cnty\master_project\OLD_NEW\Diplom\00_Master_project_main_16.02.2020\Sources_v2_08.08.2014_work\ALL\adc" -I"E:\university\cnty\master_project\OLD_NEW\Diplom\00_Master_project_main_16.02.2020\Sources_v2_08.08.2014_work\ALL\i2c" -I"E:\university\cnty\master_project\OLD_NEW\Diplom\00_Master_project_main_16.02.2020\Sources_v2_08.08.2014_work\ALL\lcd" -I"E:\university\cnty\master_project\OLD_NEW\Diplom\00_Master_project_main_16.02.2020\Sources_v2_08.08.2014_work\ALL\ls020" -I"E:\university\cnty\master_project\OLD_NEW\Diplom\00_Master_project_main_16.02.2020\Sources_v2_08.08.2014_work\ALL\one-wire" -I"E:\university\cnty\master_project\OLD_NEW\Diplom\00_Master_project_main_16.02.2020\Sources_v2_08.08.2014_work\ALL\spi" -I"E:\university\cnty\master_project\OLD_NEW\Diplom\00_Master_project_main_16.02.2020\Sources_v2_08.08.2014_work\ALL\uart" 

## Objects that must be built in order to link
OBJECTS = main.o Main_ALL.o Main_LAN.o ccsbcs.o diskio.o enc28j60.o ff.o lan.o ntp.o UART_routines.o tasks.o port.o list.o heap_2.o Main_RTOS.o croutine.o queue.o timers.o adc.o adc_ex.o at_command.o ds18x20.o hd44780.o i2c.o lcd.o onewire.o spi.o uart.o main_LS020.o spiLS020.o main_LS020_2.o ls020.o ds1307.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) main.hex main.eep main.lss size

## Compile
main.o: ../main.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

Main_ALL.o: ../Main_ALL.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

Main_LAN.o: ../Main_LAN.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

ccsbcs.o: ../ff/ccsbcs.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

diskio.o: ../ff/diskio.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

enc28j60.o: ../eth/enc28j60.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

ff.o: ../ff/ff.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

lan.o: ../eth/lan.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

ntp.o: ../eth/ntp.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

UART_routines.o: ../ff/UART_routines.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

tasks.o: ../freertos/Source/tasks.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

port.o: ../freertos/Source/portable/GCC/ATMega323/port.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

list.o: ../freertos/Source/list.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

heap_2.o: ../freertos/Source/portable/MemMang/heap_2.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

Main_RTOS.o: ../Main_RTOS.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

croutine.o: ../freertos/Source/croutine.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

queue.o: ../freertos/Source/queue.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

timers.o: ../freertos/Source/timers.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

adc.o: ../adc/adc.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

adc_ex.o: ../adc/adc_ex.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

at_command.o: ../uart/at_command.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

ds18x20.o: ../one-wire/ds18x20.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

hd44780.o: ../lcd/hd44780.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

i2c.o: ../i2c/i2c.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

lcd.o: ../lcd/lcd.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

onewire.o: ../one-wire/onewire.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

spi.o: ../spi/spi.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

uart.o: ../uart/uart.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

main_LS020.o: ../ls020/main_LS020.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

spiLS020.o: ../ls020/spiLS020.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

main_LS020_2.o: ../ls020/main_LS020_2.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

ls020.o: ../ls020/ls020.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

ds1307.o: ../i2c/ds1307.c
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
	-rm -rf $(OBJECTS) main.elf dep/* main.hex main.eep main.lss main.map


## Other dependencies
-include $(shell mkdir dep 2>NUL) $(wildcard dep/*)

