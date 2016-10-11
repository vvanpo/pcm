#include <avr/io.h>
#include <util/delay.h>

#include <avr/avr_mcu_section.h>
AVR_MCU(F_CPU, "atmega328");

const struct avr_mmcu_vcd_trace_t _trace[] _MMCU_ = {
    { AVR_MCU_VCD_SYMBOL("PORTB5"), .mask = _BV(PORTB5), .what = (void*)&PORTB, },
};

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
