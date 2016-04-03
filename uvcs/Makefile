
MCU_TARGET=atmega328p
PROGRAMMER=arduino
TTY=/dev/ttyUSB0

CC=avr-gcc
OBJCOPY=avr-objcopy
CFLAGS=-c -std=c11 -Wall -Wno-parentheses -mmcu=$(MCU_TARGET) -g
OCFLAGS=-j .text -j .data -O ihex

test.hex: test.elf
	$(OBJCOPY) $(OCFLAGS) test.elf test.hex

test.elf: test.c
	$(CC) $(CFLAGS) -o test.elf test.c

install:
	avrdude -v -p $(MCU_TARGET) -c $(PROGRAMMER) -P $(TTY) -b 57600 -D -U flash:w:test.hex:i

clean:
	rm *.hex *.elf
