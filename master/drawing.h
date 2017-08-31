#ifndef DRAWING_H
#define DRAWING_H

#include <inttypes.h>
#include "character.h"

#define BAR_LEN 20
#define BAR_X 100
#define BAR_Y 2

void drawsplash(uint8_t show_resume_option);
void drawdigit(uint8_t x, uint8_t y, uint8_t digit);
void drawnumber(uint8_t x, uint8_t y, uint8_t number);
void drawlabels();
void drawmonsterhealth(struct Character* monster);
void drawenergytank(uint8_t x, uint8_t y);
void drawdoor(int x);
void black();
void movedoorleft();
void movedoorright();
void drawdoorright_closed();
void drawdoorleft_closed();
void drawfloor();
void drawplatform();
void drawrechargeroom();

// y and height in pages
void drawsprite(uint8_t x, uint8_t y, uint8_t width, uint8_t height, const uint8_t* sprite);

// y and height in pixels
void drawsprite_px(uint8_t x, uint8_t y, uint8_t width, uint8_t height, const uint8_t* sprite);

// y and height in pages
// color is actually the byte that will be used for each page
void drawcolor(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color);

void drawletters(uint8_t x, uint8_t y, char* Sentence);

#endif 
