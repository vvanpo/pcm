
#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#define BAUD 38400
#include <util/setbaud.h>

void init_uart (void);

int main (void)
{
    cli();
    char *s = "hello world";
    init_uart();
    DDRB |= _BV(DDB5);
    while (1)
        //PORTB |= _BV(PB5);
        for (int i = 0; i < 13; i++) {
            loop_until_bit_is_set(UCSR0A, UDRE0);
            UDR0 = *(s + i);
            //GPIOR0 = *(s + i);
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
