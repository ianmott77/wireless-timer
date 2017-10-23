#ifndef SERIAL_H
#define SERIAL_H

#include <termios.h>
#include "Connection.h"


class Serial : public Connection{
	public:
		Serial();
		~Serial();
		bool setup();
		ConnectionType getType();
		int readInt();
		long readLong();
		float readFloat();
};

#endif
