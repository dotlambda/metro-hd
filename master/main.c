#include <stdlib.h>
#include <avr/pgmspace.h>

#include "uart.h"
#include "adc.h"
#include "timer.h"
#include "buttons.h"
#include "display.h"
#include "character.h"
#include "globals.h"
#include "sprites.h"

#define MAX_LEVEL_WIDTH 6 // max 6 displays for one level
#define MIN_LEVEL_WIDTH 1 

struct Character* monster;

const uint8_t* floorsprite = NULL;
const uint8_t* rotatedfloorsprite = NULL;
const uint8_t* nofloorsprite = NULL;

void init();

// x1,y1 - x1+6,y1+2
int collision(int x1, int y1, int x2, int y2)
{
    return x1 < x2+6 && x1+6 > x2
           && y1 < y2+2 && y1+2 > y2;
}

void drawdoor(int x)
{
    uint8_t i = 0;
    for (uint8_t y = 20; y < 25; y++)
    {
        for (int x_ = x; x_ < x + 33; x_++)
        {
            if (x_ < DISPLAY_WIDTH && x_ >= 0)
            {
                page(x_, y, door[i]);
            }
            i++;
        }
    }
}

void movedoorleft()
{
    clear();
    for (int x = DISPLAY_WIDTH - 6; x >= -33 + 6; x--)
    {
        drawdoor(x);
        if (x + 1 < DISPLAY_WIDTH)
        {
            page(x + 33, 20, 0);
            page(x + 33, 21, 0);
            page(x + 33, 22, 0);
            page(x + 33, 23, 0);
            page(x + 33, 24, 0);
        }
    }
}

void movedoorright()
{
    clear();
    for (int x = -33 + 6; x <= DISPLAY_WIDTH - 6; x++)
    {
        drawdoor(x);
        if (x > 0)
        {
            page(x - 1, 20, 0);
            page(x - 1, 21, 0);
            page(x - 1, 22, 0);
            page(x - 1, 23, 0);
            page(x - 1, 24, 0);
        }
    }
}

void drawdoorright_closed()
{
    uint8_t i = 0;
    for (uint8_t y = 6; y < 20; y++)
    {
        for (uint8_t x = 156; x < DISPLAY_WIDTH; x++)
        {
            page(x, y, rotatedfloorsprite[i]);
            i++;
            if (i == 16)
                i = 0;
        }
    }
    
    i = 0;
    for (uint8_t y = 20; y < 25; y++)
    {
        for (uint8_t x = 154; x < DISPLAY_WIDTH; x++)
        {
            page(x, y, doorright[i]);
            i++;
        }
    }
}

void drawdoorleft_closed()
{
    uint8_t i = 0;
    for (uint8_t y = 6; y < 20; y++)
    {
        for (uint8_t x = 0; x < 4; x++)
        {
            page(x, y, rotatedfloorsprite[i]);
            i++;
            if (i == 16)
                i = 0;
        }
    }
    
    i = 0;
    for (uint8_t y = 20; y < 25; y++)
    {
        for (uint8_t x = 0; x < 6; x++)
        {
            page(x, y, doorleft[i]);
            i++;
        }
    }
}

void selectfloor()
{
    switch (random() % 2)
    {
        case 0l:
            floorsprite = floor1;
            rotatedfloorsprite = floor1_rotated;
            break;
        case 1l:
            /*floorsprite = floor2;
            rotatedfloorsprite = floor2_rotated;
            break;*/
        case 2l:
            floorsprite = floor3;
            rotatedfloorsprite = floor3_rotated;
            break;                           
        default:
            floorsprite = 0;
    }
    switch (random() % 2)
    {
        case 0l:
            nofloorsprite = water;
            break;
        case 1l:
            nofloorsprite = spikes;
            break;
    }
}

