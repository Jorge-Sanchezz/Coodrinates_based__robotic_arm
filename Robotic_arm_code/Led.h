/*
  Led.h - Library for flashing led's
  Created by Jorge E. Sánchez
*/
#ifndef Led_h
#define Led_h

#include "Arduino.h"

class Led
{
  public:
    Led(int pin);
    void begin();
    void stand_by_mode();
  private:
    int _pin;
};

#endif