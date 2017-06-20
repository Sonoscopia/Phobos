/* PHOBOS - EnsembleThree firmware v.0.3 (20.06.2017)
 * 
 * using: 
 * Arduino v1.8.0
 * MIDI_Library v.4.3.1 by fortyseveneffects
 * Servo Library v.1.1.2
 *  
 * plays: 
 * A - 1 motor on a typewriter and 2 telephones with speakers controlled by servos
 * B - 2 walkmans (ON/OFF 3V) 
 * C - 2 turntables activated by relays
 */

#include <Servo.h>
#include <MIDI.h>
#define MIDICH 1

// TYPEWRITER AND TELEPHONE'S PINS
#define A1 6
#define A2 7
#define A3 8 
// WAKMANS' PINS
#define B1 4
#define B2 5
// TURNTABLES' PINS
#define C1 2
#define C2 3

#define WALKMAN_VOLTAGE 3

// servos
Servo servo1, servo2; 

// make note to pin array
byte note2pin[38];
byte vel, pitch, val, cc;

MIDI_CREATE_DEFAULT_INSTANCE();

void setup() {
  MIDI.begin(MIDICH); 
  MIDI.turnThruOn();
  
  // set output pins
  pinMode(A1, OUTPUT);
  servo1.attach(A2);
  servo2.attach(A3);
  
  pinMode(B1, OUTPUT);
  pinMode(B2, OUTPUT);
  
  pinMode(C1, OUTPUT);
  pinMode(C2, OUTPUT);
  // turn relays off
  digitalWrite(C1, HIGH); 
  digitalWrite(C2, HIGH);


  // set note2pin array
  note2pin[27] = A1;
  note2pin[1] = A2;
  note2pin[2] = A3;

  note2pin[28] = B1; // NOTE that both walkmans play at the same time!!!
  note2pin[29] = B2;

  note2pin[30] = C1;
  note2pin[31] = C2;
}

void loop() {
  if(MIDI.read()){
      switch(MIDI.getType()){
        case midi::NoteOn:
          pitch = MIDI.getData1();
          vel = MIDI.getData2();
          switch(pitch){
            // activate relays
            case 36:
              digitalWrite(note2pin[pitch], LOW); 
            break;
            case 37:
              digitalWrite(note2pin[pitch], LOW); 
            break;
            
            // activate both walkmans
            case 24:
              analogWrite(note2pin[pitch], voltage2byte(WALKMAN_VOLTAGE));
              analogWrite(note2pin[pitch+1], voltage2byte(WALKMAN_VOLTAGE));
            break;
            
            // activate typewriter motor
            case 0:
              analogWrite(note2pin[pitch], vel << 1);
            break;
            
            default:
            break;
          }
        break;
        
        case midi::NoteOff:
          pitch = MIDI.getData1();
          switch(pitch){
            // deactivate relays
            case 36:
              digitalWrite(note2pin[pitch], HIGH); 
            break;
            case 37:
              digitalWrite(note2pin[pitch], HIGH); 
            break;
            
            // deactivate both walkmans
            case 24:
              analogWrite(note2pin[pitch], LOW);
              analogWrite(note2pin[pitch+1], LOW);
            break;
            
            // deactivate typewriter motor
            case 0:
              analogWrite(note2pin[pitch], LOW);
            break;
            
            default:
            break;
          }
        break;
        
        // CONTROL CHANGE (TELEPHONES)
        case midi::ControlChange:
          cc = MIDI.getData1();
          val = MIDI.getData2();
          if(cc==1 || cc==2) {
            analogWrite(note2pin[cc], val);
          }
      }
  }
}

/***************** FUNTCIONS *****************/
byte voltage2byte(float v){
  return v*255/5.f;
} 

float midi2angle(byte m){
  return 180/127.f*m;
}
