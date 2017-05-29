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
        void setup();
        int slaveAddress;
        func2 transmitter;
        func receiver;

};
#endif