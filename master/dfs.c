#include "dfs.h"
#include "level.h"

// whether there is a platform/foor below
bool stand_on(uint8_t x, uint8_t y)
{
    // cannot stand on door, therefore this is different from below
    if (y == 0)
        // return 1 if there is a floor on the left AND on the right
        return (nofloor & (3l << PLATFORM_WIDTH * x / 16 * 2)) && (nofloor & (3l << (PLATFORM_WIDTH * (x + 1) - 1) / 16 * 2));
    else if (y == 1)
        return !(platforms_19 & (3l << (x * 2)));
    else if (y == 2)
        return !(platforms_13 & (3l << (x * 2)));
    return 0;
}

// whether there is a platform/foor below
bool blocked(uint8_t x, uint8_t y)
{
    if (doors & 0b00000010 && x == 0 && y == 1) // blocked by door
        return 1;
    else if (doors & 0b00000001 && x == GRAPH_WIDTH - 1 && y == 1)
        return 1;
    return stand_on(x, y);
}

bool is_door_reachable()
{
    // there are 16 different x positions for platforms
    // and 3 different y positions (floor, first and second platform level)
    // the x coordinate is stack[i] & 0b00001111
    // the y coordinate is stack[i] >> 4
    uint8_t stack[GRAPH_WIDTH * GRAPH_HEIGHT];
    int8_t top = -1;
    uint8_t goal;

    // whether (x,y) was alredy visited is stored in visited[y] & (1 << x)
    uint16_t visited[GRAPH_HEIGHT];
    for  (uint8_t y = 0; y < GRAPH_HEIGHT; ++y)
    {
        visited[y] = 0;
    }
    
    if (level >= 0)
    {
        stack[++top] = 0; // start on the left which is (0,0)
        goal = GRAPH_WIDTH - 1;
    }
    else
    {
        stack[++top] = GRAPH_WIDTH - 1; // start on the right which is (12,0)
        goal = 0;
    }

    // while the stack is not empty
    while (top != -1)
    {
        // pop
        uint8_t xy = stack[top];
        --top;

        if (xy == goal)
            return 1; // the door can be reached

        uint8_t x = xy & 0b00001111;
        uint8_t y = xy >> 4;
 
        // jump up
        for (int8_t i = -UP_DISTANCE; i <= UP_DISTANCE; ++i)
        {
            if (i == 0)
                continue;
            if (y + 1 == GRAPH_HEIGHT || x + i < 0 || x + i >= GRAPH_WIDTH)
                continue;
            if (visited[y + 1] & (1 << (x + i)))
                continue;
            if (!stand_on(x + i, y + 1)) // can't jump onto nothing
                continue;
            // check if there is no platform in the way
            uint8_t platform = 0;
            for (int8_t j = 0; j != i; j += (i < 0 ? -1 : 1))
            {
                if (blocked(x + j, y + 1))
                {
                    platform = 1;
                    break;
                }
            }
            if (platform)
                continue;
            // push onto stack
            stack[++top] = (x + i) | ((y + 1) << 4);
            visited[y + 1] |= 1 << (x + i);
        }

        // jump down
        for (int8_t i = -DOWN_DISTANCE; i <= DOWN_DISTANCE; ++i)
        {
            if (i == 0)
                continue;
            if (y == 0 || x + i < 0 || x + i >= GRAPH_WIDTH)
                continue;
            if (visited[y - 1] & (1 << (x + i)))
                continue;
            if (!stand_on(x + i, y - 1)) // can't jump onto nothing
                continue;
            // check if there is no platform in the way
            uint8_t platform = 0;
            for (int8_t j = i; j != 0; j += (i < 0 ? 1 : -1))
            {
                if (blocked(x + j, y))
                {
                    platform = 1;
                    break;
                }
            }
            if (platform)
                continue;
            // push onto stack
            stack[++top] = (x + i) | ((y - 1) << 4);
            visited[y - 1] |= 1 << (x + i);
        }

        // jump or walk
        for (int8_t i = -JUMP_DISTANCE; i <= JUMP_DISTANCE; ++i)
        {
            if (i == 0)
                continue;
            if (x + i < 0 || x + i >= GRAPH_WIDTH)
                continue;
            if (visited[y] & (1 << (x + i)))
                continue;
            if (!stand_on(x + i, y)) // can't jump into water/spikes
                continue;
            // check if there is no platform in the way
            // a platform above the current or the target position is okay
            uint8_t platform = 0;
            for (int8_t j = (i < 0 ? i + 1 : i - 1); j != 0; j += (i < 0 ? 1 : -1))
            {
                if (blocked(x + j, y + 1))
                {
                    platform = 1;
                    break;
                }
            }
            if (platform) // can't jump if there is a platform above
                continue;
            // push onto stack
            stack[++top] = (x + i) | (y << 4);
            visited[y] |= 1 << (x + i);
        }
    }
        
    return 0; // the door can't be reached
}
