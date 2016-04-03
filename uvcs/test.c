
#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#define F_OSC 16000000L
#define BAUD 57600
#define UBRR (F_OSC / (BAUD * 16L) - 1)

int main (void) {
    // Initialize UART
    UBRR0H = (uint8_t) UBRR >> 8;
    UBRR0L = (uint8_t) UBRR;
    UCSR0B = _BV(TXEN0) | _BV(RXEN0);
    char c;
    while (1) {
        c = 0;
        while (!(UCSR0A & _BV(RXC0)));
        c = UDR0;
        while (!(UCSR0A & _BV(UDRE0)));
        UDR0 = c;
    }
    return 0;
}
