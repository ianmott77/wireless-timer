#include "ConnectionManager.h"

ConnectionManager::ConnectionManager(){
	this->factory = new ConnectionFactory();
}

ConnectionManager::~ConnectionManager(){

}

void ConnectionManager::setCurrentConnection(Connection * c){
	this->connections[this->currentConnection] = c;
}

Connection * ConnectionManager::getCurrentConnection(){
	return this->connections[this->currentConnection];
}

bool ConnectionManager::connect(ConnectionType type){
	this->currentConnection = type;
	this->setCurrentConnection(this->factory->newConnection(type));
	if(!this->getCurrentConnection()->setup()){
		return false;
	}
	return true;
}

bool ConnectionManager::sendData(Packet * p){
	json * j = p->toJson();
	std::string message = (*j).dump();
	if(this->getCurrentConnection()->send(message))
		return true;
	return false;
}

Packet * ConnectionManager::readData(){
	return new Packet(json::parse(this->getCurrentConnection()->recieve().c_str()));
}


