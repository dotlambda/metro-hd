#ifndef LEVEL_H
#define LEVEL_H

#include <inttypes.h>
#include <stdbool.h>
#include "character.h"

#define PLATFORM_WIDTH 10

#define MAX_LEVEL_WIDTH 5 // max 5 displays for one level
#define MIN_LEVEL_WIDTH 1

struct Character* protagonist;
uint8_t num_rockets;
uint8_t num_bombs;

struct Character* monster;
struct Character* projectile;
struct Character* bombstruct;

const uint8_t* floorsprite;
const uint8_t* rotatedfloorsprite;
const uint8_t* nofloorsprite;

long level_seed;
long level_pos;
uint8_t max_level_pos;

long platforms_19;
long platforms_13;
long platforms_24;
long nofloor;
uint8_t doors; // 0b000000ij where i==1 if there is a door on the left and j==1 if there is one on the right
uint8_t energytank_x;
uint8_t energytank_y;
bool energytankexists;

long obstacle(uint8_t x, uint8_t y);

// whether there is a platform at y=24
long obstacle_hill(uint8_t x);

long obstacle_levelpos(uint8_t x, uint8_t y, long level_pos);

void redraw();
void selectfloor();
void newlevelpos();
void newlevel();
void newgame();

#endif
