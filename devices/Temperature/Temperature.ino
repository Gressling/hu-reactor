#include "Temperature.h"

void defSend(float temperature) {
  Serial.print("Defaultsend: ");
  Serial.println(temperature);
}

Temperature Temp1(D2, defSend); //This passes a new default function to send the values.
//Temperature Temp1(D2);          //This uses the default.

void setup() {
  Serial.begin(9600); 
}

void loop() {
  Temp1.readAndSend();
  delay(1000);
}