#ifndef UTILS_H
#define UTILS_H

#include "Connection.h"

#define DELAY_ARR 10
#define START 10

#define MONITOR 5
#define TRIGGER 6

/******Race Mode functions********/

static volatile bool receivedStartParams = false;
static bool deviceTypeSent = false;
static int bib = -1;
static int startParamData = -1;
static int deviceType = getDeviceType();
static unsigned long start = 0;
static unsigned long interval = 0;
static unsigned long nextInterval = 0;
static int ms = 0;
static bool seqStarted = false;
static int seqStatus = -1;
static uint8_t prevAdd = getPrevAddress();
static uint8_t nextAdd = getNextAddress();

void timing_mode();
Packet * send_device_type();
void send_device_type_cb();
void rec_start_params(Packet*);
void rec_start_params_cb();
void timing_mode_init();
bool hasReceivedStartParams();
bool hasDeviceTypeSent();

//starter
void starter();
Packet * starter_init_timing_seq();
void starter_init_timing_seq_cb();
Packet * starter_send_bib();
void starter_send_bib_cb();
Packet * send_bib();
void send_bib_cb();
Packet * starter_send_start_millis();
void starter_send_start_millis_cb();
void rec_seq_started(Packet *);
void rec_seq_started_cb();
void rec_millis_int(Packet *);
void rec_millis_int_cb();
void setStartTime(unsigned long);
unsigned long catch_start();
unsigned long catch_interval();
unsigned long getStartMillis();
unsigned long getNextInterval();

//interval
void interval_timer();
void interval_seq_handle_rec(Packet*);
void interval_seq_handle_rec_cb();
Packet * interval_seq_handle_sen();
void interval_seq_handle_sen_cb();
void interval_rec_millis(Packet*);
void interval_rec_millis_cb();
void interval_rec_bib(Packet*);
void interval_rec_bib_cb();
Packet * send_interval();
void send_interval_cb();
Packet * send_time();
void send_time_cb();


/******Timing Eye********************/
static int finishDistance = 0;
static int lastDistance = 0;
static int confDistData = 0;
static int confDistCount = 0;
void lidar_init();
int distance();
void set_finish_distance(int);
Packet * send_distance();
void rec_confirm_distance(Packet*);
void rec_confirm_distance_cb();
void send_distance_cb();
void find_timing_distance();

/******Send Choice to RPi functions********/

static int choice = -1;
static bool csent = false;
void make_choice();
void make_choice_callback();
Packet * make_choice_init();
int getChoice();
bool wasChoiceSent();
void setChoice(int);
void setChoiceSent(bool);

/******Send Wake Up Signal to RPi functions********/

static int wake_up = 1;
static int ack = -1;
static bool piAwake = false;
void wake_up_pi();
void sen_wake_up_callback();
void rec_confirm_callback();
void rec_confirm(Packet *);
Packet * sen_wake_up();
bool isPiAwake();
void setPiAwake(bool);


/******Add New Device functions********/

static int addData = -1;
static int addDev = -1;
static int addPos = -1;
static int addCount = 0;
static int senCount = 0;
static bool recv = false;
void get_add_device_parameters();
void rec_device_parameters_cb();
void rec_device_parameters(Packet *);
bool hasRecv();
void setRecv(bool);
int getAddDevice();
int getAddPosition();
void sen_dev_params_cb();
void send_dev_params();
Packet * sen_dev_params_init();

/******Send Device Info functions********/

static int senDevInfoC = 0;
static bool devInfoSent = false;
void send_dev_info();
Packet * send_dev_info_init();
void send_dev_info_cb();
bool hasDevInfoSent();

/******Sychronization functions********/

static unsigned long startMils = 0;
static unsigned long endMils = 0;
static unsigned long first = 0;
static unsigned long last = 0;
static unsigned long difs[DELAY_ARR];
static long del = 0;
static long dif = 0;
static unsigned long now = 0;
static unsigned long them = 0;
static unsigned long adjusted = 0;
static float factor = 0.00;
static int drift = 0;
static int c = 0 ;
static int i = 0;
static int o = 1;
static int g = 0;
static int q = 0;
static int syncCount = 0;
static int syncFunction = 0;
void sync();
void sync_rec();
Packet * sync_init();
void sync_init_cb();
void sync_rec_init(Packet *);
void sync_rec_init_cb();
void sync_relay_rec(Packet*);
void sync_relay_rec_cb();
Packet * sync_relay_sen();
void sync_relay_sen_cb();
void sync_master_rec(Packet*);
void sync_master_rec_millis(Packet*);
void sync_master_rec_cb();
Packet * sync_master_sen();
Packet * sync_master_sen_millis();
void sync_master_sen_cb();
unsigned long getTime();


#endif;