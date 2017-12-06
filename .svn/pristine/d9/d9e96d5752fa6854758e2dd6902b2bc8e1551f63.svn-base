/*Begining of Auto generated code by Atmel studio */
#include <Arduino.h>

/*End of auto generated code by Atmel studio */


//Beginning of Auto generated function prototypes by Atmel Studio
//End of Auto generated function prototypes by Atmel Studio

/*
  Input Pull-up Serial

  This example demonstrates the use of pinMode(INPUT_PULLUP). It reads a digital
  input on pin 2 and prints the results to the Serial Monitor.

  The circuit:
  - momentary switch attached from pin 2 to ground
  - built-in LED on pin 13

  Unlike pinMode(INPUT), there is no pull-down resistor necessary. An internal
  20K-ohm resistor is pulled to 5V. This configuration causes the input to read
  HIGH when the switch is open, and LOW when it is closed.

  created 14 Mar 2012
  by Scott Fitzgerald

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/InputPullupSerial
*/

#include <EnableInterrupt.h>

typedef enum FireMode
{
	Fire_Undefined = 0,
	Fire_Armed = 1,
	Fire_Go = 2,
	Fire_Stop = 3,
	Fire_End = 4
};

#define PYRO_SUBD_7		4

#define IMPULSE_TIME	30ul	// Temps d'une impulsion en ms (Temps réel d'une impulsion 50ms)
#define MIN_IDLE_TIME	100ul	// Temps idle minimum entre 2 séries d'impulsions en ms (Temps réel d'un IDLE min 150ms)

#define TIME_OUT_FB		1000ul	// Time out au delà duquel le feedback est considéré indéfini

volatile char CountImpulse = 0;

volatile unsigned long ms_fall = 0;
volatile unsigned long ms_rise = 0;
volatile unsigned long refresh = 0;

volatile FireMode CurrentMode = Fire_Undefined;
volatile FireMode MemoCurrentMode = Fire_Undefined;

void SUBD_7_CHANGE() {
	char input = digitalRead(4);
	
	refresh = millis();
	
	if (input)
	{
		ms_rise = refresh;
		if ((ms_rise - ms_fall) > IMPULSE_TIME)	{	CountImpulse ++;	}
	}
	else
	{
		ms_fall = refresh;
		if ((ms_fall - ms_rise) > MIN_IDLE_TIME)
		{
			CurrentMode = (FireMode) CountImpulse;
			CountImpulse = 0;
		}
	}
}

unsigned long read_refresh(void) {
	unsigned long m;
	uint8_t oldSREG = SREG;

	cli();
	m = refresh;
	SREG = oldSREG;

	return m;
}


void setup() {
	
	// start serial connection
	Serial.begin(9600);
  
	// Configure input en pull up
	pinMode(PYRO_SUBD_7, INPUT_PULLUP);
  
	// Configure interruption
	enableInterrupt(PYRO_SUBD_7, SUBD_7_CHANGE, CHANGE);
}

void loop() {
	
	if ((millis() - read_refresh()) > TIME_OUT_FB)	{	CurrentMode = Fire_Undefined;	}
		
	if (MemoCurrentMode != CurrentMode)
	{
		MemoCurrentMode = CurrentMode;
		
		//print out the value of the pushbutton
		Serial.println("Fire Mode:");
		switch (CurrentMode)
		{
			case Fire_Armed:		Serial.println("Armed");	break;
			case Fire_Go:			Serial.println("Go");		break;
			case Fire_Stop:			Serial.println("Stop");		break;
			case Fire_End:			Serial.println("End");		break;
			case Fire_Undefined: 
			default:				Serial.println("Unknown");	break;
		}
	}
}
