
/* code
 *  OLEDespNTP
 *  Thu 11/8/2018 7:16 PM
 *  set baud to 57600
 *    
 *  
 *  modified by: Brian Taylor
 *  added OLED
 *  needs to be modified 
 *  clock count by tenths of sec
 *  network updates only during 
 *  startup and every 30 minutes thereafter
 *  
 */
   // #define pin D4 // red
//    pinMode(pin, INPUT_PULLUP);
int pm=0;
        
// in seconds
// 
int zone = 5;
           int ZoneOffset[8] = {0,-36000,-18000,-14400,3600,10800,28800,39600};
 const char* ZoneLocation[8] = {"","Hawaii","EasterIs","Delaware","Spain","Uganda","Philippines","Sydney"};

/*
Easter Island GMT-5 -18000
Hawaii GMT-10 -36000

Delaware GMT -4 -14400 sum
Delaware GMT -5 -18000 win
*/


 
 
 //int ZoneOffset[2] = {10800};
 //const char* ZoneLocation[2]= {"Uganda"};
//int ZoneOffset[2] = 10800; int ZoneLocation[2]= "Uganda";


int Interval = 15000;// in milseconds

int rotation=0; // display upright horizontal 
/* ====== OLED Setup ====== */
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

// OLED display TWI address
#define OLED_ADDR    0x3C
Adafruit_SSD1306 display(-1);

#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif
/* ====== OLED Setup ====== */


/******************** - www.geekstips.com - Arduino Time Sync from NTP Server using ESP8266 WiFi module - Arduino code example ********************/

 #include <ESP8266WiFi.h>
 #include <WiFiUdp.h>


 char ssid[] = "xxxxxxxxx";
 // your network SSID (name)
 char pass[] = "xxxxxxxxx";
 // your network password


 unsigned int localPort = 2390;
 // local port to listen for UDP packets
 /* Don't hardwire the IP address or we won't get the benefits of the pool.
 * Lookup the IP address for the host name instead */

 //IPAddress timeServer(129, 6, 15, 28);
 // time.nist.gov NTP server 
 IPAddress timeServerIP;
 // time.nist.gov NTP server address
 const char* ntpServerName = "time.nist.gov";
 const int NTP_PACKET_SIZE = 48;
 // NTP time stamp is in the first 48 bytes of the message
 byte packetBuffer[ NTP_PACKET_SIZE];
 //buffer to hold incoming and outgoing packets
 // A UDP instance to let us send and receive packets over UDP 
WiFiUDP udp;

int attempts =0;

void setup() {
/* ====== setup section OLED ====== */
  // initialize and clear display
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();
  display.setRotation(rotation);// h 0,2  v 1,3

  // display a line of text
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0,0);
  display.println("NTP Time:");
  display.setTextSize(2);
  display.setCursor(0,15);
  //display.println(ssid);
  //display.println(password);
  display.display();
