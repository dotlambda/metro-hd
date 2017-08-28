#include "dfs.h"
#include "level.h"

//TODO there is no space for the protagonist to go down/up if there is a door

uint8_t platform_above(uint8_t x, uint8_t y)
{
    if (y == 0)
        return !(platforms_19 & (3l << (x * 2)));
    else if (y == 1)
        return !(platforms_13 & (3l << (x * 2)));
    return 0;
}

// whether there iss a platform/foor below
uint8_t platform_below(uint8_t x, uint8_t y)
{
    if (y == 0)
        // return 1 if there is a floor on the left AND on the right
        return (nofloor & (3l << PLATFORM_WIDTH * x / 16 * 2)) && (nofloor & (3l << (PLATFORM_WIDTH * (x + 1) - 1) / 16 * 2));
    else if (y == 1)
        return !(platforms_19 & (3l << (x * 2)));
    else if (y == 2)
        return !(platforms_13 & (3l << (x * 2)));
    return 0;
}

uint8_t is_door_reachable()
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
 
        // check vertex above to the left
        if (x > 0 && y < GRAPH_HEIGHT - 1 && !(visited[y+1] & (1 << (x - 1)))
            && !platform_above(x, y) && platform_below(x-1, y+1))
        {
            // push
            stack[++top] = (x - 1) | ((y + 1) << 4);
            visited[y+1] |= 1 << (x - 1);
        }
        
        // check vertex above two to the left
        if (x > 1 && y < GRAPH_HEIGHT - 2 && !(visited[y+1] & (1 << (x - 1)))
            && !platform_above(x, y) && !platform_above(x-1, y) && platform_below(x-2, y+1))
        {
            //push
            stack[++top] = (x - 2) | ((y + 1) << 4);
            visited[y+1] |= 1 << (x - 2);
        }

        // check vertex above on the right
        if (x < GRAPH_WIDTH - 1 && y < GRAPH_HEIGHT - 1 && !(visited[y+1] & (1 << (x + 1)))
            && !platform_above(x, y) && platform_below(x+1, y+1))
        {
            //push
            stack[++top] = (x + 1) | ((y + 1) << 4);
            visited[y+1] |= 1 << (x + 1);
        }

        // check vertex above two to the right
        if (x < GRAPH_WIDTH - 2 && y < GRAPH_HEIGHT - 2 && !(visited[y+1] & (1 << (x + 1)))
            && !platform_above(x, y) && !platform_above(x+1, y) && platform_below(x+2, y+1))
        {
            //push
            stack[++top] = (x + 2) | ((y + 1) << 4);
            visited[y+1] |= 1 << (x + 2);
        }

        // check vertex below on the left
        if (y > 0 && !(visited[y-1] & (1 << (x - 1)))
            && !platform_below(x-1, y) && platform_below(x-1, y-1))
        {
            // push
            stack[++top] = (x - 1) | ((y - 1) << 4);
            visited[y-1] |= 1 << (x - 1);
        }
        
        // check vertex below two to the left
        if (y > 0 && !(visited[y-1] & (1 << (x - 2)))
            && !platform_below(x-1, y) && !platform_below(x-2, y) && platform_below(x-2, y-1))
        {
            // push
            stack[++top] = (x - 2) | ((y - 1) << 4);
            visited[y-1] |= 1 << (x - 2);
        }

        // check vertex below on the right
        if (y > 0 && !(visited[y-1] & (1 << (x + 1)))
            && !platform_below(x+1, y) && platform_below(x-1, y-1))
        {
            // push
            stack[++top] = (x + 1) | ((y - 1) << 4);
            visited[y-1] |= 1 << (x + 1);
        }
        
        // check vertex below two to the right
        if (y > 0 && !(visited[y-1] & (1 << (x + 2)))
            && !platform_below(x+1, y) && !platform_below(x+2, y) && platform_below(x+2, y-1))
        {
            // push
            stack[++top] = (x + 2) | ((y - 1) << 4);
            visited[y-1] |= 1 << (x + 2);
        }

        // check vertex on the left
        if (x > 0 && !(visited[y] & (1 << (x - 1)))
            && platform_below(x-1, y))
        {
            // push
            stack[++top] = (x - 1) | (y << 4);
            visited[y] |= 1 << (x - 1);
        }

        // check vertex on the right
        if (x < GRAPH_WIDTH - 1 && !(visited[y] & (1 << (x + 1)))
            && platform_below(x+1, y))
        {
            // push
            stack[++top] = (x + 1) | (y << 4);
            visited[y] |= 1 << (x + 1);
        }
    }

    return 0; // the door can't be reached
}
