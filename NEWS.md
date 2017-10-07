# Newsblog

## 2017
* **07.10.2017** Firmware **1.0-210917** - Enhancement: EEPROM lifecycle was improved mit EEPROM.update() instead od EEPROM.write(). I tried to fix mor bugs but couldn't find any ;-). NOKO_EEPROM_DISK0 was also modified with this function. NOKO_Diagnostics now supports a library-free radio test.  
* **04.10.2017** IDE 1.8.5 is out. Still no compilier options. It seems to be AVR Core specific, maybe platform.txt is being ignored. Stay with 1.6.18 if you care about free flash.  
* **25.08.2017** IDE 1.8.4 with AVR Core 1.6.20 is out. Again, since AVR Core 1.6.19 compiler options aren't supported anymore. You can stay with 1.6.18. 
* **24.08.2017** Firmware **1.0-240817** - Bugfix: Now NOKO supports 95 stories instead of 42.  
* **10.08.2017** NOKO moved to Berlin!  
* **21.07.2017** ...and updated to V0.5. I have had some free time.  
* **20.07.2017** The NOKO diagnostic tool was updated to V0.3.  
* **30.06.2017** **Release V1.0-300617**  
Second NOKO release with new firmware 1.0-300617. I spend a lot of time looking for bugs. Since last firmware 1.0-240617, the belly button debounce was improved again, i also fred up some flash memory und did some small powersave improvents. This should be the last V1.0 release. Hopefully...  
* **24.06.2017** - Firmware **1.0-240617** - Belly button debounce improvement - it was about time! And some code optimizations. 100 Bytes freed! Wheee!  
* **19.06.2017** - Firmware **1.0-190617** - Bugfix: No time refresh when playing radio oder MP3s.  
* **09.06.2017** - Firmware **1.0-090617** - Another firmware update before the upcoming release with serval optimizations and a bugfix: The display won't turn off. 
* **07.06.2017** - New IDE 1.8.3 tested. Compiler options in platform.txt aren't working. Again. AVR Core 1.6.19 increases the sketch size by about 760 Bytes. IDE 1.8.3 with AVR Core 1.6.18 takes 17 Bytes more. NOKO stays with AVR Core 1.6.18.  
* **04.06.2017** - Firmware **1.0-040617** - Code shrinking and - as the reason for a new firmware - a nasty bug got fixed in the distance light settings. A new release will follow soon.  
* **31.05.2017** - Added an early diagnostic tool for NOKO to test functions through serial monitor.  
* **28.05.2017** - Added a TCL/TK Linux firmware flash tool. Should work with OSX, too. A Windows version will follow.  
* **21.05.2017** - Firmware **1.0-210517** - RDS rework. Works MUCH better now. Polling does not block navigation anymore. I am still working on some other improvements, expect a new release in June.    
* **17.05.2017** - **Release V1.0-170517**  
New optimized release. Install it! NOKO will be glad. It is based on firmware 1.0-150517 with the following enhancements:  
  * Custom chars were taken from PROGMEM. That caused too much troubles.  
  * Cleaned up source code, completely in english, including constants, variables and function names.    
  * Autodetection for the display address. Very handy.  
  * Very small bugfixes.  
* **15.05.2017** - Small bugfixes. PROGMEM troubles custom character.  
* **14.05.2017** - The sourcecode is now in English. Renamed constans, functions and variables for better understanding.  
* **02.05.2017** - **Release V1.0-010517 - Buggy V1.0-190417 removed 29.05.2017**  
First release after initial 120516. Optimized and bugfixed 130417. Now there is enough flash free for future enhancements. Feel free to contact me for ideas, bugs and suggestions.  
* **27.04.2017** - Tested compiling options in platform.txt. They are working again with IDE 1.8.2. This saves about 800 bytes flash.  
* **19.04.2017** - Firmware **1.0-190417** - Fixed ADC bug after power safe mode.  
* **13.04.2017** - Well - it is **1.0-130417** now. Found a more flash-saving way.   
* **12.04.2017** - New firmware **1.0-120417** is out. After 11 month NOKO gets some small enhancements:
  * Custom chars are now in PROGMEM  
  * Power saving when idle was improved.
  * The life cycle of the internal EEPROM was highly improved (several decades).
  * Small bugfixes and typos in the comments where removed.
* **24.03.2017** - New IDE 1.8.2 with AVR Core 1.6.18 works too.  
* **22.03.2017** - NOKO was guest at CeBIT faire 2017: [https://www.youtube.com/watch?v=V2i4UrzMkM8#action=share]  
* **07.03.2017** - All four issues are now available as an ebook in all common stores and [heise shop](https://shop.heise.de/katalog/make-noko-das-mp3-monster?pk_campaign=dossnoko&pk_kwd=t1).  
* **22.02.2017** - The fourth und last issue of the Make Magazin arrived: [https://shop.heise.de/katalog/frankensteins-labor].  
* **13.01.2017** - New IDE 1.8.1 with AVR Core 1.6.17 works.  
* **12.01.2017** - Added Windows 10 support for burning the eeproms.  
* **05.01.2017** - **Hardware bug found**: The resistor at D2 moved to D3.  
  
## 2016
* **24.12.2016** - Merry Xmas. A good time to test new 1.8.0 IDE. Works.  
* **15.12.2016** - The third issue of the Make magazin is out: [https://shop.heise.de/katalog/make-06-2016].  
* **02.12.2016** - New IDE 1.6.13 is out. Works, but still no compiling options possible. (Deja Vu)  
* **12.11.2016** - The ST3 002 H5W relay is out of stock. Switched to FRT5 DC5V with a 1N4001 diode.  
* **02.11.2016** - Updated compiling options for newer (1.6.10+) and older (1.6.9-) IDEs.
* **13.10.2016** - The second issue of the Make magazin with the NOKO article ist out. Get it now! Remember your friends... See [https://www.heise.de/make/inhalt/2016/5/68/) for more.  
* **28.09.2016** - New IDE 1.6.12 is out. Works, but still no compiling options possible. 
* **21.09.2016** - Added precompiled hex-files for those who have too much compiling troubles.  
* **28.08.2016** - New IDE 1.6.11 is out. Works, but new AVR Core 1.6.13 doesn't support compiling options anymore. Strange. Core 1.6.12 is still working great.    
* **12.08.2016** - NOKO appears in the next four issues of the german Make magazin! See [https://www.heise.de/make/inhalt/2016/4/66/) for more.  
* **29.07.2016** - IDE 1.6.10 now has LTO, even Windows version! Optiboot and compiler options are not needed anymore, but still recommended.  
* **28.07.2016** - New IDE 1.6.10 is out. Works with NOKO.ino. 
* **28.07.2016** - Reanimated the newsblog. For you and me and NOKO.  
* **06.01.2016** - Joined GitHub  
