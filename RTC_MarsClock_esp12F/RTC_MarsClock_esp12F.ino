/*
RTC_MarsClock

Mars Facts:
Because Mars rotates on its axis just a little bit slower than 
Earth does, though, a sol on Mars is 2.75 percent longer than 
a day on Earth: 24 hours 39 minutes 35 seconds. 

24*61*61=89304 sec
88775.244
(1 Mars second) = (1.02749125 Earth seconds)=88775.244/(24*3600)




So expressed in Earth time, 
on Mars each solar hour is 1 hour 1 minute 39 seconds 
(1/24 of a sol) long, each solar minute is 61.65 seconds long, 
and each solar second is 1.0275 seconds long.

Me:
Can you write me code for the esp32 that uses a DS3231 RTC 
that compares the RTC time to the unix timestamp(1672012800) 
of the beginning of the Mars year to get the currect Mars time?

Copilot:
Sure! Here’s a sample code for the ESP32 that uses a DS3231 RTC 
to compare the RTC time to the Unix timestamp (1672012800) of the 
beginning of the Mars year and calculates the current Mars time:

This code initializes the DS3231 RTC and calculates the current 
Martian time based on the Unix timestamp for the beginning of the 
Mars year. It then prints the current sol (Martian day) and the 
Martian time in hours, minutes, and seconds.

Feel free to modify the code as needed for your specific application. 
If you have any questions or need further assistance, just let me know!

Issue:
I can't get the esp32 to see the RTC...
This code should get the time Mars crossed Earth's Jan 1st and zero secnd line,
but it does not take in account what the rotational position Mars was at that instant..
To correct this you should be able to add or subtract a given number of seconds.

My original plan was to use this to know when he Mars Rover was facing Earth,
but that would require knowing the angle between Earth and Mars at any time.

This should be possible using (Inverse Kenomatics).. I am guessing...

Mitchell R
​​/note 8 , 9 SDA SCL WROOM32

December 26, 2022
*/
int baud = 19200;// 19200 115200
#include <SPI.h>
#include <Wire.h>
#include <RTClib.h>
//#include <uRTCLib.h>
//#include <WiFi.h>      // esp32S
#include <ESP8266WiFi.h> // esp12F
//#define SDA 8 //21
//#define SCL 9 //22

// Initialize the DS3231 RTC
RTC_DS3231 rtc;

// Unix timestamp for the beginning of the Mars year
const uint32_t marsYearStart = 1672012800;//1672012800 1672058122 

// Martian sol (day) length in seconds
const double solLength = 88775.244;

// WiFi credentials (if needed for other purposes)
//const char* ssid = "your_SSID";
//const char* password = "your_PASSWORD";
#include <Logins.h>
/* ====  store content below in: libraries/Logins/Logins.h
char ssid[] = "xxxxxxxxxx";// your network SSID (name)
char pass[] = "xxxxxxxxxx";// your network password
==== */

void setup() {
  Serial.begin(baud);
const char* wifiID = "MarsClock";// no spaces
  pinMode(LED_BUILTIN, OUTPUT);
//  Wire.begin(SDA,SCL);
  Wire.begin();

  // Initialize the RTC
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  // Check if the RTC lost power and if so, set the time
  if (rtc.lostPower()) {
    Serial.println("RTC lost power, setting the time!");
    // Set the RTC to the date & time this sketch was compiled
//    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  // Connect to WiFi (if needed)
    WiFi.hostname(wifiID);
  WiFi.begin(ssid, pass);
    Serial.println("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));// 
    delay(1000);
  }
  Serial.println("Connected to WiFi");
  digitalWrite(LED_BUILTIN, HIGH);// HIGH LOW

  
//    delay(60000);
}

void loop() {
  DateTime now = rtc.now();
  uint32_t currentUnixTime = now.unixtime();

  // Calculate the elapsed time since the start of the Mars year
  uint32_t elapsedTime = currentUnixTime - marsYearStart;

  // Calculate the current sol (Martian day) and time on Mars
  uint32_t currentSol = elapsedTime / solLength;
  uint32_t solSeconds = elapsedTime % (uint32_t)solLength;

  uint32_t solHours = solSeconds / 3600;
  uint32_t solMinutes = (solSeconds % 3600) / 60;
  uint32_t solSecondsRemaining = solSeconds % 60;

  // Print the current Martian time
  Serial.print("Current Sol: ");
  Serial.println(currentSol % 668);
  Serial.print("Martian Time: ");
  if(solHours<10){Serial.print("0");}
  Serial.print(solHours);
  Serial.print(":");
  if(solMinutes<10){Serial.print("0");}
  Serial.print(solMinutes);
  Serial.print(":");
  if(solSecondsRemaining<10){Serial.print("0");}
  Serial.println(solSecondsRemaining);

  delay(1000); // Update every second
}
