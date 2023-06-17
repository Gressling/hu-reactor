#include "Blinker.h"

Blinker ledBlinker(2); // LED is on pin 2 for Lolin board

void setup() {
  // no setup needed
}

void loop() {
  ledBlinker.blink(1000); // Blink every 1000 ms or 1 second
}
