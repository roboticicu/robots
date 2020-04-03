

/*
 * robotStreamId
 * 
 *  This sketch sends data via HTTP GET requests to data.sparkfun.com service.
 *
 *  You need to get streamId and privateKey at data.sparkfun.com and paste them
 *  below. Or just customize this script to talk to other HTTP servers.
 *  
 * https://www.esp8266.com/viewtopic.php?p=13812#
 * 
 * 
 * I added code from this page..
 * arduinojson.org/v6/example/http-client/
 * 
 * YouTube video weather sketch that uses json
 * youtube.com/watch?v=3U-qbii1saw
 * 
 * arduinojson.org/v6/assistant/
 * 
 * 
 * This is the bot under test:
 * robotic.icu/robot.php?id=934661423
 * 
 * This is the bot json code:
 * robotic.icu/robotid.php?id=934661423
 * 
 * 
 */

#include <ESP8266WiFi.h>
#include <ArduinoJson.h>


//static int savedState = 1;
 int savedState = 1;



//const char* ssid     = "your-ssid";
//const char* password = "your-password";
#include <Logins.h>
/* ==== Logins.h content below...store in libraries
char ssid[] = "xxxxxxxxxx";// your network SSID (name)
char pass[] = "xxxxxxxxxx";// your network password
==== */

const char* host = "www.robotic.icu";
const char* streamId   = "xxxxxxx";
const char* DeviceID = "934661423";// 934661423 286531102

int rotation=0; // display upright horizontal


/* ====== OLED Setup ====== */
#include <SPI.h>
#include <Wire.h>
  #include <Adafruit_SSD1306.h>
//  #include <Adafruit_SSD1306_64.h>
#include <Adafruit_GFX.h>

// OLED display TWI address
#define OLED_ADDR    0x3C
Adafruit_SSD1306 display(-1);

#if (SSD1306_LCDHEIGHT != 32)
//#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif
/* ====== OLED Setup ====== */



int State = 1;
int LED1   = 16 ;// 2 16
int LED2   = 2 ;// 2 16
int i =0;


void setup() {





  
  pinMode(LED1, OUTPUT);
  digitalWrite(LED1, LOW);// turn off
  pinMode(LED2, OUTPUT);
  digitalWrite(LED2, LOW);// turn off
  //
  pinMode(LED_BUILTIN, OUTPUT);
  //
  digitalWrite(LED_BUILTIN, LOW);// turn off
  /* ====== setup section OLED ====== */
  // initialize and clear display
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.setRotation(rotation);// h 0,2  v 1,3

  display.clearDisplay();
  // display a line of text
  display.setTextColor(WHITE,BLACK);
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("DeviceID: ");
  display.println(DeviceID);
  //display.setTextSize(2);
  //display.setCursor(15,10);
  //display.println(ssid);
  //display.println(password);
  display.display();
  /* ====== setup section OLED ====== */










  
  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, pass);
 
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    delay(500);
    Serial.print(".");
  }
  digitalWrite(LED_BUILTIN, HIGH);// turn off

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
  url += DeviceID;
  //url += "&value=";
  //url += value;
 
  //Serial.print("Requesting URL: ");
  //Serial.println(url);
 
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  delay(1);





/*================= Added this from 2nd site ================= */
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
/*================= Added this from 2nd site ================= */

/*
https://www.youtube.com/watch?v=dQyXuFWylm4


    //for(int count=1;count <17; count++){
    //Serial.println(count);
    //}
*/

 
    //Serial.println("debug 1");
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    //Serial.print("debug ");
    String line = client.readStringUntil('\n');
    //    Serial.println(line);


    const size_t capacity = JSON_OBJECT_SIZE(22) + 270;
DynamicJsonDocument doc(capacity);

/*
const char* json = "{\"id\":\"658435603\",\"admin\":\"123456789\",\"user\":\"\",\"name\":\"Prof Ferber Farber\",
\"description\":\"Robotics Debokler\",\"location\":\"Laboratory\",\"type\":\"\",\"controlpanel\":\"8nixi\",
\"access\":\"\",\"online\":\"Online\",\"image\":\"GoFigure.jpg\",\"ip\":\"\",\"batLevel\":\"75\",
\"B1\":\"\",\"B2\":\"\",\"B3\":\"\",\"B4\":\"\",\"B5\":\"\",\"B6\":\"\",\"B7\":\"\",\"B8\":\"\",\"message\":\"Hello mom!\"}";

*/ 
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




    if (P8 == 1) { 
      digitalWrite(LED1, !P1);
      digitalWrite(LED2, !P2);
      displayStat(P1, P2, P3, online);
      }  


      /*
 display.setTextColor(WHITE,BLACK);
  display.setTextSize(1);
  display.setCursor(0,10);
  display.println(name);
   display.print("P1:");
  display.print(P1);
   display.print("  P2:");
  display.print(P2);
 //  display.print("P3:");
  //display.println(P3);
  //display.print(" ");
  display.display();




    if (savedState != P1) { 
      digitalWrite(LED1, !P1); 
      savedState=P1;
      }  
    //    digitalWrite(LED1, !P1);
*/




//displayStat();

 /*
*/ 




  
  //display.print("LED_BUILTIN: ");
  //display.println(LED_BUILTIN);
  //
  //display.print(":");



/*

    if ( State == 0){
      State = 0; 
      display.print("Msg: P0");
    } else  { 
      State = 1; 
      display.print("Msg: P1");
    }
//display.print(str(name) );


*/





//const char *Buttons = P1 + P2 + P3 + P4 + P5 + P6 + P7 + P8;// wouldnt work
//if (Buttons != OldButtons){
//}



//Serial.print("P1 ]");
//Serial.print(P1);
//Serial.println("[");


   // if (P1==0){ State = 0; } else  { State = 1; }
 // digitalWrite(LED1, State);//    LED_BUILTIN







 
//displayStat(P1, P2, online);

Serial.print(">>> ");
Serial.print(P1);
Serial.print(" ");
Serial.print(P2);
Serial.print(" ");
Serial.print(P3);
Serial.print(" ");
Serial.print(P4);
Serial.print(" ");
Serial.print(P5);
Serial.print(" ");
Serial.print(P6);
Serial.print(" ");
Serial.print(P7);
Serial.print(" ");
Serial.println(P8);

    
  delay(10);
  }

  //Serial.println("closing connection");
}


void displayStat(int P1, int P2,  int P3, const char *online) {
  display.setTextColor(WHITE,BLACK);
  display.setTextSize(1);
  display.setCursor(0,10);
  display.println(online);
   display.print("P1:");
  display.print(P1);
   display.print("  P2:");
  display.print(P2);
   display.print("  P3:");
  display.println(P3);
display.display();
}

/*

    display.println(online);
    //display.println("  ");
    if ( P1 == 0){
      //State = 0; 
      display.print("\n P1 0  OFF");
    } else  { 
      //State = 1; 
      display.print("\n P1 1  ON ");
    }
    if ( P2 == 0){
      //State = 0; 
      display.print("\n P2 0  OFF");
    } else  { 
      //State = 1; 
      display.print("\n P2 1  ON ");
    }
      display.print(P1);
      display.print(P2);


*/


// const char * P1 ;

  //display.print("LED_BUILTIN: ");
  //display.println(LED_BUILTIN);
  //   display.print("Msg:");
// 



//display.print(str(name) );
 // const char* OldButtons = Buttons;// LED_BUILTIN
  

