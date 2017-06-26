/* PHOBOS - EnsembleTwo firmware v.0.3 (20.06.2017)
 * 
 * using: 
 * Arduino v1.8.0
 * MIDI_Library v.4.3.1 by fortyseveneffects
 *  
 * plays: 
 * A - 4 motors on steel tubes
 * B - 4 motors on steel rods
 * C - 4 solenoids on steel bars
 * D - 3 solenoids on two membranes and one metal gong
 */

#include <MIDI.h>
#define MIDICH 1
#define START 12 
#define SIZE 15

// STEEL TUBES' PINS 
#define A1 10
#define A2 11
#define A3 12 
#define A4 13
// STEEL RODS' PINS
#define B1 6
#define B2 7
#define B3 8
#define B4 9
// STEEL BARS' PINS
#define C1 2
#define C2 3
#define C3 4
#define C4 5
// DRUMS&GONG'S PINS
#define D1 46
#define D2 45
#define D3 44

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
  pinMode(B4, OUTPUT);
  
  pinMode(C1, OUTPUT);
  pinMode(C2, OUTPUT);
  pinMode(C3, OUTPUT);
  pinMode(C4, OUTPUT);
  
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  
  // set note2pin array
  note2pin[12-START] = A1;
  note2pin[13-START] = A2;
  note2pin[14-START] = A3;
  note2pin[15-START] = A4; 

  note2pin[16-START] = B1;
  note2pin[17-START] = B2;
  note2pin[18-START] = B3;
  note2pin[19-START] = B4;

  note2pin[20-START] = C1;
  note2pin[21-START] = C2;
  note2pin[22-START] = C3;
  note2pin[23-START] = C4;

  note2pin[24-START] = D1;
  note2pin[25-START] = D2;
  note2pin[26-START] = D3;
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
