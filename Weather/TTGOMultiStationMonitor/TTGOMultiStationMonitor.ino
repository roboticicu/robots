/*
TTGOMultiStationMonitor
TTGOrobotStreamId

============= This is the TTGO DHT22 sketch =====================
============= This is the TTGO DHT22 sketch =====================
============= This is the TTGO DHT22 sketch =====================
============= This is the TTGO DHT22 sketch =====================
============= This is the TTGO DHT22 sketch =====================


bATTERY ON AMAZON





https://silverprice.org/silver-price-charts/30-day-silver-price-history-in-us-dollars-per-ounce
//*[@id="gpxtickerLeft_price"]






This sketch sends data via HTTP GET requests to data.sparkfun.com service.

You need to get streamId and privateKey at data.sparkfun.com and paste them
below. Or just customize this script to talk to other HTTP servers.
   
  https://www.esp8266.com/viewtopic.php?p=13812#


I added code from this page..
arduinojson.org/v6/example/http-client/
  
YouTube video weather sketch that uses json
youtube.com/watch?v=3U-qbii1saw
  
arduinojson.org/v6/assistant/


========= Version settings ============
TTGO T-Display  v2.3.4
esp8266         v2.6.3
ArduinoJson     v6.x
========= Version settings ============


This is the bot under test:
  robotic.icu/robot.php?id=934661423
  
This is the bot json code:
  robotic.icu/robotid.php?id=934661423

*/
const char* host = "www.robotic.icu";
const char* streamId = "xxxxxxxxx";
const char* wifiID = "SpongeBob";// no spaces


int station=1;
// 
int zone = 4;/* <=== Change this number to choose time zone. ====== */
struct zone {
  int offset;
  char *location;
};

const struct zone DeviceID[13] = {
  {0, ""}, 
  {-36000,"849081125"}, /*=== 1 === */
  {-21600,"477383611"}, /*=== 2 === */
  {-18000,"477383611"}, /*=== 3 === */
  {-14400,"477383611"}, /*=== 4 === */
  {-10800,"477383611"}, /*=== 5 === */
  {0000,  "477383611"}, /*=== 6 === */
  {3600,  "477383611"}, /*=== 7 === */
  {10800, "477383611"}, /*=== 8 === */
  {19800, "477383611"}, /*=== 9 === */
  {28800, "477383611"}, /*=== 10 === */
  {32400, "477383611"}, /*=== 11 === */
  {39600, "477383611"}}; /*=== 12 === */
/* 
849081125 519994052 383085317 477383611 240527013 
934661423 907864801 168036030 184884263 876975430 
992438994 866880631 611452822 286531102 112518130 
368141751 368141751 770623542 749236437 513739377 
471922699 233683192 417658552 


  DeviceID[station].location
const char* DeviceID[3];
const char* DeviceID[1] = "849081125";// Space Command
const char* DeviceID[2] = "477383611";// Space Command


*/

const char * dev = "temp";// temp message
int baud = 115200;
int rotation =1;// 1 Horizontal
int vOffset =20;

#include <Logins.h>
/* ====  store content below in: libraries/Logins/Logins.h
char ssid[] = "xxxxxxxxxx";// your network SSID (name)
char pass[] = "xxxxxxxxxx";// your network password
==== */




/* ====== IR Setup ====== */
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>
/* ====== IR Setup ====== */
// 
const char *lastmessage;
float lasttemp;
const char *lasthum;
const char *lastwind;


// float lastmessage;
int t, h, irChan, irVol;
String LastResult, LastAction;
int count = 0;




#include <ArduinoJson.h>

#ifdef ESP8266
const char* device = "esp8266";
const char* board = "esp8266 on breadboard";
#include <ESP8266WiFi.h>
//#include <ESP8266HTTPClient.h>
//#include <ESP8266Ping.h>
#elif ESP32
const char* device = "esp32";
const char* board = "TTGO";
#include <WiFi.h>
#include <TFT_eSPI.h>
//#include "SPIFFS.h"
//#include <HTTPClient.h>
#endif


#include <DHT.h>
#define DHTPIN 17
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);


TFT_eSPI tft = TFT_eSPI();



