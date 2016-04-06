
#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#define BAUD 38400
#include <util/setbaud.h>

void init_uart (void);

int main (void)
{
    init_uart();
    char c;
    while (1) {
        loop_until_bit_is_set(UCSR0A, RXC0);
        c = UDR0;
        loop_until_bit_is_set(UCSR0A, UDRE0);
        UDR0 = c;
    }
    return 0;
}

void init_uart (void)
{
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;
#if USE_2X
    UCSR0A |= _BV(U2X0);
#else
    UCSR0A &= ~_BV(U2X0);
#endif
    UCSR0B = _BV(TXEN0) | _BV(RXEN0);
}
