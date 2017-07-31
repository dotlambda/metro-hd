#include <stdlib.h>

#include "uart.h"
#include "adc.h"
#include "timer.h"
#include "buttons.h"
#include "display.h"
#include "character.h"
#include "globals.h"

#define LEVEL_WIDTH 320 // 2*DISPLAY_WIDTH


void init();

// x1,y1 - x1+6,y1+2
int collision(int x1, int y1, int x2, int y2)
{
    return x1 < x2+6 && x1+6 > x2
           && y1 < y2+2 && y1+2 > y2;
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

void drawfloor()
{
   /* for (int x = 0; x < DISPLAY_WIDTH; x+=8)
    {
        page(x,   25, 0xFF);
        page(x+1, 25, 0xFF);
        page(x+2, 25, 0xFF);
        page(x+3, 25, 0xFF);
        page(x+4, 25, 0x55);
        page(x+5, 25, 0x55);
        page(x+6, 25, 0x55);
        page(x+7, 25, 0x55);
    }*/
    for (int x = 0; x < DISPLAY_WIDTH; x+=16)
    {
        page(x, 25, 0b01010110);
        page(x+1, 25, 0b10100110);
        page(x+2, 25, 0b01010110);
        page(x+3, 25, 0b10101010);
        page(x+4, 25, 0b10010101);
        page(x+5, 25, 0b10011010);
        page(x+6, 25, 0b10010101);
        page(x+7, 25, 0b10101010);
        page(x+8, 25, 0b01100110);
        page(x+9, 25, 0b01100110);
        page(x+10, 25, 0b01010110);
        page(x+11, 25, 0b10101010);
        page(x+12, 25, 0b10010101);
        page(x+13, 25, 0b10101001);
        page(x+14, 25, 0b10010101);
        page(x+15, 25, 0b10101010);
    }
    
}

long level_seed;
long level_pos = 0;
void drawplatform()
{
    srandom(level_seed + level_pos);
    platforms_20 = random();
    platforms_15 = random();
//    platforms_10 = random();
    
    for(uint8_t pos = 0; pos < DISPLAY_WIDTH/PLATFORM_WIDTH; ++pos) // draw random platforms at 20 possible positions
    {
        if (!(platforms_20 & (3l << 2 * pos)))
        {
            for (short i = 0; i < PLATFORM_WIDTH; ++i)
            {
                page(PLATFORM_WIDTH * pos + i, 20, 0xFF);
            }
        }
        if (!(platforms_15 & (3l << 2 * pos)))
        {
            for (short i = 0; i < PLATFORM_WIDTH; ++i)
            {
                page(PLATFORM_WIDTH * pos + i, 15, 0xFF);
            }
        }
//         if (!(platforms_10 & (3l << 2 * pos)))
//         {
//             for (short i = 0; i < PLATFORM_WIDTH; ++i)
//             {
//                 page(PLATFORM_WIDTH * pos + i, 10, 0xFF);
//             }
//         }
    }
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
    //drawdoor();
    uint8_t projectile_x;
    uint8_t projectile_y;
    uint8_t projectile_direction = 0;

    struct Character protagonist_;
    protagonist = &protagonist_;
    protagonist->look = LOOK_PROTAGONIST;
    initcharacter(protagonist);
    protagonist->x = 10;
    protagonist->y = 22;
    draw(protagonist);
    
    struct Character monster_;
    struct Character* monster = &monster_;
    monster->look = LOOK_ROCKET;
    initcharacter(monster);
    monster->followsprotagonist = 1;
    monster->x = 50;
    monster->y = 21;
    draw(monster);
    
    
    
    uint32_t nextmoveevent = 0;
    uint32_t nextjumpevent = 0;
    uint32_t nextprojectilevent = 0;
    uint32_t nextmonsterevent = 0;
    while (1)
    {
        //monster in Bewegung
        if(nextmonsterevent < getMsTimer())
        {
            move(monster);
            nextmonsterevent = getMsTimer() + 100;
        }
        
        //Protagonist kann sich bewegen
        if (nextmoveevent < getMsTimer())
        {
            // wenn er nicht rechts gegen eine Plattform stoeßt
            if (B_RIGHT)
            {
                if (protagonist->x + 7 == DISPLAY_WIDTH)
                {
                    ++level_pos;
                    redraw();
                    protagonist->x = 0;
                    //y = 22;
                    draw(protagonist);
                }
                else
                {
                    moveright(protagonist);
                }
                nextmoveevent = getMsTimer() + 50;
            }
            if (B_LEFT)
            {
                if (protagonist->x == 0)
                {
                    --level_pos;
                    redraw();
                    protagonist->x = DISPLAY_WIDTH - protagonist->width;
                    //y = 22;
                    draw(protagonist);
                }
                else
                {
                    moveleft(protagonist);
                }
                nextmoveevent = getMsTimer() + 50;
            }
        }
        if (protagonist->jumpstate != ON_THE_GROUND)
        {
            if (nextjumpevent < getMsTimer())
            {
                jump(protagonist);
                nextjumpevent = getMsTimer() + 150;
            }
        }
        else if (B_UP)
        {
            protagonist->jumpstate = 1;
        }
        
        //falls sich Monster und Character begegnen
        /*if (collision(x, y, a, b))
        {
            clear();
        }*/
        
        if (!projectile_direction && B_A)
        {
            if (protagonist->direction == DIRECTION_LEFT)
            {
                projectile_x = protagonist->x - 2;
                projectile_y = protagonist->y + 1;
                
                projectile_direction = -1;
                nextprojectilevent = getMsTimer();
            }
            else
            {
                projectile_x = protagonist->x + protagonist->width;
                projectile_y = protagonist->y + 1;
                page(projectile_x, projectile_y, 0b11110000);
                projectile_direction = 1;
            }
            page(projectile_x,     projectile_y, 0b11110000);
            page(projectile_x + 1, projectile_y, 0b11110000);
            nextprojectilevent = getMsTimer();
        }
        if (projectile_direction && nextprojectilevent < getMsTimer())
        {
            page(projectile_x,     projectile_y, 0x00);
            page(projectile_x + 1, projectile_y, 0x00);
            if (projectile_x == 0 || projectile_x == DISPLAY_WIDTH - 2)
            {
                projectile_direction = 0;
            }
            else
            {
                projectile_x += projectile_direction;
                page(projectile_x,     projectile_y, 0b00001111);
                page(projectile_x + 1, projectile_y, 0b00001111);
            }
            nextprojectilevent = getMsTimer() + 35;
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
