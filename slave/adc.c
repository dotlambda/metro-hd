/* 
 *	Basis
 *	2009 Benjamin Reh
 */

  

#include <avr/io.h>
#include "adc.h"

//Initialisierung
void ADCInit(uint8_t kanal)
{
	// Hier wird der gewuenschte Pin auf Eingang geschaltet.  
	//Sollen mehrere Pins verwendet werden, diese Funktion mehrfach aufrufen oder manuell die entsprechenden Pins als Eingang setzen
	DDRC &=~(1<<kanal); 
	
	// Frequenzvorteiler setzen auf 128 und ADC aktivieren
	// Frequenz des ADC = Frequenz des Controllers (16 Mhz) geteilt durch Frequenzteiler
	// Bereich bitte im Datenblatt nachlesen
	ADCSRA = (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);    
 	// AVCC als Referenz nutzen 
	//  d.h. Digitalwerte entsprechen Spannungen von 0 bis 5V (=AVCC)
	//  alternativ kann auch eine externe Spannungreferenz verwenden werden (siehe Datenblatt)
	ADMUX = (1<<REFS0) | kanal;  // den richtigen Kanal auswaehlen
	

	// nach Aktivieren des ADC wird ein "Dummy-Readout" empfohlen, man liest
    // also einen Wert und verwirft diesen, um den ADC "warmlaufen zu lassen" 
  	ADCSRA |= (1<<ADSC);              // eine ADC-Wandlung 
    while ( ADCSRA & (1<<ADSC) ) {
		// auf Abschluss der Konvertierung warten 
  	}

}

//Wert auslesen
uint16_t getADCValue(uint8_t kanal)
{
	ADMUX  &= ~0x0F; 				// Kanal auf 0 setzen
	ADMUX  |= (kanal & 0x0F);  		// den richtigen Kanal auswaehlen
	ADCSRA |= (1<<ADSC);            // eine Wandlung "single conversion"
	while ( ADCSRA & (1<<ADSC) ){}  // auf Abschluss der Konvertierung warten
	
	return ADCW;

}
