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
#include "display.h"


void init();


void full(uint8_t x) {
	page(x,5,0xFF);
	page(x,5+1,0xFf);
	page(x+1,5,0xFF);
	page(x+1,5+1,0xFf);
}
void empty(uint8_t x) {
	page(x,5,0xF0);
	page(x,5+1,0xF0);
	page(x+1,5,0xF0);
	page(x+1,5+1,0xF0);
}

void batteryMeter() {
	uint8_t x=0; 
	uint16_t adc=getADCValue(0);
	uint16_t low=696;//778; // (1024.*34./50.);
	uint16_t high=962; //(1024.*47./50.);
	uint8_t bars = 50 *(adc-low)/(high-low);
	//~ bars=adc/20;
	for(x=0; x< 50; x++)
	{
		if (x < bars) {
			full (x*3);
		} else {
			empty(x*3);
		}
	}
	
}



int main(void)
{
	//Initialisierung ausfuehren
	
	init();

	uart_putc(80);
	_delay_ms(1000);
		
	uart_putc(10);
	_delay_ms(1000);





	
	while (1) {
		batteryMeter();
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
	displayInit();
}
