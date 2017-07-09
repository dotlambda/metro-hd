#ifndef DISPLAY_H
#define DISPLAY_H

#define DISPLAY_WIDTH 160

void displayInit(void);
void clear(void);
void sendbyte(uint8_t byte,uint8_t cd);
void page(uint8_t x,uint8_t y,uint8_t h);

#endif
