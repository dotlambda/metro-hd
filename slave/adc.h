/* 
 *	Basis
 *	2009 Benjamin Reh
 */
 
#ifndef ADC_H
#define ADC_H
#include <inttypes.h>

//initialize ADC
void ADCInit(uint8_t kanal );

//Read adc-value (10 bit)
uint16_t getADCValue(uint8_t kanal);

#endif
