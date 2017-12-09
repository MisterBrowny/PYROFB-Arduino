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

// Définition des pyronums et de leur entrée
#define DefNbPyro    4

#define IN_PYRO_1   4   // Input Arduino du Pyro 1 (pin 6 - subd-9)
#define IN_PYRO_2   5   // Input Arduino du Pyro 2 (pin 7 - subd-9)
#define IN_PYRO_3   6   // Input Arduino du Pyro 3 (pin 8 - subd-9)
#define IN_PYRO_4   7   // Input Arduino du Pyro 4 (pin 9 - subd-9)

// Définition des differents modes
typedef enum FireMode
{
    Fire_Undefined = 0,
    Fire_Armed = 1,
    Fire_Go = 2,
    Fire_Stop = 3,
    Fire_End = 4
};

// Définition de la structure pour gérer le feedback
typedef struct StructPyro 
{
    volatile bool       Rise;
    volatile bool       Fall;
    unsigned long       RiseTime;
    unsigned long       FallTime;
    char                Count;
    FireMode            Mode;
    FireMode            MemoMode;
}StruPyro;

StruPyro Pyro[DefNbPyro];

// Définition des temps pour récupérer le feedback
#define IMPULSE_TIME    30ul    // Temps d'une impulsion en ms (Temps réel d'une impulsion 50ms)
#define MIN_IDLE_TIME   100ul   // Temps idle minimum entre 2 séries d'impulsions en ms (Temps réel d'un IDLE min 150ms)
#define TIME_OUT_FB     1000ul  // Time out au delà duquel le feedback est considéré indéfini

//------------------------------------------------------------------------------//

// Définition des interruptions
void Pyro_1_Interrupt() {
    if (digitalRead(IN_PYRO_1)) {   Pyro[0].Rise = true;    }
    else                        {   Pyro[0].Fall = true;    }
}

void Pyro_2_Interrupt() {
    if (digitalRead(IN_PYRO_2)) {   Pyro[1].Rise = true;    }
    else                        {   Pyro[1].Fall = true;    }
}

void Pyro_3_Interrupt() {
    if (digitalRead(IN_PYRO_3)) {   Pyro[2].Rise = true;    }
    else                        {   Pyro[2].Fall = true;    }
}

void Pyro_4_Interrupt() {
    if (digitalRead(IN_PYRO_4)) {   Pyro[3].Rise = true;    }
    else                        {   Pyro[3].Fall = true;    }
}



void setup() {
    
    // start serial connection
    Serial.begin(9600);
  
    // Configure inputs en pull up
    pinMode(IN_PYRO_1, INPUT_PULLUP);
    pinMode(IN_PYRO_2, INPUT_PULLUP);
    pinMode(IN_PYRO_3, INPUT_PULLUP);
    pinMode(IN_PYRO_4, INPUT_PULLUP);
  
    // Configure interruptions
    enableInterrupt(IN_PYRO_1, Pyro_1_Interrupt, CHANGE);
    enableInterrupt(IN_PYRO_2, Pyro_2_Interrupt, CHANGE);
    enableInterrupt(IN_PYRO_3, Pyro_3_Interrupt, CHANGE);
    enableInterrupt(IN_PYRO_4, Pyro_4_Interrupt, CHANGE);
}

void loop() {
    unsigned char i;
    unsigned long refresh;
    
    for (i = 0; i < DefNbPyro; i++)
    {
        refresh = millis();
    
        if (Pyro[i].Rise == true)
        {
            Pyro[i].RiseTime = refresh;
            Pyro[i].Rise = false;
        
            if ((Pyro[i].RiseTime - Pyro[i].FallTime) > IMPULSE_TIME)   {   Pyro[i].Count ++;   }
        }
    
        if (Pyro[i].Fall == true)
        {
            Pyro[i].FallTime = refresh;
            Pyro[i].Fall = false;
        
            if ((Pyro[i].FallTime - Pyro[i].RiseTime) > MIN_IDLE_TIME)
            {
                Pyro[i].Mode = (FireMode) Pyro[i].Count;
                Pyro[i].Count = 0;
            }
        }
        
        // Envoi de mode en cours sur la RS
        if (Pyro[i].MemoMode != Pyro[i].Mode)
        {
            Pyro[i].MemoMode = Pyro[i].Mode;
        
            Serial.print("Fire Mode Pyro_");
            Serial.print(i + 1);
            switch (Pyro[i].MemoMode)
            {
                case Fire_Armed:        Serial.println(": Armed");      break;
                case Fire_Go:           Serial.println(": Go");         break;
                case Fire_Stop:         Serial.println(": Stop");       break;
                case Fire_End:          Serial.println(": End");        break;
                case Fire_Undefined: 
                default:                Serial.println(": Unknown");    break;
            }
        }
        
        // Gestion time out en cas de perte de signal
        if ((millis() - Pyro[i].RiseTime) > TIME_OUT_FB)    {   Pyro[i].Mode = Fire_Undefined;  }
        if ((millis() - Pyro[i].FallTime) > TIME_OUT_FB)    {   Pyro[i].Mode = Fire_Undefined;  }
    }
}
