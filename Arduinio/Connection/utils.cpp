#include "utils.h"

bool hasRecv(){
    return recv;
}

bool isPiAwake(){
    return piAwake;
}

bool wasChoiceSent(){
    return csent;
}

void setChoice(int cho){
    choice = cho;
}

int getChoice(){
    return choice;
}

void setRecv(bool recd){
    recv = recd;
}

void setChoiceSent(bool sent){
    csent = sent;
}

void setPiAwake(bool awake){
    piAwake = awake;
}

bool hasDevInfoSent(){
    return devInfoSent;
}

bool hasReceivedStartParams(){
    return receivedStartParams;
}

bool hasDeviceTypeSent(){
    return deviceTypeSent;
}

void setStartTime(unsigned long s){
    start = s;
}

int distance(){
    int pulseWidth = pulseIn(MONITOR, HIGH);
    if(pulseWidth != 0)
        return pulseWidth / 10; 
    return -1;
}

void lidar_init(){
    pinMode(TRIGGER, OUTPUT); // Set trigger pin
    digitalWrite(TRIGGER, LOW); // Set trigger LOW for continuous read
    pinMode(MONITOR, INPUT); 
}

void set_finish_distance(int distance){
    finishDistance = distance;
}

void find_timing_distance(){
    if(switchTo(SER)){
        setHandlers(rec_confirm_distance, send_distance);
        setSendCallback(send_distance_cb);
        setReceiveCallback(rec_confirm_distance_cb);
        send();
    }
}

void rec_confirm_distance(Packet * pack){
    confDistData = *(int*) pack->data;
}

void rec_confirm_distance_cb(){
    send();
}

Packet * send_distance(){
    lastDistance = distance();
    return new Packet(&lastDistance, INT, sizeof(lastDistance), 0);
}

void send_distance_cb(){
       send();
}

void timing_mode(){
    setReceiveHandler(rec_start_params);
    setReceiveCallback(rec_start_params_cb);
}

void starter(){
    setSendHandler(starter_send_bib);
    setSendCallback(starter_send_bib_cb);
    send();
}

void interval_timer(){
    setReceiveHandler(interval_rec_bib);
    setReceiveCallback(interval_rec_bib_cb);
}

void interval_rec_bib(Packet* pack){
    bib = *(int*) pack->data;
}

void interval_rec_bib_cb(){
    setReceiveHandler(interval_rec_millis);
    setReceiveCallback(interval_rec_millis_cb);
}

void interval_rec_millis(Packet* pack){
    start = *(long*) pack->data;
}

void interval_rec_millis_cb(){
    //start polling for data
    setPiAwake(false);
    setChoiceSent(false);
    wake_up_pi();
       
    //check if we woke up the Pi
    if(isPiAwake()){

        //send the choice of what we're doing
        make_choice();
          
        //check if the choice was sent
        if(wasChoiceSent()){
            setSendHandler(send_bib);
            setSendCallback(send_bib_cb);
            send();
        }
    }
    
    int state = 0;
    while(state == 0){
        state = digitalRead(START);
    }
    interval =  getTime();
    ms = (interval - start);
    
    setPiAwake(false);
    wake_up_pi();
    //check if we woke up the Pi
    if(isPiAwake()){        
        setSendHandler(send_time);
        setSendCallback(send_time_cb);
        send();
        
    }
}

Packet * send_time(){
    return new Packet(&ms, INT, sizeof(ms), 0);
}

void send_time_cb(){
    
}

Packet * send_interval(){
    return new Packet(&interval, ULONG, sizeof(interval), getMyPosition()-1);
}

void send_interval_cb(){
    
}

void rec_start_params(Packet* pack){
    startParamData = *(int*) pack->data;
}

void rec_start_params_cb(){
    if(startParamData != -1){
        bib = startParamData;
        int state = 0;
        setRecv(true);
    }
}

Packet * starter_send_bib(){
    return new Packet(&bib, INT, sizeof(bib), 99);
}

void starter_send_bib_cb(){
    setSendHandler(starter_send_start_millis);
    setSendCallback(starter_send_start_millis_cb);
    send();
}

Packet * send_bib(){
    return new Packet(&bib, INT, sizeof(bib), 0);
}

void send_bib_cb(){

}

Packet * starter_send_start_millis(){
    return new Packet(&start, ULONG, sizeof(start), getMyPosition() + 1);
}

