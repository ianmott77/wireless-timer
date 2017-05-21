#ifndef CONNECTION_MANAGER_H
#define CONNECTION_MANAGER_H

#include <typeinfo>

#include "ConnectionFactory.h"
#include "Packet.h"
#include "json/json.hpp"

using json = nlohmann::json;

class ConnectionManager{
	public:
		ConnectionManager();
		~ConnectionManager();
		bool connect(ConnectionType type);
		Connection * getCurrentConnection();
		void setCurrentConnection(Connection *);
		bool sendData(Packet *);
		Packet * readData();
	private:
		Connection * connections[3];
		ConnectionType currentConnection;
		ConnectionFactory * factory;
};

#endif