#define BUTTON1PIN 35
#define BUTTON2PIN 0

bool useFahrenheit       = true; // Default to Fahrenheit
bool showTemp            = true; // Default to Temp / Humidity
long lastDebounceButton1 = 0;    // Holds Button1 last debounce
long lastDebounceButton2 = 0;    // Holds Button2 last debounce
long debounceDelay       = 200;  // 200ms between re-polling

// INTRPT Function to execute when Button 1 is Pushed
void IRAM_ATTR toggleButton1() {
  if ((millis() - lastDebounceButton1) > debounceDelay) { 
       //station++; if(station > 2){station=1;}
      // if(station == 1){station=2;}else{station=1;}
    if (useFahrenheit){useFahrenheit = false;}
    else {useFahrenheit = true;}
    lastDebounceButton1 = millis();
  }
}

// INTRPT Function to execute when Button 2 is Pushed
void IRAM_ATTR toggleButton2() {
  if ((millis() - lastDebounceButton2) > debounceDelay) { 
    if (showTemp){showTemp = false;}
    else {showTemp = true;}
    lastDebounceButton2 = millis();
  }
}








int counter,lastcount;

/* ====== int Setup ====== */
int SerialDebugging = 0;

//#define Relay1 D5
//#define Relay2 D6
int LED1   = 16 ;// 2 16
int LED2   = 2 ;// 2 16
int i =0;


int Bar = 0;
float range = 15;

int State = 1;
//static int savedState = 1;
 int savedState = 1;

/* ====== int Setup ====== */





/* ====== OLED Setup ====== */
#include <SPI.h>
#include <Wire.h>

/*
// What to display if showTemp = true
void showScrn1() {
  float t = dht.readTemperature(useFahrenheit);
  float h = dht.readHumidity();
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setCursor(0, 30);
  tft.setFreeFont(&Orbitron_Light_24);
  tft.println("Temp     Humidity");
  tft.drawLine(0, 35, 250, 35, TFT_BLUE);
  tft.setCursor(0, 60);
  tft.print(t);
  if (useFahrenheit){tft.print(F("f"));}
  else {tft.print(F("c"));}
  tft.setCursor(130, 60);
  tft.print(h);tft.print(F("%"));
}

// What to display if showTemp = false
void showScrn2() {
  float t = dht.readTemperature(useFahrenheit);
  float h = dht.readHumidity();
  float hi = dht.computeHeatIndex(t, h, useFahrenheit);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_RED, TFT_BLACK);
  tft.setCursor(50, 30);
  tft.setFreeFont(&Orbitron_Light_24);
  tft.println("Heat Index");
  tft.drawLine(0, 35, 250, 35, TFT_BLUE);
  tft.setFreeFont(&Orbitron_Light_32);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setCursor(60, 100);
  tft.print(hi);
  if (useFahrenheit){tft.print(F("f"));}
  else {tft.print(F("c"));}
}
*/


/* ====== OLED Setup ====== */



/* ====== IR pins ====== */
// An IR detector/demodulator is connected to GPIO pin 14(D5 on a NodeMCU
// board).
// Note: GPIO 16 won't work on the ESP8266 as it does not have interrupts.
const uint16_t kRecvPin = 12;//14
IRrecv irrecv(kRecvPin);
decode_results results;
/* ====== IR pins ====== */

void bug(){
     while (count<1 ){
      Serial.print("== Board ==> ");Serial.println(board);
      //Serial.print("Board MAC Address:  ");
      //Serial.println(WiFi.macAddress());
      Serial.println();
      Serial.println();
      Serial.println();
      count++;
     }
}

