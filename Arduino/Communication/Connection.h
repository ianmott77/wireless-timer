#ifndef CONNECTION_H
#define CONNECTION_H

#include <Arduino.h>
#include <Wire.h>
#include <string.h>
#include <math.h>
#include "I2C.h"
#include "Packet.h"
#include "tools.h"

typedef void (* receivePack) (Packet *);
typedef Packet * (* sendMessage) ();

static receivePack afterPackCreation;
static sendMessage beforePackCreation;
I2C * newI2C(int, receivePack, sendMessage);
Packet * createPacket(char * input);

void initOutput();
void resetInput();
void resetOutput();
void sendInt(int);
void send();
void receive(int);


#endif