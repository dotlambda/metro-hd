#include <stdlib.h>
#include "drawing.h"
#include "display.h"
#include "sprites.h"
#include "level.h"

void drawsplash()
{
    uint16_t i = 0;
    for (uint8_t y = 3; y < 3 + 19; y++)
    {
        for (uint8_t x = 15; x < 15 + 126; x++)
        {
            page(x, y, pgm_read_byte_near(splash + i));
            i++;
        }
    }
}

void drawdigit(uint8_t x, uint8_t y, uint8_t digit)
{
    const uint8_t* sprite = NULL;
    switch (digit)
    {
        case 0:
            sprite = zero;
            break;
        case 1:
            sprite = one;
            break;
        case 2:
            sprite = two;
            break;
        case 3:
            sprite = three;
            break;
        case 4: 
            sprite = four;
            break;
        case 5: 
            sprite = five;
            break;
        case 6:
            sprite = six;
            break;
        case 7:
            sprite = seven;
            break;
        case 8:
            sprite = eight;
            break;
        case 9:
            sprite = nine;
            break;
    }
    uint8_t i = 0;
    for (uint8_t y_ = y; y_ < y + 3; y_++)
    {
        for (uint8_t x_ = x; x_ < x + 3; x_++)
        {
            page(x_, y_, pgm_read_byte_near(sprite + i));
            i++;
        }
    }
}

void drawnumber(uint8_t x, uint8_t y, uint8_t number)
{
    uint8_t leftdigit = number / 10;
    uint8_t rightdigit = number % 10;
    drawdigit(x, y, leftdigit);
    drawdigit(x + 4, y, rightdigit);
}

void drawlabels()
{
    // print energy at the top
    uint8_t i = 0;
    for (uint8_t y = 1; y < 4; y++)
    {
        for (uint8_t x = 2; x < 25; x++)
        {
            page(x, y, pgm_read_byte_near(labelenergy + i));
            i++;
        }
    }
    
    // print rocket label
    i = 0;
    for (uint8_t y = 1; y < 4; y++)
    {
        for (uint8_t x = 40; x < 55; x++)
        {
            page(x, y, pgm_read_byte_near(labelrocket + i));
            i++;
        }
    }
    
    // print bomb label
    i = 0;
    for (uint8_t y = 1; y < 4; y++)
    {
        for (uint8_t x = 69; x < 83; x++)
        {
            page(x, y, pgm_read_byte_near(labelbomb + i));
            i++;
        }
    }
    
    drawnumber(29, 1, protagonist->health);
    drawnumber(57, 1, num_rockets);
    drawnumber(86, 1, num_bombs);
}

void drawdoor(int x)
{
    uint8_t i = 0;
    for (uint8_t y = 20; y < 25; y++)
    {
        for (int x_ = x; x_ < x + 33; x_++)
        {
            if (x_ < DISPLAY_WIDTH && x_ >= 0)
            {
                page(x_, y, pgm_read_byte_near(doorinverted + i));
            }
            i++;
        }
    }
}

void black()
{
    sendbyte(0, 0);
    sendbyte(16, 0);
    sendbyte(96, 0);
    for (uint16_t i = 0; i < 5 * DISPLAY_WIDTH; i++)
    {
        sendbyte(0, 1);
    }
    for (uint16_t i = 5 * DISPLAY_WIDTH; i < DISPLAY_WIDTH * DISPLAY_HEIGHT / 4; i++)
    {
        sendbyte(0xFF, 1);
    }
    sendbyte(0, 0);
    sendbyte(16, 0);
    sendbyte(96, 0);
}

void movedoorleft()
{
    black();
    drawlabels();
    for (int x = DISPLAY_WIDTH - 6; x >= -33 + 6; x--)
    {
        drawdoor(x);
        if (x + 1 < DISPLAY_WIDTH)
        {
            page(x + 33, 20, 0xFF);
            page(x + 33, 21, 0xFF);
            page(x + 33, 22, 0xFF);
            page(x + 33, 23, 0xFF);
            page(x + 33, 24, 0xFF);
        }
    }
}

void movedoorright()
{
    black();
    drawlabels();
    for (int x = -33 + 6; x <= DISPLAY_WIDTH - 6; x++)
    {
        drawdoor(x);
        if (x > 0)
        {
            page(x - 1, 20, 0xFF);
            page(x - 1, 21, 0xFF);
            page(x - 1, 22, 0xFF);
            page(x - 1, 23, 0xFF);
            page(x - 1, 24, 0xFF);
        }
    }
}