void drawfloor()
{
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
    for (uint8_t pos = 0; pos < DISPLAY_WIDTH/PLATFORM_WIDTH; ++pos) // draw random platforms at 20 possible positions
    {
        if (!(platforms_19 & (3l << 2 * pos)))
        {
            for (short i = 0; i < PLATFORM_WIDTH; ++i)
            {
                page(PLATFORM_WIDTH * pos + i, 19, floorsprite[i]);
            }
        }
        if (!(platforms_13 & (3l << 2 * pos)))
        {
            for (short i = 0; i < PLATFORM_WIDTH; ++i)
            {
                page(PLATFORM_WIDTH * pos + i, 13, floorsprite[i]);
            }
        }
    }
    for (uint8_t pos = 0; pos < DISPLAY_WIDTH/16; ++pos)
    {
        if (!(platforms_24 & (3l << 2 * pos)))
        {
            page(16 * pos,      24, 0b11000000 & floorsprite[0]);
            page(16 * pos + 1,  24, 0b11110000 & floorsprite[1]);
            page(16 * pos + 2,  24, 0b11111100 & floorsprite[2]);
            page(16 * pos + 3,  24,              floorsprite[3]);
            page(16 * pos + 4,  24,              floorsprite[4]);
            page(16 * pos + 5,  24,              floorsprite[5]);
            page(16 * pos + 6,  24,              floorsprite[6]);
            page(16 * pos + 7,  24,              floorsprite[7]);
            page(16 * pos + 8,  24,              floorsprite[8]);
            page(16 * pos + 9,  24,              floorsprite[9]);
            page(16 * pos + 10, 24,              floorsprite[10]);
            page(16 * pos + 11, 24,              floorsprite[11]);
            page(16 * pos + 12, 24,              floorsprite[12]);
            page(16 * pos + 13, 24, 0b11111100 & floorsprite[13]);
            page(16 * pos + 14, 24, 0b11110000 & floorsprite[14]);
            page(16 * pos + 15, 24, 0b11000000 & floorsprite[15]);
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
            page(x_, y_, sprite[i]);
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

enum {DOOR_LEFT, DOOR_RIGHT} exitposition = DOOR_RIGHT;

void redraw()
{
    clear();
    
    // print energy at the top
    uint8_t i = 0;
    for (uint8_t y = 1; y < 4; y++)
    {
        for (uint8_t x = 2; x < 25; x++)
        {
            page(x, y, labelenergy[i]);
            i++;
        }
    }
    
    // print rocket label
    i = 0;
    for (uint8_t y = 1; y < 4; y++)
    {
        for (uint8_t x = 40; x < 55; x++)
        {
            page(x, y, labelrocket[i]);
            i++;
        }
    }
    
    // print bomb label
    i = 0;
    for (uint8_t y = 1; y < 4; y++)
    {
        for (uint8_t x = 69; x < 83; x++)
        {
            page(x, y, labelbomb[i]);
            i++;
        }
    }
    
    // print number
    drawnumber(57, 1, 13);
    
    // print ceiling 
    for (uint8_t x = 0; x < DISPLAY_WIDTH; x++)
    {
        page(x, 5, floorsprite[x % 16]);
    }
    
    srandom(level_seed + level_pos);
    platforms_19 = random();
    platforms_13 = random();
    platforms_24 = random();
    nofloor = random();
    nofloor = INT32_MAX; // turn off water
    
    drawplatform();
    drawfloor();
    
    doors = 0;
    
    // draw door to previous level
    if (level_pos == 0)
    {
        if (exitposition == DOOR_RIGHT)
        {
            drawdoorleft_closed();
            doors |= 0b00000010;
        }
        else
        {
            drawdoorright_closed();
            doors |= 0b00000001;
        }
    }
    
    // draw exit door
    if (level_pos == MAX_LEVEL_WIDTH - 1)
    {
        drawdoorright_closed();
        doors |= 0b00000001;
    }
    else if (level_pos == -MAX_LEVEL_WIDTH + 1)
    {
        drawdoorleft_closed();
        doors |= 0b00000010;
    }
    else if (random() % 5 == 0)
    {
        if (exitposition == DOOR_RIGHT)
        {
            drawdoorright_closed();
            doors |= 0b00000001;
        }
        else
        {
            drawdoorleft_closed();
            doors |= 0b00000010;
        }
    }
    
    monster->look = random() % NUM_MONSTER_LOOKS;
    initcharacter(monster);
    monster->y = 25 - monster->height;
    draw(monster);

    draw(protagonist);
}

void newlevel()
{
    if ((exitposition == DOOR_LEFT && protagonist->x <= DISPLAY_WIDTH / 2) 
        || (exitposition == DOOR_RIGHT && protagonist->x > DISPLAY_WIDTH / 2))
    {
        level_seed += 2 * MAX_LEVEL_WIDTH;
    }
    else // back to the previous level
    {
        level_seed -= 2 * MAX_LEVEL_WIDTH;
    }
    
    if (protagonist->x > DISPLAY_WIDTH / 2)
    {
        exitposition = DOOR_RIGHT;
        movedoorleft();
    }
    else
    {
        exitposition = DOOR_LEFT;
        movedoorright();
    }
    
    if (protagonist->x > DISPLAY_WIDTH / 2)
    {
        protagonist->x = 6 + 1;
        protagonist->direction = DIRECTION_RIGHT;
    }
    else
    {
        protagonist->x = DISPLAY_WIDTH - 6 - protagonist->width - 1;
        protagonist->direction = DIRECTION_LEFT;
    }
    protagonist->y = 25 - protagonist->height;
    
    level_pos = 0;
    srandom(level_seed);
    selectfloor();
}

int main(void)
{
	init();
    uint16_t i = 0;
    for (uint8_t y = 3; y < 3 + 20; y++)
    {
        for (uint8_t x = 22; x < 22 + 115; x++)
        {
            page(x, y, pgm_read_byte_near(splash + i));
            i++;
        }
    }
    while (!B_A);
    
    // show splash screen until button A is pressed
    
    struct Character protagonist_;
    protagonist = &protagonist_;
    protagonist->look = LOOK_PROTAGONIST;
    initcharacter(protagonist);
    protagonist->x = DISPLAY_WIDTH;
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
    
    level_seed = 3451627918l;
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
            if (B_RIGHT)
            {
                if (protagonist->x + protagonist->width == DISPLAY_WIDTH)
                {
                    long obstacle = 0l;
                    for (uint8_t x = 0; x < protagonist->width; ++x)
                    {
                        for (uint8_t y = protagonist->y; y < protagonist->y + protagonist->height; ++y)
                        {
                            obstacle |= obstacle_levelpos(x, y, level_pos + 1);
                        }
                    }
                    if (!obstacle)
                    {
                        ++level_pos;
                        protagonist->x = 0;
                        redraw();
                        checkfalling(protagonist);
                    }
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
                    long obstacle = 0l;
                    for (uint8_t x = DISPLAY_WIDTH - protagonist->width; x < DISPLAY_WIDTH; ++x)
                    {
                        for (uint8_t y = protagonist->y; y < protagonist->y + protagonist->height; ++y)
                        {
                            obstacle |= obstacle_levelpos(x, y, level_pos - 1);
                        }
                    }
                    if (!obstacle)
                    {
                        --level_pos;
                        protagonist->x = DISPLAY_WIDTH - protagonist->width;
                        redraw();
                        checkfalling(protagonist);
                    }
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
        
        // change level when protagonist touches the door
        if (doors & 0b00000001
            && protagonist->x >= DISPLAY_WIDTH - 6 - protagonist->width 
            && protagonist->y >= 20 - protagonist->height)
        {
            newlevel();
            redraw();
        }
        else if (doors & 0b00000010
            && protagonist->x <= 6 
            && protagonist->y >= 20 - protagonist->height)
        {
            newlevel();
            redraw();
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
