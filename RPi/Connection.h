#ifndef CONNECTION_H
#define CONNECTION_H

#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <iostream>
#include <termios.h>
#include <math.h>
#include "Packet.h"

class Connection{
	public:
		~Connection();
		virtual bool setup() = 0;
		virtual bool send(Packet*);
		virtual Packet *  receive();
		virtual ConnectionType getType() = 0;
		virtual int readInt();
		virtual long readLong();
		virtual float readFloat();
		virtual void closeConnection();
		virtual int available();
		int file;
		int address;
		std::string filename;
	private:
		std::string readString(int);
		bool send(int);
		bool send(long);
		bool send(float);
		bool send(const char *);
};

#endif
