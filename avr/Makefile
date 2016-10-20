NAME=lcd

MCU_TARGET=atmega328p
# F_CPU is needed for <util/delay.h>
F_CPU=16000000
PROGRAMMER=arduino
PROGRAMMER_BAUD=57600
TTY=/dev/serial/by-id/usb-FTDI_FT232R_USB_UART_A98F3PTP-if00-port0

CFLAGS=-Os -std=c11 -Wall -mmcu=$(MCU_TARGET)
CPPFLAGS=-DF_CPU=$(F_CPU)

debug: CFLAGS+=-g
debug: all
nodebug: CPPFLAGS+=-DNDEBUG
nodebug: all

all: $(NAME).hex

$(NAME).elf: $(NAME).c
	avr-gcc $(CFLAGS) $(CPPFLAGS) -Wl,-Map,$(NAME).map -o $(NAME).elf \
	    $(NAME).c && \
	avr-objdump -hS $(NAME).elf > $(NAME).lst
$(NAME).hex: $(NAME).elf
	avr-objcopy -j.text -j.data -Oihex $(NAME).elf $(NAME).hex

upload: $(NAME).hex
	avrdude -v -p $(MCU_TARGET) -c $(PROGRAMMER) -P $(TTY) \
	    -b $(PROGRAMMER_BAUD) -D -U flash:w:$(NAME).hex:i

clean:
	rm -f *.o *.lst *.map *.elf *.hex *.vcd
