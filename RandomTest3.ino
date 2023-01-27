/* RandomTest3
  ESP8266 BlinkWithoutDelay by Simon Peter
  Blink the blue LED on the ESP-01 module
  Based on the Arduino Blink without Delay example
  This example code is in the public domain

  The blue LED on the ESP-01 module is connected to GPIO1
  (which is also the TXD pin; so we cannot use Serial.print() at the same time)

  Note that this sketch uses LED_BUILTIN to find the pin with the internal LED
*/


int baud = 19200;// 9600 19200

#ifndef LED_BUILTIN
#define LED_BUILTIN 2
#endif


#ifdef ESP8266 // ESP8266_NODEMCU
const char* device = "esp8266";
#elif ESP32-D0WDQ6
const char* device = "esp32S";
#elif ESP32
const char* device = "esp32";
#elif ESP32S3
const char* device = "esp32 S3";
#endif


int ledState = LOW;

unsigned long previousMillis = 0;
const long interval = 100000;

int randomNum = 0;
int count = 0;
const int testRange = 52;// 20
int numberoftests = 104000000;
int randomSample[testRange+1];
int flag= 0;


int hi = 0;
int lo = 99999999;




void setup() {
  Serial.begin(baud);
  delay(10000);
  pinMode(LED_BUILTIN, OUTPUT);
  LCD_init();         // Init the display
//  Font10();           // Create the custom characters 
}

void LCD_init()
/* Sets the LCD display to a known initialized state */
{
  Serial.write(0x5C);
  Serial.write(0x40);  //Clear Display and Set Font
  Serial.write(0x20);  //Font 0
  Serial.write(0x30);
  
  Serial.write(0x5C);
  Serial.write(0x41);  //Mode
  Serial.write(0x31);  //wrap on, scroll off
  
  Serial.write(0x5C);
  Serial.write(0x5B);  //Display incomming characters   
}


void cls(){
  //
  Serial.write(0x5C);
      Serial.write(0x40);
//      Serial.write(0x20);
      Serial.write(0x30);
      Serial.print("\n\n");
}


void clr(){
  //
  Serial.write(0x5C);
//      Serial.write(0x40);
      Serial.write(0x20);
//      Serial.write(0x30);
//      Serial.print("\n\n");
}
void LCDpos(){
  //
  Serial.write(0x5C);
  Serial.write(0x42);
  Serial.write(0x23);  //Column 0
  Serial.write(0x22);  //Row 0 (Line 1)
}




void loop() {
  
  
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    if (ledState == LOW) {
      ledState = HIGH;  // Note that this switches the LED *off*
    } else {
      ledState = LOW;  // Note that this switches the LED *on*
    }
    digitalWrite(LED_BUILTIN, ledState);
      Serial.println(device);
  }


//  for (int x = 0; x < numberoftests; x++) {
//    
//  }








  if (count  < numberoftests) {  // 1000000
     randomNum = random(0,testRange);// 20
     randomSample[randomNum]++;
    count++;
    flag = 0;
  }else{
    count = 0;
    flag = 1;
    results();
  }










  
//    if(flag = 1) results();
//    if(flag = 1) test(count);

    
    
//  if (randomNum < lo) {lo = randomNum;}
//  if (randomNum > hi) {hi = randomNum;}
//    cls();
//    LCDpos();
//    Serial.print("  device: ");
//    Serial.println(device);
//    LCDpos();
//    Serial.print("\nledState: ");
//    Serial.println(ledState);
  
}

/*
    LCDpos();
    Serial.print("\n\n  random: ");
    Serial.print(randomNum);
    Serial.println("    ");
    LCDpos();
    Serial.print("\n\n\n  hi: ");
    Serial.print(hi);
    Serial.println("    ");
    LCDpos();
    Serial.print("\n\n\n\n  lo: ");
    Serial.print(lo);
    Serial.println("    ");






void test(int count){
  // count numberoftests
    Serial.print("count: ");
    Serial.print(count);
}
*/

void clearArray(){
  for (int x = 0; x <= testRange; x++) {
  randomSample[x] = 0;    
  }
   hi = 0;
   lo = 99999999;
  
}

void results(){
    cls();
    LCDpos();
    Serial.println(" ");
    Serial.println(" ");
  for (int x = 0; x <= testRange-1; x++) {
  if (randomSample[x] < lo) {lo = randomSample[x];}
  if (randomSample[x] > hi) {hi = randomSample[x];}
//    Serial.print("count: ");
//    Serial.print(count);

    Serial.print("Turtle[");
//    Serial.print("randomSample[");
    if(x <10)
    Serial.print(" ");
    Serial.print(x);
    Serial.print("] : ");
//    Serial.print("] steps: ");
    Serial.println(randomSample[x]);
  }
//  clr();
    Serial.print("         hi: ");
    Serial.print(hi);
    Serial.println(" ");
    Serial.print("         lo: ");
    Serial.print(lo);
    Serial.println(" ");
    Serial.print("        ave: ");
    Serial.print((hi+lo)/2);
    Serial.println("  ");
    Serial.print("        dif: ");
    Serial.print(hi-lo);
    Serial.println("  ");
    Serial.print("   aveSteps: ");
    Serial.print((hi-lo)/testRange);
    Serial.println("  ");
  flag = 0;
  clearArray();
    Serial.println(" ");
    Serial.println(" ");
  
    delay(120000);
}
