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

/*SIGNAL(TIMER1_COMPA_vect)
{
    PINB ^= (1 << 1);
}*/

int main()
{
    init();

    OCR1A = 500;
    while(1);

    uint8_t i = 0;
    while (1)
    {
        uint16_t delay = pgm_read_word(&elise[i]);
        if (i > 0 && delay == 0)
        {
            OCR1A = 0;
            break;
        }
        reset_timer();
        while(getMsTimer() < delay);
        OCR1A = pgm_read_word(&elise[i+1]);
        i += 2;
    }
    OCR1A = 0;
}

void init()
{
    uartInit();   // serielle Ausgabe an PC
    timerInit();

    DDRB |= (1 << 1); // set B1 to output

    // Timer1
    //TCCR1A = (1 << WGM12); // CTC, TOP is OCR1A
    //TCCR1A |= (1 << COM1A0); // toggle pin OC1A (B1) on compare match
    TCCR1A = (1 << WGM13) | (1 << WGM10);
    TCCR1A |= (1 << COM1A1);
    TCCR1B = (1 << CS12); // prescaler = 
    //TIMSK1 = (1 << OCIE1A); // enable interrupt
}
