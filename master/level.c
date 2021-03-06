#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "timer.h"
#include "level.h"
#include "display.h"
#include "character.h"
#include "drawing.h"
#include "sprites.h"
#include "rand.h"
#include "dfs.h"
#include "uart.h"

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

EEMEM uint32_t initial_level_stored;
EEMEM int32_t level_stored;
EEMEM uint8_t health_stored;
EEMEM uint8_t num_rockets_stored;
EEMEM uint8_t num_bombs_stored;
EEMEM bool Rocket_Upgrade_stored;
EEMEM bool Run_And_Jump_Faster_Upgrade_stored;
EEMEM bool Bigger_Bomb_Explosion_stored;

long obstacle(uint8_t x, uint8_t y)
{
    if (y >= CEILING_Y && y < CEILING_Y + 4) // ceiling
        return 1l;
    // left door
    else if (doors & 0b00000010 && (x < 4 || (y >= DOOR_Y && x < 6)))
        return 1l;
    // right door
    else if (doors & 0b00000001 && (x >= DISPLAY_WIDTH - 4 || (y >= DOOR_Y && x >= DISPLAY_WIDTH - 6)))
        return 1l;
    else if (rechargeroom && x >= DISPLAY_WIDTH/2 - 12 && x < DISPLAY_WIDTH/2 + 12 && (y >= 23*4 || y < 17*4))
        return 1l;
    else if (rechargeroom && recharging && (x == DISPLAY_WIDTH/2 - 12 || x == DISPLAY_WIDTH/2 + 11))
        return 1l;
    else if (rechargeroom && y <= CEILING_Y + 4*5)
        return 1l;
    // water/spikes
    else if (y >= FLOOR_Y && y < FLOOR_Y + 4)
        return nofloor & (3l << x / 16 * 2);
    else if (y >= 19 * 4 && y < 20 * 4)
        return !(platforms_19 & (3l << (x / PLATFORM_WIDTH * 2)));
    else if (y >= 13 * 4 && y < 14 * 4)
        return !(platforms_13 & (3l << (x / PLATFORM_WIDTH * 2)));
    else if (y >= 24 * 4 && y < 25 * 4)
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

    if (y >= CEILING_Y && y < CEILING_Y + 4) // ceiling
        return 1l;
    else if (y >= 19 * 4 && y < 20 * 4)
        return !(platforms_19 & (3l << (x / PLATFORM_WIDTH * 2)));
    else if (y >= 13 * 4 && y < 14 * 4)
        return !(platforms_13 & (3l << (x / PLATFORM_WIDTH * 2)));
    else if (y >= 24 * 4 && y < 25 * 4)
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
        page(x, 5, pgm_read_byte_near(ceilingsprite + x % 16));
    }

    drawplatform();
    drawfloor();

    if (doors & 0b00000010)
    {
        drawdoorleft_closed();
        if (left_door_open)
            drawsprite(0, 20, 6, 5, doorleft_open);
    }
    if (doors & 0b00000001)
    {
        drawdoorright_closed();
        if (right_door_open)
            drawsprite(DISPLAY_WIDTH - 6, 20, 6, 5,  doorright_open);
    }

    for (uint8_t i = 0; i < NUM_MONSTERS; ++i)
    {
        if (monsters[i]->movement != HIDDEN)
            draw(monsters[i]);
    }
    for (uint8_t i = 0; i < NUM_ROCKETS; ++i)
    {
        if (projectiles[i]->movement != HIDDEN)
            draw(projectiles[i]);
    }
    if (bombstruct->movement != HIDDEN)
        draw(bombstruct);

    if (energytankstruct->movement != HIDDEN)
    {
        draw(energytankstruct);
    }

    if (rechargeroom)
    {
        drawrechargeroom();
    }

    draw(protagonist);
}

