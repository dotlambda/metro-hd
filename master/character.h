#ifndef CHARACTER_H
#define CHARACTER_H

#define ON_THE_GROUND 0
#define CLIMBING -1

#define NUM_MONSTER_LOOKS 8

#include <inttypes.h>

struct Character
{
    uint8_t x;
    uint8_t y;
    enum {LOOK_MONSTER_GEEGA, LOOK_MONSTER_MEMU, LOOK_MONSTER_METROID, LOOK_MONSTER_LITTLE, LOOK_EYEMONSTER, LOOK_MONSTER_ZOOMER, LOOK_MONSTER_HORNOAD, LOOK_MONSTER_SIDEHOPPER, LOOK_BOSS_SECROB, LOOK_BOSS_ZAZABI, LOOK_BOSS_DRAGON, LOOK_NEO_RIDLEY_DRAGON, LOOK_BIGXPARASITE, LOOK_PROTAGONIST,  LOOK_ROCKET, LOOK_BOMB, LOOK_ENERGYTANK, LOOK_XPARASITE1, LOOK_XPARASITE2, LOOK_FIREBALL, LOOK_ARROW, LOOK_ARROW_UP, LOOK_HIDDEN} look;
    uint8_t lookstate; // to e.g. store whether the wings are turned upwards or downwards
    uint32_t lastlookstatechg;
    uint8_t width;  // in pixels
    uint8_t height; // in pixels
    enum {DIRECTION_LEFT, DIRECTION_RIGHT} direction;
    enum {DIRECTION_UP, DIRECTION_DOWN} verticaldirection;
    int8_t jumpstate;
    uint8_t initial_health;
    int8_t health;
    uint8_t damage;
    uint8_t jumpheight;
    enum {FOLLOW_PROTAGONIST, BACK_AND_FORTH, PROJECTILE, HIDDEN, JUMP, JUMPMOVE, FLYING_AROUND, BOMB, ENERGYTANK, XPARASITE, FIREBALL, SECROB, BOSS_DRAGON_GROUND, ARROW, ARROW_UP, BOSS_DRAGON_ATTACK, BOSS_DRAGON_AIR, ZAZABI} movement;
    uint8_t x_pace;
    uint8_t y_pace;
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
