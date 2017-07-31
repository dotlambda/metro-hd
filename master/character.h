#ifndef CHARACTER_H
#define CHARACTER_H

#define DIRECTION_LEFT 0
#define DIRECTION_RIGHT 1
#define ON_THE_GROUND 0
#define FALLING_DOWN 7

#include <inttypes.h>

struct Character
{
    int16_t x;
    int16_t y;
    enum {LOOK_PROTAGONIST, LOOK_MONSTER_1, LOOK_MONSTER_2, LOOK_EYEMONSTER, LOOK_ROCKET} look;
    uint8_t width;  // in pixels
    uint8_t height; // in pages
    uint8_t direction;
    uint8_t jumpstate;
    enum {FOLLOW_PROTAGONIST, BACK_AND_FORTH, PROJECTILE, HIDDEN} movement;
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