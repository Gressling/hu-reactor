#include "Blinker.h"

Blinker::Blinker(int pin) {
  pinMode(pin, OUTPUT);
  _pin = pin;
}

void Blinker::blink(int duration) {
  digitalWrite(_pin, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(duration);                       // wait for a second
  digitalWrite(_pin, LOW);    // turn the LED off by making the voltage LOW
  delay(duration);                       // wait for a second
}
