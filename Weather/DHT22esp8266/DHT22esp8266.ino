// DHT22esp8266




//////////////////////////////////////////////////////////////////
// DHT22 Test program for the ESP32 TTGO-T Display              //
// Jordan Rubin 2020                                            //
// More at https://www.youtube.com/c/jordanrubin6502            //
// http://technocoma.blogspot.com                               //
//////////////////////////////////////////////////////////////////

// Provides a graphical output on the built in display of the TTGO for both
// Temperature and humidity or switch between those and heat index with a
// push of a button.  The second button switches everything between Celsius
// and Fahrenheit. Both switches have built in interupt and debounce.

// Directly related to the video https://youtu.be/u7277VShso4

/*
========= Version settings ============
esp8266         v2.6.3
ArduinoJson     v6.x
========= Version settings ============


======== DHT22 pins ============
DHT    D1mini  NodeMCU
Vcc1    3.3    3.3
Sig2    D7     D6
GND4    GND    GND
======== DHT22 pins ============
*/

// #include <getActiveBoard.h>

// char build;



//#define ESP8266 D6
//#define D1MINI D4
//#define ESP32 1




//#define ESP8266

// boards.txt
//#define ESP8266_NODEMCU // line 2538
//#define ESP8266_WEMOS_D1R1 // line 4322
//#define WEMOS_D1MINI
//#define ARDUINO_ESP8266_WEMOS_D1MINI
//#define WEMOS


int numnum = 4; // dhtPin on esp
int led = 0; // 1 = blink 0=noblink



int debug = 1;
int count =0;
String location;
//const char* location = "";
//String location = "Hello_World";
//const char* location = "Living%20Room%20Forteen";
const char* host = "www.robotic.icu";
const char* streamId = "xxxxxxxxx";
//const char* DeviceID = "849081125";// Space Command Driveway
//const char* DeviceID = "477383611";// LivingRoom
/* 
477383611 849081125
449738478 519994052 383085317 477383611 
240527013 934661423 907864801 168036030 184884263
876975430 992438994 866880631 611452822 286531102 
112518130 368141751 368141751 770623542 749236437 
513739377 471922699 233683192 417658552 
*/

int dhtdelay = 2000;
int baud = 115200;
int rotation =0;// 1 Horizontal
int IR=0;
String localip;

#include <Logins.h>
/* ====  store content below in: libraries/Logins/Logins.h
char ssid[] = "xxxxxxxxxx";// your network SSID (name)
char pass[] = "xxxxxxxxxx";// your network password
==== */

#include <KeyRoboUser.h>
/* ====  store content below in: libraries/KeyRoboUser/KeyRoboUser.h
char KeyRoboUser[] = "d0d0bf1bb7xxxx2e5dce67c95f4fd0800";// your Robot Key
==== */

/* ====== OLED Setup ====== */
#include <SPI.h>
#include <Wire.h>
  #include <Adafruit_SSD1306.h>
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &Wire);// 32 or 64 dot mode
#include <Adafruit_GFX.h>// setTextSize

// OLED display TWI address
#define OLED_ADDR    0x3C
//Adafruit_SSD1306 display(-1);

#if (SSD1306_LCDHEIGHT != 32)
//#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif
/* ====== OLED Setup ====== */



#include <ArduinoJson.h>



/* ====== Doesn't work yet ====== */
//#ifdef ESP8266 
#ifdef ESP8266_NODEMCU
//#include <ESP8266WiFi.h>
//const char* ledBI = "16 ]";
//const char* device = "esp8266 BB";
//const char* board = "esp8266 on breadboard";
//#define DHTPIN D6
#elif ESP8266_WEMOS_D1R1
//#include <ESP8266WiFi.h>
//const char* ledBI = "2 ]";
//const char* device = "D1 Mini";
//const char* board = "D1 Mini on Window Sill";
//#define DHTPIN D7
#elif ESP32
const char* device = "esp32";
const char* board = "TTGO";
#include <WiFi.h>
#include <TFT_eSPI.h>
//#include "SPIFFS.h"
//#include <HTTPClient.h>
#endif
/* ====== Doesn't work yet ====== */



