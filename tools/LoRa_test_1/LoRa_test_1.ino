#include <MemoryFree.h>

#include <Connection.h>

#define DELAY_ARR 10

uint8_t t_add = 1;
uint8_t n_add = 2;

static unsigned long startMils = 0;
static unsigned long endMils = 0;
unsigned long first = 0;
unsigned long last = 0;
unsigned long difs[DELAY_ARR];
unsigned long del = 0;
unsigned long dif = 0;
unsigned long now = 0;
unsigned long them;
unsigned long adjusted = 0;

int drift = 0;
float factor = 0;
int c = 0 ;
int i = 0;
int o = 1;

unsigned long getTimez(){
    if(factor < 0.00 || factor > 0.00){
        /*
        adf = adjusted drift factor
        s = seconds
        now = adjusted
        ftm = first
        f = factor
        s = (now - ftm)/1000.00 (this gives how man seconds have passed since we first synced)
        adf = s*f (here we multiply the ms/s drift by the number of seconds that have gone by to get the number of milliseconds to add to the time)
        */
        //adjust the time for the diffrence between the clocks
        Serial.print("first: ");
        Serial.println(first);
        Serial.print("n: ");
        Serial.println(now);
        Serial.print("dif: ");
        Serial.println(dif);
        Serial.print("factor: ");
        Serial.println(factor);
        unsigned long a = now - dif;
        Serial.print("a: ");
        Serial.println(a);
        
        //calculate the elapsed seconds between when we first synced and now
        float s = (a - first)/1000.00;
        Serial.print("s: ");
        Serial.println(s);
        //multiply the seconds by the ms/s that the clocks drift apart by to obtain the adujsted drift factor
        
        float adf = (factor > 0) ? s*factor : s*(factor*-1);
        Serial.print("adf: ");
        Serial.println(adf);
        a += adf;
        Serial.print("final: ");
        Serial.println(a);
        Serial.flush();
        return a;
    }else{
        return now;
    }
}

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
    endMils = millis();
    adjusted = endMils - startMils;
    int d = 0;
    
    //add delay in order
    for(int k = i; k >= 0 && d == 0; k--){
      if(k > 0) difs[k] = difs[k-1];
      if(difs[k] <= adjusted){
        difs[k] = adjusted;
        d = k;
      }
    }
      
    Serial.print(c);
    Serial.print(") Delay: ");
    Serial.println(difs[d]);
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

    them -= del;
    dif = now - them;
    first = now - dif;
    /*********************/
    Serial.print("Them:");
    Serial.println(them);
    Serial.print("Me:  ");
    Serial.println(now);
    Serial.print("Adjusted: ");
    Serial.println(first);
    Serial.print("Difference: ");
    Serial.println(dif);
    
    Serial.println();
    Serial.flush();
    i++;
    now = millis();
  }else{
    them -= del;
    adjusted = (factor == 0) ? now - dif : getTimez();
    drift = adjusted - them;
    if(i == DELAY_ARR + 2){
      Serial.print("Them ");
      Serial.print("         ");
      Serial.print("Me   ");
      Serial.print("         ");
      Serial.print("Adjusted  ");
      Serial.print("    ");
      Serial.println("Drift");      
    }else if(i == DELAY_ARR +32){
        last = adjusted;
        factor = drift / ((last - first)/1000.0000);
        Serial.print("Drift ms/s = ");
        Serial.println(factor);
    }
    Serial.print(them);
    Serial.print("          ");
    Serial.print(now);
    Serial.print("          ");
    Serial.print(adjusted);
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
  return new Packet(&o, INT, sizeof(o),0);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial) ;
  Serial.println("Ready!");
  setBlocking(false);
  switchTo(I2);
  setThisAddress(t_add);
  if(!switchTo(LORA))
    Serial.println("init failed!");
  setNextAddress(n_add);
  setReceiveCallback(rec_c);
  setSendCallback(sen_c);
  setHandlers(rec, sen);
  startMils = millis();
  send();
}

void loop() {}