/* ====== setup section OLED ====== */

 Serial.begin(115200);
 Serial.println();
 Serial.println();
 // We start by connecting to a WiFi network
 Serial.print("Connecting to ");
 Serial.print(ssid);
 Serial.print(" - ");
 Serial.println(pass);
 WiFi.begin(ssid, pass);
 while (WiFi.status() != WL_CONNECTED) {
 delay(500);
 Serial.print(".");
    if (++attempts > 100) {
       display.print ( attempts );
       display.display();
       ESP.restart();
    }
 }
 Serial.println("");
 Serial.println("WiFi connected");
 Serial.println("IP address: ");
 Serial.println(WiFi.localIP());
 Serial.println("Starting UDP");
 udp.begin(localPort);
 Serial.print("Local port: ");
 Serial.println(udp.localPort());
 }

 void loop() {
 //get a random server from the pool

 WiFi.hostByName(ntpServerName, timeServerIP);
 sendNTPpacket(timeServerIP);
 // send an NTP packet to a time server
 // wait to see if a reply is available
 delay(1000);
 int cb = udp.parsePacket();
 if (!cb) {
 Serial.println("no packet yet");
 } else {
 Serial.print("packet received, length=");
 Serial.println(cb);
 // We've received a packet, read the data from it
 udp.read(packetBuffer, NTP_PACKET_SIZE);
 // read the packet into the buffer
 //the timestamp starts at byte 40 of the received packet and is four bytes,
 // or two words, long. First, esxtract the two words:
 unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
 unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
 // combine the four bytes (two words) into a long integer
 // this is NTP time (seconds since Jan 1 1900):
 unsigned long secsSince1900 = highWord << 16 | lowWord;
 Serial.print("\n\nSeconds since Jan 1 1900 = " );
 Serial.println(secsSince1900);
 // now convert NTP time into everyday time:
 Serial.print("Unix time = ");
 // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
 //
 const unsigned long seventyYears = 2208988800UL;
 // subtract seventy years:


/*
        if(digitalRead(pin)){
          int zone = 1;
        }else{
          int zone = 2;
        }
 */

 
 unsigned long epoch = secsSince1900 - seventyYears + ZoneOffset[zone];

 



 
 // print Unix time:
 Serial.println(epoch);
 // print the hour, minute and second:
 Serial.print("The UTC time is ");
 // UTC is the time at Greenwich Meridian (GMT)



  DisplayTime();

 
 Serial.print((epoch % 86400L) / 3600);
 if(((epoch % 86400L) / 3600)<10){

 display.print('0');
  
 }
 if(((epoch % 86400L) / 3600)>12){

 display.print('0');
 display.print(((epoch % 86400L) / 3600)-12);
int pm=1;
  
 }else if((((epoch % 86400L) / 3600)-12)<10){

 display.print(".");
 display.print(((epoch % 86400L) / 3600)-12);
  
 }else{
 display.print((epoch % 86400L) / 3600);
int pm=0;
 }
 // print the hour (86400 equals secs per day)
 Serial.print(':');
 display.print(':');
 if ( ((epoch % 3600) / 60) < 10 ) {
 // In the first 10 minutes of each hour, we'll want a leading '0'
 Serial.print('0');
 display.print('0');
 }
 Serial.print((epoch % 3600) / 60);
 display.print((epoch % 3600) / 60);
 // print the minute (3600 equals secs per minute)
 Serial.print(':');
 display.print(':');
 if ( (epoch % 60) < 10 ) {
 // In the first 10 seconds of each minute, we'll want a leading '0'
 Serial.print('0');
 display.print('0');
 }
 Serial.println(epoch % 60);
 display.print(epoch % 60);
 if(pm){display.println("p");}
 // print the second
 }
  display.display();
 // wait ten seconds before asking for the time again
 delay(Interval);
 }

 // send an NTP request to the time server at the given address
 unsigned long sendNTPpacket(IPAddress& address) {
 Serial.println("sending NTP packet...");
 // set all bytes in the buffer to 0
 memset(packetBuffer, 0, NTP_PACKET_SIZE);
 // Initialize values needed to form NTP request
 // (see URL above for details on the packets)
 packetBuffer[0] = 0b11100011;
 // LI, Version, Mode
 packetBuffer[1] = 0;
 // Stratum, or type of clock
 packetBuffer[2] = 6;
 // Polling Interval
 packetBuffer[3] = 0xEC;
 // Peer Clock Precision
 // 8 bytes of zero for Root Delay & Root Dispersion
 packetBuffer[12] = 49;
 packetBuffer[13] = 0x4E;
 packetBuffer[14] = 49;
 packetBuffer[15] = 52;
 // all NTP fields have been given values, now
 // you can send a packet requesting a timestamp:
 udp.beginPacket(address, 123);
 //NTP requests are to port 123
 udp.write(packetBuffer, NTP_PACKET_SIZE);
 udp.endPacket();
 }

void DisplayTime(){
  
    display.fillRect(0, display.height()-22, 127, 22, BLACK);// BLACK WHITE
  
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(60,0);
//if(ZoneOffset[zone] == -14400){display.println(ZoneLocation[zone]);}
//if(ZoneOffset[zone] == 10800){display.println("Uganda");}



  display.println(ZoneLocation[zone]);
  
  display.setTextSize(2);
  display.setCursor(0,15);
  
  
  
  // display.display();
  }

/* ===== end of sketch ===== */
