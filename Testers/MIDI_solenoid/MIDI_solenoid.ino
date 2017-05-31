// Simple solenoid test with MIDI

#include <MIDI.h>
#define MIDICH 9

#define PIN 2
byte vel; 

MIDI_CREATE_DEFAULT_INSTANCE();

void setup() {
  MIDI.begin(MIDICH);
  //MIDI.turnThruOn();
  pinMode(PIN, OUTPUT);

}

void loop() {
    if(MIDI.read()){
      switch(MIDI.getType()){
        case midi::NoteOn:
          vel = MIDI.getData2();
          analogWrite(PIN, vel << 1);
        break;

        case midi::NoteOff:
          analogWrite(PIN, LOW);
        break;
      }
  }
}
