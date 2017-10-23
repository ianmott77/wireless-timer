#include "Connection.h"
#include <MemoryFree.h>

#define OUT 7
#define IN 8

void m(){
    Serial.print("mem=");
    Serial.println(freeMemory());
    Serial.flush();
}

int getDeviceID(){
    return EEPROM.read(0);
}

int getDeviceType(){
    return EEPROM.read(2);
}

void wireReceiver(int bytes){
    i2cOriginating = true;
    if(!blocking){
        digitalWrite(IN, HIGH);
        accept();
    }else if(currentConnection == I2){
        digitalWrite(IN, HIGH);
        accept();
    } 
}

void sendWire(){
    i2cOriginating = true;
    if(!blocking){
        digitalWrite(OUT, HIGH);
        transmit();
    }else if(currentConnection == I2){
        digitalWrite(OUT, HIGH);
        transmit();
    }
}

void setNextAddress(uint8_t next){
    _next_address = next;
}

void setPrevAddress(uint8_t prev){
    _prev_address = prev;
}

void setThisAddress(uint8_t t_address){
    _this_address = t_address;
    if(manager != '\0')
        manager->setThisAddress(_this_address);
}

uint8_t getPrevAddress(){
    return _prev_address;
}

uint8_t getNextAddress(){
    return _next_address;
}

uint8_t getThisAddress(){
    return _this_address;
}

uint8_t getMyPosition(){
    return my_position;
}

void setMyPosition(uint8_t pos){
    my_position = pos;
}

void setI2COriginating(bool orig){
    i2cOriginating = orig;
}

RHReliableDatagram * getManager(){
    return manager;
}

bool switchTo(Connection c){
    if(c != SER && c != I2 && c != SPIC && c != LORA) 
        return false;
    currentConnection = c;
    i2cOriginating = false;
    pinMode(IN, OUTPUT);
    pinMode(OUT, OUTPUT);
    if(currentConnection == I2){
        Wire.begin(SLAVE_ADDRESS);
        Wire.setClock(400000);
        Wire.onReceive(wireReceiver);
        Wire.onRequest(sendWire); 
    }else if(currentConnection == LORA){
        if(driver == '\0'){
            driver = new RH_RF95(RFM95_CS, RFM95_INT);
        }
        
        if(manager == '\0' && driver != '\0'){
            manager = new RHReliableDatagram(*driver, _this_address);
            if (!manager->init()){
                return false;
            }
            
        }
        manager->setRetries(5);
        driver->setTxPower(23, false);
        driver->setFrequency(915.0);

    }
    return true;
}

Connection getCurrentConnection(){
    return currentConnection;
}

void setHandlers(receivePack r, sendMessage s){
    afterPackCreation = r;
    beforePackCreation = s;
}

void setReceiveHandler(receivePack r){
    afterPackCreation = r;
}

void setSendHandler(sendMessage s){
    beforePackCreation = s;
}

void setSendCallback(func f){
    sendCallback = f;
}

void setReceiveCallback(func f){
    receiveCallback = f;
}

void setBlocking(bool val){
    blocking = val;
}

char read(){
    if(currentConnection == I2){
        return Wire.read();
    }else if(currentConnection == SER){
        return Serial.read();
    }else if(currentConnection == SPIC){
    
    }
}

int write(char data){
    if(currentConnection == I2){
        return Wire.write(data);
    }else if(currentConnection == SER){
        int n = Serial.write(data);
        Serial.flush();
        return n;
    }else if(currentConnection == SPIC){
    }else if(currentConnection == LORA){
        uint8_t to = (_send_address == 255) ? _next_address : _send_address;
        int g = manager->sendtoWait(data, sizeof(data), to);
        _send_address = 255;
        return g;
    }
}

int write(uint8_t * buf, int size){
    if(currentConnection == I2){
        return Wire.write(buf, size);;
    }else if(currentConnection == SER){
        int n = Serial.write(buf, size);
        Serial.flush();
        return n;
    }else if(currentConnection == SPIC){
        
    }else if(currentConnection == LORA){
        uint8_t to;
        if(_send_address == 255) {
             to = _next_address; 
        }else{
             to = _send_address;
        }
        if(manager->sendtoWait(buf, size, to)){
            _send_address = 255;
            return size;
        }
        return 0;
    }
}

