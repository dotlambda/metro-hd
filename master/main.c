#include <stdlib.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdbool.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include "uart.h"
#include "adc.h"
#include "timer.h"
#include "buttons.h"
#include "display.h"
#include "character.h"
#include "level.h"
#include "sprites.h"
#include "rand.h"
#include "drawing.h"

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

#define DIST_AFTER_DAMAGE 10

uint32_t nextmoveevent = 0;
uint32_t nextjumpevent = 0;
uint32_t nextprojectilevent[NUM_ROCKETS];
uint32_t nextshootevent = 0;
uint32_t nextbombevent = 0;
uint32_t explode = 0;
uint32_t nextmonstermoveevent[NUM_MONSTERS];
uint32_t nextmonsterjumpevent[NUM_MONSTERS];
uint32_t nextfireballmoveevent[NUM_FIREBALLS];
uint32_t nextfireballjumpevent[NUM_FIREBALLS];
uint32_t nextfireevent = 0;
uint32_t nextrechargeevent = 0;

void init();

void takingdamage(uint8_t damage)
{
    uint32_t blink_for = 650;
    protagonist->health = protagonist->health - damage;
    if(protagonist->health > 0)
    {
        eeprom_write_byte(&health_stored, protagonist->health);
        drawnumber(29, 1, protagonist->health);
    }
    else
    {
        initial_level = 0;
        eeprom_write_dword(&initial_level_stored, initial_level);
        drawnumber(29, 1, 0);
        blink_for = 2000;
    }
    uint32_t blinking_time = getMsTimer();
    while(blinking_time + blink_for >= getMsTimer())
    {
        // make protagonist and energy level blink (latter only if energy <= 0)
        hide(protagonist);
        if (protagonist->health <= 0)
        {
            drawcolor(29, 1, 7, 3, 0x00);
        }
        delay(50);
        draw(protagonist);
        if (protagonist->health <= 0)
        {    
            drawnumber(29, 1, 0);
        }
        delay(100);
    }
    if (protagonist->health <= 0)
    {
        // game over
        uint16_t i = 0;
        for (uint8_t y = 0; y < DISPLAY_HEIGHT / 4; y++)
        {
            for (uint8_t x = 0; x < DISPLAY_WIDTH; x++)
            {
                if (y > 7 && y < 21 && x > 17 && x < 141)
                {
                    page(x, y, pgm_read_byte_near(gameover + i));
                    i++;
                }
                else
                {
                    page(x, y, 0);
                }
            }
            delay(30);
        }
        while (!B_A);
        newgame();
    }
}

bool open_door(struct Character* projectile)
{
    if (projectile->x <= 6 && projectile->y >= 15 && (doors & 0b00000010))
    {
        if(!(level >= 0 && level % BOSS_LEVEL_DISTANCE == BOSS_LEVEL_DISTANCE - 1)
            || (level < 0 && level % BOSS_LEVEL_DISTANCE == 0))
        {
            drawsprite(0, 20, 6, 5, doorleft_open);
            left_door_open = true;
            return 1;
        }
        else if(((level >= 0 && level % BOSS_LEVEL_DISTANCE == BOSS_LEVEL_DISTANCE - 1) // boss level
            || (level < 0 && level % BOSS_LEVEL_DISTANCE == 0))
            && monsters[0]->movement == HIDDEN)
        {
            drawsprite(0, 20, 6, 5, doorleft_open);
            left_door_open = true;
            return 1;                    
        }
    }
    else if (projectile->x >= DISPLAY_WIDTH - 6 - projectile->width && projectile->y >= 15 && (doors & 0b00000001))
    {
        if(!((level >= 0 && level % BOSS_LEVEL_DISTANCE == BOSS_LEVEL_DISTANCE - 1)
            || (level < 0 && level % BOSS_LEVEL_DISTANCE == 0)))
        {
            drawsprite(DISPLAY_WIDTH - 6, 20, 6, 5, doorright_open); 
            right_door_open = true;
            return 1;
        }
        else if(((level >= 0 && level % BOSS_LEVEL_DISTANCE == BOSS_LEVEL_DISTANCE - 1) // boss level
            || (level < 0 && level % BOSS_LEVEL_DISTANCE == 0))
            && monsters[0]->movement == HIDDEN)
        {
            drawsprite(DISPLAY_WIDTH - 6, 20, 6, 5, doorright_open);
            right_door_open = true;
            return 1;                 
        }
    }
    return 0;
}

