#include "properties.h"
#include "WIFI.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

void WIFI::post(){

  WiFiClient wifiClient;

  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    String postData;
    postData.concat("val1=");
    postData.concat(VAL1);
    postData.concat("&val2=");
    postData.concat(VAL2);

    // Specify the server and endpoint
    String url;
    url.concat(HOST);
    url.concat(VERSION);
    url.concat(TEST);
    http.begin(wifiClient, url);

    // Set the content type header
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    // Send the HTTP POST request
    int httpResponseCode = http.POST(postData);
    // Check the response code
    if (httpResponseCode > 0) {
      Serial.print("HTTP POST request sent. Response code: ");
      Serial.println(httpResponseCode);

      // Get the response body
      String responseBody = http.getString();
      Serial.println("Response body: " + responseBody);
    } else {
      Serial.print("Error sending HTTP POST request. Error code: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  }

}