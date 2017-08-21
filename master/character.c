#include <stdlib.h>
#include <avr/pgmspace.h>
#include "timer.h"
#include "character.h"
#include "level.h"
#include "display.h"
#include "sprites.h"
#include "rand.h"

void initcharacter(struct Character* character)
{
    character->health = 4;
    character->movement = FOLLOW_PROTAGONIST;
    character->jumpheight = 7;
    switch (character->look)
    {
        case LOOK_PROTAGONIST:
            character->width = 8;
            character->height = 4;
            character->health = 99;
            break;
        case LOOK_MONSTER_LITTLE: 
            character->width = 8;
            character->height = 2;
            character->damage = 10;
            break;
        case LOOK_EYEMONSTER:
            character->width = 12;
            character->height = 4;
            character->damage = 15;
            break;
        case LOOK_MONSTER_ZOOMER:
            character->width = 15;
            character->height = 3;
            character->damage = 20;
            break;
        case LOOK_ROCKET:
            character->width = 14;
            character->height = 2;
            character->damage = 2;
            character->movement = HIDDEN;
            break;
        case LOOK_BOSS_ZAZABI:
            character->width = 17;
            character->height = 8;
            character->damage = 30;
            break;
        case LOOK_MONSTER_METROID:
            character->width = 14;
            character->height = 3;
            character->damage = 15;
            break;
        case LOOK_MONSTER_HORNOAD:
            character->width = 14;
            character->height = 3;
            character->damage = 15;
            character->jumpheight = 5;
            character->movement = JUMPMOVE;
            break;
        case LOOK_MONSTER_SIDEHOPPER:
            character->width = 16;
            character->height = 3;
            character->damage = 20;
            character->jumpheight = 10;
            character->movement = JUMPMOVE;
            break;
        case LOOK_MONSTER_MEMU:
            character->width = 15;
            character->height = 2;
            character->damage = 10;
            character->movement = FLYING_AROUND;
            break;
        case LOOK_BOSS_DRAGON:
            character->width = 30;
            character->height = 9;
            character->damage = 15;
            character->movement = XPARASITE;
            break;
        case LOOK_BOMB:
            character->width = 4;
            character->height = 1;
            character->damage = 5;
            character->movement = HIDDEN;
            break;
        case LOOK_MONSTER_GEEGA:
            character->width = 16;
            character->height = 4;
            character->damage = 5;
            character->movement = FLYING_AROUND;
            break;
        case LOOK_XPARASITE1:
            character->width = 6;
            character->height = 2;
            character->movement = XPARASITE;
            break;
        case LOOK_XPARASITE2:
            character->width = 6;
            character->height = 2;
            character->movement = XPARASITE;
            break;
        case LOOK_ENERGYTANK:
            character->width = 9;
            character->height = 2;
            character->movement = ENERGYTANK;
            break;
        case LOOK_BOSS_SECROB:
            character->width = 37;
            character->height = 6;
            character->movement = FOLLOW_PROTAGONIST;
        case LOOK_FIREBALL:
            character->width = 8;
            character->height = 2;
            character->damage = 20;
            character->movement = HIDDEN;
    }
    character->lookstate = 0;
    character->lastlookstatechg = getMsTimer();
    character->jumpstate = ON_THE_GROUND;
}

