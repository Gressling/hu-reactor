#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <TimeLib.h>
#include <ESP8266HTTPClient.h>
#include "Adafruit_AS726x.h"
#include <DallasTemperature.h>
#include <OneWire.h>

const char* ssid = "";
const char* password = "";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 0;
const int   daylightOffset_sec = 3600;

WiFiUDP udp;
NTPClient timeClient(udp, ntpServer, gmtOffset_sec, daylightOffset_sec);
WiFiClient wifiClient;
HTTPClient http;

String host = "http://labinformatics.gearhostpreview.com/v1";
String uri = "lims-timeseries.php";

Adafruit_AS726x ams;
float calibratedValues[AS726x_NUM_CHANNELS];

//pins
// D0,1,2                   //color sensor
const int BUTTON_PIN = D4;   //button
const int ENA_PIN = D6;     //motor
const int IN1_PIN = D7;     //motor
const int IN2_PIN = D8;     //motor
const int PH_PIN = A0;      //ph sensor
const int peltierPin = D5;   //peltier
const int temp_pin = D3;    //temp sensor

//temp
OneWire oneWire(temp_pin);
DallasTemperature sensors(&oneWire);

//button
volatile int buttonState = LOW;
bool experimentRunning = false;

//peltier
float goal_temp = 40;

//button interrupt
void ICACHE_RAM_ATTR buttonISR() {
  buttonState = digitalRead(BUTTON_PIN);

  if (buttonState == LOW) {
    // Button is pressed, start the experiment
    experimentRunning = !experimentRunning;
    Serial.println("button: " + String(experimentRunning));
  }
}

void setUpWifi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
    delay(100);
}

void initialize() {
  //motor
  pinMode(ENA_PIN, OUTPUT);
  pinMode(IN1_PIN, OUTPUT);
  pinMode(IN2_PIN, OUTPUT);

  analogWrite(ENA_PIN, 0);
  //peltier
  pinMode(peltierPin, OUTPUT);
  digitalWrite(peltierPin, LOW);
  
  //button
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonISR, CHANGE);
  //color sensor
  if (!ams.begin()) {
    Serial.println("Could not connect to sensor! Please check your wiring.");
    while (1);
  }
  ams.setConversionType(MODE_2);
  delay(1000); //sensor needs a second to set up
  //time
  timeClient.begin();
  timeClient.update();
}

float getTemp() {
  sensors.requestTemperatures();
  return sensors.getTempCByIndex(0);
}
float getPH() {
  float measurings = analogRead(PH_PIN);//0;
  float voltage = 3.3/1024 * measurings;//(measurings/3); 	  //convert the analog into millivolt
  float phValue = 7 + (((3.3/2) - voltage)/ 0.18);//-5.7 * voltage + calibration;    //convert the millivolt into pH value
  return phValue;
}
String getCol() {
  String payload = "";
  ams.readCalibratedValues(calibratedValues);
  for (int i = 0; i < AS726x_NUM_CHANNELS; i++) {
    int roundedValue = round(calibratedValues[i]); // Round the value to the nearest integer
    payload += String(roundedValue); // Convert the rounded value to a string and append it to the result string

    if (i < AS726x_NUM_CHANNELS - 1) {
      payload += "|"; // Add the separator '|' between values (except for the last value)
    }
  }
  return payload;
}
String gatherSensorData() {
  //time
  timeClient.update();
  String time = timeClient.getFormattedTime();
  time_t now = timeClient.getEpochTime();
  int currentYear = year(now);
  int currentMonth = month(now);
  int currentDay = day(now);

  //ph
  float ph = getPH();

  //temp
  float temp = getTemp();

  //color
  String col = getCol();

  //concatenate
  String payload = "Col: " + col + " | PH:  " + String(ph) + " | Temp: " + String(temp);

  //post
  String postData = "fkeyLimsDevice=6&timestamp=" + String(currentYear) + "-" + String(currentMonth) +
    "-" + String(currentDay) + " " + time + "&sensorValue= " + payload;

  return postData;
}
void sendToDatabase(String postData) {
  http.begin(wifiClient, host + "/" + uri);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  int httpCode = http.POST(postData);
}
bool control_temp() {
  float current_temp = getTemp();
  if (current_temp < (goal_temp - 3)) {
    digitalWrite(peltierPin, HIGH);
    Serial.println(current_temp);
    return false;
  }
  digitalWrite(peltierPin, LOW);
  return true;
}

void setup() {
  Serial.begin(9600);
  // setUpWifi(); -- temp. deactivated
  initialize();
}

void loop() {

  String postData = gatherSensorData();
  // sendToDatabase(postData);
  Serial.println(postData); //for debug
  if (experimentRunning){
    //turn on motor
    digitalWrite(IN1_PIN, HIGH);
    digitalWrite(IN2_PIN, LOW);
    analogWrite(ENA_PIN, 30);

    //turn on peltier
    digitalWrite(peltierPin, HIGH);
  }
  else {
    //turn off peltier
    digitalWrite(peltierPin, LOW);
    //turn off motor
    digitalWrite(IN1_PIN, LOW);
    digitalWrite(IN2_PIN, LOW);
    analogWrite(ENA_PIN, 0);
  }
  delay(2000);
}














