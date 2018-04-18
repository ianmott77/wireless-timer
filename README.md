# wireless-timer
 This project aims at maiking a wireless timing system that's primary use is for athletics. The project's main sport of focus to start is alpine ski racing.
 # Sports
 ## Alpine Ski Racing
 In alpine ski racing a racer starts at the top of a run, and goes through a wand which starts thier time and the finish is triggered by "eye's" at the bottom. Finish eyes traditionally use some form of LASER or light break beam. During training sessions some teams will use wireless timing systems which are capabale of doing this task with no wired set up required. This is a great training tool as it can tell you who is going fastest and when people are improving, and most teams who have them are able to see more improvments. One issue with current systems is they are expensive, some of them are outdated and haven't been upgraded in decades, and many of them are not very user friendly. Thats why this project was born! The goal is to use Arduino's and Raspberry Pi's to to create a wireless timer which is cheaper, and aims at usability.
 
# Things You Should Know Before Continuing
* How to solder
* Basic C/C++ programming knowledge would be an asset (e.g. loops, constants)
* Basic Knowledge of Linux systems (e.g. file system, terminal use)
* How to upload Arduino libraries
* How to import new Arduino libraries

# Hardware
The idea behind the harware set up is that the Arduino is used for sensor's for the outside world while the Raspberry Pi (RPi) is responsible for handling the display as well as doing the information processing and storing information. 
## Set Up
### Hardware Needed
#### For All Pieces
* Raspberry Pi Zero W (https://www.raspberrypi.org/products/raspberry-pi-zero-w/)
* Arduino UNO (https://store.arduino.cc/usa/arduino-uno-rev3)
* Waveshare 4inch HDMI LCD, 800×480, IPS (https://www.waveshare.com/4inch-HDMI-LCD.htm)
* Adafruit RFM95W LoRa Radio Transceiver Breakout - 868 or 915 MHz (https://www.adafruit.com/product/3072)
* 4-channel I2C-safe Bi-directional Logic Level Converter (https://www.adafruit.com/product/757)
* MCP3008 - 8-Channel 10-Bit ADC With SPI Interface (https://www.adafruit.com/product/856)
* PowerBoost 1000 Charger - Rechargeable 5V Lipo USB Boost @ 1A - 1000C (https://www.adafruit.com/product/2465)
* Lithium Ion Battery Pack - 3.7V 6600mAh (https://www.adafruit.com/product/353)
* 1 x MicroSD Card
* 4 x 10k resistor
* 3 x 4001N diode
* 3 x 330 Ohm resistor
* 2 x 6.8k resistor
* 1 x 100k resistor
* 1 x Momentary push button 
* 1 x Yellow LED
* 1 x Red LED
* 1 x Blue LED

#### Interval Only
* LIDAR-Lite v3 (https://www.sparkfun.com/products/14032)

#### Starter Only
* Magnetic contact switch (https://www.adafruit.com/product/375)

### Diagrams
If you look in the diagarams folder you will find fritzing diagrams of how the different pieceies need to be set up. The RPi and Arduino are connected through two different connections, the serial pins (UART) and I2C.

#### Starter Set Up
![alt text](https://raw.githubusercontent.com/ianmott77/wireless-timer/master/Diagrams/starter%20breadboard.png)

#### Finisher Set Up
![alt_text](https://raw.githubusercontent.com/ianmott77/wireless-timer/master/Diagrams/finisher%20breadboard.png)

##### Notes
Where the wires disapear in the diagram is where they connect to a Waveshare 4" resitive touchscreen LCD. Another thing to note is that in my set up I used a 6600 mAH battery.

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
## Arduino
To set up the the software for the Arduino start by downloading the WirelessTimer repo. The CLI folder contains the main program for the Arduino while the Conneciton folder contains the framework that it's built on. To install the program add the Connection library to the Arduino software (instruction here: https://www.arduino.cc/en/hacking/libraries).

#### Device ID
Inside the repo you will also find a folder named "tools" inside this folder you will find a script name "WriteDeviceID" in order to set up the Arduino we need to write a Device ID to it's EEPROM which will be saved permamnetly and should be unique for all the timers in your network. If you open up the script you will find a script like this
```
#include <EEPROM.h>
#define DEVICE_ID x

void setup(){
    EEPROM.write(0, DEVICE_ID);
}
void loop(){}
```
In order to write your desired unique device ID to the Arudino simply change where the "x" is to the unique device ID number you want and upload it to the Arduino. The device ID will be save permanently to the Arduino,after you run this script once you won't have to run it on an Arduino again unless you are changing it's device type.

 
#### Device Type
Inside the repo you will also find a folder named "tools" inside this folder you will find a script name "WriteDeviceType" in order to set up the Arduino we need to write a the device type to the Arduino. There are 3 different device types 

* 1 = Starter
* 2 = Interval (Finish Eyes)
* 3 = Monitor Node (not yet implemented)

```
#include <EEPROM.h>
#define DEVICE_TYPE x

void setup(){
    EEPROM.write(1, DEVICE_TYPE);
}
void loop(){}
```
In order to set the device type of the Arduino simply change the "x" in the script to the device type of you need and upload it to the Arduino. The device type will be save permanently to the Arduino, after you run this script once you won't have to run it on an Arduino again unless you are changing it's device ID.

#### CLI
The final set up step for an Arduino is to upload the CLI script. Once you have done that you are done!

## Raspberry Pi
### Operating System
This project uses Raspbian Stretch Lite (https://www.raspberrypi.org/downloads/raspbian/) as it's base but requires several steps to set up. In future this wil all be rolled in to a custom image but currently requires manual set up.
#### Configuring Raspbian
##### Downloading and seting up the MicroSD
In order to set up the Wireless timer software you first need to write the Raspbian Lite image (https://downloads.raspberrypi.org/raspbian_lite_latest) to a micro SD card which is compatible with Raspberry Pi Zero W's (most micro SD's should work). If you're not sure how to do this you can find detailed instructions here https://www.raspberrypi.org/documentation/installation/installing-images/
