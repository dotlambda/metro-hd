#ifndef MUSIC_H
#define MUSIC_H

#include <inttypes.h>
#include <avr/pgmspace.h>

// a Change has a size of 32 bits
typedef struct __attribute__((packed)) {
    uint16_t delay;
    unsigned track: 2;
    unsigned increment: 14;
} Change;
typedef union {
    Change change;
    uint32_t bits;
} Event;

#define STOP { { 0xFFFF, 0, 0 } }

extern const Event ingame2[669] PROGMEM;

extern const Event ingame1[706] PROGMEM;

extern const Event boss1[53] PROGMEM;

extern const Event boss2[401] PROGMEM;

extern const Event boss3[329] PROGMEM;

extern const Event gameover[25] PROGMEM;

#endif