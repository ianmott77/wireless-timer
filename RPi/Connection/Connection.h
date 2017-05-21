#ifndef CONNECTION_H
#define CONNECTION_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <iostream>

class Connection{
	public:
		~Connection();
		virtual bool setup() = 0;
		virtual bool send(std::string) = 0;
		virtual std::string recieve() = 0;
	private:
};

#endif
