#include "ConnectionManager.h"
#include "SerialMonitor.h"
#include <thread>
#include <csignal>


ConnectionManager * cm;
SerialMonitor * mon; 
int choice = 0;
volatile bool userI = false;
volatile bool serialI = false;
int addDevice = -1;
int addPos = -1;
volatile bool monitor = false;
volatile bool running = true;
volatile unsigned long start = 0;
volatile bool ready = false;
int bib = -1;
bool intSignal = false;

int receiveChoice(){
	return *(int*) cm->read()->getData();
}

unsigned long receiveMillis(){
	return *(unsigned long*) cm->read()->getData();
}

void receiveInteravalSignal(SerialMonitor * mon){
	std::thread serMon(&SerialMonitor::monitor, mon);
	serMon.join();
	if(!mon->wasCancelled()){
		intSignal = true;
	}
}

void choiceMonitor(){
	std::thread serMon(&SerialMonitor::monitor, mon);
	serMon.join();
	if(!mon->wasCancelled()){
		choice  = receiveChoice();
		serialI = true;
	}
}
void startReadyMonitor(SerialMonitor * readyM){
	std::thread serMon(&SerialMonitor::monitor, readyM);
	serMon.join();
	if(!readyM->wasCancelled()){
		ready = true;
	}
}
void startMonitor(SerialMonitor * starter){
	std::thread serMon(&SerialMonitor::monitor, starter);
	serMon.join();
	if(!starter->wasCancelled()){
		start = receiveMillis();
	}
}

void getNewDeviceParameters(){
	std::cout << "What is the device ID of the device you wish to add?" << std::endl;
	std::cin >> addDevice;
	std::cout << "what is the position of the device you are adding?" << std::endl;
	std::cin >> addPos;
}

void getStartParams(){
	std::cout << "Enter Bib #:" <<std::endl;
	std::cin >> bib;
}

void inputMonitor(){
	fd_set r_set, w_set, e_set;
	struct timeval tv;
	int std = fileno(stdin);

	tv.tv_usec = 0;
	tv.tv_sec = 0;
	
	bool in = false;
	
	//checking for input or if the serial monitor got input
	while(!in && !monitor){
		FD_ZERO(&r_set);
		FD_SET(std, &r_set);
		int ret = select(std+1, &r_set, NULL, NULL, &tv);
		
		//if there is input in stdin
		if(FD_ISSET(std, &r_set)){
			std::cin >> choice;
			in = true;
		}
	}
	
	if(in){
		if(choice == 1){
			getNewDeviceParameters();
		}else if(choice == 4){
			
		}
		userI = true;
	}
}

void syncBlock(){
	if(cm->connect(SERIAL)){
		std::cout << "Synchronizing" << std::flush;
		for(int i = 0; cm->getCurrentConnection()->available() <= 0; i++){
			if(i%100000 == 0)
				std::cout << "." << std::flush;
		}
		int done = *(int*)cm->read()->getData();
		std::cout << std::endl << "Sync Complete!" << std::endl;
	}
}

