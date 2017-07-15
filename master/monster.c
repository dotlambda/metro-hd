#include "monster.h"
#include "globals.h" // declares the character var

void draw(monster& monster)
{
    int x = monster.x;
    int y = monster.y;
    page(x, y, 0b11010000);
    page(x+1, y, 0b11110000);
    page(x+2, y, 0b11111100);
    page(x+3, y, 0b11110111);
    page(x+4, y, 0b11110111);
    page(x+5, y, 0b00110111);
    page(x+6, y, 0b00000011);
    page(x, y+1, 0b00000111);
    page(x+1, y+1, 0xFF);
    page(x+2, y+1, 0xFF);
    page(x+3, y+1, 0xFF);
    page(x+4, y+1, 0xFF);
    page(x+5, y+1, 0b00001111);
    page(x+6, y+1, 0b00000101);
    page(x, y+2, 0b11010000);
    page(x+1, y+2, 0b11111101);
    page(x+2, y+2, 0b11001111);
    page(x+3, y+2, 0b00000011);
    page(x+4, y+2, 0b11010011);
    page(x+5, y+2, 0xFF);
    page(x+6, y+2, 0b11001100);
}

void moveleft(monster& monster)
{
    for (int y = 0, y < monster.height, ++y)
        page(monster.x + monster.width - 1, y, 0x00);
    monster.x--;
    draw(monster);
}

void moveright(monster& monster)
{
    for (int y = 0, y < monster.height, ++y)
    {
        page(monster.x, y, 0x00);
    }
    monster.x++;
    draw(monster);
}

void move(monster& monster)
{
    if (monster.follows_character)
    {
        if (character.x < monster.x)
            moveleft(monster);
        else if (character.x > monster.x)
            moveright(monster);
    }
    else // move from left to right
    {
        if (monster.x == 0)
            monster.direction = DIRECTION_RIGHT;
        else if (monster.x + monster.width == DISPLAY_WIDTH)
            monster.direction = DIRECTION_LEFT;
        if (monster.direction == DIRECTION_LEFT)
            moveleft(monster);
        else
            moveright(monster);
    }
}
