#include "ConnectionManager.h"
#include <ctime>

int main(){
    //create a connection manager object so we can manage connections
	ConnectionManager * cm = new ConnectionManager();
    
    //connect to the i2c connection
	if(cm->connect(I2C)){
		std::cout << "Connected" << std::endl;
		std::string result = "Hello World!";
        
        //create packet object Packet(void* data, std::string dataType, int size)
		Packet *  p = new Packet((char*)result.c_str(), "string", result.size());
        
        //send the packet accros the current connection (I2C)
		if(cm->sendData(p)){
			std::cout << "Sent" << std::endl;
		}
		
        //read data from slave which returns a new packet
		Packet * p2 = cm->readData();
        
        //check for the data type so we can properly convert it
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