void mainTh(){
	std::thread sMonitor(choiceMonitor);
	std::thread iMonitor(inputMonitor);

	while(!userI && !serialI); //wait till there is some input
	if(userI){
		mon->cancel();
		iMonitor.join();
		sMonitor.join();
		if(choice == 1){
			if(cm->connect(I2C)){
				if(cm->send(choice, 0)){
					//send the parameters of the device to add
					if(cm->send(addDevice, 0)){
						if(cm->send(addPos, 0)){
							std::cout << "Parameters Sent" << std::endl;
						}
					}
				}
			}	
		}else if(choice == 2){
			if(cm->connect(I2C)){
				if(cm->send(choice, 0)){
					if(cm->connect(SERIAL)){
						syncBlock();
					}
				}
			}
		}else if(choice == 3){
			if(cm->connect(I2C)){
				if(cm->send(200, 0)){
					int deviceID = *(int*) cm->read()->getData();
					int position = *(int*) cm->read()->getData();
					int nextAddress = *(int*) cm->read()->getData();
					int prevAddress = *(int*) cm->read()->getData();
					if(cm->send(201, 0)){
						std::cout << "Device ID: " << deviceID << std::endl;
						std::cout << "Device Position: " << position << std::endl;
						std::cout << "Next Device Address: " << nextAddress << std::endl;
						std::cout << "Previous Device Address: " << prevAddress << std::endl;
					}
				}
			}
		}else if(choice == 4){
			//if we got user input to start then we have to be at a start node
			getStartParams();
			if(cm->connect(I2C)){
				if(cm->send(choice, 0)){
					if(cm->send(bib, 0)){
						if(cm->connect(SERIAL)){
							std::cout << "Ready!" << std::endl;
							for(int i = 0; cm->getCurrentConnection()->available() <= 0; i++){
								if(i%100000 == 0)
									std::cout << "." << std::flush;
							}
							std::cout << std::endl;
							start = *(unsigned long*) cm->read()->getData();
							std::cout << "Start ms:" << start << std::endl;
						}
					}
				}
			}
		}else if(choice == 5){
			if(cm->connect(I2C)){
				if(cm->send(204, 0)){
					userI = false;
					int initial = 0;
					int distance = 0;
					int set = 0;
					if(cm->connect(SERIAL)){
						bool okay = false;
						for(int i = 0; !okay; i++){
							distance = *(int*) cm->read()->getData();
							if(i == 0){
								initial = distance;
								set = distance;
								std::cout << "Distance: " << distance << std::endl << std::flush;
							}
							if(distance > initial + 5 || distance < initial- 5){
								initial = distance;
								std::cout << "Distance: " << distance/100.00 << "m" << std::endl << std::flush;
							}
							if(cm->send(0, 0)){
								
							}							
						}
					}	
				}
			}
		}else if(choice == 0){
			running = false;
		}
	}else if(serialI){
		monitor = true;
		sMonitor.join();
		iMonitor.join();
		std::cout <<"Choice: " << choice << std::endl;
		if(choice == 1){
			addDevice = *(int*) cm->read()->getData();
			addPos = *(int*) cm->read()->getData();
			std::cout << "Add Device: " << addDevice << std::endl;
			std::cout << "Add Position: " << addPos << std::endl;
		}else if(choice == 2){
			syncBlock();
		}else if(choice == 4){
			std::cout << "Run Started" << std::endl <<std::flush;
			int bib = *(int*) cm->read()->getData();
			std::cout << "Bib #: " << bib << std::endl; 
			SerialMonitor * mon = new SerialMonitor(cm);
			std::thread thr(receiveInteravalSignal, mon);
			for(int i = 0; !intSignal; i++){
				if(i % 1000000 == 0)
					std::cout << "." << std::flush;
			}
			std::cout << std::endl << std::flush;
			thr.join();
			int time = *(int*) cm->read()->getData();
			double t = time/1000.0000;
			std::cout << "Time : " << t << std::endl; 			
		}
	}
	serialI = false;
	userI = false;
	monitor = false;
	ready = false;
	intSignal = false;
}

void options(){
	std::cout <<"Options" << std::endl;
	std::cout <<"1. Add Device" << std::endl;
	std::cout <<"2. Sync" << std::endl;
	std::cout <<"3. Show Device Info" << std::endl;
	std::cout <<"4. Timing Mode" <<std::endl;
	std::cout <<"5. Set Timing Eye Distance" << std::endl;
	std::cout <<"0. Exit" << std::endl;
}

int main(){
	cm = new ConnectionManager();
	while(running){
		mon = new SerialMonitor(cm);
		std::thread mainThread(mainTh);
		options();
		mainThread.join();
		delete mon;
	}	
	delete cm;
	return 1;
}	
