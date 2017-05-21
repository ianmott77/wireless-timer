#include "I2C.h"

i2c::i2c() : Connection(){
}

i2c::~i2c(){
	
}

bool i2c::setup(){
	this->filename = "/dev/i2c-1";
	this->address = 0x40;
	this->file = open(this->filename.c_str(), O_RDWR);
	if(ioctl(this->file, I2C_SLAVE, this->address) > -1)
		return true;
	return false;
}

bool i2c::send(std::string buffer){
	//we use  the chunk size we want -1 to allow for a null char byte
	double chunk = 31.00;
	
	//get the number of chunks well need
	double num = ceil(buffer.size()/chunk);
	
	//we need to allocate an extra chunk because we need 4 bytes for our EOF
	if((chunk * num) < buffer.size() + 4)	num += 1;

	//loop through however many section the buffer was split into
	for(int i = 0; i < num; i++){
		//our string 
		std::string part;
		
		//check to see if the there is another element in the array
		if(i + 1 < num){
			//not last element in the array
			part = buffer.substr(i*chunk, chunk);
			 
		}else{
			//last element in the array
			
			//check if there is anything left in the string to make part otherwise make it an empty string
			part = (i*chunk < buffer.size()) ? buffer.substr(i*chunk) : "";
				
			//add padding from the buffer to the end of the 32 bytes
			for(int j = buffer.size(); j < (i+1)*chunk; j += 4){
				part.append("0x00");
			}
		}
		
		//add null character to end of string
		part.push_back('\0');
		
		//send the part that was just made to the i2c device
		if(write(this->file, part.c_str() , chunk+1) != chunk+1){
			//if there was an error return false and print error
			std::cout << strerror(errno) << std::endl;
			return false;
		}
	}
	return true;		
}

std::string i2c::recieve(){
	int size = 32;
	std::string input = "";
	while(true){
		char * buf = new char[2];
		if(read(this->file, buf, 2) == 2){
			size = buf[0] << 8 | buf[1] & 0xFF;
			char * buffer = new char[size];
			if(read(this->file, buffer, size) != size){
				std::cout << strerror(errno) << std::endl;
				return 0;
			}
			input.append(buffer);
			if(input.rfind("0x00") != -1){
				int pos = input.find("0x00");
				return input.substr(0, pos);
			}
		}
	}
}
