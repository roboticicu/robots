/* 
  NTPzonesToRTC
  
  Update: 2018-11-08 7:16 PM Thurs
  Update: 2020-03-21 9:30 PM Sat
  
  
  modified by: Brian Taylor
  added OLED
  needs to be modified 
  clock count by tenths of sec
  network updates only during 
  startup and every 30 minutes thereafter
  
  set baud to 115200
  
 */

 // ==== goto line xxxxy for section that needs fixing ========


 
// #define pin D4 // red
//    pinMode(pin, INPUT_PULLUP);
int pm=0;
int tmd = 12;// 12 24 hour
int baud = 19200;// 115200 19200

/* =========== RTC (set/run) mode =========== */
// int flag = 0;// set to 0 to set time..
// 
int flag = 1;// set to 1 to not set time..
/* =========== RTC (set/run) mode =========== */



int hr;

//#include <TimeLib.h>

#include <uRTCLib.h>
uRTCLib rtc;

/* =========== RTC =========== */





// in seconds
// 
int zone = 4;/* <=== Change this number to choose time zone. ====== */
struct zone {
  int offset;
  char *location;
};

const struct zone zones[13] = {
  {0, ""}, 
  {-36000,"HAWAII"},       /*=== 1 === */
  {-21600,"Early Island"}, /*=== 2 === */
  {-18000,"Easter Island"},/*=== 3 === */
  {-14400,"Delaware"},     /*=== 4 === */
  {-10800,"Brazil"},       /*=== 5 === */
  {0000,  "Gambia"},       /*=== 6 === */
  {3600,  "Spain"},        /*=== 7 === */
  {10800, "Uganda"},       /*=== 8 === */
  {19800, "India"},        /*=== 9 === */
  {28800, "Philippines"},  /*=== 10 === */
  {32400, "Japan"},        /*=== 11 === */
  {39600, "Sydney"}};      /*=== 12 === */

/*
  {10800, "Uganda"},
Easter Island GMT-5 -18000
Hawaii GMT-10 -36000

Delaware GMT -4 -14400 sum
Delaware GMT -5 -18000 win
*/
//  oled_32();
int Interval = 29000;// in milseconds
int rotation=0; // display upright horizontal


/* ====== OLED Setup ====== */
#include <SPI.h>
#include <Wire.h>
  #include <Adafruit_SSD1306.h>
//  #include <Adafruit_SSD1306_64.h>
#include <Adafruit_GFX.h>

// OLED display TWI address
#define OLED_ADDR    0x3C
//Adafruit_SSD1306 display(-1);
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 64, &Wire);// 32 or 64 dot mode

#if (SSD1306_LCDHEIGHT != 32)
//#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif
/* ====== OLED Setup ====== */



/******************** 
- www.geekstips.com - Arduino Time Sync from NTP Server 
using ESP8266 WiFi module - Arduino code example
********************/
//#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>


#include <Logins.h>
/* ====
char ssid[] = "xxxxxxxxxx";// your network SSID (name)
char pass[] = "xxxxxxxxxx";// your network password
==== */

unsigned int localPort = 2390;
// local port to listen for UDP packets
/* Don't hardwire the IP address or we won't get the benefits of the pool.
* Lookup the IP address for the host name instead */

//IPAddress timeServer(129, 6, 15, 28);
// time.nist.gov NTP server 
IPAddress timeServerIP;
// time.nist.gov NTP server address/ pool.ntp.org time.nist.gov
const char* ntpServerName = "pool.ntp.org";
const int NTP_PACKET_SIZE = 48;
// NTP time stamp is in the first 48 bytes of the message
byte packetBuffer[ NTP_PACKET_SIZE];
//buffer to hold incoming and outgoing packets
// A UDP instance to let us send and receive packets over UDP 
WiFiUDP udp;

int attempts =0;

