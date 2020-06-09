/*
 RandomTest

 Does not work!

*/
/*
int dog = 0;
int cat = 0;
int ant = 0;
int bug = 0;
toInt()

String dog = "1234567890"; 
String cat = "6519208374"; 
String ant = "1902384756"; 
String bug = "5740635241";
*/




int result = 1;
int randomInt = 3;
int tmp=0;
int buf[10];
int last = 0;

void setup() {
  Serial.begin(9600);
  for (int i=7; i>0; i--){
    buf[i]=i;
  }
}

int rndnum(){
  randomInt = random(1,9);
  int test=0;// no match
  
if (last != randomInt){
  for (int i=6; i>0; i--){
        if (buf[i] == randomInt){
          test=1;
        }
  }
  buf[1] = randomInt;
  last = randomInt;
}
    if (test ==1){
        rndnum();  
    }
}// end rndnum


void loop() {
  for (int i=6; i>0; i--){
      buf[i+1] =  buf[i];// shift
  }
  rndnum();


  
Serial.print(randomInt);
Serial.print("< ");
for (int i=1; i<9; i++){
Serial.print(buf[i]);
Serial.print(" ");  
}
Serial.println();
delay(500);
}



/*

  for (int i=1; i<6; i++){
    if (buf[i] == randomInt){
      test = 1;
    }else{
      buf[i] = randomInt;
    }
  }
*/

// result =  dog[ cat[ ant[ bug[ randomInt].toInt()].toInt()].toInt()].toInt();
