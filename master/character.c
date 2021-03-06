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
            character->health = 2;
            character->movement = BACK_AND_FORTH;
            break;
        case LOOK_EYEMONSTER:
            character->width = 12;
            character->height = 16;
            character->damage = 15;
            character->health = 4;
            break;
        case LOOK_MONSTER_ZOOMER:
            character->width = 15;
            character->height = 12;
            character->damage = 20;
            character->health = 4;
            break;
        case LOOK_ROCKET:
            character->width = 14;
            character->height = 8;
            character->damage = 1;
            character->movement = HIDDEN;
            break;
        case LOOK_BOSS_ZAZABI:
            character->width = 17;
            character->height = 32;
            character->damage = 10;
            character->health = 10;
            character->movement = ZAZABI;
            break;
        case LOOK_MONSTER_METROID:
            character->width = 14;
            character->height = 12;
            character->damage = 15;
            character->health = 3;
            character->movement = BACK_AND_FORTH;
            break;
        case LOOK_MONSTER_HORNOAD:
            character->width = 14;
            character->height = 12;
            character->damage = 15;
            character->jumpheight = 20;
            character->health = 3;
            character->movement = JUMPMOVE;
            break;
        case LOOK_MONSTER_SIDEHOPPER:
            character->width = 16;
            character->height = 12;
            character->damage = 20;
            character->health = 3;
            character->jumpheight = 40;
            character->movement = JUMPMOVE;
            break;
        case LOOK_MONSTER_MEMU:
            character->width = 15;
            character->height = 8;
            character->damage = 10;
            character->movement = FLYING_AROUND;
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
            character->health = 5;
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
            character->health = 20;
            character->damage = 10;
            character->y_pace = 10;
            character->health = 20;
            break;
        case LOOK_FIREBALL:
            character->width = 8;
            character->height = 8;
            character->damage = 10;
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
            character->health = 15;
            character->movement = BOSS_DRAGON_GROUND;
            break;

        case LOOK_BIGXPARASITE:
            character->width = 15;
            character->height = 16;
            character->damage = 5;
            character->health = 1;
            character->movement = FLYING_AROUND;
            break;
        case LOOK_HIDDEN:
            character->movement = HIDDEN;
            break;
        case LOOK_BOSS_MEGACOREX:
            character->width = 44;
            character->height = 44;
            character->damage = 5;
            character->health = 15;
            character->movement = FLYING_AROUND;
            character->x_pace = 70;
            character->y_pace = 35;
            break;
    }
    character->initial_health = character->health;
    character->lookstate = 0;
    character->lastlookstatechg = getMsTimer();
    character->jumpstate = ON_THE_GROUND;
}

void draw(struct Character* character)
{
    const uint8_t* sprite = NULL;
    uint8_t inverted = 0;
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
                inverted = 1;
            
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
            break;

        case LOOK_MONSTER_LITTLE:
            sprite = little;
            break;
            
        case LOOK_EYEMONSTER:
            if(character->direction == DIRECTION_RIGHT)
                inverted = 1;
            sprite = eyemonster_left;
            break;
        
        case LOOK_MONSTER_ZOOMER:
            sprite = zoomer;
            break;
            
        case LOOK_ROCKET:
            if(character->direction == DIRECTION_RIGHT)
                inverted = 1;
            sprite = rocket;
            break;
            
        case LOOK_BOSS_ZAZABI:
            sprite = zazabi;
            break;

        case LOOK_MONSTER_METROID:
            sprite = metroid;
            break;

        case LOOK_MONSTER_HORNOAD:
            if (character->direction == DIRECTION_RIGHT)
                inverted = 1;
            sprite = hornoadleft;
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
        case LOOK_NEO_RIDLEY_DRAGON:
            if (character->direction == DIRECTION_RIGHT)
	            inverted = 1;

	        if(character->movement == BOSS_DRAGON_GROUND)
	        {
	            sprite = dragon2_left;
	        }
            else
            {
                if(character->lookstate)
                {
                    sprite = dragon2_left;
                }
                else
                {
                    sprite = dragon2_flying_left;
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
            if(character->direction == DIRECTION_LEFT)
                inverted = 1;

            if (character->lookstate) // wings up and moving left
            {
                sprite = geega1;
            }
            else// wings down
            {
                sprite = geega2;
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
            break;
        case LOOK_HIDDEN:
            sprite = NULL;
            break;
        case LOOK_BOSS_MEGACOREX:
            switch (character->lookstate)
            {
                case 0:
                    sprite = megacorex1;
                    break;
                case 1:
                    sprite = megacorex2;
                    break;
                case 2:
                    sprite = megacorex3;
                    break;
            }
            if (character->lastlookstatechg < getMsTimer())
            {
                character->lookstate++;
                if (character->lookstate == 3)
                    character->lookstate = 0;
                character->lastlookstatechg = getMsTimer() + 100;
            }
            break;
    }
    
    if (inverted)
        drawsprite_px_rotated(character->x, character->y, character->width, character->height, sprite);
    else
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

    drawcolor(character->x, (character->y + character->height - 1) / 4, character->width, 1, 0x00);
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

    drawcolor(character->x, character->y / 4, character->width, 1, 0x00);
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
            if (protagonist->x < character->x
                && obstacle(character->x - 1, FLOOR_Y)) // if it wouldn't fall into water
            {
                moveleft(character);
            }
            else if (protagonist->x > character->x
                && obstacle(character->x + character->width, FLOOR_Y)) // if it wouldn't fall into water
            {
                moveright(character);
            }
            else
            {
                draw(character);
            }
            break;
        case BACK_AND_FORTH:
            if (character->direction == DIRECTION_LEFT)
            {
                if (!obstacle(character->x - 1, FLOOR_Y) || !moveleft(character))
                    character->direction = DIRECTION_RIGHT;
            }
            else
            {
                if (!obstacle(character->x + character->width, FLOOR_Y) || !moveright(character))
                    character->direction = DIRECTION_LEFT;
            }
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
                && really_random_below(3) == 0) // 1/3 probability to attack
            {
                // move down towards the middle of the screen,
                // then move up again while continuing in the previous horizontal direction
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
            if (character->jumpstate == ON_THE_GROUND && really_random_below(10) == 0)
            {
                character->jumpheight = 30 + really_random_below(10);
                character->x_pace = 10;
                character->jumpstate = 1;
            }
            break;

        case BOSS_DRAGON_GROUND:
            if(character->health > character->initial_health/2)
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
        case ZAZABI:
            if (character->jumpstate == ON_THE_GROUND)
            {
                if (really_random_below(10) == 0)
                {
                    // begin jumping in the opposite direction
                    character->direction = 1 - character->direction;
                    character->jumpstate = 1;
                    character->y_pace = 20;
                    character->jumpheight = 30 + really_random_below(20);
                    if (character->health < 10)
                    {
                        character->x_pace = 10 + really_random_below(15);
                    }
                    else
                    {
                        character->x_pace = 20 + really_random_below(15);
                    }
                }
            }
            else
            {
                if (character->y_pace == 40) // Zazabi is moving down in a straight line
                {
                    break;
                }
                else if (character->x + character->width / 2 == protagonist->x + protagonist->width / 2) // if Zazabi is right above the protagonist
                {
                    // fall down slowly
                    character->y_pace = 40;
                    character->jumpstate = character->jumpheight;
                }
                else
                {
                    if (character->direction == DIRECTION_LEFT)
                        moveleft(character);
                    else
                        moveright(character);
                }

            }
            break;
    }
}
