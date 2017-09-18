/* 
 *	Basis
 *	2009 Benjamin Reh und Joachim Schleicher
 */
#include <avr/io.h>
#include <inttypes.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stddef.h>
#include "uart.h"
#include "music.h"
#include "fx.h"

#define INTERRUPT_COMPARE 128

void init();

#define CONCURRENT_TONES 4
#define EFFECT 3
volatile uint16_t state[CONCURRENT_TONES];
volatile uint16_t increment[CONCURRENT_TONES];

volatile uint32_t time = 0;
const Event* playing = NULL;
uint16_t i;
uint16_t delay;
uint8_t single_channel = 0;
uint8_t repeat = 0;

volatile uint32_t time_fx = 0;
const uint16_t* playing_fx = NULL;
uint16_t i_fx;
uint16_t delay_fx;

volatile uint8_t pwm = 0;
volatile uint8_t next_sample = 0;

SIGNAL(TIMER2_COMPA_vect)
{
    time++;
    time_fx++;
    OCR1A = pwm;
    next_sample = 1;
}

static inline void start_playing(const Event* music, uint8_t single_channel_, uint8_t repeat_)
{
    cli();
    i = 0;
    playing = music;
    delay = 0;
    next_sample = 1;
    for (uint8_t i = 0; i < EFFECT; ++i)
    {
        increment[i] = 0;
        state[i] = 0;
    }
    single_channel = single_channel_;
    repeat = repeat_;
    sei();
}

static inline void start_playing_fx(const uint16_t* effect)
{
    cli();
    i_fx = 0;
    playing_fx = effect;
    delay_fx = 0;
    next_sample = 1;
    sei();
}

Event event;
static inline void update_increment()
{
    if (playing != NULL)
    {
        while (time > delay)
        {
            event.bits = pgm_read_dword(&playing[i]);
            if (event.change.delay == 0xFFFF) // stop
            {
                if (repeat)
                {
                    i = 0;
                }
                else
                {
                    playing = NULL;
                    for (uint8_t i = 0; i < EFFECT; ++i)
                    {
                        increment[i] = 0;
                        state[i] = 0;
                    }
                    break;
                }
            }
            else
            {
                time = 0;
                delay = event.change.delay;
                increment[event.change.track] = event.change.increment;
            }
            i++;
        }
    }

    if (playing_fx != NULL)
    {
        if (time_fx > delay_fx)
        {
            time_fx = 0;
            delay_fx = pgm_read_word(&playing_fx[i_fx]);
            if (delay_fx == 0xFFFF) // stop
            {
                playing_fx = NULL;
                increment[EFFECT] = 0;
                state[EFFECT] = 0;
            }
            else
            {
                increment[EFFECT] = pgm_read_word(&playing_fx[i_fx+1]);
            }
            i_fx += 2;
        }
    }
}

int main()
{
    init();

    while (1)
    {
        if (uart_data_waiting())
        {
            switch (uart_getc())
            {
                case 'i':
                    start_playing(ingame1, 0, 1);
                    break;
                case 'j':
                    start_playing(ingame2, 0, 1);
                    break;
                case 's':
                    start_playing_fx(shoot);
                    break;
                case 'e':
                    start_playing_fx(explosion);
                    break;
                case 'b':
                    start_playing(boss1, 1, 1);
                    break;
                case 'c':
                    start_playing(boss2, 1, 1);
                    break;
                case 'd':
                    start_playing(boss3, 1, 1);
                    break;
                case 'g':
                    start_playing(gameover, 1, 0);
                    break;
            }
        }

        if (next_sample)
        {
            uint16_t tmp = 0;

            if (single_channel)
            {
                tmp += 3 * ((state[0] >> 8) & 0x80);
                state[0] += increment[0];
            }
            else
            {
                for (uint8_t i = 0; i < EFFECT; ++i)
                {
                    //tmp += state[i] >> 8; // saw
                    tmp += (state[i] >> 8) & 0x80; // square
                    /*if (state[i] < 0x8000) // triangle
                        tmp += state[i] >> 7;
                    else
                        tmp += (0xFFFF - (state[i] & 0x7FFF << 1)) >> 8;*/
                    state[i] += increment[i];
                }
            }

            // >> 7 to make fx louder
            tmp += state[EFFECT] >> 7;
            state[EFFECT] += increment[EFFECT];

            pwm = tmp >> 2; // divide by 4

            update_increment();
            next_sample = 0;
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
