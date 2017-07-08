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

void drawmonster(int x, int y)
{
    page(x, y, 0xFF);
    page(x+1, y, 0b11010011);
    page(x+2, y, 0b11000011);
    page(x+3, y, 0xFF);
    page(x+4, y, 0b11010011);
    page(x+5, y, 0b11000011);
    page(x+6, y, 0xFF);
    page(x, y+1, 0xFF);
    page(x+1, y+1, 0b11001111);
    page(x+2, y+1, 0b00001111);
    page(x+3, y+1, 0b00001111);
    page(x+4, y+1, 0b00001111);
    page(x+5, y+1, 0b11001111);
    page(x+6, y+1, 0xFF);
    page(x+1, y+2, 0b00000011);
    page(x+2, y+2, 0xFF);
    page(x+3, y+2, 0b00000011);
    page(x+4, y+2, 0xFF);
    page(x+5, y+2, 0b00000011);
    
}

void drawfloor()
{
    for (int x = 0; x < 208; x+=8)
    {
        page(x,   25, 0xFF);
        page(x+1, 25, 0xFF);
        page(x+2, 25, 0xFF);
        page(x+3, 25, 0xFF);
        page(x+4, 25, 0x55);
        page(x+5, 25, 0x55);
        page(x+6, 25, 0x55);
        page(x+7, 25, 0x55);
    }
}

uint32_t platforms;
void drawplatform()
{
    for(int i = 0; i<8; i++)
    {
        page(i+48, 20, 0xFF);
        platforms |= 1 << (48 / 8);
    }
}

uint32_t hasplatform(int x)
{
    return platforms & (1 << (x / 8));
}
    
int main(void)
{
	init();
    drawfloor();
    drawplatform();
    int x = 10;
    int y = 22;
    int jumpcounter = 0;
    uint32_t nextmoveevent = 0;
    uint32_t nextjumpevent = 0;
    while(1)
    {
        if (nextmoveevent < getMsTimer())
        {
            if (B_RIGHT)
            {
                page(x, y, 0x00);
                page(x, y+1, 0x00);
                page(x+1, y+2, 0x00);
                drawmonster(++x, y);
                nextmoveevent = getMsTimer() + 50;
            }
            if (B_LEFT)
            {
                page(x+6, y, 0x00);
                page(x+6, y+1, 0x00);
                page(x+5, y+2, 0x00);
                drawmonster(--x, y);
                nextmoveevent = getMsTimer() + 50;
            }
        }
        if (jumpcounter != 0)
        {
            if (nextjumpevent < getMsTimer())
            {
                
                if (jumpcounter < 6)
                {
                    
                    if(!hasplatform(x))
                    {
                    page(x, y+1, 0x00);
                    page(x+1, y+2, 0x00);
                    page(x+2, y+2, 0x00);
                    page(x+3, y+2, 0x00);
                    page(x+4, y+2, 0x00);
                    page(x+5, y+2, 0x00);
                    page(x+6, y+1, 0x00);
                    drawmonster(x, --y);
                    }
                }
                else if ((jumpcounter >= 6 && !hasplatform(x)) || (jumpcounter < 6 && hasplatform(x)))
                {
                    page(x, y, 0x00);
                    page(x+1, y, 0x00);
                    page(x+2, y, 0x00);
                    page(x+3, y, 0x00);
                    page(x+4, y, 0x00);
                    page(x+5, y, 0x00);
                    page(x+6, y, 0x00);
                    drawmonster(x, ++y);
                }
                jumpcounter = (jumpcounter+1) % 11;
                nextjumpevent = getMsTimer() + 150;
            }
        }
        else if (B_UP)
        {
            jumpcounter = 1;
            nextjumpevent = getMsTimer();
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
