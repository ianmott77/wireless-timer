#include "I2C.h"

i2c::i2c() : Connection(){
}

i2c::~i2c(){
	
}

bool i2c::setup(){
	this->filename = "/dev/i2c-1";
	this->address = 0x40;
	this->file = open(this->filename.c_str(), O_RDWR);
	if(ioctl(this->file, I2C_SLAVE, this->address) > -1)
		return true;
	return false;
}

ConnectionType i2c::getType(){
	return I2C;
}
