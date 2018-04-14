# wireless-timer
 This project aims at maiking a wireless timing system that's primary use is for athletics. The project's main sport of focus to start is alpine ski racing. 
 
 # Sports
 
 ## Alpine Ski Racing
 In alpine ski racing a racer starts at the top of a run, and goes through a wand which starts thier time and the finish is triggered by "eye's" at the bottom. Finish eyes traditionally use some form of LASER or light break beam. During training sessions some teams will use wireless timing systems which are capabale of doing this task with no wired set up required. This is a great training tool as it can tell you who is going fastest and when people are improving, and most teams who have them are able to see more improvments. One issue with current systems is they are expensive, some of them are outdated and haven't been upgraded in decades, and many of them are not very user friendly. Thats why this project was born! The goal is to use Arduino's and Raspberry Pi's to to create a wireless timer which is cheaper, and aims at usability.
  
# Hardware

## Diagrams
If you look in the diagarams folder you will find fritzing diagrams of how the different pieceies need to be set up. The Raspberry Pi (RPi) and Arduino are connected through two different connections, the serial pins (UART) and I2C.

# Software
## Repositories
 ### Raspberry Pi
 * WirelessTimer: https://github.com/ianmott77/wireless-timer-rpi
 * pi_power (modified): https://github.com/craic/pi_power (Big thanks to @craic!!)
 
 ### Arduino
 * WierlessTimer: https://github.com/ianmott77/wireless-timer-arduino
 * RadioHead: http://www.airspayce.com/mikem/arduino/RadioHead/
 * LIDARLite V3: https://github.com/garmin/LIDARLite_v3_Arduino_Library
