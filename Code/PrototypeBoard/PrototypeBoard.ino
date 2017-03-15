/*
 PHOBOS (Sonoscopia)
 #Prototype Board 
 4 DC motor control, 4 servo control, 1 solenoid control
 External Libs: Servo, MIDI 
*/
#define VIN 12 // set value according to external(non-USB) input voltage
#define SMAXV 12 // define solenoid max voltage
#define SMINV 5 // define solenoid min voltage 
#define MMAXV 5 // define motor max voltage
// ports
#define SLNPIN 11
#define SRVPIN1 4 
#define SRVPIN2 5
#define SRVPIN3 6
#define SRVPIN4 7
#define MTRPIN1 2
#define MTRPIN2 3
#define MTRPIN3 44
#define MTRPIN4 46
// MIDI note relative to port
byte notes[] = {2, 3, 44, 46, 4, 5, 6, 7, 11};

boolean incoming;
byte in, num, val, count = 0;

void setup(){
  Serial.begin(9600);
}

void loop(){
  if(Serial.available()){
    in = Serial.read();  
    if(in == 255) incoming = true;
    if(in == 254) { incoming = false; count = 0; }; 
    if(incoming == true){
      switch(count){
        case 0:
          num = in;
          count++;
          Serial.println(num);
        break;
      
        case 1:
          val = in;
          analogWrite(num, velocity2byte(val));
          Serial.println(val);
        break;

        default:
          break; 
      }      
    }
  }
}

// Convert voltages to bytes (0..255)
byte voltage2byte(float v){ 
  return (255*v)/VIN;
}

// Convert velocity to byte
byte velocity2byte(int v){
  return v << 1;
}



