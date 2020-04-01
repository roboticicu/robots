

/*
 * robotStreamId
 * 
 *  This sketch sends data via HTTP GET requests to data.sparkfun.com service.
 *
 *  You need to get streamId and privateKey at data.sparkfun.com and paste them
 *  below. Or just customize this script to talk to other HTTP servers.
 *  
 * https://www.esp8266.com/viewtopic.php?p=13812#
 * 
 * 
 * I added code from this page..
 * arduinojson.org/v6/example/http-client/
 * 
 * YouTube video weather sketch that uses json
 * youtube.com/watch?v=3U-qbii1saw
 * 
 * arduinojson.org/v6/assistant/
 */

#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <Logins.h>

//OLEDs
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
//  #include <Adafruit_SSD1306_64.h>
#include <Adafruit_GFX.h>

/* ==== Logins.h content below...store in libraries
char ssid[] = "xxxxxxxxxx";// your network SSID (name)
char pass[] = "xxxxxxxxxx";// your network password
==== */

const char* host = "www.robotic.icu";
const char* streamId   = "xxxxxxx";
const char* DeviceID = "934661423";// 934661423 286531102

/* ====== OLED Setup ====== */
// OLED display TWI address
#define OLED_ADDR    0x3C
Adafruit_SSD1306 display(-1);
int State = 1;
int LED   = 2;
int rotation=0; // display upright horizontal



void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  /* ====== setup section OLED ====== */
  // initialize and clear display
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();
  display.setRotation(rotation);// h 0,2  v 1,3

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
  /* ====== setup section OLED ====== */
  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected"); 
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  delay(1000);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
 
  // We now create a URI for the request
  String url = "/robotid.php?id=";
  url += DeviceID; 
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
    Serial.print(F("Unexpected response: %s"), status);
    return;
  }

  // Skip HTTP headers
  char endOfHeaders[] = "\r\n\r\n";
  if (!client.find(endOfHeaders)) {
    Serial.println(F("Invalid response"));
    return;
  }

  // Read all the lines of the reply from server and print them to Serial
  // Each line should be its own JSON document. Process the document and then continue.
  while(client.available()){
    //Serial.print("debug ");
    String line = client.readStringUntil('\n');
    //    Serial.println(line);


    const size_t capacity = JSON_OBJECT_SIZE(22) + 370;
    DynamicJsonDocument doc(capacity);


    deserializeJson(doc, line);

    const char* id = doc["id"]; // "658435603"
    const char* admin = doc["admin"]; // "123456789"
    const char* user = doc["user"]; // ""
    const char* name = doc["name"]; // "Prof Ferber Farber"
    const char* description = doc["description"]; // "Robotics Debokler"
    const char* location = doc["location"]; // "Laboratory"
    const char* type = doc["type"]; // ""
    const char* controlpanel = doc["controlpanel"]; // "8nixi"
    const char* access = doc["access"]; // ""
    const char* online = doc["online"]; // "Online"
    const char* image = doc["image"]; // "GoFigure.jpg"
    const char* ip = doc["ip"]; // ""
    const char* batLevel = doc["batLevel"]; // "75"
    const char* P1 = doc["B1"]; // ""
    const char* P2 = doc["B2"]; // ""
    const char* P3 = doc["B3"]; // ""
    const char* P4 = doc["B4"]; // ""
    const char* P5 = doc["B5"]; // ""
    const char* P6 = doc["B6"]; // ""
    const char* P7 = doc["B7"]; // ""
    const char* P8 = doc["B8"]; // ""
    const char* message = doc["message"]; // "Hello mom!"
    displayStat(name);

    State = !(P1 == "0");
    digitalWrite(LED, State);//    LED_BUILTIN

    //  Serial.print(CLS);
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
    Serial.println(P8);
   delay(10);
  }

  //Serial.println("closing connection");
}


void displayStat(const char *name) {
  display.setTextColor(WHITE,BLACK);
  display.setTextSize(2);
  display.setCursor(0,20);
  display.print("Msg:");
  display.print(name);
  display.display();
}

