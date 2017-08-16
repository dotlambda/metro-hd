#include <stdlib.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdbool.h>
#include <util/delay.h>
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

#define INITIAL_LEVEL 3451627918l

#define DIST_AFTER_DAMAGE 10

struct Character* monster;

const uint8_t* floorsprite = NULL;
const uint8_t* rotatedfloorsprite = NULL;
const uint8_t* nofloorsprite = NULL;
bool Title_ = true;
bool Game_Over_ = false;

void init();

// delay without interrupts
void delay(uint32_t ms)
{
    uint32_t time = getMsTimer();
    while (getMsTimer() < time + ms);
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
            page(x_, y_, pgm_read_byte_near(sprite + i));
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

void drawlabels()
{
    // print energy at the top
    uint8_t i = 0;
    for (uint8_t y = 1; y < 4; y++)
    {
        for (uint8_t x = 2; x < 25; x++)
        {
            page(x, y, pgm_read_byte_near(labelenergy + i));
            i++;
        }
    }
    
    // print rocket label
    i = 0;
    for (uint8_t y = 1; y < 4; y++)
    {
        for (uint8_t x = 40; x < 55; x++)
        {
            page(x, y, pgm_read_byte_near(labelrocket + i));
            i++;
        }
    }
    
    // print bomb label
    i = 0;
    for (uint8_t y = 1; y < 4; y++)
    {
        for (uint8_t x = 69; x < 83; x++)
        {
            page(x, y, pgm_read_byte_near(labelbomb + i));
            i++;
        }
    }
    
    // print number
    drawnumber(58, 1, 13);
    drawnumber(29, 1, protagonist->health);
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
                page(x_, y, pgm_read_byte_near(doorinverted + i));
            }
            i++;
        }
    }
}

void black()
{
    sendbyte(0, 0);
    sendbyte(16, 0);
    sendbyte(96, 0);
    for (uint16_t i = 0; i < 5 * DISPLAY_WIDTH; i++)
    {
        sendbyte(0, 1);
    }
    for (uint16_t i = 5 * DISPLAY_WIDTH; i < DISPLAY_WIDTH * DISPLAY_HEIGHT; i++)
    {
        sendbyte(0xFF, 1);
    }
    sendbyte(0, 0);
    sendbyte(16, 0);
    sendbyte(96, 0);
}

void movedoorleft()
{
    black();
    drawlabels();
    for (int x = DISPLAY_WIDTH - 6; x >= -33 + 6; x--)
    {
        drawdoor(x);
        if (x + 1 < DISPLAY_WIDTH)
        {
            page(x + 33, 20, 0xFF);
            page(x + 33, 21, 0xFF);
            page(x + 33, 22, 0xFF);
            page(x + 33, 23, 0xFF);
            page(x + 33, 24, 0xFF);
        }
    }
}

void movedoorright()
{
    black();
    drawlabels();
    for (int x = -33 + 6; x <= DISPLAY_WIDTH - 6; x++)
    {
        drawdoor(x);
        if (x > 0)
        {
            page(x - 1, 20, 0xFF);
            page(x - 1, 21, 0xFF);
            page(x - 1, 22, 0xFF);
            page(x - 1, 23, 0xFF);
            page(x - 1, 24, 0xFF);
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
            page(x, y, pgm_read_byte_near(rotatedfloorsprite + i));
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
            page(x, y, pgm_read_byte_near(doorright + i));
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
            page(x, y, pgm_read_byte_near(rotatedfloorsprite + i));
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
            page(x, y, pgm_read_byte_near(doorleft + i));
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
                page(x, 25, pgm_read_byte_near(nofloorsprite + x % 16));
            else
                page(x, 25, pgm_read_byte_near(floorsprite + x % 16));
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
                page(PLATFORM_WIDTH * pos + i, 19, pgm_read_byte_near(floorsprite + i));
            }
        }
        if (!(platforms_13 & (3l << 2 * pos)))
        {
            for (short i = 0; i < PLATFORM_WIDTH; ++i)
            {
                page(PLATFORM_WIDTH * pos + i, 13, pgm_read_byte_near(floorsprite + i));
            }
        }
    }
    for (uint8_t pos = 0; pos < DISPLAY_WIDTH/16; ++pos)
    {
        if (!(platforms_24 & (3l << 2 * pos)))
        {
            page(16 * pos,      24, 0b11000000 & pgm_read_byte_near(floorsprite + 0));
            page(16 * pos + 1,  24, 0b11110000 & pgm_read_byte_near(floorsprite + 1));
            page(16 * pos + 2,  24, 0b11111100 & pgm_read_byte_near(floorsprite + 2));
            page(16 * pos + 3,  24,              pgm_read_byte_near(floorsprite + 3));
            page(16 * pos + 4,  24,              pgm_read_byte_near(floorsprite + 4));
            page(16 * pos + 5,  24,              pgm_read_byte_near(floorsprite + 5));
            page(16 * pos + 6,  24,              pgm_read_byte_near(floorsprite + 6));
            page(16 * pos + 7,  24,              pgm_read_byte_near(floorsprite + 7));
            page(16 * pos + 8,  24,              pgm_read_byte_near(floorsprite + 8));
            page(16 * pos + 9,  24,              pgm_read_byte_near(floorsprite + 9));
            page(16 * pos + 10, 24,              pgm_read_byte_near(floorsprite + 10));
            page(16 * pos + 11, 24,              pgm_read_byte_near(floorsprite + 11));
            page(16 * pos + 12, 24,              pgm_read_byte_near(floorsprite + 12));
            page(16 * pos + 13, 24, 0b11111100 & pgm_read_byte_near(floorsprite + 13));
            page(16 * pos + 14, 24, 0b11110000 & pgm_read_byte_near(floorsprite + 14));
            page(16 * pos + 15, 24, 0b11000000 & pgm_read_byte_near(floorsprite + 15));
        }
    }
}

