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
    float batLevel = analogRead(A0);
    WiFiClient client;
    const char *host = "robotic.icu";
    const int httpPort = 80;
    static unsigned long lastMsg = 0;
    int c_len;
    String url = "/robotid.php?id=";
    url+=deviceId;
    url+="&bat=";
    url+=batLevel;
    String line = "";


    if (millis() - lastMsg < MESSAGE_INTERVAL)
        return 0;
    lastMsg = millis();
    Serial.print("connecting to ");
    Serial.println(host);

    if (!client.connect(host, httpPort)) {
//        Serial.println("Failed to connect host: "+host);// ==== error ========
        return -1;
    }
    Serial.print("Requesting URL: [");
    Serial.print(url);
    Serial.println("]");

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
    Serial.println("DAK -- Line is [ " + line + "]");
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
//const char * message = doc["message"]; // "Hello mom!"
    int validData = doc["bit"];// Brian changed
    if (!validData) {
        Serial.println("Invalid Data Read");
        return -1;
    }
    
    if (validData) {
     String message = doc["message"];
      Serial.print("\nBDT: ");
      Serial.println(message);
    }
    if (strncmp(message, doc["message"], MESSAGE_LENGTH) == 0) {
        Serial.println("Messages are equal");
        return 0;
    }
    //    strncpy(message, doc["message"], MESSAGE_LENGTH);
    lastMsg = millis();
   //  
   Serial.println("New message is: [" + String(message)  + "]");// ==== error ========
   
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
/* ======== error report ================

Arduino: 1.8.12 (Linux), Board: "NodeMCU 1.0 (ESP-12E Module), 80 MHz, Flash, Disabled (new can abort), All SSL ciphers (most compatible), 4MB (FS:2MB OTA:~1019KB), 2, v2 Lower Memory, Disabled, None, Only Sketch, 115200"

In file included from /home/brian/.arduino15/packages/esp8266/hardware/esp8266/2.6.3/tools/sdk/libc/xtensa-lx106-elf/include/../include/time.h:15:0,
                 from /home/brian/.arduino15/packages/esp8266/hardware/esp8266/2.6.3/cores/esp8266/HardwareSerial.h:31,
                 from /home/brian/.arduino15/packages/esp8266/hardware/esp8266/2.6.3/cores/esp8266/Arduino.h:244,
                 from sketch/hellowifi.ino.cpp:1:
/home/brian/Arduino/hellowifi/hellowifi.ino: In function 'void loop()':
hellowifi:130:32: error: array must be initialized with a brace-enclosed initializer
     static char message[250] = NULL;  
                                ^
hellowifi:136:47: error: expected ')' before '{' token
     if (newMessage && (newMessage != message) {
                                               ^
In file included from /home/brian/Arduino/libraries/ArduinoJson/src/ArduinoJson/Operators/VariantOr.hpp:9:0,
                 from /home/brian/Arduino/libraries/ArduinoJson/src/ArduinoJson/Operators/VariantOperators.hpp:8,
                 from /home/brian/Arduino/libraries/ArduinoJson/src/ArduinoJson/Variant/VariantRef.hpp:12,
                 from /home/brian/Arduino/libraries/ArduinoJson/src/ArduinoJson/Array/ArrayIterator.hpp:8,
                 from /home/brian/Arduino/libraries/ArduinoJson/src/ArduinoJson/Array/ArrayRef.hpp:8,
                 from /home/brian/Arduino/libraries/ArduinoJson/src/ArduinoJson.hpp:17,
                 from /home/brian/Arduino/libraries/ArduinoJson/src/ArduinoJson.h:9,
                 from /home/brian/Arduino/hellowifi/hellowifi.ino:18:
/home/brian/Arduino/libraries/ArduinoJson/src/ArduinoJson/Variant/VariantAs.hpp: In instantiation of 'T ArduinoJson6152_0000010::variantAs(ArduinoJson6152_0000010::VariantData*, ArduinoJson6152_0000010::MemoryPool*) [with T = char*]':
/home/brian/Arduino/libraries/ArduinoJson/src/ArduinoJson/Variant/VariantRef.hpp:252:37:   required from 'ArduinoJson6152_0000010::VariantRef::operator T() const [with T = char*]'
/home/brian/Arduino/libraries/ArduinoJson/src/ArduinoJson/Object/MemberProxy.hpp:80:30:   required from 'ArduinoJson6152_0000010::MemberProxy<TParent, TStringRef>::operator T() const [with T = char*; TObject = ArduinoJson6152_0000010::JsonDocument&; TStringRef = const char*]'
/home/brian/Arduino/hellowifi/hellowifi.ino:118:16:   required from here
/home/brian/Arduino/libraries/ArduinoJson/src/ArduinoJson/Variant/VariantAs.hpp:93:27: error: invalid conversion from 'ArduinoJson6152_0000010::enable_if<true, const char*>::type {aka const char*}' to 'char*' [-fpermissive]
   return variantAs<T>(data);
                           ^
exit status 1
array must be initialized with a brace-enclosed initializer

This report would have more information with
"Show verbose output during compilation"
option enabled in File -> Preferences.


*/
