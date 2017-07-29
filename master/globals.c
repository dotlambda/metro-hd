#include "globals.h"

long obstacle(uint8_t x, uint8_t y)
{
    if (y == 25)
        return 1l;
    if (y == 20)
        return platforms & (1l << (x / 8));
    else
        return 0l;
}
