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
#include "rand.h"

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

#define MAX_LEVEL_WIDTH 6 // max 6 displays for one level
#define MIN_LEVEL_WIDTH 1

#define DIST_AFTER_DAMAGE 10


struct Character* monster;
struct Character* projectile;
struct Character* bombstruct;
uint8_t num_rockets;
uint8_t num_bombs;
uint8_t energytankexists;
enum {DOOR_LEFT, DOOR_RIGHT} door_back;

uint32_t nextmoveevent;
uint32_t nextjumpevent;
uint32_t nextprojectilevent;
uint32_t nextbombevent;
uint32_t explode;
uint32_t nextmonstermoveevent;
uint32_t nextmonsterjumpevent;

const uint8_t* floorsprite = NULL;
const uint8_t* rotatedfloorsprite = NULL;
const uint8_t* nofloorsprite = NULL;

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
    
    drawnumber(29, 1, protagonist->health);
    drawnumber(57, 1, num_rockets);
    drawnumber(86, 1, num_bombs);
}

void drawenergytank(uint8_t x, uint8_t y)
{
    uint8_t i = 0;
    for (uint8_t y_ = y; y_ < y + 2; y_++)
    {
        for(uint8_t x_ = x; x_ < x + 9; x_++)
        {
            page(x_, y_, pgm_read_byte_near(energytank + i));
            i++;
        }
    }
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
    switch (random_below(4))
    {
        case 0l:
            floorsprite = floor1;
            rotatedfloorsprite = floor1_rotated;
            break;
        case 1l:
            floorsprite = floor2;
            rotatedfloorsprite = floor2_rotated;
            break;
        case 2l:
            floorsprite = floor3;
            rotatedfloorsprite = floor3_rotated;
            break;                           
        case 3l:
            floorsprite = floor4;
            rotatedfloorsprite = floor4_rotated;
            break;
    }
    switch (random_below(2))
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

void redraw()
{
    clear();
    
    drawlabels();
    
    // print ceiling 
    for (uint8_t x = 0; x < DISPLAY_WIDTH; x++)
    {
        page(x, 5, pgm_read_byte_near(floorsprite + x % 16));
    }

    drawplatform();
    drawfloor();

    if (doors & 0b00000010)
        drawdoorleft_closed();
    if (doors & 0b00000001)
        drawdoorright_closed();

    if (monster->movement != HIDDEN)
        draw(monster);
    if (projectile->movement != HIDDEN)
        draw(projectile);
    if (bombstruct->movement != HIDDEN)
        draw(bombstruct);
   
    if (obstacle(90, 19))
    {
        drawenergytank(90, 17);
    }
    draw(protagonist);
}

void newlevelpos()
{
    srand(level_seed + level_pos);
    srandom(level_seed + level_pos);
    
    platforms_13 = random();
    platforms_19 = random();
    platforms_24 = random();
    platforms_24 |= 3l << 0; // no hill at the display boundary
    platforms_24 |= 3l << 2 * (DISPLAY_WIDTH/16 - 1);
    nofloor = random();
    nofloor = INT32_MAX; // turn off water
    doors = 0;
    
    // draw door to previous level
    if (level_pos == 0)
    {
        if (door_back == DOOR_LEFT)
            doors |= 0b00000010;
        else
            doors |= 0b00000001;
    }

    // draw exit door
    if (level_pos == MAX_LEVEL_WIDTH - 1 && door_back == DOOR_LEFT)
    {
        doors |= 0b00000001;
    }
    else if (level_pos == -MAX_LEVEL_WIDTH + 1 && door_back == DOOR_RIGHT)
    {
        doors |= 0b00000010;
    }
    else if (random_below(5) == 0)
    {
        if (door_back == DOOR_LEFT)
        {
            doors |= 0b00000001;
        }
        else
        {
            doors |= 0b00000010;
        }
    }
    
    monster->look = random_below(NUM_MONSTER_LOOKS);
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
        
    projectile->movement = HIDDEN;
    bombstruct->movement = HIDDEN;

    redraw();
}

void newlevel()
{
    if (protagonist->x > DISPLAY_WIDTH / 2)
    {
        level_seed += 2 * MAX_LEVEL_WIDTH + 1;
        door_back = DOOR_LEFT;
    }
    else // back to the previous level
    {
        level_seed -= 2 * MAX_LEVEL_WIDTH - 1;
        door_back = DOOR_RIGHT;
    }

    if (protagonist->x > DISPLAY_WIDTH / 2)
    {
        movedoorleft();
    }
    else
    {
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
    
    srand(level_seed);
    srandom(level_seed);

    selectfloor();

    level_pos = 0;
    newlevelpos();
}

void newgame()
{
    level_seed = 2845215237l;
    num_rockets = 20;
    num_bombs = 20;

    protagonist->look = LOOK_PROTAGONIST;
    initcharacter(protagonist);
    protagonist->x = DISPLAY_WIDTH; // make the protagonist appear on the left

    projectile->look = LOOK_ROCKET;
    initcharacter(projectile);
    
    bombstruct->look = LOOK_BOMB;
    initcharacter(bombstruct);

    nextmoveevent = 0;
    nextjumpevent = 0;
    nextprojectilevent = 0;
    nextbombevent = 0;
    nextmonstermoveevent = 0;
    nextmonsterjumpevent = 0;

    newlevel();
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
        blink_for = 2000;
    }
    uint32_t blinking_time = getMsTimer();
    while(blinking_time + blink_for >= getMsTimer())
    {
        hide(protagonist);
        if (protagonist->health <= 0)
        {
            for (uint8_t y = 1; y < 4; y++)
            {
                for (uint8_t x = 29; x < 36; x++)
                {
                    page(x, y, 0);
                }
            }
        }
        delay(50);
        draw(protagonist);
        if (protagonist->health <= 0)
        {    
            drawnumber(29, 1, 0);
        }
        delay(100);
    }
    if (protagonist->health <= 0)
    {
        uint16_t i = 0;
        for (uint8_t y = 0; y < DISPLAY_HEIGHT; y++)
        {
            for (uint8_t x = 0; x < DISPLAY_WIDTH; x++)
            {
                if (y > 7 && y < 21 && x > 17 && x < 141)
                {
                    page(x, y, pgm_read_byte_near(gameover + i));
                    i++;
                }
                else
                {
                    page(x, y, 0);
                }
            }
            delay(30);
        }
        while (!B_A);
        newgame();
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

    // show splash screen until button A is pressed
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
    
    struct Character protagonist_;
    protagonist = &protagonist_;
        
    struct Character monster_;
    monster = &monster_;
 
    struct Character projectile_;
    projectile = &projectile_;
    
    struct Character bomb_;
    bombstruct = &bomb_;
        
    newgame();
    
    while (1)
    {
        //monster in Bewegung
        if(monster->movement != HIDDEN && nextmonstermoveevent < getMsTimer())
        {
            move(monster);
            nextmonstermoveevent = getMsTimer() + 100;
        }
        if(monster->movement != HIDDEN && nextmonsterjumpevent < getMsTimer())
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
                        newlevelpos();
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
                        newlevelpos();
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
        }
        else if (doors & 0b00000010
            && protagonist->x <= 6 
            && protagonist->y >= 20 - protagonist->height)
        {
            newlevel();
        }
        
        //falls sich Monster und Character begegnen
        /*if (collision(x, y, a, b))
        {
            clear();
        }*/
        
        if (projectile->movement == HIDDEN
            && num_rockets > 0
            && nextprojectilevent < getMsTimer()
            && B_A)
        {
            uint8_t enough_space = 1;
            projectile->direction = protagonist->direction;
            projectile->y = protagonist->y + 1;
            if (protagonist->direction == DIRECTION_LEFT)
            {
                if (protagonist->x < projectile->width)
                    enough_space = 0;
                projectile->x = protagonist->x - projectile->width;
            }
            else
            {
                if (protagonist->x + protagonist->width + projectile->width >= DISPLAY_WIDTH)
                    enough_space = 0;
                projectile->x = protagonist->x + protagonist->width;
            }
            if (enough_space)
            {
                for (uint8_t x = projectile->x; x < projectile->x + projectile->width; ++x)
                {
                    for (uint8_t y = projectile->y; y < projectile->y + projectile->height; ++y)
                    {
                        if (obstacle(x, y))
                            enough_space = 0;
                    }
                }
            }
            if (enough_space)
            {
                projectile->movement = PROJECTILE;
                draw(projectile);
                num_rockets--;
                drawnumber(57, 1, num_rockets);
                nextprojectilevent = getMsTimer() + 35;
            }
        }
        else if (projectile->movement != HIDDEN
            && nextprojectilevent < getMsTimer())
        {
            move(projectile);
            if (projectile->movement == HIDDEN)
                nextprojectilevent = getMsTimer() + 500;
            else
                nextprojectilevent = getMsTimer() + 35;
        }
        
        if (nextbombevent < getMsTimer())
        {
            if (bombstruct->movement != HIDDEN)
            {
                jump(bombstruct);
                nextbombevent = getMsTimer() + 100;
            }
            else if (B_DOWN && num_bombs > 0)
            {
                bombstruct->x = protagonist->x;
                bombstruct->y = protagonist->y + protagonist->height - bombstruct->height;
                bombstruct->movement = BOMB;
                num_bombs--;
                drawnumber(86, 1, num_bombs);
                checkfalling(bombstruct);
                explode = getMsTimer() + 2000;
                nextbombevent = getMsTimer() + 100;
            }
        }
        if (bombstruct->movement != HIDDEN)
        {
            if (explode < getMsTimer())
            {
                hide(bombstruct);
                uint8_t blast_x1 = MAX(0, bombstruct->x - 6);
                uint8_t blast_x2 = MIN(bombstruct->x + bombstruct->width + 6, DISPLAY_WIDTH);
                uint8_t blast_y1 = MAX(6, bombstruct->y - 1);
                uint8_t blast_y2 = MIN(bombstruct->y + bombstruct->height + 3, 25);
                uint16_t i = 0;
                for (int16_t y = bombstruct->y - 1; y < bombstruct->y + 3; y++)
                {
                    for (int16_t x = bombstruct->x - 6; x < bombstruct->x + 10; x++)
                    {
                        if (x >= 0 && x < DISPLAY_WIDTH
                            && y >= 6 && y < 25
                            && !obstacle(x, y))
                        {
                            page(x, y, pgm_read_byte_near(explosion + i));
                        }
                        i++;
                    }
                }
                delay(600);
                i = 0;
                for (int16_t y = bombstruct->y - 1; y < bombstruct->y + 3; y++)
                {
                    for (int16_t x = bombstruct->x - 6; x < bombstruct->x + 10; x++)
                    {   
                        if (x >= 0 && x < DISPLAY_WIDTH
                            && y >= 6 && y < 25
                            && !obstacle(x, y))
                        {
                            page(x, y, 0);
                        }
                        i++;
                    }
                } 
                if (monster->movement != HIDDEN &&
                    blast_x1 < monster->x + monster->width && blast_x2 > monster->x &&
                    blast_y1 < monster->y + monster->height && blast_y2 > monster->y)
                {
                    monster->health -= bombstruct->damage;
                    if (monster->health <= 0)
                        hide(monster);
                    else
                        draw(monster);
                }
                if (blast_x1 < protagonist->x + protagonist->width && blast_x2 > protagonist->x &&
                    blast_y1 < protagonist->y + protagonist->height && blast_y2 > protagonist->y)
                {
                    takingdamage(bombstruct->damage);
                }
            }
            else
            {
                draw(bombstruct);
            }
        }
            

        /*if (protagonist->y > DISPLAY_HEIGHT - protagonist->height) // fell into water/spikes
        {
            Game_Over();
        }*/
        
        if (monster->movement != HIDDEN && collision(protagonist, monster))
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

        if (projectile->movement != HIDDEN && monster->movement != HIDDEN && collision(projectile, monster))
        {
            hide(projectile);
            monster->health -= projectile->damage;
            if (monster->health <= 0)
                hide(monster);
            else
                draw(monster);
            nextprojectilevent = getMsTimer() + 500;
        }

        if(B_PAUSE)
        {
            while (B_PAUSE); // wait until button is released
            clear();
            uint16_t i = 0;
            for (uint8_t y = 12; y < 16 ; y++)
            {
                for (uint8_t x = 60; x < 99; x++)
                {
                    page(x, y, pgm_read_byte_near(pause + i));
                    i++;
                }
            }
            while (!B_PAUSE);
            while (B_PAUSE); // wait until button is released
            redraw();
        
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
