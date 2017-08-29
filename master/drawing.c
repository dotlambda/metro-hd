#include <stdlib.h>
#include "drawing.h"
#include "display.h"
#include "sprites.h"
#include "level.h"
#include "stdio.h"
#include "string.h"

void drawsplash(uint8_t show_resume_option)
{
    drawsprite(15, 3, 126, 19, splash);
    if (show_resume_option)
        drawsprite(43, 23, 74, 3, resume);
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
    drawsprite(x, y, 3, 3, sprite);
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
    drawsprite(2, 1, 23, 3, labelenergy);
    
    // print rocket label
    drawsprite(40, 1, 15, 3, labelrocket);
    
    // print bomb label
    drawsprite(69, 1, 14, 3, labelbomb);
    
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
    
    drawsprite(154, 20, 6, 5, doorright);
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
    
    drawsprite(0, 20, 6, 5, doorleft);
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

// y and height in pages
void drawsprite(uint8_t x, uint8_t y, uint8_t width, uint8_t height, const uint8_t* sprite)
{
    sendbyte(0b11110100, 0); // set window start column
    sendbyte(x, 0);
    sendbyte(0b11110110, 0); // set window end column
    sendbyte(x + width - 1, 0);
    sendbyte(0b11110101, 0); // set window start page
    sendbyte(y, 0);
    sendbyte(0b11110111, 0); // set window end page
    sendbyte(y + height - 1, 0);
    sendbyte(0b11111001, 0); // enable window function
    for (uint16_t i = 0; i < width * height; ++i)
        sendbyte(pgm_read_byte_near(sprite + i), 1);
    sendbyte(0b11111000, 0); // disable window function
}

// y and height in pixels
void drawsprite_px(uint8_t x, uint8_t y, uint8_t width, uint8_t height, const uint8_t* sprite)
{
    uint8_t offset = y % 4;
    if (offset == 0)
    {
        drawsprite(x, y / 4, width, height / 4, sprite);
    }
    else
    {
        sendbyte(0b11110100, 0); // set window start column
        sendbyte(x, 0);
        sendbyte(0b11110110, 0); // set window end column
        sendbyte(x + width - 1, 0);
        sendbyte(0b11110101, 0); // set window start page
        sendbyte(y / 4, 0);
        sendbyte(0b11110111, 0); // set window end page
        sendbyte(y / 4 + height / 4, 0);
        sendbyte(0b11111001, 0); // enable window function
        uint16_t i = 0;
        for (; i < width; ++i)
            sendbyte(pgm_read_byte_near(sprite + i) << offset, 1);
        for (; i < height / 4 * width; ++i)
            sendbyte(pgm_read_byte_near(sprite + i) << offset | pgm_read_byte_near(sprite + i - width) >> (8 - offset), 1);
        for (; i < (height / 4 + 1) * width; ++i)
            sendbyte(pgm_read_byte_near(sprite + i - width) >> (8 - offset), 1);
        sendbyte(0b11111000, 0); // disable window function
    }
}

void drawletters(uint8_t x, uint8_t y, char* Sentence)
{
    int len = strlen ( Sentence );
    for (int i = 0; i < len; i++)
    {
       char Letter = Sentence[i];
        switch(Letter)
        {
            case 'A':
                drawsprite_px(x, y, 5, 9, A);
                x += 1;
                break;
                
            case 'B':
                drawsprite_px(x, y, 5, 9, B);
                x += 1;
                break;
                            
            case 'C':
                drawsprite_px(x, y, 5, 9, C);
                x += 1;
                break;
                
            case 'D':
                drawsprite_px(x, y, 5, 9, D);
                x += 1;
                break;
                
            case 'E':
                drawsprite_px(x, y, 5, 9, E);
                x += 1;
                break;
                
            case 'F':
                drawsprite_px(x, y, 5, 9, F);
                x += 1;
                break;
                
            case 'G':
                drawsprite_px(x, y, 5, 9, G);
                x += 1;
                break;
                
            case 'H':
                drawsprite_px(x, y, 5, 9, H);
                x += 1;
                break;
                
            case 'I':
                drawsprite_px(x, y, 5, 9, I);
                x += 1;
                break;
                
            case 'J':
                drawsprite_px(x, y, 5, 9, J);
                x += 1;
                break;
                
            case 'K':
                drawsprite_px(x, y, 5, 9, K);
                x += 1;
                break;
                
            case 'L':
                drawsprite_px(x, y, 5, 9, L);
                x += 1;
                break;
                
            case 'M':
                drawsprite_px(x, y, 5, 9, M);
                x += 1;
                break;
                
            case 'N':
                drawsprite_px(x, y, 5, 9, N);
                x += 1;
                break;
                
            case 'O':
                drawsprite_px(x, y, 5, 9, O);
                x += 1;
                break;
                
            case 'P':
                drawsprite_px(x, y, 6, 9, P);
                x += 1;
                break;
                
            case 'Q':
                drawsprite_px(x, y, 5, 9, Q);
                x += 1;
                break;
                
            case 'R':
                drawsprite_px(x, y, 5, 9, R);
                x += 1;
                break;
                
            case 'S':
                drawsprite_px(x, y, 4, 9, S);
                x += 1;
                break;
                
            case 'T':
                drawsprite_px(x, y, 5, 9, T);
                x += 1;
                break;
                
            case 'U':
                drawsprite_px(x, y, 5, 9, U);
                x += 1;
                break;
                
            case 'V':
                drawsprite_px(x, y, 5, 9, V);
                x += 1;
                break;
                
            case 'W':
                drawsprite_px(x, y, 5, 9, W);
                x += 1;
                break;
                
            case 'X':
                drawsprite_px(x, y, 5, 9, X);
                x += 1;
                break;
                
            case 'Y':
                drawsprite_px(x, y, 5, 9, Y);
                x += 1;
                break;
                
            case 'Z':
                drawsprite_px(x, y, 5, 9, Z);
                x += 1;
                break;
            case ' ':
                x += 4;
                break;
                
        }
    }
} 

