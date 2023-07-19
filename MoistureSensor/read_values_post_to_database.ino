#include <SPI.h>
#include <Wire.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include "NTPClient.h"
#include "ESP8266WiFi.h"
#include "WiFiUdp.h"


const char *ssid = "Your_Wifi_name";
const char *password = "Your_Wifi_password";

const long utcOffsetInSeconds = 7200;

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

WiFiClient wifiClient;
HTTPClient http;


int sensorPin = A0; // select the input pin for moisture sensor

int sensorValue = 0; // variable to store the value coming from the sensor
void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while( WiFi.status() != WL_CONNECTED ) {
  delay( 500 );
  }
  timeClient.begin();

}

void loop() {
  
  delay(60000); // wait one minute
  
  timeClient.update();
  sensorValue = analogRead(sensorPin); // read the value from the sensor  
  
  unsigned long epochTime = timeClient.getEpochTime(); // get time
  struct tm *ptm = gmtime ((time_t *)&epochTime);
  
  String HttpDate = String(ptm->tm_year+1900) + "-" + String(ptm->tm_mon+1) + "-" + String(ptm->tm_mday);

  String Data = "fkeyLimsDevice=8&timestamp=" + String(HttpDate) + " " + timeClient.getFormattedTime() + "&sensorValue=" + String(sensorValue);

  String host = "http://labinformatics.gearhostpreview.com/v1";
  String uri = "lims-timeseries.php";

  http.begin(wifiClient, host + "/" + uri);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  int httpCode = http.POST(Data);
 
}
