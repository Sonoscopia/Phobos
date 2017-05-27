/* Simple Servo Robot that is afraid of light 
 based on Adafruit - Arduino Lesson 14

by Tiago Ângelo - Sonoscopia 11-2016
*/
#include <Servo.h>
#include "mServo.h"
// PINS
#define SERVO 9
#define LED 11
#define LDR 0 
#define POT 5

// angles
#define MAXANG 180
#define MINANG 0
// resolution (ms)
#define DLY 15

Servo servo; 
mServo sv1;

int threshold, darkness; 

void setup() {
  Serial.begin(9600);
  //Servo1
  servo.attach(SERVO);
  sv1 = *new mServo();
  sv1.setResolution(1);

  pinMode(LED, OUTPUT);
  pinMode(SERVO, OUTPUT);
}

void loop() {
  //Servo1
  threshold = analogRead(POT);
  //Serial.print("threshold ");
  //Serial.println(threshold);
  darkness = analogRead(LDR);
  Serial.print("darkness ");
  Serial.println(darkness);
  if(darkness > threshold){
    servo.write(sv1.scan(MINANG, MAXANG));
    digitalWrite(LED, 255-(darkness >> 2));
  }
  else{
    digitalWrite(LED, LOW);
  }
  
  delay(DLY);
}



