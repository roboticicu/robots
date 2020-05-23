/*
    This sketch trys to Connect to the best AP based on a given list

*/

int DeviceID = 123456789;
const char * location = "unset  ";
int baud = 115200;

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

ESP8266WiFiMulti wifiMulti;

#ifdef ESP8266
const char* device = "esp8266";
//#include <ESP8266WiFi.h>
#elif ESP32
const char* device = "esp32  ";
//#include <WiFi.h>
#endif

/* ====== OLED Setup ====== */
#include <SPI.h>
#include <Wire.h>
int rotation=0; // display upright horizontal
  #include <Adafruit_SSD1306.h>
//  #include <Adafruit_SSD1306_64.h>
#include <Adafruit_GFX.h>

// OLED display TWI address
#define OLED_ADDR    0x3C
Adafruit_SSD1306 display(-1);

#if (SSD1306_LCDHEIGHT != 32)
//#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif
/* ====== OLED Setup ====== */


void setup() {
  Serial.begin(baud);
  /* ====== setup section OLED ====== */
  // initialize and clear display
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.setRotation(rotation);// h 0,2  v 1,3
  /* ====== setup section OLED ====== */


  WiFi.mode(WIFI_STA);
#include <MultiWifi.h>
/* ====  store content below in: libraries/MultiWifi/MultiWifi.h
  wifiMulti.addAP("ssid_from_AP_1", "your_password_for_AP_1");
  wifiMulti.addAP("ssid_from_AP_2", "your_password_for_AP_2");
  wifiMulti.addAP("ssid_from_AP_3", "your_password_for_AP_3");
==== */

  Serial.println("Connecting Wifi...");
  if (wifiMulti.run() == WL_CONNECTED) {
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  }
}

void loop() {
  if (wifiMulti.run() != WL_CONNECTED) {
    Serial.println("WiFi not connected!");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  GetLocation();

ConnectState();
    delay(1000);
  }else{
    GetLocation();
    Serial.println("WiFi connected!");
  Serial.print("SSID: ");  Serial.print(WiFi.SSID());
  Serial.print("  location: ");  Serial.print(location);
  Serial.print("  IP address: ");  Serial.println(WiFi.localIP());
ConnectState();
    delay(1000);
    }
}

void GetLocation(){
  if (WiFi.SSID() == "XkV-mock-7048") {
    location = "Home    ";
  }else if (WiFi.SSID() == "john316") {
    location = "Church  ";
  }else if (WiFi.SSID() == "routeratmoms") {
    location = "Mom's   ";
  }else if (WiFi.SSID() == "xfinitywifi") {
    location = "Hotel   ";
  }else if (WiFi.SSID() == "OfficeWiFi") {
    location = "Office  ";
  }else if (WiFi.SSID() == "SecretHideout") {
    location = "HideOut ";
  }   
}

void ConnectState() {
  

  
  display.clearDisplay();
  // display a line of text
  display.setTextColor(WHITE,BLACK);
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("DeviceID     ");
  display.setCursor(65,0);
  display.print("Location\n");
  display.print(DeviceID);
  display.print("      ");
  display.setCursor(65,8);
  //display.print("Location: ");
  display.println(location);
    display.print("\nIP: ");
    display.println(WiFi.localIP());
  display.display();
}
