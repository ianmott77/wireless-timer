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
	if(this->currentConnection == type) 
		return true;
	this->currentConnection = type;
	this->setCurrentConnection(this->factory->newConnection(this->currentConnection));
	if(!this->getCurrentConnection()->setup()){
		return false;
	}
	return true;
}

void ConnectionManager::switchTo(ConnectionType type){
	this->currentConnection = type;
}

bool ConnectionManager::send(Packet * p){
	if(this->getCurrentConnection()->send(p))
		return true;
	return false;
}

bool ConnectionManager::send(int data, int position){
	return this->send(new Packet(&data, INT, sizeof(data), position));
}

bool ConnectionManager::send(long data, int position){
	return this->send(new Packet(&data, LONG, sizeof(data), position));
}

bool ConnectionManager::send(float data, int position){
	return this->send(new Packet(&data, FLOAT, sizeof(data), position));
}

bool ConnectionManager::send(std::string data, int position){
	return this->send(new Packet((void*)data.c_str(), STRING, data.length(), position));
}

Packet * ConnectionManager::read(){
	return this->getCurrentConnection()->receive();
}

void ConnectionManager::closeConnection(){
	this->getCurrentConnection()->closeConnection();
}

