#ifndef MUSIC_H
#define MUSIC_H

#include <inttypes.h>
#include <avr/pgmspace.h>

#define STOP 0xFFFF

extern const uint16_t ingame2[2005] PROGMEM;

extern const uint16_t ingame1[2116] PROGMEM;

extern const uint16_t boss1[157] PROGMEM;

extern const uint16_t boss2[1201] PROGMEM;

extern const uint16_t boss3[985] PROGMEM;

extern const uint16_t gameover[73] PROGMEM;

#endif