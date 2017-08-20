#include <stdlib.h>
#include "level.h"
#include "display.h"
#include "character.h"
#include "drawing.h"
#include "sprites.h"
#include "rand.h"

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

long obstacle_hill(uint8_t x)
{
    return !(platforms_24 & (3l << (x / 16 * 2)));
}

long obstacle_levelpos(uint8_t x, uint8_t y, long level_pos)
{
    srandom(level_seed + level_pos);
    long platforms_13 = random();
    long platforms_19 = random();
    long platforms_24 = random();
    platforms_24 |= 3l << 0; // no hill at the display boundary
    platforms_24 |= 3l << 2 * (DISPLAY_WIDTH/16 - 1); 
    long nofloor = random();
    //nofloor = INT32_MAX; // turn off water
    
    if (y == 5) // ceiling
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

void redraw()
{
    clear();
    
    drawlabels();
    
    // print ceiling 
    for (uint8_t x = 0; x < DISPLAY_WIDTH; x++)
    {
        page(x, 5, pgm_read_byte_near(floorsprite + x % 16));
    }

    drawplatform();
    drawfloor();

    if (doors & 0b00000010)
        drawdoorleft_closed();
    if (doors & 0b00000001)
        drawdoorright_closed();

    for (uint8_t i = 0; i < NUM_MONSTERS; ++i)
    {
        if (monsters[i]->movement != HIDDEN)
            draw(monsters[i]);
    }
    if (projectile->movement != HIDDEN)
        draw(projectile);
    if (bombstruct->movement != HIDDEN)
        draw(bombstruct);

    if (energytankstruct->movement != HIDDEN)
    {
        draw(energytankstruct);
    }
    
    draw(protagonist);
}

void selectfloor()
{
    switch (random_below(7))
    {
        case 0:
            floorsprite = floor1;
            rotatedfloorsprite = floor1_rotated;
            break;
        case 1:
            floorsprite = floor2;
            rotatedfloorsprite = floor2_rotated;
            break;
        case 2:
            floorsprite = floor3;
            rotatedfloorsprite = floor3_rotated;
            break;                           
        case 3:
            floorsprite = floor4;
            rotatedfloorsprite = floor4_rotated;
            break;
        case 4:
            floorsprite = floor5;
            rotatedfloorsprite = floor5_rotated;
            break;
        case 5:
            floorsprite = floor6;
            rotatedfloorsprite = floor6_rotated;
            break;
        case 6:
            floorsprite = floor7;
            rotatedfloorsprite = floor7_rotated;
            break;
    }
    switch (random_below(2))
    {
        case 0:
            nofloorsprite = water;
            break;
        case 1:
            nofloorsprite = spikes;
            break;
    }
}

void newlevelpos()
{
    srand(level_seed + level_pos);
    srandom(level_seed + level_pos);
    
    platforms_13 = random();
    platforms_19 = random();
    platforms_24 = random();
    platforms_24 |= 1l << 0; // no hill at the display boundary
    platforms_24 |= 1l << 2 * (DISPLAY_WIDTH/16 - 1);
    nofloor = random();
    nofloor|= 1l << 0; 
    nofloor|= 1l << 3 * (DISPLAY_WIDTH/16 - 1);
    for (uint8_t pos = 0; pos < DISPLAY_WIDTH / 16; ++pos)
    {
        if (!(platforms_24 & (3l << 2 * pos)))
        {
            nofloor |= 1l << 3 * pos;
        }
    }
    doors = 0;
    
    // draw door to previous level
    if (level_pos == 0)
    {
        doors |= 0b00000010;
    }

    // draw exit door
    if (level_pos == max_level_pos)
    {
        doors |= 0b00000001;
    }
    
    monsters[0]->look = random_below(NUM_MONSTER_LOOKS);
    for (uint8_t i = 0; i < NUM_MONSTERS; ++i)
    {
        if (i > 0 && monsters[0]->look != LOOK_MONSTER_MEMU)
        {
            monsters[i]->movement = HIDDEN;
            continue;
        }
        // make memus appear in swarms
        if (i > 0)
            monsters[i]->look = LOOK_MONSTER_MEMU;
        initcharacter(monsters[i]);
        monsters[i]->x = (DISPLAY_WIDTH - monsters[i]->width) / 2;
    
        // move monster to the right if there is water/spikes below
        uint8_t nofloor = 1;
        while (nofloor)
        {
            nofloor = 0;
            for (uint8_t x = monsters[i]->x; x < monsters[i]->x + monsters[i]->width; x++)
            {
                if (!obstacle(x, 25))
                    nofloor = 1;
            }
            if (nofloor)
                monsters[i]->x++;
        }
        monsters[i]->y = 25 - monsters[i]->height;
        
        // draw monster higher if it's on a hill
        for (uint8_t x = monsters[i]->x; x < monsters[i]->x + monsters[i]->width; x++)
        {
            if (obstacle_hill(x))
            {
                monsters[i]->y--;
                break;
            }
        }
    }

    projectile->movement = HIDDEN;
    bombstruct->movement = HIDDEN;

    energytankstruct->look = LOOK_ENERGYTANK;
    initcharacter(energytankstruct);
    energytankstruct->x = really_random_below(DISPLAY_WIDTH - 9);
    if (really_random_below(2) == 0)
    {
        energytankstruct->y = 19 - energytankstruct->height;
    }
    else
    {
        energytankstruct->y = 13 - energytankstruct->height;
    }
    for (uint8_t x = energytankstruct->x; x < energytankstruct->x + 9; x++)
    {
        if (!obstacle(x, energytankstruct->y + energytankstruct->height))
        {
            energytankstruct->movement = HIDDEN;
        }
    }
    
    redraw();
}

void newlevel()
{
    if (protagonist->x > DISPLAY_WIDTH / 2)
    {
        level_seed += 2 * MAX_LEVEL_WIDTH + 1;
    }
    else // back to the previous level
    {
        level_seed -= 2 * MAX_LEVEL_WIDTH + 1;
    }

    srand(level_seed);
    srandom(level_seed);
    
    max_level_pos = random_below(MAX_LEVEL_WIDTH);

    if (protagonist->x > DISPLAY_WIDTH / 2)
    {
        level_pos = 0;
        movedoorleft();
        protagonist->x = 6 + 1;
        protagonist->direction = DIRECTION_RIGHT;
    }
    else
    {
        level_pos = max_level_pos;
        movedoorright();
        protagonist->x = DISPLAY_WIDTH - 6 - protagonist->width - 1;
        protagonist->direction = DIRECTION_LEFT;
    }
    
    protagonist->y = 25 - protagonist->height;
    
    selectfloor();

    newlevelpos();
}

void newgame()
{
    level_seed = 2845215237l;
    num_rockets = 20;
    num_bombs = 20;

    protagonist->look = LOOK_PROTAGONIST;
    initcharacter(protagonist);
    protagonist->x = DISPLAY_WIDTH; // make the protagonist appear on the left

    projectile->look = LOOK_ROCKET;
    initcharacter(projectile);
    
    bombstruct->look = LOOK_BOMB;
    initcharacter(bombstruct);

    newlevel();
}