void drawdoorright_closed()
{
    uint8_t i = 0;
    for (uint8_t y = 6; y < 20; y++)
    {
        for (uint8_t x = 156; x < DISPLAY_WIDTH; x++)
        {
            page(x, y, pgm_read_byte_near(rightrotatedfloorsprite + i));
            i++;
            if (i == 16)
                i = 0;
        }
    }
    
    i = 0;
    for (uint8_t y = 20; y < 25; y++)
    {
        for (uint8_t x = 154; x < DISPLAY_WIDTH; x++)
        {
            page(x, y, pgm_read_byte_near(doorright + i));
            i++;
        }
    }
}

void drawdoorleft_closed()
{
    uint8_t i = 0;
    for (uint8_t y = 6; y < 20; y++)
    {
        for (uint8_t x = 0; x < 4; x++)
        {
            page(x, y, pgm_read_byte_near(leftrotatedfloorsprite + i));
            i++;
            if (i == 16)
                i = 0;
        }
    }
    
    i = 0;
    for (uint8_t y = 20; y < 25; y++)
    {
        for (uint8_t x = 0; x < 6; x++)
        {
            page(x, y, pgm_read_byte_near(doorleft + i));
            i++;
        }
    }
}

void drawfloor()
{
    for (uint8_t pos = 0; pos < DISPLAY_WIDTH / 16; ++pos)
    {
        for (int x = 16 * pos; x < 16 * (pos + 1); ++x)
        {
            if (!(nofloor & (3l << 2 * pos))) // there is a 1/8 chance of drawing water/spikes
                page(x, 25, pgm_read_byte_near(nofloorsprite + x % 16));
            else
                page(x, 25, pgm_read_byte_near(floorsprite + x % 16));
        }
    }
}

void drawplatform()
{
    for (uint8_t pos = 0; pos < DISPLAY_WIDTH/PLATFORM_WIDTH; ++pos) // draw random platforms at 20 possible positions
    {
        if (!(platforms_19 & (3l << 2 * pos)))
        {
            for (short i = 0; i < PLATFORM_WIDTH; ++i)
            {
                page(PLATFORM_WIDTH * pos + i, 19, pgm_read_byte_near(floorsprite + i));
            }
        }
        if (!(platforms_13 & (3l << 2 * pos)))
        {
            for (short i = 0; i < PLATFORM_WIDTH; ++i)
            {
                page(PLATFORM_WIDTH * pos + i, 13, pgm_read_byte_near(floorsprite + i));
            }
        }
    }
    for (uint8_t pos = 0; pos < DISPLAY_WIDTH/16; ++pos)
    {
        if (!(platforms_24 & (3l << 2 * pos)))
        {
            page(16 * pos,      24, 0b11000000 & pgm_read_byte_near(floorsprite + 0));
            page(16 * pos + 1,  24, 0b11110000 & pgm_read_byte_near(floorsprite + 1));
            page(16 * pos + 2,  24, 0b11111100 & pgm_read_byte_near(floorsprite + 2));
            page(16 * pos + 3,  24,              pgm_read_byte_near(floorsprite + 3));
            page(16 * pos + 4,  24,              pgm_read_byte_near(floorsprite + 4));
            page(16 * pos + 5,  24,              pgm_read_byte_near(floorsprite + 5));
            page(16 * pos + 6,  24,              pgm_read_byte_near(floorsprite + 6));
            page(16 * pos + 7,  24,              pgm_read_byte_near(floorsprite + 7));
            page(16 * pos + 8,  24,              pgm_read_byte_near(floorsprite + 8));
            page(16 * pos + 9,  24,              pgm_read_byte_near(floorsprite + 9));
            page(16 * pos + 10, 24,              pgm_read_byte_near(floorsprite + 10));
            page(16 * pos + 11, 24,              pgm_read_byte_near(floorsprite + 11));
            page(16 * pos + 12, 24,              pgm_read_byte_near(floorsprite + 12));
            page(16 * pos + 13, 24, 0b11111100 & pgm_read_byte_near(floorsprite + 13));
            page(16 * pos + 14, 24, 0b11110000 & pgm_read_byte_near(floorsprite + 14));
            page(16 * pos + 15, 24, 0b11000000 & pgm_read_byte_near(floorsprite + 15));
        }
    }
}
