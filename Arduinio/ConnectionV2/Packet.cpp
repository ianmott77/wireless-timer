#include "Packet.h"

Packet::Packet(void * data, DataType dataType, int size, int position){
    this->dataType = dataType;
	this->size = size;
    this->data = malloc(size);
    this->position = position;
    if(this->dataType == LONG){
        memcpy(this->data, (long*) data, size);
    }else if(this->dataType == ULONG){
        memcpy(this->data, (unsigned long*) data, size);
    }else if(this->dataType == INT){
        memcpy(this->data, (int*) data, size);
    }else if(this->dataType == FLOAT){
        memcpy(this->data, (float*)data, size);
    }else if(this->dataType == STRING){
        strcpy((char*)this->data,(char*) data);
    }
}

Packet::~Packet(){
    free(this->data);
    this->data = '\0';
}
