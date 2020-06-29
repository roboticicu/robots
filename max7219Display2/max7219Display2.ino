/*
Name: max7219Display
Creator: Marco Arduino channel
Date:    35,175 views•Jan 8, 2017

SourceCode: pastebin.com/raw/V1Qja7Dw

YouTube: youtube.com/watch?v=dzuMXUQwKx8
Title: Nodemcu ( esp8266 ) and max7219 led matrix display

*/
int deviceId = 849081125;
/* 849081125 812385871 */
int msglen = 0;

#include <ESP8266WiFi.h>
#include <Wire.h>  // This library is already built in to the Arduino IDE
#include <SPI.h>
#include <bitBangedSPI.h>
#include <MAX7219_Dot_Matrix.h>
#include <string.h>

//const char* ssid = "network_name"; // put your router name
//const char* password = "password";// put your password 
#include <Logins.h>
/* ====  store content below in: libraries/Logins/Logins.h
char ssid[] = "xxxxxxxxxx";// your network SSID (name)
char pass[] = "xxxxxxxxxx";// your network password
==== */

#include <ArduinoJson.h>

#define MAX(a, b) (( a < b ) ? a : b ) 

/* Globals */
const byte chips = 4; /* how many display modules */
unsigned long lastMoved = 0;
unsigned long MOVE_INTERVAL = 25;  /* mS */
int  messageOffset;
MAX7219_Dot_Matrix display (chips, 2);  /* Chips / LOAD  */
char message[90] = "";
char messx [90] = ""; /* messx */
String me="";

int range = 18;
float batLev = analogRead(A0);
/* int Level = analogRead(A0);
 * const char* host = "api.thingspeak.com";
 */
const char* host = "robotic.icu";
int value = 0;

int find_text(String needle, String haystack,int from) {
    if (haystack.length()<needle.length())
        return -1;

    for (int i = from; (i < haystack.length() - needle.length()); i++)
         if (haystack.substring(i,needle.length()+i) == needle)
             return i;
    return -1;
}

void updateDisplay ()
{
  display.sendSmooth(messx, messageOffset);
  
  // next time show one pixel onwards
  if (messageOffset++ >= (int) (strlen (messx) * 8))
    messageOffset = - chips * 8;
}
 
void setup() {
    display.begin();
    Serial.begin(115200);
    delay(100);
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
    /* read the input on analog pin 0: */
    int sensorValue = analogRead(A0);

    /* Convert the analog reading 
     * (which goes from 0 - 1023)
     * to a voltage (0 - 5V):
    */
    /* float Level = (map(bat, 0, 1023, 0, 5000))/1; */
    float Level = sensorValue * (range / 1023.00);
    /* float Bar=(Level/range)*(display.width()); */
    float Percent=(Level/range)*100;
 
    Serial.print("connecting to ");
    Serial.println(host);
  
    // Use WiFiClient class to create TCP connections
    WiFiClient client;
    const int httpPort = 80;
    if (!client.connect(host, httpPort))
        Serial.println("connection failed");
  
    /* We now create a URI for the request
     * String url = "/apps/thinghttp/send_request?api_key=ZMWY350CCZFKKXAU";
     */
    String url = "/robotid.php?id=";
    url += deviceId;
    url += "&batLevel=";
    url += batLev;
    Serial.print("Requesting URL: ");
    Serial.println(url);
    /* This will send the request to the server */
    client.print(String("GET ") + url + " HTTP/1.0\r\n" +
                 "Host: " + host + "\r\n" + 
                 "Connection: close\r\n\r\n");
    /* delay(2000); */
    for (int i=0;i<2000;i++) {
        delay(1);

        /* update display if time is up */
        if (millis() - lastMoved >= MOVE_INTERVAL) {
            updateDisplay();
            lastMoved = millis();
        }
    }
    me=".";  
    /* Read all the lines of the reply from server and print them to Serial */
    while (client.available()) {
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

        const char * message = doc["message"]; // "Hello mom!"
        /* String msg = doc["message"]; // "Hello mom!" */
        int validData = doc["bit"]; // ""

        if (validData != 1)
            continue;
        /*  disptext = "        ";
         *  disptext += message;
         *  message = disptext;
         */
        strncpy(messx, message, MAX(strlen(message), 90));
        msglen = strlen(message);
        /* for (int i=0; i < 18; i++) {
         *     Serial.print(" ");
         * }
         */
        for (int i=0; i < msglen; i++)
            Serial.print(message[i]);

    
        /*  Serial.println(line); */
        Serial.println();
        Serial.println("closing connection");

        for (int i=0;i<80000;i++) {
            delay(1);
            /* update display if time is up */
            if (millis () - lastMoved >= MOVE_INTERVAL) {
                updateDisplay ();
                lastMoved = millis ();
            }
            /* do other stuff here */    
        }   
}
