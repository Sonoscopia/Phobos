
#include "mServo.h"
#include "Arduino.h"
//#include <Servo.h>

mServo::mServo(){ // default constructor
    //servo.attach(9);
    angle = 0;
    dir = 1;
    resolution = 1;  
}

void mServo::setStartAngle(int _angle){
  angle = _angle;   
}
void mServo::setResolution(float _r){
  resolution = _r; 
}

float mServo::scan(int minAngle, int maxAngle){
  if(dir == 1){
    if(angle < maxAngle) angle+=resolution;
    else dir = 0; 
  }
  else{
    if(angle > minAngle) angle-=resolution;
    else dir = 1; 
  }
  //servo.write(angle);
  return angle;  
}
