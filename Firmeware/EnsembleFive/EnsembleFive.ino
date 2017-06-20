/* PHOBOS - EnsembleFive firmware v.0.3 (20.06.2017)
 * 
 * using: 
 * Arduino v1.8.0
 * MIDI_Library v.4.3.1 by fortyseveneffects
 * Servo Library v.1.1.2
 * EEPROM Library v.2.0.0
 *  
 * plays: 
 * A - 1 motor on a vynil with attached strings
 * B - 1 motor with loose strings 
 * C - 3 servos plucking strings
 * D - 4 lights controlled by relays
 */

#include <EEPROM.h>
#include <Servo.h>
#include <MIDI.h>
#define MIDICH 1

// VM VYNIL'S PINS
#define A1 10
// VM STRINGS' PINS
#define B1 9
// VM SERVOS' PINS
#define C1 6
#define C2 7
#define C3 8
// LIGHTS' PINS
#define D1 2
#define D2 3
#define D3 4
#define D4 5

/* Addresses on Arduino EEPROM for servo control parameters*/
#define S1MININC 0 // servo1 min increment (cc=116)
#define S1MAXINC 1 // servo1 max increment (cc=117)

#define S2MININC 2 // servo2 min increment (cc=118)
#define S2MAXINC 3 // servo2 max increment (cc=119)

#define S3MININC 4 // servo3 min increment (cc=120)
#define S3MAXINC 5 // servo3 max increment (cc=121)


#define S1MINANG 6 // servo1 min angle (cc=122)
#define S1MAXANG 7 // servo1 max angle (cc=123)

#define S2MINANG 8 // servo2 min angle (cc=124)
#define S2MAXANG 9 // servo2 max angle (cc=125)

#define S3MINANG 10 // servo3 min angle (cc=126)
#define S3MAXANG 11 // servo3 max angle (cc=127)

// servos
Servo servo1, servo2, servo3;
bool servoState[3]; // servo state to control servos out of MIDI loop
bool _servoState[3]; // servo previous state
float servoPos[3]; // servo current position
float servoInc[3]; //servo increment value which relates to movement speed
float servoMinInc[3]; // servo min increment values 
float servoMaxInc[3]; // servo min increment values 
float servoMinAng[3]; // servo min angle
float servoMaxAng[3]; // servo max angle 

// make note to pin array
byte note2pin[40];
byte vel, pitch, val, cc;

MIDI_CREATE_DEFAULT_INSTANCE();

void setup() {
  MIDI.begin(MIDICH); 
  MIDI.turnThruOn();
  
  // set output pins
  pinMode(A1, OUTPUT);
  
  pinMode(B1, OUTPUT);
  
  servo1.attach(C1);
  servo2.attach(C2);
  servo3.attach(C3);

  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);
  // turn relays off
  digitalWrite(D1, HIGH); 
  digitalWrite(D2, HIGH);
  digitalWrite(D3, HIGH); 
  digitalWrite(D4, HIGH);
  
  // set servos to initial position
  servoPos[0] = midi2angle(EEPROM.read(S1MINANG));
  servo1.write(servoPos[0]);
  
  servoPos[1] = midi2angle(EEPROM.read(S2MINANG));
  servo2.write(servoPos[1]);
  
  servoPos[2] = midi2angle(EEPROM.read(S3MINANG));
  servo3.write(servoPos[2]);

  // set servos previous state
  _servoState[0] = false;
  _servoState[1] = false;
  _servoState[2] = false;
  
  // set servos' min&max increment values
  // NOTE: min/max inc values are between 0 and 127 (after mapped correspond to 0.1 to 10 degrees per millisecond)
  servoMinInc[0] = EEPROM.read(S1MININC);
  servoMaxInc[0] = EEPROM.read(S1MAXINC);

  servoMinInc[1] = EEPROM.read(S2MININC);
  servoMaxInc[1] = EEPROM.read(S2MAXINC);

  servoMinInc[2] = EEPROM.read(S3MININC);
  servoMaxInc[2] = EEPROM.read(S3MAXINC);
  
  // set servos0 min&max angle
  // NOTE: min/max angle values are already converted to angles
  servoMinAng[0] = midi2angle( EEPROM.read(S1MINANG) );
  servoMaxAng[0] = midi2angle( EEPROM.read(S1MAXANG) );

  servoMinAng[1] = midi2angle( EEPROM.read(S2MINANG) );
  servoMaxAng[1] = midi2angle( EEPROM.read(S2MAXANG) );

  servoMinAng[2] = midi2angle( EEPROM.read(S3MINANG) );
  servoMaxAng[2] = midi2angle( EEPROM.read(S3MAXANG) );
  
  // set note2pin array
  note2pin[43] = A1;

  note2pin[44] = B1;

  note2pin[45] = C1;
  note2pin[46] = C2;
  note2pin[47] = C3;
  
  note2pin[48] = D1;
  note2pin[49] = D2;
  note2pin[50] = D3;
  note2pin[51] = D4;
}

