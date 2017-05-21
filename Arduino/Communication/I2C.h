#ifndef I2C_H
#define I2C_H

#include <Arduino.h>
#include <Wire.h>

typedef void (* func) (int bytes);
typedef void (* func2) ();


class I2C{
    public:
        I2C(int, func2, func);
        ~I2C();
        int getSlave();
        void setSlave(int);
        func2 getTransmitter();
        void setTransmitter(func2);
        func getReceiver();
        void setReceiver(func);
        void setup();
        int slaveAddress;
    private:
        func2 transmitter;
        func receiver;

};
#endif