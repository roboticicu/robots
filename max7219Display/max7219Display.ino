/*
Name: max7219Display
Creator: Marco Arduino channel
Date:    35,175 views•Jan 8, 2017

SourceCode: pastebin.com/raw/V1Qja7Dw

YouTube: youtube.com/watch?v=dzuMXUQwKx8
Title: Nodemcu ( esp8266 ) and max7219 led matrix display

*/
#include <ESP8266WiFi.h>
#include <Wire.h>  // This library is already built in to the Arduino IDE

#include <SPI.h>
#include <bitBangedSPI.h>
#include <MAX7219_Dot_Matrix.h>
const byte chips = 4;//how many display modules
unsigned long lastMoved = 0;
unsigned long MOVE_INTERVAL = 25;  // mS
int  messageOffset;

MAX7219_Dot_Matrix display (chips, 2);  // Chips / LOAD 

char message [90] = "";
String me="";

//const char* ssid = "network_name"; // put your router name
//const char* password = "password";// put your password 
#include <Logins.h>
/* ====  store content below in: libraries/Logins/Logins.h
char ssid[] = "xxxxxxxxxx";// your network SSID (name)
char pass[] = "xxxxxxxxxx";// your network password
==== */
 
//const char* host = "api.thingspeak.com";
//
const char* host = "robotic.icu";

int value = 0;

int find_text(String needle, String haystack,int from) {
  int foundpos = -1;
  if (haystack.length()<needle.length())
    return foundpos;
  for (int i = from; (i < haystack.length() - needle.length()); i++) {
    if (haystack.substring(i,needle.length()+i) == needle) {
      foundpos = i;
      return foundpos;
    }
  }
  return foundpos;
}

void updateDisplay ()
  {
  display.sendSmooth (message, messageOffset);
  
  // next time show one pixel onwards
  if (messageOffset++ >= (int) (strlen (message) * 8))
    messageOffset = - chips * 8;
  }  // end of updateDisplay

 
void setup() {


  
    
  display.begin ();



  
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
  String url = "/robotid.php?id=812385871";
  Serial.print("Requesting URL: ");
  Serial.println(url);
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.0\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
 // delay(2000);
  for (int i=0;i<2000;i++)
  {
  delay(1);


  // update display if time is up
  if (millis () - lastMoved >= MOVE_INTERVAL)
    {
    updateDisplay ();
    lastMoved = millis ();
    }


  
  }






  
  me="_";  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
   String line = client.readStringUntil('\r');
   

 

int start_loc= find_text("description",line,0);// price.0
if (start_loc>0) start_loc+=8;
int end_loc=find_text(",",line,0);// </span>


if (start_loc>0 && end_loc>0)
{
  Serial.println("price:  ");

for (int i=start_loc+3;i<end_loc;i++)
{
Serial.print(line[i]);
me+=line[i];

}
Serial.println("");
me+=" c: ";
int start_loc2= find_text("id",line,end_loc+1)+6;// price.1

 //Serial.println(line);
int end_loc2=find_text(",",line,end_loc+1);// </span
if (start_loc2>0 && end_loc2>0)
{
  
  Serial.println("views:");
for (int i=start_loc2+3;i<end_loc2;i++)
{
 
Serial.print(line[i]);
me+=line[i];
}
}


}
    
 //   Serial.println(line);
  }
  
  Serial.println();
  Serial.println("closing connection");

for (int i=0;i<90;i++)
message[i]=me[i];



for(int i=0;i<80000;i++)
{
  delay(1);
   // update display if time is up
  if (millis () - lastMoved >= MOVE_INTERVAL)
    {
    updateDisplay ();
    lastMoved = millis ();
    }

  // do other stuff here    
}
 
  
 
  
 
  
 
  
 
 
}
