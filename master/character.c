#include <stdlib.h>
#include <avr/pgmspace.h>
#include "timer.h"
#include "character.h"
#include "level.h"
#include "display.h"
#include "sprites.h"
#include "rand.h"
#include "drawing.h"

void initcharacter(struct Character* character)
{
    character->health = 4;
    character->movement = FOLLOW_PROTAGONIST;
    character->jumpheight = 28;
    character->x_pace = 100;
    character->y_pace = 40;
    switch (character->look)
    {
        case LOOK_PROTAGONIST:
            character->width = 8;
            character->height = 16;
            character->health = 99;
            break;
        case LOOK_MONSTER_LITTLE: 
            character->width = 8;
            character->height = 8;
            character->damage = 10;
            break;
        case LOOK_EYEMONSTER:
            character->width = 12;
            character->height = 16;
            character->damage = 15;
            break;
        case LOOK_MONSTER_ZOOMER:
            character->width = 15;
            character->height = 12;
            character->damage = 20;
            break;
        case LOOK_ROCKET:
            character->width = 14;
            character->height = 8;
            character->damage = 2;
            character->movement = HIDDEN;
            break;
        case LOOK_BOSS_ZAZABI:
            character->width = 17;
            character->height = 32;
            character->damage = 30;
            break;
        case LOOK_MONSTER_METROID:
            character->width = 14;
            character->height = 12;
            character->damage = 15;
            break;
        case LOOK_MONSTER_HORNOAD:
            character->width = 14;
            character->height = 12;
            character->damage = 15;
            character->jumpheight = 20;
            character->movement = JUMPMOVE;
            break;
        case LOOK_MONSTER_SIDEHOPPER:
            character->width = 16;
            character->height = 12;
            character->damage = 20;
            character->jumpheight = 40;
            character->movement = JUMPMOVE;
            break;
        case LOOK_MONSTER_MEMU:
            character->width = 15;
            character->height = 8;
            character->damage = 10;
            character->movement = FLYING_AROUND;
            break;
        case LOOK_BOSS_DRAGON:
            character->width = 30;
            character->height = 36;
            character->damage = 15;
            character->health = 4;
            character->movement = BOSS_DRAGON_GROUND;
            break;
        case LOOK_BOMB:
            character->width = 4;
            character->height = 4;
            character->damage = 5;
            character->movement = HIDDEN;
            break;
        case LOOK_MONSTER_GEEGA:
            character->width = 16;
            character->height = 16;
            character->damage = 5;
            character->movement = FLYING_AROUND;
            break;
        case LOOK_XPARASITE1:
            character->width = 6;
            character->height = 8;
            character->movement = XPARASITE;
            break;
        case LOOK_XPARASITE2:
            character->width = 6;
            character->height = 8;
            character->movement = XPARASITE;
            break;
        case LOOK_ENERGYTANK:
            character->width = 9;
            character->height = 8;
            character->movement = ENERGYTANK;
            break;
        case LOOK_BOSS_SECROB:
            character->width = 37;
            character->height = 24;
            character->movement = SECROB;
            character->jumpheight = 36;
            character->health = 10;
            character->damage = 10;
            character->y_pace = 10;
            break;
        case LOOK_FIREBALL:
            character->width = 8;
            character->height = 8;
            character->damage = 20;
            character->movement = HIDDEN;
            break;
        case LOOK_ARROW:
            character->width = 6;
            character->height = 4;
            character->damage = 10;
            character->movement = HIDDEN;
            break;
        case LOOK_ARROW_UP:
            character->width = 3;
            character->height = 4;
            character->damage = 10;
            character->movement = HIDDEN;
            break;
	case LOOK_NEO_RIDLEY_DRAGON:
	    character->width = 28;
	    character->height = 33;
	    character->damage = 15;
	    character->health = 4;
	    character->movement = BOSS_DRAGON_GROUND;
	    break;

        case LOOK_BIGXPARASITE:
            character->width = 15;
            character->height = 16;
            character->damage = 5;
            character->health = 4;
            character->movement = FLYING_AROUND;
            break;
    }
    character->lookstate = 0;
    character->lastlookstatechg = getMsTimer();
    character->jumpstate = ON_THE_GROUND;
}

