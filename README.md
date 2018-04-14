# wireless-timer
 This project aims at maiking a wireless timing system that's primary use is for athletics. The project's main sport of focus to start is alpine ski racing.
 # Sports
 ## Alpine Ski Racing
 In alpine ski racing a racer starts at the top of a run, and goes through a wand which starts thier time and the finish is triggered by "eye's" at the bottom. Finish eyes traditionally use some form of LASER or light break beam. During training sessions some teams will use wireless timing systems which are capabale of doing this task with no wired set up required. This is a great training tool as it can tell you who is going fastest and when people are improving, and most teams who have them are able to see more improvments. One issue with current systems is they are expensive, some of them are outdated and haven't been upgraded in decades, and many of them are not very user friendly. Thats why this project was born! The goal is to use Arduino's and Raspberry Pi's to to create a wireless timer which is cheaper, and aims at usability.
# Hardware
The idea behind the harware set up is that the Arduino is used for sensor's for the outside world while the Raspberry Pi (RPi) is responsible for handling the display as well as doing the information processing and storing information. 
## Set Up
### Diagrams
If you look in the diagarams folder you will find fritzing diagrams of how the different pieceies need to be set up. The RPi and Arduino are connected through two different connections, the serial pins (UART) and I2C.
# Software
## Repositories
### Raspberry Pi
* WirelessTimer: https://github.com/ianmott77/wireless-timer-rpi
* pi_power (modified): https://github.com/craic/pi_power (Big thanks to @craic!!)
### Arduino
* WierlessTimer: https://github.com/ianmott77/wireless-timer-arduino
* RadioHead: http://www.airspayce.com/mikem/arduino/RadioHead/
* LIDARLite V3: https://github.com/garmin/LIDARLite_v3_Arduino_Library
## Set Up
### Arduino
To set up the the software for the Arduino start by downloading the WirelessTimer repo. The CLI folder contains the main program for the Arduino while the Conneciton folder contains the framework that it's built on. To install the program add the Connection library to the Arduino software (instruction here: https://www.arduino.cc/en/hacking/libraries).
#### Device ID
Inside the repo you will also find a folder named "tools" inside this folder you will find a script name "WriteDeviceID" in order to set up the Arduino we need to write a Device ID to it's EEPROM which will be saved permamnetly and should be unique for all the timers in your network. If you open up the script you will find a script like this
```
#include <EEPROM.h>
#define DEVICE_ID <x>

void setup(){
    EEPROM.write(0, DEVICE_ID);
}
void loop(){}
```
In order to write your desired unique device ID to the Arudino simply change where the "<x>" is to the unique  device ID number you want and upload it to the Arduino
 
#### Device Type
and then upload the CLI script to your Arduino and you are done!

### Operating System
This project is based on using 
