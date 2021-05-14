/*
robotStreamIdpcf8574

This sketch sends data via HTTP GET requests to data.sparkfun.com service.

You need to get streamId and privateKey at data.sparkfun.com and paste thembelow. 
Or just customize this script to talk to other HTTP servers.

 https://www.esp8266.com/viewtopic.php?p=13812#


I added code from this page..
arduinojson.org/v6/example/http-client/

YouTube video weather sketch that uses json
youtube.com/watch?v=3U-qbii1saw

arduinojson.org/v6/assistant/


 */
// ========= Version settings ============
// esp8266       v2.6.3
// ArduinoJson   v6.17.2
// PCF8574       v2.2.2 reef
// ========= Version settings ============
/* 
 * 
 * This is the bot under test:
 * robotic.icu/robot.php?id=934661423
 * 
 * This is the bot json code:
 * robotic.icu/robotid.php?id=934661423
 * 
 * 
 */
const char* host = "www.robotic.icu";
const char* streamId = "xxxxxxxxx";
//const char* DeviceID = "144221858";// Space Command
/* 
144221858 519994052 849081125 383085317 477383611 240527013 
934661423 907864801 168036030 184884263
876975430 992438994 866880631 611452822 286531102 
112518130 368141751 368141751 770623542 749236437 
513739377 471922699 233683192 417658552 
*/

int baud = 115200;



#include <Logins.h>
/* ====  store content below in: libraries/Logins/Logins.h
char ssid[] = "xxxxxxxxxx";// your network SSID (name)
char pass[] = "xxxxxxxxxx";// your network password
==== */




/* ====== PCF8574 Setup ====== */
#include <Arduino.h>
#include <PCF8574.h>

// Set i2c address
PCF8574 pcf8574(0x24);//20-27 39
/* ====== PCF8574 Setup ====== */


#include <ArduinoJson.h>

/* ====== Works, but not best solution ====== */
// xxxxxxxxxxxx
// this is a work around(cheat) that works 
// if you remember to use pin 16 on the NodeMCU 
// during upload but I would like all boards to 
// adopt a standardized board specific identifier

#if LED_BUILTIN == 16 
  //
  int unit = 2;
//  #include <ESP8266WiFi.h>
  const char* ledBI = "16 ]";
  const char* device = "Breadboard esp8266";
  const char* wifiID = "Mad_Scientists_Workbench_8Relays";// no spaces
  const char* board = "esp8266 on breadboard";
  #define DHTPIN D6
  //  const char* DeviceID = "477383611";// LivingRoom
  const char* DeviceID = "144221858";// Relays
#else
  //
  int unit = 1;
//  #include <ESP8266WiFi.h>
  const char* ledBI = "2 ]";
  const char* device = "D1 Mini";
  const char* wifiID = "WeatherStation__Driveway";// no spaces
  const char* board = "D1 Mini on Window Sill";
  #define DHTPIN D7 //D7 D5 
  //
  const char* DeviceID = "849081125";// Space Command Driveway
#endif
/* ====== Works, but not best solution ====== */


#ifdef ESP8266
#include <ESP8266WiFi.h>
//#include <ESP8266HTTPClient.h>
//#include <ESP8266Ping.h>
#elif ESP32
#include <WiFi.h>
//#include "SPIFFS.h"
//#include <HTTPClient.h>
#endif

/* ====== int Setup ====== */
int SerialDebugging = 0;

#define Relay1 D5
#define Relay2 D6
int LED1   = 16 ;// 2 16
int LED2   = 2 ;// 2 16
int i =0;


int Bar = 0;
float range = 15;

int State = 1;
//static int savedState = 1;
 int savedState = 1;

 const char * Status;

/* ====== int Setup ====== */





/* ====== OLED Setup ====== */
#include <SPI.h>
#include <Wire.h>
int rotation=0; // display upright horizontal
  #include <Adafruit_SSD1306.h>
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 64, &Wire);// 32 or 64 dot mode
#include <Adafruit_GFX.h>

// OLED display TWI address
#define OLED_ADDR    0x3C
//Adafruit_SSD1306 display(-1);

#if (SSD1306_LCDHEIGHT != 32)
//#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif
/* ====== OLED Setup ====== */




