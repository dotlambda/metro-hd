#include "globals.h"
#include "display.h"

long obstacle(uint8_t x, uint8_t y)
{
    if (y == 5) // ceiling
        return 1l;
    if (doors & 0b00000010 && (x < 4 || (y >= 20 && x < 6)))
        return 1l;
    else if (doors & 0b00000001 && (x >= DISPLAY_WIDTH - 4 || (y >= 20 && x >= DISPLAY_WIDTH - 6)))
        return 1l;
    else if (y == 25)
        return nofloor & (7l << x / 16 * 3);
    else if (y == 19)
        return !(platforms_19 & (3l << (x / PLATFORM_WIDTH * 2)));
    else if (y == 13)
        return !(platforms_13 & (3l << (x / PLATFORM_WIDTH * 2)));
    else if (y == 24)
         return !(platforms_24 & (3l << (x / 16 * 2)));
    else
        return 0l;
}