int available(){
    if(currentConnection == I2){
        return Wire.available();    
    }else if(currentConnection == SER){
        return Serial.available();
    }else if(currentConnection == SPIC){
        
    }else if(currentConnection == LORA){
        return (int) manager->available();
    }
}

bool send(int data){
    uint8_t * buf = (uint8_t*) malloc(2);
    buf[0] = data >> 8;
    buf[1] = data & 0xFF;
    if(write(buf, 2) == 2){
        delete buf;
        buf = '\0';
        return true;
    }
    delete buf;
    buf = '\0';
    return false;
}

bool send(long data){
    uint8_t * buf = (uint8_t*) malloc(4);
    buf[0] = data & 0xFF;
    for(int i = 1; i < 4; i++)
        buf[i] = (data >> (i*8)) & 0xFF;
    if(write(buf, 4) == 4){
        delete buf;
        buf = '\0';
        return true;
    }
    delete buf;
    buf = '\0';
    return false;
}

bool send(unsigned long data){
    uint8_t * buf = (uint8_t*) malloc(4);
    buf[0] = data & 0xFF;
    for(int i = 1; i < 4; i++)
        buf[i] = (data >> (i*8)) & 0xFF;
    if(write(buf, 4) == 4){
        delete buf;
        buf = '\0';
        return true;
    }
    delete buf;
    buf = '\0';
    return false;
}


bool send(char * data){
    if(currentConnection != LORA){
        for(int i = 0; i < strlen(data); i++){
            if(!write(data[i]))
                return false;
        }
    }else{
        if(!manager->sendtoWait(data, sizeof(data), _next_address)){
           return false; 
        }
    }
    return true;
}

int readInt(){
    int r = NULL;
    if(currentConnection != LORA){
        if(available() >= 2){
            r = read() << 8;
            r |= read() & 0xFF;
        }
    }else{
        uint8_t * buf = malloc(2);
        uint8_t * len = malloc(2);
        uint8_t l = 2;
        memcpy(len, &l, 2);
        uint8_t  * from = malloc(2);
        if (manager->recvfromAck(buf, len, from)){
            r = buf[0] << 8;
            r |= buf[1] & 0xFF;
        }else{
            Serial.println("receive Failed");
            Serial.flush();
        }
        _prev_address = *from;
       
        delete buf;
        delete len;
        delete from;
    }
    return  r;
}

long readLong(){
    if(currentConnection != LORA){
        union{
            uint8_t buf[4];
            long val;
        } long_union;
        
        for(int i = 0; i < 4; i++){
            long_union.buf[i] = read();
        }
        return long_union.val;
    }else{
        uint8_t * buf = (uint8_t*) malloc(4);
        uint8_t * len = malloc(2);
        uint8_t r = 4;
        memcpy(len, &r, sizeof(r));
        uint8_t * from = malloc(2);
        
        if (!manager->recvfromAck(buf, len, from)){
            delete buf;
            delete len;
            delete from;
            return NULL;
        }
        _prev_address = *from;
        long val = (buf[3] << 24) | (buf[2] << 16) | (buf[1] << 8) | buf[0];
        delete buf;
        delete len;
        delete from;
        
        return val;
    }
}

unsigned long readULong(){
    if(currentConnection != LORA){
        union{
            uint8_t buf[4];
            unsigned long val;
        } long_union;
        
        for(int i = 0; i < 4; i++){
            long_union.buf[i] = read();
        }
        return long_union.val;
    }else{
        uint8_t * buf = malloc(4);
        uint8_t * len = malloc(2);
        uint8_t r = 4;
        memcpy(len, &r, sizeof(r));
        uint8_t * from = malloc(2);
        
        if (!manager->recvfromAck(buf, len, from)){
            delete buf;
            delete len;
            delete from;
            return NULL;
        }
        _prev_address = *from;
        unsigned long val = (buf[3] << 24) | (buf[2] << 16) | (buf[1] << 8) | buf[0];
        delete buf;
        delete len;
        delete from;
        
        return val;
    }
}

