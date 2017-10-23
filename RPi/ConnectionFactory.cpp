#include "ConnectionFactory.h"

ConnectionFactory::ConnectionFactory(){

}

ConnectionFactory::~ConnectionFactory(){

}

Connection * ConnectionFactory::newConnection(ConnectionType type){
	Connection * c;
	if(type == SPI){
		c = new Spi();
	}else if(type == I2C){
		c =  new i2c();
	}else if(type == SERIAL){
		c = new Serial();
	}
	return c;
}
