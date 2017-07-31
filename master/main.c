#include <stdlib.h>

#include "uart.h"
#include "adc.h"
#include "timer.h"
#include "buttons.h"
#include "display.h"
#include "character.h"
#include "globals.h"

#define LEVEL_WIDTH 320 // 2*DISPLAY_WIDTH

struct Character* monster;

void init();

// x1,y1 - x1+6,y1+2
int collision(int x1, int y1, int x2, int y2)
{
    return x1 < x2+6 && x1+6 > x2
           && y1 < y2+2 && y1+2 > y2;
}

void drawdoor_closed(int x, int y)
{
    page(x,y, 0xFF);
    page(x+1,y, 0xFF);
    page(x+2,y, 0b11101010);
    page(x+3,y, 0b11111010);
    page(x+4,y, 0b00001110);
    page(x+5,y, 0b00000011);

    page(x,y+1, 0xFF);
    page(x+1,y+1, 0xFF);
    page(x+2,y+1, 0b10111111);
    page(x+3,y+1, 0b10101010);
    page(x+4,y+1, 0b10100101);
    page(x+5,y+1, 0b10010101);

    page(x,y+2, 0xFF);
    page(x+1,y+2, 0xFF);
    page(x+2,y+2, 0b11111110);
    page(x+3,y+2, 0b10101010);
    page(x+4,y+2, 0b01011010);
    page(x+5,y+2, 0b01010110);

    page(x,y+3, 0xFF);
    page(x+1,y+3, 0xFF);
    page(x+2,y+3, 0b10101011);
    page(x+3,y+3, 0b10101111);
    page(x+4,y+3, 0b10110000);
    page(x+5,y+3, 0b11000000);
}

void drawdoor_open(int x, int y)
{
    page(x,y, 0xFF);
    page(x+1,y, 0xFF);
    page(x+2,y, 0b11101010);
    page(x+3,y, 0b11111000);

    page(x,y+1, 0xFF);
    page(x+1,y+1, 0xFF);
    page(x+2,y+1, 0b10111111);
    page(x+3,y+1, 0b00000000);

    page(x,y+2, 0xFF);
    page(x+1,y+2, 0xFF);
    page(x+2,y+2, 0b11111110);
    page(x+3,y+2, 0b00000000);

    page(x,y+3, 0xFF);
    page(x+1,y+3, 0xFF);
    page(x+2,y+3, 0b10101011);
    page(x+3,y+3, 0b00101111);

}

void drawfloor()
{
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

long level_seed = 3451627918l;
long level_pos = 0;
void drawplatform()
{
    srandom(level_seed + level_pos);
    platforms_20 = random();
    platforms_15 = random();
    platforms_24 = random();
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
        if (!(platforms_24 & (3l << 2 * pos)))
        {
            page(PLATFORM_WIDTH * pos, 24, 0b01010110);
            page(PLATFORM_WIDTH * pos+1, 24, 0b10100110);
            page(PLATFORM_WIDTH * pos+2, 24, 0b01010110);
            page(PLATFORM_WIDTH * pos+3, 24, 0b10101010);
            page(PLATFORM_WIDTH * pos+4, 24, 0b10010101);
            page(PLATFORM_WIDTH * pos+5, 24, 0b10011010);
            page(PLATFORM_WIDTH * pos+6, 24, 0b10010101);
            page(PLATFORM_WIDTH * pos+7, 24, 0b10101010);
            page(PLATFORM_WIDTH * pos+8, 24, 0b01100110);
            page(PLATFORM_WIDTH * pos+9, 24, 0b01100110);
        }
    }
}

void redraw()
{
    clear();
    drawfloor();
    drawplatform();
    
    monster->look = random() % NUM_MONSTER_LOOKS;
    initcharacter(monster);
    monster->y = 25 - monster->height;
    draw(monster);
}
    
int main(void)
{
	init();
    
    struct Character protagonist_;
    protagonist = &protagonist_;
    protagonist->look = LOOK_PROTAGONIST;
    initcharacter(protagonist);
    protagonist->x = 10;
    protagonist->y = 22;
    protagonist->direction = DIRECTION_RIGHT;
    draw(protagonist);
    
    struct Character monster_;
    monster = &monster_;
    monster->movement = FOLLOW_PROTAGONIST;
    monster->x = 50;
 
    struct Character projectile_;
    struct Character* projectile = &projectile_;
    projectile->look = LOOK_ROCKET;
    initcharacter(projectile);
    projectile->movement = HIDDEN;

    redraw();
    //drawdoor();
    
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
                if (protagonist->x + protagonist->width == DISPLAY_WIDTH)
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
        
        if (projectile->movement == HIDDEN && B_A)
        {
            uint8_t enough_space = 1;
            projectile->direction = protagonist->direction;
            if (protagonist->direction == DIRECTION_LEFT)
            {
                if (protagonist->x < projectile->width)
                    enough_space = 0;
                projectile->x = protagonist->x - projectile->width;
                projectile->y = protagonist->y + 1;
            }
            else
            {
                if (protagonist->x + protagonist->width + projectile->width >= DISPLAY_WIDTH)
                    enough_space = 0;
                projectile->x = protagonist->x + protagonist->width;
                projectile->y = protagonist->y + 1;
            }
            if (enough_space)
            {
                projectile->movement = PROJECTILE;
                draw(projectile);
                nextprojectilevent = getMsTimer() + 35;
            }
        }
        else if (projectile->movement != HIDDEN && nextprojectilevent < getMsTimer())
        {
            move(projectile);
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
