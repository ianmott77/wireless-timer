#include "Connection.h"

Connection::~Connection(){}

void Connection::closeConnection(){
	close(this->file);
}

int Connection::available(){
	fd_set fd;
	FD_ZERO(&fd);
	FD_SET(this->file, &fd);
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 0;
	return select(this->file +1, &fd, NULL, NULL, &tv); 
}

bool Connection::send(Packet* pack){
	if(this->send(pack->getSize())){
		if(this->send(pack->getDataType())){
			if(this->send(pack->getPosition())){
				if(pack->getDataType() == INT){
					if(this->send(*(int*)pack->getData())){
						return true;
					}
				}else if(pack->getDataType() == LONG || pack->getDataType() == ULONG){
					if(this->send(*(long*)pack->getData())){
						return true;
					}
				}else if(pack->getDataType() == STRING){
					if(this->send((const char*) pack->getData())){
						return true;
					}
				}else{
					std::cout << "Invalid data type" << std::endl;
					return false;
				}
			}else{
				//failed to send dataType
				std::cout << "Failed to send position" << std::endl;
				std::cout << "Write Error # " << errno << ":"<< strerror(errno) << std::endl;
				return false;
			}
		}else{
			//failed to send dataType
			std::cout << "Failed to send data type" << std::endl;
			std::cout << "Write Error # " << errno << ":"<< strerror(errno) << std::endl;
			return false;
		}
	}else{
		//failed to send size
		std::cout << "Failed to send size" << std::endl;
		std::cout << "Write Error # " << errno << ":"<< strerror(errno) << std::endl;
		return false;
	}
}

bool Connection::send(int data){
	uint8_t * buffer = (uint8_t*) malloc(2);
	buffer[0] = data >> 8;
	buffer[1] = data & 0xFF;
	if(write(this->file, buffer, 2) != 2){
		return false;
	}
	return true;
}

bool Connection::send(const char * data){
	for(int i = 0; i < strlen(data); i++){
		if(write(this->file, &data[i], 1) != 1){
			return false;
		}
	}
}

bool Connection::send(long data){
	uint8_t * buffer = (uint8_t*) malloc(4);
	buffer[0] = data & 0xFF;
	for(int i = 1; i < 4; i++){
		buffer[i] = (data >> i * 8) & 0xFF;
	}
	if(write(this->file, buffer, 4) != 4){
		return false;
	}
	return true;
}

bool Connection::send(float data){
	
}

int Connection::readInt(){
	uint8_t buf[2];
	int status = read(this->file, buf, 2);
	if(status != 2){
		std::cout << "Read error(" << status << ") #" << errno << " : " << strerror(errno) << std::endl;
		return status;
	}
	return buf[0] << 8 | buf[1] & 0xFF;
}

long Connection::readLong(){
	union{
		uint8_t buf[4];
		long val;
	}long_union;
	
	if(read(this->file, long_union.buf, 4) != 4){
		std::cout << "Read error #" << errno << " : " << strerror(errno) << std::endl;
		return -1;
	}
	
	return long_union.val;
}

float Connection::readFloat(){
	union{
		uint8_t buf[4];
		float val;
	}float_union;
	
	if(read(this->file, float_union.buf, 4) != 4){
		std::cout << "Read error #" << errno << " : " << strerror(errno) << std::endl;
		return -1;
	}
	
	return float_union.val;
}

std::string Connection::readString(int size){
	std::string r;
	for(int i = 0; i < size; i++){
		char buf[1];
		if(read(this->file, buf, 1) < 0){
			std::cout << "Read error #" << errno << " : " << strerror(errno) << std::endl;
			return "\0";
		}
		if(isprint(buf[0]))
			r += buf[0];
	}	
	return r;
}

Packet * Connection::receive(){	
	std::string input = "";
	int size = this->readInt();
	if(size > 0){
		DataType dataType = (DataType) this->readInt();
		int position = this->readInt();
		if(dataType == 0){
			int data = this->readInt();
			return new Packet(&data, dataType, size, position);
		}else if(dataType == 1){
			float data = this->readLong();
			return new Packet(&data, dataType, size, position);
		}else if(dataType == 2){
			signed long data = this->readLong();
			return new Packet(&data, dataType, size, position);
		}else if(dataType == 3){
			unsigned long data = this->readLong();
			return new Packet(&data, dataType, size, position);
		}else if(dataType == 4){
			std::string data = this->readString(size);
			return new Packet((char*) data.c_str(), dataType, size, position);	
		}else{
			return 0;
		}
	}else{
		return 0;
	}
}
