/* 
 *	Basis
 *	2009 Benjamin Reh und Joachim Schleicher
 */
#include <avr/io.h>
#include <inttypes.h>
#include <util/delay.h>
#include "uart.h"
#include "adc.h"
#include "timer.h"
#include "buttons.h"
#include "display.h"

void init();

int main(void)
{
	init();
	int x = 100;
    int y = 100;
	while (1) {
        switch(y % 4) {
            case 0:
                page(x, y/4, 0b00000011);
                break;
            case 1:
                page(x, y/4, 0b00001100);
                break;
            case 2:
                page(x, y/4, 0b00110000);
                break;
            case 3:
                page(x, y/4, 0b11000000);
                break;
        }
		if(B_UP)
        {
            _delay_ms(100);
            page(x, y/4, 0x00);
            if (y > 0)
                y -= 1;
        }
		if(B_DOWN)
        {
            _delay_ms(100);
            page(x, y/4, 0x00);
			if (y < 104)
                y += 1;
        }
		if(B_RIGHT)
        {
            _delay_ms(100);
            page(x, y/4, 0x00);
			if (x < 208)
                x += 1;
        }
		if(B_LEFT)
        {
            _delay_ms(100);
            page(x, y/4, 0x00);
			if (x > 0)
                x -= 1;
        }
	}
}


void init()
{
	uartInit();   // serielle Ausgabe an PC
	ADCInit(0);   // Analoge Werte einlesen
	timerInit();  // "Systemzeit" initialisieren
	buttonsInit();
	displayInit();
}
