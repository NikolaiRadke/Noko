/* 
Library for Sparkfun Si4703 breakout board.
Simon Monks 2011-09-09
Modified by Gary Bell 2013-12-05
Modified by Nikolai Radke 2017-05-17

This is a library wrapper and a few extras to the excellent code produced
by Nathan Seidle from Sparkfun (Beerware).

Nathan's comments......
Look for serial output at 57600bps.
The Si4703 ACKs the first byte, and NACKs the 2nd byte of a read.
1/18 - after much hacking, I suggest NEVER write to a register without first reading the contents of a chip.
ie, don't updateRegisters without first readRegisters.
The Si4703 breakout does work with line out into a stereo or other amplifier. Be sure to test with different length 3.5mm
cables. Too short of a cable may degrade reception.

Nikolai's comments...
This Library was slightly modified for use with NOKO. Thanks to Nathan's great RDS text funtion. Function delay() is
used by this library. However, NOKO uses newdelay() to save a lot of flash space. So newdelay() was
added. 

si4703_init()       Modified for NOKO with direct port manipulation
newdelay()          Was added - saves flash!
powerOff()          Was added
setSoftMuteOff()    Was added (but unused)
setSoftMuteOn()     Was added (but unused)
Set to STEREO       Line 174 in Si4703.cpp
readRDS             Was modified for polling without timeout
readRDS_RadioTtext  The same

Maybe one day I will test RDS performance mode?
*/

#ifndef Si4703_h
#define Si4703_h

#include "Arduino.h"

class Si4703
{
  public:
    Si4703(byte resetPin, byte sdioPin, byte sclkPin);
	void newdelay(uint16_t z); // NEW
	void powerOn();					// call in setup
	void powerOff(); // NEW	
	void setChannel(uint16_t channel);  	// 3 digit channel number
	uint16_t seekUp(); 					// returns the tuned channel or 0
	uint16_t seekDown(); 	
	void setSoftMuteOff(); // NEW	
	void setSoftMuteOn(); // NEW			
	void setVolume(byte volume); 	// 0 to 15
	void readRDS_Radiotext(char* message);
	void readRDS(char* message);	
									// message should be at least 9 chars
									// result will be null terminated
									// timeout in milliseconds
  private:
	byte  _resetPin;
	byte  _sdioPin;
	byte  _sclkPin;
	void si4703_init();
	void readRegisters();
	byte updateRegisters();
	uint16_t seek(byte seekDirection);
	uint16_t getChannel();
	uint16_t si4703_registers[16]; //There are 16 registers, each 16 bits large
	static const uint16_t  FAIL = 0;
	static const uint16_t  SUCCESS = 1;

	static const int  SI4703 = 0x10; //0b._001.0000 = I2C address of Si4703 - note that the Wire function assumes non-left-shifted I2C address, not 0b.0010.000W
	static const uint16_t  I2C_FAIL_MAX = 10; //This is the number of attempts we will try to contact the device before erroring out
	static const uint16_t  SEEK_DOWN = 0; //Direction used for seeking. Default is down
	static const uint16_t  SEEK_UP = 1;

	//Define the register names
	static const uint16_t  DEVICEID = 0x00;
	static const uint16_t  CHIPID = 0x01;
	static const uint16_t  POWERCFG = 0x02;
	static const uint16_t  CHANNEL = 0x03;
	static const uint16_t  SYSCONFIG1 = 0x04;
	static const uint16_t  SYSCONFIG2 = 0x05;
	static const uint16_t  STATUSRSSI = 0x0A;
	static const uint16_t  READCHAN = 0x0B;
	static const uint16_t  RDSA = 0x0C;
	static const uint16_t  RDSB = 0x0D;
	static const uint16_t  RDSC = 0x0E;
	static const uint16_t  RDSD = 0x0F;

	//Register 0x02 - POWERCFG
	static const uint16_t  SMUTE = 15;
	static const uint16_t  DMUTE = 14;
	static const uint16_t  SETMONO = 13;
	static const uint16_t  SKMODE = 10;
	static const uint16_t  SEEKUP = 9;
	static const uint16_t  SEEK = 8;
	static const uint16_t  DISABLE = 6;

	//Register 0x03 - CHANNEL
	static const uint16_t  TUNE = 15;

	//Register 0x04 - SYSCONFIG1
	static const uint16_t  RDS = 12;
	static const uint16_t  DE = 11;

	//Register 0x05 - SYSCONFIG2
	static const uint16_t  SPACE1 = 5;
	static const uint16_t  SPACE0 = 4;

	//Register 0x0A - STATUSRSSI
	static const uint16_t  RDSR = 15;
	static const uint16_t  STC = 14;
	static const uint16_t  SFBL = 13;
	static const uint16_t  AFCRL = 12;
	static const uint16_t  RDSS = 11;
	static const uint16_t  STEREO = 8;
};

#endif