void setup() {
     int rotation=1; // display upright horizontal


  pinMode(BUTTON1PIN, INPUT);
  pinMode(BUTTON2PIN, INPUT);
  attachInterrupt(BUTTON1PIN, toggleButton1, FALLING);
  attachInterrupt(BUTTON2PIN, toggleButton2, FALLING);


//  digitalWrite(Relay1, LOW);// turn off
//       pinMode(Relay1, OUTPUT);
//  digitalWrite(Relay2, LOW);// turn off
//       pinMode(Relay2, OUTPUT);



  
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
//.  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
//..  display.setRotation(rotation);// h 0,2  v 1,3
  /* ====== setup section OLED ====== */

  tft.init();
  tft.setRotation(rotation);
  tft.fillScreen(TFT_BLACK);
  tft.setFreeFont(&Orbitron_Light_24);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setCursor(0, 30);
  tft.println("DeviceID: ");
  tft.setCursor(0, 60);
  tft.print(DeviceID[station].location);
  delay(5000);
  //display.println(DeviceID);
  //display.setTextSize(2);
  //display.setCursor(15,10);
  //display.println(ssid);
  //display.println(password);
 // display.display();










  
  Serial.begin(baud);
  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
//  WiFi.hostname(wifiID);
//  WiFi.hostname("SpongeBob");
  WiFi.begin(ssid, pass);
 
  tft.fillScreen(TFT_BLACK);
  tft.setFreeFont(&Orbitron_Light_24);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setCursor(0, 30);
  tft.println("Login: ");
  
  login();

  Serial.println("");
  Serial.println("WiFi connected"); 
  //Serial.println("IP address: ");
  //Serial.println(WiFi.localIP());
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(50, 60);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.println("IP address: ");
  tft.setTextColor(TFT_ORANGE, TFT_BLACK);
  tft.print("   ");
  tft.println(WiFi.localIP());
//  tft.print("LED_BUILTIN: ");
//  tft.println(LED_BUILTIN);
  delay(5000);


/* ====== IR start ====== */

/* ====== IR start ====== */



  
}

void cls(){
  tft.fillScreen(TFT_BLACK);
}


/* ====== IR start ====== */

/* ====== Loss WiFi restart ====== */
void login(){
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    delay(500);
    //Serial.print(".");
  tft.setCursor(50, 60);
  tft.setTextColor(TFT_BLACK, TFT_BLACK);
            tft.print(lastcount);
  tft.setCursor(50, 60);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
            tft.print(counter);
            lastcount = counter;
            counter++;
            if(counter > 100){
              ESP.restart();
            }
  }
 // 
  digitalWrite(LED_BUILTIN, HIGH);// turn off  
}



//int value = 0;

