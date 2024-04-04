/*
IoTWeather

WeatherBot
weatherwidget@robotic.icu
9bX6YmdCKQhmF7a

*/




/***************************************************************************************************************
 *  TITLE: ONLINE WEATHER DISPLAY WIDGET: Using The WeMos D1 Mini, OLED Module And OpenWeatherMap
 *  DESCRIPTION: This sketch communicates with and obtains weather informaion from the OpenWeatherMap service. 
 *  It then displays this on an OLED module.
 *
 *  By Frenoy Osburn
 *  YouTube Video: https://youtu.be/3U-qbii1saw
 *  BnBe Post: https://www.bitsnblobs.com/online-weather-widget-using-esp8266
 ***************************************************************************************************************/

/****************************************************************************************************
 *  Board Settings:
 *  Board: "WeMos D1 R1 or Mini"
 *  Upload Speed: "921600"
 *  CPU Frequency: "80MHz"
 *  Flash Size: "4MB (FS:@MB OTA:~1019KB)"
 *  Debug Port: "Disabled"
 *  Debug Level: "None"
 *  VTables: "Flash"
 *  IwIP Variant: "v2 Lower Memory"
 *  Exception: "Legacy (new can return nullptr)"
 *  Erase Flash: "Only Sketch"
 *  SSL Support: "All SSL ciphers (most compatible)"
 *  COM Port: Depends *On Your System*
 **************************************************************************************************************/


#include <AppKey.h>
/* ====  store content below in: libraries/AppKey/AppKey.h
String AppKey = "abcAppdefKeyghi";
==== */


const char * msg = "Visit robotic.icu";//message
//String zipCode = "19355";//Marcus hook
//String zipCode = "19355";//Marcus hook
//String zipCode = "19355";//Marcus hook
//String zipCode = "59639";//Lincoln, Montana
//String zipCode = "60007";//Chicago Illinois
//String zipCode = "99501";//Anchorage alaska
//String zipCode = "19803";//Wilmington
//String zipCode = "19711";//Wilmington
//
String zipCode = "19720";//Newcastle
//
String locCity = "kampala,ug";//Uganda



//
String url= "/data/2.5/weather?zip=" + zipCode + ",us&units=imperial&APPID=" + AppKey;
//String url= "/data/2.5/weather?q=" + locCity + ",us&units=imperial&APPID=" + AppKey;



    //    client.println("GET /data/2.5/weather?q=kampala,ug&units=imperial&APPID=abcAppdefKeyghi HTTP/1.1");



    // int n = url.length(); 
  
    // declaring character array 
    // char char_array[1024]; 
  
    // copying the contents of the 
    // string to char array 
    //strcpy(char_array, url.c_str());
    //url.toCharArray(char_array, 1024);
    
/*

String url= "/data/2.5/weather?zip=";
   url += zipCode;
   url += ",us&units=imperial&APPID=";
   url += AppKey;

*/

 
#ifdef ESP8266
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif
#include <stdio.h>
#include <ArduinoJson.h>
#include <U8g2lib.h>
//#include "credentials.h"

int zipcode = 19720;

#define SUN  0
#define SUN_CLOUD  1
#define CLOUD 2
#define RAIN 3
#define THUNDER 4

//
U8G2_SSD1306_128X64_NONAME_1_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   // All Boards without Reset of the Display

//initiate the WifiClient
WiFiClient client;

//const char* ssid = mySSID;                          //from credentials.h file
//const char* password = myPASSWORD;                  //from credentials.h file


#include <Logins.h>
/* ====  store content below in: libraries/Logins/Logins.h
char ssid[] = "xxxxxxxxxx";// your network SSID (name)
char pass[] = "xxxxxxxxxx";// your network password
==== */



const char* scale = "F";


unsigned long lastCallTime = 0;               // last time you called the updateWeather function, in milliseconds
const unsigned long postingInterval = 30L * 1100L;  // delay between updates, in milliseconds

