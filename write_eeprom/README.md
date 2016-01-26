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

### DISK0 format
The Disk0 file contains the author and title of the stories. Every item has exactly **2x20** characters, including unused spaces. The next item follows immediately. Do not use the return key, the file is like a single long string.

### DISK1 format
Nearly the same as DISK0. No return key, just space bar. 
* **0000-3999** are for the swearword generator:  
*German version*  
**0000-0999** 10 chars for an adjective. If it's male, the funtions adds an "r" and a space.  
**1000-1999** 10 chars for the first part of the noun.  
**2000-2999** 10 chars for male noun.  
**3000-3999** 10 chars for female noun.  
*English version*  
**0000-0999** 10 chars for an adjective. The function adds a space.  
**1000-1999** 10 chars for the first part of the noun. The function adds a space.  
**2000-3999** 10 chars for noun.  

* **4000-9999** for the phrases:  
Every phrase starts with "Wusstest Du, dass..." in German an "Did you know that..." in English. So each item has **3x20** Characters.

* **10000-17999** for the quotations:    
Just **4x20** chars. NOKO pretends to be sophisticated.

**18000-20079** for the poems:   
Again **4x20**. Try to be lyrical.

Both files have to be plain text, don't use a Word processor, just a simple text editor.  
**Note:** NOKO counts from zero, maybe your editor starts with 1.  
if you want to write less items, just chance the starting adresses in *NOKO.ino*. Right now, the swearword generator needs 4000 chars, so the phrases always start at 4000. Chance the adresses of the quotations and poems in line **99 and 100**.