void setup() {


  digitalWrite(Relay1, LOW);// turn off
       pinMode(Relay1, OUTPUT);
  digitalWrite(Relay2, LOW);// turn off
       pinMode(Relay2, OUTPUT);



  
  digitalWrite(LED1, LOW);// turn off
  pinMode(LED1, OUTPUT);
  digitalWrite(LED2, LOW);// turn off
  pinMode(LED2, OUTPUT);
  //
  digitalWrite(LED_BUILTIN, LOW);// turn off
  //
  pinMode(LED_BUILTIN, OUTPUT);
  /* ====== setup section OLED ====== */
  // initialize and clear display
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.setRotation(rotation);// h 0,2  v 1,3
  /* ====== setup section OLED ====== */

  display.clearDisplay();
  // display a line of text
  display.setTextColor(WHITE,BLACK);
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("DeviceID: ");
  display.println(DeviceID);
  //display.setTextSize(2);
  //display.setCursor(15,10);
  //display.println(ssid);
  //display.println(password);
  display.display();










  
  Serial.begin(baud);
  delay(10);


  // Set pinMode to OUTPUT
  pcf8574.pinMode(P0, OUTPUT);
  pcf8574.pinMode(P1, OUTPUT);
  pcf8574.pinMode(P2, OUTPUT);
  pcf8574.pinMode(P3, OUTPUT);
  pcf8574.pinMode(P4, OUTPUT);
  pcf8574.pinMode(P5, OUTPUT);
  pcf8574.pinMode(P6, OUTPUT);
  pcf8574.pinMode(P7, OUTPUT);
  Serial.print("Init pcf8574...");
  if (pcf8574.begin()){
    Serial.println("OK");
  }else{
    Serial.println("KO");
  }





  

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
    WiFi.hostname(wifiID);
 
  WiFi.begin(ssid, pass);
 
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    delay(500);
    Serial.print(".");
  }
  digitalWrite(LED_BUILTIN, HIGH);// turn off

  Serial.println("");
  Serial.println("WiFi connected"); 
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

int value = 0;

