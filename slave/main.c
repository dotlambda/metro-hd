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
#include "music.h"

void init();

volatile uint8_t tone_nr = 0;

volatile uint8_t tone_on[NO_TONES];
volatile uint16_t t[NO_TONES];
volatile uint32_t time;

SIGNAL(TIMER2_COMPA_vect)
{
    uint16_t pwm = 0;
    uint8_t nr_tones = 0;
    for (uint8_t i = 0; i < NO_TONES; ++i)
    {
        pwm += tone_on[i] * pgm_read_byte(&tones[t[i] + toneOffset[i]]);
        nr_tones += tone_on[i];
        if (t[i] >= toneLength[i])
            t[i] = 0;
    }
    if (nr_tones == 0)
        OCR1A = 0;
    else
        OCR1A = (uint8_t) (pwm / nr_tones);
}

int main(void)
{
    init();

    for (uint8_t i = 0; i < 2078; ++i)
    {
        _delay_ms(pgm_read_word(&elise_times[i]));
        uint8_t tone = pgm_read_byte(&elise_tones[i]) & ~(1 << 7);
        if (elise_tones[i] & (1 << 7))
            tone_on[tone]++;
        else
            tone_on[tone]--;
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
