#include "I2C.h"

I2C::I2C(int slaveAddress,  func2 transmitter, func receiver){
    this->slaveAddress = slaveAddress;
    this->receiver = receiver;
    this->transmitter = transmitter;
    this->setup();
}

I2C::~I2C(){
    
}

void I2C::setup(){
    Wire.begin(this->slaveAddress);
    Wire.onReceive(this->receiver);
    Wire.onRequest(this->transmitter);
}

