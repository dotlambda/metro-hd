#include "character.h"
#include "globals.h"
#include "display.h"

void initcharacter(struct Character* character)
{
    switch (character->look)
    {
        case LOOK_PROTAGONIST:
        case LOOK_MONSTER_1:
            character->width = 7;
            character->height = 3;
            break;
    }
}

void draw(struct Character* character)
{
    switch (character->look)
    {
        case LOOK_PROTAGONIST:
            page(character->x,     character->y,     0b11010000);
            page(character->x + 1, character->y,     0b11110000);
            page(character->x + 2, character->y,     0b11111100);
            page(character->x + 3, character->y,     0b11110111);
            page(character->x + 4, character->y,     0b11110111);
            page(character->x + 5, character->y,     0b00110111);
            page(character->x + 6, character->y,     0b00000011);
            page(character->x,     character->y + 1, 0b00000111);
            page(character->x + 1, character->y + 1, 0xFF);
            page(character->x + 2, character->y + 1, 0xFF);
            page(character->x + 3, character->y + 1, 0xFF);
            page(character->x + 4, character->y + 1, 0xFF);
            page(character->x + 5, character->y + 1, 0b00001111);
            page(character->x + 6, character->y + 1, 0b00000101);
            page(character->x,     character->y + 2, 0b11010000);
            page(character->x + 1, character->y + 2, 0b11111101);
            page(character->x + 2, character->y + 2, 0b11001111);
            page(character->x + 3, character->y + 2, 0b00000011);
            page(character->x + 4, character->y + 2, 0b11010011);
            page(character->x + 5, character->y + 2, 0xFF);
            page(character->x + 6, character->y + 2, 0b11001100);
            break;
        case LOOK_MONSTER_1:
            page(character->x,     character->y,     0xFF);
            page(character->x + 1, character->y,     0b11010011);
            page(character->x + 2, character->y,     0b11000011);
            page(character->x + 3, character->y,     0xFF);
            page(character->x + 4, character->y,     0b11010011);
            page(character->x + 5, character->y,     0b11000011);
            page(character->x + 6, character->y,     0xFF);
            page(character->x,     character->y + 1, 0xFF);
            page(character->x + 1, character->y + 1, 0b11001111);
            page(character->x + 2, character->y + 1, 0b00001111);
            page(character->x + 3, character->y + 1, 0b00001111);
            page(character->x + 4, character->y + 1, 0b00001111);
            page(character->x + 5, character->y + 1, 0b11001111);
            page(character->x + 6, character->y + 1, 0xFF);
            page(character->x,     character->y + 2, 0b11000000);
            page(character->x + 1, character->y + 2, 0b11000011);
            page(character->x + 2, character->y + 2, 0xFF);
            page(character->x + 3, character->y + 2, 0b00000011);
            page(character->x + 4, character->y + 2, 0xFF);
            page(character->x + 5, character->y + 2, 0b11000011);
            page(character->x + 6, character->y + 2, 0b11000000);
            break;
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

    for (uint8_t y = 0; y < character->height; ++y)
        page(character->x + character->width - 1, y, 0x00);
    --character->x;
    draw(character);

    long feet_on_ground = 0l;
    for (uint8_t x = character->x; x < character->x + character->width; ++x)
        feet_on_ground |= obstacle(x, character->y + character->height);
    if (!feet_on_ground)
        character->jumpstate = FALLING_DOWN;

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
    ++character->x;
    draw(character);

    long feet_on_ground = 0l;
    for (uint8_t x = character->x; x < character->x + character->width; ++x)
        feet_on_ground |= obstacle(x, character->y + character->height);
    if (!feet_on_ground)
        character->jumpstate = FALLING_DOWN;

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
            ++character->jumpstate;
    }
}

// determine the direction in which a monster should move
void move(struct Character* monster)
{
    if (monster->followsprotagonist) // follow protagonist
    {
        if (protagonist->x < monster->x)
            moveleft(monster);
        else if (protagonist->x > monster->x)
            moveright(monster);
    }
    else // move from left to right
    {
        if (monster->x == 0)
            monster->direction = DIRECTION_RIGHT;
        else if (monster->x + monster->width == DISPLAY_WIDTH)
            monster->direction = DIRECTION_LEFT;
        if (monster->direction == DIRECTION_LEFT)
            moveleft(monster);
        else
            moveright(monster);
    }
}
