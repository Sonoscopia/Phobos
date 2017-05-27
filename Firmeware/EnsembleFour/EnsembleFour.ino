/* PHOBOS - EnsembleFour firmware v.0.2 (27.05.2017)
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
#define MIDICH 4

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
  MIDI.begin(MIDICH); // Launch MIDI and listen to channel 1
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
  note2pin[0] = A1;
  note2pin[1] = A2;
  note2pin[2] = A3;

  note2pin[12] = B1;
  note2pin[13] = B2;
  note2pin[14] = B3;
  note2pin[15] = B4;

  note2pin[24] = C1;
  note2pin[25] = C2;
  note2pin[26] = C3;
  note2pin[27] = C4;
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