void draw(struct Character* character)
{
    const uint8_t* sprite = NULL;
    switch (character->look)
    {
        case LOOK_PROTAGONIST:
            if (character->lastlookstatechg + 300 < getMsTimer())
            {
                character->lookstate = 1 - character->lookstate;
                character->lastlookstatechg = getMsTimer();
            }
            
            character->height = 16;
            if (character->direction == DIRECTION_LEFT)
            {
                if (character->lookstate)
                {
                    if (character->jumpstate == CLIMBING && character->y == CEILING_Y + 4)
                    {
                        character->height = 20;
                        sprite = protagonistleftclimb;
                    }
                    else
                    {    
                        sprite = protagonistleft;
                    }
                }
                else 
                {
                    if (character->jumpstate == CLIMBING && character->y == CEILING_Y + 4)
                    {
                        character->height = 20;
                        sprite = protagonistleftclimbtwo;
                    }
                    else
                    {
                        sprite = protagleftwalk;
                    }
                }
            }
            else
            {
                if (character->lookstate)
                {
                    if (character->jumpstate == CLIMBING && character->y == CEILING_Y + 4)
                    {
                        character->height = 20;
                        sprite = protagonistrightclimb;
                    }
                    else
                    {
                        sprite = protagonistright;
                    }
                }   
                else 
                {
                    if (character->jumpstate == CLIMBING && character->y == CEILING_Y + 4)
                    {
                        character->height = 20;
                        sprite = protagonistrightclimbtwo;
                    }
                    else
                    {
                        sprite = protagrightwalk;
                    }
                }
            }
            break;

        case LOOK_MONSTER_LITTLE:
            sprite = little;
            break;
            
        case LOOK_EYEMONSTER:
            if(character->direction == DIRECTION_LEFT)
            {
                sprite = eyemonster_left;
            }
            else
            {
                sprite = eyemonster_right;
            }
            break;
        
        case LOOK_MONSTER_ZOOMER:
            sprite = zoomer;
            break;
            
        case LOOK_ROCKET:
            if(character->direction == DIRECTION_LEFT)
            {
                sprite = rocket;
            }
            else
            {
                sprite = rocketinverted;
            }
            break;
            
        case LOOK_BOSS_ZAZABI:
            sprite = zazabi;
            break;

        case LOOK_MONSTER_METROID:
            sprite = metroid;
            break;
        

        case LOOK_MONSTER_HORNOAD:
            if (character->direction == DIRECTION_LEFT)
            {
                sprite = hornoadleft;
            }
            else
            {
                sprite = hornoadright;
            }
            break;

        case LOOK_MONSTER_SIDEHOPPER:
            sprite = sidehopper;
            break;

        case LOOK_MONSTER_MEMU:
            if (character->lookstate) // wings down
            {
                sprite = memu0;
            }
            else // wings up
            {
                sprite = memu1;
            }
            // toggle wing state
            character->lookstate = 1 - character->lookstate;
            break;

        case LOOK_BOSS_DRAGON:
            if (character->direction == DIRECTION_LEFT)
            {
                sprite = dragon_left;
            }
            else
            {
                sprite = dragon_right;
            }
	    break;
        case LOOK_NEO_RIDLEY_DRAGON:
	        if(character->movement == BOSS_DRAGON_GROUND)
	        {
	            if (character->direction == DIRECTION_LEFT)
	            {
	                sprite = dragon2_left;
	            }
	            else
	            {
	                sprite = dragon2_right;
	            }
	        }
            else
            {
                if (character->direction == DIRECTION_LEFT)
                {
                    if(character->lookstate)
                    {
                        sprite = dragon2_left;
                    }
                    else
                    {
                        sprite = dragon2_flying_left;
                    }
                }
                else
                {
                    if(!character->lookstate)
                    {
                        sprite = dragon2_right;
                    }
                    else
                    {
                        sprite = dragon2_flying_right;
                    }
                }
                if (character->lastlookstatechg < getMsTimer())
                {
                    character->lookstate = 1 - character->lookstate;
                    character->lastlookstatechg = getMsTimer() + 300;
                }
            }
            break;
        case LOOK_BOMB:
            sprite = bomb;
            break;
        case LOOK_MONSTER_GEEGA:
            if(character->direction == DIRECTION_RIGHT)
            {
                if (character->lookstate) // wings up and moving left
                {
                    sprite = geega1;
                }
                else// wings down
                {
                    sprite = geega2;
                }
            }
            else
            {
                if (character->lookstate) // wings up and moving right
                {
                    sprite = geega1inverted;
                }
                else// wings down
                {
                    sprite = geega2inverted;
                }
            }

            // toggle wing state
            if (character->lastlookstatechg < getMsTimer())
            {
                character->lookstate = 1 - character->lookstate;
                character->lastlookstatechg = getMsTimer() + 300;
            }
            break;
        case LOOK_ENERGYTANK:
            sprite = energytank;
            break;
        case LOOK_XPARASITE1:
            sprite = xparasite1;
            break;
        case LOOK_XPARASITE2:
            sprite = xparasite2;
            break;
        case LOOK_BOSS_SECROB:
            if (character->jumpstate == ON_THE_GROUND)
            {    
                if (character->lookstate)  
                {
                    sprite = securityrobup;
                }
                else
                {
                    sprite = securityrobdown;
                }
            }
            else
            {
                sprite = securityrobup;
            }
            if (character->lastlookstatechg < getMsTimer())
            {
                character->lookstate = 1 - character->lookstate;
                character->lastlookstatechg = getMsTimer() + 300;
            }
            break;
        case LOOK_FIREBALL:
            sprite = fireball2;
            break;
        case LOOK_ARROW:
            if (character->direction == DIRECTION_LEFT)
                sprite = secrobmunitionleft;
            else
                sprite = secrobmunitionright;
            break;
        case LOOK_ARROW_UP:
            sprite = secrobmunitionup;
            break;
        case LOOK_BIGXPARASITE:
            sprite = bigxparasite;
    }
    
    drawsprite_px(character->x, character->y, character->width, character->height, sprite);
}