char * readString(int size){
    char * str = (char *) malloc(size);
    if(currentConnection != LORA){
        for(int i = 0; i < size; i++){
            str[i] = read();
        }
    }else{
        uint8_t * len = malloc(2);
        uint8_t r = sizeof(str);
        memcpy(len, &r, sizeof(r));
        uint8_t * from = malloc(2);
        if (!manager->recvfromAck(str, len, from)){
            delete len;
            delete from;
            return '\0';
        }
        _prev_address = *from;
        delete len;
        delete from;
    }
    return str;   
}

bool isI2COriginating(){
    return i2cOriginating;
}

void receive(){
    //receive size
    digitalWrite(IN, HIGH);
    accept();
    //receive data type
    accept();
    //receive position
    accept();
    //receive data
    accept();
}

void send(){
    //transmit size
    digitalWrite(OUT, HIGH);
    transmit();
    //transmit data type
    transmit();
    //transmit position
    transmit();
    //transmit data
    transmit();
}

void transmit(){
    sDone = false;
    if(tCount == 0){
        if(i2cOriginating){
            temp = currentConnection;
            currentConnection = I2;
        }
        message = beforePackCreation();
        if(message->position < getMyPosition() && message->position != 0){
            _send_address = getPrevAddress();
        }
        if(message != 0){
            if(send(message->size)){
                tCount++;
            }
        }
    }else if(tCount == 1){
        if(send(message->dataType)){
            tCount++;
        }
    }else if(tCount == 2){
        if(send(message->position)){
            tCount++;
        }
    }else if(tCount >= 3){
        if(message->dataType == INT){
            if(send(*(int*) message->data))
                sDone = true;
        }else if(message->dataType == FLOAT){
            
        }else if(message->dataType == LONG){
            if(send(*(long*) message->data))
                sDone = true;
        }else if(message->dataType == ULONG){
            if(send(*(unsigned long*) message->data))
                sDone = true;
        }else if(message->dataType == STRING){
            if(currentConnection  != I2){
                if(send((char*) message->data))
                    sDone = true;
            }else{
                //not properly implemented yet
                if(tCount == 3)
                    output = (char*) message->data;
                if(write(output[tCount - 3]))
                    tCount++;
                if(tCount - 3 == size)
                    sDone = true;
            }
        }
        if(sDone){
            tCount = 0;
            delete output;
            delete message;
            position = 0;
            output = '\0';
            if(i2cOriginating)
                currentConnection = temp;
            digitalWrite(OUT, LOW);
            if(sendCallback != '\0')
                sendCallback();

        }
    }
}

void accept(){
    rDone = false;
    if(rCount == 0){
        if(i2cOriginating){
            temp = currentConnection;
            currentConnection = I2;
        }
        if(currentConnection == LORA)
           manager->waitAvailable();
        size = readInt();
        rCount++;
    }else if(rCount == 1){
        if(currentConnection == LORA)
            manager->waitAvailable();
        dataType = readInt();
        rCount++;
    }else if(rCount == 2){
        if(currentConnection == LORA)
            manager->waitAvailable();
        position = readInt();
        rCount++;
    }else if(rCount >= 3){
        Packet * p;
        if(currentConnection == LORA)
            manager->waitAvailable();
        if(dataType == 0){
            int data = readInt();
            p = new Packet(&data, INT, size, position);
            rDone = true;
        }else if(dataType == 1){
            float data = (float) readLong();
            p = new Packet(&data, FLOAT, size, position);
            rDone = true;
        }else if(dataType == 2){
            long data = readLong();
            p = new Packet(&data, LONG, size, position);
            rDone = true;
        }else if(dataType == 3){
            unsigned long data = readULong();
            p = new Packet(&data, ULONG, size, position);
            rDone = true;
        }else if(dataType == 4){
            if(currentConnection != I2){
                input = readString(size);
                rDone = true;
            }else{
                if(rCount == 3){
                    input = (char*) malloc(size);
                }
                input[rCount - 3] = read();
                rCount++;
                if(rCount - 3 == size){
                    rDone = true;
                }
            }
            if(rDone)
                p = new Packet(input, STRING, size, position);
        }
        if(rDone){
            afterPackCreation(p);
            rCount = 0;
            dataType = -1;
            size = 0;
            position = 0;
            delete p;
            delete input;
            input[0] = '\0';
            if(i2cOriginating)
                currentConnection = temp;
            digitalWrite(IN, LOW);
            if(receiveCallback != '\0')
                receiveCallback();

        }
    }
}