void loop() {
  if(MIDI.read()){
      switch(MIDI.getType()){
        case midi::NoteOn:
          pitch = MIDI.getData1();
          vel = MIDI.getData2();
          
          if(pitch > 23){ // activate servos and relays
            if(pitch < 27){ // set servos' values only (activation is done in runServos function)
              servoInc[pitch-24] = velocity2inc( servoMinInc[pitch-24], servoMaxInc[pitch-24], vel);
              servoState[pitch-24] = true;
              _servoState[pitch-24] = true;
              if(pitch==24) servo1.attach(C1);
              if(pitch==25) servo2.attach(C2);
              if(pitch==26) servo3.attach(C3);
            }
            if(pitch > 35){ // activate relay
              digitalWrite(note2pin[pitch], LOW); 
            }
          }
          else{ // activate motors
            analogWrite(note2pin[pitch], vel << 1); 
          }
          break;

        case midi::NoteOff:
          pitch = MIDI.getData1();
          if(pitch > 23){ 
            if(pitch < 27){ // deactivate servos
              servoState[pitch-24] = false; 
            }
            if(pitch > 35){ // deactivate relays
              digitalWrite(note2pin[pitch], HIGH);  
            }  
          }
          else{ // deactivate motors
            analogWrite(note2pin[pitch], LOW);
          }
          break;
        
        // CONTROL CHANGE (SET SERVO PARAMETERS)
        case midi::ControlChange:
          cc = MIDI.getData1();
          val = MIDI.getData2();
          // starting on cc=116
          EEPROM.write(cc-116, val); // store...
          updateServoParams(cc, val);//...and update
      }
  }
  
  runServos(); // activate servos 
  //delay(1); // (used only to set servo increments in a millisecond basis)
}

/***************** FUNTCIONS *****************/
byte voltage2byte(float v){
  return v*255/5.f;
} 

float midi2angle(byte m){
  return 180/127.f*m;
}

float velocity2inc(byte _min, byte _max, byte v){ // translate velocity values into position increments usign a min/max inc.
  return map(v, 0, 127, _min, _max);   
}

void runServos(){
  // servo1 attack
  if( servoState[0]==true && servoPos[0] < servoMaxAng[0] ){
    servo1.write(servoPos[0]);
    servoPos[0] += servoInc[0];
  }
  // servo1 release  
  else if (servoState[0] == false && _servoState[0] == true){
    servoPos[0] = servoMinAng[0]; 
    servo1.write( servoPos[0] );
    _servoState[0] = false;  
  }

  // servo2 attack
  if( servoState[1]==true && servoPos[1] < servoMaxAng[1] ){
    servo2.write(servoPos[1]);
    servoPos[1] += servoInc[1];
  }
  // servo2 release  
  else if (servoState[1] == false && _servoState[1] == true){
    servoPos[1] = servoMinAng[1]; 
    servo2.write( servoPos[1] );
    _servoState[1] = false;  
  }

  // servo3 attack
  if( servoState[2]==true && servoPos[2] < servoMaxAng[2] ){
    servo3.write(servoPos[2]);
    servoPos[2] += servoInc[2];
  }
  // servo3 release  
  else if (servoState[2] == false && _servoState[2] == true){
    servoPos[2] = servoMinAng[2]; 
    servo3.write( servoPos[2] );
    _servoState[2] = false;  
  }
}

void updateServoParams(byte addr, byte v){
  switch(addr){
    case 100:
      servoMinInc[0] = v;
      break;
    case 101: 
      servoMaxInc[0] = v;
      break;
    case 102:
      servoMinInc[1] = v;
      break;
    case 103:
      servoMaxInc[1] = v;
      break;
    case 104:
      servoMinInc[2] = v;
      break;
    case 105: 
      servoMaxInc[2] = v;
      break;
    case 106:
      servoMinAng[0] = midi2angle(v);
      servo1.write(servoMinAng[0]);
      break;
    case 107: 
      servoMaxAng[0] = midi2angle(v);
      servo1.write(servoMaxAng[0]);
      break;
    case 108:
      servoMinAng[1] = midi2angle(v);
      servo2.write(servoMinAng[1]);     
      break;
    case 109: 
      servoMaxAng[1] = midi2angle(v);
      servo2.write(servoMaxAng[1]);
      break;
    case 110:
      servoMinAng[2] = midi2angle(v);
      servo3.write(servoMinAng[2]);
      break;
    case 111:
      servoMaxAng[2] = midi2angle(v);
      servo3.write(servoMaxAng[2]);
      break;

    default:
    break;
  }
}
