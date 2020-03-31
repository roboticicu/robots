/*
 * 
 * 
 *  This sketch sends data via HTTP GET requests to data.sparkfun.com service.
 *
 *  You need to get streamId and privateKey at data.sparkfun.com and paste them
 *  below. Or just customize this script to talk to other HTTP servers.
 *  
 * https://www.esp8266.com/viewtopic.php?p=13812#
 * 
 * 
 * I added code             from  this page..
 * arduinojson.org/v6/example/http-client/
 * 
 */

#include <ESP8266WiFi.h>

//const char* ssid     = "your-ssid";
//const char* password = "your-password";
#include <Logins.h>
/* ====
char ssid[] = "xxxxxxxxxx";// your network SSID (name)
char pass[] = "xxxxxxxxxx";// your network password
==== */

const char* host = "www.robotic.icu";
const char* streamId   = "xxxxxxx";
const char* privateKey = "339412013";

void setup() {
  Serial.begin(115200);
  delay(10);

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

int value = 0;

void loop() {
  delay(1000);
  ++value;

  //Serial.print("connecting to ");
  //Serial.println(host);
 
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
 
  // We now create a URI for the request
  String url = "/robotid.php";
  //url += streamId;
  url += "?id=";
  url += privateKey;
  //url += "&value=";
  //url += value;
 
  //Serial.print("Requesting URL: ");
  //Serial.println(url);
 
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  delay(10);





/*================== Added this from 2nd site ===============*/
  // Check HTTP status
  char status[32] = {0};
  client.readBytesUntil('\r', status, sizeof(status));
  if (strcmp(status, "HTTP/1.1 200 OK") != 0) {
    Serial.print(F("Unexpected response: "));
    //Serial.println(status);
    return;
  }

  // Skip HTTP headers
  char endOfHeaders[] = "\r\n\r\n";
  if (!client.find(endOfHeaders)) {
    Serial.println(F("Invalid response"));
    return;
  }
/*================== Added this from 2nd site ===============*/




 
    //Serial.println("debug 1");
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    //for(int count=1;count <17; count++){
    //Serial.print("debug ");
    //Serial.println(count);
    String line = client.readStringUntil('\n');
    //
    Serial.println(line);
    //}
  delay(10);
  }
 
    //String line = client.readStringUntil('{\r');
    //Serial.println(line);
    //Serial.println("debug 3");
 // Serial.println();
  //Serial.println("closing connection");
}

