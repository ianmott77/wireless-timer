#include "SerialMonitor.h"

SerialMonitor::SerialMonitor(ConnectionManager * manager){
	this->manager = manager;
	this->cont = true;
}

SerialMonitor::~SerialMonitor(){}

bool SerialMonitor::sendAck(){
	if(this->manager->send(1)){
		return true;
	}
	return false;
}

void SerialMonitor::cancel(){
	this->cont = false;
}

bool SerialMonitor::wasCancelled(){
	if(cont) 
	 return false;
 	else
	  return true;
}

void SerialMonitor::monitor(){
	this->cont = true;
	if(this->manager->connect(SERIAL)){
		while(this->manager->getCurrentConnection()->available() <= 0 && this->cont); //wait for input
		if(this->cont){
			this->pac = this->manager->read();
			if(this->pac != '\0'){
				delete this->pac;
				if(this->sendAck()){
				}
			}
		}
	}
}
