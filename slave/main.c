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
#include "music.h"

void init();

volatile uint32_t time = 0;

#define CONCURRENT_TONES 2
volatile uint32_t period[CONCURRENT_TONES]; // in 1/10,000 seconds
volatile uint32_t t[CONCURRENT_TONES];
#define MUSIC period[0]
#define EFFECT period[1]

const uint16_t* playing;
uint16_t i;
uint16_t delay;

SIGNAL(TIMER2_COMPA_vect)
{
    time++; // this interrupt is called at a frequency of 10kHz
    
    // generate sound
    uint32_t pwm = 0;
    uint8_t nr_tones = 0;
    for (uint8_t i = 0; i < CONCURRENT_TONES; ++i)
    {
        if (period[i])
        {
            nr_tones++;

            // triangle
            /*if (2 * t[i] < period[i])
                pwm += 510 * t[i] / period[i];
            else if (t[i] < period[i])
                pwm += 510 - 510 * t[i] / period[i];
            else
                t[i] = 0; // pwm += 0
            t[i]++;*/

            // sawtooth
            pwm += 255 * t[i] / period[i];
            t[i]++;
            if (t[i] > period[i])
                t[i] = 0;
        }
    }
    if (nr_tones == 0)
        OCR1A = 0;
    else
        OCR1A = pwm / nr_tones;

    if (time > delay)
    {
        time = 0;
        i += 2;
        delay = pgm_read_word(&playing[i]);
        if (i > 0 && delay == 0)
        {
            i = 0;
            MUSIC = 0;
        }
        else
        {
            MUSIC = pgm_read_word(&playing[i+1]);
        }
    }
}

void start_playing(const uint16_t* music)
{
    i = 0;
    playing = music;
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
                    EFFECT = i;
                    _delay_us(500);
                }
                OCR1A = 0;
                break;
            case 'e': // explosion
                if (j % 2)
                {
                    for (uint16_t i = 50000; i > 5000; i -= 500)
                    {
                        EFFECT = i;
                        _delay_ms(1);
                    }
                    for (uint16_t i = 5000; i < 50000; i += 40)
                    {
                        EFFECT = i;
                        _delay_ms(1);
                    }
                }
                else
                {
                    for (uint16_t i = 5000; i < 40000; i *= 1.002)
                    {
                        EFFECT = i;
                        _delay_us(1000);
                        EFFECT /= 2;
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
    }
}

void init()
{
    uartInit();   // serielle Ausgabe an PC

    for (uint8_t i = 0; i < CONCURRENT_TONES; ++i)
    {
        period[i] = 0;
    }

    // Timer1
    DDRB |= (1 << 1); // set B1 to output
    TCCR1A |= (1 << WGM10); // Fast PWM, 8-bit
    TCCR1B |= (1 << WGM12);
    TCCR1A |= (1 << COM1A1); // toggle pin B1 on compare match
    TCCR1B |= (1 << CS10); // prescaler = 1

    // Timer2
    TCCR2A = (1 << WGM21); // CTC
    TCCR2B = (1 << CS22); // prescaler = 64
    OCR2A = 25;
    TIMSK2 |= (1 << OCIE2A); // enable interrupt
    sei();
}
