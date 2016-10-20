// lcd.c 
//  Initialization and control code for Hitachi HD44780
//  ADC initialization for ATmega
//  Not modular, well-documented, or even legible, so let's hope I don't need to
//  use this twice.

#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>

void write (uint8_t v)
{
    // In 4-bit mode PD2 is DB7, to PD5 is DB4
    // Set PD2--PD5 as output
    DDRD |= _BV(DDD2) | _BV(DDD3) | _BV(DDD4) | _BV(DDD5);
    // Write value
    if (0x08 & v) PORTD |= _BV(DDD2);
    else PORTD &= ~_BV(DDD2);
    if (0x04 & v) PORTD |= _BV(DDD3);
    else PORTD &= ~_BV(DDD3);
    if (0x02 & v) PORTD |= _BV(DDD4);
    else PORTD &= ~_BV(DDD4);
    if (0x01 & v) PORTD |= _BV(DDD5);
    else PORTD &= ~_BV(DDD5);
    // Pulse ENABLE pin
    PORTB &= ~_BV(PORTB3);
    _delay_us(1);
    PORTB |= _BV(PORTB3);
    _delay_us(1);
    PORTB &= ~_BV(PORTB3);
    _delay_us(100);
}

void send (uint8_t v, uint8_t rs)
{
    if (rs) PORTB |= _BV(DDB4);
    else PORTB &= ~_BV(DDB4);
    write(v >> 4);
    write(v);
}

// 2 rows (0 and 1), 16 columns (0 through 0x18)
void position (uint8_t column, uint8_t row)
{
    send(0x80 | (0x40 * row + column), 0);
}

// Refer to Arduino's LiquidCrystal library for a reference implementation:
//  https://github.com/arduino/Arduino/tree/master/libraries/LiquidCrystal/src
// and the HD44780 datasheet: ./HD44780.pdf
void initialize_lcd (void)
{
    // Set PB4 as RS pin
    DDRB |= _BV(DDB4);
    // Set PB3 as ENABLE pin
    DDRB |= _BV(DDB3);
    // Wait >40ms for device power-on
    _delay_ms(50);
    // Begin commands by pulling RS and ENABLE low
    PORTB &= ~_BV(PORTB4);
    PORTB &= ~_BV(PORTB3);
    // Put in 4-bit mode
    write(0x03);
    _delay_ms(5);
    write(0x03);
    _delay_us(150);
    write(0x03);
    write(0x02);
    // # of lines + font
    send(0x28, 0);
    // Display on, no cursor or blinking
    send(0x04, 0);
    // Display clear
    send(0x01, 0);
    _delay_ms(2);
    // Entry mode set
    send(0x06, 0);
}

void initialize_adc (void)
{
    // Set ADC prescaler division factor to 128, at F_CPU=16 MHz this means the
    //  ADC frequency is 125 kHz
    ADCSRA = _BV(ADEN) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);
    // Set PC0/ADC0 as ADC input, set reference voltage to Vcc (remember to
    //  connect a capacitor between Aref and GND)
    ADMUX = _BV(REFS0);
}

unsigned int adc_convert ()
{
    ADCSRA |= _BV(ADSC);
    while (ADCSRA & _BV(ADSC));
    return ADCW;
}

int main (void)
{
    initialize_lcd();
    initialize_adc();
    for (char *s = "ADC0: "; *s; s++) send(*s, 1);
    while (1) {
        position(6, 0);
        char s[5] = {};
        itoa(adc_convert(), s, 10);
        for (int i = 0; s[i]; i++) send(s[i], 1);
        _delay_ms(200);
    }
    return 0;
}