void loop() {
  delay(500);//100-1000
  ++value;

/* ============= voltmeter input =================== */
  // read the input on analog pin 0:
   float sensorValue = analogRead(A0);
//  float Level = analogRead(A0);
  
  /* Convert the analog reading 
     (which goes from 0 - 1023)
     to a voltage (0 - 5V):
   */

 // float Level = (map(bat, 0, 1023, 0, 5000))/1;
   float Level = sensorValue * (range / 1023.00);
   //float Bar=(Level/range)*(display.width());
   //float Percent=(Level/range)*100;
   //float Level = (map(bat, 0, 1023, 0, 5000))/1;
   float batLev=(Level/range)*100;
    Serial.print("batLev:");
    Serial.print(batLev);
    Serial.print("   millis:");
    Serial.println(millis());
/* ============= voltmeter input =================== */


  //Serial.print("connecting to ");
  //Serial.println(host);
 
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
 
  // We now create a URI for the request
  String url = "/robotid.php";
  //url += streamId;
  url += "?id=";
  url += DeviceID;
  url += "&online=";
  url += Status;
  url += "&batLevel=";
  url += batLev;
  //url += "Busy";
 
  //Serial.print("Requesting URL: ");
  //Serial.println(url);
 
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  delay(1);





/*================= Added this from 2nd site ================= */
  // Check HTTP status
  char status[32] = {0};
  client.readBytesUntil('\r', status, sizeof(status));
  if (strcmp(status, "HTTP/1.1 200 OK") != 0) {
    Serial.print(F("Unexpected response: "));
    //Serial.println(status);
    return;
  }

  // Skip HTTP headers
  char endOfHeaders[] = "\r\n\r\n";
  if (!client.find(endOfHeaders)) {
    Serial.println(F("Invalid response"));
    return;
  }
/*================= Added this from 2nd site ================= */

/*
https://www.youtube.com/watch?v=dQyXuFWylm4


    //for(int count=1;count <17; count++){
    //Serial.println(count);
    //}
*/

 
    //Serial.println("debug 1");
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    //Serial.print("debug ");
    String line = client.readStringUntil('\n');
    //    Serial.println(line);


    const size_t capacity = JSON_OBJECT_SIZE(22) + 1024;
DynamicJsonDocument doc(capacity);

/*
const char* json = "{\"id\":\"658435603\",\"admin\":\"123456789\",\"user\":\"\",\"name\":\"Prof Ferber Farber\",
\"description\":\"Robotics Debokler\",\"location\":\"Laboratory\",\"type\":\"\",\"controlpanel\":\"8nixi\",
\"access\":\"\",\"online\":\"Online\",\"image\":\"GoFigure.jpg\",\"ip\":\"\",\"batLevel\":\"75\",
\"B1\":\"\",\"B2\":\"\",\"B3\":\"\",\"B4\":\"\",\"B5\":\"\",\"B6\":\"\",\"B7\":\"\",\"B8\":\"\",\"message\":\"Hello mom!\"}";

*/ 
deserializeJson(doc, line);

const char * id = doc["id"]; // "658435603"
const char * admin = doc["admin"]; // "123456789"
const char * user = doc["user"]; // ""
const char * name = doc["name"]; // "Prof Ferber Farber"
const char * description = doc["description"]; // "Robotics Debokler"
const char * location = doc["location"]; // "Laboratory"
const char * type = doc["type"]; // ""
const char * controlpanel = doc["controlpanel"]; // "8nixi"
const char * access = doc["access"]; // ""
const char * online = doc["online"]; // "Online"
const char * image = doc["image"]; // "GoFigure.jpg"
const char * ip = doc["ip"]; // ""
const char * batLevel = doc["batLevel"]; // "75"
int T0 = doc["B1"]; // ""
int T1 = doc["B2"]; // ""
int T2 = doc["B3"]; // ""
int T3 = doc["B4"]; // ""
int T4 = doc["B5"]; // ""
int T5 = doc["B6"]; // ""
int T6 = doc["B7"]; // ""
int T7 = doc["B8"]; // ""
int validData = doc["bit"]; // ""
const char * message = doc["message"]; // "Hello mom!"


    int startTime = millis();


    if (controlpanel != "meter") {
      if (validData == 1) {
        //digitalWrite(Relay1, T0);// D5
        //digitalWrite(Relay2, T1);// D4

        pcf8574.digitalWrite(P0, !T0);
        pcf8574.digitalWrite(P1, !T1);
        pcf8574.digitalWrite(P2, !T2);
        pcf8574.digitalWrite(P3, !T3);
        pcf8574.digitalWrite(P4, !T4);
        pcf8574.digitalWrite(P5, !T5);
        pcf8574.digitalWrite(P6, !T6);
        pcf8574.digitalWrite(P7, !T7);
        Status = (T0?"Online":"Offline");

        if(T7){SerialDebugging = 1;}else{SerialDebugging = 0;}

        
        if (SerialDebugging == 1) {
          digitalWrite(LED1,!digitalRead(LED1));
        }else{
          digitalWrite(LED1, !T2);
        }
        digitalWrite(LED2, !T3);
        displayStat(T0, T1, T2, T3, T4, T5, T6, T7, name, description);
         
        if (SerialDebugging == 1) {
         Serial.print("Relay1 "); Serial.print((T0?"On":"Off"));
         Serial.print("\tRelay2 "); Serial.print((T1?"On":"Off"));
         Serial.print("\tStatus "); Serial.println(Status);
         //digitalWrite(LED2,!digitalRead(LED2));
        }
      }
    }




    if (controlpanel == "meter") {
      if (validData == 1) { 
        digitalWrite(Relay1, T0);// D5
        digitalWrite(Relay2, T1);// D4
        digitalWrite(LED1, !T1);
        digitalWrite(LED2, !T2);
        //displayBatt(T0, T1, T2, T3, T4, T5, T6, batLevel, name, description);
      }  
    }  





      /*
 display.setTextColor(WHITE,BLACK);
  display.setTextSize(1);
  display.setCursor(0,10);
  display.println(name);
   display.print("T0:");
  display.print(T0);
   display.print("  T1:");
  display.print(T1);
 //  display.print("T2:");
  //display.println(T2);
  //display.print(" ");
  display.display();




    if (savedState != T0) { 
      digitalWrite(LED1, !T0); 
      savedState=T0;
      }  
    //    digitalWrite(LED1, !T0);
*/




//displayStat();

 /*
*/ 




  
  //display.print("LED_BUILTIN: ");
  //display.println(LED_BUILTIN);
  //
  //display.print(":");



/*

    if ( State == 0){
      State = 0; 
      display.print("Msg: P0");
    } else  { 
      State = 1; 
      display.print("Msg: T0");
    }
//display.print(str(name) );

*/


//const char *Buttons = T0 + T1 + T2 + T3 + T4 + T5 + T6 + T7;// wouldnt work
//if (Buttons != OldButtons){
//}


   // if (T0==0){ State = 0; } else  { State = 1; }
 // digitalWrite(LED1, State);//    LED_BUILTIN

 
//displayStat(T0, T1, online);



      //Serial.println(SerialDebugging);
      //Serial.print(b);
    if (SerialDebugging==1){
    if (validData==1){
      Serial.print(">>> ");
      Serial.print(T0);
      Serial.print(" ");
      Serial.print(T1);
      Serial.print(" ");
      Serial.print(T2);
      Serial.print(" ");
      Serial.print(T3);
      Serial.print(" ");
      Serial.print(T4);
      Serial.print(" ");
      Serial.print(T5);
      Serial.print(" ");
      Serial.print(T6);
      Serial.print(" ");
      Serial.print(T7);
      Serial.print(" ");
      Serial.println(message);
    //Serial.println((T7 ? "ON" : "OFF"));
    }
    }
    //#endif



    if (T7 == 2) { 
Serial.print(">>> ");
Serial.print(T0);
Serial.print(" ");
Serial.print(T1);
Serial.print(" ");
Serial.print(T2);
Serial.print(" ");
Serial.print(T3);
Serial.print(" ");
Serial.print(T4);
Serial.print(" ");
Serial.print(T5);
Serial.print(" ");
Serial.print(T6);
Serial.print(" ");
Serial.print(T7);
Serial.print(" ");
Serial.println(message);
    }
    int difTime = millis()-startTime;
    //Serial.println(difTime);
  delay(10);
  }

  //Serial.println("closing connection");
}


