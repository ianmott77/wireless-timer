#include <Connection.h>
#include <Packet.h>

int in = 0;

//send callback
void sen_c(){
  while(!available());
  receive();
}

//receive callback
void rec_c(){
  in++;
  send();
}

//receiver
void rec(Packet * p){
  in = *(int*) p->data;
}

//sender
Packet * sen(){
 return new Packet(&in, INT, sizeof(in));
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while(!Serial); //waiting for ready
  if(switchTo(SER)){
    setHandlers(rec, sen);
    setSendCallback(sen_c);
    setReceiveCallback(rec_c);
    while(!available());
    receive();
  }
}
void loop() {}
