#include "ConnectionManager.h"
#include <ctime>

int main(){
	ConnectionManager * cm = new ConnectionManager();
	if(cm->connect(I2C)){
		std::cout << "Connected" << std::endl;
		
		std::string result = "Hello World!";
		Packet *  p = new Packet((char*)result.c_str(), "string", result.size());
		if(cm->sendData(p)){
			std::cout << "Sent" << std::endl;
		}
		
		//sleep(1);
		
		Packet * p2 = cm->readData();
		if(p2->getDataType() == "string"){
			std::cout << (char*) p2->getData() << std::endl;
		}else if(p2->getDataType() == "long"){
			long i = *((long*) p2->getData());
			std::cout << i << std::endl;
		}else if(p2->getDataType() == "int"){
			int i = *((int*) p2->getData());
			std::cout << i << std::endl;
		}else if(p2->getDataType() == "float" || p2->getDataType() == "double"){
			double i = *((double*) p2->getData());
			std::cout << i << std::endl;
		}else if(p2->getDataType() == "char"){
			char i = *((char*) p2->getData());
			std::cout << i << std::endl;
		}
	}else{
		std::cout << "Error" << std::endl;
	}
	exit(1);
}