void setup() {
  Serial.begin(baud);
  
   URTCLIB_WIRE.begin();
  rtc.set_rtc_address(0x68);

  
  /* ====== setup section OLED ====== */
  // initialize and clear display
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();
  display.setRotation(rotation);// h 0,2  v 1,3

  // display a line of text
  display.setTextColor(WHITE,BLACK);
  display.setTextSize(1);
  display.setCursor(0,0);
  display.println("NTPTime:");
  display.setTextSize(2);
  display.setCursor(0,15);
  //display.println(ssid);
  //display.println(password);
  display.display();
  /* ====== setup section OLED ====== */

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
      if(attempts && 10){
        DisplayTime();        
          display.setTextSize(1);
          display.print ("Login: ");
          display.setTextSize(2);
          display.print ( attempts );
        display.display();
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

int get_hour_am_pm(unsigned long epoch) { 
int  hour = (epoch % 86400L) / 3600;
  /* Assumes AM/PM not 24 hour clock */ 
  if (hour > 11) {
    pm=1;
    hour-=12;
  }
  if (hour == 0)
    hour = 12;
  //return (hour , pm);
  //
  return hour;
}

void loop() {
  //int pm=0;
  int cb;
  char hour[3];
  unsigned long highWord;
  unsigned long lowWord;
  unsigned long secsSince1900;
  unsigned long epoch;
  // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
  //
  const unsigned long seventyYears = 2208988800UL;
  // subtract seventy years:

  //get a random server from the pool
  WiFi.hostByName(ntpServerName, timeServerIP);
  sendNTPpacket(timeServerIP);
  // send an NTP packet to a time server
  // wait to see if a reply is available
  delay(1000);
  cb = udp.parsePacket();
  if (!cb) {
    Serial.println("no packet yet");
    display.display();
    // wait ten seconds before asking for the time again
    delay(Interval);
    return;
  }
  Serial.print("packet received, length=");
  Serial.println(cb);





  
  // We've received a packet, read the data from it
  udp.read(packetBuffer, NTP_PACKET_SIZE);
  int timestamp1 = millis();
  // read the packet into the buffer
  //the timestamp starts at byte 40 of the received packet and is four bytes,
  // or two words, long. First, esxtract the two words:
  highWord = word(packetBuffer[40], packetBuffer[41]);
  lowWord = word(packetBuffer[42], packetBuffer[43]);
  // combine the four bytes (two words) into a long integer
  // this is NTP time (seconds since Jan 1 1900):
  secsSince1900 = highWord << 16 | lowWord;
  Serial.print("\n\nSeconds since Jan 1 1900 = " );
  Serial.println(secsSince1900);
  // now convert NTP time into everyday time:
  Serial.print("Unix time = ");


  epoch = secsSince1900 - seventyYears + zones[zone].offset;




  
  int secsinyear =  365.25*24*3600;           // seconds in a year
  int epyears    =  secsSince1900/secsinyear;    // total years since 1900
  int epyear     =  secsSince1900/secsinyear-100; // This year
  int dayssince  =  secsSince1900/(24*3600);   // 
  int daysOver   = dayssince - (epyears*365.25);// 
  long months = 30.4375 * 24 * 3600;
  
//  int epmonth =  1451-(epyears * 12);









  

  /*
  if(digitalRead(pin)){
    int zone = 1;
  } else {
    int zone = 2;
  }
  */

  // print Unix time:
  Serial.println(epoch);
  // print the hour, minute and second:
  Serial.print("The UTC time for ");
  Serial.print(zones[zone].location);
  Serial.print(" is ");
  
  // UTC is the time at Greenwich Meridian (GMT)

  DisplayTime();
  sprintf(hour, "%02d", get_hour_am_pm(epoch));
  // 
  Serial.print(hour);


  /*
  Serial.println("..");
  if(((epoch % 86400L) / 3600)<1) {
    Serial.print("..0");
    Serial.print((epoch % 86400L) / 3600);
    display.print("..0");// hours
  }









  if(((epoch % 86400L) / 3600)<10) {
    Serial.print("0");
    Serial.print((epoch % 86400L) / 3600);
    display.print("0");// hours
  }
  if(    (((epoch % 86400L) / 3600)>13)&&((((epoch % 86400L) / 3600)-12)<10)   ) {
    Serial.print(".0");
    Serial.print(((epoch % 86400L) / 3600)-12);
    display.print(".0");// hours
    display.print(((epoch % 86400L) / 3600)-12);
    pm=1;
  } else if((((epoch % 86400L) / 3600)-12)<1) {
    Serial.print("0");
    display.print("0");
    display.print(((epoch % 86400L) / 3600)-12);  
  } else if((((epoch % 86400L) / 3600)-12)<10) {
    Serial.print("0");
    display.print("0");
    display.print(((epoch % 86400L) / 3600)-12);  
  } else {
    display.print((epoch % 86400L) / 3600);
    pm=0;
  }
  
*/


  
  display.print(hour);
  // print the hour (86400 equals secs per day)
  Serial.print(":");
  display.print(":");
  if ( ((epoch % 3600) / 60) < 10 ) {
    // In the first 10 minutes of each hour, we'll want a leading '0'
    Serial.print("0");
    display.print("0");// minutes
  }
  Serial.print((epoch % 3600) / 60);
  display.print((epoch % 3600) / 60);
  // print the minute (3600 equals secs per minute)
  Serial.print(":");
  display.print(":");
  if ( (epoch % 60) < 10 ) {
    // In the first 10 seconds of each minute, we"ll want a leading "0"
    Serial.print("0");
    display.print("0");// seconds
  }
  Serial.print(epoch % 60);
  display.print(epoch % 60);

// 
//  int hr = (epoch % 3600);

       // timestamp1 = millis();// Placed on line after data fetched from server

       // ===== This is the section I can't figure out ============= xxxxy
       rtc.refresh();
       int epmonth = rtc.month();// 12;// (epoch % 12);// not working right
       int yy = rtc.year();// 21;//epyear;// secsSince1900/secsinyear-100;
       int mm = rtc.month();// 1;//epmonth;// don't know how
       int dd = rtc.day();// 4;// don't know how
       int dow = rtc.dayOfWeek();// 1;//dayOfWeek(); // Day of week// don't know how

  int hr = (epoch % 86400L) / 3600;
  int mn = ((epoch % 3600) / 60);
  int sec = (epoch % 60);
       int timestamp2 = millis();
  sec = sec +((timestamp2 - timestamp1)/2); if(sec >60){sec = sec - 60;}

  if (!flag){// xxxxy
    // 
    //yy = 22;
    //mm = 9;
    //dd = 12;
    rtc.set(sec, mn, hr, dow, dd, mm, yy);// This line sets the clock
    flag =  1;
  }




  
   // display.println(pm);
  if (pm) {
    display.print("p");
    Serial.println("pm");
  }else{
    display.print(" ");
    Serial.print("am");
  }
  // print the second
  display.display();

    Serial.print(" Y:");
    Serial.print(epyear);
    Serial.print(" M:");
    Serial.print(epmonth);
    Serial.print(" D:");
    Serial.println(dd);



  rtc.refresh();
  if(sec != rtc.second()) {
    Serial.print(" ntp:");
    Serial.print(sec);
    Serial.print("  rtc:");
    Serial.print(rtc.second());
    Serial.print("  drift:");
    Serial.print(rtc.second()-sec);
    Serial.print("  Timestamp diff:");
    Serial.print(timestamp2 - timestamp1);//
    }else {
    Serial.print(" << match >> ");
    }

//    Serial.print(year(epoch));
//    Serial.print(" ");
//    Serial.print(month(epoch));
//    Serial.print(" ");
//    Serial.print(day(epoch));
//    Serial.print(" ");







  
  // wait ten seconds before asking for the time again
  delay(Interval);
}

// send an NTP request to the time server at the given address
unsigned long sendNTPpacket(IPAddress& address) {
  Serial.println("\n===================================");
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
  //display.fillRect(0, 10, 127, 50, BLACK);// BLACK WHITE
  //display.fillRect(0, display.height()-22, 127, 50, BLACK);// BLACK WHITE
  display.setTextColor(WHITE,BLACK);
  display.setTextSize(1);
  display.setCursor(50,0);
  //display.setCursor(20,45);
  display.println(zones[zone].location);
  //if(ZoneOffset[zone] == -14400){display.println(ZoneLocation[zone]);}
  //if(ZoneOffset[zone] == 10800){display.println("Uganda");}
  
  display.setTextSize(2);
  display.setCursor(0,15);
  // display.display();
  /*
  
    u8g2.setFontDirection(0);
    u8g2.setFont(u8g2_font_inb16_mf);
    u8g2.drawStr(0, 22, zones[zone].location);
  
  */
}



void DisplayDate(){
  display.setTextSize(2);
  display.setCursor(0,15);
//    display.print(yy);
    display.print("-");
//    display.print(mm);
    display.print("-");
//    display.println(dd);
  display.display();
  
}



/* ===== end of sketch ===== */
