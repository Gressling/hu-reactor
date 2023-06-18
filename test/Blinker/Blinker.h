#ifndef Blinker_h
#define Blinker_h

#include "Arduino.h"

class Blinker {
  public:
    Blinker(int pin);
    void blink(int duration);
  private:
    int _pin;
};

#endif
