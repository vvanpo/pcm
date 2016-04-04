
MCU_TARGET=atmega328p
F_CPU=16000000UL

PROGRAMMER=arduino
TTY=/dev/ttyUSB0

CC=avr-gcc
OBJCOPY=avr-objcopy
CFLAGS=-std=c11 -Wall -Wno-parentheses -mmcu=$(MCU_TARGET) -g
CPPFLAGS=-DF_CPU=$(F_CPU)
LDFLAGS=
LDLIBS=

test.hex: test.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -Wl,--oformat=ihex -o test.hex test.c

test.elf: test.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -o test.elf test.c

install:
	avrdude -v -p $(MCU_TARGET) -c $(PROGRAMMER) -P $(TTY) -b 57600 -D -U flash:w:test.hex:i

clean:
	rm -f *.hex *.elf
