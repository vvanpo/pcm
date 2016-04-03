
#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

enum { UP, DOWN };

ISR (TIMER1_OVF_vect)
{
    static uint16_t pwm;
    static uint8_t direction;
    if (direction == UP && ++pwm == 0x03ff)
        direction = DOWN;
    else if (direction == DOWN && --pwm == 0)
        direction = UP;
    OCR1A = pwm;
}

int main (void)
{
    TCCR1A = _BV(WGM10) | _BV(WGM11) | _BV(COM1A1);
    TCCR1B = _BV(CS11) | _BV(CS12);
    OCR1A = 0;
    DDRB = _BV(DDB1);
    TIMSK1 = _BV(TOIE1);
    sei();
    while (1)
        sleep_mode();
    return 0;
}
