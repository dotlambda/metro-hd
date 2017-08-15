#include <stdlib.h>
#include <avr/pgmspace.h>
#include "timer.h"
#include "character.h"
#include "globals.h"
#include "display.h"
#include "sprites.h"

void initcharacter(struct Character* character)
{
    switch (character->look)
    {
        case LOOK_PROTAGONIST:
            character->width = 8;
            character->height = 4;
            break;
        case LOOK_MONSTER_LITTLE: 
            character->width = 8;
            character->height = 2;
            break;
        case LOOK_EYEMONSTER:
            character->width = 12;
            character->height = 4; 
            break;
        case LOOK_MONSTER_ZOOMER:
            character->width = 15;
            character->height = 3;
            break;
        case LOOK_ROCKET:
            character->width = 13;
            character->height = 2;
            break;
        case LOOK_BOSS_ZAZABI:
            character->width = 17;
            character->height = 8;
            break;
        case LOOK_MONSTER_METROID:
            character->width = 14;
            character->height = 3;
            break;
        case LOOK_MONSTER_HORNOAD:
            character->width = 14;
            character->height = 3;
            break;
        case LOOK_MONSTER_SIDEHOPPER:
            character->width = 16;
            character->height = 3;
            break;
        case LOOK_MONSTER_MEMU:
            character->width = 15;
            character->height = 2;
            break;
        case LOOK_BOSS_DRAGON:
            character->width = 30;
            character->height = 9;
            break;
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
            sprite = rocket;
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
            character->jumpstate = FALLING_DOWN;
    }
}

void jump(struct Character* character)
{
    if (character->jumpstate == FALLING_DOWN)
    {
        if (!movedown(character))
            character->jumpstate = ON_THE_GROUND;
    }
    else if (character->jumpstate != ON_THE_GROUND)
    {
        if (!moveup(character))
            character->jumpstate = FALLING_DOWN;
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
            if (protagonist->x < character->x)
                moveleft(character);
            else if (protagonist->x > character->x)
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
            if (character->x <= 0 || character->x + character->width == DISPLAY_WIDTH)
                hide(character);
            else if (character->direction == DIRECTION_LEFT)
                moveleft(character);
            else
                moveright(character);
            break;
        case HIDDEN:
            break;
    }
}
