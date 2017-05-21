#include <Connection.h>

#define SLAVE_ADDRESS 0x40

void receiver(Packet * pack){
  Serial.print("Size: ");
  Serial.println(pack->getSize());
  if(pack->getDataType() == "int"){
    Serial.println(*(int*)pack->getData());
  }else if(pack->getDataType() == "long"){
    Serial.println(*(long*)pack->getData());
  }else if(strcmp(pack->getDataType() , "string") == 0){
    Serial.println((char*)pack->getData());
  }
}

Packet * sender(){
  unsigned long m = millis();
  Packet * p = new Packet(&m, (char*) "long", sizeof(m));
  return p;
}

void setup() {
  Serial.begin(9600); // start serial for output
  while (!Serial) {
    // wait serial port initialization
  }
  Serial.println("Ready");
  // put your setup code here, to run once:
  I2C * i2c = newI2C(SLAVE_ADDRESS, receiver, sender);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(50);
}
