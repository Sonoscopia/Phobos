/*
 * EnsembleOne Prototype for ArduinoMega2560
 *PHOBOS (Sonoscopia)
 *
 * Total outptus: 4 DC motor control and 8 solenoids
 * Instruments:
 * A) 4 motors - mousse bowls
 * B) 3 solenoids - shaker cans
 * C) 3 solenoids - metal cans
 * D) 2 solenoids - woodblocks
*/

// ports
/*
A1 - 2 
A2 - 3
A3 - 4
A4 - 5
B1 - 6
B2 - 7
B3 - 8
C1 - 9
C2 - 10
C3 - 11
D1 - 44 
D2 - 45
*/
byte sbuf[2] = {0, 0}; // serial buffer

void setup(){
  Serial.begin(38400);
  for(int i = 2; i < 12; i++){
    pinMode(i, OUTPUT);  
  }
  pinMode(44, OUTPUT);
  pinMode(45, OUTPUT);
}

void loop(){
  if(Serial.available()){
    if(Serial.readBytes(sbuf, 2 ) == 2){
      if(sbuf[0] < 10){
        analogWrite(sbuf[0]+2, sbuf[1] << 1);
      }
      else {
        analogWrite(sbuf[0]+32, sbuf[1] << 1);
      }
    }
  }
  delay(1);
}



