#include "Connection.h"

char * input = '\0';
char * output = '\0';
char * part = '\0';

bool sizeSent = false;
bool last = false;
bool allDone = true;
bool done = false;

int marker = 0;
double blocks = 0;
int block = 0;
int inputSize = 0;

I2C * i2c = '\0';
Packet * message = '\0';


I2C * newI2C(int address, receivePack r, sendMessage s){
    afterPackCreation = r;
    beforePackCreation = s;
    i2c = new I2C(address, send, receive); 
    return i2c;
}

void sendInt(int n){
    uint8_t * buf = (uint8_t*) malloc(2);
    buf[0] = n >> 8;
    buf[1] = n & 0xFF;
    Wire.write(buf, 2);
    delete buf;
    buf = '\0';
}

void resetOutput(){
    delete output;
    output = '\0';
    marker = 0;
    block = 0;
    blocks = 0;
    inputSize = 0;
    done = false;
    last = false;
    allDone = true;
}

void resetInput(){
    delete message;
    delete input;
    input = '\0';    
    marker = 0;
    done = false;   
}

void initOutput(){
    StaticJsonBuffer<200> buffer;
    JsonObject& root = buffer.createObject();
    message->toJson(root);
    inputSize = root.measureLength()+1;
    blocks = ceil(inputSize/31.00);
    if(inputSize > (blocks * 31)-5) 
        blocks++;
    output = (char*) malloc(inputSize);
    root.printTo(output, inputSize);
    allDone = false;
}

void send(){
    
    if(allDone){
        //initialize
        message = beforePackCreation();
        initOutput();
        delete message;
    }
    
    if(!sizeSent){

        //prepare part
        int markerInc = 32;
    
        if(inputSize < 32){
            if(inputSize < 26){
                markerInc = inputSize+1;
            }else{
                markerInc = 27;
            }
        }

        if(blocks > 1 && block != blocks-1){
            //not the last block
            part = substr(output, marker, markerInc-1);
            part[markerInc] = '\0';
            last = false;
        }else if(block == blocks-1){
            //last block
            part = substr(output, marker, (inputSize-marker)-1);
            char * donez = str_append(part, "0x00\0");
        
            delete part;
            part = '\0';
        
            part = (char*) malloc(((inputSize-marker)-1)+5);
            strcpy(part, donez);
        
            delete donez;
            donez = '\0';
        
            last = true;
        }
        
        //send size of part
        sendInt(strlen(part));
        
        //size was sent
        //keep block increment in this statemnet, otherwise doesn't work
        sizeSent = true;
        marker += markerInc;
        block++;

    }else{
        //send part
        Wire.write(part);
        delete part;
        part = '\0';
        //reset to send size of next packet
        if(last)
            done = true;
        sizeSent = false;
    }
    if(done){
        //reset
        resetOutput();
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
            resetInput();
        }
    }
}