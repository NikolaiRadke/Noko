# Newsblog

## 2018  
* **11.05.2018** The female voice set ist finished.  
* **30.04.2018** Firmware **X.0-300418** Slightly optimized random generator.  
* **26.04.2018** Good news: The 3D printable casing is ready! Stay tuned for a new 2.0 repository!  
* **06.04.2018** Firmware **X.0-060418** When uploading files NOKO should be quiet. The equalizer menu sets NOKO in upload mode.  
* **04.04.2018** My hoster was offline for more than two weeks. I moved to a new one. The graphics in the GitHub are back now. What a waste of time...  
* **19.03.2018** German version V2.0 now has full Umlaute support.  
* **13.03.2018** The german manual got an update. All new functions are described now.  
* **07.03.2018** Firmware **X.0-070318** - Enhancement: You can select weekdays for the alarm. Very handy!  
* **02.03.2018** Firmware **X.0-020318** - Bugfix: No more compiling warnings. I sacrificed two precious bytes for your relief.  
* **01.03.2018** Testing firmware and tools to be released. The manual got an update too.  
* **28.02.2018** Firmware **1.0-280218**  
  PCB NOKOs got the new update yesterday, today it's time for handmade NOKO.  
  * Enhancement: Older NOKOs got the equalizer, too.  
  * Removed: The obsolete quiet mode.  
* **27.02.2018** Firmware **2.0-270218**  
  Found some time today to do a little work at the player.
  * Enhancement: Added an equalizer menue fot the MP3 Module. See settings menue.  
  * Enhancement: Added two navigation buttons, +10 and -10 to the player for faster file browsing.  
  * Removed: The almost obsolete quiet mode is gone. The equalizier is now in his place. If you want NOKO to be quiet, use the poti.  
  * Bugfix: Very small code cleanup.  
* **26.02.2018** Firmware **2.0-260218** - Bugfix: Fixed wrong story information adressing. Sorry, you need to flash again.   
* **24.02.2018** Firmware **2.0-240218** - There it is. Story informations are now stored in the not-optional-anmymore 24LC256.  
* **23.02.2018** Firmware **X.0-230218** - Enhancement: NOKO shows the duration of a MP3 or a story by his own! Typing the times into NOKO_EEPROM_Disk0.ino is not needed anymore. In an upcoming firmware of V2.0 the story titles and authors will be written to 24LC256 to store more stories.  
* **09.02.2018** Firmware **X.0-080218** - Instead of NewTone library, which caused some troubles, NOKO now uses TimerFreeTone library. NOKO has his lost flash space back and tone generation works great.   
* **08.02.2018** Added Firmware **2.0-080218** - Bugfix: I encountered too much troubles with the tone generation on the PCB. Switched back from the beloved NewTone library to standard tone library. Now it is working fine, but takes 1500 Bytes more flash. Sight.  
* **07.02.2018** Added Firmware **1.0-070218** - Some tone code reworks.   
* **06.02.2018** Added Firmware **2.0-060218** and directory V20. These files are for PCB-NOKOs. V1.0 in folder V10 will certainly get the same maintenance. No NOKO will be left behind!  
* **04.02.2018** Finished testing the PCB. Nearly everything is working! There are some minor issues with the hub left.  
* **03.02.2018** The diagnostics tool got an update to V0.8. I needed more functions to test the new PCB.  
* **15.01.2018** Firmware **1.0-150118** - Enhancement: Story menu no shows length in hours and minutes.  
* **12.01.2018** Added a simple tool to set the current time without touching the EEPROM.  
* **08.01.2018** Finished the enhanced voice set. Firmware was updatet to **1.0-080118**. Have fun!  
  Warning: The new voice set ist preselected. If you want to use the old set, modify the comments in line **116** and **117**.  
* **01.01.2018** Happy new year! Stay tuned for the new NOKO board and the enhanced voice set!  
  
## 2017
* **30.12.2017** Firmware **1.0-301217** - Enhancement/optional: NOKO can read the busy signal reliable analog via A1  
* **18.12.2017** The gerber files are now online! Build your NOKO in an easy way! Thanks to the community!  
* **30.11.2017** Firmware **1.0-301117** - Bugfix: The story addressing is now working correctly.  
NOTE: You need to reflash your AH24C32. Sorry. I'm a lousy coder.  
* **07.10.2017** Firmware **1.0-210917** - Enhancement: EEPROM lifecycle was improved mit EEPROM.update() instead of EEPROM.write(). I tried to fix more bugs but couldn't find any of them ;-). NOKO_EEPROM_DISK0 was also modified with this function. NOKO_Diagnostics V0.5 now supports a library-free radio test.  
* **04.10.2017** IDE 1.8.5 is out. Still no compilier options. It seems to be AVR Core specific, maybe platform.txt is being ignored. Stay with 1.6.18 if you care about free flash.  
* **25.08.2017** IDE 1.8.4 with AVR Core 1.6.20 is out. Again, since AVR Core 1.6.19 compiler options aren't supported anymore. You can stay with 1.6.18. 
* **24.08.2017** Firmware **1.0-240817** - Bugfix: Now NOKO supports 95 stories instead of 42.  
* **10.08.2017** NOKO moved to Berlin!  
* **21.07.2017** ...and updated to V0.4. I have had some free time.  
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