void draw(struct Character* character)
{
    const uint8_t* sprite = NULL;
    switch (character->look)
    {
        case LOOK_PROTAGONIST:
            if (character->lastlookstatechg + 300 < getMsTimer())
            {
                character->lookstate = 1 - character->lookstate;
                character->lastlookstatechg = getMsTimer();
            }
            if (character->direction == DIRECTION_LEFT)
            {
                if (character->lookstate)
                {
                    sprite = protagonistleft;
                }
                else 
                {
                    sprite = protagleftwalk;
                }
            }
            else
            {
                if (character->lookstate)
                {
                    sprite = protagonistright;
                }   
                else 
                {
                    sprite = protagrightwalk;
                }
            }
            break;

        case LOOK_MONSTER_LITTLE:
            sprite = little;
            break;
            
        case LOOK_EYEMONSTER:
            if(character->direction == DIRECTION_LEFT)
            {
                sprite = eyemonster_left;
            }
            else
            {
                sprite = eyemonster_right;
            }
            break;
        
        case LOOK_MONSTER_ZOOMER:
            sprite = zoomer;
            break;
            
        case LOOK_ROCKET:
            if(character->direction == DIRECTION_LEFT)
            {
                sprite = rocket;
            }
            else
            {
                sprite = rocketinverted;
            }
            break;
            
        case LOOK_BOSS_ZAZABI:
            sprite = zazabi;
            break;

        case LOOK_MONSTER_METROID:
            sprite = metroid;
            break;
        

        case LOOK_MONSTER_HORNOAD:
            if (character->direction == DIRECTION_LEFT)
            {
                sprite = hornoadleft;
            }
            else
            {
                sprite = hornoadright;
            }
            break;

        case LOOK_MONSTER_SIDEHOPPER:
            sprite = sidehopper;
            break;

        case LOOK_MONSTER_MEMU:
            if (character->lookstate) // wings down
            {
                sprite = memu0;
            }
            else // wings up
            {
                sprite = memu1;
            }
            // toggle wing state
            character->lookstate = 1 - character->lookstate;
            break;

        case LOOK_BOSS_DRAGON:
            if (character->direction == DIRECTION_LEFT)
            {
                sprite = dragon_left;
            }
            else
            {
                sprite = dragon_right;
            }
            break;
        case LOOK_BOMB:
            sprite = bomb;
            break;
        case LOOK_MONSTER_GEEGA:
            if(character->direction == DIRECTION_RIGHT)
            {
                if (character->lookstate) // wings up and moving left
                {
                    sprite = geega1;
                }
                else// wings down
                {
                    sprite = geega2;
                }
            }
            else
            {
                if (character->lookstate) // wings up and moving right
                {
                    sprite = geega1inverted;
                }
                else// wings down
                {
                    sprite = geega2inverted;
                }
            }

            // toggle wing state
            if (character->lastlookstatechg < getMsTimer())
            {
                character->lookstate = 1 - character->lookstate;
                character->lastlookstatechg = getMsTimer() + 300;
            }
            break;
        case LOOK_ENERGYTANK:
            sprite = energytank;
            break;
        case LOOK_XPARASITE1:
            sprite = xparasite1;
            break;
        case LOOK_XPARASITE2:
            sprite = xparasite2;
            break;
        case LOOK_BOSS_SECROB:
             if (character->lookstate)  
            {
                sprite = securityrobup;
            }
            else
            {
                sprite = securityrobdown;
            }
            character->lookstate = 1 - character->lookstate;
            break;
        case LOOK_FIREBALL:
            sprite = fireball2;
            break;
    }
    
    uint16_t i = 0;
    for (uint8_t y = character->y; y < character->y + character->height; y++)
    {
        for (uint8_t x = character->x; x < character->x + character->width; x++)
        {
            page(x, y, pgm_read_byte_near(sprite + i));
            i++;
        }
    }
}

void hide(struct Character* character)
{
    character->movement = HIDDEN;
    for (int16_t x = character->x; x < character->x + character->width; ++x)
    {
        for (int16_t y = character->y; y < character->y + character->height; ++y)
        {
            if (x >= 0 && y >= 0 && x < DISPLAY_WIDTH)
                page(x, y, 0x00);
        }
    }
}

uint8_t moveleft(struct Character* character)
{
    if (character->x == 0)
        return 0;

    // don't move if there is an obstacle to the left
    for (uint8_t y = character->y; y < character->y + character->height; ++y)
    {
        if (obstacle(character->x - 1, y))
        {
            if (character->y + character->height == 25 && obstacle_hill(character->x - 1))
            {
                moveup(character);
            }
            else
            {
                draw(character);
                return 0;
            }
        }
    }

    for (uint8_t y = character->y; y < character->y + character->height; ++y)
        page(character->x + character->width - 1, y, 0x00);
    character->x--;
    draw(character);
    checkfalling(character);
    character->direction = DIRECTION_LEFT;

    return 1;
}

uint8_t moveright(struct Character* character)
{
    if (character->x + character->width == DISPLAY_WIDTH)
        return 0;

    // don't move if there is an obstacle to the right
    for (uint8_t y = character->y; y < character->y + character->height; ++y)
    {
        if (obstacle(character->x + character->width, y))
        {
            if (character->y + character->height == 25 && obstacle_hill(character->x + character->width))
            {
                moveup(character);
            }
            else
            {
                draw(character);
                return 0;
            }
        }
    }

    for (uint8_t y = character->y; y < character->y + character->height; ++y)
        page(character->x, y, 0x00);
    character->x++;
    draw(character);
    checkfalling(character);
    character->direction = DIRECTION_RIGHT;
    
    return 1;
}