/* ====== Works, but not best solution ====== */
// xxxxxxxxxxxx
// this is a work around(cheat) that works 
// if you remember to use pin 16 on the NodeMCU 
// during upload but I would like all boards to 
// adopt a standardized board specific identifier

#if LED_BUILTIN == 16 
  //
  int unit = 2;
  #include <ESP8266WiFi.h>
  const char* ledBI = "16 ]";
  const char* device = "Breadboard esp8266";
  const char* wifiID = "WeatherStation__Inside";// no spaces
  const char* board = "esp8266 on breadboard";
  #define DHTPIN D6
  //
  const char* DeviceID = "477383611";// LivingRoom
#else
  //
  int unit = 1;
  #include <ESP8266WiFi.h>
  const char* ledBI = "2 ]";
  const char* device = "D1 Mini";
  const char* wifiID = "WeatherStation__Driveway";// no spaces
  const char* board = "D1 Mini on Window Sill";
  #define DHTPIN D7 //D7 D5 
  //
  const char* DeviceID = "849081125";// Space Command Driveway
#endif
/* ====== Works, but not best solution ====== */




// #define DHTPIN D4
/* ====== Setup DHT ====== */
#include <DHT.h>
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
/* ====== Setup DHT ====== */


//const char* ledBI = "=== 16 ===";
//const char* device = "esp8266";
//const char* board = "esp8266 on breadboard";
//const char* ledBI = "=== 2 ===";
//const char* device = "D1 Mini";
//const char* board = "D1 Mini";
//#define DHTPIN D4 // D4 D6

//#include <ESP8266HTTPClient.h>
//#include <ESP8266Ping.h>
//#include <ESP8266HTTPClient.h>
//#include <ESP8266Ping.h>



//int dhtpin; 
//if (LED_BUILTIN ==16) { 
//    dhtpin = D6;
//const char* ledBI = "=== 16 ===";
//const char* device = "esp8266";
//const char* board = "esp8266 on breadboard";
//  } else { 
//    dhtpin = D4;
//const char* ledBI = "=== 2 ===";
//const char* device = "D1 Mini";
//const char* board = "D1 Mini";
//  }







//const char* ledBI = "=== 2 ===";
//const char* device = "D1 Mini";
//const char* board = "D1 Mini on Window Sill";



//#if DHTPIN
////Serial.print("DHTPIN: ");
////   Serial.println(DHTPIN);
//#else
////Serial.print("DHTPIN: Not  Defined");
//#endif







//#ifdef LED_BUILTIN == 16
//const char* ledBI = "=== 16 ===";
//const char* device = "D1 Mini";
//const char* board = "D1 Mini";
//#define DHTPIN D4 // D4 D6
//#elif LED_BUILTIN == 2
//const char* ledBI = "=== 2 ===";
//const char* device = "esp8266";
//const char* board = "esp8266 on breadboard";
//#define DHTPIN D6 // D4 D6
//#endif





// WEMOS 1
// ARDUINO_ESP8266_WEMOS_D1MINI
// D1MINI


      
//. #ifdef ESP8266
//const char* board = "esp8266 on breadboard";
//#define DHTPIN D6 // D4 D6
//#elif ARDUINO_ESP8266_WEMOS_D1MINI
//const char* device = "D1 Mini";
//const char* board = "D1 Mini";
//#define DHTPIN D4 // D4 D6
//#include <ESP8266WiFi.h>
//. #elif ESP32
//. #define DHTPIN 17
//. #include <TFT_eSPI.h>
//. TFT_eSPI tft = TFT_eSPI();
//. #endif


/* 

WEMOS_D1MINI

ARDUINO_ESP8266_WEMOS_D1MINI
if (dhtPin == 4) {
  #define DHTPIN D4 // D6
} else {
  #define DHTPIN D6 // D6  
}

if (dhtPin == 4){#define DHTPIN D4}
    else {#define DHTPIN D6}

    
if (numnum != 4){ #define DHTPIN D4
  } else {
    //  #define DHTPIN D6
      }
 */


