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
        uint16_t i = 0;
        for (uint8_t y = 0; y < DISPLAY_HEIGHT; y++)
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

int main(void)
{
	init();

    // show splash screen until button A is pressed
    uint16_t i = 0;
    for (uint8_t y = 3; y < 3 + 20; y++)
    {
        for (uint8_t x = 22; x < 22 + 115; x++)
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

    struct Character xparasite1_;
    xparasite1struct = &xparasite1_;

    struct Character xparasite2_;
    xparasite2struct = &xparasite2_;
    
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
                nextmonstermoveevent[i] = getMsTimer() + 100;
            }
            if(monsters[i]->movement != HIDDEN && nextmonsterjumpevent[i] < getMsTimer())
            {
                jump(monsters[i]);
                nextmonsterjumpevent[i] = getMsTimer() + 150;
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
                nextjumpevent = getMsTimer() + 150;
            }
        }
        else if (B_UP)
        {
            protagonist->jumpstate = 1;
        }
        
        // change level when protagonist touches the door
        if (doors & 0b00000001
            && protagonist->x >= DISPLAY_WIDTH - 6 - protagonist->width 
            && protagonist->y >= 20 - protagonist->height)
        {
            newlevel();
        }
        else if (doors & 0b00000010
            && protagonist->x <= 6 
            && protagonist->y >= 20 - protagonist->height)
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
            projectile->y = protagonist->y + 1;
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
                    hide(monsters[i]);
                else
                    draw(monsters[i]);
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
                if (bombstruct->y > 25 - bombstruct->height)
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
                uint8_t blast_y1 = MAX(6, bombstruct->y - 1);
                uint8_t blast_y2 = MIN(bombstruct->y + bombstruct->height + 3, 25);
                uint16_t i = 0;
                for (int16_t y = bombstruct->y - 1; y < bombstruct->y + 3; y++)
                {
                    for (int16_t x = bombstruct->x - 6; x < bombstruct->x + 10; x++)
                    {
                        if (x >= 0 && x < DISPLAY_WIDTH
                            && y >= 6 && y < 25
                            && !obstacle(x, y))
                        {
                            page(x, y, pgm_read_byte_near(explosion + i));
                        }
                        i++;
                    }
                }
                delay(600);
                i = 0;
                for (int16_t y = bombstruct->y - 1; y < bombstruct->y + 3; y++)
                {
                    for (int16_t x = bombstruct->x - 6; x < bombstruct->x + 10; x++)
                    {   
                        if (x >= 0 && x < DISPLAY_WIDTH
                            && y >= 6 && y < 25
                            && !obstacle(x, y))
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
                            hide(monsters[i]);
                        else
                            draw(monsters[i]);
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
                while (!obstacle(protagonist->x, 25))
                {
                    protagonist->x++;
                }
            }
            else
            {
                while(!obstacle(protagonist->x + protagonist->width - 1, 25))
                {
                    protagonist->x--;
                }
            }
            protagonist->y = 25 - protagonist->height;
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
                        if (j == 0 || j == 2)
                            moveup(protagonist);
                        if (j > 3 && j % 2 == 0)
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
                        if (j == 0 || j == 2)
                            moveup(protagonist);
                        if (j > 3 && j % 2 == 0)
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
        for(int i = 0; i <= NUM_MONSTERS; i++)
        {
            if(monsters[i]->movement == HIDDEN)
            {
                switch (random_below(3))
                {
                    case 0:
                        xparasite1struct->x = monsters[i]->x + monster[i]->width / 2;
                        xparasite1struct->y = monsters[i]->y + monster[i]->height / 2;
                        draw(xparasite1struct);
                        break;
                    case 1:
                        xparasite2struct->x = monsters[i]->x + monster[i]->width / 2;
                        xparasite2struct->y = monsters[i]->y + monster[i]->height / 2;
                        draw(xparasite2struct);
                        break;
                    case 2:
                        break;
                }
            }
        }
        if (collision(protagonist, xparasite1struct) && xparasite1struct->movement != HIDDEN)
        {
            xparasite1struct->movement = HIDDEN;
            drawnumber(57, 1, num_rockets + 2);
        }
        if (collision(protagonist, xparasite2struct) && xparasite2struct->movement != HIDDEN)
        {
            xparasite2struct->movement = HIDDEN;
            drawnumber(86, 1, num_bombs + 2);
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
