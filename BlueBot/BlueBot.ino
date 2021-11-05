


/* code
 *  BlueBot
 *  Thu 11/8/2018 7:16 PM
 *  set baud to 57600
 *    
 *  
 *  modified by: Brian Taylor
 *  added OLED
 *  needs to be modified 
 *  clock count by tenths of sec
 *  network updates only during 
 *  startup and every 30 minutes thereafter
 *  
 */

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


const int rightIR = 6;
const int leftIR = 7;


/* ================ motors ================ */
int motor_left[] = {9, 5};
int motor_right[] = {11, 10};

int speakerPin = 8;


void setup() {

    /* ==== OLED start ==== */
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.setRotation(rotation);// h 0,2  v 1,3
    // Clear the buffer.
    display.clearDisplay();
    display.display();
    display.setTextColor(WHITE); // or BLACK);
    display.setTextSize(1);
    display.setCursor(10, 0);
    display.println("BlueBot sounds");
    display.display();
    /* ==== OLED end ==== */



  pinMode(rightIR, INPUT);
  pinMode(leftIR,  INPUT);
  // Setup motors
  int i;
  for(i = 0; i < 2; i++){
  pinMode(motor_left[i], OUTPUT);
  pinMode(motor_right[i], OUTPUT);
  
  pinMode(speakerPin, OUTPUT); // sets the speakerPin to be an output 
  }
}

void loop()
{
 drive_forward();
}

int speed_right(){
  int rspeed = 0;
  if(avg_right() == 0){
    rspeed = 0;
  }
  else if(avg_right() <= 10){
    rspeed = 10;
 }
  else if(avg_right() <= 20){
    rspeed = 20;
 }
  else if(avg_right() <= 30){
    rspeed = 30;
 }
  else if(avg_right() <= 40){
    rspeed = 40;
 }
  else if(avg_right() <= 50){
    rspeed = 50;
 }
  else if(avg_right() <= 60){
    rspeed = 60;
 } 
  else if(avg_right() <= 70){
    rspeed = 70;
 } 
  else if(avg_right() <= 80){
    rspeed = 80;
 } 
  else if(avg_right() <= 90){
    rspeed = 90;
 } 
  else {
    rspeed = 100; 
}
return rspeed;
}

int speed_left(){
  int lspeed = 0;
  if(avg_left() == 0){
    lspeed = 0;
  }
  else if(avg_left() <= 10){
    lspeed = 10;
 }
  else if(avg_left() <= 20){
    lspeed = 20;
 }
  else if(avg_left() <= 30){
    lspeed = 30;
 }
  else if(avg_left() <= 40){
    lspeed = 40;
 }
  else if(avg_left() <= 50){
    lspeed = 50;
 }
  else if(avg_left() <= 60){
    lspeed = 60;
 } 
  else if(avg_left() <= 70){
    lspeed = 70;
 } 
  else if(avg_left() <= 80){
    lspeed = 80;
 } 
  else if(avg_left() <= 90){
    lspeed = 90;
 } 

  else {
    lspeed = 100; 
}
return lspeed;
}

long avg_left(){
  long lsample1 = burst_left();
  delay(20);
  long lsample2 = burst_left();
  delay(20);
  long lsample3 = burst_left();
  delay(20);
  return (lsample1 + lsample2 + lsample3) / 3;
}

long avg_right(){
  long rsample1 = burst_right();
  delay(20);
  long rsample2 = burst_right();
  delay(20);
  long rsample3 = burst_right();
  delay(20);
  return (rsample1 + rsample2 + rsample3) / 3;
}

int burst_right(){
  int r = 0;
  int sample_right = 0;
  while(r < 100) {
    sample_right = sample_right + digitalRead(rightIR);
    r++;
  }
  return sample_right;
}

int burst_left(){
  int l = 0;
  int sample_left = 0;
  while(l < 100) {
    sample_left = sample_left + digitalRead(leftIR);
    l++;
  }
  return sample_left;
}

void drive_forward(){
  int sleft = speed_left();
  int sright = speed_right();
  if(sleft <= 20 && sright <= 20)
  {
   // Stop Motors
   hard_stop();
   delay(1000);
   bonk();
   drive_backward();
   delay(500);
  }
  else
  {
  long pwm_left = sleft * 255 / 100 * 0.5;
  long pwm_right = sright * 255 / 100 * 0.5;
  analogWrite(motor_left[0], pwm_right); 
  analogWrite(motor_left[1], LOW);     
  analogWrite(motor_right[0], pwm_left); 
  analogWrite(motor_right[1], LOW);  
  ping(sleft * 10);
  delay(20);
 }
}

void drive_backward(){
    analogWrite(motor_left[1], 150); 
    analogWrite(motor_left[0], LOW);     
    analogWrite(motor_right[1], 150); 
    analogWrite(motor_right[0], LOW);  
}

void hard_stop(){
analogWrite(motor_left[0], LOW); 
analogWrite(motor_left[1], LOW); 

analogWrite(motor_right[0], LOW); 
analogWrite(motor_right[1], LOW);
}

void beep (unsigned char speakerPin, int frequencyInHertz, long timeInMilliseconds)     // the sound producing function  
{    
          int x;   
          long delayAmount = (long)(1000000/frequencyInHertz); 
          long loopTime = (long)((timeInMilliseconds*1000)/(delayAmount*2)); 
          for (x=0;x<loopTime;x++)   
          {   
              digitalWrite(speakerPin,HIGH); 
              delayMicroseconds(delayAmount); 
              digitalWrite(speakerPin,LOW); 
              delayMicroseconds(delayAmount); 
          }   

}     

