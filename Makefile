
CFLAGS=-std=c11 -Wall -Wno-parentheses
CPPFLAGS=
LDFLAGS=
LDLIBS=

# AVR #
MCU_TARGET=atmega328p
F_CPU=16000000
PROGRAMMER=arduino
PROGRAMMER_BAUD=57600
TTY=/dev/ttyUSB0
SIMFLAGS=-m $(MCU_TARGET) -f $(F_CPU)
SIMPATH=$(HOME)/.local
AVR_CC=avr-gcc
AVR_CFLAGS=$(CFLAGS) -mmcu=$(MCU_TARGET)
AVR_CPPFLAGS=$(CPPFLAGS) -DF_CPU=$(F_CPU) -I$(SIMPATH)/include/
AVR_LDFLAGS=
AVR_LDLIBS=
#######

debug: CFLAGS+=-g
debug: all
nodebug: CPPFLAGS+=-DNDEBUG
nodebug: all

all: isp avr

isp: ft232h_isp
ft232h_isp: LDLIBS+=-lftdi1
ft232h_isp: ft232h_isp.c
	$(CC) -o ft232h_isp ft232h_isp.c $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) $(LDLIBS)

avr: avr/test.elf avr/test.hex
avr/test.o: avr/test.c
	cd avr; \
	$(AVR_CC) -c test.c $(AVR_CFLAGS) $(AVR_CPPFLAGS)
avr/test.elf: avr/test.o
	cd avr; \
	$(AVR_CC) -o test.elf test.o -Wl,-Map,test.map $(AVR_LDFLAGS) $(AVR_LDLIBS); \
	avr-objdump -hS test.elf > test.lst
avr/test.hex: avr/test.o
	cd avr; \
	avr-objcopy -j.text -j.data -Oihex test.o test.hex

avr/sim: avr/test.elf
	simavr $(SIMFLAGS) avr/test.elf
avr/debug: avr/test.elf
	simavr -g $(SIMFLAGS) avr/test.elf
avr/upload: avr/test.hex
	avrdude -v -p $(MCU_TARGET) -c $(PROGRAMMER) -P $(TTY) -b $(PROGRAMMER_BAUD) -D -U flash:w:avr/test.hex:i

clean:
	rm -f ft232h_isp avr/*.o avr/*.lst avr/*.map avr/*.elf avr/*.hex