void hide(struct Character* character)
{
    character->movement = HIDDEN;
    for (int16_t x = character->x; x < character->x + character->width; ++x)
    {
        for (int16_t y = character->y / 4; y <= (character->y + character->height - 1) / 4; ++y)
        {
            if (x >= 0 && y >= 0 && x < DISPLAY_WIDTH)
                page(x, y, 0x00);
        }
    }
}

uint8_t moveleft(struct Character* character)
{
    if (character->x == 0)
        return 0;

    // don't move if there is an obstacle to the left
    for (uint8_t y = character->y; y < character->y + character->height; ++y)
    {
        if (obstacle(character->x - 1, y))
        {
            if (character->y + character->height == FLOOR_Y && obstacle_hill(character->x - 1))
            {
                // move up by 1 page
                moveup(character);
                moveup(character);
                moveup(character);
                moveup(character);
            }
            else
            {
                draw(character);
                return 0;
            }
        }
    }

    for (uint8_t y = character->y / 4; y <= (character->y + character->height - 1) / 4; ++y)
        page(character->x + character->width - 1, y, 0x00);
    character->x--;
    draw(character);
    checkfalling(character);
    character->direction = DIRECTION_LEFT;

    return 1;
}

uint8_t moveright(struct Character* character)
{
    if (character->x + character->width == DISPLAY_WIDTH)
        return 0;

    // don't move if there is an obstacle to the right
    for (uint8_t y = character->y; y < character->y + character->height; ++y)
    {
        if (obstacle(character->x + character->width, y))
        {
            if (character->y + character->height == FLOOR_Y && obstacle_hill(character->x + character->width))
            {
                // move up by 1 page
                moveup(character);
                moveup(character);
                moveup(character);
                moveup(character);
            }
            else
            {
                draw(character);
                return 0;
            }
        }
    }

    for (uint8_t y = character->y / 4; y <= (character->y + character->height - 1) / 4; ++y)
        page(character->x, y, 0x00);
    character->x++;
    draw(character);
    checkfalling(character);
    character->direction = DIRECTION_RIGHT;
    
    return 1;
}

