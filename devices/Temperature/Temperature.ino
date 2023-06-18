#include Temperature.h

Temperature Temp1(D1); // is T Sensor is on pin 10 for Lolin board?

void setup() {
}

void loop() {
  System.out(Temp1.read()); 
  delay(1000);
}