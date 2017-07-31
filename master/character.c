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
        case LOOK_MONSTER_2: 
            character->width = 8;
            character->height = 2;
            break;
        case LOOK_EYEMONSTER:
            character->width = 12;
            character->height = 4; 
            break;
        case LOOK_ROCKET:
            character->width = 13;
            character->height = 2;
    }
    character->jumpstate = ON_THE_GROUND;
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
        case LOOK_MONSTER_2:
            page(character->x,     character->y,     0b00000100);
            page(character->x + 1, character->y,     0b00010100);
            page(character->x + 2, character->y,     0b10010000);
            page(character->x + 3, character->y,     0b10000000);
            page(character->x + 4, character->y,     0b10000000);
            page(character->x + 5, character->y,     0b10010000);
            page(character->x + 6, character->y,     0b00010100);
            page(character->x + 7, character->y,     0b00000100);
            page(character->x,     character->y + 1, 0b00000000);
            page(character->x + 1, character->y + 1, 0b11001000);
            page(character->x + 2,  character->y + 1, 0b11111010);
            page(character->x + 3, character->y + 1, 0b11111001);
            page(character->x + 4, character->y + 1, 0b11111001);
            page(character->x + 5, character->y + 1, 0b11111010);
            page(character->x + 6, character->y + 1, 0b11001000);
            page(character->x + 7, character->y + 1, 0b00000000);
            break;
        case LOOK_EYEMONSTER:
            page(character->x,      character->y, 0b00000000);
            page(character->x + 1,  character->y, 0b00000000);
            page(character->x + 2,  character->y, 0b01000000);
            page(character->x + 3,  character->y, 0b11000000);
            page(character->x + 4,  character->y, 0b10110100);
            page(character->x + 5,  character->y, 0b10110101);
            page(character->x + 6,  character->y, 0b10110100);
            page(character->x + 7,  character->y, 0b11000000);
            page(character->x + 8,  character->y, 0b01000000);
            page(character->x + 9,  character->y, 0b01000000);
            page(character->x + 10, character->y, 0b00000000);
            page(character->x + 11, character->y, 0b00000000);
            
            page(character->x,      character->y + 1, 0b00010000);
            page(character->x + 1,  character->y + 1, 0b11111100);
            page(character->x + 2,  character->y + 1, 0b01011111);
            page(character->x + 3,  character->y + 1, 0b01011110);
            page(character->x + 4,  character->y + 1, 0b11111100);
            page(character->x + 5,  character->y + 1, 0b00000000);
            page(character->x + 6,  character->y + 1, 0b01010000);
            page(character->x + 7,  character->y + 1, 0b01000010);
            page(character->x + 8,  character->y + 1, 0b10101011);
            page(character->x + 9,  character->y + 1, 0b11111101);
            page(character->x + 10, character->y + 1, 0b01010000);
            page(character->x + 11, character->y + 1, 0b01000000);
            
            page(character->x,      character->y + 2, 0b00000000);
            page(character->x + 1,  character->y + 2, 0b00000011);
            page(character->x + 2,  character->y + 2, 0b00011111);
            page(character->x + 3,  character->y + 2, 0b00111011);
            page(character->x + 4,  character->y + 2, 0b11100011);
            page(character->x + 5,  character->y + 2, 0b11100000);
            page(character->x + 6,  character->y + 2, 0b11100001);
            page(character->x + 7,  character->y + 2, 0b01111100);
            page(character->x + 8,  character->y + 2, 0b01011110);
            page(character->x + 9,  character->y + 2, 0b00000011);
            page(character->x + 10, character->y + 2, 0b00000001);
            page(character->x + 11, character->y + 2, 0b00000000);
            
            page(character->x,      character->y + 3, 0b00000000);
            page(character->x + 1,  character->y + 3, 0b00000000);
            page(character->x + 2,  character->y + 3, 0b00000000);
            page(character->x + 3,  character->y + 3, 0b00000000);
            page(character->x + 4,  character->y + 3, 0b00000001);
            page(character->x + 5,  character->y + 3, 0b00000101);
            page(character->x + 6,  character->y + 3, 0b00000001);
            page(character->x + 7,  character->y + 3, 0b00000000);
            page(character->x + 8,  character->y + 3, 0b00000000);
            page(character->x + 9,  character->y + 3, 0b00000000);
            page(character->x + 10, character->y + 3, 0b00000000);
            page(character->x + 11, character->y + 3, 0b00000000);
            break;
        case LOOK_ROCKET:
            page(character->x,      character->y, 0b11000000);
            page(character->x + 1,  character->y, 0b10110000);
            page(character->x + 2,  character->y, 0b00110000);
            page(character->x + 3,  character->y, 0b00110000);
            page(character->x + 4,  character->y, 0b00110000);
            page(character->x + 5,  character->y, 0b10110000);
            page(character->x + 6,  character->y, 0b00101100);
            page(character->x + 7,  character->y, 0b00100100);
            page(character->x + 8,  character->y, 0b01111100);
            page(character->x + 9,  character->y, 0b00000100);
            page(character->x + 10, character->y, 0b00010100);
            page(character->x + 11, character->y, 0b01010000);
            page(character->x + 12, character->y, 0b01000000);
            
            page(character->x,      character->y + 1, 0b00000011);
            page(character->x + 1,  character->y + 1, 0b00001110);
            page(character->x + 2,  character->y + 1, 0b00001110);
            page(character->x + 3,  character->y + 1, 0b00001110);
            page(character->x + 4,  character->y + 1, 0b00001110);
            page(character->x + 5,  character->y + 1, 0b00001110);
            page(character->x + 6,  character->y + 1, 0b00111010);
            page(character->x + 7,  character->y + 1, 0b00011010);
            page(character->x + 8,  character->y + 1, 0b00111101);
            page(character->x + 9,  character->y + 1, 0b00010000);
            page(character->x + 10, character->y + 1, 0b00010100);
            page(character->x + 11, character->y + 1, 0b00000101);
            page(character->x + 12, character->y + 1, 0b00000001);
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

// determine the direction in which a monster should move
void move(struct Character* monster)
{
    if (monster->followsprotagonist) // follow protagonist
    {
        if (protagonist->x < monster->x)
        {
            moveleft(monster);
        }
        else if (protagonist->x > monster->x)
        {
            moveright(monster);
        }
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
