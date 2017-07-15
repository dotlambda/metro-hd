#include <stdbool.h>

#define DIRECTION_LEFT -1
#define DIRECTION_RIGHT 1

struct monster
{
    uint8_t x;
    uint8_t y;
    bool follows_character;
    uint8_t width;
    uint8_t height;
    uint8_t direction;
}

void draw(monster& monster);

void moveleft(monster& monster);
void moveright(monster& monster);
void move(monster& monster);
