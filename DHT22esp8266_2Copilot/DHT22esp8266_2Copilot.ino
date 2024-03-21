// DHT22esp8266_Copilot2

// This code was created with help from Bing Copilot

// FB help: https://www.facebook.com/groups/arduino.support.forum/posts/1123220475551245/


// The following code only posts data to a url



int flag = 1;
  String DeviceID ="477383611";// Blue Cabin
  String controlpanel ="numbers";
//  String DeviceID ="449738478";// Thermostat
//  String controlpanel ="thermostat";
  String location ="esp8266_DHT22 Blue Cabin";
  const char* wifiID = "WeatherStation__Lab";// no spaces
  const char* host = "robotic.icu"; // Change to your server's IP or domain


int baud = 19200; // 19200 115200
int Counter = 400000;// xxxx xxxx 139238 131740 127444 108858
int DHTdelay = 3000;

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <DHT.h> //          ESP-6A81D5


#include <Logins.h>
/* ====  store content below in: libraries/Logins/Logins.h
char ssid[] = "xxxxxxxxxx";// your network SSID (name)
char pass[] = "xxxxxxxxxx";// your network password
==== */

/* ====== Setup DHT ====== */
#define DHTPIN D6 // NodeMCU
//#define DHTPIN D7 // D1Mini
//#define DHTPIN 26 // esp32S3 - does not work
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
bool useFahrenheit       = true; // Default to Fahrenheit
/* ====== Setup DHT ====== */

void setup() {
  Serial.begin(baud);
  WiFi.begin(ssid, pass);
    WiFi.hostname(wifiID);
    
  //
  digitalWrite(LED_BUILTIN, LOW);// turn off
  //
  pinMode(LED_BUILTIN, OUTPUT);

  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    delay(500);// 1000
//    Serial.println("Connecting to WiFi...");
  }
  Serial.print("\n\nConnected to WiFi: ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  
  Serial.print("\n\nDeviceID: ");
  Serial.println(DeviceID);
  Serial.print("    Device: ");
  Serial.println(location);
  Serial.print("\n\n");

    delay(500);// 1000
  digitalWrite(LED_BUILTIN, HIGH);// turn off
  dht.begin();
}

void loop() {
//  float temp = dht.readTemperature(useFahrenheit);
  float tempFahrenheit = dht.readTemperature(useFahrenheit);

  float tempCelsius = dht.readTemperature(); // Temperature in Celsius
//  float tempFahrenheit = (tempCelsius * 9.0 / 5.0) + 32.0; // Convert to Fahrenheit
  float humidity = dht.readHumidity();
//  float hi = dht.computeHeatIndex(t, h, useFahrenheit);
  if (isnan(humidity) || isnan(tempFahrenheit)){  
    return;
  }

  String ip = WiFi.localIP().toString();
  String postData = "";
     postData += "&location=";
     postData += String(location);
     postData += "&Temp=";
     postData += String(tempFahrenheit);
     postData += "&Humidity=";
     postData += String(humidity);
     postData += "&batLevel=";
     postData += String(humidity);// dummy data for test
     postData += "&windSpd=";
     postData += String(Counter);// counts how many times data was posted
     
     if(flag){ // ========= if (flag) post only once =============
       postData += "&controlpanel=";
       postData += String(controlpanel);
       postData += "&ip=";
       postData += ip;
       flag=0;
     }
//     postData += "&windSpd=" + String(Counter);

  HTTPClient http;
  http.begin("http://" + String(host) + "/robotid.php?id=" + DeviceID);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  int httpCode = http.POST(postData);
  if (httpCode == HTTP_CODE_OK) {
//    Serial.println("Data sent successfully!");
    Serial.print(Counter);
    Serial.print(" Temp: ");
    Serial.print(tempFahrenheit,2);
    Serial.print("  Humidity: ");
    Serial.println(humidity,2);
    Serial.print("  ip: ");
    Serial.println(ip);
    
  } else {
    Serial.println("Error sending data");
  }

  http.end();

  Counter++;

  delay(DHTdelay); // Adjust as needed
}
