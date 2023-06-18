#ifndef Temperature_h
#define Temperature_h

#include "Arduino.h"

class Temperature {
  public:
    Temperature(int pin);
    int read();
  private:
    int _pin;
};

#endif
