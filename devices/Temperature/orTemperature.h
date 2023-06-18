#ifndef orTemperature_h
#define orTemperature_h

#include "Arduino.h"

class orTemperature {
  public:
    orTemperature(int pin);
    int read();
  private:
    int _pin;
};

#endif
