# ![NOKO](http://www.nikolairadke.de/NOKO/noko_klein.png) NOKO

The **NOKO Monster**, lovely and naughty, ready to build on your own. And everyone is invited to **contribute**. NOKO is for everyone. 

<table border="0">
  <tr>
    <td><img src="http://www.nikolairadke.de/NOKO/noko_nr2.png" /></td>
    <td>
    NOKO is a multifunctional monster with an <b>Arduino Nano</b> as microcontroller, display, clock, radio, mp3 player,           four buttons as user interface, battery, voice set and  tons of gimmicks, packed in a cuddly monster suit - sturdy,
    lovely and sometimes a bit mean and annoying. Just the way you want him to be. He, she or it - this is your decision     and only depends on the voice set. In this repository NOKO is a lad, a real bloke! A buddy for life and a companion      by day and night. All he needs is a litte power every now and then.<br />
    <br>
    <b>NOKO</b> stands for <b>NO</b>ra's and ni<b>KO</b>'s monster.
    </td> 
  </tr>
</table>

###Notes for non-German NOKO makers
There are a German and an English version. Right now, the building wiki, the voice set, the manual and the optional EEPROM text files are in German. I'll try to translate the wiki and the manual when the German wiki is completed. Until then, feel free to ask me for translation, if you need help.  **Contribute!** I need an English voice set and English text files. In addition, any other language is welcome, NOKO is to be a cosmopolitan!

### Prerequisites
NOKO needs [Arduino IDE 1.6.7](https://www.arduino.cc/en/Main/Software) or [Arduino IDE 1.6.6](https://www.arduino.cc/en/Main/OldSoftwareReleases#previous). Copy the folder `NOKO/src/[language]/NOKO/` into your sketch folder. At this moment, *NOKO.ino* won't compile, if you are a Windows user. The Sketch Size is too large. See [how to compile](https://github.com/NikolaiRadke/NOKO/tree/master/howto_compile) to solve this issue.

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
|   The Schematics for the NOKO modules and the NOKO sewing pattern.
|   └── geda/
|       Source documents for gEDA.
|
├── src/
|   Arduino sketches
|   ├── Deutsch/
|   |   German version
|   |   ├── NOKO/
|   |   |   Main sourcecode with all the required libraries.
|   |   ├── NOKO_Disk1/
|   |   |   Sketch to write the DISK1 textfile to 24LC256-EEPROMs.
|   |   └── NOKO_EEPROM_Disk0/
|   |       Sketch to write NOKO presets to the Arduino EEPROM and the DISK0 textfile to the
|   |       AH24C32 EEPROM.
|   |
|   └── English/
|       English version. Same files but translated. 
|       ├── NOKO/
|       ├── NOKO_Disk1/
|       └── NOKO_EEPROM_Disk0/
|      
└── write_EEPROM/
    Tools to write data through serial connection to the Arduino and the EEPROMs. 
```
###Let's get started!

Interested in NOKO? Great! Let's start here:  
[German wiki](https://github.com/NikolaiRadke/NOKO/wiki).  
English wiki will follow...
