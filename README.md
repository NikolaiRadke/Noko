# ![Noko](http://www.nikolairadke.de/NOKO/noko_back.jpg)
  
**Note: This Repository is getting a rework.**

The DIY-**Noko Monster**, lovely and naughty, ready to build on your own. And everyone is invited to **contribute**. Noko is for everyone. What's new? See the [newsblog](https://github.com/NikolaiRadke/NOKO/tree/master/NEWS.md).  
  
![Noko Orange](http://www.nikolairadke.de/NOKO/noko_echt.png)  

Noko is a multifunctional monster with an ATmega328  microcontroller on an **Arduino Nano**, display, clock, radio, mp3 player, four buttons as user interface, battery, voice set and  tons of gimmicks, packed in a cuddly monster suit - sturdy,
lovely and sometimes a bit mean and annoying. Just the way you want him to be. He, she or it - this is your decision     and only depends on the voice set. In this repository Noko is a lad, a real bloke! A buddy for life and a companion by day and night. All he needs is a little power every now and then.  

**Noko** stands for **No**ra and ni**ko**'s monster.

### Notes for non-German Noko makers
There are a German and an English version. Right now, the building wiki, the voice set, the manual and the optional EEPROM text files are in German. I'll try to translate the wiki and the manual one day. Until then, feel free to ask me for translation, if you need help. **Contribute!** I need an English voice set and English text files. In addition, any other language is welcome, Noko is to be cosmopolitan!

### Prerequisites
Noko needs Arduino IDE 1.6.6 - [1.8.2](https://www.arduino.cc/en/Main/Software) Copy the folder `Noko/V10/src/[language]/Noko/` into your sketch folder. When using 1.6.10 or newer with AVR core 1.6.12 or newer, everything will compile out-of-the-box. With 1.6.9 or lower, *Noko.ino* won't compile, the sketch size is too large. 

**Option 1:** Install newest  [IDE](https://www.arduino.cc/en/Main/Software) and relax.  
**Option 2:** If you are using IDE 1.6.9 or lower, you need to install [Optiboot](https://github.com/Optiboot/optiboot) onto your Nano.  
**Option 3:** Take one of the precompiled hex-files and follow the instructions in [how to compile](https://github.com/NikolaiRadke/Noko/tree/master/howto_compile).  

**In any case, Optiboot is strongly recommended.**

### Content

```
Noko/
├── howto_compile/
|   Manual how compile the sketches in your Arduino IDE and precompiled sketches.  
|   └── precompiled/
|       Precompiled hex-files and windows tools.
|       ├── Deutsch/
|       |   German Versions.
|       ├── English/
|       |   English Versions.
|       ├── Flashtool/
|       |   Linux firmware flasher.
|       └── avrdude_win/
|           Avrdude files for windows.
|
├── manual/
|   Noko manual in PDF format and a template for a packaging design.
|   └── scribus/
|       Source documents for Scribus with all illustrations and fonts.
|
├── mp3/
|   Female and male voice sets for Noko in German. English needed? Contribute!
|   ├── female/  
|   |   Female voice set for Nokos.
|   |   └── VoiceSet_226/
|   |       New voice files.
|   └── male/
|       Male voice sets
|       ├── alternative/
|       |   Some less rude voice files.
|       ├── VoiceSet_111/
|       |   Voice set for older Nokos.
|       └── VoiceSet_226/
|           Enhanced voice set.
| 
├── schematics/
|   The Schematics for the Noko parts, the Noko sewing pattern and shopping lists.
|   └── geda/
|       Source documents for gEDA.
|
├── src/
|   Arduino sketches. V10 for the Noko of this Wiki, V20 for PCB Nokos.  
|   ├── V10/
|   |   ├── deutsch/
|   |   |   German version
|   |   |   └── Noko/
|   |   |       Main sourcecode with all the required libraries.
|   |   ├── english/
|   |   |   English version. 
|   |   |   └── Noko/
|   |   |       Same files but translated. 
|   |   └── tools/
|   |       Tools for using components and diagnostic.    
|   |       ├── I2C-Scanner/
|   |       |   Sketch to scan adresses of all connected I2C-devices.
|   |       ├── Noko_Diagnostics/
|   |       |   Tool to test Noko functions via serial monitor.
|   |       ├── Noko_Disk1/
|   |       |   Sketch to write the Disk1 textfile into 24LC256-EEPROMs.
|   |       ├── Noko_EEPROM_Disk0/
|   |       |   Sketch to write Noko presets into the Arduino EEPROM and the Disk0 textfile into the
|   |       |   AH24C32 EEPROM.
|   |       └── Noko_Time/
|   |           Simple tool to set the current time without touching the EEPROM
|   └── V20/
|       ├── deutsch/
|       |   German version
|       |   └── Noko/
|       |       Main sourcecode with all the required libraries.
|       ├── english/
|       |   English version. 
|       |   └── Noko/
|       |       Same files but translated. 
|       └── tools/
|           Tools for using components and diagnostic.    
|           ├── I2C-Scanner/
|           |   Sketch to scan adresses of all connected I2C-devices.
|           ├── Noko_Diagnostics/
|           |   Tool to test Noko functions via serial monitor.
|           ├── Noko_EEPROM/
|           |   Sketch to write the textfile EEPROM into 24LC256-EEPROMs.
|           ├── Noko_Settings/
|           |   Sketch to write Noko presets into the Arduino EEPROM and the constants into the
|           |   AH24C32 EEPROM.
|           └── Noko_Time/
|               Simple tool to set the current time without touching the EEPROM.
|    
└── write_EEPROM/
    Tools to write data through serial connection to the Arduino and the EEPROMs. 
    ├──  V10/
    |    Old version, working with this wiki.  
    └──  V20/
         New upcoming version for PCB Nokos.
```
### Let's get started!

Interested in Noko? Great! Let's start here: [German wiki](https://github.com/NikolaiRadke/Noko/wiki).  
English wiki will follow. One day...
