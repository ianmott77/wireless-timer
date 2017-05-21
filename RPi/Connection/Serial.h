#ifndef SERIAL_H
#define SERIAL_H

#include "Connection.h"

class Serial : public Connection{
	public:
		Serial();
		~Serial();
		bool setup();
		bool send(std::string);
		std::string recieve();
	private:
};

#endif
