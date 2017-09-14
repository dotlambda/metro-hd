/* 
 *	Basis
 *	2009 Benjamin Reh und Joachim Schleicher
 */
#include <avr/io.h>
#include <inttypes.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <math.h>
#include "uart.h"
#include "timer.h"
#include "music.h"

void init();

SIGNAL(TIMER1_COMPA_vect)
{
    PORTB ^= (1 << 1);
}

const uint16_t* playing;
uint16_t i;
void play(const uint16_t* music)
{
    while (1)
    {
        while (1)
        {
            uint16_t delay = pgm_read_word(&music[i]);
            if (i > 0 && delay == 0)
                break;
            reset_timer();
            while(getMsTimer() < delay && !uart_data_waiting());
            if (uart_data_waiting())
                break;
            OCR1A = pgm_read_word(&music[i+1]);
            i += 2;
        }
        OCR1A = 0;
        if (uart_data_waiting())
            break;
        i = 0;
    }
}

void start_playing(const uint16_t* music)
{
    i = 0;
    playing = music;
    play(music);
}

void continue_playing()
{
    play(playing);
}

int main()
{
    init();
    
    uint16_t j = 0;
    while (1)
    {
        while(!uart_data_waiting());
        switch (uart_getc())
        {
            case 0:
                start_playing(splash);
                break;
            case 1:
                start_playing(ingame);
                break;
            case 's': // shoot
                for (uint16_t i = 0; i < 10000; i += 15)
                {
                    OCR1A = i;
                    _delay_us(500);
                }
                OCR1A = 0;
                break;
            case 'e': // explosion
                if (j % 2)
                {
                    for (uint16_t i = 50000; i > 5000; i -= 500)
                    {
                        OCR1A = i;
                        _delay_ms(1);
                    }
                    for (uint16_t i = 5000; i < 50000; i += 40)
                    {
                        OCR1A = i;
                        _delay_ms(1);
                    }
                }
                else
                {
                    for (uint16_t i = 5000; i < 40000; i *= 1.002)
                    {
                        OCR1A = i;
                        _delay_us(1000);
                        OCR1A /= 2;
                        _delay_us(100);
                    }
                }
                OCR1A = 0;
                j++;
                break;
            case 'b':
                start_playing(boss1);
                break;
            case 'c':
                start_playing(boss2);
                break;
            case 'd':
                start_playing(boss3);
                break;
            case 'g':
                start_playing(gameover);
                break;
        }
        if (!uart_data_waiting())
            continue_playing();
    }

    // random
    /*while(1)
    {
        OCR1A = random() % 100 * 100 + 2000;
        _delay_ms(300);
    }*/
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
