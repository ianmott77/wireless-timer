#include "Packet.h"

Packet::Packet(void * data, std::string dataType, int size){
	this->data = data;
	this->dataType = dataType;
	this->size = size;
}

Packet::Packet(json j){
	this->size = j["s"];
	this->dataType = j["dt"];
	if(this->dataType == "string"){
		std::string data = j["d"];
		this->data = (char*)data.c_str();
	}else if(this->dataType == "int" || this->dataType == "long"){
		auto a = j["d"].get_ptr<json::number_integer_t*>();
		this->data = a;
	}else if(this->dataType== "double" || this->dataType == "float"){
		auto a = j["d"].get_ptr<json::number_float_t*>();
		this->data = a;
	}else if(this->dataType == "char"){
		std::string data = j["d"];
		char * str = (char*) data.c_str();
		this->data = &str[0];
	}
}

Packet::~Packet(){}

void * Packet::getData(){
	return this->data;
}

int Packet::getSize(){
	return this->size;
}

std::string Packet::getDataType(){
	return this->dataType;
}

json * Packet::toJson(){
	this->j["s"] = this->size;
	this->j["dt"] = this->dataType;
	if(this->dataType == "string"){
		this->j["d"] = (char *) data;
	}else if(this->dataType == "int"){
		this->j["d"] = *(int*) data;
	}else if(this->dataType == "long"){
		this->j["d"] = *(long*) data;
	}else if(this->dataType== "double"){
		this->j["d"] = *(double*) data;
	}else if(this->dataType == "float"){
		this->j["d"] = *(float*) data;
	}else if(this->dataType == "char"){
		this->j["d"] = *(char*) data;
	}
	return &this->j;
}
