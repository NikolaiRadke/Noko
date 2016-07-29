#Compiling options for NOKO
###NOTE: These options are used globally for all sketches.

**Prerequisits:**  
If you have Optiboot installed, you don't need to change anything. If you have IDE 1.6.10 with AVR Core 1.6.12, relax. But it's still recomended to do both. 

Linux: with IDE 1.6.6-1.6.9 use AVR Core uns see intructions below.
Windows: 1.6.9 - 1.6.9 do *not* work without Optiboot. Install Optiboot and see intructions below.

To Compile NOKO.ino, you need to modify the file **platform.txt**.
- In Linux, it can be found under 
`/usr/share/arduino/hardware/arduino/avr/platform.txt`

- In Windows under 
`[Arduino folder]/hardware/arduino/avr/platform.txt`.

Open your **platform.txt** and overwrite the lines after the line
```
# These can be overridden in platform.local.txt
```
with the lines below. Without these options the compiled sketch would exceed the 30kb flash memory of the Arduino Nano. With Optiboot, it will just fit.

Alas, until now I wasn't able to find out more about *platform.local.txt*, which allows the user to define a specific platform.txt locally for each sketch. So better backup your old platform.txt to save your old compiler options. 

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
Unfortunately, on Windows the Link Time Optimization (LTO) plugin is disabled in IDE 1.6.6-1.6.9, the *-flto* switch is unusable. The sketch fits barely into flash memory, so **Optiboot** is a **must have**! See [Issue 3](https://github.com/NikolaiRadke/NOKO/issues/3).  
You can find an intruction on how to flash a new bootloader in German [here in the wiki](https://github.com/NikolaiRadke/NOKO/wiki/Optiboot). An English instruction will follow. Until then take a look [here](https://www.arduino.cc/en/Tutorial/ArduinoISP).

###Note:
The sketch will **not** run without the Si4703 radio module. If you want to test it anyway, you have to modify *NOKO,ino*. Change the line  
```#define def_radio 1           // Using Radio?```   
in the beginning of the sketch into   
```//#define def_radio 1           // Using Radio?```    

Now, the radio functions are commented out and disabled.

