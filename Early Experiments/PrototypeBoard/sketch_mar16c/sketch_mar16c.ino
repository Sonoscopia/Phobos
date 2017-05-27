byte sbuf[] = {0, 0};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(11, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  while(Serial.available()){
    if(Serial.readBytes(sbuf, 2 ) == 2){
      Serial.print(sbuf[0]);  
      Serial.print(" ");
      Serial.println(sbuf[1]);
    }
  }
  analogWrite(sbuf[0], sbuf[1]<<1);
  delay(1);
  analogWrite(11, LOW);
}
