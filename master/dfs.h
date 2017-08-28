#include <inttypes.h>

#define GRAPH_WIDTH 16
#define GRAPH_HEIGHT 3

uint8_t platform_above(uint8_t x, uint8_t y);
uint8_t platform_below(uint8_t x, uint8_t y);

uint8_t is_door_reachable();
