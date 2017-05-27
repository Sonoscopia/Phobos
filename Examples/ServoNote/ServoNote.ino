#include <Servo.h>

#define BPIN 7
#define S1PIN 6
#define POSMIN 0
#define POSMAX 180

bool button, pbutton = 0; 
Servo servo1;
float pos = POSMIN;
float posinc = 1.f; 
int counter = 0;
 
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(BPIN, INPUT);
  servo1.attach(S1PIN);
}

void loop() {
  // put your main code here, to run repeatedly:
  button = digitalRead(BPIN);
  /*if (pbutton!=button){
    pbutton = button;
    Serial.println(button);
  }*/

  if(button==1 && pos < POSMAX){
    servo1.write(pos);
    pos += posinc;
  }  
  else if (button == 0){
    pos = POSMIN; 
    servo1.write(pos);  
  }

  delay(1);
}