void loop() {
  bug();
if (WiFi.status() != WL_CONNECTED) {
  counter = 1000;
    cls();
  login();
}
  
  delay(500);//100-1000
 // ++value;

/* ============= IR input =================== */
  if (irrecv.decode(&results)) {
    // print() & println() can't handle printing long longs. (uint64_t)
    //serialPrintUint64(results.value, HEX);

/*
    I can't get results.value in a form the OLED likes.
    Which I guess would be a problem for mysql too..
*/
    if (results.value == 0x2FDD827) {// Channel up
      irChan += 1; if(irChan > 99){irChan = 99;}
      LastAction = "Cup";
    }else if (results.value == 0x2FDF807) {// Channel dwn
      irChan -= 1; if(irChan <  0){irChan = 0;}
      LastAction = "Cdn";
    }else if (results.value == 0x2FD58A7) {// Vol up
      irVol += 1; if(irVol > 20){irVol = 20;}
      LastAction = "Vup";
    }else if (results.value == 0x2FD7887) {// Vol dwn
      irVol -= 1; if(irVol <  0){irVol = 0;}
      LastAction = "Vdn";
    }else if (results.value == 0xFFFFFFFFFFFFFFFF) {// Repeat
      LastResult = "FF";
    } 

  if(LastResult == "FF"){
      LastResult = "00";
      if(LastAction == "Cup"){
      irChan += 1; if(irChan > 99){irChan = 99;}
      }
      if(LastAction == "Cdn"){
      irChan -= 1; if(irChan <  0){irChan = 0;}
      }
      if(LastAction == "Vup"){
      irVol += 1; if(irVol > 20){irVol = 20;}
      }
      if(LastAction == "Vdn"){
      irVol -= 1; if(irVol <  0){irVol = 0;}
      }
  }
/* ============= IR input =================== */

  //Serial.println("closing connection");
    irrecv.resume();  // Receive the next value
  }

 

/* ============= voltmeter input =================== */
  // read the input on analog pin 0:
  float sensorValue = analogRead(A0);

  /* Convert the analog reading 
     (which goes from 0 - 1023)
     to a voltage (0 - 5V):
   */

 // float Level = (map(bat, 0, 1023, 0, 5000))/1;
   float Level = sensorValue * (range / 1023.00);
   //float Bar=(Level/range)*(display.width());
   //float Percent=(Level/range)*100;
   //float Level = (map(bat, 0, 1023, 0, 5000))/1;
   //float batLev=(Level/range)*100;
   // Serial.println(millis());
    //ttgoDisplay(const char *name, const char *message);
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
  url += DeviceID[station].location;
  url += "&batLevel=";
  url += t;// batLev irChan
 
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
    Serial.print(F("Unexpected response: \n"));
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
int P1 = doc["B1"]; // ""
int P2 = doc["B2"]; // ""
int P3 = doc["B3"]; // ""
int P4 = doc["B4"]; // ""
int P5 = doc["B5"]; // ""
int P6 = doc["B6"]; // ""
int P7 = doc["B7"]; // ""
int P8 = doc["B8"]; // ""
int validData = doc["bit"]; // ""
const char * message = doc["message"]; // "Hello mom!"

float Temp = doc["Temp"]; // "Hello mom!"
const char * Humidity = doc["Humidity"]; // "Hello mom!"
const char * Baro = doc["Baro"]; // "Hello mom!"
const char * windSpd = doc["windSpd"]; // "Hello mom!"
const char * windDir = doc["windDir"]; // "Hello mom!"
const char * PrecipVol = doc["PrecipVol"]; // "Hello mom!"
const char * PrecipType = doc["PrecipType"]; // "Hello mom!"
const char * Cloudiness = doc["Cloudiness"]; // "Hello mom!"

/*

*/

if(validData){
    if (showTemp){// Temp Humidity
      station = 1;
    if (useFahrenheit){// Temp Humidity
      //  name, message, batLevel, access, ip, location, rotation
      //  name, message, Temp, Humidity, windSpd, location, rotation
      ttgoDisplay( name, message, Temp, Humidity, windSpd, location, rotation, count);// === This one is it =====
    }else{
      ttgoDisplayMess( name, message, Temp, location, rotation);// === This one is it =====
    }
    }else{
      station = 2;
    if (useFahrenheit){// Temp Humidity
      ttgoDisplay( name, message, Temp, Humidity, windSpd, location, rotation, count);// === This one is it =====
    }else{
      ttgoDisplayMess( name, message, Temp, location, rotation);// === This one is it =====
    }
    }

  
/*

    if (useFahrenheit){// Temp Humidity
      ttgoDisplay( name, message, batLevel, access, ip, location, rotation);// === This one is it =====
    }else{
      ttgoDisplayMess( name, message, batLevel, location, rotation);// === This one is it =====
    }
      ttgoDisplay( name, message, batLevel, access, ip, location, rotation);// === This one is it =====

*/

  
//    Serial.print("Temp:");
//    Serial.print(batLevel);// batLev
//    Serial.print(" F :");
//    
//    Serial.print("showTemp >");
//    Serial.print(showTemp);// batLev
//    Serial.println(" <");





//    
//    if(dev == "temp") {
//      ttgoDisplay( name, message, batLevel, ip, location, rotation);// === This one is it =====
//    }else{
//      ttgoDisplayMess( name, message, batLevel, location, rotation);// === This one is it =====
//    }
}

//.**   












    int startTime = millis();


    if (controlpanel != "meter") {
      if (validData == 1) {
           //  if(message != lastmessage){
           //  tft.fillScreen(TFT_BLACK);
           //  lastmessage = message;
           //  }


        
//        digitalWrite(Relay1, P1);// D5
//        digitalWrite(Relay2, P2);// D4

        if(P8){SerialDebugging = 1;}else{SerialDebugging = 0;}

        
        if (SerialDebugging == 1) {
          digitalWrite(LED1,!digitalRead(LED1));
        }else{
          digitalWrite(LED1, !P3);
        }
        digitalWrite(LED2, !P4);
     //.   displayStat(P1, P2, P3, P4, P5, P6, P7, P8, name, description);

         
        if (SerialDebugging == 1) {
         Serial.print("Relay1 "); Serial.print((P1?"On":"Off"));
         Serial.print("\tRelay2 "); Serial.println((P2?"On":"Off"));
         //digitalWrite(LED2,!digitalRead(LED2));
        }
      }
    }




    if (controlpanel == "meter") {
      if (validData == 1) { 
//        digitalWrite(Relay1, P1);// D5
//        digitalWrite(Relay2, P2);// D4
        digitalWrite(LED1, !P2);
        digitalWrite(LED2, !P3);
        //displayBatt(P1, P2, P3, P4, P5, P6, P7, batLevel, name, description);
      }  
    }  





      /*
 display.setTextColor(WHITE,BLACK);
  display.setTextSize(1);
  display.setCursor(0,10);
  display.println(name);
   display.print("P1:");
  display.print(P1);
   display.print("  P2:");
  display.print(P2);
 //  display.print("P3:");
  //display.println(P3);
  //display.print(" ");
  display.display();




    if (savedState != P1) { 
      digitalWrite(LED1, !P1); 
      savedState=P1;
      }  
    //    digitalWrite(LED1, !P1);
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
      display.print("Msg: P1");
    }
//display.print(str(name) );

*/


//const char *Buttons = P1 + P2 + P3 + P4 + P5 + P6 + P7 + P8;// wouldnt work
//if (Buttons != OldButtons){
//}


   // if (P1==0){ State = 0; } else  { State = 1; }
 // digitalWrite(LED1, State);//    LED_BUILTIN

 
//displayStat(P1, P2, online);



      //Serial.println(SerialDebugging);
      //Serial.print(b);
    if (SerialDebugging==1){
    if (validData==1){
      Serial.print(">>> ");
      Serial.print(P1);
      Serial.print(" ");
      Serial.print(P2);
      Serial.print(" ");
      Serial.print(P3);
      Serial.print(" ");
      Serial.print(P4);
      Serial.print(" ");
      Serial.print(P5);
      Serial.print(" ");
      Serial.print(P6);
      Serial.print(" ");
      Serial.print(P7);
      Serial.print(" ");
      Serial.print(P8);
      Serial.print(" ");
      Serial.println(message);
    //Serial.println((P8 ? "ON" : "OFF"));
    }
    }
    //#endif



    if (P8 == 2) { 
Serial.print(">>> ");
Serial.print(P1);
Serial.print(" ");
Serial.print(P2);
Serial.print(" ");
Serial.print(P3);
Serial.print(" ");
Serial.print(P4);
Serial.print(" ");
Serial.print(P5);
Serial.print(" ");
Serial.print(P6);
Serial.print(" ");
Serial.print(P7);
Serial.print(" ");
Serial.print(P8);
Serial.print(" ");
Serial.println(message);
    }
    int difTime = millis()-startTime;
    //Serial.println(difTime);
  delay(10);
  }







