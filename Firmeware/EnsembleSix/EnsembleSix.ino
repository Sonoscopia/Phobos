/* PHOBOS - EnsembleFive firmware v.0.2 (27.05.2017)
 * 
 * using: 
 * Arduino v1.8.0
 * MIDI_Library v.4.3.1 by fortyseveneffects
 * Servo Library v.1.1.2
 * EEPROM Library v.2.0.0
 *  
 * plays: 
 * A - 2 sevos striking pendulums
 * B - 4 lights controlled by relays 
 */

#include <EEPROM.h>
#include <Servo.h>
#include <MIDI.h>
#define MIDICH 6

// PENDULUMS' PINS
#define A1 6
#define A2 7
// LIGHTS' PINS
#define B1 2
#define B2 3
#define B3 4
#define B4 5

/* Addresses on Arduino EEPROM for servo control parameters*/
#define S1MININC 0 // servo1 min increment (cc=120)
#define S1MAXINC 1 // servo1 max increment (cc=121)

#define S2MININC 2 // servo2 min increment (cc=122)
#define S2MAXINC 3 // servo2 max increment (cc=123)

#define S1MINANG 4 // servo1 min angle (cc=124)
#define S1MAXANG 5 // servo1 max angle (cc=125)

#define S2MINANG 6 // servo2 min angle (cc=126)
#define S2MAXANG 7 // servo2 max angle (cc=127)

// servos
Servo servo1, servo2;
bool servoState[2]; // servo state to control servos out of MIDI loop
bool _servoState[2]; // servo previous state
float servoPos[2]; // servo current position
float servoInc[2]; //servo increment value which relates to movement speed
float servoMinInc[2]; // servo min increment values 
float servoMaxInc[2]; // servo min increment values 
float servoMinAng[2]; // servo min angle
float servoMaxAng[2]; // servo max angle 

// make note to pin array
byte note2pin[16];
byte vel, pitch, val, cc;

MIDI_CREATE_DEFAULT_INSTANCE();

void setup() {
  MIDI.begin(MIDICH); 
  MIDI.turnThruOn();
  
  // set output pins
  servo1.attach(A1);
  servo2.attach(A2);
  
  pinMode(B1, OUTPUT);
  pinMode(B2, OUTPUT);
  pinMode(B3, OUTPUT);
  pinMode(B4, OUTPUT);
  // turn relays off
  digitalWrite(B1, HIGH); 
  digitalWrite(B2, HIGH);
  digitalWrite(B3, HIGH); 
  digitalWrite(B4, HIGH);
  
  // set servos to initial position
  servoPos[0] = midi2angle(EEPROM.read(S1MINANG));
  servo1.write(servoPos[0]);
  
  servoPos[1] = midi2angle(EEPROM.read(S2MINANG));
  servo2.write(servoPos[1]);

  // set servos previous state
  _servoState[0] = false;
  _servoState[1] = false;
  
  // set servos' min&max increment values
  // NOTE: min/max inc values are between 0 and 127 (after mapped correspond to 0.1 to 10 degrees per millisecond)
  servoMinInc[0] = EEPROM.read(S1MININC);
  servoMaxInc[0] = EEPROM.read(S1MAXINC);

  servoMinInc[1] = EEPROM.read(S2MININC);
  servoMaxInc[1] = EEPROM.read(S2MAXINC);
  
  // set servos0 min&max angle
  // NOTE: min/max angle values are already converted to angles
  servoMinAng[0] = midi2angle( EEPROM.read(S1MINANG) );
  servoMaxAng[0] = midi2angle( EEPROM.read(S1MAXANG) );

  servoMinAng[1] = midi2angle( EEPROM.read(S2MINANG) );
  servoMaxAng[1] = midi2angle( EEPROM.read(S2MAXANG) );  

  // set note2pin array
  note2pin[0] = A1;
  note2pin[1] = A2;
  
  note2pin[12] = B1;
  note2pin[13] = B2;
  note2pin[14] = B3;
  note2pin[15] = B4;
}

void loop() {
  if(MIDI.read()){
      switch(MIDI.getType()){
        case midi::NoteOn:
          pitch = MIDI.getData1();
          vel = MIDI.getData2();
          
          if(pitch < 2){ // set servos' values only (activation is done in runServos function)            if(pitch < 27){ // 
              servoInc[pitch] = velocity2inc( servoMinInc[pitch], servoMaxInc[pitch], vel);
              servoState[pitch] = true;
              _servoState[pitch] = true;
          }
          else{ // activate relays
            digitalWrite(note2pin[pitch], LOW); 
          }
          break;

        case midi::NoteOff:
          pitch = MIDI.getData1();
          if(pitch < 2){ // deactivate servos
              servoState[pitch] = false; 
          }
          else{ // deactivate relays
            digitalWrite(note2pin[pitch], HIGH);
          }  
          break;
        
        // CONTROL CHANGE (SET SERVO PARAMETERS)
        case midi::ControlChange:
          cc = MIDI.getData1();
          val = MIDI.getData2();
          // starting on cc=120
          EEPROM.write(cc-120, val); // store...
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
}

void updateServoParams(byte addr, byte val){
  switch(addr){
    case 120:
      servoMinInc[0] = val;
      break;
    case 121: 
      servoMaxInc[0] = val;
      break;
    case 122:
      servoMinInc[1] = val;
      break;
    case 123:
      servoMaxInc[1] = val;
      break;
    case 124:
      servoMinAng[0] = midi2angle(val);
      servo1.write(servoMinAng[0]);
      break;
    case 125: 
      servoMaxAng[0] = midi2angle(val);
      servo1.write(servoMaxAng[0]);
      break;
    case 126:
      servoMinAng[1] = midi2angle(val);     
      servo1.write(servoMinAng[1]);
      break;
    case 127: 
      servoMaxAng[1] = midi2angle(val);
      servo1.write(servoMaxAng[1]);
      break;

    default:
    break;
  }
}
