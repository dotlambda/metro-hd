#include "drawing.h"
#include "display.h"
#include "sprites.h"
#include "level.h"
#include "string.h"

void drawsplash(uint8_t show_resume_option)
{
    drawsprite(15, 6, 40, 7, splashleft);
    drawsprite(55, 3, 52, 15, splashcenter);
    drawsprite(107, 6, 34, 7, splashright);
    drawsprite(39, 19, 84, 3, splashtext);
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

void drawdigit2(uint8_t x, uint8_t y, uint8_t digit)
{
    const uint8_t* sprite = NULL;
    switch (digit)
    {
        case 0:
            sprite = zero2;
            break;
        case 1:
            sprite = one2;
            break;
        case 2:
            sprite = two2;
            break;
        case 3:
            sprite = three2;
            break;
        case 4: 
            sprite = four2;
            break;
        case 5: 
            sprite = five2;
            break;
        case 6:
            sprite = six2;
            break;
        case 7:
            sprite = seven2;
            break;
        case 8:
            sprite = eight2;
            break;
        case 9:
            sprite = nine2;
            break;
    }
    drawsprite_inverted(x, y, 3, 2, sprite);
}

void drawnumber(uint8_t x, uint8_t y, uint8_t number, bool inverted)
{
    uint8_t leftdigit = number / 10;
    uint8_t rightdigit = number % 10;
    if (inverted)
    {
        drawdigit2(x, y, leftdigit);
        drawdigit2(x + 4, y, rightdigit);
    }
    else
    {
        drawdigit(x, y, leftdigit);
        drawdigit(x + 4, y, rightdigit);
    }
}

void drawlabels()
{
    // print energy at the top
    drawsprite(2, 1, 23, 3, labelenergy);
    
    // print rocket label
    drawsprite(40, 1, 15, 3, labelrocket);
    
    // print bomb label
    drawsprite(69, 1, 14, 3, labelbomb);
    
    drawnumber(29, 1, protagonist->health, false);
    drawnumber(57, 1, num_rockets, false);
    drawnumber(86, 1, num_bombs, false);
}

void drawmonsterhealth(struct Character* monster)
{
    uint8_t bar_len = monster->health <= 0 ? 0 : ((uint16_t) monster->health * BAR_LEN) / monster->initial_health;
    page(BAR_X, BAR_Y, 0b00111100);
    drawcolor(BAR_X + 1, BAR_Y, bar_len, 1, 0b11101011);
    drawcolor(BAR_X + 1 + bar_len, BAR_Y, BAR_LEN - bar_len, 1, 0b11000011);
    page(BAR_X + 1 + BAR_LEN, BAR_Y, 0b00111100);
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
    drawletters(60, 15, "LEVEL", true);
    if (level < 0)
    {
        drawnumber(85, 15, -level, true);
    }
    else
    {
        drawnumber(85, 15, level, true);
    }
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
    drawletters(60, 15, "LEVEL", true);
    if (level < 0)
    {
        drawnumber(85, 15, -level, true);
    }
    else
    {
        drawnumber(85, 15, level, true);
    }
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

void enable_window(uint8_t x, uint8_t y, uint8_t width, uint8_t height)
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
}

void disable_window()
{
    sendbyte(0b11111000, 0); // disable window function
}

// y and height in pages
void drawsprite(uint8_t x, uint8_t y, uint8_t width, uint8_t height, const uint8_t* sprite)
{
    enable_window(x, y, width, height);
    for (uint16_t i = 0; i < width * height; ++i)
        sendbyte(pgm_read_byte_near(sprite + i), 1);
    disable_window();
}

// color inverted
void drawsprite_inverted(uint8_t x, uint8_t y, uint8_t width, uint8_t height, const uint8_t* sprite)
{
    enable_window(x, y, width, height);
    for (uint16_t i = 0; i < width * height; ++i)
    {
        sendbyte(~pgm_read_byte_near(sprite + i), 1);
    }
    disable_window();
}

// y axis inverted
void drawsprite_rotated(uint8_t x, uint8_t y, uint8_t width, uint8_t height, const uint8_t* sprite)
{
    enable_window(x, y, width, height);
    for (uint16_t i = 0; i < width * height; i += width)
    {
        for (int16_t j = width - 1; j >= 0; --j)
        {
            sendbyte(pgm_read_byte_near(sprite + i + j), 1);
        }
    }
    disable_window();
}

// y and height in pixels
void drawsprite_px(uint8_t x, uint8_t y, uint8_t width, uint8_t height, const uint8_t* sprite)
{
    uint8_t offset = 2 * (y % 4);
    if (offset == 0)
    {
        drawsprite(x, y / 4, width, height / 4, sprite);
    }
    else
    {
        enable_window(x, y / 4, width, height / 4 + 1);
        uint16_t i = 0;
        for (; i < width; ++i)
            sendbyte(pgm_read_byte_near(sprite + i) << offset, 1);
        for (; i < height / 4 * width; ++i)
            sendbyte(pgm_read_byte_near(sprite + i) << offset | pgm_read_byte_near(sprite + i - width) >> (8 - offset), 1);
        for (; i < (height / 4 + 1) * width; ++i)
            sendbyte(pgm_read_byte_near(sprite + i - width) >> (8 - offset), 1);
        disable_window();
    }
}

// y axis inverted
void drawsprite_px_rotated(uint8_t x, uint8_t y, uint8_t width, uint8_t height, const uint8_t* sprite)
{
    uint8_t offset = 2 * (y % 4);
    if (offset == 0)
    {
        drawsprite_rotated(x, y / 4, width, height / 4, sprite);
    }
    else
    {
        enable_window(x, y / 4, width, height / 4 + 1);
        for (int16_t j = width - 1; j >= 0; --j)
            sendbyte(pgm_read_byte_near(sprite + j) << offset, 1);
        for (uint16_t i = width; i < height / 4 * width; i += width)
        {
            for (int16_t j = width - 1; j >= 0; --j)
            {
                sendbyte(pgm_read_byte_near(sprite + i + j) << offset | pgm_read_byte_near(sprite + i - width + j) >> (8 - offset), 1);
            }
        }
        for (int16_t j = (height / 4) * width - 1; j >= (height / 4 - 1) * width; --j)
            sendbyte(pgm_read_byte_near(sprite + j) >> (8 - offset), 1);
        disable_window();
    }
}

// y and height in pages
// color is actually the byte that will be used for each page
void drawcolor(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color)
{
    enable_window(x, y, width, height);
    for (uint16_t i = 0; i < width * height; ++i)
        sendbyte(color, 1);
    disable_window();
}

void drawrechargeroom()
{
    drawcolor(4, 6, 19, 4, 0xFF);
    drawsprite(23, 6, 68, 4, rechargeleft);
    drawcolor(91, 6, 12, 4, 0xFF);
    drawsprite(103, 6, 37, 4, rechargeright);
    drawcolor(140, 6, 16, 4, 0xFF);
    drawsprite(DISPLAY_WIDTH/2 - 12, 10, 24, 7, rechargetop);
    drawsprite(DISPLAY_WIDTH/2 - 12, 23, 24, 2, mountain); 
}

void drawletters(uint8_t x, uint8_t y, char* string, bool inverted)
{
    int len = strlen (string);
    for (int i = 0; i < len; i++)
    {
        if (string[i] == ' ')
        {
            page(x, y, inverted ? 0xFF : 0);
            page(x, y + 1, inverted ? 0xFF : 0);
            page(x + 1, y, inverted ? 0xFF : 0);
            page(x + 1, y + 1, inverted ? 0xFF : 0);
            x += 2;
        }
        else if (string[i] == ',')
        {
            drawsprite(x, y + 1, 2, 2, comma);
            page(x + 2, y, inverted ? 0xFF : 0);
            page(x + 2, y + 1, inverted ? 0xFF : 0);
            x += 3; // 2 pixels for the comma, 1 for the space between letters
        }
        else
        {
            const uint8_t* sprite = NULL;
            uint8_t width = 0;
            switch(string[i])
            {
                case 'A':
                    sprite = A;
                    width = 4;
                    break;
                case 'B':
                    sprite = B;
                    width = 4;
                    break;
                case 'C':
                    sprite = C;
                    width = 3;
                    break;
                case 'D':
                    sprite = D;
                    width = 4;
                    break;
                case 'E':
                    sprite = E;
                    width = 4;
                    break;
                case 'F':
                    sprite = F;
                    width = 4;
                    break;
                case 'G':
                    sprite = G;
                    width = 4;
                    break;
                case 'H':
                    sprite = H;
                    width = 4;
                    break;
                case 'I':
                    sprite = I;
                    width = 1;
                    break;
                case 'J':
                    sprite = J;
                    width = 3;
                    break;
                case 'K':
                    sprite = K;
                    width = 5;
                    break;
                case 'L':
                    sprite = L;
                    width = 3;
                    break;
                case 'M':
                    sprite = M;
                    width = 5;
                    break;
                case 'N':
                    sprite = N;
                    width = 5;
                    break;
                case 'O':
                    sprite = O;
                    width = 4;
                    break;
                case 'P':
                    sprite = P;
                    width = 4;
                    break;
                case 'Q':
                    sprite = Q;
                    width = 4;
                    break;
                case 'R':
                    sprite = R;
                    width = 4;
                    break;
                case 'S':
                    sprite = S;
                    width = 3;
                    break;
                case 'T':
                    sprite = T;
                    width = 5;
                    break;
                case 'U':
                    sprite = U;
                    width = 4;
                    break;
                case 'V':
                    sprite = V;
                    width = 5;
                    break;
                case 'W':
                    sprite = W;
                    width = 7;
                    break;
                case 'X':
                    sprite = X;
                    width = 6;
                    break;
                case 'Y':
                    sprite = Y;
                    width = 5;
                    break;
                case 'Z':
                    sprite = Z;
                    width = 4;
                    break;
                case '!':
                    sprite = ex;
                    width = 1;
                    break;
                case '.':
                    sprite = dot;
                    width = 1;
                    break;
            }
            if (inverted)
            {
                drawsprite_inverted(x, y, width, 2, sprite);
            }
            else
            {
                drawsprite(x, y, width, 2, sprite);
            }
            page(x + width, y, inverted ? 0xFF : 0);
            page(x + width, y + 1, inverted ? 0xFF : 0); 
            x += width + 1;
        }
    }
}