/*
There is a battle being fought over this nation. A battle between good and evil. There is someone running for president that wants to legalize everything that God calls sin. We need to stand up agains
What do you get when you cross a duck with an loaf of bread? Thanks Giving.
Charles Stanley 
Sunday 11:00am
ch. 17.1

*/


}

/*
const char * Temp = doc["Temp"]; // "Hello mom!"
const char * Humidity = doc["Humidity"]; // "Hello mom!"
const char * Baro = doc["Baro"]; // "Hello mom!"
const char * windSpd = doc["windSpd"]; // "Hello mom!"
const char * windDir = doc["windDir"]; // "Hello mom!"
const char * PrecipVol = doc["PrecipVol"]; // "Hello mom!"
const char * PrecipType = doc["PrecipType"]; // "Hello mom!"
const char * Cloudiness = doc["Cloudiness"]; // "Hello mom!"
*/
// int validData,// === This one is it =====
                                                   //  name, message, batLevel, access, ip, location, rotation
                                                   //  name, message, Temp, Humidity, windSpd, location, rotation
void ttgoDisplay( const char *name, const char *message, float Temp, const char * Humidity, const char * windSpd, const char * location, int rotation, int count){

       if(Temp != lasttemp){// message
        tft.fillScreen(TFT_BLACK);
      lasttemp = Temp;// message
     }
     
//if (validData){
    tft.setRotation(rotation);
 //  tft.fillScreen(TFT_BLACK);
  tft.setFreeFont(&Orbitron_Light_24);
      tft.setTextSize(0);
        tft.fillScreen(TFT_BLACK);// ========= comment this line out if scren flickers =========
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setCursor(0, vOffset);
            tft.print(location);

            // Temp
  tft.setCursor(0, 50);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
            tft.print(" Temp: ");
      tft.setTextColor(TFT_BLACK, TFT_BLACK);
      tft.println(lasttemp);
//      tft.println(" °F   ");
      tft.println(F(" °F"));
  tft.setCursor(0, 50);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
            tft.print(" Temp: ");
            if(Temp < 32.1){
              tft.drawRect(95, 28, 130, 28, TFT_ORANGE);
              //tft.fillRect(95, 28, 130, 28, TFT_ORANGE, TFT_GREY);
              tft.setTextColor(TFT_GOLD, TFT_BLACK);
            }else if(Temp > 73.70){
              tft.drawRect(95, 28, 130, 28, TFT_RED);
              tft.setTextColor(TFT_PINK, TFT_BLACK);
            }else{
              tft.setTextColor(TFT_YELLOW, TFT_BLACK);
            }
  
            tft.print(Temp);// message
      tft.println(F(" °F"));
            lasttemp = Temp;

            // Humid
  tft.setCursor(0, 80);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
            tft.print("Humid: ");
      tft.setTextColor(TFT_BLACK, TFT_BLACK);
      tft.println(lasthum);
      tft.println(" %   ");
  tft.setCursor(0, 80);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
            tft.print("Humid: ");
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
            tft.print(Humidity);// access
            tft.println(" %   ");
            lasthum = Humidity;// access

            // wind
  tft.setCursor(0, 110);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
            tft.print("Wind: ");
      tft.setTextColor(TFT_BLACK, TFT_BLACK);
      tft.println(lastwind);
      tft.println(" mph   ");
  tft.setCursor(0, 110);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
            tft.print("Wind: ");
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
            tft.print(windSpd);// ip
            tft.println(" mph   ");
            lastwind = windSpd;



 //   batLevel, access, ip,
 //   Temp, Humidity, windSpd,

            
    Serial.print(board);// board
    Serial.print(" | ");
    Serial.print(location);// Temp
    Serial.print("  Temp:");
    Serial.print(Temp);// Temp
    Serial.print(" °F ");
    Serial.print("Hum:");
    Serial.print(Humidity);// Humidity
    Serial.print(" % ");
    Serial.print("Wind:");
    Serial.print(windSpd);//  Wind Speed
    Serial.print(" mph |");
    Serial.print("| count: ");
    Serial.print(count);//
    Serial.print("\n");
//}
}

  //tft.setCursor(50, 85);
    //  tft.setTextSize(1);
  //tft.setCursor(50, 85);