uint8_t moveup(struct Character* character)
{
    // don't move if there is an obstacle above
    for (uint8_t x = character->x; x < character->x + character->width; ++x)
    {
        if (obstacle(x, character->y - 1))
            return 0;
    }

    for (uint8_t x = character->x; x < character->x + character->width; ++x)
        page(x, character->y + character->height - 1, 0x00);
    --character->y;
    draw(character);
    character->verticaldirection = DIRECTION_UP;

    return 1;
}

uint8_t movedown(struct Character* character)
{
    // don't move if there is an obstacle below
    for (uint8_t x = character->x; x < character->x + character->width; ++x)
    {
        if (obstacle(x, character->y + character->height))
            return 0;
    }

    for (uint8_t x = character->x; x < character->x + character->width; ++x)
        page(x, character->y, 0x00);
    ++character->y;
    draw(character);
    character->verticaldirection = DIRECTION_DOWN;

    return 1;
}

void checkfalling(struct Character* character)
{
    if (character->jumpstate == ON_THE_GROUND)
    {
        long feet_on_ground = 0l;
        for (uint8_t x = character->x; x < character->x + character->width; ++x)
            feet_on_ground |= obstacle(x, character->y + character->height);
        if (!feet_on_ground)
            character->jumpstate = character->jumpheight;
    }
}

void jump(struct Character* character)
{
    if (character->movement == FLYING_AROUND)
    {
        if (character->verticaldirection == DIRECTION_UP)
        {
            if (character->y <= 7 || !moveup(character) || !really_random_below(20))
                character->verticaldirection = really_random_below(2);
        }
        else
        {
            if (character->y >= 25 - character->height - 1 || !movedown(character) || !really_random_below(20))
                character->verticaldirection = really_random_below(2);
        }
    }
    else if (character->jumpstate == character->jumpheight)
    {
        if (!movedown(character))
        {
            character->jumpstate = ON_THE_GROUND;
            if (character->movement == FIREBALL)
                hide(character);
        }
    }
    else if (character->jumpstate != ON_THE_GROUND)
    {
        if (!moveup(character))
            character->jumpstate = character->jumpheight;
        else
            character->jumpstate++;
    }
}

// determine the direction in which a monster or projectile should move
void move(struct Character* character)
{
    switch (character->movement)
    {
        case FOLLOW_PROTAGONIST:
            if (protagonist->x < character->x && obstacle(character->x - 1, 25))
                moveleft(character);
            else if (protagonist->x > character->x && obstacle(character->x + character->width, 25))
                moveright(character);
            else
                draw(character);
            break;
        case BACK_AND_FORTH:
            if (character->x <= 0)
                character->direction = DIRECTION_RIGHT;
            else if (character->x + character->width >= DISPLAY_WIDTH)
                character->direction = DIRECTION_LEFT;
            if (character->direction == DIRECTION_LEFT)
                moveleft(character);
            else
                moveright(character);
            break;
        case PROJECTILE:
        case FIREBALL:
            if (character->direction == DIRECTION_LEFT)
            {
                if (!moveleft(character))
                    hide(character);
            }
            else
            {
                if (!moveright(character))
                    hide(character);
            }
            break;
        case HIDDEN:
            break;
        case JUMP:
            if(character->jumpstate == ON_THE_GROUND)
            {    
                character->jumpstate = 1;
            }
            break;
       case JUMPMOVE:
            if(character->jumpstate == ON_THE_GROUND)
            {    
                character->jumpstate = 1;
            }
            if (protagonist->x < character->x)
                    moveleft(character);
                else if (protagonist->x > character->x)
                    moveright(character);
                else
                    draw(character);
            break;
        case BOMB:
            break;
        case FLYING_AROUND:
            if (character->direction == DIRECTION_LEFT)
            {
                if (!moveleft(character) || !really_random_below(20))
                    character->direction = really_random_below(2);
            }
            else
            {
                if (!moveright(character) || !really_random_below(20))
                    character->direction = really_random_below(2);
            }
            break;
        case ENERGYTANK:
            break;
        case XPARASITE:
            break;
        
    }
}
