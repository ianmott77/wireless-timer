#include "Connection.h"

char * input = '\0';
char *  output = '\0';
int marker = 0;
double blocks = 0;
int block = 0;
int inputSize = 0;
bool start = false;
bool done = false;
I2C * i2c = '\0';
Packet * message = '\0';
bool sizeSent = false;
char * part = '\0';
int size = 0;

I2C * newI2C(int address, receivePack r, sendMessage s){
    afterPackCreation = r;
    beforePackCreation = s;
    i2c = new I2C(address, send, receive); 
    return i2c;
}

void send(){
    if(!start){
        start = true;
        message = beforePackCreation();
        StaticJsonBuffer<200> buffer;
        JsonObject& root = buffer.createObject();
        message->toJson(root);
        inputSize = root.measureLength()+1;
        blocks = ceil(inputSize/31.00);
        if(inputSize > (blocks * 31)-5) blocks++;
        output = (char*) malloc(inputSize);
        root.printTo(output, inputSize);
        delete message;
    }
    
    if(!sizeSent){
    
        int markerInc = 32;
    
        if(inputSize < 32){
            if(inputSize < 26){
                markerInc = inputSize;
            }else{
                markerInc = 27;
            }
        }
    
        if(blocks > 1 && block != blocks-1){
            //not the last block
            part = substr(output, marker, markerInc);
            part[markerInc] = '\0';
        }else if(block == blocks-1){
            //last block
            part = substr(output, marker, (inputSize-marker)-1);
            char * done = str_append(part, "0x00\0");
        
            delete part;
            part = '\0';
        
            part = (char*) malloc(((inputSize-marker)-1)+5);
            strcpy(part, done);
        
            delete done;
            done = '\0';
        
            start = false;
        }
        size = strlen(part);
        uint8_t * buf = (uint8_t*) malloc(2);
        buf[0] = size >> 8;
        buf[1] = size & 0xFF;
        Wire.write(buf, 2);
        sizeSent = true;
        delete buf;
        buf = '\0';
        marker += markerInc;
        block++;
    }else{
        Wire.write(part);
        delete part;
        part = '\0';
        sizeSent = false;
    }
    if(!start){
        marker = 0;
        block = 0;
        delete output;
        output = '\0';
        size = 0;
        inputSize = 0;
    }
}

Packet * createPacket(char * input){
    StaticJsonBuffer<200> buffer;
    JsonObject& root = buffer.parseObject(input);
    if(root.success()){
        return new Packet(root);
    }
    return 0;
}

void receive(int bytes){
    if(bytes > 0){
        done = false;
        char * temp = (char*) malloc(marker);
        strcpy(temp, input);
        delete input;
        input = '\0';
        input = (char*) malloc(marker+bytes+1);
        strncpy(input, temp, marker);
        delete temp;
        temp = '\0';
        for(int i = 0; Wire.available(); i++){
            const char c = Wire.read();
            if(!done){
                input[marker] = c;
                marker++;
                input[marker] = '\0';
                if(str_ends(input, "0x00")){
                    done = true;
                    marker -= 4;
                    input[marker] = '\0';
                }
            }
        }
        if(!done)
            marker--;
        if(done){
            message = createPacket(input);
            afterPackCreation(message);
            delete message;
            delete input;
            input = '\0';
            marker = 0;
        }
    }
}