void setup()
{  
  u8g2.begin();
  
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_ncenB14_tr);
    u8g2.drawStr(0,20,"Online");
    u8g2.drawStr(28,40,"Weather");
    u8g2.drawStr(56,60,"Display");
  } while ( u8g2.nextPage() );
  
  u8g2.enableUTF8Print();   //for the degree symbol
  
  Serial.begin(115200);
  Serial.println("\n\nOnline Weather Display\n");

  Serial.println("Connecting to network");
  WiFi.begin(ssid, pass);

  int counter = 0;
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(200);    
    if (++counter > 100) 
      ESP.restart();
    Serial.print( "." );
  }
  Serial.println("\nWiFi connected");
  printWifiStatus();
}

void loop() 
{    
  if (millis() - lastCallTime > postingInterval) 
  {
    updateWeather();
  }
}

void updateWeather()
{     
  // if there's a successful connection:
  if (client.connect("api.openweathermap.org", 80)) 
  {
    Serial.println("Connecting to OpenWeatherMap server...");//
    // send the HTTP PUT request:metric imperial
    //    client.println("GET /data/2.5/weather?q=kampala,ug&units=imperial&APPID=abcAppdefKeyghi HTTP/1.1");
    //    client.println("GET /data/2.5/weather?q=wilmington,us&units=imperial&APPID=abcAppdefKeyghi HTTP/1.1");
    //    client.println("GET /data/2.5/weather?zip=19720,us&units=imperial&APPID=abcAppdefKeyghi HTTP/1.1");
    // 
    client.println("GET "+url+" HTTP/1.1");
    client.println("Host: api.openweathermap.org");
    client.println("Connection: close");
    client.println();

    // Check HTTP status
    char status[32] = {0};
    client.readBytesUntil('\r', status, sizeof(status));
    // It should be "HTTP/1.0 200 OK" or "HTTP/1.1 200 OK"
    if (strcmp(status + 9, "200 OK") != 0) 
    {
      Serial.print(F("Unexpected response: "));
      Serial.println(status);
      return;
    }

    // Skip HTTP headers
    char endOfHeaders[] = "\r\n\r\n";
    if (!client.find(endOfHeaders)) 
    {
      Serial.println(F("Invalid response"));
      return;
    }

    // Allocate the JSON document
    // Use arduinojson.org/v6/assistant to compute the capacity.
    const size_t capacity = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(1) + 2*JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(4) + 2*JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(13) + 370;
    DynamicJsonDocument doc(capacity);
    
    // Parse JSON object
    DeserializationError error = deserializeJson(doc, client);
    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.c_str());
      return;
    }
        
    int weatherId            = doc["weather"][0]["id"].as<int>();
    float weatherTemperature = doc["main"]["temp"].as<float>();
    int weatherHumidity      = doc["main"]["humidity"].as<int>();
    float weatherFeelsLike   = doc["main"]["pressure"].as<float>();
    char description         = doc["main"]["description"].as<char>();
    //const char* name = doc["name"];
    
JsonObject weather_0 = doc["weather"][0];
int weather_0_id = weather_0["id"]; // 800
const char* weather_0_main = weather_0["main"]; // "Clear"
const char* weather_0_description = weather_0["description"]; // "clear sky"
const char* weather_0_icon = weather_0["icon"]; // "01d"
const char* name = doc["name"]; // "New Castle"


    //Disconnect
    client.stop();
    
    Serial.println(F("Response:"));
    Serial.print("Weather: ");
    Serial.println(weatherId);
    Serial.print("Temperature: ");
    Serial.println(weatherTemperature);
    Serial.print("Humidity: ");
    Serial.println(weatherHumidity);
    Serial.print("FeelsLike: ");
    Serial.println(weatherFeelsLike);
    Serial.print("description: ");
    Serial.println(description);
    Serial.print("name: ");
    Serial.println(name);
    Serial.print("conditions: ");
    Serial.println(weather_0_description);
    Serial.println();
    
    char scrollText[15];
    //sprintf(scrollText, "FeelsLike:%4d% ", weatherFeelsLike);
    //
    sprintf(scrollText, "Humidity:%3d%%", weatherHumidity);
    //    sprintf(scrollText, "Cond: ", weather_0_description);
    //sprintf(scrollText, msg);

    if(weatherId == 800)    //clear
    {
      draw(scrollText, SUN, weatherTemperature);
    }
    else
    {
      switch(weatherId/100)
      {
        case 2:     //Thunderstorm
            draw(scrollText, THUNDER, weatherTemperature);
            break;
    
        case 3:     //Drizzle
        case 5:     //Rain
            draw(scrollText, RAIN, weatherTemperature);
            break;
    
        case 7:     //Sun with clouds
            draw(scrollText, SUN_CLOUD, weatherTemperature);
            break;
        case 8:     //clouds
            draw(scrollText, CLOUD, weatherTemperature);
            break;
        
        default:    //Sun with clouds           
            draw(scrollText, SUN_CLOUD, weatherTemperature);
            break;
      }    
    }
  } 
  else 
  {
    // if you couldn't make a connection:
    Serial.println("connection failed");
  }

  // note the time that this function was called
   lastCallTime = millis();
}

