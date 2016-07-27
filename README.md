# ![NOKO](http://www.nikolairadke.de/NOKO/noko_klein.png) NOKO

The **NOKO Monster**, lovely and naughty, ready to build on your own. And everyone is invited to **contribute**. NOKO is for everyone.  

![NOKO Orange](http://www.nikolairadke.de/NOKO/noko_echt.png)  

NOKO is a multifunctional monster with an ATmega328  microcontroller on an **Arduino Nano**, display, clock, radio, mp3 player, four buttons as user interface, battery, voice set and  tons of gimmicks, packed in a cuddly monster suit - sturdy,
lovely and sometimes a bit mean and annoying. Just the way you want him to be. He, she or it - this is your decision     and only depends on the voice set. In this repository NOKO is a lad, a real bloke! A buddy for life and a companion by day and night. All he needs is a little power every now and then.  

**NOKO** stands for **NO**ra's and ni**KO**'s monster.

###Notes for non-German NOKO makers
There are a German and an English version. Right now, the building wiki, the voice set, the manual and the optional EEPROM text files are in German. I'll try to translate the wiki and the manual when the German wiki is completed. Until then, feel free to ask me for translation, if you need help.  **Contribute!** I need an English voice set and English text files. In addition, any other language is welcome, NOKO is to be cosmopolitan!

### Prerequisites
NOKO needs Arduino IDE 1.6.6 - [1.6.10](https://www.arduino.cc/en/Main/Software) Copy the folder `NOKO/src/[language]/NOKO/` into your sketch folder. At this moment, *NOKO.ino* won't compile, the sketch size is too large. 

**Option 1:** If you are using AVR Boards Core 1.6.10 or higher, you need to install [Optiboot](https://github.com/Optiboot/optiboot) onto your Nano.  
**Option 2:** If you are using Linux, install **AVR Core 1.6.9** and see [how to compile](https://github.com/NikolaiRadke/NOKO/tree/master/howto_compile) to solve this issue. Optiboot is strongly recommended.  

### Content

```
NOKO/
├── howto_compile/
|   Hints compile the sketches in your Arduino IDE.
|   ├── linux/
|   |   Modified platform.txt for Linux
|   └── windows/
|       Modified platform.txt for Windows
|
├── manual/
|   NOKO manual in PDF format and a template for a packaging desin.
|   └── scribus/
|       Source documents for Scribus with all illustrations and fonts.
|
├── mp3/
|   Voiceset for NOKO in German. English needed? Contribute!
|          
├── schematics/
|   The Schematics for the NOKO parts, the NOKO sewing pattern and shopping lists.
|   └── geda/
|       Source documents for gEDA.
|
├── src/
|   Arduino sketches
|   ├── Deutsch/
|   |   German version
|   |   └── NOKO/
|   |       Main sourcecode with all the required libraries.
|   ├── English/
|   |   English version. 
|   |   └── NOKO/
|   |       Same files but translated. 
|   ├── I2C-Scanner/
|   |   Sketch to scan adresses of all connected I2C-devices.
|   ├── NOKO_Disk1/
|   |   Sketch to write the Disk1 textfile to 24LC256-EEPROMs.
|   └── NOKO_EEPROM_Disk0/
|       Sketch to write NOKO presets to the Arduino EEPROM and the Disk0 textfile to the
|       AH24C32 EEPROM.
|
└── write_EEPROM/
    Tools to write data through serial connection to the Arduino and the EEPROMs. 
```
###Let's get started!

Interested in NOKO? Great! Let's start here:  
[German wiki](https://github.com/NikolaiRadke/NOKO/wiki).  
English wiki will follow. One day...
