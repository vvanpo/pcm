
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <libftdi1/ftdi.h>
#include <assert.h>

#define SK      0x01
#define D0      0x02
#define D1      0x04
#define CS      0x08
#define GPIOL0  0x10
#define GPIOL1  0x20
#define GPIOL2  0x40
#define GPIOL3  0x80
#define GPIOH0  0x01
#define GPIOH1  0x02
#define GPIOH2  0x04
#define GPIOH3  0x08
#define GPIOH4  0x10
#define GPIOH5  0x20
#define GPIOH6  0x40
#define GPIOH7  0x80

enum value {LOW, HIGH};
enum direction {IN, OUT};

struct ft232h {
    struct ftdi_context *ftdi;
    unsigned char pin_direction[2];
    unsigned char pin_value[2];
    bool clock_polarity;
    bool clock_phase;
};

struct ft232h *init (void);
void set_spi_mode (struct ft232h *, bool, int);
void set_pins (struct ft232h *, enum value, unsigned char, enum direction);
void write_pins (struct ft232h *, enum value, unsigned char, unsigned char);
unsigned char read_pins (struct ft232h *, enum value, unsigned char);
void transfer (struct ft232h *, unsigned char *, size_t);

int main (int argc, char **argv)
{
    struct ft232h *context = init();
    while (1) {
        char buf[] = "hello world\n";
        transfer(context, (unsigned char *) buf, strlen(buf));
    }
    return 0;
}

struct ft232h *init (void)
{
    int ret;
    struct ft232h *context = calloc(1, sizeof (struct ft232h));
    context->ftdi = ftdi_new();
    if (ftdi_init(context->ftdi) < 0
            || ftdi_usb_open(context->ftdi, 0x0403, 0x6014) < 0
            || ftdi_set_bitmode(context->ftdi, 0x00, BITMODE_MPSSE) < 0) {
        fputs(ftdi_get_error_string(context->ftdi), stderr);
        return NULL;
    }
    // Set GPIO pins as inputs, value LOW
    unsigned char buf[6] = {0x80, 0x00, 0x00, 0x82, 0x00, 0x00};
    if ((ret = ftdi_write_data(context->ftdi, buf, 6)) < 0) {
        fputs(ftdi_get_error_string(context->ftdi), stderr);
        return NULL;
    }
    assert(ret == 6);
    set_spi_mode(context, true, 0);
    return context;
}

void set_spi_mode (struct ft232h *context, bool master, int mode)
{
    int ret;
    context->pin_direction[0] &= 0xf0;
    context->pin_direction[0] |= master ? 0x0b : 0x03;
    context->pin_value[0] &= 0xf0;
    context->pin_value[0] |= (context->clock_polarity = (mode == 2 || 3)) ? 0x01 : 0x00;
    context->clock_phase = (mode == 1 || 3);
    unsigned char buf[3] = {0x80, context->pin_direction[0], context->pin_value[0]};
    if ((ret = ftdi_write_data(context->ftdi, buf, 3)) < 0)
        fputs(ftdi_get_error_string(context->ftdi), stderr);
    assert(ret == 3);
}

void set_pins (struct ft232h *context, enum value byte, unsigned char pins, enum direction direction)
{
    int ret;
    if (direction == IN) {
        context->pin_direction[byte] &= ~pins;
        context->pin_value[byte] &= ~pins;
    } else
        context->pin_direction[byte] |= pins;
    unsigned char buf[3] = {byte ? 0x82 : 0x80, context->pin_direction[byte], context->pin_value[byte]};
    if ((ret = ftdi_write_data(context->ftdi, buf, 3)) < 0)
        fputs(ftdi_get_error_string(context->ftdi), stderr);
    assert(ret == 3);
}

// Sets pin directions as OUT and writes values
void write_pins (struct ft232h *context, enum value byte, unsigned char pins, unsigned char values)
{
    context->pin_value[byte] &= ~pins;
    context->pin_value[byte] |= values & pins;
    set_pins(context, byte, pins, OUT);
}

// Sets pin directions as IN and reads values
unsigned char read_pins (struct ft232h *context, enum value byte, unsigned char pins)
{
    int ret;
    if (pins ^ context->pin_direction[byte] != pins)
        set_pins(context, byte, pins, IN);
    unsigned char buf = byte ? 0x83 : 0x81;
    if ((ret = ftdi_write_data(context->ftdi, &buf, 1)) < 0)
        fputs(ftdi_get_error_string(context->ftdi), stderr);
    assert(ret == 1);
    for (int data = 0; data < 1; data += ret) {
        if ((ret = ftdi_read_data(context->ftdi, &buf, 1)) < 0)
            fputs(ftdi_get_error_string(context->ftdi), stderr);
    }
    return buf & pins;
}

void transfer (struct ft232h *context, unsigned char *buf, size_t size)
{
    int ret;
    if (size > 0x10000 || size < 1) return;
    unsigned char *wbuf = malloc(3 + size);
    wbuf[0] = 0x30
        | (context->clock_polarity ^ context->clock_phase) << 3
        | (context->clock_polarity & context->clock_phase);
    wbuf[1] = 0xff & (size - 1);
    wbuf[2] = (size - 1) >> 8;
    memcpy(wbuf + 3, buf, size);
    if ((ret = ftdi_write_data(context->ftdi, wbuf, size + 3)) < 0)
        fputs(ftdi_get_error_string(context->ftdi), stderr);
    assert(ret == 3 + size);
    for (int data = 0; data < size; data += ret) {
        if ((ret = ftdi_read_data(context->ftdi, buf, size)) < 0)
            fputs(ftdi_get_error_string(context->ftdi), stderr);
    }
}
