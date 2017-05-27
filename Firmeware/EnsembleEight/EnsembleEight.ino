/* PHOBOS - EnsembleEight firmware v.0.2 (27.05.2017)
 * 
 * using: 
 * Arduino v1.8.0
 * MIDI_Library v.4.3.1 by fortyseveneffects
 *  
 * plays: 
 * A - 3 motors 
 * B - 3 fans
 * C - 3 relays controlling aquarium motors
 */

#include <MIDI.h>
#define MIDICH 8

// MOTORS' PINS
#define A1 8
#define A2 9
#define A3 10
// FANS' PINS
#define B1 5
#define B2 6
#define B3 7
// RELAYS' PINS
#define C1 46
#define C2 45
#define C3 44

// make note to pin array
byte note2pin[25];
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
  
  pinMode(C1, OUTPUT);
  pinMode(C2, OUTPUT);
  pinMode(C3, OUTPUT);
  // turn relays off
  digitalWrite(C1, HIGH); 
  digitalWrite(C2, HIGH);
  digitalWrite(C3, HIGH); 

  
  
  // set note2pin array
  note2pin[0] = A1;
  note2pin[2] = A2;
  note2pin[7] = A3;

  note2pin[1] = B1;
  note2pin[12] = B2;
  note2pin[24] = B3;

  note2pin[13] = C1;
  note2pin[14] = C2;
  note2pin[3] = C3;

}

void loop() {
  if(MIDI.read()){
      switch(MIDI.getType()){
        case midi::NoteOn:
          pitch = MIDI.getData1();
          vel = MIDI.getData2();
          if(pitch == 3 || pitch == 13 || pitch == 14){
            digitalWrite(note2pin[pitch], LOW);
          }
          else{
            analogWrite(note2pin[pitch], vel << 1);
          }
        break;

        case midi::NoteOff:
          pitch = MIDI.getData1();
          if(pitch == 3 || pitch == 13 || pitch == 14){
            digitalWrite(note2pin[pitch], HIGH);
          }
          else{
            analogWrite(note2pin[pitch], LOW);
          }
        break;
      }
  }
}
