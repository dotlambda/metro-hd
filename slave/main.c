/* 
 *	Basis
 *	2009 Benjamin Reh und Joachim Schleicher
 */
#include <avr/io.h>
#include <inttypes.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "uart.h"
#include "timer.h"
#include "music.h"

void init();

SIGNAL(TIMER1_COMPA_vect)
{
    PORTB ^= (1 << 1);
}

int main()
{
    init();

    // shoot
    for (uint16_t i = 0; i < 10000; i += 15)
    {
        OCR1A = i;
        _delay_ms(1);
    }
    OCR1A = 0;
    _delay_ms(1000);

    // explosion
    for (uint16_t i = 5000; i < 40000; i *= 1.002)
    {
        OCR1A = i;
        _delay_us(1000);
        OCR1A /= 2;
        _delay_us(100);
    }
    OCR1A = 0;
    _delay_ms(1000);

    while (1)
    {
        uint8_t i = 0;
        while (1)
        {
            uint16_t delay = pgm_read_word(&elise[i]);
            if (i > 0 && delay == 0)
                break;
            reset_timer();
            while(getMsTimer() < delay);
            OCR1A = pgm_read_word(&elise[i+1]);
            i += 2;
        }
        OCR1A = 0;
        _delay_ms(100);
    }
}

void init()
{
    uartInit();   // serielle Ausgabe an PC
    timerInit();

    DDRB = (1 << 1); // set B1 to output

    // Timer1
    OCR1A = 0;
    TCCR1B = (1 << WGM12); // CTC, TOP is OCR1A
    TCCR1A = (1 << COM1A0); // toggle pin OC1A (B1) on compare match
    //TCCR1A = (1 << WGM13) | (1 << WGM11) | (1 << WGM10);
    //TCCR1A |= (1 << COM1A1);
    //TCCR1A = (1 << WGM13) | (1 << WGM12) | (1 << WGM11) | (1 << WGM10);
    //TCCR1A |= (1 << COM1A0);
    //TIMSK1 = (1 << OCIE1A); // enable interrupt
    TCCR1B |= (1 << CS11); // prescaler = 8
    //sei();
}
