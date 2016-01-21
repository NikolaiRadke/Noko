#Compiling options for NOKO
###NOTE: These options are used global for all sketches.

To Compile NOKO.ino in the Arduino IDE 1.6.6, you need to modify the file **platform.txt**.
In Linux, it can be found under 
> /usr/share/arduino/hardware/arduino/avr/platform.txt

In Windows under 
> [Arduino folder]/hardware/arduino/avr/platform.txt.

Open your **platform.txt** and overwrite the lines after the line
```
# These can be overridden in platform.local.txt
```
with the lines below.

Alas, until now i wasn't able to find out more about *platform.local.txt*. 
So backup you old platform.txt to save your old compiler options. 

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
