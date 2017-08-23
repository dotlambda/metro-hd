#include <stdlib.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdbool.h>
#include <util/delay.h>
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
uint32_t nextprojectilevent = 0;
uint32_t nextbombevent = 0;
uint32_t explode = 0;
uint32_t nextmonstermoveevent[NUM_MONSTERS];
uint32_t nextmonsterjumpevent[NUM_MONSTERS];
uint32_t nextfireballmoveevent[NUM_FIREBALLS];
uint32_t nextfireballjumpevent[NUM_FIREBALLS];
uint32_t nextfireevent = 0;

void init();

// delay without interrupts
void delay(uint32_t ms)
{
    uint32_t time = getMsTimer();
    while (getMsTimer() < time + ms);
}

void takingdamage(uint8_t damage)
{
    uint32_t blink_for = 650;
    protagonist->health = protagonist->health - damage;
    if(protagonist->health > 0)
    {
        drawnumber(29, 1, protagonist->health);
    }
    else
    {
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
            for (uint8_t y = 1; y < 4; y++)
            {
                for (uint8_t x = 29; x < 36; x++)
                {
                    page(x, y, 0);
                }
            }
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

int main(void)
{
	init();

    // show splash screen until button A is pressed
    uint16_t i = 0;
    for (uint8_t y = 3; y < 3 + 20; y++)
    {
        for (uint8_t x = 15; x < 15 + 126; x++)
        {
            page(x, y, pgm_read_byte_near(splash + i));
            i++;
        }
    }
    while (!B_A);
    
    struct Character protagonist_;
    protagonist = &protagonist_;

    for (uint8_t i = 0; i < NUM_MONSTERS; ++i)
    {
        nextmonstermoveevent[i] = 0;
        nextmonsterjumpevent[i] = 0;
        monsters[i] = &monsters_[i];
    }

    struct Character projectile_;
    projectile = &projectile_;

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
        if (protagonist->jumpstate != ON_THE_GROUND)
        {
            if (nextjumpevent < getMsTimer())
            {
                jump(protagonist);
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
            && protagonist->y >= DOOR_Y - protagonist->height)
        {
            newlevel();
        }
        else if (doors & 0b00000010
            && protagonist->x <= 6 
            && protagonist->y >= DOOR_Y - protagonist->height)
        {
            newlevel();
        }
        
        //PROJECTILE
        if (projectile->movement == HIDDEN
            && num_rockets > 0
            && nextprojectilevent < getMsTimer()
            && B_A)
        {
            uint8_t enough_space = 1;
            projectile->direction = protagonist->direction;
            projectile->y = protagonist->y + 4;
            if (protagonist->direction == DIRECTION_LEFT)
            {
                if (protagonist->x < projectile->width)
                    enough_space = 0;
                projectile->x = protagonist->x - projectile->width;
            }
            else
            {
                if (protagonist->x + protagonist->width + projectile->width >= DISPLAY_WIDTH)
                    enough_space = 0;
                projectile->x = protagonist->x + protagonist->width;
            }
            if (enough_space)
            {
                for (uint8_t x = projectile->x; x < projectile->x + projectile->width; ++x)
                {
                    for (uint8_t y = projectile->y; y < projectile->y + projectile->height; ++y)
                    {
                        if (obstacle(x, y))
                            enough_space = 0;
                    }
                }
            }
            if (enough_space)
            {
                projectile->movement = PROJECTILE;
                draw(projectile);
                num_rockets--;
                drawnumber(57, 1, num_rockets);
                nextprojectilevent = getMsTimer() + 35;
            }
        }
        else if (projectile->movement != HIDDEN
            && nextprojectilevent < getMsTimer())
        {
            move(projectile);
            if (projectile->movement == HIDDEN)
                nextprojectilevent = getMsTimer() + 500;
            else
                nextprojectilevent = getMsTimer() + 35;
        }

        for (uint8_t i = 0; i < NUM_MONSTERS; ++i)
        {
            if (projectile->movement != HIDDEN && monsters[i]->movement != HIDDEN && collision(projectile, monsters[i]))
            {
                hide(projectile);
                monsters[i]->health -= projectile->damage;
                if (monsters[i]->health <= 0)
                {
                    hide(monsters[i]);
                    newxparasite(i); 
                }
                else
                {
                    draw(monsters[i]);
                }
                nextprojectilevent = getMsTimer() + 500;
            }
        }
        //PROJECTILE END

        //BOMB
        if (nextbombevent < getMsTimer())
        {
            if (bombstruct->movement != HIDDEN)
            {
                jump(bombstruct);
                if (bombstruct->y > FLOOR_Y - bombstruct->height)
                {
                    hide(bombstruct);
                    drawfloor();
                }
                nextbombevent = getMsTimer() + 100;
            }
            else if (B_DOWN && num_bombs > 0)
            {
                bombstruct->x = protagonist->x;
                bombstruct->y = protagonist->y + protagonist->height - bombstruct->height;
                bombstruct->movement = BOMB;
                num_bombs--;
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
                        monsters[i]->health -= bombstruct->damage;
                        if (monsters[i]->health <= 0)
                        {
                            hide(monsters[i]);
                            newxparasite(i);
                        }
                        else
                        {
                            draw(monsters[i]);
                        }
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
                    protagonist->y--;
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
                if (fireballs[i]->movement != HIDDEN && collision(fireballs[i], protagonist))
                {
                    hide(fireballs[i]);
                    takingdamage(fireballs[i]->damage);
                }
                if(collision(projectile, fireballs[i]) && fireballs[i]->movement != HIDDEN)
                {
                    hide(projectile);
                    hide(fireballs[i]);
                }
            }
        }

        //PAUSE SCREEN
        if(B_PAUSE)
        {
            while (B_PAUSE); // wait until button is released
            clear();
            uint16_t i = 0;
            for (uint8_t y = 12; y < 16 ; y++)
            {
                for (uint8_t x = 60; x < 99; x++)
                {
                    page(x, y, pgm_read_byte_near(pause + i));
                    i++;
                }
            }
            while (!B_PAUSE);
            while (B_PAUSE); // wait until button is released
            redraw();
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
