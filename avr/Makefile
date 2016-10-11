NAME=test

MCU_TARGET=atmega328p
F_CPU=16000000
PROGRAMMER=arduino
PROGRAMMER_BAUD=57600
TTY=/dev/ttyUSB0

CFLAGS=-std=c11 -Wall -mmcu=$(MCU_TARGET)
CPPFLAGS=-DF_CPU=$(F_CPU) -I$(SIMPATH)/include/

debug: CFLAGS+=-g
debug: all
nodebug: CPPFLAGS+=-DNDEBUG
nodebug: all

all: $(NAME).elf $(NAME).hex

$(NAME).o: $(NAME).c
	gcc -c $(CFLAGS) $(CPPFLAGS) $(NAME).c
$(NAME).elf: $(NAME).o
	gcc -o $(NAME).elf $(NAME).o -Wl,-Map,$(NAME).map && \
	avr-objdump -hS $(NAME).elf > $(NAME).lst
$(NAME).hex: $(NAME).o
	avr-objcopy -j.text -j.data -Oihex $(NAME).o $(NAME).hex

SIMFLAGS=-m $(MCU_TARGET) -f $(F_CPU)
SIMPATH=$(HOME)/.local

sim: $(NAME).elf
	simavr $(SIMFLAGS) $(NAME).elf
sim-debug: $(NAME).elf
	simavr -g $(SIMFLAGS) $(NAME).elf

upload: $(NAME).hex
	avrdude -v -p $(MCU_TARGET) -c $(PROGRAMMER) -P $(TTY) -b $(PROGRAMMER_BAUD) -D -U flash:w:$(NAME).hex:i

clean:
	rm -f *.o *.lst *.map *.elf *.hex
