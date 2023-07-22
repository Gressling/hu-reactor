/* Example program for using TCS3200 color sensor
 * author: Panjkrc
 * date: 12/14/2019
 * url: https://github.com/Panjkrc/TCS3200_library
 */

#include <tcs3200.h>    // Include TCS3200 library 

#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <MySQL_Generic.h>
#include "WiFiCredentials.h"
#include "DBCredentials.h"


MySQL_Connection conn((Client *)&client);
MySQL_Query *query_mem;
// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 3600*2);

//Week Days
String weekDays[7]={"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

//Month names
String months[12]={"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};


#define num_of_colors 7   // Declares the number of colors the program can recognise (number of calibrated colors)

int S0 = D1;
int S1 = D4;
int S2 = D3;
int S3 = D6;
//int OE = D6;
int OUT = D7;
int button1 = D5;
int button2 = D2;
int map_from_c = 100;
int map_to_c = 0;
int dark_calibrate_c = 100;
int map_from_r = 100;
int map_to_r = 0;
int dark_calibrate_r = 100;
int map_from_b = 100;
int map_to_b = 0;
int dark_calibrate_b = 100;
int map_from_g = 100;
int map_to_g = 0;
int dark_calibrate_g = 100;


int red, green, blue, clear, n_repeat;
float red_normalized, green_normalized, blue_normalized;
int clear_duration, clear_duration_sum, clear_duration_avg;
int red_duration, red_duration_sum, red_duration_avg;
int blue_duration, blue_duration_sum, blue_duration_avg;
int green_duration, green_duration_sum, green_duration_avg;
int button1_pressed, button2_pressed;
long c_min, c_max, r_min, r_max, b_min,b_max,g_min,g_max;



String INSERT_SQL = "";

void setup() {

  pinMode(S0,OUTPUT);
  pinMode(S1,OUTPUT);
  //pinMode(OE,OUTPUT);

  pinMode(OUT,INPUT);
  pinMode(S2,OUTPUT);
  digitalWrite(S2,HIGH);
  pinMode(S3,OUTPUT);

  pinMode(button1,INPUT);
  pinMode(button2,INPUT);

  
  Serial.begin(9600);  
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
  }
  Serial.println("");
  Serial.println("Serial Console is ready");
  Serial.println("");


 Serial.println("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  timeClient.begin();
  Serial.println("");
  Serial.print("Connecting to SQL...  ");
  WiFi.hostByName(hostname, server_ip);
  if (conn.connect(server_ip, 3306, user, password_db)) {
    Serial.println("OK.");
  }
  else{
    Serial.println("FAILED.");
  }
  // create MySQL cursor object

  /*
  digitalWrite(S2,HIGH);
  digitalWrite(S3,LOW);
  dark_calibrate_c = pulseIn(OUT,LOW);
  c_min = 0;
  c_max = dark_calibrate_c;
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  dark_calibrate_r = pulseIn(OUT,LOW);
  r_min = 0;
  r_max = dark_calibrate_r;
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  dark_calibrate_b = pulseIn(OUT,LOW);
  b_min = 0;
  b_max = dark_calibrate_b;
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  dark_calibrate_g = pulseIn(OUT,LOW);
  g_min = 0;
  g_max = dark_calibrate_g;*/
  digitalWrite(S0,HIGH);
  digitalWrite(S1,HIGH);






}

void loop() {

  button1_pressed = digitalRead(button1);
  button2_pressed = digitalRead(button2);

  if(button1_pressed == 1){
    Serial.println("Calibrate to calibration standard (calibration solution)");
    delay(100);

    c_max = dark_calibrate_c;
    digitalWrite(S2,HIGH);
    digitalWrite(S3,LOW);
    c_min = pulseIn(OUT,LOW);

    r_max = dark_calibrate_r;
    digitalWrite(S2,LOW);
    digitalWrite(S3,LOW);
    r_min = pulseIn(OUT,LOW);
    

    b_max = dark_calibrate_b;
    digitalWrite(S2,LOW);
    digitalWrite(S3,HIGH);
    b_min = pulseIn(OUT,LOW);
    

    g_max = dark_calibrate_g;
    digitalWrite(S2,HIGH);
    digitalWrite(S3,HIGH);
    g_min = pulseIn(OUT,LOW);

    button1_pressed = 0;
  } else if(button2_pressed == 1){
    Serial.println("Push to Database");
  
    timeClient.update();
    unsigned long epochTime = timeClient.getEpochTime();

    struct tm *ptm = gmtime ((time_t *)&epochTime);
    int currentYear = ptm->tm_year;
    String date = String(currentYear+1900)  
      + "-" + String(ptm->tm_mon+1) 
      + "-" + String(ptm->tm_mday) 
      + " " + timeClient.getFormattedTime();
    INSERT_SQL = "INSERT INTO situation.rigl_experiment (limsDevice, date, clear, red, blue, green) VALUES (9, \"" + date + "\", " + String(clear) + ", " + String(red) + ", " + String(blue) + ", " + String(green) + ");";
    Serial.println(INSERT_SQL);
    MySQL_Query query_mem = MySQL_Query(&conn);
    if ( !query_mem.execute(INSERT_SQL.c_str()) )
    {
      MYSQL_DISPLAY("Insert error");
    }
    else
    {
      MYSQL_DISPLAY("Data Inserted.");
    }


    delay(1000);
    button2_pressed == 0;
  }
  else{

    n_repeat = 10;

    clear_duration_sum = 0;
    red_duration_sum = 0;
    blue_duration_sum = 0;
    green_duration_sum = 0;

    for (int ct=0; ct<n_repeat; ct++) {
      digitalWrite(S2,HIGH);
      digitalWrite(S3,LOW);
      clear_duration = map(pulseIn(OUT,LOW),c_min,c_max,map_from_c,map_to_c);
      clear_duration_sum += clear_duration;

      digitalWrite(S2,LOW);
      digitalWrite(S3,LOW);
      red_duration = map(pulseIn(OUT,LOW),r_min,r_max,map_from_r,map_to_r);
      red_duration_sum += red_duration;


      digitalWrite(S2,LOW);
      digitalWrite(S3,HIGH);
      blue_duration = map(pulseIn(OUT,LOW),b_min,b_max,map_from_b,map_to_b);
      blue_duration_sum += blue_duration;
      

      digitalWrite(S2,HIGH);
      digitalWrite(S3,HIGH);
      green_duration = map(pulseIn(OUT,LOW),g_min,g_max,map_from_g,map_to_g);
      green_duration_sum += green_duration;

    }



    clear_duration_avg = clear_duration_sum / n_repeat;
    clear = (clear_duration_avg);
    Serial.print("CLEAR_miliSec:");
    Serial.print(clear);
    Serial.print(",");
    Serial.print("CLEAR_result:");
    Serial.println(clear/clear);
    //delay(100);


    red_duration_avg = red_duration_sum / n_repeat;
    red = (red_duration_avg);
    red_normalized = (float)clear/(float)red;
    Serial.print("red:");
    Serial.print(red);
    Serial.print(",");
    Serial.print("red_normalized:");
    Serial.println(red_normalized);
    //delay(100);

    blue_duration_avg = blue_duration_sum / n_repeat;
    blue = (blue_duration_avg);
    blue_normalized = (float)clear/(float)blue;
    Serial.print("blue:");
    Serial.print(blue);
    Serial.print(",");
    Serial.print("blue_normalized:");
    Serial.println(blue_normalized);
    //delay(100);
    
    green_duration_avg = green_duration_sum / n_repeat;
    green = (green_duration_avg);
    green_normalized = (float)clear/(float)green;
    Serial.print("green:");
    Serial.print(green);
    Serial.print(",");
    Serial.print("green_normalized:");
    Serial.println(green_normalized);

    delay(200);

  }

}
