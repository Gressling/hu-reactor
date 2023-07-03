#include <Wire.h>
#include "Adafruit_AS726x.h"

#define SENSOR_MAX 5000

// Create the object
Adafruit_AS726x ams;

// Buffer to hold calibrated values
float calibratedValues[AS726x_NUM_CHANNELS];

// Array of color names
const char* colorNames[AS726x_NUM_CHANNELS] = {
  "Violet",
  "Blue",
  "Green",
  "Yellow",
  "Orange",
  "Red"
};

void setup()
{
  Serial.begin(9600);

  // Initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  // Begin and make sure we can talk to the sensor
  if (!ams.begin())
  {
    Serial.println("Could not connect to sensor! Please check your wiring.");
    while (1);
  }

  ams.setConversionType(MODE_2);

  // Uncomment this if you want to use the driver LED (off by default)
  //ams.drvOn();
}

void loop()
{
  if (ams.dataReady())
  {
    // Read the values
    ams.readCalibratedValues(calibratedValues);

    // Send the color names to Serial Plotter
    for (int i = 0; i < AS726x_NUM_CHANNELS; i++)
    {
      Serial.print(colorNames[i]);
      Serial.print(": ");
      Serial.print(calibratedValues[i]);
      if (i < AS726x_NUM_CHANNELS - 1)
        Serial.print('\t'); // Use tab as the separator
    }
    Serial.println();
  }
}
