#ifndef PACKET_H
#define PACKET_H

#include <stdio.h>
#include <string.h>
#include <string>
#include <stdlib.h>
#include "defs.h"
/*
 * DataTypes:
 * 0 = int
 * 1 = float
 * 2 = signed long
 * 3 = unsigned long
 * 4 = string
 * * * * * * * * * */
class Packet{
	public:
		Packet(void*, DataType, int, int);
		~Packet();
		int getSize();
		void * getData();
		DataType getDataType();
		std::string toString();
		int getPosition();
	private:
		int size;
		void * data;
		DataType dataType;
		int position;
};

#endif