//.  tft.drawRect(45, 55, 130, 40, TFT_ORANGE);
          // tft.println(name);
          //  tft.print("msg ");



// int validData,// === This one is it =====
                                                   //  name, message, batLevel, access, ip, location, rotation
                                                   //  name, message, Temp, Humidity, windSpd, location, rotation
void ttgoDisplayMess( const char *name, const char *message, float Temp, const char * location, int rotation){

       if(message != lastmessage){// message
        tft.fillScreen(TFT_BLACK);
      lastmessage = message;// message
     }
     
//if (validData){
    tft.setRotation(rotation);
 //  tft.fillScreen(TFT_BLACK);
  tft.setFreeFont(&Orbitron_Light_24);
      tft.setTextSize(0);
      //tft.setTextSize(1);
        tft.fillScreen(TFT_BLACK);// ========= comment this line out if scren flickers =========
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setCursor(0, vOffset);
      tft.setTextColor(TFT_BLACK, TFT_BLACK);
      tft.println(lastmessage);
  tft.setCursor(0, vOffset);
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.setTextColor(TFT_PINK, TFT_BLACK);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
            tft.print(message);// message
            lastmessage = message;

            
    Serial.print("Mess:");
    Serial.println(message);// batLev
    //Serial.print(" F :");


  
      //tft.println(" F   ");
           // tft.print(location);
 // tft.setCursor(0, 50);
           // tft.print("Temp: ");
          // tft.println(name);
          //  tft.print("msg ");
  //tft.drawRect(45, 55, 130, 40, TFT_ORANGE);
            //tft.println(" F   ");
//}
}







