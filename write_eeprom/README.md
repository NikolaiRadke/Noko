#Setting up a serial connection and writing to EEPROMs

Open the NOKO writer sketch **NOKO_EEPROM_Disk0.ino** or **NOKO_Disk1.ino** in the Arduino IDE and start it. 
Wait for *10 seconds*. When running *NOKO_Disk1.ino* you can go on at once.

##Linux

1. Open a terminal  

2. To communicate through the connection, the user must be a member of the group **Dialout**. This can easily be done once-for-all with this command:  
``` sudo usermod -aG dialout $(whoami) ```  

3. Set parameter of serial communication with  
``` stty -F /dev/ttyUSB0 cs8 9600 ignbrk -brkint -imaxbel -opost -onlcr -isig -icanon -iexten -echo -echoe -echok -echoctl -echoke noflsh -ixon -crtscts ```  
if another USB port is used by NOKO, modify /dev/USB0 to /dev/USB1 or whatever the IDE tells you.

4. Chance the directoy to ` NOKO/write_eeprom/ `

5. Write the text file to EEPROM with the command  
``` ./write_Disk0 > /dev/ttyUSB0 ```  
or  
``` ./write_Disk1 > /dev/ttyUSB0 ```  
The program sends the content of the text file **Disk0** or **Disk1** via Arduino to the EEPROM. You can watch your Arduino's RX and TX LEDs blinking flurry. This may take a while, Disk1 needs about 20 minutes... *yawn*

### NOTE 
*write_DiskX* was compiled with *x86_64*. For other platforms, see *write_DiskX.c* comments to compile it easy for yourself.

## Windows

1. Open a command line. Click on *Start* and type ``` cmd ```.  

2. Chance the directoy to ` NOKO/write_eeprom/  `  

3. Write the text file to EEPROM with the command  
``` write_Disk0 > COM1 ```  
or  
``` ./write_Disk1 > COM1 ```  
If another USB port is used by NOKO, modify COM1 to COM2 or whatever the IDE tells you. The program sends the content of the text file **Disk0** or **Disk1** via Arduino to the EEPROM. You can watch your Arduino's RX and TX LEDs blinking flurry. This may take a while, Disk1 needs about 20 minutes... *yawn*

### NOTE 
*write_DiskX.exe* was compiled with *x86_64*. For other platforms, see *write_DiskX.c* comments to compile it easy for yourself.

## The textfiles DISK0 and DISK1

### DISK0 format
The Disk0 file contains the author and title of the stories. Every item has exactly **2x20** characters, including unused spaces. The next item follows immediately. Do not use the return key, the file is like a single long string. It has to be plain text, don't use a word processor, just a simple text editor. **Note:** NOKO counts from zero, maybe your editor starts with 1 - change nothing. Just type.

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

* **4000-9999** are for the phrases:  
Every phrase starts with *Wusstest Du, dass...* in German and *Did you know that...* in English. So each item has **3x20** characters.

* **10000-17999** are for the quotations:    
Just **4x20** chars. NOKO pretends to be sophisticated.

* **18000-20079** are for the poems:   
Again **4x20**. Try to be lyrical.

if you want to write less items, just change the starting adresses in *NOKO.ino*. Right now, the swearword generator needs 4000 chars, so the phrases always start at 4000. Change the adresses of the quotations and poems in line **95 and 96**.
