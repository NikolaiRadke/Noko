#Setup a serial connection and write to EEPROMs (Linux)

1. Open the NOKO writer in the Arduino IDE and start it. 

2. Open a terminal  

3. To communicate through the connection, the user must be a member of the group **Dialout**. This can easily be done once-for-all with this command:  
``` sudo usermod -aG dialout $(whoami) ```  

4. Set parameter of serial communication with  
``` stty -F /dev/ttyUSB0 cs8 9600 ignbrk -brkint -imaxbel -opost -onlcr -isig -icanon -iexten -echo -echoe -echok -echoctl -echoke noflsh -ixon -crtscts ```  
if another USB port is used by NOKO, modify /dev/USB0 to /dev/USB1 or whatever the IDE tells you.

5. chance the directoy to NOKO/Write_EEPROM/

6. Write the text file to EEPROM with the command  
``` ./write_Disk0 > /dev/ttyUSB0 ```  
or  
``` ./write_Disk1 > /dev/ttyUSB0 ```  
The program sends the contend of the text file **Disk0** or **Disk1** via Arduino to the EEPROM. You can watch your Arduino's TX-LED blinking flurry. This may take a while, Disk1 needs about 10 Minutes. 

### NOTE 
*write_DiskX* was compiled with *x86_64*. For other platforms, see *write_DiskX.c* comments to compile it easy for yourself.
