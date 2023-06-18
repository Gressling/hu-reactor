#include "Temperature.h"

Temperature::Temperature(int pin) {
  pinMode(pin, OUTPUT);
  _pin = pin;
}

void Temperature::read() {
  // code here
  return();
}
