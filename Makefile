
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
AVR_FORMAT=ihex
AVR_CC=avr-gcc
AVR_CFLAGS=$(CFLAGS) -mmcu=$(MCU_TARGET)
AVR_CPPFLAGS=$(CPPFLAGS) -DF_CPU=$(F_CPU)
AVR_LDFLAGS=-Wl,--oformat=$(AVR_FORMAT)
AVR_LDLIBS=
#######

debug: CFLAGS+=-g
debug: all
nodebug: CPPFLAGS+=-DNDEBUG
nodebug: all

all: isp avr/test.hex

isp: ft232h_isp
ft232h_isp: LDLIBS+=-lftdi1
ft232h_isp: ft232h_isp.c
	$(CC) -o ft232h_isp ft232h_isp.c $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) $(LDLIBS)

avr/test.hex: avr/test.c
	cd avr; \
	$(AVR_CC) -o test.hex test.c $(AVR_CFLAGS) $(AVR_CPPFLAGS) $(AVR_LDFLAGS) $(AVR_LDLIBS)

upload: avr/test.hex
	avrdude -v -p $(MCU_TARGET) -c $(PROGRAMMER) -P $(TTY) -b $(PROGRAMMER_BAUD) -D -U flash:w:avr/test.hex:i

clean:
	rm -f ft232h_isp avr/*.hex
