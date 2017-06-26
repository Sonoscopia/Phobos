/* PHOBOS - EnsembleNine firmware v.0.3 (20.06.2017)
 * 
 * using: 
 * Arduino v1.8.0
 * MIDI_Library v.4.3.1 by fortyseveneffects
 *  
 * plays: 
 * A - 4 solenoid valves
 * B - 3 solenoid valves
 */

#include <MIDI.h>
#define MIDICH 1
#define START 80 
#define SIZE 7

// SOLENOID VALVES_1 PINS
#define A1 5
#define A2 6
#define A3 7 
#define A4 8
// SOLENOID VALVES_2 PINS
#define B1 2
#define B2 3
#define B3 4

// make note to pin array
byte note2pin[SIZE];
byte vel, pitch;

MIDI_CREATE_DEFAULT_INSTANCE();

void setup() {
  MIDI.begin(MIDICH);
  MIDI.turnThruOn();
  
  // set output pins
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A4, OUTPUT);
  
  pinMode(B1, OUTPUT);
  pinMode(B2, OUTPUT);
  pinMode(B3, OUTPUT);

  // set note2pin array
  note2pin[80-START] = A1;
  note2pin[81-START] = A2;
  note2pin[82-START] = A3;
  note2pin[83-START] = A4; 

  note2pin[84-START] = B1;
  note2pin[85-START] = B2;
  note2pin[86-START] = B3;

}

void loop() {
  if(MIDI.read()){
      switch(MIDI.getType()){
        case midi::NoteOn:
          pitch = MIDI.getData1();
          vel = MIDI.getData2();
          if(pitch >= START && pitch < START+SIZE){
            analogWrite(note2pin[pitch-START], vel << 1);
          }
        break;

        case midi::NoteOff:
          pitch = MIDI.getData1();
          if(pitch >= START && pitch < START+SIZE){
            analogWrite(note2pin[pitch-START], LOW);
          }
        break;
      }
  }
}
