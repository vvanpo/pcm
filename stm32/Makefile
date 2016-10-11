NAME=uvcs.elf

CFLAGS=-std=c11 -Wall -mcpu=cortex-m3 -mabi=aapcs -mfloat-abi=hard

debug: CFLAGS+=-g
debug: all

nodebug: CPPFLAGS+=-DNDEBUG
nodebug: all

all: $(NAME)

$(NAME):
	gcc $(CFLAGS) $(CPPFLAGS) -o $(NAME) *.c
