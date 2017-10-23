#ifndef SPI_H
#define SPI_H

#include "Connection.h"

class Spi : public Connection{
	public:
		Spi();
		~Spi();
		bool setup();
		ConnectionType getType();
	private:
};

#endif
