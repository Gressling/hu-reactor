#if !defined(ESP8266)
 #error This code is designed to run on ESP8266 and ESP8266-based boards! Please check your Tools->Board setting.
#endif

// These define's must be placed at the beginning before #include "ESP8266TimerInterrupt.h"
// _TIMERINTERRUPT_LOGLEVEL_ from 0 to 4
// Don't define _TIMERINTERRUPT_LOGLEVEL_ > 0. Only for special ISR debugging only. Can hang the system.

#define TIMER_INTERRUPT_DEBUG         1
#define _TIMERINTERRUPT_LOGLEVEL_     1
// Select a Timer Clock
#define USING_TIM_DIV1                false           // for shortest and most accurate timer
#define USING_TIM_DIV16               false           // for medium time and medium accurate timer
#define USING_TIM_DIV256              true            // for longest timer but least accurate. Default
#include "ESP8266TimerInterrupt.h"

#define BUILTIN_LED     2       // Pin D4 mapped to pin GPIO2/TXD1 of ESP8266, NodeMCU and WeMoS, control on-board LED
volatile bool statusLed = false;
volatile uint32_t lastMillis = 0;
#define TIMER_INTERVAL_MS 1000
ESP8266Timer ITimer;

void IRAM_ATTR TimerHandler(){
 static bool started = false;
 if (!started) {
   started = true;
   pinMode(BUILTIN_LED, OUTPUT);
 }
 digitalWrite(BUILTIN_LED, statusLed);  //Toggle LED Pin
 statusLed = !statusLed;
#if (TIMER_INTERRUPT_DEBUG > 0)
 Serial.println("Delta ms = " + String(millis() - lastMillis));
 lastMillis = millis();
#endif
}

void setup(){
 Serial.begin(115200);
 while (!Serial);
 delay(300);
 Serial.print(F("\nStarting TimerInterruptTest on "));  
 Serial.println(ARDUINO_BOARD);
 Serial.println(ESP8266_TIMER_INTERRUPT_VERSION);
 Serial.print(F("CPU Frequency = ")); Serial.print(F_CPU / 1000000);  
 Serial.println(F(" MHz"));
// Interval in microsecs
 if (ITimer.attachInterruptInterval(TIMER_INTERVAL_MS * 1000, TimerHandler)){
   lastMillis = millis();
   Serial.print(F("Starting ITimer OK, millis() = ")); 
   Serial.println(lastMillis);
 }
 else
   Serial.println(F("Can't set ITimer correctly. Select another freq. or interval"));
}

void loop()
{
   Serial.println(lastMillis);
   delay(560);
}
