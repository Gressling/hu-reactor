#include "Input.h"
#include "PinLayout.h"
#include <Keypad.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

const byte COLS = 4;
const byte ROWS = 4;

char hexaKeys[ROWS][COLS] = {
  {'1','2','3', 'A'},
  {'4','5','6', 'B'},
  {'7','8','9', 'C'},
  {'*','0','#', 'D'}
};

Keypad Tastenfeld = Keypad(makeKeymap(hexaKeys), ROW_PINS, COL_PINS, ROWS, COLS);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

String Input::getInput(String inputText) {
  delay(100); // For serial Monitors
  
  String input = "";
  
  Serial.begin(115200);
  Serial.println("Bitte eingeben, " + inputText);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  delay(500);

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.println(inputText);
  display.display();

  while (true) {
    char key = Tastenfeld.getKey();

    if (key) {
      if (key == '#') {
        break;
      } else {
        input += key;
  
        display.print(key); 
        display.display();
  
        Serial.println(input);
      }
    }

    yield();
  }

  return input;
}

void Input::message(String message) {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.println(message);
  display.display();
}
