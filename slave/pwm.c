/* 
 *	Basis
 *	2009 Benjamin Reh
 */

#include <avr/io.h>

//Pulsweite initialisieren
void PWMInit()
{
	DDRD |=  (1 << 6); //Pin 6 an PORTD auf Ausgang stellen
	TCCR0A = (1<<WGM00)|(1<<COM0A1); //  Timer/Counter als nicht invertierenden 8-Bit PWM
	TCCR0B = (1<<CS01) | (1<<CS00); //  Takt von CK / 64 generieren
}

//Pulsweite setzen
void setPWM(uint8_t pwmWert)
{
	//Wert in das Register schreiben
	OCR0A = pwmWert;
}
