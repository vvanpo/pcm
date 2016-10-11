#include <avr/io.h>
#include <util/delay.h>

// Blink period in ms
#define BLINK_DELAY 500

int main (void)
{
    // Set PB5 (Arduino pin 13) as output
    DDRB |= _BV(DDB5);
    for (int i = 0; i < 10; i++) {
        PORTB |= _BV(PORTB5);
        _delay_ms(BLINK_DELAY);
        PORTB &= ~_BV(PORTB5);
        _delay_ms(BLINK_DELAY);
    }
    return 0;
}
