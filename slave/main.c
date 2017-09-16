/* 
 *	Basis
 *	2009 Benjamin Reh und Joachim Schleicher
 */
#include <avr/io.h>
#include <inttypes.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "uart.h"
#include "music.h"

#define INTERRUPT_COMPARE 128

void init();

volatile uint32_t time = 0;

#define CONCURRENT_TONES 2
volatile uint16_t state[CONCURRENT_TONES];
volatile uint16_t increment[CONCURRENT_TONES];
#define MUSIC increment[0]
#define EFFECT increment[1]

const uint16_t* playing;
uint16_t i;
uint16_t delay;

static inline void update_increment()
{
    if (time > delay)
    {
        time = 0;
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
        i += 2;
    }
}

SIGNAL(TIMER2_COMPA_vect)
{
    time++;
    
    // music
    uint32_t pwm = state[0] >> 9;
    state[0] += increment[0];
    
    if (increment[1])
    {
        // effect
        pwm += state[1] >> 8;
        state[1] += increment[1];

        OCR1A = pwm >> 1;
    }
    else
    {
        OCR1A = pwm;
    }

    update_increment();
}

static inline void start_playing(const uint16_t* music)
{
    cli();
    i = 0;
    playing = music;
    update_increment();
    sei();
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
                //start_playing(boss2);
                break;
            case 's': // shoot
                for (uint16_t i = 4000; i > 1500; i -= 10)
                {
                    EFFECT = i;
                    uint32_t wait = time + 10;
                    while (time != 0 && time < wait);
                }
                EFFECT = 0;
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
                EFFECT = 0;
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
        increment[i] = 0;
        state[i] = 0;
    }

    // Timer1
    DDRB = (1 << 1); // set B1 to output
    TCCR1A = (1 << WGM10); // Fast PWM, 8-bit
    TCCR1B = (1 << WGM12);
    TCCR1A |= (1 << COM1A1); // toggle pin B1 on compare match
    TCCR1B |= (1 << CS10); // prescaler = 1

    // Timer2
    TCCR2A = (1 << WGM21); // CTC
    TCCR2B = (1 << CS21); // prescaler = 8
    OCR2A = INTERRUPT_COMPARE;
    TIMSK2 = (1 << OCIE2A); // enable interrupt
    sei();
}