void displayStat(int P1, int P2, int P3, int P4, int P5, int P6, int P7, int P8, const char *name, const char *description) {
//const  char* mess = description;
//char dest[21];
//strncpy(dest,mess,20);


//.  display.fillRect(0, 8, 128, 28, BLACK);
  
//.  display.setTextColor(WHITE,BLACK);
//.  display.setTextSize(1);
//.  display.setCursor(0,8);
//.  display.println(name);
//.  display.println(description);
//.  display.setCursor(15,25);
   //   display.print("P: ");
  tft.fillScreen(TFT_BLACK);
  tft.setFreeFont(&Orbitron_Light_24);
  tft.setCursor(0, 30);
if(P1){
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.print("1");
}else{
  tft.setTextColor(TFT_BLACK, TFT_GREEN);
  tft.print("1");
} 
  tft.setTextColor(TFT_BLACK);tft.print(" ");
if(P2){
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.print("2");
}else{
  tft.setTextColor(TFT_BLACK, TFT_GREEN);
  tft.print("2");
} 
  tft.setTextColor(TFT_BLACK);tft.print(" ");
if(P3){
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.print("3");
}else{
  tft.setTextColor(TFT_BLACK, TFT_GREEN);
  tft.print("3");
} 
  tft.setTextColor(TFT_BLACK);tft.print(" ");
if(P4){
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.print("4");
}else{
  tft.setTextColor(TFT_BLACK, TFT_GREEN);
  tft.print("4");
} 
  tft.setTextColor(TFT_BLACK);tft.print(" ");
if(P5){
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.print("5");
}else{
  tft.setTextColor(TFT_BLACK, TFT_GREEN);
  tft.print("5");
} 
  tft.setTextColor(TFT_BLACK);tft.print(" ");
if(P6){
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.print("6");
}else{
  tft.setTextColor(TFT_BLACK, TFT_GREEN);
  tft.print("6");
} 
  tft.setTextColor(TFT_BLACK);tft.print(" ");
if(P7){
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.print("7");
}else{
  tft.setTextColor(TFT_BLACK, TFT_GREEN);
  tft.print("7");
} 
  tft.setTextColor(TFT_BLACK);tft.print(" ");
if(P8){
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.print("8");
}else{
  tft.setTextColor(TFT_BLACK, TFT_GREEN);
  tft.print("8");
} 
  tft.setTextColor(TFT_BLACK);
   
//display.display();
}


void displayBatt(int P1, int P2, int P3, int P4, int P5, int P6, int P7, int batLevel, const char *name, const char *description) {
//.  display.setTextColor(WHITE,BLACK);
//.  display.setTextSize(1);
//.  display.setCursor(0,8);
//.  display.println(name);
//.  display.println(batLevel);
//.  display.setCursor(15,25);

   float Level = batLevel * (range / 1023.00);
//   float Bar=(Level/range)*(display.width());
   float Percent=(Level/range)*100;



  

//.  display.setTextSize(2);
//.  display.setCursor(10,10);
//  display.print(batLevel,5);// Level Bar Percent
  //display.println(" v");
  
//.  display.setTextSize(1);
//.  display.setCursor(0,20);
//.  display.print("0");//uid
//.  display.setCursor((display.width()-12),20);
//.  display.print(range);//uid

/* ===display bargraph== */
//.  display.drawLine(0, 27, display.width(), 27, WHITE);
//.  display.fillRect(0, 28, Bar, 3, WHITE);
//.  display.fillRect((Bar +1), 28, display.width(), 3, BLACK);
//.  display.drawLine(0, 31, display.width(), 31, WHITE);
  //display.drawRoundRect(17, 8, 93, 18, 4, BLACK);
//.  display.display();
}










//display.print(str(name) );
 // const char* OldButtons = Buttons;// LED_BUILTIN
  
