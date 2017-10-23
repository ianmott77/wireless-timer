#ifndef I2C_TWO_H
#define I2C_TWO_H

#include <linux/i2c-dev.h>
#include <sys/ioctl.h>

#include "Connection.h"

class i2c : public Connection{
	public:
		i2c();
		~i2c();
		bool setup();
		ConnectionType getType();
};

#endif