uint8_t moveup(struct Character* character)
{
    // don't move if there is an obstacle above
    for (uint8_t x = character->x; x < character->x + character->width; ++x)
    {
        if (obstacle(x, character->y - 1))
            return 0;
    }

    for (uint8_t x = character->x; x < character->x + character->width; ++x)
        page(x, (character->y + character->height - 1) / 4, 0x00);
    --character->y;
    draw(character);
    character->verticaldirection = DIRECTION_UP;

    return 1;
}

uint8_t movedown(struct Character* character)
{
    // don't move if there is an obstacle below
    for (uint8_t x = character->x; x < character->x + character->width; ++x)
    {
        if (obstacle(x, character->y + character->height))
            return 0;
    }

    for (uint8_t x = character->x; x < character->x + character->width; ++x)
        page(x, character->y / 4, 0x00);
    ++character->y;
    draw(character);
    character->verticaldirection = DIRECTION_DOWN;

    return 1;
}

void checkfalling(struct Character* character)
{
    if (character->jumpstate == ON_THE_GROUND)
    {
        long feet_on_ground = 0l;
        for (uint8_t x = character->x; x < character->x + character->width; ++x)
            feet_on_ground |= obstacle(x, character->y + character->height);
        if (!feet_on_ground)
            character->jumpstate = character->jumpheight;
    }
}

void jump(struct Character* character)
{
    if (character->movement == ARROW)
    {
        return;
    }
    else if (character->movement == ARROW_UP)
    {
        if (!moveup(character))
            hide(character);
    }
    else if (character->movement == FLYING_AROUND || character->movement == BOSS_DRAGON_AIR)
    {
        if (character->verticaldirection == DIRECTION_UP)
        {
            // don't touch the ceiling
            // switch direction randomly or if the character can't continue in its current direction
            if (character->y <= CEILING_Y + 8 || !moveup(character) || !really_random_below(20))
                character->verticaldirection = really_random_below(2);
        }
        else
        {
            // don't touch the ground
            // switch direction randomly or if the character can't continue in its current direction
            if (character->y >= FLOOR_Y - character->height - 4 || !movedown(character) || !really_random_below(20))
                character->verticaldirection = really_random_below(2);
        }
    }
    else if (character->jumpstate == character->jumpheight)
    {
        if (!movedown(character))
        {
            character->jumpstate = ON_THE_GROUND;
            if (character->movement == FIREBALL)
                hide(character);
            else if (character->movement == SECROB)
            {
                character->x_pace = 100;
            }
        }
    }
    else if (character->jumpstate != ON_THE_GROUND)
    {
        if (!moveup(character))
            character->jumpstate = character->jumpheight;
        else
            character->jumpstate++;
    }
}

