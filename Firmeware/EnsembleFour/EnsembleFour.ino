/* PHOBOS - EnsembleFour firmware v.0.3 (20.06.2017)
 * 
 * using: 
 * Arduino v1.8.0
 * MIDI_Library v.4.3.1 by fortyseveneffects
 *  
 * plays: 
 * A - 3 motors on mecano pieces playing strings
 * B - 4 motros moving dog toys
 * C - 4 solenoids on a steel triangle loaded with springs
 */

#include <MIDI.h>
#define MIDICH 1

// MECANO'S PINS
#define A1 10
#define A2 11
#define A3 12 

// DOGS' PINS
#define B1 6
#define B2 7
#define B3 8
#define B4 9

// SPRINGS' PINS
#define C1 2
#define C2 3
#define C3 4
#define C4 5

// make note to pin array
byte note2pin[28];
byte vel, pitch;

MIDI_CREATE_DEFAULT_INSTANCE();

void setup() {
  MIDI.begin(MIDICH); 
  MIDI.turnThruOn();
  
  // set output pins
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  
  pinMode(B1, OUTPUT);
  pinMode(B2, OUTPUT);
  pinMode(B3, OUTPUT);
  pinMode(B4, OUTPUT);
  
  pinMode(C1, OUTPUT);
  pinMode(C2, OUTPUT);
  pinMode(C3, OUTPUT);
  pinMode(C4, OUTPUT);

  // set note2pin array
  note2pin[32] = A1;
  note2pin[33] = A2;
  note2pin[34] = A3;

  note2pin[35] = B1;
  note2pin[36] = B2;
  note2pin[37] = B3;
  note2pin[38] = B4;

  note2pin[39] = C1;
  note2pin[40] = C2;
  note2pin[41] = C3;
  note2pin[42] = C4;
}

void loop() {
  if(MIDI.read()){
      switch(MIDI.getType()){
        case midi::NoteOn:
          pitch = MIDI.getData1();
          vel = MIDI.getData2();
          analogWrite(note2pin[pitch], vel << 1);
        break;

        case midi::NoteOff:
          pitch = MIDI.getData1();
          analogWrite(note2pin[pitch], LOW);
        break;
      }
  }
}
