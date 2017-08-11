#include <stdlib.h>

#include "uart.h"
#include "adc.h"
#include "timer.h"
#include "buttons.h"
#include "display.h"
#include "character.h"
#include "globals.h"
#include "sprites.h"

#define LEVEL_WIDTH 320 // 2*DISPLAY_WIDTH

struct Character* monster;

void init();

// x1,y1 - x1+6,y1+2
int collision(int x1, int y1, int x2, int y2)
{
    return x1 < x2+6 && x1+6 > x2
           && y1 < y2+2 && y1+2 > y2;
}

void drawdoorright_closed()
{
    uint8_t i = 0;
    for (uint8_t y = 20; y < 25; y++)
    {
        for (uint8_t x = 154; x < DISPLAY_WIDTH; x++)
        {
            page(x, y, doorright[i]);
        }
    }
}

void drawdoorleft_closed()
{
    uint8_t i = 0;
    for (uint8_t y = 20; y < 25; y++)
    {
        for (uint8_t x = 0; x < 6; x++)
        {
            page(x, y, doorleft[i]);
        }
    }
}

long level_seed = 3451627918l;
long level_pos = 0;

const uint8_t* floorsprite = NULL;
const uint8_t* nofloorsprite = NULL;

void selectfloor()
{
    srandom(level_seed);
    switch (random() % 3)
    {
        case 0:
            floorsprite = floor1;
            break;
        case 1:
            floorsprite = floor2;
            break;
        case 2:
            floorsprite = floor3;
            break;
    }
    switch (random() % 2)
    {
        case 0:
            nofloorsprite = water;
            break;
        case 1:
            nofloorsprite = spikes;
            break;
    }
}

void drawfloor()
{
    nofloor = random();
    for (uint8_t pos = 0; pos < DISPLAY_WIDTH / 16; ++pos)
    {
        for (int x = 16 * pos; x < 16 * (pos + 1); ++x)
        {
            if (!(nofloor & (7l << 3 * pos))) // there is a 1/8 chance of drawing water/spikes
                page(x, 25, nofloorsprite[x % 16]);
            else
                page(x, 25, floorsprite[x % 16]);
        }
    }
}

void drawplatform()
{
    platforms_20 = random();
    platforms_15 = random();
    platforms_24 = random();
    
    for(uint8_t pos = 0; pos < DISPLAY_WIDTH/PLATFORM_WIDTH; ++pos) // draw random platforms at 20 possible positions
    {
        if (!(platforms_20 & (3l << 2 * pos)))
        {
            for (short i = 0; i < PLATFORM_WIDTH; ++i)
            {
                page(PLATFORM_WIDTH * pos + i, 20, floorsprite[i]);
            }
        }
        if (!(platforms_15 & (3l << 2 * pos)))
        {
            for (short i = 0; i < PLATFORM_WIDTH; ++i)
            {
                page(PLATFORM_WIDTH * pos + i, 15, floorsprite[i]);
            }
        }
        if (!(platforms_24 & (3l << 2 * pos)))
        {
            page(16 * pos,    24, 0b11000000 & floorsprite[0]);
            page(16 * pos+1,  24, 0b11110000 & floorsprite[1]);
            page(16 * pos+2,  24, 0b11111100 & floorsprite[2]);
            page(16 * pos+3,  24,              floorsprite[3]);
            page(16 * pos+4,  24,              floorsprite[4]);
            page(16 * pos+5,  24,              floorsprite[5]);
            page(16 * pos+6,  24,              floorsprite[6]);
            page(16 * pos+7,  24,              floorsprite[7]);
            page(16 * pos+8,  24,              floorsprite[8]);
            page(16 * pos+9,  24,              floorsprite[9]);
            page(16 * pos+10, 24,              floorsprite[10]);
            page(16 * pos+11, 24,              floorsprite[11]);
            page(16 * pos+12, 24,              floorsprite[12]);
            page(16 * pos+13, 24, 0b11111100 & floorsprite[13]);
            page(16 * pos+14, 24, 0b11110000 & floorsprite[14]);
            page(16 * pos+15, 24, 0b11000000 & floorsprite[15]);
        }
    }
}

void drawdigit(uint8_t x, uint8_t y, uint8_t digit)
{
    const uint8_t* sprite = NULL;
    switch (digit)
    {
        case 0:
            sprite = zero;
            break;
        case 1:
            sprite = one;
            break;
        case 2:
            sprite = two;
            break;
        case 3:
            sprite = three;
            break;
        case 4: 
            sprite = four;
            break;
        case 5: 
            sprite = five;
            break;
        case 6:
            sprite = six;
            break;
        case 7:
            sprite = seven;
            break;
        case 8:
            sprite = eight;
            break;
        case 9:
            sprite = nine;
            break;
    }
    uint8_t i = 0;
    for (uint8_t y_ = y; y_ < y + 3; y_++)
    {
        for (uint8_t x_ = x; x_ < x + 3; x_++)
        {
            page(x, y, sprite[i]);
            i++;
        }
    }
}

void drawnumber(uint8_t x, uint8_t y, uint8_t number)
{
    uint8_t leftdigit = number / 10;
    uint8_t rightdigit = number % 10;
    drawdigit(x, y, leftdigit);
    drawdigit(x + 4, y, rightdigit);
}

void redraw()
{
    clear();
    
    // print energy at the top
    uint8_t i = 0;
    for (uint8_t y = 0; y < 3; y++)
    {
        for (uint8_t x = 2; x < 25; x++)
        {
            page(x, y, labelenergy[i]);
            i++;
        }
    }
    
    // print rocket label
    for (uint8_t y = 0; y < 3; y++)
    {
        for (uint8_t x = 40; x < 55; x++)
        {
            page(x, y, labelrocket[i]);
            i++;
        }
    }
    
    // print bomb label
    for (uint8_t y = 0; y < 3; y++)
    {
        for (uint8_t x = 69; x < 83; x++)
        {
            page(x, y, labelbomb[i]);
            i++;
        }
    }
    
    // print ceiling 
    for (uint8_t x = 0; x < DISPLAY_WIDTH; x++)
    {
        page(x, 5, floorsprite[x % 16]);
    }
    
    srandom(level_seed + level_pos);
    drawfloor();
    drawplatform();
    
    monster->look = random() % NUM_MONSTER_LOOKS;
    initcharacter(monster);
    monster->y = 25 - monster->height;
    draw(monster);
}

enum {DOOR_LEFT, DOOR_RIGHT} exitposition = DOOR_RIGHT;

void newlevel()
{
    selectfloor();
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
    
    newlevel();
    redraw();
    
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

        if (protagonist->y > DISPLAY_HEIGHT - protagonist->height) // fell into water/spikes
        {
            clear();
            // GAME OVER
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