void printWifiStatus() 
{
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void drawWeatherSymbol(u8g2_uint_t x, u8g2_uint_t y, uint8_t symbol)
{
  // fonts used:
  // u8g2_font_open_iconic_embedded_6x_t
  // u8g2_font_open_iconic_weather_6x_t
  // encoding values, see: https://github.com/olikraus/u8g2/wiki/fntgrpiconic
  
  switch(symbol)
  {
    case SUN:
      u8g2.setFont(u8g2_font_open_iconic_weather_6x_t);
      u8g2.drawGlyph(x, y, 69);  
      break;
    case SUN_CLOUD:
      u8g2.setFont(u8g2_font_open_iconic_weather_6x_t);
      u8g2.drawGlyph(x, y, 65); 
      break;
    case CLOUD:
      u8g2.setFont(u8g2_font_open_iconic_weather_6x_t);
      u8g2.drawGlyph(x, y, 64); 
      break;
    case RAIN:
      u8g2.setFont(u8g2_font_open_iconic_weather_6x_t);
      u8g2.drawGlyph(x, y, 67); 
      break;
    case THUNDER:
      u8g2.setFont(u8g2_font_open_iconic_embedded_6x_t);
      u8g2.drawGlyph(x, y, 67);
      break;      
  }
}

void drawWeather(uint8_t symbol, int degree)
{
  drawWeatherSymbol(0, 48, symbol);
  u8g2.setFont(u8g2_font_logisoso32_tf);
  u8g2.setCursor(48+3, 42);
  u8g2.print(degree);
  if(scale=="F"){
  u8g2.print("°F");   // requires enableUTF8Print()
  }else{
  u8g2.print("°C");   // requires enableUTF8Print()
  }
}

/*
  Draw a string with specified pixel offset. 
  The offset can be negative.
  Limitation: The monochrome font with 8 pixel per glyph
*/
void drawScrollString(int16_t offset, const char *s)
{
  static char buf[36];  // should for screen with up to 256 pixel width 
  size_t len;
  size_t char_offset = 0;
  u8g2_uint_t dx = 0;
  size_t visible = 0;
  len = strlen(s);
  if ( offset < 0 )
  {
    char_offset = (-offset)/8;
    dx = offset + char_offset*8;
    if ( char_offset >= u8g2.getDisplayWidth()/8 )
      return;
    visible = u8g2.getDisplayWidth()/8-char_offset+1;
    strncpy(buf, s, visible);
    buf[visible] = '\0';
    u8g2.setFont(u8g2_font_8x13_mf);
    u8g2.drawStr(char_offset*8-dx, 62, buf);
  }
  else
  {
    char_offset = offset / 8;
    if ( char_offset >= len )
      return; // nothing visible
    dx = offset - char_offset*8;
    visible = len - char_offset;
    if ( visible > u8g2.getDisplayWidth()/8+1 )
      visible = u8g2.getDisplayWidth()/8+1;
    strncpy(buf, s+char_offset, visible);
    buf[visible] = '\0';
    u8g2.setFont(u8g2_font_8x13_mf);
    u8g2.drawStr(-dx, 62, buf);
  }
}

void draw(const char *s, uint8_t symbol, int degree)
{
  int16_t offset = -(int16_t)u8g2.getDisplayWidth();
  int16_t len = strlen(s);
  for(;;)
  {
    u8g2.firstPage();
    do {
      drawWeather(symbol, degree);      
      drawScrollString(offset, s);
    } while ( u8g2.nextPage() );
    delay(20);
    offset+=2;
    if ( offset > len*8+1 )
      break;
  }
}
