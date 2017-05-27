/* PHOBOS - EnsembleSeven firmware v.0.2 (27.05.2017)
 * 
 * using: 
 * Arduino v1.8.0
 * MIDI_Library v.4.3.1 by fortyseveneffects
 * Servo Library v.1.1.2
 * EEPROM Library v.2.0.0
 *  
 * plays: 
 * A - 2 motors rubbing strings
 * B - 4 solenoids plucking strings 
 * C - 4 solenoids plucking strings
 * D - 2 servos plucking strings
 */

#include <EEPROM.h>
#include <Servo.h>
#include <MIDI.h>
#define MIDICH 7

// MOTORS PINS
#define A1 9
#define A2 10
// SOLENOIDS_1 PINS
#define B1 5
#define B2 6
#define B3 7
#define B4 8
// SOLENOIDS_2 PINS
#define C1 44
#define C2 2
#define C3 3
#define C4 4
// SERVOS' PINS
#define D1 45
#define D2 46

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
float servoPos[2]; // servo current position
float servoInc[2]; //servo increment value which relates to movement speed
float servoMinInc[2]; // servo min increment values 
float servoMaxInc[2]; // servo min increment values 
float servoMinAng[2]; // servo min angle
float servoMaxAng[2]; // servo max angle 

// make note to pin array
byte note2pin[38];
byte vel, pitch, val, cc;

MIDI_CREATE_DEFAULT_INSTANCE();

void setup() {
  MIDI.begin(MIDICH); // Launch MIDI and listen to channel 1
  MIDI.turnThruOn();
  
  // set output pins
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  
  pinMode(B1, OUTPUT);
  pinMode(B2, OUTPUT);
  pinMode(B3, OUTPUT);
  pinMode(B4, OUTPUT);
  
  pinMode(C1, OUTPUT);
  pinMode(C2, OUTPUT);
  pinMode(C3, OUTPUT);
  pinMode(C4, OUTPUT);
  
  servo1.attach(D1);
  servo2.attach(D2);
  
  // set servos to initial position
  servoPos[0] = midi2angle(EEPROM.read(S1MINANG));
  servo1.write(servoPos[0]);
  
  servoPos[1] = midi2angle(EEPROM.read(S2MINANG));
  servo2.write(servoPos[1]);
  
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
  note2pin[1] = A1;
  note2pin[3] = A2;

  note2pin[0] = B1;
  note2pin[2] = B2; 
  note2pin[12] = B3; 
  note2pin[13] = B4; 

  note2pin[25] = C1;
  note2pin[26] = C1;
  note2pin[27] = C1;
  note2pin[36] = C1;

  note2pin[14] = D1;
  note2pin[24] = D2;
  //note2pin[37] —> there was no servo for this!!!
}

void loop() {
  if(MIDI.read()){
      switch(MIDI.getType()){
        case midi::NoteOn:
          pitch = MIDI.getData1();
          vel = MIDI.getData2();
          switch(pitch){
            // set servos' values only (activation is done in runServos function)
            case 14:
              servoInc[0] = velocity2inc( servoMinInc[0], servoMaxInc[0], vel);
              servoState[0] = true;
            break;
            case 24:
              servoInc[1] = velocity2inc( servoMinInc[1], servoMaxInc[1], vel);
              servoState[1] = true; 
            break;
            // activate motors and solenoids
            default:
              analogWrite(note2pin[pitch], vel << 1); 
            break;
          }

        case midi::NoteOff:
          pitch = MIDI.getData1();
          switch(pitch){
            // deactivate servos
            case 14:
              servoState[0] = false;
            break;
            case 24:
              servoState[1] = false;
            break;
            // deactivate motors and solenoids
            default:
              analogWrite(note2pin[pitch], LOW);
            break;  
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
  delay(1); // (used only to set servo increments in a millisecond basis)
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
  else if (servoState[0] == false){
    servoPos[0] = servoMinAng[0]; 
    servo1.write( servoPos[0] );  
  }

  // servo2 attack
  if( servoState[1]==true && servoPos[1] < servoMaxAng[1] ){
    servo2.write(servoPos[1]);
    servoPos[1] += servoInc[1];
  }
  // servo2 release  
  else if (servoState[1] == false){
    servoPos[1] = servoMinAng[1]; 
    servo2.write( servoPos[1] );  
  }
}

void updateServoParams(byte addr, byte v){
  switch(addr){
    case 120:
      servoMinInc[0] = v;
      break;
    case 121: 
      servoMaxInc[0] = v;
      break;
    case 122:
      servoMinInc[1] = v;
      break;
    case 123:
      servoMaxInc[1] = v;
      break;
    case 124:
      servoMinAng[0] = midi2angle(v);
      break;
    case 125: 
      servoMaxAng[0] = midi2angle(v);
      break;
    case 126:
      servoMinAng[1] = midi2angle(v);     
      break;
    case 127: 
      servoMaxAng[1] = midi2angle(v);
      break;

    default:
    break;
  }
}
