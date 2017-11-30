# ![NOKO](http://www.nikolairadke.de/NOKO/noko_klein.png) NOKO

The DIY-**NOKO Monster**, lovely and naughty, ready to build on your own. And everyone is invited to **contribute**. NOKO is for everyone. What's new? See the [newsblog](https://github.com/NikolaiRadke/NOKO/tree/master/NEWS.md).  

**31.11.2017** - The PCB is ready! Stay tuned and read more in a frew days!  

![NOKO Orange](http://www.nikolairadke.de/NOKO/noko_echt.png)  

NOKO is a multifunctional monster with an ATmega328  microcontroller on an **Arduino Nano**, display, clock, radio, mp3 player, four buttons as user interface, battery, voice set and  tons of gimmicks, packed in a cuddly monster suit - sturdy,
lovely and sometimes a bit mean and annoying. Just the way you want him to be. He, she or it - this is your decision     and only depends on the voice set. In this repository NOKO is a lad, a real bloke! A buddy for life and a companion by day and night. All he needs is a little power every now and then.  

**NOKO** stands for **NO**ra's and ni**KO**'s monster.

### Notes for non-German NOKO makers
There are a German and an English version. Right now, the building wiki, the voice set, the manual and the optional EEPROM text files are in German. I'll try to translate the wiki and the manual when the German wiki is completed. Until then, feel free to ask me for translation, if you need help.  **Contribute!** I need an English voice set and English text files. In addition, any other language is welcome, NOKO is to be cosmopolitan!

### Prerequisites
NOKO needs Arduino IDE 1.6.6 - [1.8.2](https://www.arduino.cc/en/Main/Software) Copy the folder `NOKO/src/[language]/NOKO/` into your sketch folder. When using 1.6.10 or newer with AVR core 1.6.12 or newer, everything will compile out-of-the-box. With 1.6.9 or lower, *NOKO.ino* won't compile, the sketch size is too large. 

**Option 1:** Install newest  [IDE](https://www.arduino.cc/en/Main/Software) and relax.  
**Option 2:** If you are using IDE 1.6.9 or lower, you need to install [Optiboot](https://github.com/Optiboot/optiboot) onto your Nano.  
**Option 3:** Take one of the precompiled hex-files and follow the instructions in [how to compile](https://github.com/NikolaiRadke/NOKO/tree/master/howto_compile).  

**In any case, Optiboot is strongly recommended.**

### Content

```
NOKO/
├── howto_compile/
|   Manual how compile the sketches in your Arduino IDE and precompiled sketched  
|   └── precompiled/
|       Precompiled hex-files and windows tools
|       ├── Deutsch/
|       |   German Versions
|       ├── English/
|       |   English Versions
|       ├── Flashtool/
|       |   Linux firmware flasher
|       └── avrdude_win/
|           Avrdude files for windows/
|
├── manual/
|   NOKO manual in PDF format and a template for a packaging desin.
|   └── scribus/
|       Source documents for Scribus with all illustrations and fonts.
|
├── mp3/
|   Voiceset for NOKO in German. English needed? Contribute!
|   └── alternative/
|       Some less rude voice files.
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
|   └── tools/
|       Tools for using components and diagnostic    
|       ├── I2C-Scanner/
|       |   Sketch to scan adresses of all connected I2C-devices.
|       ├── NOKO_Diagnostics/
|       |   Tool to test NOKO functions via serial monitor.
|       ├── NOKO_Disk1/
|       |   Sketch to write the Disk1 textfile into 24LC256-EEPROMs.
|       └── NOKO_EEPROM_Disk0/
|           Sketch to write NOKO presets into the Arduino EEPROM and the Disk0 textfile into the
|           AH24C32 EEPROM.
└── write_EEPROM/
    Tools to write data through serial connection to the Arduino and the EEPROMs. 
```
### Let's get started!

Interested in NOKO? Great! Let's start here:  
[German wiki](https://github.com/NikolaiRadke/NOKO/wiki).  
English wiki will follow. One day...
