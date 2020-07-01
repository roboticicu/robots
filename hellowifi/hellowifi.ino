/*
 * Name: max7219Display
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

#include <ESP8266Wifi.h>
#include <Logins.h>
#include <ArduinoJson.h>

/* Constants -- Global to this file */
const unsigned long MOVE_INTERVAL = 25;  /* mS */
const unsigned long MESSAGE_INTERVAL = 125; /* ms */
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

char *getMessageFromClient(char *oldMess)
{
    WiFiClient client;
    const char *host = "robotic.icu";
    const int httpPort = 80;
    static unsigned long lastMsg = 0;
    String url = "/robotid.php?id="+deviceId;
    String line;

    if (millis() - lastMsg < MESSAGE_INTERVAL)
        return oldMess;
    lastMoved = millis();
    Serial.print("connecting to ");
    Serial.println(host);

    if (!client.connect(host, httpPort))
        return NULL;

    Serial.print("Requesting URL: ");
    Serial.println(url);
    // This will send the request to the server
    client.print(String("GET ") + url + " HTTP/1.0\r\n" +
                 "Host: " + host + "\r\n" + 
                 "Connection: close\r\n\r\n");
    while (client.connected()) {
        String line;
        const size_t capacity = JSON_OBJECT_SIZE(22) + 1024;
        char *newMessage;
        int validData;
        
        line = client.readStringUntil('\r');
        if (line.startsWith("Content-Length: ")) {
            c_len = line.substring(15).toInt();
        }
        if (line == '\r')
            break;
    }
    if (!client.connected())
        return NULL;
    line = client.readStringUntil('\r');
    client.stop();
    DynamicJsonDocument doc(capacity);
    deserializeJson(doc, line);
    message = doc["message"];
    validData = doc["bit"];

    if (!validData)
        return NULL;
    lastMoved = millis();
    return message;
}

void loop() {
    static char defaultMessage[20] = "no connection";
    static int messageOffset = 0;
    static char message[250] = NULL;
    char *newMessage;
    delay(1);

    newMessage = getMessageFromClient();
    if (newMessage && (newMessage != message) {
        strncpy(message, newMessage, strlen(newMessage));
        messageOffset = 0;
    }
    /* update display if time is up */
    messageOffset = updateDisplay(message, messageOffset);
}