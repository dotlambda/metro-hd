#ifndef CHARACTER_H
#define CHARACTER_H

#define DIRECTION_LEFT 0
#define DIRECTION_RIGHT 1
#define ON_THE_GROUND 0

#define NUM_MONSTER_LOOKS 8

#include <inttypes.h>

struct Character
{
    uint8_t x;
    uint8_t y;
    enum {LOOK_MONSTER_GEEGA, LOOK_MONSTER_MEMU, LOOK_MONSTER_METROID, LOOK_MONSTER_LITTLE, LOOK_EYEMONSTER, LOOK_MONSTER_ZOOMER, LOOK_MONSTER_HORNOAD, LOOK_MONSTER_SIDEHOPPER, LOOK_BOSS_ZAZABI, LOOK_BOSS_DRAGON, LOOK_PROTAGONIST, LOOK_ROCKET, LOOK_BOMB} look;
    uint8_t lookstate; // to e.g. store whether the wings are turned upwards or downwards
    uint32_t lastlookstatechg;
    uint8_t width;  // in pixels
    uint8_t height; // in pages
    uint8_t direction;
    uint8_t jumpstate;
    int8_t health;
    uint8_t damage;
    uint8_t jumpheight;
    enum {FOLLOW_PROTAGONIST, BACK_AND_FORTH, PROJECTILE, HIDDEN, JUMP, JUMPMOVE, BOMB} movement;
};

void initcharacter(struct Character* character);

void draw(struct Character* character);
void hide(struct Character* character);

uint8_t moveleft(struct Character* character);
uint8_t moveright(struct Character* character);
uint8_t moveup(struct Character* character);
uint8_t movedown(struct Character* character);

void checkfalling(struct Character* character);
void jump(struct Character* character);
void move(struct Character* monster);

#endif
