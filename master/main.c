/* 
 *	Basis
 *	2009 Benjamin Reh und Joachim Schleicher
 */
#include <avr/io.h>
#include <inttypes.h>
#include <util/delay.h>
#include "uart.h"
#include "adc.h"
#include "timer.h"
#include "buttons.h"


void init();

int main(void)
{
	//Initialisierung ausfuehren
	
	init();

	uart_putc(80);
	_delay_ms(1000);
	uart_putc(10);
	_delay_ms(1000);


	while (1) {
		if(B_SELECT)
			uart_putc(20);
		if(B_PAUSE)
			uart_putc(30);
		if(B_UP)
			uart_putc(50);
		if(B_DOWN)
			uart_putc(60);
		if(B_RIGHT)
			uart_putc(70);
		if(B_LEFT)
			uart_putc(80);

		if(B_A)
			uart_putc(90);
		if(B_B)
			uart_putc(100);

	}	
}


//INIT
void init()
{
	uartInit();   // serielle Ausgabe an PC
	ADCInit(0);   // Analoge Werte einlesen
	timerInit();  // "Systemzeit" initialisieren
	buttonsInit();
}
