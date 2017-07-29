#ifndef CHARACTER_H
#define CHARACTER_H

#define DIRECTION_LEFT 0
#define DIRECTION_RIGHT 1
#define ON_THE_GROUND 0
#define FALLING_DOWN 7

#include <inttypes.h>

struct Character
{
    uint8_t x;
    uint8_t y;
    enum {LOOK_PROTAGONIST, LOOK_MONSTER_1} look;
    uint8_t width;  // in pixels
    uint8_t height; // in pages
    uint8_t direction;
    uint8_t jumpstate;
    uint8_t followsprotagonist;
};

void initcharacter(struct Character* character);

void draw(struct Character* character);

uint8_t moveleft(struct Character* character);
uint8_t moveright(struct Character* character);
uint8_t moveup(struct Character* character);
uint8_t movedown(struct Character* character);

void jump(struct Character* character);
void move(struct Character* monster);

#endif
