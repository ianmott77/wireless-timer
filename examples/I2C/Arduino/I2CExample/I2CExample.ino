#include <Connection.h>

#define SLAVE_ADDRESS 0x40

//reciever function
void receiver(Packet * pack){
  Serial.print("Size: ");
  Serial.println(pack->size);
  if(strcmp(pack->dataType, "int") == 0){
    Serial.println(*(int*)pack->data);
  }else if(strcmp(pack->dataType, "long") == 0){
    Serial.println(*(long*)pack->data);
  }else if(strcmp(pack->dataType , "string") == 0){
    Serial.println((char*)pack->data);
  }
}

//sender function
Packet * sender(){
  char * str = new char[10];
  strcpy(str, "hello wor");
  Packet * p = new Packet(str, (char*) "string", 10);
  delete str;
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
  mem();
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(50);
}