void displayStat(int T0, int T1, int T2, int T3, int T4, int T5, int T6, int T7, const char *name, const char *description) {
//const  char* mess = description;
//char dest[21];
//strncpy(dest,mess,20);


  display.fillRect(0, 8, 128, 28, BLACK);
  
  display.setTextColor(WHITE,BLACK);
  display.setTextSize(1);
  display.setCursor(0,8);
  display.println(name);
  display.println(description);
  display.setCursor(15,25);
   //   display.print("P: ");
if(T0){
  display.setTextColor(BLACK,WHITE);
  display.print("1");
}else{
  display.setTextColor(WHITE,BLACK);
  display.print("1");
} 
  display.setTextColor(WHITE,BLACK);display.print(" ");
if(T1){
  display.setTextColor(BLACK,WHITE);
  display.print("2");
}else{
  display.setTextColor(WHITE,BLACK);
  display.print("2");
} 
  display.setTextColor(WHITE,BLACK);display.print(" ");
if(T2){
  display.setTextColor(BLACK,WHITE);
  display.print("3");
}else{
  display.setTextColor(WHITE,BLACK);
  display.print("3");
} 
  display.setTextColor(WHITE,BLACK);display.print(" ");
if(T3){
  display.setTextColor(BLACK,WHITE);
  display.print("4");
}else{
  display.setTextColor(WHITE,BLACK);
  display.print("4");
} 
  display.setTextColor(WHITE,BLACK);display.print(" ");
if(T4){
  display.setTextColor(BLACK,WHITE);
  display.print("5");
}else{
  display.setTextColor(WHITE,BLACK);
  display.print("5");
} 
  display.setTextColor(WHITE,BLACK);display.print(" ");
if(T5){
  display.setTextColor(BLACK,WHITE);
  display.print("6");
}else{
  display.setTextColor(WHITE,BLACK);
  display.print("6");
} 
  display.setTextColor(WHITE,BLACK);display.print(" ");
if(T6){
  display.setTextColor(BLACK,WHITE);
  display.print("7");
}else{
  display.setTextColor(WHITE,BLACK);
  display.print("7");
} 
  display.setTextColor(WHITE,BLACK);display.print(" ");
if(T7){
  display.setTextColor(BLACK,WHITE);
  display.print("8");
}else{
  display.setTextColor(WHITE,BLACK);
  display.print("8");
} 
  display.setTextColor(WHITE,BLACK);
   
display.display();
}


void displayBatt(int T0, int T1, int T2, int T3, int T4, int T5, int T6, int batLevel, const char *name, const char *description) {
  display.setTextColor(WHITE,BLACK);
  display.setTextSize(1);
  display.setCursor(0,8);
  display.println(name);
  display.println(batLevel);
  display.setCursor(15,25);

   float Level = batLevel * (range / 1023.00);
   float Bar=(Level/range)*(display.width());
   float Percent=(Level/range)*100;



  

  display.setTextSize(2);
  display.setCursor(10,10);
  display.print(batLevel,5);// Level Bar Percent
  //display.println(" v");
  
  display.setTextSize(1);
  display.setCursor(0,20);
  display.print("0");//uid
  display.setCursor((display.width()-12),20);
  display.print(range);//uid

/* ===display bargraph== */
  display.drawLine(0, 27, display.width(), 27, WHITE);
  display.fillRect(0, 28, Bar, 3, WHITE);
  display.fillRect((Bar +1), 28, display.width(), 3, BLACK);
  display.drawLine(0, 31, display.width(), 31, WHITE);
  //display.drawRoundRect(17, 8, 93, 18, 4, BLACK);
  display.display();
}










//display.print(str(name) );
 // const char* OldButtons = Buttons;// LED_BUILTIN
  
