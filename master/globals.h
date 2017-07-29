#ifndef GLOBALS_H
#define GLOBALS_H

#include <inttypes.h>

#include "character.h"

#define PLATFORM_WIDTH 10

struct Character* protagonist;

long platforms_20;
long platforms_15;
//long platforms_10;
long obstacle(uint8_t x, uint8_t y);

#endif
