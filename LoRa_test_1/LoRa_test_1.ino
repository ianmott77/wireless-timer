#include <MemoryFree.h>

#include <Connection.h>

#define DELAY_ARR 10

uint8_t t_add = 1;
uint8_t n_add = 2;

static unsigned long startMils = 0;
static unsigned long endMils = 0;
unsigned long difs[DELAY_ARR];
unsigned long del = 0;
unsigned long dif = 0;
unsigned long now = 0;
unsigned long them = 0;
unsigned long t = 0;
int drift = 0;
int c = 0 ;
int i = 0;
int o = 1;

void sen_c(){
  receive();
}

void rec_c(){
  if(i < DELAY_ARR){
    /*
     * Here we build an array with the round trip delay times.
     * We build a sorted array so that we can discard values that are
     * out highest and lowest.
     */
    m();
    endMils = millis();
    t = endMils - startMils;
    int d = 0;
    
    //add delay in order
    for(int k = i; k >= 0 && d == 0; k--){
      if(k > 0) difs[k] = difs[k-1];
      if(difs[k] <= t){
        difs[k] = t;
        d = k;
      }
    }
      
    Serial.print(c);
    Serial.print(") Delay: ");
    Serial.print(difs[d]);
    Serial.print("     ");
    m();
    i++;
    startMils = millis();
  }else if(i == DELAY_ARR){
    
    /*
     * Here we calculate the average round trip delay over 10 
     * trips in order to be able to synchronize the milliseconds
     * between the two arduinos 
     */
    Serial.println();

    long total = 0;
    for(int g = 1; g < DELAY_ARR - 1; g++){
      total += difs[g];
    }
    
    del = total/(DELAY_ARR - 2);
    Serial.print("Delay:");
    Serial.println(del);
    
    Serial.println();

    Serial.flush();
    
    setReceiveHandler(recMils);
    o = 2;
    i++;
    now = millis();
  }else if(i == DELAY_ARR + 1){    
    /*
     *Here the delay is subtracted from the millis recived from the 
     *other Arduino in order to simulate millis on the other arduino
     *being called at the same time as on this one, and then from there
     *the difference between the two can be calculated 
     */

    dif = (them - del) - now;
    t = (them - del) - dif ;
    
    /*********************/
    Serial.print("Them:");
    Serial.println(them);
    Serial.print("Me:  ");
    Serial.println(now);
    Serial.print("Adjusted: ");
    Serial.println(t);
    Serial.print("Difference: ");
    Serial.println(dif);
    Serial.print("Memory: ");
    Serial.println(freeMemory());
    
    Serial.println();
    Serial.flush();
    i++;
    now = millis();
  }else{
    t = (them - del) - dif;
    drift = t - now;
    if(i == DELAY_ARR + 2){
      Serial.print("Them ");
      Serial.print("         ");
      Serial.print("Me   ");
      Serial.print("         ");
      Serial.print("Adjusted  ");
      Serial.print("    ");
      Serial.print("Memory    ");
      Serial.print("    ");
      Serial.println("Drift");      
    }
    Serial.print(them);
    Serial.print("          ");
    Serial.print(now);
    Serial.print("          ");
    Serial.print(t);
    Serial.print("          ");
    Serial.print(freeMemory());
    Serial.print("           ");
    Serial.println(drift);
    Serial.flush();
    i++;
    now = millis();
  }
  send();
}

//receiver
void recMils(Packet * pack) {
  them  = *(unsigned long*) pack->data;
}

//receiver
void rec(Packet * pack) {
  c  = *(int*) pack->data;
}

//sender
Packet * sen() {
  m();
  return new Packet(&o, INT, sizeof(o));
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial) ;
  Serial.println("Ready!");
  m();
  setThisAddress(t_add);
  if(!switchTo(LORA))
    Serial.println("init failed!");
  m();
  setNextAddress(n_add);
  setReceiveCallback(rec_c);
  setSendCallback(sen_c);
  setHandlers(rec, sen);
  startMils = millis();
  m();
  send();
}

void loop() {}