void selectfloor()
{
    // random floor sprite
    const uint8_t* rotatedfloorsprite = NULL;
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
    ceilingsprite = floorsprite;
    leftrotatedfloorsprite = rotatedfloorsprite;
    rightrotatedfloorsprite = rotatedfloorsprite;

    // select from water and spikes
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
    protagonist->jumpheight = 28; // reset jumpheight because protagonist can jump higher in secrob level
    rechargeroom = false;
    bosslevel = false;

    for (uint8_t i = 0; i < NUM_MONSTERS; ++i)
        monsters[i]->look = LOOK_HIDDEN;

    if ((level >= 0 && level % BOSS_LEVEL_DISTANCE == BOSS_LEVEL_DISTANCE - 2) // recharge level
        || (level < 0 && (level - 1) % BOSS_LEVEL_DISTANCE == 0))
    {
        platforms_13 = UINT32_MAX;
        platforms_19 = UINT32_MAX;
        platforms_24 = UINT32_MAX;
        nofloor = UINT32_MAX;
        doors = 0b00000011;

        rechargeroom = true;
    }
    else if ((level >= 0 && level % BOSS_LEVEL_DISTANCE == BOSS_LEVEL_DISTANCE - 1) // boss level
        || (level < 0 && level % BOSS_LEVEL_DISTANCE == 0))
    {
        level_pos = 0;
        srand(level_seed);
        srandom(level_seed);

        platforms_13 = UINT32_MAX;
        platforms_19 = UINT32_MAX;
        platforms_24 = UINT32_MAX;
        nofloor = UINT32_MAX;
        doors = 0b00000011;

        bosslevel = true;

        monsters[0]->direction = 1 - protagonist->direction; // look at the protagonist

        switch(random_below(4))
        {
            case 0:
                monsters[0]->look = LOOK_BOSS_MEGACOREX;
                uart_putc('b');
                break;
            case 1:
                monsters[0]->look = LOOK_BOSS_SECROB;
                uart_putc('c');
                for (uint8_t i = 0; i < 4; ++i)
                {
                    fireballs[i]->look = LOOK_ARROW;
                    initcharacter(fireballs[i]);
                }
                for (uint8_t i = 4; i < 6; ++i)
                {
                    fireballs[i]->look = LOOK_ARROW_UP;
                    initcharacter(fireballs[i]);
                }
                protagonist->jumpheight = 36;
                ceilingsprite = climbceiling;
                leftrotatedfloorsprite = climbleft;
                rightrotatedfloorsprite = climbright;
                break;
            case 2: 
                monsters[0]->look = LOOK_BOSS_ZAZABI;
                uart_putc('d');
                monsters[0]->direction = protagonist->direction; // begin jumping towards the protagonist
                break;
            case 3:
                monsters[0]->look = LOOK_NEO_RIDLEY_DRAGON;
                uart_putc('d');
                for (uint8_t i = 0; i < NUM_FIREBALLS; ++i)
                {
                    fireballs[i]->look = LOOK_FIREBALL;
                    initcharacter(fireballs[i]);
                }
                platforms_13 = 0b00111111111111111111111111111100;
                platforms_19 = 0b00111111111111111111111111111100;
                break;
        }
    }
    else // normal level
    {
        srand(level_seed + level_pos);
        srandom(level_seed + level_pos);

        platforms_13 = random();
        platforms_19 = random();
        platforms_24 = random();
        platforms_24 |= 1l << 0; // no hill at the display boundary
        platforms_24 |= 1l << 2 * (DISPLAY_WIDTH/16 - 1);

        // set nofloor to a new random value as long as the door is not reachable
        do
        {
            nofloor = random();
            nofloor|= 1l << 0; 
            nofloor|= 1l << 2 * (DISPLAY_WIDTH/16 - 1);
            for (uint8_t pos = 0; pos < DISPLAY_WIDTH / 16; ++pos)
            {
                if (!(platforms_24 & (3l << 2 * pos)))
                {
                    nofloor |= 1l << 2 * pos;
                }
            }
        }
        while (!is_door_reachable());

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
        if (monsters[0]->look == LOOK_MONSTER_MEMU) // make memus appear in swarms
        {
            for (uint8_t i = 1; i < NUM_MONSTERS; ++i)
                monsters[i]->look = LOOK_MONSTER_MEMU;
        }
        else
        {
            uint8_t bosses_killed;
            if (level >= 0)
                bosses_killed = level / BOSS_LEVEL_DISTANCE;
            else
                bosses_killed = (-level + 1) / BOSS_LEVEL_DISTANCE;
            // the number of monsters that can possibly appear increases for every boss killed
            uint8_t num_monsters = random_below(MIN(bosses_killed + 1, NUM_MONSTERS)) + 1;
            for (uint8_t i = 1; i < num_monsters; ++i)
            {
                monsters[i]->look = random_below(NUM_MONSTER_LOOKS);
                if (monsters[i]->look == LOOK_MONSTER_MEMU)
                    monsters[i]->look = LOOK_HIDDEN;
            }
        }
    }

    // initialize monsters, their look, position etc.
    for (uint8_t i = 0; i < NUM_MONSTERS; ++i)
    {
        initcharacter(monsters[i]);
        if (monsters[i]->look == LOOK_HIDDEN)
            continue;

        monsters[i]->x = (DISPLAY_WIDTH - monsters[i]->width) / 2;
        if (monsters[i]->look == LOOK_BOSS_MEGACOREX || monsters[i]->look == LOOK_BOSS_SECROB || monsters[i]->look == LOOK_BOSS_ZAZABI || monsters[i]->look == LOOK_NEO_RIDLEY_DRAGON)
        {
            if (protagonist->x <= DISPLAY_WIDTH/2)
            {
                monsters[i]->x = DISPLAY_WIDTH - 12 - monsters[i]->width;
            }
            else
            {
                monsters[i]->x = 12;
            }
        }
        // move monster to the right if there is water/spikes below
        uint8_t nofloor = 1;
        while (nofloor)
        {
            nofloor = 0;
            for (uint8_t x = monsters[i]->x; x < monsters[i]->x + monsters[i]->width; x++)
            {
                if (!obstacle(x, FLOOR_Y))
                    nofloor = 1;
            }
            if (nofloor)
                monsters[i]->x++;
        }

        monsters[i]->y = FLOOR_Y - monsters[i]->height;
        // draw monster higher if it's on a hill
        for (uint8_t x = monsters[i]->x; x < monsters[i]->x + monsters[i]->width; x++)
        {
            if (obstacle_hill(x))
            {
                monsters[i]->y -= 4;
                break;
            }
        }
    }

    for (uint8_t i = 0; i < NUM_FIREBALLS; ++i)
    {
        fireballs[i]->movement = HIDDEN;
    }

    // no water/spikes when there is a frog/sidehopper
    // these would otherwise fall into the void
    for (uint8_t i = 0; i < NUM_MONSTERS; ++i)
    {
        if (monsters[i]->movement == JUMPMOVE)
            nofloor = UINT32_MAX;
    }

    for (uint8_t i = 0; i < NUM_ROCKETS; ++i)    
        projectiles[i]->movement = HIDDEN;
    bombstruct->movement = HIDDEN;

    energytankstruct->look = LOOK_ENERGYTANK;
    initcharacter(energytankstruct);
    if (rechargeroom || bosslevel)
    {
        energytankstruct->movement = HIDDEN;
    }
    else
    {
        // -8 and +4 to ensure the energy tank is not inside the wall
        energytankstruct->x = really_random_below(DISPLAY_WIDTH - energytankstruct->width - 8) + 4;
        if (really_random_below(2) == 0)
        {
            energytankstruct->y = 19 * 4 - energytankstruct->height;
        }
        else
        {
            energytankstruct->y = 13 * 4 - energytankstruct->height;
        }
        for (uint8_t x = energytankstruct->x; x < energytankstruct->x + energytankstruct->width; x++)
        {
            // ensure that the energy tank is standing on a platform
            if (!obstacle(x, energytankstruct->y + energytankstruct->height))
            {
                energytankstruct->movement = HIDDEN;
            }
        }
    }

    for (uint8_t i = 0; i < NUM_MONSTERS; ++i)
    {
        xparasites[i]->movement = HIDDEN;
    }

    if (bosslevel)
    {
        // show a nice message informing about the boss's abilities
        redraw();
        char line[2][MAX_STRING_LEN];
        switch (monsters[0]->look)
        {
            case LOOK_BOSS_SECROB:
                strncpy(line[0], "SAMUS, THIS IS THE SECROB", MAX_STRING_LEN);
                strncpy(line[1], "CLIMB THE WALL TO SURVIVE!", MAX_STRING_LEN);
                break;
            case LOOK_BOSS_MEGACOREX:
                strncpy(line[0], "SAMUS WATCH OUT! MEGACOREX", MAX_STRING_LEN);
                strncpy(line[1], "IS TRYING TO KILL YOU", MAX_STRING_LEN);
                break;
            case LOOK_BOSS_ZAZABI:
                strncpy(line[0], "SAMUS, FIGHT AGAINST ZAZABI", MAX_STRING_LEN);
                strncpy(line[1], "TRY NOT TO GET EATEN", MAX_STRING_LEN);
                break;
            case LOOK_NEO_RIDLEY_DRAGON:
                strncpy(line[0], "SAMUS, BEWARE OF NEO RIDLEY.", MAX_STRING_LEN);
                strncpy(line[1], "WATCH OUT WHEN HE IS FLYING.", MAX_STRING_LEN);
                break;
            default:
                break;
        }
        // print text
        char buffer[MAX_STRING_LEN];
        for (int j = 0; j < 2; j++)
        {
            uint8_t len = strlen(line[j]);
            for (int i = 0; i < len; i++)
            {
                buffer[i] = line[j][i];
                buffer[i + 1] = '\0';
                drawletters(10, CEILING_Y / 4 + 3 + j * 3, buffer, false);
                delay(100);             
            }
        }
        delay(1000);
    }

    redraw();
}

