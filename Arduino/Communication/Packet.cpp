#include "Packet.h"

Packet::Packet(void * data, char * dataType, int size){
    this->dataType = dataType;
	this->size = size;
    this->data = malloc(size);
    if(strcmp(this->dataType, "long") == 0){
        memcpy(this->data, (long*) data, size);
    }else if(strcmp(this->dataType, "int") == 0){
        memcpy(this->data, (int*)data, size);
    }else if(strcmp(this->dataType, "double") == 0){
        memcpy(this->data, (double*)data, size);
    }else if(strcmp(this->dataType, "float") == 0){
        memcpy(this->data, (float*)data, size);
    }else if(strcmp(this->dataType, "string") == 0){
        strcpy((char*)this->data,(char*) data);
        delay(10);
    }else if(strcmp(this->dataType, "char") == 0){
        memcpy(this->data, (char*) data, size);
    }
}

Packet::Packet(JsonObject& root){
    const char * str = root["dt"];
    this->dataType = (char*) str;
    this->size = root["s"];
    this->data = malloc(this->size);
    if(strcmp(this->dataType, "int") == 0){
        int d = (int)root["d"];
        memcpy(this->data, &d, size);
        delay(10);
    }else if(strcmp(this->dataType, "string") == 0){
        strcpy((char*)this->data, root["d"]);
        delay(10);
    }else if(strcmp(this->dataType, "long") == 0){
        long d = root["d"];
        memcpy(this->data, &d, size);
        delay(10);
     }else if(strcmp(this->dataType, "double") == 0){
        double d = root["d"];
        memcpy(this->data, &d, size);
        delay(10);
    }else if(strcmp(this->dataType, "float") == 0){
        float d = root["d"];
        memcpy(this->data, &d, size);
        delay(10);
    }else if(strcmp(this->dataType, "char") == 0){
        char d = root["d"];
        memcpy(this->data, &d, size);
        delay(10);
    }
}

Packet::~Packet(){
    if(strcmp(this->dataType, "string") == 0){
        free(this->data);
    }
    this->data = '\0';
}

void * Packet::getData(){
	return this->data;
}

int Packet::getSize(){
	return this->size;
}

char * Packet::getDataType(){
	return this->dataType;
}

void Packet::toJson(JsonObject& root){
    root["s"] = this->size;
    root["dt"] = this->dataType;
    if(strcmp(this->dataType, "int") == 0){    
        root["d"] = *(int*) this->data;
    }else if(strcmp(this->dataType, "string") == 0){
        root["d"] = (const char*) this->data;
    }else if(strcmp(this->dataType, "long") == 0){
        root["d"] = *(long*) this->data;
    }else if(strcmp(this->dataType, "double") == 0){
        root["d"] = *(double*) this->data;
    }else if(strcmp(this->dataType, "float") == 0){
        root["d"] = *(float*) this->data;
    }else if(strcmp(this->dataType, "char") == 0){
        root["d"] = *(char*) this->data;
    }
}

