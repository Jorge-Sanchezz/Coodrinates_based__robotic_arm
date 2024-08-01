/*
  Led.h - Library for flashing led's
  Created by Jorge E. Sánchez
*/

#include "Arduino.h"
#include "Led.h"

unsigned long currentTime;
unsigned long ledPreviousTime = 0;
byte ledStepFlag = 0;

Led::Led(int pin)
{
  _pin = pin;
}

void Led::begin()
{
  pinMode(_pin, OUTPUT);
}

void Led::stand_by_mode(){
  currentTime = millis();

  if(ledStepFlag == 0 && currentTime - ledPreviousTime >= 80UL){
    digitalWrite(_pin, HIGH);
    ledStepFlag = 1;
    ledPreviousTime = currentTime;
  }
  else if(ledStepFlag == 1 && currentTime - ledPreviousTime >= 80UL){
    digitalWrite(_pin, LOW);
    ledStepFlag = 2;
    ledPreviousTime = currentTime;
  }
  else if(ledStepFlag == 2 && currentTime - ledPreviousTime >= 80UL){   
    digitalWrite(_pin, HIGH);
    ledStepFlag = 3;
    ledPreviousTime = currentTime;
  }
  else if(ledStepFlag == 3 && currentTime - ledPreviousTime >= 80UL){   
    digitalWrite(_pin, LOW);
    ledStepFlag = 4;
    ledPreviousTime = currentTime;
  }
  else if(ledStepFlag == 4 && currentTime - ledPreviousTime >= 920UL){   
    digitalWrite(_pin, LOW);
    ledStepFlag = 0;
    ledPreviousTime = currentTime;
  }
}
