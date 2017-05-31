/* 
 *	Basis
 *	2009 Benjamin Reh und Joachim Schleicher
 */
#ifndef UART_H
#define UART_H

#include <avr/pgmspace.h>

//USART
#define BAUD 9600UL          // Baudrate

// F_CPU muss gesetzt sein.
#ifndef F_CPU
#warning "F_CPU war noch nicht definiert, wird nun nachgeholt mit 16000000"
#define F_CPU 16000000UL    // Systemtakt in Hz - Definition als unsigned long beachten >> Ohne ergeben Fehler in der Berechnung
#endif

// Baud Berechnungen
#define UBRR_VAL ((F_CPU+BAUD*8)/(BAUD*16)-1)   // clever runden
#define BAUD_REAL (F_CPU/(16*(UBRR_VAL+1)))     // Reale Baudrate
#define BAUD_ERROR ((BAUD_REAL*1000)/BAUD) // Fehler in Promille, 1000 = kein Fehler.

#if ((BAUD_ERROR<990) || (BAUD_ERROR>1010))
  #error "Systematischer Fehler der Baudrate groesser 1% und damit zu hoch!"
#endif


//Schnittstelle initialisieren
void uartInit();

//ein Zeichen (char) senden
int uart_putc(unsigned char c);

//eine Zeichenkette senden (char*)
//eine Neue Zeile wird mit dem Zeilenumruch '\n' begonnen. 
//z.B. uart_puts("\r\n");
void uart_puts(char *s);

// Zeichenkette aus Program-Space senden (verbraucht keinen SRAM Speicherplatz fuer die Strings)
// Beispiel: uart_puts_pgm(PSTR("Hello World!\r\n");
void uart_puts_pgm (const char* PROGMEM  str);

//eine 16bit-Zahl ausgeben
void uart_puti(int16_t i);

//ein einzelnes Zeichen empfangen
unsigned char uart_getc();

//Liegt ein Zeichen im Eingangspuffer?
#define uart_data_waiting() (UCSR0A & (1<<RXC0))


// eine Art uart_gets kann man sich noch ausdenken, sollte dann nicht mehr schwer sein

#endif
