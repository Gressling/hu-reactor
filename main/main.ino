#include "Blinker.h"

Blinker ledBlinker(2); // LED is on pin 2 for Lolin board

void setup() {
  // no setup needed
}

void loop() {
  ledBlinker.blink(500); // Blink every 1/2 second
}
