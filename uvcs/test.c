
#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#define BAUD 250000
#include <util/setbaud.h>

int main (void) {
    // Initialize UART
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;
#if USE_2X
    UCSR0A |= _BV(U2X0);
#else
    UCSR0A &= ~_BV(U2X0);
#endif
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
