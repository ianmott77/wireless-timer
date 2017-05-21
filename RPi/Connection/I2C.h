#ifndef I2C_TWO_H
#define I2C_TWO_H

#include <linux/i2c-dev.h>
#include <sys/ioctl.h>

#include "Connection.h"
#include <math.h>

class i2c : public Connection{
	public:
		i2c();
		~i2c();
		bool setup();
		bool send(std::string);
		std::string recieve();
	private:
		int file;
		int address;
		std::string filename;
};

#endif
