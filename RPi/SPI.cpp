#include "SPI.h"

Spi::Spi() : Connection(){

}

Spi::~Spi(){

}

bool Spi::setup(){

}

ConnectionType Spi::getType(){
	return SPI;
}
