#include <libftdi1/ftdi.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	struct ftdi_context *ftdi = ftdi_new();
	if (!ftdi) {
		fprintf(stderr, "Allocation failure\n");
		return -1;
	}
	if (ftdi_usb_open(ftdi, 0x0403, 0x6014)) goto ftdi_err;
	if (ftdi_usb_close(ftdi)) goto ftdi_err;
	if (ftdi_set_bitmode(ftdi, 0x00, BITMODE_MPSSE)) goto ftdi_err;
	ftdi_free(ftdi);
	return 0;
ftdi_err:
	fprintf(stderr, "USB failure: %s\n", ftdi_get_error_string(ftdi));
	ftdi_free(ftdi);
	return -1;
}
