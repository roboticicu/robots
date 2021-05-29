// RemoteDVM



/*
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
*/

// ========= Version settings ============
// esp8266            v2.6.3
// Adafruit SSD1306   v2.4.5 <== Not tested yet
// ArduinoJson        v6.x
// ========= Version settings ============


// ======== DHT22 pins ============
// DHT    D1mini  NodeMCU
// Vcc1    3.3    3.3
// Sig2    D7     D6
// GND4    GND    GND
// ======== DHT22 pins ============

/* ==================== beginning of code ==================== */



int PostTime;

int dhtdelay = 6000;// ==== 2000 minimum ============

int baud = 9600;
int rotation =0;// 1 Horizontal



const char * Status;
float BatLevel;



/* =========== ADC setup =========== */
#include <Adafruit_ADS1X15.h>
//#include <Adafruit_ADS1015.h>
//#include <Wire.h>
Adafruit_ADS1115 ads; //(0x48)        Adress of the ADC

/* =========== ADC setup =========== */




/* =========== ADC Voltagee Cal setup =========== */
int calib = 7; // Value of calibration of ADS1115 to reduce error
float voltage = (1.0000); // used to store voltage value
float Radjust = 0.043421905; // Voltage divider factor ( R2 / R1+R2 )
float vbat; //final voltage after calcs- voltage of the battery

//variables for refreshing the screen without using delay
unsigned long previousMillis = 0;        // will store last time the screen was refreshed

// constants won't change:
const long interval = 250;           // interval at which to refresh the screen (milliseconds)
/* =========== ADC Voltagee Cal setup =========== */







//  if (dhtdelay < 2000){
//    dhtdelay = 2000;
//  }

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
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 64, &Wire);// 32 or 64 dot mode
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
  const char* wifiID = "Wireless_DVM";// no spaces
  const char* board = "esp8266 on breadboard";
  #define DHTPIN D6
  //  const char* DeviceID = "477383611";// LivingRoom
  const char* DeviceID = "104595466";// Voltmeter
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









// String location = WiFi.macAddress();








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
void postVolts(){
/* ====== READ ADC ====== */
int16_t adc0; // 16 bits ADC read of input A0
adc0 = ads.readADC_SingleEnded(0);
voltage = ((adc0 + calib) * 0.1875)/1000;

unsigned long currentMillis = millis();
//vbat = ((voltage/Radjust),4);
vbat = (voltage/Radjust)*1.000;
//vbat = (vbat,4);

//Prevent displaying negative voltage when battery is disconnected  
 if (vbat < 0.01) {
        vbat = 0.0001;
 }
  Serial.print("vbat:");
  Serial.print(vbat,22);//works here but not in URL += 
  Serial.print("    ");
/* ====== READ ADC ====== */

/* ====== LED Blink on weather condition ====== */
if((vbat > 90)&&(unit==1)){// vbat 
  // blink
  led = 1;
}else if((vbat > 47.000)&&(unit==2)){// vbat 
  // blink
  led = 1;
}else if((vbat < 70.000)&&(unit==2)){// vbat 
  // blink
  led = 1;
}else{
  // no blink
  led = 0;
  digitalWrite(LED_BUILTIN, HIGH);// turn off
}
/* ====== LED Blink on weather condition ====== */






//  Serial.print(DHTPIN);
Serial.print(" Dev: ");
   Serial.print(device);
Serial.print(" ip: ");
   Serial.println(localip);


  
// xxxxxxxxxxxx
  display.setTextSize(1);
//  display.setTextSize(2);
  display.setTextColor(WHITE,BLACK);
  
  display.setCursor(0,0);
//  display.println(location);



  display.display();





/* ===== add post to web here ====== */
  // Use WiFiClient class 
  // to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }


float setpoint = 25;             // Battery Cutoff
float vmax = 3.83;                // Battery Full Charge
float vmin = 3.2;                // Battery Min Charge
float vdif = vmax-vmin;          // Battery Charge dif
float BatLevel = ((vbat-vmin)*100/(vmax-vmin));// Bat percent
Status = ((BatLevel>setpoint)?"Online":"Offline");


//  String strbat= (vbat.toConst.toString(),6);
  String strbat;
   sprintf(strbat, "%3f", vbat);
  // We now create a URI for the request
  String url = "/robotid.php";
  url += "?id=";
  url += DeviceID;
  url += "&KeyRoboUser=";
  url += KeyRoboUser;
  url += "&online=";
  url += Status;       // <=======
  url += "&batLevel=";// BatLevel
  url += BatLevel;     // <=======
  url += "&Temp=";
  url += strbat;    // <=======
  url += "&Humidity=";
  url += vmax;     // <=======







  display.clearDisplay();
  // display a line of text
  display.setTextColor(WHITE,BLACK);
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("DeviceID: ");
  display.println(DeviceID);
  display.print("Status: ");
  display.println(Status);
  display.println(" ");
  display.print("vbat: ");
  display.println(vbat,6);

  display.display();







//

 /*



  url += number_format(vbat, 3, '.', ' ');         // <=======
 
float BatLevel = ((vbat-vdif)*100/(vmax-vdif));// Bat percent
  url += "&ip=";
  url += localip;
//        float BatLevel = (vbat*100/setpoint);// Bat percent
//        float BatLevel = map((vbat*100),(vmin*100),(vmax*100),0,100);
        float voLevel = (vbat/setpoint)*100;
//  float(11), vbat;
//  float vbat(10,3);
//  url += number_format(vbat, 3, '.');
//  url += number_format(vbat, 3, '.', ' ');

//  url += float(3.1415926);
//  url += "10.123456789";
//  url += (float)vbat;
//  url += (vbat* 1000);
//  url += (vbat*1.3141592653589727);

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
  
//  float t = dht.readTemperature(useFahrenheit);




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

  
//  float t = dht.readTemperature(useFahrenheit);
//  float h = dht.readHumidity();




  
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
  ads.begin();
  
  Serial.begin(baud);
  pinMode(LED_BUILTIN, OUTPUT);






  /* ====== setup section OLED ====== */
  // initialize and clear display
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.setRotation(rotation);// h 0,2  v 1,3
  /* ====== setup section OLED ====== */


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
  //delay(dhtdelay); //Required by this device to function properly
  
  if ((millis() - PostTime) > dhtdelay) { 
     postVolts();
       if(led == 1){
         digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
       }
    PostTime = millis();
  }
  if(debug){debuger(); }
}
