
#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#define BAUD 38400
#include <util/setbaud.h>

#include <avr/avr_mcu_section.h>
//AVR_MCU_SIMAVR_CONSOLE(GPIOR0);
AVR_MCU(F_CPU, "atmega328");

const struct avr_mmcu_vcd_trace_t _trace[] _MMCU_ = {
    { AVR_MCU_VCD_SYMBOL("PORTB5"), .mask = _BV(PORTB5), .what = (void*)&PORTB, },
};

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
