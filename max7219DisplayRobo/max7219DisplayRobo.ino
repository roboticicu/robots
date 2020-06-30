/*
 * Name: max7219DisplayRobo
 * Creator: Marco Arduino channel
 * Date:    35,175 views•Jan 8, 2017
 *
 * SourceCode: pastebin.com/raw/V1Qja7Dw
 *
 * YouTube: youtube.com/watch?v=dzuMXUQwKx8
 * Title: Nodemcu ( esp8266 ) and max7219 led matrix display
 */
   int deviceId = 849081125;
   /* 849081125 812385871


   */

#include <ESP8266WiFi.h>
#include <Logins.h>
/* ====  store content below in: libraries/Logins/Logins.h
char ssid[] = "xxxxxxxxxx";// your network SSID (name)
char pass[] = "xxxxxxxxxx";// your network password
==== */

#include <ArduinoJson.h>

//
const char* host = "robotic.icu";










 
int msglen = 0;

#include <MAX7219_Dot_Matrix.h>
#include <string.h>

#define MAX(a, b) (( a < b ) ? a : b ) 

/* Constants -- Global to this file */
const unsigned long MOVE_INTERVAL = 25;  /* mS */
const byte chips = 4;

/* Global -- needs to be used in both setup and loop */
MAX7219_Dot_Matrix display(chips, 2);

/*
 * updateDisplay: updates the max7219 display with message at new position
 * 
 * @messx:         Message to display.
 * @messageOffset: Offset to display the message at
 * Returns the new offset for the next message to be displayed.
 * 
 * Updates the message with a smooth scroll, won't go faster than
 * MOVE_INTERVAL even if you send another message.
 */ 
int updateDisplay (char *messx, int messageOffset)
{
    static unsigned long lastMoved = 0;

    if (millis() - lastMoved < MOVE_INTERVAL)
        return messageOffset;
    display.sendSmooth(messx, messageOffset);
    // next time show one pixel onwards
    if (messageOffset++ >= (int) (strlen (messx) * 8))
        messageOffset = - chips * 8;
    lastMoved = millis();
    return messageOffset;
}

void setup()
{
    display.begin();


  Serial.begin(115200);
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


  Serial.print("connecting to ");
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
   
  }

  // We now create a URI for the request
//  String url = "/apps/thinghttp/send_request?api_key=ZMWY350CCZFKKXAU";
  String url = "/robotid.php?id=";
  url += deviceId;


  Serial.print("Requesting URL: ");
  Serial.println(url);
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.0\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
 // delay(2000);

/* ============== client connection ================ */
  while(client.available()){
   String line = client.readStringUntil('\r');
   const size_t capacity = JSON_OBJECT_SIZE(22) + 1024;
DynamicJsonDocument doc(capacity);

deserializeJson(doc, line);


const char * id = doc["id"]; // "658435603"
const char * admin = doc["admin"]; // "123456789"
const char * user = doc["user"]; // ""
const char * name = doc["name"]; // "Prof Ferber Farber"
const char * description = doc["description"]; // "Robotics Debokler"
const char * location = doc["location"]; // "Laboratory"
const char * type = doc["type"]; // ""
const char * controlpanel = doc["controlpanel"]; // "8nixi"
const char * access = doc["access"]; // ""
const char * online = doc["online"]; // "Online"
const char * image = doc["image"]; // "GoFigure.jpg"
const char * ip = doc["ip"]; // ""
const char * batLevel = doc["batLevel"]; // "75"
int P1 = doc["B1"]; // ""
int P2 = doc["B2"]; // ""
int P3 = doc["B3"]; // ""
int P4 = doc["B4"]; // ""
int P5 = doc["B5"]; // ""
int P6 = doc["B6"]; // ""
int P7 = doc["B7"]; // ""
int P8 = doc["B8"]; // ""
//
static char  message = doc["message"]; // "Hello mom!"
//String msg = doc["message"]; // "Hello mom!"
int validData = doc["bit"]; // ""



    //static char message[90] = "Hello World";
    //static char message[220] = "For God so loved the world that he gave his one and only Son, that whoever believes in him shall not perish but have eternal life. -- John 3:16 -- ";
    
    //static int messageOffset = 0;
     int messageOffset = 0;

    delay(1);
    /* update display if time is up */
    messageOffset = updateDisplay(message, messageOffset);

  }// end while client

/* ============== client connection ================ */



  
}
