#include "Temperature.h"

void defaultSend(float temperature) {
  Serial.print("The Temperature is: ");
  Serial.println(temperature);
}

void Temperature::Construct(int pin) { //not sure if this works with 
  _pin = pin;
  OneWire oneWire(pin);
  DallasTemperature sensors(&oneWire);

  _wire = oneWire;
  _temp = sensors;
  _temp.begin();
}

Temperature::Temperature(int pin, void (*defaultSendFunc)(float temperature)) {
  _sendFunction = defaultSendFunc;
  Construct(pin);
}

Temperature::Temperature(int pin) {
  Serial.begin(9600);
  _sendFunction = defaultSend;
  Construct(pin);
}

float Temperature::read() {
  _temp.requestTemperatures();
  lastTemperature = _temp.getTempCByIndex(0);
  return lastTemperature;
}

void Temperature::send() {
  _sendFunction(lastTemperature);
}

void Temperature::readAndSend() {
  read();
  send();
}