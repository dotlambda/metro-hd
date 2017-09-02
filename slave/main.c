/* 
 *	Basis
 *	2009 Benjamin Reh und Joachim Schleicher
 */
#include <avr/io.h>
#include <inttypes.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "uart.h"
#include "tone.h"

void init();

volatile uint8_t tone_nr = 0;
volatile uint16_t t = 0;

SIGNAL(TIMER2_COMPA_vect)
{
    if (t >= toneLength[tone_nr])
        t = 0;
    uint16_t offset = toneOffset[tone_nr];
    uint8_t pwm = pgm_read_byte(&tones[t + offset]);
    OCR1A = pwm; 
    t++;
}

int main(void)
{
    init();

    for (uint8_t i = 0; i < NO_TONES; ++i)
    {
        tone_nr = i;
        _delay_ms(100);
    }
}

void init()
{
    uartInit();   // serielle Ausgabe an PC

    // Timer1
    DDRB |= (1 << 1); // set B1 to output
    TCCR1A = (1 << WGM12) | (1 << WGM10); // Fast PWM, 8-bit
    TCCR1A |= (1 << COM1A1); // toggle pin B1 on compare match
    TCCR1B = (1 << CS10); // prescaler = 1

    // Timer2
    TCCR2A = (1 << WGM21); // CTC
    TCCR2B = (1 << CS22); // prescaler = 64
    OCR2A = 25;
    TIMSK2 |= (1 << OCIE2A);
    sei();
}
