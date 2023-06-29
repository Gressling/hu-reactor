#ifndef Temperature_h
#define Temperature_h

#include <DallasTemperature.h>
#include <OneWire.h>


class Temperature {
  public:
    Temperature(int pin);
    Temperature(int pin, void (*defaultSendFunc)(float temperature));
    float lastTemperature;
    float read();
    void send();
    void readAndSend();
  private:
    void Construct(int pin);  //to lazy to reformat it to CP++ 11 syntax thus it uses a Construct function
    int _pin;
    void (*_sendFunction)(float temperature);
    OneWire _wire;
    DallasTemperature _temp;
};

#endif
