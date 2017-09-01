#ifndef LEVEL_H
#define LEVEL_H

#include <inttypes.h>
#include <stdbool.h>
#include <avr/eeprom.h>
#include "character.h"

#define PLATFORM_WIDTH 10

#define FLOOR_Y 100
#define DOOR_Y 80
#define HILL_Y 76
#define CEILING_Y 20

#define MAX_STRING_LEN 40

#define MAX_LEVEL_WIDTH 5 // max 5 displays for one level
#define MIN_LEVEL_WIDTH 1

#define BOSS_LEVEL_DISTANCE 5 // every 5th level is a boss level

struct Character* protagonist;
extern EEMEM uint8_t health_stored;
uint8_t num_rockets;
extern EEMEM uint8_t num_rockets_stored;
uint8_t num_bombs;
extern EEMEM uint8_t num_bombs_stored;

#define NUM_MONSTERS 3
#define NUM_FIREBALLS 6
#define NUM_ROCKETS 3
struct Character monsters_[NUM_MONSTERS];
struct Character* monsters[NUM_MONSTERS];
struct Character projectiles_[NUM_ROCKETS];
struct Character* projectiles[NUM_ROCKETS];
struct Character* bombstruct;
struct Character* energytankstruct;
struct Character xparasites_[NUM_MONSTERS];
struct Character* xparasites[NUM_MONSTERS];
struct Character fireballs_[NUM_FIREBALLS];
struct Character* fireballs[NUM_FIREBALLS];

const uint8_t* floorsprite;
const uint8_t* leftrotatedfloorsprite;
const uint8_t* rightrotatedfloorsprite;
const uint8_t* nofloorsprite;
const uint8_t* ceilingsprite;

uint32_t initial_level;
extern EEMEM uint32_t initial_level_stored;
int32_t level;
extern EEMEM int32_t level_stored;
long level_seed;
long level_pos;
uint8_t max_level_pos;

long platforms_19;
long platforms_13;
long platforms_24;
long nofloor;
uint8_t doors; // 0b000000ij where i==1 if there is a door on the left and j==1 if there is one on the right
bool left_door_open;
bool right_door_open;
bool rechargeroom;
bool recharging; 
bool bosslevel;

long obstacle(uint8_t x, uint8_t y);

// whether there is a platform at 96<=y<100
long obstacle_hill(uint8_t x);

long obstacle_levelpos(uint8_t x, uint8_t y, long level_pos);

void redraw();
void selectfloor();
void newlevelpos();
void newlevel();
void newgame();

#endif
