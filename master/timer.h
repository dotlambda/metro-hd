/*/* 
 *	Basis
 *	2009 Benjamin Reh
 */
#ifndef TIMER_H
#define TIMER_H

#ifndef F_CPU
#warning "adjust OCR2A and set F_CPU"
#define F_CPU 16000000UL
#endif
#define OCR2A_VAL (F_CPU/(128000UL)) // 1 Millisekunde bei 128-Prescaler

uint8_t pauseTimer;

void timerInit(void);
uint32_t getMsTimer();
void delay(uint32_t ms);

#endif
