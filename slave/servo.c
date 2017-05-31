#include <avr/io.h>
#include "servo.h"

// Initialisiert die Pins B1 und B2 zur Ansteuerung von Servos
void servoInit() {
	DDRB |= 1<< PB1; //OC1A als Ausgang
	DDRB |= 1<< PB2;//OC1B als Ausgang
	
	// Wir verwenden den Phase-and-Freequency-Correct-PWM-Modus 
	// mit ICR1 als Top-Register.
	// Der Counter wird in Hardware inkrementiert und die Ausgangspins 
	// bei Match mit Vergleichsregister auf '0' gesetzt. 
	// Bei TOP==ICR1 startet der Zaehler wieder bei Null und der Ausgang
	// wird auf '1' gezogen.
	TCCR1A = (1<<COM1A1)|(0<<COM1A0)|(1<<COM1B1)|(0<<COM1B0)|(0<<WGM11)|(0<<WGM10);
	TCCR1B = (1<<WGM13)|(0<<WGM12)|(1<<CS11)|(0<<CS12)|(0<<CS10); //prescaler 8
	
	TCNT1 = 0;	  // Start bei 0
	ICR1 = 20000; // TOP => 20 ms  (bei externem 16MHz Quarz)
	
	// Servos in Mittelstellung
	OCR1A=(SERVO_MIN+SERVO_MAX)/2;   // Output-Compare-Match Register A
	OCR1B=(SERVO_MIN+SERVO_MAX)/2;   //                               B
	
}

// Steuert ein Servo auf den entsprechenden Winkel. 
// Positionsangabe us zwischen 1000us und 2000us.
void setServo(uint8_t nr, uint16_t us) {
	if (us>SERVO_MAX || us < SERVO_MIN)  // Nur Werte zwischen 1ms und 2ms sind erlaubt
		return;
	else if (nr==0)
		OCR1A=us;   // Vergleichsregister direkt setzen
	else
		OCR1B=us;
}
