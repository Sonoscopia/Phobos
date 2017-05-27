/* PHOBOS - EnsembleThree firmware v.0.2 (27.05.2017)
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
#define MIDICH 3

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
  MIDI.begin(MIDICH); // Launch MIDI and listen to channel 1
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
  note2pin[0] = A1;
  note2pin[1] = A2;
  note2pin[2] = A3;

  note2pin[24] = B1; // NOTE that both walkmans play at the same time!!!
  note2pin[25] = B2;

  note2pin[36] = C1;
  note2pin[37] = C2;
}

void loop() {
  if(MIDI.read()){
      switch(MIDI.getType()){
        case midi::NoteOn:
          pitch = MIDI.getData1();
          vel = MIDI.getData2();
          if(pitch == 36 || pitch == 37){ // activate relay
            digitalWrite(note2pin[pitch], LOW);  
          }
          else{
            if(pitch == 24){ // activate both walkmans
              analogWrite(note2pin[pitch], voltage2byte(WALKMAN_VOLTAGE));
              analogWrite(note2pin[pitch+1], voltage2byte(WALKMAN_VOLTAGE));
            }
            else{
              analogWrite(note2pin[pitch], vel << 1); // activate typewriter motor
            }
          }

        case midi::NoteOff:
          pitch = MIDI.getData1();
          if(pitch == 36 || 37){ // deactivate relays
            digitalWrite(pitch, HIGH);
          }
          else{
            analogWrite(note2pin[pitch], LOW);
          }
        break;
        
        // CONTROL CHANGE (TELEPHONES)
        case midi::ControlChange:
          cc = MIDI.getData1();
          val = MIDI.getData2();
          analogWrite(note2pin[cc], val); 
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
