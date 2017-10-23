#include <Connection.h>
#include <Packet.h>

uint8_t t_add = 2;
uint8_t n_add = 1;

int g = 0;
int q = 0;
unsigned long now = 0;

void mc(){
  receive();
}

void sen_c(){
  receive();
}

void rec_c(){
  if(q == 2){
    setSendHandler(sendMils);
    setSendCallback(mc);
  }
  send();
}

//receiver
void rec(Packet * pack) {
 q  = *(int*) pack->data;
}

//sender
Packet * sendMils() {
  now = millis();
  return new Packet(&now, ULONG, sizeof(now), 0);
}

//sender
Packet * sen() {
  g++;
  return new Packet(&g, INT, sizeof(g), 0);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial) ;
  Serial.println("ready!");
  setBlocking(false);
  switchTo(I2);
  setThisAddress(t_add);
  if(!switchTo(LORA))
    Serial.println("init failed!");
  Serial.flush();
  setNextAddress(n_add);
  setReceiveCallback(rec_c);
  setSendCallback(sen_c);
  setHandlers(rec, sen);
  receive();
}

void loop() {}
