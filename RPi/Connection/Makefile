all: Connection.o Packet.o I2C.o Serial.o SPI.o ConnectionFactory.o ConnectionManager.o main.o
	g++ -std=c++11 -o connect Connection.o Packet.o I2C.o Serial.o SPI.o ConnectionFactory.o ConnectionManager.o main.o

main.o: main.cpp
	g++ -std=c++11 -c main.cpp

ConnectionFactory.o: ConnectionFactory.cpp
	g++ -std=c++11 -c ConnectionFactory.cpp
	
ConnectionManager.o: ConnectionManager.cpp
	g++ -std=c++11 -c ConnectionManager.cpp

Serial.o: Serial.cpp
	g++ -std=c++11 -c Serial.cpp

SPI.o: SPI.cpp
	g++ -std=c++11 -c SPI.cpp

I2C.o: I2C.cpp 
	g++ -std=c++11 -c I2C.cpp

Connection.o: Connection.cpp
	g++ -std=c++11 -c Connection.cpp 

Packet.o: Packet.cpp
	g++ -std=c++11 -c Packet.cpp

clean: 
	rm -f *.o main Packet I2C Serial SPI ConnectionManager ConnectionFactory Connection
