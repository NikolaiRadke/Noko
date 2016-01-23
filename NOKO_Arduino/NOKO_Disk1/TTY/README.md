#How to set a serial connection to NOKO (Linux)

1. Open the NOKO writer in the Arduino IDE and start it. 

2. Open a terminal

3. Set parameter of serial communication with  
``` stty -F /dev/ttyUSB0 cs8 9600 ignbrk -brkint -imaxbel -opost -onlcr -isig -icanon -iexten -echo -echoe -echok -echoctl -echoke noflsh -ixon -crtscts ```  
if another USB port is used by NOKO, modify /dev/USB0 to /dev/USB1 or whatever the IDE tells you.

4. chance the directoy to NOKO/NOKO_Disk1/TTY or NOKO/NOKO_EEPROM_DISK0/TTY

5. Write the text file to EEPROM with the command  
``` ./burn > /dev/ttyUSB0 ```  

### NOTE 
*burn* was compiled with *x86_64*. For other platforms, see *burn.c* comments to compile it easy for yourself.
