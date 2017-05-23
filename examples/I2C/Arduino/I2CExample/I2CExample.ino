#include <Connection.h>

#define SLAVE_ADDRESS 0x40

//reciever function
void receiver(Packet * pack){
  Serial.print("Size: ");
  Serial.println(pack->getSize());
  if(strcmp(pack->getDataType(),  "int") == 0){
    Serial.println(*(int*)pack->getData());
  }else if(strcmp(pack->getDataType(), "long") == 0){
    Serial.println(*(long*)pack->getData());
  }else if(strcmp(pack->getDataType() , "string") == 0){
    Serial.println((char*)pack->getData());
  }
}

//sender function
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
  //create a new I2C connecton (int slaveAddress, function reciever, function sender)
  //I2C connection uses events so we use functions as parameters 
  //sender function must return a Packet* 
  //reciever function takes Packet* as parameter and void return
  I2C * i2c = newI2C(SLAVE_ADDRESS, receiver, sender);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(50);
}