enum {DOOR_LEFT, DOOR_RIGHT} exitposition = DOOR_RIGHT;

void redraw()
{
    clear();
    
    drawlabels();
    
    // print ceiling 
    for (uint8_t x = 0; x < DISPLAY_WIDTH; x++)
    {
        page(x, 5, pgm_read_byte_near(floorsprite + x % 16));
    }
    
    srandom(level_seed + level_pos);
    platforms_19 = random();
    platforms_13 = random();
    platforms_24 = random();
    platforms_24 |= 3l << 0; // no hill at the display boundary
    platforms_24 |= 3l << 2 * (DISPLAY_WIDTH/16 - 1); 
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
    if (level_pos == MAX_LEVEL_WIDTH - 1 && exitposition == DOOR_RIGHT)
    {
        drawdoorright_closed();
        doors |= 0b00000001;
    }
    else if (level_pos == -MAX_LEVEL_WIDTH + 1 && exitposition == DOOR_LEFT)
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
    monster->x = (DISPLAY_WIDTH - monster->width) / 2;
    monster->y = 25 - monster->height;
    // draw monster higher if it's on a hill
    for (uint8_t x = monster->x; x < monster->x + monster->width; x++)
    {
        if (obstacle_hill(x))
        {
            monster->y--;
            break;
        }
    }
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

void takingdamage(uint8_t damage)
{
    uint32_t blink_for = 650;
    protagonist->health = protagonist->health - damage;
    if(protagonist->health > 0)
    {
        drawnumber(29, 1, protagonist->health);
    }
    else
    {
        drawnumber(29, 1, 0);
        uint16_t i = 0;
        for (uint8_t y = 2; y < 9; y++)
        {
            for (uint8_t x = 20; x < 140; x++)
            {
                page(x, y, pgm_read_byte_near(energy0 + i));
                i++;
            }
        }
        blink_for = 2000;
    }
    uint32_t blinking_time = getMsTimer();
    while(blinking_time + blink_for >= getMsTimer())
    {
        hide(protagonist);
        delay(50);
        draw(protagonist);
        delay(100);
    }
    if (protagonist->health <= 0)
    {
        clear();
        uint16_t i = 0;
        for (uint8_t y = 5; y < 13 ; y++)
        {
            for (uint8_t x = 51; x < 108; x++)
            {
                page(x, y, pgm_read_byte_near(game_over + i));
                i++;
            }
        }
        i = 0;
        for (uint8_t y = 20; y < 23; y++)
        {
            for (uint8_t x = 32; x < 128; x++)
            {
                page(x, y, pgm_read_byte_near(restart + i));
                i++;
            }
        }
        while (!B_A);
        level_seed = INITIAL_LEVEL;
        protagonist->health = 99;
        newlevel();
        redraw();
    }

}

bool collision(struct Character* protagonist, struct Character* monster)
{
    return (protagonist->x < monster->x + monster->width && protagonist->x + protagonist->width > monster->x &&
        protagonist->y < monster->y + monster->height && protagonist->y + protagonist->height > monster->y);
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
    
    level_seed = INITIAL_LEVEL;
    newlevel();
    redraw();
    
    uint32_t nextmoveevent = 0;
    uint32_t nextjumpevent = 0;
    uint32_t nextprojectilevent = 0;
    uint32_t nextmonstermoveevent = 0;
    uint32_t nextmonsterjumpevent = 0;
    while (1)
    {
        //monster in Bewegung
        if(nextmonstermoveevent < getMsTimer())
        {
            move(monster);
            nextmonstermoveevent = getMsTimer() + 100;
        }
        if(nextmonsterjumpevent < getMsTimer())
        {
            jump(monster);
            nextmonsterjumpevent = getMsTimer() + 150;
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

        /*if (protagonist->y > DISPLAY_HEIGHT - protagonist->height) // fell into water/spikes
        {
            Game_Over();
        }*/
        
        if (collision(protagonist, monster))
        {
            // if the monster is right of the protagonist
            if (monster->x + monster->width/2 >= protagonist->x + protagonist->width)
            {
                uint8_t i = 0;
                while (protagonist->x + protagonist->width + DIST_AFTER_DAMAGE > monster->x)
                {
                    if (!moveleft(protagonist))
                        break;
                    if (i == 0 || i == 2)
                        moveup(protagonist);
                    if (i > 3 && i % 2 == 0)
                        movedown(protagonist);
                    i++;
                    draw(monster);
                    delay(50);
                }
                while (protagonist->x + protagonist->width + DIST_AFTER_DAMAGE > monster->x)
                {
                    if (!moveright(monster))
                        break;
                }
            }
            else
            {
                uint8_t i = 0;
                while (monster->x + monster->width + DIST_AFTER_DAMAGE > protagonist->x)
                {
                    if (!moveright(protagonist))
                        break;
                    if (i == 0 || i == 2)
                        moveup(protagonist);
                    if (i > 3 && i % 2 == 0)
                        movedown(protagonist);
                    i++;
                    draw(monster);
                    delay(50);
                }
                while (monster->x + monster->width + DIST_AFTER_DAMAGE > protagonist->x)
                {
                    if (!moveleft(monster))
                        break;
                }
            }
            takingdamage(monster->damage);
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