void ping(int dly) {
       beep(speakerPin,100,50);
       delay(dly);
       beep(speakerPin,100,50);
       delay(dly);
       beep(speakerPin,100,50);
       delay(dly);
}
     
void scale() 
{    
          beep(speakerPin,2093,500); //C: play the note C (C7 from the chart linked to above) for 500ms 
          beep(speakerPin,2349,500); //D 
          beep(speakerPin,2637,500); //E 
          beep(speakerPin,2793,500); //F 
          beep(speakerPin,3136,500); //G 
          beep(speakerPin,3520,500); //A 
          beep(speakerPin,3951,500); //B 
          beep(speakerPin,4186,500); //C 
}  

void r2D2(){
          beep(speakerPin,3520,100); //A 
          beep(speakerPin,3136,100); //G 
          beep(speakerPin,2637,100); //E 
          beep(speakerPin,2093,100); //C
          beep(speakerPin,2349,100); //D 
          beep(speakerPin,3951,100); //B 
          beep(speakerPin,2793,100); //F 
          beep(speakerPin,4186,100); //C 
          beep(speakerPin,3520,100); //A 
          beep(speakerPin,3136,100); //G 
          beep(speakerPin,2637,100); //E 
          beep(speakerPin,2093,100); //C
          beep(speakerPin,2349,100); //D 
          beep(speakerPin,3951,100); //B 
          beep(speakerPin,2793,100); //F 
          beep(speakerPin,4186,100); //C 
}

void closeEncounters() {
          beep(speakerPin,932,300); //B b
          delay(50);
          beep(speakerPin,1047,300); //C
          delay(50);
          beep(speakerPin,831,300); //A b
          delay(50);
          beep(speakerPin,415,300); //A b
          delay(50);
          beep(speakerPin,622,500); //E b   
          delay(500);     
          
          beep(speakerPin,466,300); //B b
          delay(100);
          beep(speakerPin,524,300); //C
          delay(100);
          beep(speakerPin,415,300); //A b
          delay(100);
          beep(speakerPin,208,300); //A b
          delay(100);
          beep(speakerPin,311,500); //E b   
          delay(500);  
          
          beep(speakerPin,233,300); //B b
          delay(200);
          beep(speakerPin,262,300); //C
          delay(200);
          beep(speakerPin,208,300); //A b
          delay(500);
          beep(speakerPin,104,300); //A bp
          delay(550);
          beep(speakerPin,156,500); //E b      
}

void ariel() {

          beep(speakerPin,1047,300); //C
          delay(50);
          beep(speakerPin,1175,300); //D
          delay(50);
          beep(speakerPin,1245,600); //D#
          delay(250);
          
          beep(speakerPin,1175,300); //D
          delay(50);
          beep(speakerPin,1245,300); //D#
          delay(50);
          beep(speakerPin,1397,600); //F
          delay(250);
          
          beep(speakerPin,1047,300); //C
          delay(50);
          beep(speakerPin,1175,300); //D
          delay(50);
          beep(speakerPin,1245,500); //D#
          delay(50);          
          beep(speakerPin,1175,300); //D
          delay(50);
          beep(speakerPin,1245,300); //D#
          delay(50);             
          beep(speakerPin,1175,300); //D
          delay(50);
          beep(speakerPin,1245,300); //D#
          delay(50);
          beep(speakerPin,1397,600); //F
          delay(50); 

}

void bonk() {
  beep(speakerPin,125,500); //C
  delay(100);
  uhoh();
}

void uhoh() {
   beep(speakerPin,415,100); //C
          delay(80);
    beep(speakerPin,279,100); //C
          delay(80);
}

void squeak() {
 /* for (int i=100; i<3000; i=i*1.05) {
    beep(speakerPin,i,20);
  }
  delay(10);
  for (int i=100; i<200; i=i*1.15) {
    beep(speakerPin,i,60);
  }
  for (int i=400; i>100; i=i*.85) {
    beep(speakerPin,i,60);
  }*/
  for (int i=100; i<5000; i=i*1.45) {
    beep(speakerPin,i,60);
  }
  delay(10);
  for (int i=100; i<6000; i=i*1.5) {
    beep(speakerPin,i,20);
  }
}

void waka() {
  for (int i=1000; i<3000; i=i*1.05) {
    beep(speakerPin,i,10);
  }
  delay(100);
  for (int i=2000; i>1000; i=i*.95) {
    beep(speakerPin,i,10);
  }
    for (int i=1000; i<3000; i=i*1.05) {
    beep(speakerPin,i,10);
  }
  delay(100);
  for (int i=2000; i>1000; i=i*.95) {
    beep(speakerPin,i,10);
  }
    for (int i=1000; i<3000; i=i*1.05) {
    beep(speakerPin,i,10);
  }
  delay(100);
  for (int i=2000; i>1000; i=i*.95) {
    beep(speakerPin,i,10);
  }
    for (int i=1000; i<3000; i=i*1.05) {
    beep(speakerPin,i,10);
  }
  delay(100);
  for (int i=2000; i>1000; i=i*.95) {
    beep(speakerPin,i,10);
  }
}

void catcall() {
  for (int i=1000; i<5000; i=i*1.05) {
    beep(speakerPin,i,10);
  }
 delay(300);
 
  for (int i=1000; i<3000; i=i*1.03) {
    beep(speakerPin,i,10);
  }
  for (int i=3000; i>1000; i=i*.97) {
    beep(speakerPin,i,10);
  }
}

void ohhh() {
  for (int i=1000; i<2000; i=i*1.02) {
    beep(speakerPin,i,10);
  }
  for (int i=2000; i>1000; i=i*.98) {
    beep(speakerPin,i,10);
  }
}



/* ===== end of sketch ===== */
