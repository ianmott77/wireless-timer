#include "Serial.h"

Serial::Serial() : Connection(){

}

Serial::~Serial(){

}

bool Serial::setup(){
	struct termios tty;
	struct termios tty_old;
	
	bool connected = false;
	//https://stackoverflow.com/questions/18108932/linux-c-serial-port-reading-writing
	for(int i = 0; i < 5; i++){
		this->filename = "/dev/ttyAMA0";
		
		//this->filename.append(std::to_string(i));
		this->file = open(this->filename.c_str(), O_RDWR |O_NOCTTY);
		if(this->file >= 0) {
			connected = true;
			break;
		}
	}	
	
	if(!connected){
		std::cout << "Open device error:" << strerror(errno) << std::endl;
		return false;
	}
	
	memset(&tty, 0, sizeof tty);
	
	if(tcgetattr(this->file, &tty) != 0){
		printf("tcgetattr error: %s\n", strerror(errno));
		return false;
	}
	
	tty_old = tty;
	
	cfsetospeed(&tty, (speed_t)B9600);
	cfsetispeed(&tty, (speed_t)B9600);
	
	
	/*
	tty.c_cflag &= ~PARENB;
	tty.c_cflag &= ~CSTOPB;
	tty.c_cflag &= ~CSIZE;
	tty.c_cflag &= ~HUPCL;
	tty.c_cflag |= CS8;
	tty.c_cflag &= ~CRTSCTS;
	tty.c_cflag |= (CREAD | CLOCAL);
	
	/*
	 * non-canonical mode
	tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | IGNCR | INLCR| ICRNL | IXON);
	tty.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
	tty.c_oflag &= ~OPOST;
	*/
	
	tty.c_cc[VMIN] = 0;
	tty.c_cc[VTIME] = 0;
	
	cfmakeraw(&tty);
	
	tcflush(this->file, TCIFLUSH);
	
	
	if(tcsetattr(this->file, TCSANOW, &tty) != 0){
		printf("tcsetattr error: %s\n", strerror(errno));
		return false;
	}
	
	return true;
}

ConnectionType Serial::getType(){
	return SERIAL;
}

int Serial::readInt(){
	uint8_t buf[2];
	for(int i = 0 ; i < 2; i++){
		if(read(this->file, &buf[i], 1) < 0){
			return -1;
		}
	}
	return buf[0] << 8 | buf[1] & 0xFF;
}

long Serial::readLong(){
	union{
		uint8_t buf[4];
		long val;
	}long_union;
	
	for(int i = 0; i < 4; i++){
		if(read(this->file, &long_union.buf[i], 1) <= 0){
			std::cout << "Read error #" << errno << " : " << strerror(errno) << std::endl;
			return -1;
		}
	}
	return long_union.val;
}

float Serial::readFloat(){
	union{
		uint8_t buf[4];
		float val;
	}float_union;
	
	for(int i = 0; i < 4; i++){
		if(read(this->file, &float_union.buf[i], 1) <= 0){
			std::cout << "Read error #" << errno << " : " << strerror(errno) << std::endl;
			return -1;
		}
	}
	
	return float_union.val;
}
