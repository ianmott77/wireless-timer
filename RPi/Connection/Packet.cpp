#include "Packet.h"

Packet::Packet(void * data, DataType dataType, int size, int position){
	this->dataType = dataType;
	this->size = size;
	this->data = malloc(size);
	this->position = position;
	if(this->dataType == INT){
		memcpy(this->data, (int*) data, size);
	}else if(this->dataType == FLOAT){
		memcpy(this->data, (float*) data, size);
	}else if(this->dataType == LONG){
		memcpy(this->data, (signed long*) data, size);
	}else if(this->dataType == ULONG){
		memcpy(this->data, (unsigned long*) data, size);
	}else if(this->dataType == STRING){
		strcpy((char*)this->data, (char*) data);
	}
}

Packet::~Packet(){
	delete data;
}

void * Packet::getData(){
	return this->data;
}

int Packet::getPosition(){
	return this->position;
}

int Packet::getSize(){
	return this->size;
}

DataType Packet::getDataType(){
	return this->dataType;
}

std::string Packet::toString(){
	char s[10];
	char dt[4];
	char * dat;
	char * out;
	std::string stuff;
	std::string fina;
	sprintf(s, "%i", this->size);
	std::string si = s;
	sprintf(dt, "%i", this->dataType);
	std::string d = dt;
	if(this->dataType == STRING){
		dat = new char[this->size];
		sprintf(dat, "%s", (char*) this->data);
	}else{
		dat = new char[20];
		if(this->dataType == INT){
			sprintf(dat, "%i", *(int*)  this->data);
		}else if(this->dataType == FLOAT){
			sprintf(dat, "%f", *((float*)this->data));
		}else if(this->dataType == LONG){
			sprintf(dat, "%ld", *((signed long*) this->data));
		}else if(this->dataType == ULONG){
			sprintf(dat, "%lu", *((unsigned long*) this->data));
		}
	}
	stuff = dat;
	out = new char[si.length()+d.length()+stuff.length()];
	sprintf(out, "%s%s%s", si.c_str(), d.c_str(), stuff.c_str());
	fina = out;
	return fina;
}
