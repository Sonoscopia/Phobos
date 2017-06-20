/* PHOBOS - EnsembleEight firmware v.0.3 (20.06.2017)
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
#define MIDICH 1

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
  MIDI.begin(MIDICH); 
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
  note2pin[71] = A1; // -> motors
  note2pin[73] = A2;
  note2pin[75] = A3;

  note2pin[72] = B1; // -> fans
  note2pin[76] = B2;
  note2pin[79] = B3;

  note2pin[77] = C1; // -> relays
  note2pin[78] = C2;   
  note2pin[74] = C3;

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
