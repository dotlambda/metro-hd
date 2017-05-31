#ifndef BUTTONS_H
#define BUTTONS_H
inline void buttonsInit() {
	DDRD &=~( (1<<2) | (1<<3) | (1<<4) | (1<<5) | (1<<6) | (1<<7)); 
	PORTD |=( (1<<2) | (1<<3) | (1<<4) | (1<<5) | (1<<6) | (1<<7)); 

	DDRC &= ~( (1<<4) | (1<<5));
	PORTC |= ( (1<<4) | (1<<5));
}

#define B_UP (!(PIND & (1<<2)))
#define B_DOWN (!(PIND & (1<<3)))
#define B_LEFT (!(PIND & (1<<4)))
#define B_RIGHT (!(PIND & (1<<5)))
#define B_A (!(PIND & (1<<6)))
#define B_B (!(PIND & (1<<7)))

#define B_PAUSE (!(PINC & (1<<4)))
#define B_SELECT (!(PINC & (1<<5)))



#endif
