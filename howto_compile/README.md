#Compiling options for NOKO
###NOTE: These options are used global for all sketches.

To Compile NOKO.ino in the Arduino IDE 1.6.6 or 1.6.7, you need to modify the file **platform.txt**.
- In Linux, it can be found under 
`/usr/share/arduino/hardware/arduino/avr/platform.txt`

- In Windows under 
`[Arduino folder]/hardware/arduino/avr/platform.txt`.

Open your **platform.txt** and overwrite the lines after the line
```
# These can be overridden in platform.local.txt
```
with the lines below. Without these options the compiled sketch would exceed the 32kb flash memory of the Arduino Nano.

Alas, until now i wasn't able to find out more about *platform.local.txt*, which allows the user to define a specific platform.txt locally for each sketch. So better backup your old platform.txt to save your old compiler options. 

### Linux  

```
# These can be overridden in platform.local.txt
# Compiling options for NOKO and NOKO.ino
compiler.c.extra_flags=-Wextra -flto -funsafe-math-optimizations -mcall-prologues -maccumulate-args -ffunction-sections -fdata-sections -fmerge-constants
compiler.c.elf.extra_flags=-w -flto -funsafe-math-optimizations -mcall-prologues -maccumulate-args -ffunction-sections -fdata-sections -fmerge-constants
compiler.S.extra_flags=
compiler.cpp.extra_flags=-Wextra -flto -funsafe-math-optimizations -mcall-prologues -maccumulate-args -ffunction-sections -fdata-sections -fmerge-constants
compiler.ar.extra_flags=
compiler.objcopy.eep.extra_flags=
compiler.elf2hex.extra_flags=
```
### Windows
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
Unfortunately, on Windows I couldn't enable Link Time Optimizations with the *-flto* switch. The sketch fits barely into flash memory, now **Optiboot** is a **must have**! See [Issue 3](https://github.com/NikolaiRadke/NOKO/issues/3).  
An intruction on how to flash a new bootloader will follow in the Wiki. Until than take a look [here](https://www.arduino.cc/en/Tutorial/ArduinoISP).

###Note:
The sketch will **not** run without the Si4703 radio module. If you want to test it anyway, you have to modify *NOKO,ino*. Change the line  
```#define def_radio 1           // Using Radio?```   
in the beginning of the sketch into   
```//#define def_radio 1           // Using Radio?```  
Now, the radio functions are commented out and disabled.

