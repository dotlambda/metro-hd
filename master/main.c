/* 
 *	Basis
 *	2009 Benjamin Reh und Joachim Schleicher
 */
#include <stdlib.h>
#include "uart.h"
#include "adc.h"
#include "timer.h"
#include "buttons.h"
#include "display.h"

#define LEVEL_WIDTH 320 // 2*DISPLAY_WIDTH

#define ON_THE_GROUND 0
#define FALLING_DOWN 7

void init();

// x1,y1 - x1+6,y1+2
int collision(int x1, int y1, int x2, int y2)
{
    return x1 < x2+6 && x1+6 > x2
           && y1 < y2+2 && y1+2 > y2;
}

void drawmonster(int x, int y)
{
    page(x, y, 0b11010000);
    page(x+1, y, 0b11110000);
    page(x+2, y, 0b11111100);
    page(x+3, y, 0b11110111);
    page(x+4, y, 0b11110111);
    page(x+5, y, 0b00110111);
    page(x+6, y, 0b00000011);
    page(x, y+1, 0b00000111);
    page(x+1, y+1, 0xFF);
    page(x+2, y+1, 0xFF);
    page(x+3, y+1, 0xFF);
    page(x+4, y+1, 0xFF);
    page(x+5, y+1, 0b00001111);
    page(x+6, y+1, 0b00000101);
    page(x, y+2, 0b11010000);
    page(x+1, y+2, 0b11111101);
    page(x+2, y+2, 0b11001111);
    page(x+3, y+2, 0b00000011);
    page(x+4, y+2, 0b11010011);
    page(x+5, y+2, 0xFF);
    page(x+6, y+2, 0b11001100);
}
    

void drawdoor()
{
    page(0,24, 0xFF);
    page(1,24, 0xFF);
    page(2,24, 0xFF);
    page(0,23, 0xFF);
    page(1,23, 0xFF);
    page(2,23, 0xFF);
    page(0,22, 0xFF);
    page(1,22, 0xFF);
    page(2,22, 0xFF);
    page(0,21, 0xFF);
    page(1,21, 0xFF);
    page(2,21, 0xFF);
    page(0,20, 0xFF);
    page(1,20, 0xFF);
    page(2,20, 0xFF);
    
}

void drawcharacter(int x, int y)
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
    for (int x = 0; x < DISPLAY_WIDTH; x+=8)
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

long level_seed;
long level_pos = 0;
long platforms;
void drawplatform()
{
    srandom(level_seed + level_pos);
    platforms = random();
    for(uint8_t pos = 0; pos < DISPLAY_WIDTH/8; ++pos) // draw random platforms at 20 possible positions
    {
        if (platforms & (1l << pos))
        {
            for (short i = 0; i < 8; ++i)
            {
                page(8*pos + i, 20, 0xFF);
            }
        }
    }
}

long platform(int x, int y)
{
    if (y == 20)
        return platforms & (1l << (x / 8));
    else
        return 0;
}

void redraw()
{
    clear();
    drawfloor();
    drawplatform();
}
    
int main(void)
{
	init();
    level_seed = random();
    redraw();
    drawdoor();
    int a = 50;
    int b = 22;
    drawmonster(a, b);
    int direction = 0;
    int x = 10;
    int y = 22;
    drawcharacter(x,y);
    
    int jumpcounter = ON_THE_GROUND;
    uint32_t nextmoveevent = 0;
    uint32_t nextjumpevent = 0;
    uint32_t nextmonsterevent = 0;
    while (1)
    {
        //monster in Bewegung
        if(nextmonsterevent < getMsTimer())
        {
            if (a != 0 && direction == 0)
            {
                page(a + 6, b, 0x00);
                page(a + 6, b + 1, 0x00);
                page(a + 6, b + 2, 0x00);
                a = a - 1;
                drawmonster(a,b);
            }
            else if (a == 0)
            {
                direction = 1;
            }
            if (a != DISPLAY_WIDTH - 7 && direction == 1)
            {
                page(a, b, 0x00);
                page(a, b + 1, 0x00);
                page(a, b + 2, 0x00);
                a = a + 1;
                drawmonster(a,b);
            }
            else if (a == DISPLAY_WIDTH - 7)
            { 
                direction = 0;
            }
            nextmonsterevent = getMsTimer() + 100;
        }
        
        //Character kann sich bewegen
        if (nextmoveevent < getMsTimer())
        {
            // wenn er nicht rechts gegen eine Plattform stoeßt
            if (B_RIGHT)
            {
                if (x + 7 == DISPLAY_WIDTH)
                {
                    ++level_pos;
                    redraw();
                    x = 0;
                    //y = 22;
                    drawcharacter(x,y);
                }
                else if (!platform(x+7,y) &&!platform(x+7, y+1)&&!platform(x+7, y+2))
                {
                    //falls er am rechten Ende von Plattform
                    if (platform(x,y+3) && !platform(x+1,y+3))
                    {    
                        jumpcounter = FALLING_DOWN;
                    }
                    page(x, y, 0x00);
                    page(x, y+1, 0x00);
                    page(x, y+2, 0x00);
                    drawcharacter(++x, y);
                    nextmoveevent = getMsTimer() + 50;
                }
        }
            if (B_LEFT &&!platform(x-1,y) &&!platform(x-1, y+1)&&!platform(x-1, y+2))
            {
                if (x == 0)
                {
                    --level_pos;
                    redraw();
                    x = 153;
                    //y = 22;
                    drawcharacter(x,y);
                }
                if (platform(x+6,y+3) && !platform(x+5,y+3))
                {
                    jumpcounter = FALLING_DOWN;
                }
                page(x+6, y, 0x00);
                page(x+6, y+1, 0x00);
                page(x+6, y+2, 0x00);
                drawcharacter(--x, y);
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
                    drawcharacter(x, y);
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
                        drawcharacter(x, y);
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
        
        //falls sich Monster und Character begegnen
        if (collision(x, y, a, b))
        {
            clear();
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
