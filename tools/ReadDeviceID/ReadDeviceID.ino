#include <EEPROM.h>

void setup(){
    Serial.begin(9600);
    while (!Serial) ;
    int devID = EEPROM.read(0);
    Serial.print("Device ID: ");
    Serial.println(devID);
}
void loop(){}