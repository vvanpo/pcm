
#include <stdlib.h>
#include <stdio.h>
#include <libftdi1/ftdi.h>

struct ftdi_context * init_ft232h (void);

int main (int argc, char **argv)
{
    struct ftdi_context *ftdi = init_ft232h();
    return 0;
}

struct ftdi_context * init_ft232h (void)
{
    struct ftdi_context *ftdi = ftdi_new();
    if (ftdi_init(ftdi) < 0
            || ftdi_usb_open(ftdi, 0x0403, 0x6014) < 0
            || ftdi_set_bitmode(ftdi, 0x00, BITMODE_MPSSE) < 0) {
        fputs(ftdi_get_error_string(ftdi), stderr);
        return NULL;
    }
    return ftdi;
}
