#ifndef CONNECTION_FACTORY_H
#define CONNECTION_FACTORY_H

#include "I2C.h"
#include "Serial.h"
#include "SPI.h"

class ConnectionFactory{
	public:
		ConnectionFactory();
		~ConnectionFactory();
		Connection * newConnection(ConnectionType);
	private:
	
};

#endif

