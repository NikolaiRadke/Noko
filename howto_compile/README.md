# Compiling options for NOKO
### NOTE: These options are used globally for all sketches.

**Prerequisits**  
If you have Optiboot installed, you don't need to change anything.  
If you have IDE 1.6.10 or higher with AVR Core 1.6.12 or higher, relax, but Optiboot is always recommended.  

Linux: with IDE 1.6.6-1.6.9 use AVR Core 1.6.9 and see intructions below.  
Windows: IDE 1.6.6-1.6.9 will *not* work without Optiboot. Install Optiboot and see intructions below.

### Note:
The sketch will **not** run without the Si4703 radio module. If you want to test it anyway, you have to modify *NOKO,ino*. Change the line  
```#define def_radio 1           // Using Radio?```   
in the beginning of the sketch into   
```//#define def_radio 1           // Using Radio?```    

Now, the radio functions are commented out and disabled.  

##Instructions for older IDEs   
To compile NOKO.ino, you need to modify the file **platform.txt**.
- In Linux, it can be found under 
`/usr/share/arduino/hardware/arduino/avr/platform.txt`

- In Windows under 
`[Arduino folder]/hardware/arduino/avr/platform.txt`

Open your **platform.txt** and overwrite the lines after the line
```
# These can be overridden in platform.local.txt
```
with the lines below. Without these options the compiled sketch would exceed the 30kb flash memory of the Arduino Nano. With Optiboot, it will just fit.

Alas, until now I wasn't able to find out more about *platform.local.txt*, which allows the user to define a specific platform.txt locally for each sketch. So better backup your old platform.txt to save your old compiler options. 

### Linux and Windows 1.8.2 and higher (optional, saves about 700 byte flash)

```
# These can be overridden in platform.local.txt
compiler.c.extra_flags=-funsafe-math-optimizations -mcall-prologues -maccumulate-args -ffunction-sections -fdata-sections -fmerge-constants
compiler.c.elf.extra_flags=-funsafe-math-optimizations -mcall-prologues -maccumulate-args -ffunction-sections -fdata-sections -fmerge-constants
compiler.S.extra_flags=
compiler.cpp.extra_flags=-funsafe-math-optimizations -mcall-prologues -maccumulate-args -ffunction-sections -fdata-sections -fmerge-constants
compiler.ar.extra_flags=
compiler.objcopy.eep.extra_flags=
compiler.elf2hex.extra_flags=
```  

### Linux 1.6.9 and below  

```
# These can be overridden in platform.local.txt
# Compiling options for NOKO and NOKO.ino
compiler.c.extra_flags=-flto -funsafe-math-optimizations -mcall-prologues -maccumulate-args -ffunction-sections -fdata-sections -fmerge-constants
compiler.c.elf.extra_flags=-flto -funsafe-math-optimizations -mcall-prologues -maccumulate-args -ffunction-sections -fdata-sections -fmerge-constants
compiler.S.extra_flags=
compiler.cpp.extra_flags=-flto -funsafe-math-optimizations -mcall-prologues -maccumulate-args -ffunction-sections -fdata-sections -fmerge-constants
compiler.ar.extra_flags=
compiler.objcopy.eep.extra_flags=
compiler.elf2hex.extra_flags=
```
### Windows 1.6.9 and below  
```
# These can be overridden in platform.local.txt
# Compiling options for NOKO and NOKO.ino
compiler.c.extra_flags=-funsafe-math-optimizations -mcall-prologues -maccumulate-args -ffunction-sections -fdata-sections -fmerge-constants
compiler.c.elf.extra_flags=-funsafe-math-optimizations -mcall-prologues -maccumulate-args -ffunction-sections -fdata-sections -fmerge-constants
compiler.S.extra_flags=
compiler.cpp.extra_flags=-funsafe-math-optimizations -mcall-prologues -maccumulate-args -ffunction-sections -fdata-sections -fmerge-constants
compiler.ar.extra_flags=
compiler.objcopy.eep.extra_flags=
compiler.elf2hex.extra_flags=
```
Unfortunately, on Windows the Link Time Optimization (LTO) plugin is disabled in IDE 1.6.6-1.6.9, the *-flto* switch is unusable. The sketch fits barely into flash memory, so **Optiboot** is a **must have**! See [Issue 3](https://github.com/NikolaiRadke/NOKO/issues/3).  
You can find an intruction on how to flash a new bootloader in German [here in the wiki](https://github.com/NikolaiRadke/NOKO/wiki/Optiboot). An English instruction will follow. Until then take a look [here](https://www.arduino.cc/en/Tutorial/ArduinoISP).

## How to upload a precompiled hex-file  

Select the hex-file of your choice in [howto_compile/precompiled](https://github.com/NikolaiRadke/NOKO/tree/master/howto_compile/precompiled).

**Linux:**  
See if you habe *avrdude* installed. Open a shell and type ```avrdude```. If not, install is with ```sudo apt-get install avrdude```. Test again. Connect your Arduino via USB, open a terminal and use the following command:

With OptiBoot:  
```avrdude -Cavrdude.conf -v -patmega328p -carduino -P/dev/ttyUSBX -b115200 -D -Uflash:w:XXXXX.hex:i```  
Without OptiBoot:  
```avrdude -Cavrdude.conf -v -patmega328p -carduino -P/dev/ttyUSBX -b57600 -D -Uflash:w:XXXXX.hex:i```  

* XXXXX.hex is the name of your selected hex-file.  
* X in ttyUSBX is the number of your USB-port connected to the Arduino. See *lsusb* or look into the Arduino-IDE for the correct number.  

**Windows: (Stupid stupid Windows)**  
Get *avrdude* either from sourceforge or better, because you need libusb0.dll as well, from [howto_compile/precompiled/avrdude_win](https://github.com/NikolaiRadke/NOKO/tree/master/howto_compile/precompiled/avrdude_win). Copy the files to your machine and add the hex-file of your choice from [howto_compile/precompiled](https://github.com/NikolaiRadke/NOKO/tree/master/howto_compile/precompiled) into the same directory. Open the command prompt with admin rights und use the following command:  

With OptiBoot:  
```avrdude v -patmega328p -carduino -P\\.\COMX -b115200 -D -Uflash:w:XXXXX.hex:i```  
Without OptiBoot:  
```avrdude v -patmega328p -carduino -P\\.\COMX -b57600 -D -Uflash:w:XXXXX.hex:i```   

* XXXXX.hex is the name of your selected hex-file.  
* X in COMX is the number of your USB-port connected to the Arduino. Look into the Arduino-IDE for the correct number. 




