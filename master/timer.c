/* 
 *	Basis
 *	2009 Benjamin Reh
 */
#include <avr/interrupt.h>
#include "timer.h"

volatile uint32_t ms_timer;

void timerInit(void)
{
	TCNT2  = 0x00;			// TIMER vorladen
	TCCR2A = 1<<WGM21;		// CTC Mode
		
	TCCR2B = (1<<CS22) | (1<<CS20); //prescaler auf 128 setzen
	OCR2A = OCR2A_VAL; 			//Wert für Vergleichregister setzen (kleiner bedeutet schneller)
	TIMSK2  |= 1<<OCIE2A;		// TIMER2 Output Compare Match A Interrupt an
	
	sei();				// enable interrupts

	
	
}

SIGNAL (TIMER2_COMPA_vect)
{

//	hier koennen weitere Befehle ergaenzt werden, die jede Millisekunde ausgefuehrt werden sollen

	ms_timer++;  // Die "Systemzeit" inkrementieren


}

uint32_t getMsTimer() {
	uint32_t ret;
	cli(); // interrupts aus
	ret = ms_timer; // Wert aus volatile Variable kopieren
	sei(); // interrupts wieder an
	return ret;
}