bool collision(struct Character* protagonist, struct Character* monster)
{
    return (protagonist->x < monster->x + monster->width && protagonist->x + protagonist->width > monster->x &&
        protagonist->y < monster->y + monster->height && protagonist->y + protagonist->height > monster->y);
}

void newxparasite(uint8_t i) // i is the index of the dead monster
{
    switch (really_random_below(3))
    {
        case 0:
            xparasites[i]->look = LOOK_XPARASITE1;
            initcharacter(xparasites[i]);
            break;
        case 1:
            xparasites[i]->look = LOOK_XPARASITE2;
            initcharacter(xparasites[i]);
            break;
        case 2:
            break;
    }
    if (xparasites[i]->movement != HIDDEN)
    {
        xparasites[i]->x = monsters[i]->x + monsters[i]->width / 2;
        xparasites[i]->y = monsters[i]->y;
        draw(xparasites[i]);
    }
}

void monstertakedamage(uint8_t i, uint8_t damage) // i is the index of the monster taking damage
{
    monsters[i]->health -= damage;
    drawmonsterhealth(monsters[i]);
    if (monsters[i]->health <= 0)
    {
        hide(monsters[i]);
        if (monsters[i]->look == LOOK_BOSS_DRAGON || monsters[i]->look == LOOK_BOSS_ZAZABI || monsters[i]->look == LOOK_BOSS_SECROB || monsters[i]->look == LOOK_NEO_RIDLEY_DRAGON)
        {
            monsters[i]->look = LOOK_BIGXPARASITE;
            initcharacter(monsters[i]);
        }
        else
        {
            newxparasite(i); 
        }
    }
    else
    {
        draw(monsters[i]);
    }
}