void starter_send_start_millis_cb(){
     //the start was sent let it fall back to reset();
}

void rec_millis_int(Packet * pack){
    nextInterval = *(unsigned long*) pack->data;
}

void rec_millis_int_cb(){
    //don't know what to do with this yet
}

unsigned long catch_start(){
    
}

unsigned long catch_interval(){
    int state = 0;
    while(true){
        state = digitalRead(START);
        if(state == 1){
            return getTime();
        }
    }
}

unsigned long getStartMillis(){
    return start;
}

unsigned long getNextInterval(){
    return nextInterval;
}

void sync(){
    setSendHandler(sync_init);
    setSendCallback(sync_init_cb);
    if(getMyPosition() == 1){
        syncFunction = 1;
    }else{
        syncFunction = 2;
    }
    send();
}

void sync_rec(){
    setReceiveHandler(sync_rec_init);
    setReceiveCallback(sync_rec_init_cb);
}

void sync_rec_init(Packet * pack){
    syncFunction = *(int*) pack->data;
}

void sync_rec_init_cb(){
    if(syncFunction == 1){
        //this means that this device should initiate the sync with the previous device
        //set up the sender to be the sync initiator and difference calculator
        
        //send info to the RPi
        wake_up_pi();
       
        //check if we woke up the Pi
        if(isPiAwake()){

            //send the choice of what we're doing
            make_choice();
          
            //check if the choice was sent
            if(wasChoiceSent()){
                //send the paramaters of the device to add
                setChoiceSent(false);
            }
        }
        if(switchTo(LORA)){
            setReceiveCallback(sync_master_rec_cb);
            setSendCallback(sync_master_sen_cb);
            setHandlers(sync_master_rec, sync_master_sen);
            startMils = millis();
            send();
        }
    }else if(syncFunction == 2){
        if(getMyPosition() == 1){
            //this means that we should tell the next device to initiate a sync with us
        }else{
            //this means that we should send the choice and sync data to the previous device
        }
    }
}

Packet * sync_init(){
    int to;
    if(getMyPosition() == 1){
        to = getMyPosition() + 1;
    }else{
        to = getMyPosition() - 1;
    }
    return new Packet(&syncFunction, INT, sizeof(syncFunction), to);

}

void sync_init_cb(){
    if(getMyPosition() == 1){
        setReceiveCallback(sync_relay_rec_cb);
        setSendCallback(sync_relay_sen_cb);
        setHandlers(sync_relay_rec, sync_relay_sen);
        receive();
    }
}

Packet * sync_relay_sen_millis(){
  unsigned long time = getTime();
  return new Packet(&time, ULONG, sizeof(time), getMyPosition() + 1);
}

Packet * sync_relay_sen(){
  g++;
  return new Packet(&g, INT, sizeof(g), getMyPosition() + 1);
}

void sync_relay_sen_cb(){
    if(q < 4)
        receive();
}

void sync_relay_rec(Packet* pack){
    q  = *(int*) pack->data;
}

void sync_relay_rec_cb(){
    if(q == 2){
        setSendHandler(sync_relay_sen_millis);
        setSendCallback(sync_relay_sen_cb);
    }else if(q == 4){
        if(switchTo(SER)){
            q = 5;
            send();
        }
    }
    if(q < 4)
        send();
}

Packet * sync_master_sen(){
    return new Packet(&o, INT, sizeof(o), getMyPosition() - 1);
}

void sync_master_sen_cb(){
    if(o < 4){
        receive();
    }else if(o == 4){
        o = 5;
        if(switchTo(SER))
            send();
    }else{
        i = 0;
        o = 1;
    }
}

void sync_master_rec_millis(Packet* pack){
    them  = *(unsigned long*) pack->data;
}

void sync_master_rec(Packet* pack){
    c = *(int*) pack->data;
}

