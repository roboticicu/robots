/*
RTC_MarsClock

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
*/
int baud = 115200;// 19200 115200
#include <SPI.h>
#include <Wire.h>
#include <RTClib.h>
//#include <uRTCLib.h>
#include <WiFi.h>
//#define SDA 8 //21
//#define SCL 9 //22

// Initialize the DS3231 RTC
RTC_DS3231 rtc;

// Unix timestamp for the beginning of the Mars year
const uint32_t marsYearStart = 1672012800;

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
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  
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
  Serial.println(currentSol);
  Serial.print("Martian Time: ");
  Serial.print(solHours);
  Serial.print(":");
  Serial.print(solMinutes);
  Serial.print(":");
  Serial.println(solSecondsRemaining);

  delay(5000); // Update every second
}
