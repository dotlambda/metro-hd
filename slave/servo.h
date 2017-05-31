#ifndef SERVO_H
#define SERVO_H

/////////////////////////////////////////////////////////////////////
// Einfache Ansteuerung fuer zwei Servos.
// 2009. Benjamin Reh, Sven Ebser, Joachim Schleicher
//
// Wir verwenden hier den Hardware-Timer, um das Signal zu erzeugen.
//
// Grundlagen zur Servo-Ansteuerung:
// http://www.rn-wissen.de/index.php/Servos
/////////////////////////////////////////////////////////////////////

#define SERVO_MIN 1000  // Links-Anschlag des Servos
#define SERVO_MAX 2000  // Rechts-Anschlag des Servos

void servoInit();
void setServo(uint8_t nr, uint16_t us);

#endif
