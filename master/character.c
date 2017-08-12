#include <stdlib.h>
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
            character->health = 99;
            character->damage = 15; // folgende damage und health Zahlen sind Platzhalter Zahlen
            break;
        case LOOK_MONSTER_LITTLE: 
            character->width = 8;
            character->height = 2;
            character->health = 99;
            character->damage = 15;
            break;
        case LOOK_EYEMONSTER:
            character->width = 12;
            character->height = 4;
            character->health = 99;
            character->damage = 15;
            break;
        case LOOK_MONSTER_ZOOMER:
            character->width = 15;
            character->height = 3;
            character->health = 99;
            character->damage = 15;
            break;
        case LOOK_ROCKET:
            character->width = 13;
            character->height = 2;
            character->health = 99;
            character->damage = 15;
            break;
        case LOOK_MONSTER_ZAZABI:
            character->width = 17;
            character->height = 8;
            character->health = 99;
            character->damage = 15;
            break;
        case LOOK_MONSTER_METROID:
            character->width = 14;
            character->height = 3;
            character->health = 99;
            character->damage = 15;
            break;
        case LOOK_MONSTER_HORNOAD:
            character->width = 14;
            character->height = 3;
            character->health = 99;
            character->damage = 15;
            break;
        case LOOK_MONSTER_SIDEHOPPER:
            character->width = 16;
            character->height = 4;
            character->health = 99;
            character->damage = 15;
            break;
        case LOOK_MONSTER_MEMU:
            character->width = 15;
            character->height = 2;
            character->health = 99;
            character->damage = 15;
            break;
        case LOOK_BOSS_DRAGON:
            character->width = 30;
            character->height = 9;
            character->health = 99;
            character->damage = 15;
            break;
    }
    character->lookstate = 0;
    character->jumpstate = ON_THE_GROUND;
}

void draw(struct Character* character)
{
    const uint8_t* sprite = NULL;
    switch (character->look)
    {
        case LOOK_PROTAGONIST:
            if (character->direction == DIRECTION_LEFT)
            {
                sprite = protagonistleft;
            }
            else
            {
                sprite = protagonistright;
            }
            break;

        case LOOK_MONSTER_LITTLE:
            sprite = little;
            break;
            
        case LOOK_EYEMONSTER:
            sprite = eyemonster;
            break;
            
        case LOOK_MONSTER_ZOOMER:
            sprite = zoomer;
            break;
            
        case LOOK_ROCKET:
            sprite = rocket;
            break;
            
        case LOOK_MONSTER_ZAZABI:
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
                sprite = hornoadleft;
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
            sprite = dragon;
            break;
    }
    
    uint8_t i = 0;
    for (uint8_t y = character->y; y < character->y + character->height; y++)
    {
        for (uint8_t x = character->x; x < character->x + character->width; x++)
        {
            page(x, y, sprite[i]);
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
            return 0;
    }

    for (uint8_t y = character->y; y < character->y + character->height; ++y)
        page(character->x + character->width - 1, y, 0x00);
    character->x--;
    draw(character);

    if (character->jumpstate == ON_THE_GROUND)
    {
        long feet_on_ground = 0l;
        for (uint8_t x = character->x; x < character->x + character->width; ++x)
            feet_on_ground |= obstacle(x, character->y + character->height);
        if (!feet_on_ground)
            character->jumpstate = FALLING_DOWN;
    }
    character->direction = DIRECTION_LEFT;

    return 1;
}

uint8_t moveright(struct Character* character)
{
    // don't move if there is an obstacle to the right
    for (uint8_t y = character->y; y < character->y + character->height; ++y)
    {
        if (obstacle(character->x + character->width, y))
            return 0;
    }

    for (uint8_t y = character->y; y < character->y + character->height; ++y)
        page(character->x, y, 0x00);
    character->x++;
    draw(character);

    if (character->jumpstate == ON_THE_GROUND)
    {
        long feet_on_ground = 0l;
        for (uint8_t x = character->x; x < character->x + character->width; ++x)
            feet_on_ground |= obstacle(x, character->y + character->height);
        if (!feet_on_ground)
            character->jumpstate = FALLING_DOWN;
    }
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