// determine the direction in which a monster or projectile should move
void move(struct Character* character)
{
    switch (character->movement)
    {
        case FOLLOW_PROTAGONIST:
            if (protagonist->x < character->x && obstacle(character->x - 1, FLOOR_Y))
                moveleft(character);
            else if (protagonist->x > character->x && obstacle(character->x + character->width, FLOOR_Y))
                moveright(character);
            else
                draw(character);
            break;
        case BACK_AND_FORTH:
            if (character->x <= 0)
                character->direction = DIRECTION_RIGHT;
            else if (character->x + character->width >= DISPLAY_WIDTH)
                character->direction = DIRECTION_LEFT;
            if (character->direction == DIRECTION_LEFT)
                moveleft(character);
            else
                moveright(character);
            break;
        case PROJECTILE:
        case FIREBALL:
        case ARROW:
            if (character->direction == DIRECTION_LEFT)
            {
                if (!moveleft(character))
                    hide(character);
            }
            else
            {
                if (!moveright(character))
                    hide(character);
            }
            break;
        case HIDDEN:
            break;
        case JUMP:
            if(character->jumpstate == ON_THE_GROUND)
            {    
                character->jumpstate = 1;
            }
            break;
        case JUMPMOVE:
            if(character->jumpstate == ON_THE_GROUND)
            {
                character->jumpstate = 1;
            }
            if (protagonist->x < character->x)
                    moveleft(character);
                else if (protagonist->x > character->x)
                    moveright(character);
                else
                    draw(character);
            break;
        case BOMB:
            break;

        case BOSS_DRAGON_AIR:
            if (character->y < CEILING_Y + 16
                && (character->x < 20 || character->x > DISPLAY_WIDTH - character->width - 20)
                && really_random_below(5) == 0) // 1/5 probability to attack
            {
                character->movement = BOSS_DRAGON_ATTACK;
                if (character->x > DISPLAY_WIDTH / 2)
                    character->direction = DIRECTION_LEFT;
                else
                    character->direction = DIRECTION_RIGHT;
                // begin flying down
                character->jumpstate = character->jumpheight = FLOOR_Y - character->y;
                character->x_pace = 30;
                character->y_pace = 10;
                break;
            }
            // else fly around
        case FLYING_AROUND:
            if (character->direction == DIRECTION_LEFT)
            {
                if (!moveleft(character) || !really_random_below(20))
                    character->direction = really_random_below(2);
            }
            else
            {
                if (!moveright(character) || !really_random_below(20))
                    character->direction = really_random_below(2);
            }
            break;
        case ENERGYTANK:
            break;
        case XPARASITE:
            break;

        case SECROB:
//             if (character->x == (DISPLAY_WIDTH - character->width) / 2)
//             {
//                 character->direction = 1 - character->direction;
//             }
            if (character->x >= DISPLAY_WIDTH - character->width - 8)
            {
                character->direction = DIRECTION_LEFT;
            }
            else if (character->x == 8)
            {
                character->direction = DIRECTION_RIGHT;
            }
            if  (character->direction == DIRECTION_LEFT)
            {
                moveleft(character);
            }
            else
            {
                moveright(character);
            }
            if (character->jumpstate == ON_THE_GROUND && really_random_below(20) == 0)
            {
                character->x_pace = 5;
                character->jumpstate = 1;
            }
            break;

        case BOSS_DRAGON_GROUND:
            if(character->health > 2)
            {
                if(protagonist->x <= DISPLAY_WIDTH / 2)
                {
                    moveleft(character);
                }
                else
                {
                    moveright(character);
                }
            }
            else
            {
                while(character->y > CEILING_Y + 16)
                {
                    moveup(character);
                }
                character->movement = BOSS_DRAGON_AIR;
            }
            break;

        case BOSS_DRAGON_ATTACK:
            if (character->direction == DIRECTION_LEFT)
            {
                // if we're on the left and have reached the highest point
                if (character->x <= DISPLAY_WIDTH / 2
                    && character->jumpstate == character->jumpheight)
                {
                    character->movement = BOSS_DRAGON_AIR;
                    character->x_pace = 100;
                    character->y_pace = 40;
                }
                else
                {
                    moveleft(character);
                    if (character->x < 30)
                        character->jumpstate = 1;
                }
            }
            else
            {
                // if we're on the right and have reached the highest point
                if (character->x > DISPLAY_WIDTH / 2
                    && character->jumpstate == character->jumpheight)
                {
                    character->movement = BOSS_DRAGON_AIR;
                    character->x_pace = 100;
                    character->y_pace = 40;
                }
                else
                {
                    moveright(character);
                    if (character->x > DISPLAY_WIDTH - character->width - 30)
                        character->jumpstate = 1;
                }
            }
            break;

        case ARROW_UP:
            break;
    }
}
