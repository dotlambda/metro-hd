#include "globals.h"

long obstacle(uint8_t x, uint8_t y)
{
    if (y == 25)
        return 1l;
    else if (y == 20)
        return !(platforms_20 & (3l << (x / PLATFORM_WIDTH * 2)));
    else if (y == 15)
        return !(platforms_15 & (3l << (x / PLATFORM_WIDTH * 2)));
//     else if (y == 10)
//         return !(platforms_10 & (3l << (x / PLATFORM_WIDTH * 2)));
    else
        return 0l;
}
