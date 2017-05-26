/* PHOBOS - EnsembleOne firmware v.0.1 (16.05.2017)
 *  
 *  
 */

#include <MIDI.h>

// MOUSSE BOWLS' PINS
#define A1 7
#define A2 8
#define A3 9 
#define A4 10
// MEDALS' PINS
#define B1 4
#define B2 5
#define B3 6
// SHAKERS' PINS
#define C1 44
#define C2 2
#define C3 3
// WOOD BLOCKS' PINS
#define D1 46
#define D2 45

// make note to pin array
byte note2pin[38];
byte vel, pitch;

MIDI_CREATE_DEFAULT_INSTANCE();

void setup() {
  MIDI.begin(1); // Launch MIDI and listen to channel 1
  
  // set output pins
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A4, OUTPUT);
  
  pinMode(B1, OUTPUT);
  pinMode(B2, OUTPUT);
  pinMode(B3, OUTPUT);
  
  pinMode(C1, OUTPUT);
  pinMode(C2, OUTPUT);
  pinMode(C3, OUTPUT);
  
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);

  // set note2pin array
  note2pin[0] = A1;
  note2pin[1] = A2;
  note2pin[2] = A3;
  note2pin[3] = A4; 

  note2pin[12] = B1;
  note2pin[13] = B2;
  note2pin[14] = B3;

  note2pin[24] = C1;
  note2pin[25] = C2;
  note2pin[26] = C3;

  note2pin[36] = D1;
  note2pin[37] = D2;
}

void loop() {
  if(MIDI.read()){
      switch(MIDI.getType(){
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
