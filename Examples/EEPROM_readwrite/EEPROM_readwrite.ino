#include <EEPROM.h>

int address = 0; 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  EEPROM.write(address, 127);

}

void loop() {
  // put your main code here, to run repeatedly:
  byte val = EEPROM.read(address);
  Serial.println(val);
}
