#include <inttypes.h>
#include <stdbool.h>

#define GRAPH_WIDTH 16
#define GRAPH_HEIGHT 3

#define JUMP_DISTANCE 3
#define UP_DISTANCE 3
#define DOWN_DISTANCE 3

bool platform_above(uint8_t x, uint8_t y);
bool platform_below(uint8_t x, uint8_t y);

bool is_door_reachable();
