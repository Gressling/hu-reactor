#include "orTemperature.h"

orTemperature::orTemperature(int pin) {
  pinMode(pin, OUTPUT);
  _pin = pin;
}

void orTemperature::read() {
  // code here
  return();
}
