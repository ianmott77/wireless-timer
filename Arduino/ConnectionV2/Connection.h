#ifndef CONNECTION_H
#define CONNECTION_H

#include <RHReliableDatagram.h>
#include <RH_RF95.h>
#include <Wire.h>
#include <SPI.h>

#include "Packet.h"

#define SLAVE_ADDRESS 0x40
#define RFM95_CS 4
#define RFM95_RST 2
#define RFM95_INT 3


enum Connection{NIL = 0, I2 = 1, SER = 2, SPIC = 3, LORA = 4};

typedef void (* func) ();

typedef void (* receivePack) (Packet *);

typedef Packet * (* sendMessage) ();


static receivePack afterPackCreation;
static sendMessage beforePackCreation;

static func receiveCallback = '\0';
static func sendCallback = '\0';

static Connection currentConnection;
static bool blocking = true;
static int rCount = 0;
static int tCount = 0;
static int size = -1;
static uint8_t _this_address = 0;
static uint8_t _next_address = 0;
static int dataType = -1;
static bool rDone = false;
static bool sDone = false;
static char * input = '\0';
static char * output = '\0';
static Packet * message = '\0';

static RH_RF95 * driver ='\0';
static RHReliableDatagram *  manager = '\0';

int available();
bool send(int);
bool send(long);
bool send(unsigned long);
bool send(char*);
void send();
void receive();
long readLong();
unsigned long readULong();
int readInt();
char * readString(int);
char read();
int write(char);
int write(uint8_t*, int);
void accept();
void transmit();
void setHandlers(receivePack, sendMessage);
void setReceiveHandler(receivePack);
void setSendHandler(sendMessage);
bool switchTo(Connection);
void setSendCallback(func);
void setReceiveCallback(func);
void receiveWire(int);
void sendWire();
void setBlocking(bool);
void setNextAddress(uint8_t);
void setThisAddress(uint8_t);
void m();
#endif;