// String location = WiFi.macAddress();






//#define irmotionPin D3




#include <SPI.h>

#define BUTTON1PIN 35
#define BUTTON2PIN 0


bool useFahrenheit       = true; // Default to Fahrenheit
bool showTemp            = true; // Default to Temp / Humidity
long lastDebounceButton1 = 0;    // Holds Button1 last debounce
long lastDebounceButton2 = 0;    // Holds Button2 last debounce
long debounceDelay       = 200;  // 200ms between re-polling

// wind
float atk = 1.35;
float sus = 2.35;
float de = 1.68;
float rel = 1.90;
float newspeed = 0.000;
int diff;
float w;
float ls;
float Baro;

// INTRPT Function to execute when Button 1 is Pushed
void IRAM_ATTR toggleButton1() {
  if ((millis() - lastDebounceButton1) > debounceDelay) { 
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
void postTemp(){
  float t = dht.readTemperature(useFahrenheit) * 1.00;
  float h = dht.readHumidity() * 1.00;

/* ====== LED Blink on weather condition ====== */
if((t < 32.010)&&(unit==1)){// t < 47.100 h > 50.00
  // blink
  led = 1;
}else if((h > 47.000)&&(unit==2)){// t < 47.100 h > 50.00
  // blink
  led = 1;
}else if((t < 70.000)&&(unit==2)){// t < 47.100 h > 50.00
  // blink
  led = 1;
}else{
  // no blink
  led = 0;
  digitalWrite(LED_BUILTIN, HIGH);// turn off
}
/* ====== LED Blink on weather condition ====== */



/* ====== Random Wind ====== */
  //   float ls = ((float) rand() *1.1) ;
  //float ls = rand( 0.78, 4.5 );
  // w = (ls/2099999);

//randNumber = random(min, max);
diff = (w/10);
long r = random(0, 2);

if(r == 1){
  if(w < 0.1){w=0.1111;}
    //w=w+(ls-(w*1.15));
    w=(w*1.15);
  if(w > 100.1){w=100;}
  }else{
  if(w > 100.1){w=100;}
    //w=w-(ls+(w*1.15));
    w=(w/1.15);
  if(w < 0.1){w=0.1111;}
  }
ls = w;
/* ====== Random Wind ====== */
  

//  Serial.print("r:");
//  Serial.print(r);
//  Serial.print("  w:");
//  Serial.print(w);
//  Serial.print("  ls:");
//  Serial.println(ls);



//  if(digitalRead(irmotionPin)==LOW){
//     IR=!IR;
//     digitalWrite(irmotionPin,IR);
//
//  }


  Serial.print("t:");
  Serial.print(t);
  Serial.print("  h:");
  Serial.print(h);
  Serial.print("  w:");
  Serial.print(w);
//  Serial.print(" [ledBI: ");
//    Serial.print(ledBI);
//  Serial.print(LED_BUILTIN);
//  Serial.print("] => DHTPIN: ");
//  Serial.print(DHTPIN);
Serial.print(" Dev: ");
   Serial.println(device);


//    Serial.print("  IR: ");
//    Serial.print(IR);
  
// xxxxxxxxxxxx
  display.setTextSize(1);
//  display.setTextSize(2);
  display.setTextColor(WHITE,BLACK);
  
  display.setCursor(0,0);
//  display.println(location);
  display.print("Temp:");
   display.print(t);
    display.println(" F ");
  display.print("Humid:");
   display.print(h);
    display.println(" % ");
  display.print("Wind:");
   display.print(w);
    display.println(" mph ");

  display.print("IP:");
   display.print(localip);

  display.display();

  
  //float w = (ls + 1.4) + (ls*atk)+(ls*sus)-(ls*de)-(ls*rel) - ls;
  //ls = w;
  
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

/* ===== add post to web here ====== */
  // Use WiFiClient class 
  // to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  // We now create a URI for the request
  String url = "/robotid.php";
  url += "?id=";
  url += DeviceID;
  url += "&KeyRoboUser=";
  url += KeyRoboUser;
  url += "&ip=";
  url += localip;
  /* ===== Weather ====== */
  url += "&Temp=";
  url += t;// Temperature
  url += "&batLevel=";
  url += t;// BatLevel
  url += "&Humidity=";
  url += h;//h Humidity
  url += "&windSpd=";
  url += w;// Wind
  url += "&Baro=";
  url += Baro;// air pressure
  /* ===== Weather ====== */



  
 /*
  url += "&location=";
  url += location;
  
  url += "&batLevel=";
  url += t;// Temperature
  url += "&access=";
  url += h;// Humidity
  url += "&ip=";
  url += w;// Wind
  url += "&B3=";
  url += IR;
  */
  //Serial.print("Requesting URL: ");
  //Serial.println(url);
 
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  delay(1);  
/* ===== add post to web here ====== */  
}



// What to display if showTemp = true
void showScrn1() {
  float t = dht.readTemperature(useFahrenheit);
//  float h = dht.readHumidity();
//  tft.fillScreen(TFT_BLACK);
//  tft.setTextColor(TFT_GREEN, TFT_BLACK);
//  tft.setCursor(0, 30);
//  tft.setFreeFont(&Orbitron_Light_24);
//  tft.println("Temp     Humidity");
//  tft.drawLine(0, 35, 250, 35, TFT_BLUE);
//  tft.setCursor(0, 60);
//  tft.print(t);
//  if (useFahrenheit){tft.print(F("f"));}
//  else {tft.print(F("c"));}
//  tft.setCursor(130, 60);
//  tft.print(h);tft.print(F("%"));
}

// What to display if showTemp = false
void showScrn2() {
  float t = dht.readTemperature(useFahrenheit);
  float h = dht.readHumidity();
//  float hi = dht.computeHeatIndex(t, h, useFahrenheit);
//  tft.fillScreen(TFT_BLACK);
//  tft.setTextColor(TFT_RED, TFT_BLACK);
//  tft.setCursor(50, 30);
//  tft.setFreeFont(&Orbitron_Light_24);
//  tft.println("Heat Index");
//  tft.drawLine(0, 35, 250, 35, TFT_BLUE);
//  tft.setFreeFont(&Orbitron_Light_32);
//  tft.setTextColor(TFT_GREEN, TFT_BLACK);
//  tft.setCursor(60, 100);
//  tft.print(hi);
//  if (useFahrenheit){tft.print(F("f"));}
//  else {tft.print(F("c"));}
}

void setup() {
    dht.begin();
  pinMode(DHTPIN, INPUT);

  
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);













  

    // Some more OLED setup
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setRotation(rotation);// h 0,2  v 1,3
  display.clearDisplay();    // Clear the buffer.
  display.display();

  



  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
    WiFi.hostname(wifiID);
  WiFi.begin(ssid, pass);



  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    delay(500);
    //Serial.print(".");
  }
  digitalWrite(LED_BUILTIN, HIGH);// turn off
  Serial.println("");
  Serial.println("WiFi connected"); 
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
//  localip = String(WiFi.localIP());
  localip = WiFi.localIP().toString();

// String location = WiFi.localIP();


















  
  pinMode(BUTTON1PIN, INPUT);
  pinMode(BUTTON2PIN, INPUT);
  attachInterrupt(BUTTON1PIN, toggleButton1, FALLING);
  attachInterrupt(BUTTON2PIN, toggleButton2, FALLING);
//  dht.begin();
  // tft.begin();
  // tft.setRotation(1); //Landscape
}

  /*
  if (showTemp){showScrn1();} // Temp Humidity
  else {showScrn2();}         // Heat Index
  */

void debuger(){
  //Serial.println(" ===== D1 mini ===== ");
  //  Serial.println(" ===== esp8266 ===== ");
while (count<1 ){
  Serial.print("== Board ==> ");Serial.println(board);
  Serial.print("ESP Board MAC Address:  ");
  Serial.println(WiFi.macAddress());
  count++;
  }  
}


void loop() {
  delay(dhtdelay); //Required by this device to function properly
  postTemp();
  if(led == 1){
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }
  if(debug){debuger(); }
}
