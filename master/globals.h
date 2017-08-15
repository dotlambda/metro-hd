#ifndef GLOBALS_H
#define GLOBALS_H

#include <inttypes.h>

#include "character.h"

#define PLATFORM_WIDTH 10

struct Character* protagonist;

long platforms_19;
long platforms_13;
long platforms_24;
long nofloor;
uint8_t doors; // 0b000000ij where i==1 if there is a door on the left and j==1 if there is one on the right
long obstacle(uint8_t x, uint8_t y);

#endif
