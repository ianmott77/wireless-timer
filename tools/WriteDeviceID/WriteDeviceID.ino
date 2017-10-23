#include <EEPROM.h>
#define DEVICE_ID 2

void setup(){
    EEPROM.write(0, DEVICE_ID);
}
void loop(){}