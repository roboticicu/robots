/*
 * IRremoteESP8266: IRrecvDemo - demonstrates receiving IR codes with IRrecv
 * This is very simple teaching code to show you how to use the library.
 * If you are trying to decode your Infra-Red remote(s) for later replay,
 * use the IRrecvDumpV2.ino (or later) example code instead of this.
 * An IR detector/demodulator must be connected to the input kRecvPin.
 * Copyright 2009 Ken Shirriff, http://arcfn.com
 * Example circuit diagram:
 *  https://github.com/crankyoldgit/IRremoteESP8266/wiki#ir-receiving
 * Changes:
 *   Version 0.2 June, 2017
 *     Changed GPIO pin to the same as other examples.
 *     Used our own method for printing a uint64_t.
 *     Changed the baud rate to 115200.
 *   Version 0.1 Sept, 2015
 *     Based on Ken Shirriff's IrsendDemo Version 0.1 July, 2009
 */

#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>

int baud = 115200;
//int irChan = 10;
int irChan, irVol;
String LastResult, LastAction;

/* ====== OLED Setup ====== */
#include <SPI.h>
#include <Wire.h>
int rotation=0; // display upright horizontal
  #include <Adafruit_SSD1306.h>
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 64, &Wire);// 32 or 64 dot mode
#include <Adafruit_GFX.h>

// OLED display TWI address
#define OLED_ADDR    0x3C
//Adafruit_SSD1306 display(-1);

#if (SSD1306_LCDHEIGHT != 32)
//#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif
/* ====== OLED Setup ====== */




// An IR detector/demodulator is connected to GPIO pin 14(D5 on a NodeMCU
// board).
// Note: GPIO 16 won't work on the ESP8266 as it does not have interrupts.
const uint16_t kRecvPin = 14;

IRrecv irrecv(kRecvPin);

decode_results results;

void setup() {
  Serial.begin(baud);

  /* ====== setup section OLED ====== */
  // initialize and clear display
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.setRotation(rotation);// h 0,2  v 1,3
  /* ====== setup section OLED ====== */


clrDisplay();



  
  irrecv.enableIRIn();  // Start the receiver
  while (!Serial)  // Wait for the serial connection to be establised.
    delay(50);
  Serial.println();
  Serial.print("IRrecvDemo is now running and waiting for IR message on Pin ");
  Serial.println(kRecvPin);
}

void loop() {
  if (irrecv.decode(&results)) {
    // print() & println() can't handle printing long longs. (uint64_t)
    serialPrintUint64(results.value, HEX);


/*
    I can't get results.value in a form the OLED likes.
    Which I guess would be a problem for mysql too..
*/
    if (results.value == 0x2FDD827) {// Channel up
      irChan += 1; if(irChan > 600){irChan = 0;}
      LastAction = "Cup";
    }else if (results.value == 0x2FDF807) {// Channel dwn
      irChan -= 1; if(irChan <  0){irChan = 600;}
      LastAction = "Cdn";
    }else if (results.value == 0x2FD58A7) {// Vol up
      irVol += 1; if(irVol > 20){irVol = 20;}
      LastAction = "Vup";
    }else if (results.value == 0x2FD7887) {// Vol dwn
      irVol -= 1; if(irVol <  0){irVol = 0;}
      LastAction = "Vdn";
    }else if (results.value == 0xFFFFFFFFFFFFFFFF) {// Repeat
      LastResult = "FF";
    } 

  if(LastResult == "FF"){
      LastResult = "00";
      if(LastAction == "Cup"){
      irChan += 10; if(irChan > 600){irChan = 0;}
      }
      if(LastAction == "Cdn"){
      irChan -= 10; if(irChan <  0){irChan = 600;}
      }
      if(LastAction == "Vup"){
      irVol += 1; if(irVol > 20){irVol = 20;}
      }
      if(LastAction == "Vdn"){
      irVol -= 1; if(irVol <  0){irVol = 0;}
      }
    

  }



    
    ConnectState(irChan, irVol, LastAction);
    Serial.println("");
    irrecv.resume();  // Receive the next value
  }
  delay(100);
}





void ConnectState(int irChan, int irVol, String LastAction) {

  display.clearDisplay();
  // display a line of text
  display.setTextColor(WHITE,BLACK);
  display.setTextSize(2);
  display.setCursor(0,0);
  display.print("IR Message  ");
  //   int irChan = (results.value, HEX);
  display.setCursor(0,18);
  display.print("Chn:");
  display.println(irChan);
  display.print("Vol:");
  display.println(irVol);
  display.print("Last:");
  display.println(LastAction);
  // display.print(irChan);
  display.display();
}






void clrDisplay(){
  
  display.clearDisplay();
  display.display();
}