void sync_master_rec_cb(){
    if(i < DELAY_ARR){
    /*
     * Here we build an array with the round trip delay times.
     * We build a sorted array so that we can discard values that are
     * the highest and lowest.
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

    i++;
    startMils = millis();
  }else if(i == DELAY_ARR){
    
    /*
     * Here we calculate the average round trip delay over 10 
     * trips in order to be able to synchronize the milliseconds
     * between the two arduinos 
     */
    
    long total = 0;
    for(int u = 1; u < DELAY_ARR - 1; u++){
      total += difs[u];
    }
    
    del = total/(DELAY_ARR - 2);

    setReceiveHandler(sync_master_rec_millis);
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
    o = 3;
    i++;
    now = millis();
  }else{
    if(i < DELAY_ARR + 8){
        them -= del;
        adjusted = now - dif;  
        drift = adjusted - them;
        if(i == DELAY_ARR + 7){
            last = adjusted;
            factor = drift / ((last - first)/1000.00);
            o = 4;
        }
        now = millis();
    }
    i++;
  }
  if(i <= DELAY_ARR + 8)
    send();
}

unsigned long getTime(){
    unsigned long n = millis();
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
        adjusted = n - dif;
        
        //calculate the elapsed seconds between when we first synced and now
        float s = (adjusted - first)/1000.00;
        
        //multiply the seconds by the ms/s that the clocks drift apart by to obtain the adujsted drift factor
        float adf = s*(factor*-1);
        adjusted += adf;
        return adjusted;
    }else{
        return n;
    }
}

void make_choice(){
    setSendHandler(make_choice_init);
    setSendCallback(make_choice_callback);
    send();
}

//send the choice to RPi
Packet * make_choice_init(){
  return new Packet(&choice, INT, sizeof(choice), 0);
}

//callback from sending the chioce 
void make_choice_callback(){
    csent = true;
}

void send_dev_info(){
    devInfoSent = false;
    setSendHandler(send_dev_info_init);
    setSendCallback(send_dev_info_cb);
}

Packet * send_dev_info_init(){
    senDevInfoC++;
    if(senDevInfoC == 1){
        int devId = getDeviceID();
        return new Packet(&devId, INT, sizeof(devId), 0);
    }else if(senDevInfoC == 2){
        int pos = getMyPosition();
        return new Packet(&pos, INT, sizeof(pos), 0);
    }else if(senDevInfoC == 3){
        int nextAdd = getNextAddress();
        return new Packet(&nextAdd, INT, sizeof(nextAdd), 0);
    }else if(senDevInfoC == 4){
        int prevAdd = getPrevAddress();
        return new Packet(&prevAdd, INT, sizeof(prevAdd), 0);
    }
}

void send_dev_info_cb(){
    if(senDevInfoC >= 4){
        senDevInfoC = 0;
        devInfoSent = true;
    }
}

//send callback
void sen_wake_up_callback(){
  while(!available());
  receive();
}

//receive confirm callback
void rec_confirm_callback(){
  if(ack > -1){
      piAwake = true;
  }
}

//receive confirm from rpi
void rec_confirm(Packet * p){
  ack = *(int*) p->data;
}

//send wake up to rpi
Packet * sen_wake_up(){
 return new Packet(&wake_up, INT, sizeof(wake_up), 0);
}

//init waking up pi
void wake_up_pi(){
  if(switchTo(SER)){
    setHandlers(rec_confirm, sen_wake_up);
    setSendCallback(sen_wake_up_callback);
    setReceiveCallback(rec_confirm_callback);
    send();
  }
}

void rec_device_parameters_cb(){
  if(addCount == 0){
    addDev = addData;
    addCount++;
  }else{
    addPos = addData;
    //if the one we are adding is the position after us make it our next address
    if(isI2COriginating()){
        if(addPos == getMyPosition() + 1 || getNextAddress() == 255){
            setNextAddress(addDev);
        }
        setI2COriginating(false);
    }
    addCount = 0;
    recv = true;
  }
}

int getAddDevice(){
    return addDev;
}

int getAddPosition(){
    return addPos;
}

void rec_device_parameters(Packet * pack){
  addData = *(int*) pack->data;
}

void get_add_device_parameters(){
  setReceiveHandler(rec_device_parameters);
  setReceiveCallback(rec_device_parameters_cb);
}

void send_dev_params(){
    setSendHandler(sen_dev_params_init);
    setSendCallback(sen_dev_params_cb);
    send();
}

void sen_dev_params_cb(){
  if(senCount > 1){
    senCount = 0;
  }else{
    send();
  }
}

Packet * sen_dev_params_init(){
  senCount++;
  if(senCount == 1){
    return new Packet(&addDev, INT, sizeof(addDev), getMyPosition() + 1);
  }else{
    return new Packet(&addPos, INT, sizeof(addPos), getMyPosition() + 1);
  }
}