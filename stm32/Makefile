NAME=blink

DEVICE=STM32F103xB
# BOARD=st_nucleo_f103rb

CFLAGS=-Os -std=c11 -Wall -mcpu=cortex-m3 -mthumb
CPPFLAGS=-D$(DEVICE) -ICMSIS/Include -ICMSIS/Device/ST/STM32F1xx/Include
LDFLAGS=-TSTM32F103XB_FLASH.ld --specs=nosys.specs

debug: CFLAGS+=-g
debug: all
nodebug: CPPFLAGS+=-DNDEBUG
nodebug: all

all: $(NAME).hex

$(NAME).o system.o startup.o: system.h $(NAME).c system.c startup.s
	arm-none-eabi-gcc -c $(CFLAGS) $(CPPFLAGS) $(NAME).c system.c startup.s

$(NAME).elf: $(NAME).o system.o startup.o
	arm-none-eabi-gcc $(CFLAGS) $(LDFLAGS) -o $(NAME).elf \
	    $(NAME).o system.o startup.o && \
	arm-none-eabi-objdump -hS $(NAME).elf > $(NAME).lst

$(NAME).hex: $(NAME).elf
	objcopy -Oihex $(NAME).elf $(NAME).hex

clean:
	rm -f *.o *.lst *.elf *.hex
