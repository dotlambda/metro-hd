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

int main()
{
    uint8_t i = 0;
    while (1)
    {
        uint16_t delay = pgm_read_word(&elise[i]);
        if (i > 0 && delay == 0)
        {
            PINB = 0;
            break;
        }
        i += 2;
        reset_timer();
        while(getMsTimer() < delay);
        OCR1A = pgm_read_word(&elise[i+1]);
    }
    OCR1A = 0;
}

void init()
{
    uartInit();   // serielle Ausgabe an PC
    timerInit();

    DDRB |= (1 << 1); // set B1 to output

    // Timer1
    // Fast PWM, TOP is OCR1A
    TCCR1A = (1 << WGM13) | (1 << WGM12) | (1 << WGM11) | (1 << WGM10);
    TCCR1A |= (1 << COM1A1); // toggle pin OC1A (B1) on compare match
    TCCR1B = (1 << CS11); // prescaler = 8
    //TIMSK1 = (1 << OCIE1A); // enable interrupt
}
