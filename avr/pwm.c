
#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

enum { UP, DOWN };

ISR (TIMER0_OVF_vect)
{
    static uint8_t direction;
    if (direction == UP && ++OCR0B == 0x3f)
        direction = DOWN;
    else if (direction == DOWN && --OCR0B == 0)
        direction = UP;
}

int main (void)
{   
    // Fast-PWM, TOP = 0xff
    TCCR0A = _BV(COM0B1) | _BV(WGM01) | _BV(WGM00);
    // No prescaler
    TCCR0B = _BV(CS02) | _BV(CS00);
    // OCOB pin as output
    DDRD |= _BV(PD5);
    // Set timer overflow interrupt
    TIMSK0 = _BV(TOIE0);
    // Enable interrupts
    sei();
    while (1)
        sleep_mode();
    return 0;
}
