#include "Input.h"

int ENABLE = D8; //Motor Driver
//IN1 => G
//IN2 => 3V
//ENA => D8
float conversionRate = 60; // ml per minute

void setup() {
  Serial.begin(115200);
  delay(500);

  pinMode(ENABLE, OUTPUT);
  //pinMode(INP, OUTPUT);
  //digitalWrite(INP, HIGH);
  digitalWrite(ENABLE, LOW);  

}

Input inputHandler;
bool noTime = false;

void loop() 
{

  String ml_string = inputHandler.getInput("Menge\n(in ml):");
  Serial.println("ml: " + ml_string);

  float ml = ml_string.toFloat();
  float minTime = ml / conversionRate * 60; // Time in seconds
  float ratio = 0;
  float realTime = 0;
  
  inputHandler.message("Mindest-\nzeit:\n" + String(minTime) + " s");
  Serial.println("Mindestzeit:" + String(minTime));
  delay(1000);

  while(true) {
    String time_string = inputHandler.getInput("Zeit\n(in s):");
    Serial.println("Zeit: " + time_string);
    realTime = time_string.toFloat();

    if(time_string.equals("")) {
      noTime = true;
      break;
    }
    
    if(realTime >= minTime) { // Enough time
      noTime = false;
      ratio = minTime / realTime;
      break;
    }

    inputHandler.message("Nicht gen-\nug Zeit!");
    Serial.println("Nicht genug Zeit!");
    delay(1000);
    inputHandler.message("Mindest-\nzeit:\n" + String(minTime) + " s");
    delay(1000);
  }

  inputHandler.message("Laeuft ...");

  if(noTime) {
    Serial.println("Start");
    digitalWrite(ENABLE, HIGH);
    for(int i = 0; i < minTime; i++) {
      inputHandler.message("Zeit\nuebrig:\n" + String(minTime - i) + "s");
      delay(1000);


    } 
    digitalWrite(ENABLE, LOW);  
    Serial.println("End");
  } else {
    
    for(int i = 0; i < realTime; i++) {
      Serial.println("Start");
      // Motor starts
      digitalWrite(ENABLE, HIGH);

      inputHandler.message("Zeit\nuebrig:\n" + String(realTime - i) + "s");
      delay(ratio * 1000);
      Serial.println("Ende");
      // Motor stops
      digitalWrite(ENABLE, LOW);

      delay(1000 - ratio * 1000);
      // Waits remaining time
    }

  }

  inputHandler.message("Fertig.");
  delay(1500);

}
