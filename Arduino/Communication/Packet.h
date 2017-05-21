#ifndef PACKET_H
#define PACKET_H

#include <ArduinoJson.h>
#include <Arduino.h>
class Packet{
	public:
		Packet(void*, char *, int);
		Packet(JsonObject&);
        ~Packet();
		int getSize();
		void * getData();
		char * getDataType();
        void toJson(JsonObject&);
		int size;
		void * data;
		char * dataType;
};

#endif
