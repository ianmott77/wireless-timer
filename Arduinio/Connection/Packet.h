#ifndef PACKET_H
#define PACKET_H

#include <Arduino.h>
enum DataType{INT = 0, FLOAT = 1, LONG = 2, ULONG = 3, STRING = 4};

class Packet{
	public:
		Packet(void*, DataType, int, int);
        ~Packet();
		int size;
        int position;
		void * data;
		DataType dataType;
};

#endif