void newlevel()
{
    //ingame music
    if (level % 2)
        uart_putc('i');
    else
        uart_putc('j');

    eeprom_write_block(&level, &level_stored, sizeof level);

    level_seed = initial_level + level * (2 * MAX_LEVEL_WIDTH + 1);

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

    protagonist->y = FLOOR_Y - protagonist->height;

    selectfloor();

    left_door_open = false;
    right_door_open = false; 

    newlevelpos();
}

void newgame()
{
    protagonist->look = LOOK_PROTAGONIST;
    initcharacter(protagonist);

    if (initial_level == 0) // start a new game
    {
        initial_level = getMsTimer();
        level = 0;
        Run_And_Jump_Faster_Upgrade = false;
        Rocket_Upgrade = false;
        Bigger_Bomb_Explosion = false;
        eeprom_write_block(&Bigger_Bomb_Explosion, &Bigger_Bomb_Explosion_stored, sizeof Bigger_Bomb_Explosion);
        eeprom_write_block(&Rocket_Upgrade, &Rocket_Upgrade_stored, sizeof Rocket_Upgrade);
        eeprom_write_block(&Run_And_Jump_Faster_Upgrade, &Run_And_Jump_Faster_Upgrade_stored, sizeof Run_And_Jump_Faster_Upgrade);
        eeprom_write_block(&initial_level, &initial_level_stored, sizeof initial_level);
        eeprom_write_block(&level, &level_stored, sizeof level);
        eeprom_write_block(&protagonist->health, &health_stored, sizeof protagonist->health);
        num_rockets = 30;
        eeprom_write_block(&num_rockets, &num_rockets_stored, sizeof num_rockets);
        num_bombs = 10;
        eeprom_write_block(&num_bombs, &num_bombs_stored, sizeof num_bombs);
    }
    else // resume previous game
    {
        eeprom_read_block(&Rocket_Upgrade, &Rocket_Upgrade_stored, sizeof Rocket_Upgrade);
        eeprom_read_block(&Run_And_Jump_Faster_Upgrade, &Run_And_Jump_Faster_Upgrade_stored, sizeof Run_And_Jump_Faster_Upgrade);
        eeprom_read_block(&level, &level_stored, sizeof level);
        protagonist->health = eeprom_read_byte(&health_stored);
        num_rockets = eeprom_read_byte(&num_rockets_stored);
        num_bombs = eeprom_read_byte(&num_bombs_stored);
    }

    if (level < 0)
        protagonist->x = 0; // make the protagonist appear on the right
    else
        protagonist->x = DISPLAY_WIDTH; // make the protagonist appear on the left

    for (uint8_t i = 0; i < NUM_ROCKETS; ++i)
    {
        projectiles[i]->look = LOOK_ROCKET;
        initcharacter(projectiles[i]);
    }

    bombstruct->look = LOOK_BOMB;
    initcharacter(bombstruct);

    left_door_open = true;
    right_door_open = true; 

    newlevel();
}
