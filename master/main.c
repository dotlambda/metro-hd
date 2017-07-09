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

#define ON_THE_GROUND 0
#define FALLING_DOWN 7

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
    page(x, y+2, 0b11000000);
    page(x+1, y+2, 0b11000011);
    page(x+2, y+2, 0xFF);
    page(x+3, y+2, 0b00000011);
    page(x+4, y+2, 0xFF);
    page(x+5, y+2, 0b11000011);
    page(x+6, y+2, 0b11000000);
    
    
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

uint32_t platform(int x, int y)
{
    if (y == 20)
        return platforms & (1 << (x / 8));
    else
        return 0;
}
    
int main(void)
{
	init();
    drawfloor();
    drawplatform();
    int x = 10;
    int y = 22;
    int jumpcounter = ON_THE_GROUND;
    uint32_t nextmoveevent = 0;
    uint32_t nextjumpevent = 0;
    while (1)
    {
        if (nextmoveevent < getMsTimer())
        {
            if (B_RIGHT && !platform(x+7,y) &&!platform(x+7, y+1)&&!platform(x+7, y+2))
            {
                //falls er am rechten Ende von Plattform
                if (platform(x,y+3) && !platform(x+1,y+3))
                {    
                    jumpcounter = FALLING_DOWN;
                }
                page(x, y, 0x00);
                page(x, y+1, 0x00);
                page(x, y+2, 0x00);
                drawmonster(++x, y);
                nextmoveevent = getMsTimer() + 50;
            }
            if (B_LEFT &&!platform(x-1,y) &&!platform(x-1, y+1)&&!platform(x-1, y+2))
            {
                if (platform(x+6,y+3) && !platform(x+5,y+3))
                {
                    jumpcounter = FALLING_DOWN;
                }
                page(x+6, y, 0x00);
                page(x+6, y+1, 0x00);
                page(x+6, y+2, 0x00);
                drawmonster(--x, y);
                nextmoveevent = getMsTimer() + 50;
            }
        }
        if (jumpcounter != ON_THE_GROUND)
        {
            if (nextjumpevent < getMsTimer())
            {
                //Plattform direkt über dem Monster
                if (platform(x, y-1) || platform(x+6, y-1))
                {
                    jumpcounter = FALLING_DOWN;
                }
                if (jumpcounter != FALLING_DOWN) // if we are moving upwards
                {
                    page(x, y+2, 0x00);
                    page(x+1, y+2, 0x00);
                    page(x+2, y+2, 0x00);
                    page(x+3, y+2, 0x00);
                    page(x+4, y+2, 0x00);
                    page(x+5, y+2, 0x00);
                    page(x+6, y+2, 0x00);
                    y = y - 1;
                    drawmonster(x, y);
                    ++jumpcounter;
                }
                else //fallen
                {
                    //noch kein boden, keine Plattform
                    if (y+3 != 25 && !platform(x, y+3) && !platform(x+6, y+3))
                    {
                        page(x, y, 0x00);
                        page(x+1, y, 0x00);
                        page(x+2, y, 0x00);
                        page(x+3, y, 0x00);
                        page(x+4, y, 0x00);
                        page(x+5, y, 0x00);
                        page(x+6, y, 0x00);
                        ++y;
                        drawmonster(x, y);
                    }
                    else //boden oder Plattform erreicht
                    {
                        jumpcounter = ON_THE_GROUND;
                    }
                }
                
                nextjumpevent = getMsTimer() + 150;
            }
        }
        else if (B_UP)
        {
            jumpcounter = 1;
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
