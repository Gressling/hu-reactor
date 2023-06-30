# DS18B20 sensor

https://www.ardumotive.com/how-to-use-the-ds18b20-temperature-sensor-en.html 

You will need the [OneWire](https://www.arduinolibraries.info/libraries/one-wire) and [DallasTemperature](https://www.arduinolibraries.info/libraries/dallas-temperature) library to use this. Either download them at the given links or use the selector in the Arduino IDE.

Most of the library should be obvious but it allows you to pass a function that will then be used to send data whenever you use the send or readAndSend functions.