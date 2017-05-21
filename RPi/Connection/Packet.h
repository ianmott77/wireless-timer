#ifndef PACKET_H
#define PACKET_H

#include <string>
#include "json/json.hpp"

using json = nlohmann::json;

class Packet{
	public:
		Packet(void*, std::string, int);
		Packet(json);
		~Packet();
		int getSize();
		void * getData();
		std::string getDataType();
		json * toJson();
	private:
		int size;
		void * data;
		std::string dataType;
		json j;
};

#endif
