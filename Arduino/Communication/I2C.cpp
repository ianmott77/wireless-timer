#include "I2C.h"

I2C::I2C(int slaveAddress,  func2 transmitter, func receiver){
    this->slaveAddress = slaveAddress;
    this->receiver = receiver;
    this->transmitter = transmitter;
    this->setup();
}

I2C::~I2C(){
    
}

int I2C::getSlave(){
    return this->slaveAddress;
}

void I2C::setSlave(int slave){
    this->slaveAddress = slave;
}

func I2C::getReceiver(){
    return this->receiver;
}

void I2C::setReceiver(func receiver){
    this->receiver = receiver;
}

func2 I2C::getTransmitter(){
    return this->transmitter;
}

void I2C::setTransmitter(func2 transmitter){
    this->transmitter = transmitter;
}

void I2C::setup(){
    Wire.begin(this->slaveAddress);
    Wire.onReceive(this->receiver);
    Wire.onRequest(this->transmitter);
}

