# Setting up a serial connection and writing to EEPROMs

Open the NOKO writer sketch **NOKO_Disk1.ino** in the Arduino IDE and start it. 

## Linux

1. Open a terminal  

2. To communicate through the connection, the user must be a member of the group **Dialout**. This can easily be done once-for-all with this command:  
``` sudo usermod -aG dialout $(whoami) ```  

3. Set parameter of serial communication with  
``` stty -F /dev/ttyUSB0 cs8 9600 ignbrk -brkint -imaxbel -opost -onlcr -isig -icanon -iexten -echo -echoe -echok -echoctl -echoke noflsh -ixon -crtscts ```  
if another USB port is used by NOKO, modify /dev/USB0 to /dev/USB1 or whatever the IDE tells you.

4. Change the directoy to ` NOKO/write_eeprom/ `

5. Write the text file to EEPROM with the command  
``` ./write_eeprom > /dev/ttyUSB0 ```  
or, if you want to see the progress in your console,    
``` ./write_eeprom | tee /dev/ttyUSB0 ```  
The program sends the content of the text file **EEPROM** via Arduino to the EEPROM. You can watch your Arduino's RX and TX LEDs blinking furiously. This may take a while, Disk1 needs about 18 minutes... *yawn*

### NOTE 
*write_eeprom* was compiled with *x86_64*. For other platforms, see *write_eeprom.c* comments to compile it for yourself.

## Windows

1. Open a command line. Click on *Start* and type ``` cmd ```.  

2. Chance the directoy to ` NOKO/write_eeprom/  `  

3. Write the text file to EEPROM with the command  ``` write_eeprom > COM1 ```  
If another USB port is used by NOKO, modify COM1 to COM2 or whatever the IDE tells you. The program sends the content of the text file **EEPROM** via Arduino to the EEPROM. You can watch your Arduino's RX and TX LEDs blinking furiously. This may take a while, Disk1 needs about 18 minutes... *yawn*

## Windows 8  

Users report troubles writing an EEPROM with the provided tools. Please try another platform.  

## Windows 10

Like written above, but first Windows 10 needs another **baud rate**. You need admin rights.  
 
or **Line 17** in **NOKO_Disk1.ino**.  
  
2. *Right click Start* -> *Device Manager* -> *Ports* -> *USB Serial Port (COMX)* -> *Right click Port Settings*  

3. Set *Bits per second* to **115200**.

4. Take steps above.  

### NOTE 
*write_eeprom.exe* was compiled with *x86_64*. For other platforms, see *write_eeprom.c* comments to compile it for yourself.

## The textfiles DISK0 and DISK1

### EEPROM format
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

* **4000-9999** are for the phrases:  
Every phrase starts with *Wusstest Du, dass...* in German and *Did you know that...* in English. So each item has **3x20** characters.

* **10000-17999** are for the quotations:    
Just **4x20** chars. NOKO pretends to be sophisticated.

* **18000-20079** are for the poems:   
Again **4x20**. Try to be lyrical.

* **20080-XXX** 20 chars for author's name and 20 chars for the story title

if you want to write less items, just change the starting adresses in *NOKO.ino*.
