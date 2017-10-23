#ifndef SERIAL_MONITOR_H
#define SERIAL_MONITOR_H

#include "ConnectionManager.h"
#include <thread>

class SerialMonitor{
	public:
		SerialMonitor(ConnectionManager *);
		~SerialMonitor();
		void monitor();
		void cancel();
		bool wasCancelled();
	private:
		ConnectionManager * manager;
		Packet * pac;
		bool sendAck();
		bool  cont;
};

#endif
