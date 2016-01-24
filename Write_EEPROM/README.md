#How to set a serial connection to NOKO (Linux)

1. Open the NOKO writer in the Arduino IDE and start it. 

2. Open a terminal  

3. To communicate through the connection, the user must be a member of the group **Dialout**. This can easily be done with this command:  
``` sudo usermod -aG dialout $(whoami) ```  

4. Set parameter of serial communication with  
``` stty -F /dev/ttyUSB0 cs8 9600 ignbrk -brkint -imaxbel -opost -onlcr -isig -icanon -iexten -echo -echoe -echok -echoctl -echoke noflsh -ixon -crtscts ```  
if another USB port is used by NOKO, modify /dev/USB0 to /dev/USB1 or whatever the IDE tells you.

5. chance the directoy to NOKO/Write_EEPROM/

6. Write the text file to EEPROM with the command  
``` ./write_Disk0 > /dev/ttyUSB0 ```  
or  
``` ./write_Disk1 > /dev/ttyUSB0 ```  


### NOTE 
*burn* was compiled with *x86_64*. For other platforms, see *burn.c* comments to compile it easy for yourself.
