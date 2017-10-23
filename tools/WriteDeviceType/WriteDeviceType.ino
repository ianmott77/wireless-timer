#include <EEPROM.h>
#define DEVICE_TYPE 2

void setup(){
    EEPROM.write(2, DEVICE_TYPE);
}
void loop(){}