int main(void)
{
	init();

    // show splash screen until button A is pressed
    initial_level = eeprom_read_dword(&initial_level_stored);
    drawsplash(initial_level != 0);
    while (1)
    {
        if (B_A)
        {
            initial_level = 0; // start a new game
            break;
        }
        if (initial_level != 0 && B_B)
        {
            break;
        }
    }
            
    
    struct Character protagonist_;
    protagonist = &protagonist_;

    for (uint8_t i = 0; i < NUM_MONSTERS; ++i)
    {
        nextmonstermoveevent[i] = 0;
        nextmonsterjumpevent[i] = 0;
        monsters[i] = &monsters_[i];
    }
    
    for (uint8_t i = 0; i < NUM_ROCKETS; ++i)
    {
        nextprojectilevent[i]=0;
        projectiles[i] = &projectiles_[i];
    }

    struct Character energytank_;
    energytankstruct = &energytank_;

    for (uint8_t i = 0; i < NUM_MONSTERS; ++i)
    {
        xparasites[i] = &xparasites_[i];
    }
    
    for (uint8_t i = 0; i < NUM_FIREBALLS; ++i)
    {
        nextfireballmoveevent[i] = 0;
        nextfireballjumpevent[i] = 0;
        fireballs[i] = &fireballs_[i];
    }
    
    struct Character bomb_;
    bombstruct = &bomb_;
    
    left_door_open = true;
    right_door_open = true;    
    newgame();
    
    while (1)
    {
        //monster in Bewegung
        for (uint8_t i = 0; i < NUM_MONSTERS; ++i)
        {
            if(monsters[i]->movement != HIDDEN && nextmonstermoveevent[i] < getMsTimer())
            {
                move(monsters[i]);
                nextmonstermoveevent[i] = getMsTimer() + monsters[i]->x_pace;
            }
            if(monsters[i]->movement != HIDDEN && nextmonsterjumpevent[i] < getMsTimer())
            {
                jump(monsters[i]);
                nextmonsterjumpevent[i] = getMsTimer() + monsters[i]->y_pace;
            }
        }
        
        //Protagonist kann sich bewegen
        if (nextmoveevent < getMsTimer())
        {
            if (B_RIGHT)
            {
                if (protagonist->x + protagonist->width == DISPLAY_WIDTH)
                {
                    long obstacle = 0l;
                    for (uint8_t x = 0; x < protagonist->width; ++x)
                    {
                        for (uint8_t y = protagonist->y; y < protagonist->y + protagonist->height; ++y)
                        {
                            obstacle |= obstacle_levelpos(x, y, level_pos + 1);
                        }
                    }
                    if (!obstacle)
                    {
                        ++level_pos;
                        protagonist->x = 0;
                        newlevelpos();
                        checkfalling(protagonist);
                    }
                }
                else
                {
                    moveright(protagonist);
                }
                nextmoveevent = getMsTimer() + 50;
            }
            if (B_LEFT)
            {
                if (protagonist->x == 0)
                {
                    long obstacle = 0l;
                    for (uint8_t x = DISPLAY_WIDTH - protagonist->width; x < DISPLAY_WIDTH; ++x)
                    {
                        for (uint8_t y = protagonist->y; y < protagonist->y + protagonist->height; ++y)
                        {
                            obstacle |= obstacle_levelpos(x, y, level_pos - 1);
                        }
                    }
                    if (!obstacle)
                    {
                        --level_pos;
                        protagonist->x = DISPLAY_WIDTH - protagonist->width;
                        newlevelpos();
                        checkfalling(protagonist);
                    }
                }
                else
                {
                    moveleft(protagonist);
                }
                nextmoveevent = getMsTimer() + 50;
            }
        }
        if (protagonist->jumpstate == CLIMBING)
        {
            if (nextjumpevent < getMsTimer())
            {
                if (B_DOWN)
                {
                    if (protagonist->x == 4 || protagonist->x == DISPLAY_WIDTH - 4 - protagonist->width)
                    {
                        movedown(protagonist);
                    }
                    else
                    {
                        hide(protagonist);
                        protagonist->jumpstate = protagonist->jumpheight;
                        jump(protagonist);
                    }
                }
                if (B_UP && protagonist->y != CEILING_Y + 4)
                {
                    moveup(protagonist);
                }
                if (protagonist->y != CEILING_Y + 4
                    && protagonist->x != 4 && protagonist->x != DISPLAY_WIDTH - 4 - protagonist->width)
                {
                    // fall down if she's not right below the ceiling or at the left/right boundary
                    hide(protagonist);
                    protagonist->jumpstate = protagonist->jumpheight;
                    jump(protagonist);
                }
                nextjumpevent = getMsTimer() + 50;
            }
        }
        else if (protagonist->jumpstate != ON_THE_GROUND)
        {
            if (nextjumpevent < getMsTimer())
            {
                if (monsters[0]->look == LOOK_BOSS_SECROB
                    && (protagonist->y == CEILING_Y + 4
                        || protagonist->x == 4
                        || protagonist->x == DISPLAY_WIDTH - 4 - protagonist->width)
                   )
                {
                    hide(protagonist);
                    protagonist->jumpstate = CLIMBING;
                    draw(protagonist);
                }
                else
                {
                    jump(protagonist);
                }
                    
                if (protagonist->jumpstate == protagonist->jumpheight)
                    nextjumpevent = getMsTimer() + 20;
                else
                    nextjumpevent = getMsTimer() + 30;
            }
        }
        else if (B_UP)
        {
            protagonist->jumpstate = 1;
        }
        
        // change level when protagonist touches the door
        if (doors & 0b00000001
            && protagonist->x >= DISPLAY_WIDTH - 6 - protagonist->width 
            && protagonist->y >= DOOR_Y - protagonist->height && right_door_open)
        {
            // im endbosslevel
            if (monsters[0]->look == LOOK_BOSS_DRAGON || monsters[0]->look == LOOK_BOSS_SECROB || monsters[0]->look == LOOK_BOSS_ZAZABI || monsters[0]->look == LOOK_BIGXPARASITE)
            {
                // falls boss tot
                if (monsters[0]->movement == HIDDEN)
                {
                        level++;
                        newlevel();
                }
            }
            else 
            {
                level++;
                newlevel();
            }
        }
        else if (doors & 0b00000010
            && protagonist->x <= 6 
            && protagonist->y >= DOOR_Y - protagonist->height && left_door_open)
        {
            if(monsters[0]->look == LOOK_BOSS_DRAGON || monsters[0]->look == LOOK_BOSS_SECROB || monsters[0]->look == LOOK_BOSS_ZAZABI || monsters[0]->look == LOOK_BIGXPARASITE)
            {
                if (monsters[0]->movement == HIDDEN)
                {
                    level--;
                    newlevel();
                }
            }
            else 
            {
                level--;
                newlevel();
            }
        }
        
        //PROJECTILE
        for (uint8_t i = 0; i < NUM_ROCKETS; ++i)
        {    
            if (projectiles[i]->movement == HIDDEN
                && num_rockets > 0
                && nextshootevent < getMsTimer()
                && B_A)
            {
                uint8_t enough_space = 1;
                projectiles[i]->direction = protagonist->direction;
                projectiles[i]->y = protagonist->y + 4;
                if (protagonist->direction == DIRECTION_LEFT)
                {
                    if (protagonist->x < projectiles[i]->width)
                    {
                        enough_space = 0;
                        projectiles[i]->x = 0;
                    }
                    else
                    {
                        projectiles[i]->x = protagonist->x - projectiles[i]->width;
                    }
                }
                else
                {
                    if (protagonist->x + protagonist->width + projectiles[i]->width >= DISPLAY_WIDTH)
                    {
                        enough_space = 0;
                        projectiles[i]->x = DISPLAY_WIDTH;
                    }

                    else
                    {
                        projectiles[i]->x = protagonist->x + protagonist->width;
                    }
                }
                if (enough_space)
                {
                    for (uint8_t x = projectiles[i]->x; x < projectiles[i]->x + projectiles[i]->width; ++x)
                    {
                        for (uint8_t y = projectiles[i]->y; y < projectiles[i]->y + projectiles[i]->height; ++y)
                        {
                            if (obstacle(x, y))
                                enough_space = 0;
                        }
                    }
                }
                if (enough_space)
                {
                    projectiles[i]->movement = PROJECTILE;
                    draw(projectiles[i]);
                    num_rockets--;
                    eeprom_write_byte(&num_rockets_stored, num_rockets);
                    drawnumber(57, 1, num_rockets);
                    nextprojectilevent[i] = getMsTimer() + 35;
                    nextshootevent = getMsTimer() + 500;
                }
                else if(open_door(projectiles[i]))
                {
                    num_rockets--;
                    eeprom_write_byte(&num_rockets_stored, num_rockets);
                    drawnumber(57, 1, num_rockets);
                    nextshootevent = getMsTimer() + 500;
                }
            }
            else if (projectiles[i]->movement != HIDDEN
                && nextprojectilevent[i] < getMsTimer())
            {
                move(projectiles[i]);
                open_door(projectiles[i]);
                nextprojectilevent[i] = getMsTimer() + 35;
            }

            for (uint8_t j = 0; j < NUM_MONSTERS; ++j)
            {
                if (projectiles[i]->movement != HIDDEN && monsters[j]->movement != HIDDEN && collision(projectiles[i], monsters[j]))
                {
                    hide(projectiles[i]);
                    monstertakedamage(j, projectiles[i]->damage);
                }
            }
        }
        //PROJECTILE END

        //BOMB
        if (nextbombevent < getMsTimer())
        {
            if (bombstruct->movement != HIDDEN)
            {
                jump(bombstruct);
                for (uint8_t i = 0; i < NUM_MONSTERS; i++)
                {
                    if(monsters[i]->movement != HIDDEN)
                    {
                        draw(monsters[i]);
                    }
                }
                if (bombstruct->y > FLOOR_Y - bombstruct->height)
                {
                    hide(bombstruct);
                    drawfloor();
                }
                else if (!collision(protagonist, bombstruct)) // don't make the bomb overwrite protagonist
                {
                    draw(protagonist);
                }
                nextbombevent = getMsTimer() + 20;
            }
            else if (B_B && num_bombs > 0)
            {
                bombstruct->x = protagonist->x;
                bombstruct->y = protagonist->y + protagonist->height - bombstruct->height;
                bombstruct->movement = BOMB;
                num_bombs--;
                eeprom_write_byte(&num_bombs_stored, num_bombs);
                drawnumber(86, 1, num_bombs);
                checkfalling(bombstruct);
                explode = getMsTimer() + 2000;
                nextbombevent = getMsTimer() + 100;
            }
        }
        if (bombstruct->movement != HIDDEN)
        {
            if (explode < getMsTimer())
            {
                hide(bombstruct);
                uint8_t blast_x1 = MAX(0, bombstruct->x - 6);
                uint8_t blast_x2 = MIN(bombstruct->x + bombstruct->width + 6, DISPLAY_WIDTH);
                uint8_t blast_y1 = MAX(CEILING_Y + 4, bombstruct->y - 4);
                uint8_t blast_y2 = MIN(bombstruct->y + bombstruct->height + 8, FLOOR_Y);
                uint16_t i = 0;
                for (int16_t y = bombstruct->y / 4 - 1; y < bombstruct->y / 4 + bombstruct->height / 4 + 2; y++)
                {
                    for (int16_t x = bombstruct->x - 6; x < bombstruct->x + 10; x++)
                    {
                        if (x >= 0 && x < DISPLAY_WIDTH
                            && y > CEILING_Y / 4 && y < FLOOR_Y / 4
                            && !obstacle(x, 4 * y))
                        {
                            page(x, y, pgm_read_byte_near(explosion + i));
                        }
                        i++;
                    }
                }
                delay(600);
                i = 0;
                for (int16_t y = bombstruct->y / 4 - 1; y < bombstruct->y / 4 + bombstruct->height / 4 + 2; y++)
                {
                    for (int16_t x = bombstruct->x - 6; x < bombstruct->x + 10; x++)
                    {   
                        if (x >= 0 && x < DISPLAY_WIDTH
                            && y > CEILING_Y / 4 && y < FLOOR_Y / 4
                            && !obstacle(x, 4 * y))
                        {
                            page(x, y, 0);
                        }
                        i++;
                    }
                } 
                for (uint8_t i = 0; i < NUM_MONSTERS; ++i)
                {
                    if (monsters[i]->movement != HIDDEN &&
                        blast_x1 < monsters[i]->x + monsters[i]->width && blast_x2 > monsters[i]->x &&
                        blast_y1 < monsters[i]->y + monsters[i]->height && blast_y2 > monsters[i]->y)
                    {
                        monstertakedamage(i, bombstruct->damage);
                    }
                }
                if (blast_x1 < protagonist->x + protagonist->width && blast_x2 > protagonist->x &&
                    blast_y1 < protagonist->y + protagonist->height && blast_y2 > protagonist->y)
                {
                    takingdamage(bombstruct->damage);
                }
            }
            else
            {
                draw(bombstruct);
            }
        }
        //BOMB END

        if (protagonist->y > DISPLAY_HEIGHT - protagonist->height) //PROTAGONIST fell into water/spikes
        {
            hide(protagonist);
            drawfloor();
            if (monsters[0]->x < protagonist->x)
            {
                while (!obstacle(protagonist->x, FLOOR_Y))
                {
                    protagonist->x++;
                }
            }
            else
            {
                while(!obstacle(protagonist->x + protagonist->width - 1, FLOOR_Y))
                {
                    protagonist->x--;
                }
            }
            protagonist->y = FLOOR_Y - protagonist->height;
            for (uint8_t x = protagonist->x; x < protagonist->x + protagonist->width; x++)
            {
                if (obstacle_hill(x))
                {
                    protagonist->y -= 4;
                    break;
                }
            }
            takingdamage(20);
        }
        
        for (uint8_t i = 0; i < NUM_MONSTERS; ++i)
        {
            if (monsters[i]->movement != HIDDEN && monsters[i]->y > DISPLAY_HEIGHT - monsters[i]->height) //MONSTER fell into water/spikes
            {
                hide(monsters[i]);
                drawfloor();
            }
            //KNOCKBACK after collision
            if (monsters[i]->movement != HIDDEN && collision(protagonist, monsters[i]))
            {
                // if the monster is right of the protagonist
                if (monsters[i]->x + monsters[i]->width/2 >= protagonist->x + protagonist->width)
                {
                    uint8_t j = 0;
                    while (protagonist->x + protagonist->width + DIST_AFTER_DAMAGE > monsters[i]->x)
                    {
                        if (!moveleft(protagonist))
                            break;
                        if (j < 4)
                            moveup(protagonist);
                        else
                            movedown(protagonist);
                        j++;
                        draw(monsters[i]);
                        delay(50);
                    }
                    while (protagonist->x + protagonist->width + DIST_AFTER_DAMAGE > monsters[i]->x)
                    {
                        if (!moveright(monsters[i]))
                            break;
                    }
                }
                else
                {
                    uint8_t j = 0;
                    while (monsters[i]->x + monsters[i]->width + DIST_AFTER_DAMAGE > protagonist->x)
                    {
                        if (!moveright(protagonist))
                            break;
                        if (j < 4)
                            moveup(protagonist);
                        else
                            movedown(protagonist);
                        j++;
                        draw(monsters[i]);
                        delay(50);
                    }
                    while (monsters[i]->x + monsters[i]->width + DIST_AFTER_DAMAGE > protagonist->x)
                    {
                        if (!moveleft(monsters[i]))
                            break;
                    }
                }
                takingdamage(monsters[i]->damage);
            }
        }
        
        for (uint8_t i = 0; i < NUM_MONSTERS; ++i)
        {
            if (xparasites[i]->movement != HIDDEN && collision(protagonist, xparasites[i]))
            {
                hide(xparasites[i]);
                if (xparasites[i]->look == LOOK_XPARASITE1)
                {
                    num_rockets += 2;
                    if (num_rockets > 20)
                        num_rockets = 20;
                    drawnumber(57, 1, num_rockets);
                }
                else
                {
                    num_bombs += 2;
                    if (num_bombs > 20)
                        num_bombs = 20;
                    drawnumber(86, 1, num_bombs);
                }
            }
            else if (xparasites[i]->movement != HIDDEN)
            {
                draw(xparasites[i]);
            }
        }

        if(energytankstruct->movement != HIDDEN)
        {
            draw(energytankstruct);
        }
        if(energytankstruct->movement != HIDDEN && collision(protagonist, energytankstruct))
        {
            hide(energytankstruct);
            if(protagonist->health + 30 > 99)
            {
                protagonist->health = 99;
                drawnumber(29, 1, 99);
            }
            else
            {
                protagonist->health += 30;
                drawnumber(29, 1, protagonist->health);
            }
        }
        
        if (monsters[0]->look == LOOK_BOSS_DRAGON)
        {
            for (uint8_t i = 0; i < NUM_FIREBALLS; ++i)
            {
                if (fireballs[i]->movement == HIDDEN && monsters[0]->movement != HIDDEN && nextfireevent < getMsTimer())
                {
                    fireballs[i]->movement = FIREBALL;
                    fireballs[i]->jumpstate = 1;
                    fireballs[i]->jumpheight = 8 + 4 * really_random_below(4);
                    if (protagonist->x < monsters[0]->x)
                    {
                        fireballs[i]->x = monsters[0]->x - fireballs[i]->width;
                        fireballs[i]->direction = DIRECTION_LEFT;
                    }
                    else
                    {
                        fireballs[i]->x = monsters[0]->x + monsters[0]->width;
                        fireballs[i]->direction = DIRECTION_RIGHT;
                    }
                    fireballs[i]->y = monsters[0]->y + 8;
                    draw(fireballs[i]);
                    nextfireevent = getMsTimer() + (really_random_below(5) == 0 ? 1000 : 400);
                }
            }
        }
        else if (monsters[0]->look == LOOK_NEO_RIDLEY_DRAGON)
        {
            for (uint8_t i = 0; i < 3; ++i)
            {
                if (fireballs[i]->movement == HIDDEN && monsters[0]->movement != HIDDEN && monsters[0]->movement != BOSS_DRAGON_ATTACK && nextfireevent < getMsTimer())
                {
                    uint8_t enough_space = 1;
                    if (monsters[0]->direction == DIRECTION_LEFT)
                    {
                        if (monsters[0]->x < fireballs[i]->width + 4)
                            enough_space = 0;
                        else
                            fireballs[i]->x = monsters[0]->x - fireballs[i]->width;
                    }
                    else
                    {
                        if (monsters[0]->x + monsters[0]->width + fireballs[i]->width + 4 > DISPLAY_WIDTH)
                            enough_space = 0;
                        else
                            fireballs[i]->x = monsters[0]->x + monsters[0]->width;
                    }
                    if (enough_space)
                    {
                        fireballs[i]->y = monsters[0]->y + 8;
                        for (uint8_t x = fireballs[i]->x; x < fireballs[i]->x + fireballs[i]->width; x++)
                        {
                            for (uint8_t y = fireballs[i]->y; y < fireballs[i]->y + fireballs[i]->height; y++)
                            {
                                if(obstacle(x, y))
                                    enough_space = 0;
                            }
                        }
                        if (enough_space)
                        {
                            fireballs[i]->movement = FIREBALL;
                            fireballs[i]->jumpstate = 1;
                            fireballs[i]->jumpheight = 8 + 4 * really_random_below(4);
                            fireballs[i]->direction = monsters[0]->direction;
                            draw(fireballs[i]);
                            nextfireevent = getMsTimer() + (i == 2 ? 3000 : 300);
                        }
                    }
                }
            }
        }
        else if (monsters[0]->look == LOOK_BOSS_SECROB)
        {
            if (really_random_below(1000) == 0 && monsters[0]->jumpstate == ON_THE_GROUND && monsters[0]->x > 6 + fireballs[0]->width && monsters[0]->x < DISPLAY_WIDTH - 6 - monsters[0]->width - fireballs[0]->width)
            {
                if(fireballs[0]->movement == HIDDEN && fireballs[1]->movement == HIDDEN && 
                    fireballs[2]->movement == HIDDEN && fireballs[3]->movement == HIDDEN)
                {
                    fireballs[0]->y = fireballs[2]->y = monsters[0]->y + 4;
                    fireballs[1]->y = fireballs[3]->y = monsters[0]->y + 12;
                    fireballs[0]->x = fireballs[1]->x = monsters[0]->x - fireballs[0]->width;
                    fireballs[2]->x = fireballs[3]->x = monsters[0]->x + monsters[0]->width;
                    fireballs[0]->direction = fireballs[1]->direction = DIRECTION_LEFT;
                    fireballs[2]->direction = fireballs[3]->direction = DIRECTION_RIGHT;
                    draw(fireballs[0]);
                    draw(fireballs[1]);
                    draw(fireballs[2]);
                    draw(fireballs[3]);
                    fireballs[0]->movement = fireballs[1]->movement = fireballs[2]->movement = fireballs[3]->movement = ARROW;
                    nextmonstermoveevent[0] = nextmonsterjumpevent[0] = getMsTimer() + 1500;
                }
            }
            if (monsters[0]->jumpstate == ON_THE_GROUND
                && fireballs[4]->movement == HIDDEN && fireballs[5]->movement == HIDDEN
                && really_random_below(1000) == 0)
            {
                fireballs[4]->y = fireballs[5]->y = monsters[0]->y - fireballs[4]->height;
                fireballs[4]->x = monsters[0]->x + monsters[0]->width / 4;
                fireballs[5]->x = monsters[0]->x + monsters[0]->width / 4 * 3;
                draw(fireballs[4]);
                draw(fireballs[5]);
                fireballs[4]->movement = fireballs[5]->movement = ARROW_UP;
                nextmonstermoveevent[0] = nextmonsterjumpevent[0] = getMsTimer() + 1500;
            }
        }
        for (uint8_t i = 0; i < NUM_FIREBALLS; ++i)
        {
            if (fireballs[i]->movement != HIDDEN && nextfireballmoveevent[i] < getMsTimer())
            {
                move(fireballs[i]);
                nextfireballmoveevent[i] = getMsTimer() + 30;
            }
            if (fireballs[i]->movement != HIDDEN && nextfireballjumpevent[i] < getMsTimer())
            {
                jump(fireballs[i]);
                nextfireballjumpevent[i] = getMsTimer() + 25;
            }
            if (fireballs[i]->movement != HIDDEN && collision(fireballs[i], protagonist))
            {
                hide(fireballs[i]);
                takingdamage(fireballs[i]->damage);
            }
            for (uint8_t j = 0; j < NUM_ROCKETS; ++j)
            {
                if(fireballs[i]->movement != HIDDEN && collision(projectiles[j], fireballs[i]))
                {
                    hide(projectiles[j]);
                    hide(fireballs[i]);
                }
            }
        }
        
        if (rechargeroom
            && protagonist->x + protagonist->width < DISPLAY_WIDTH/2 + 12
            && protagonist->x > DISPLAY_WIDTH/2 - 11
            && nextrechargeevent < getMsTimer())
        {
            if (protagonist->health != 99 ||  num_bombs != 20 || num_rockets != 20)
            {   
                recharging = true;
                drawcolor(DISPLAY_WIDTH/2 - 12, 17, 1, 6, 0b01010101);
                drawcolor(DISPLAY_WIDTH/2 + 11, 17, 1, 6, 0b01010101);
            }
            else 
            {
                recharging = false;
                drawcolor(DISPLAY_WIDTH/2 - 12, 17, 1, 6, 0x00);
                drawcolor(DISPLAY_WIDTH/2 + 11, 17, 1, 6, 0x00);
            }
            
            if (protagonist->health < 99)
            {
                protagonist->health++;
                eeprom_write_byte(&health_stored, protagonist->health);
                drawnumber(29, 1, protagonist->health);
            }
            if (num_rockets < 20)
            {   
                num_rockets++;
                eeprom_write_byte(&num_rockets_stored, num_rockets);
                drawnumber(57, 1, num_rockets);
            }
            if (num_bombs < 20)
            {
                num_bombs++;
                eeprom_write_byte(&num_bombs_stored, num_bombs);
                drawnumber(86, 1, num_bombs);
            }
            nextrechargeevent = getMsTimer() + 100;
        }

        //PAUSE SCREEN
        if(B_PAUSE)
        {
            pauseTimer = 1;
            while (B_PAUSE); // wait until button is released
            clear();
            drawsprite(60, 12, 39, 4, pause);
            while (!B_PAUSE);
            while (B_PAUSE); // wait until button is released
            redraw();
            pauseTimer = 0;
        }
    }
}


void init()
{
	uartInit();   // serielle Ausgabe an PC
	ADCInit(0);   // Analoge Werte einlesen
	timerInit();  // "Systemzeit" initialisieren
	buttonsInit();
	displayInit();
}
