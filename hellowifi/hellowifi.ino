/*
 * Name: hellowifi
 * Variant of: max7219Display
 *
 * Creator: Marco Arduino channel
 * Date:    35,175 views•Jan 8, 2017
 *
 * SourceCode: pastebin.com/raw/V1Qja7Dw
 *
 * YouTube: youtube.com/watch?v=dzuMXUQwKx8
 * Title: Nodemcu ( esp8266 ) and max7219 led matrix display
 */
#include <MAX7219_Dot_Matrix.h>
#include <string.h>

#include <ESP8266WiFi.h>
#include <Logins.h>
#include <ArduinoJson.h>

/* Constants -- Global to this file */
const unsigned long MOVE_INTERVAL = 25;  /* mS */
const unsigned long MESSAGE_INTERVAL = 1000; /* 1 sec */
const int MESSAGE_LENGTH = 250;
const byte chips = 4;

/* Global -- needs to be used in both setup and loop */
MAX7219_Dot_Matrix display(chips, 2);
int deviceId = 849081125;
int batLevel = 0;

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

int getMessageFromClient(char *message)
{
    String oldMessageStr = String(message);

    int responseTime = millis();// <=== fix this =========
    
    int bat = analogRead(A0);             // ===== Keep this =======
    float batLevel = (map(bat, 0, 1023, 0, 5000.000))/1;// ===== Keep this =======

    
    WiFiClient client;
    const char *host = "robotic.icu";
    const int httpPort = 80;
    static unsigned long lastMsg = 0;
    int c_len;
    String url = "/robotid.php?id=";
    url+=deviceId;
    url+="&bat=";
    url+=batLevel;
    url+="&time=";
    url+=responseTime;
    String line = "";


    if (millis() - lastMsg < MESSAGE_INTERVAL)
        return 0;
    lastMsg = millis();
//    Serial.print("connecting to ");
//    Serial.println(host);

    if (!client.connect(host, httpPort)) {
//        Serial.println("Failed to connect host: "+host);// ==== error ========
        return -1;
    }
    //Serial.print("Requesting URL: [");
    //Serial.print(url);
    //Serial.println("]");

    // This will send the request to the server
    client.print(String("GET ") + url + " HTTP/1.0\r\n" +
                 "Host: " + host + "\r\n" + 
                 "Connection: close\r\n\r\n");
    while (client.connected()) {
        line = client.readStringUntil('\r');
        if (line.startsWith("Content-Length: ")) {
            c_len = line.substring(15).toInt();
        }
        if (line == "\r")
            break;
    }
    //Serial.println("DAK -- Line is [ " + line + "]");
   // if (!client.connected()) {
     //      Serial.println("Client disconnected before close.");// ==== error ========
   //     return -1;
   // }
   // line = client.readStringUntil('\r');
  //  /* Got package so make sure we are disconnected and we don't leave broken headers */
   // Serial.println("DAK -- line is now : [" + line + "]");
    client.stop();
    /* 22 is calculated by the amount of extra spaces in the JSON */
    //const size_t capacity = JSON_OBJECT_SIZE(22) + c_len;
    const size_t capacity = JSON_OBJECT_SIZE(22) + 1024;
    DynamicJsonDocument doc(capacity); 
    deserializeJson(doc, line);
    //const String newMessageStr = String(doc["message"]); // "Hello mom!"
    const String newMessageStr = doc["message"]; // "Hello mom!"
    const int validData = doc["bit"];// Brian changed
    if (!validData) {
        Serial.println("Invalid Data Read: " + newMessageStr);
        return -1;
    }
    
    //if (validData) {
    //  Serial.print("\nBDT: ");
    //  Serial.println(newMessageStr);
   // }
    if (newMessageStr.equals(oldMessageStr)) {
        Serial.println("Messages are equal");
        return 0;
    }
    newMessageStr.toCharArray(message, MESSAGE_LENGTH);
    lastMsg = millis();  
   // Serial.println("New message is: [" + String(message)  + "]");// ==== error ========
   
   //Serial.print("message: ");
   //Serial.println(message);
    return strlen(message);
}

void loop() {
    static char defaultMessage[20] = "no connection";
    static int messageOffset = 0;
    static char message[MESSAGE_LENGTH] = "";
    int batLevel= 3.3;

    delay(1);
    /* If getMessageFromClient returns a positive number a new message was gotten, reset the offset */
    if (getMessageFromClient(message) > 0)
        messageOffset = 0;
    /* update display if time is up */
    messageOffset = updateDisplay(message, messageOffset);
}
