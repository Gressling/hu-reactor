#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <TimeLib.h>
#include <ESP8266HTTPClient.h>
#include "Adafruit_AS726x.h"
#include <Wire.h>

const char* ssid = "7520Low";
const char* password = "62272999407145972343";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 0;
const int   daylightOffset_sec = 3600;

WiFiUDP udp;
NTPClient timeClient(udp, ntpServer, gmtOffset_sec, daylightOffset_sec);

WiFiClient wifiClient;
HTTPClient http;

// const int sensorPin = A0;
// int valA0 = 0;
// int counter = 0;

Adafruit_AS726x ams;
float calibratedValues[AS726x_NUM_CHANNELS];

// const char* colorNames[AS726x_NUM_CHANNELS] = {
//   "Violet",
//   "Blue",
//   "Green",
//   "Yellow",
//   "Orange",
//   "Red"
// };

void setup() {
  Serial.begin(9600);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    // Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  if (!ams.begin()) {
    Serial.println("Could not connect to sensor! Please check your wiring.");
    while (1);
  }
  ams.setConversionType(MODE_2);
  delay(1000); //sensor needs a second to set up

  timeClient.begin();
  timeClient.update();

  for (int i = 0; i < 10; i++) {
    run_once();
  }
}

void loop() {
  // Serial.println("I'm a loop!");
  delay(10000);
}

void run_once() {
  timeClient.update();

  String time = timeClient.getFormattedTime();
  
  time_t now = timeClient.getEpochTime();
  int currentYear = year(now);
  int currentMonth = month(now);
  int currentDay = day(now);

  ams.readCalibratedValues(calibratedValues);

  for (int i = 0; i < AS726x_NUM_CHANNELS; i++) {
    String postData = "fkeyLimsDevice=6&timestamp=" + String(currentYear) + "-" + String(currentMonth) +
      "-" + String(currentDay) + " " + time + "&sensorValue=" + String(i + 1) + String(int(round(calibratedValues[i])));
    
    String host = "http://labinformatics.gearhostpreview.com/v1";
    String uri = "lims-timeseries.php";
  
  // if (counter > 0) {
  //   counter--;
  // } else {
    http.begin(wifiClient, host + "/" + uri);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    int httpCode = http.POST(postData);
    Serial.println(postData);
  // }
  }
}