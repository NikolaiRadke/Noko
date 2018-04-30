#include "Arduino.h"
#include "Si4703.h"
#include "Wire.h"

Si4703::Si4703(byte resetPin, byte sdioPin, byte sclkPin)
{
  _resetPin = resetPin;
  _sdioPin = sdioPin;
  _sclkPin = sclkPin;
}

void Si4703::newdelay(uint16_t z)  // New delay function to save flash
{
  unsigned long zmillis=millis();
  while (millis()-zmillis<z);
}

void Si4703::powerOn()
{
    si4703_init();
}

void Si4703::powerOff()
{
//	Power off via Reset:
//  digitalWrite(_sdioPin, LOW); //A low SDIO indicates a 2-wire interface
//  digitalWrite(_resetPin, LOW); //Put Si4703 into reset
//  newdelay(1); //Some newdelays while we allow pins to settle
	readRegisters();
	si4703_registers[POWERCFG] |= (1<<DISABLE);
	updateRegisters(); 
}

void Si4703::setChannel(uint16_t channel)
{
  //Freq(MHz) = 0.200(in USA) * Channel + 87.5MHz
  //97.3 = 0.2 * Chan + 87.5
  //9.8 / 0.2 = 49
  uint16_t newChannel = channel * 10; //973 * 10 = 9730
  newChannel -= 8750; //9730 - 8750 = 980
  newChannel /= 10; //980 / 10 = 98

  //These steps come from AN230 page 20 rev 0.5
  readRegisters();
  si4703_registers[CHANNEL] &= 0xFE00; //Clear out the channel bits
  si4703_registers[CHANNEL] |= newChannel; //Mask in the new channel
  si4703_registers[CHANNEL] |= (1<<TUNE); //Set the TUNE bit to start
  updateRegisters();

  //newdelay(60); //Wait 60ms - you can use or skip this newdelay

  //Poll to see if STC is set
  while(1) {
    readRegisters();
    if( (si4703_registers[STATUSRSSI] & (1<<STC)) != 0) break; //Tuning complete!
  }

  readRegisters();
  si4703_registers[CHANNEL] &= ~(1<<TUNE); //Clear the tune after a tune has completed
  updateRegisters();

  //Wait for the si4703 to clear the STC as well
  while(1) {
    readRegisters();
    if( (si4703_registers[STATUSRSSI] & (1<<STC)) == 0) break; //Tuning complete!
  }
}

uint16_t Si4703::seekUp()
{
	return seek(SEEK_UP);
}

uint16_t Si4703::seekDown()
{
	return seek(SEEK_DOWN);
}

void Si4703::setSoftMuteOn()
{
	readRegisters();
	si4703_registers[POWERCFG] &= ~(1<<DMUTE);
	updateRegisters();
}

void Si4703::setSoftMuteOff()
{
	readRegisters();
	si4703_registers[POWERCFG] |= (1<<DMUTE);
	updateRegisters();
}

void Si4703::setVolume(byte volume)
{
  readRegisters(); //Read the current register set
  if(volume < 0) volume = 0;
  if (volume > 15) volume = 15;
  si4703_registers[SYSCONFIG2] &= 0xFFF0; //Clear volume bits
  si4703_registers[SYSCONFIG2] |= volume; //Set new volume
  updateRegisters(); //Update
}

//To get the Si4703 inito 2-wire mode, SEN needs to be high and SDIO needs to be low after a reset
//The breakout board has SEN pulled high, but also has SDIO pulled high. Therefore, after a normal power up
//The Si4703 will be in an unknown state. RST must be controlled
//NOKO: RST is defined in NOKO.ino. PinMode und digitalWrite were removed. 
void Si4703::si4703_init() 
{
  PORTD &= ~(1<<5); //digitalWrite(_resetPin, LOW); //Put Si4703 into reset
  newdelay(1); //Some newdelays while we allow pins to settle
  PORTD |= (1<<5); //digitalWrite(_resetPin, HIGH); //Bring Si4703 out of reset with SDIO set to low and SEN pulled high with on-board resistor
  newdelay(1); //Allow Si4703 to come out of reset

  Wire.begin(); //Now that the unit is reset and I2C inteface mode, we need to begin I2C

  readRegisters(); //Read the current register set
  //si4703_registers[0x07] = 0xBC04; //Enable the oscillator, from AN230 page 9, rev 0.5 (DOES NOT WORK, wtf Silicon Labs datasheet?)
  si4703_registers[0x07] = 0x8100; //Enable the oscillator, from AN230 page 9, rev 0.61 (works)
  updateRegisters(); //Update

  newdelay(500); //Wait for clock to settle - from AN230 page 9

  readRegisters(); //Read the current register set
  si4703_registers[POWERCFG] = 0x4001; //Enable the IC
  si4703_registers[POWERCFG] |= (1<<SMUTE) | (1<<DMUTE); //Disable Mute, disable softmute
  si4703_registers[SYSCONFIG1] |= (1<<RDS); //Enable RDS

  si4703_registers[SYSCONFIG1] |= (1<<DE); //50kHz Europe setup
  si4703_registers[SYSCONFIG2] |= (1<<SPACE0); //100kHz channel spacing for Europe

  //si4703_registers[POWERCFG] |= (1 << SETMONO); // Mono as default

  si4703_registers[SYSCONFIG2] &= 0xFFF0; //Clear volume bits
  si4703_registers[SYSCONFIG2] |= 15; //Set volume to highest
  updateRegisters(); //Update

  newdelay(110); //Max powerup time, from datasheet page 13
}

//Read the entire register control set from 0x00 to 0x0F
void Si4703::readRegisters(){

  //Si4703 begins reading from register upper register of 0x0A and reads to 0x0F, then loops to 0x00.
  Wire.requestFrom(SI4703, 32); //We want to read the entire register set from 0x0A to 0x09 = 32 bytes.

  //while(Wire.available() < 32) ; //Wait for 16 doubles/32 bytes to come back from slave I2C device - Not needed!
  //We may want some time-out error here

  //Remember, register 0x0A comes in first so we have to shuffle the array around a bit
  for(int x = 0x0A ; ; x++) { //Read in these 32 bytes
    if(x == 0x10) x = 0; //Loop back to zero
    si4703_registers[x] = Wire.read() << 8;
    si4703_registers[x] |= Wire.read();
    if(x == 0x09) break; //We're done!
  }
}

//Write the current 9 control registers (0x02 to 0x07) to the Si4703
//It's a little weird, you don't write an I2C addres
//The Si4703 assumes you are writing to 0x02 first, then increments
byte Si4703::updateRegisters() {

  Wire.beginTransmission(SI4703);
  //A write command automatically begins with register 0x02 so no need to send a write-to address
  //First we send the 0x02 to 0x07 control registers
  //In general, we should not write to registers 0x08 and 0x09
  for(int regSpot = 0x02 ; regSpot < 0x08 ; regSpot++) {
    byte high_byte = si4703_registers[regSpot] >> 8;
    byte low_byte = si4703_registers[regSpot] & 0x00FF;

    Wire.write(high_byte); //Upper 8 bits
    Wire.write(low_byte); //Lower 8 bits
  }

  //End this transmission
  byte ack = Wire.endTransmission();
  if(ack != 0) { //We have a problem! 
    return(FAIL);
  }

  return(SUCCESS);
}

//Seeks out the next available station
//Returns the freq if it made it
//Returns zero if failed
uint16_t Si4703::seek(byte seekDirection){
  readRegisters();
  //Set seek mode wrap bit
  //si4703_registers[POWERCFG] |= (1<<SKMODE); //Allow wrap
  si4703_registers[POWERCFG] &= ~(1<<SKMODE); //Disallow wrap - if you disallow wrap, you may want to tune to 87.5 first
  if(seekDirection == SEEK_DOWN) si4703_registers[POWERCFG] &= ~(1<<SEEKUP); //Seek down is the default upon reset
  else si4703_registers[POWERCFG] |= 1<<SEEKUP; //Set the bit to seek up

  si4703_registers[POWERCFG] |= (1<<SEEK); //Start seek
  updateRegisters(); //Seeking will now start

  //Poll to see if STC is set
  while(1) {
    readRegisters();
    if((si4703_registers[STATUSRSSI] & (1<<STC)) != 0) break; //Tuning complete!
  }

  readRegisters();
  int valueSFBL = si4703_registers[STATUSRSSI] & (1<<SFBL); //Store the value of SFBL
  si4703_registers[POWERCFG] &= ~(1<<SEEK); //Clear the seek bit after seek has completed
  updateRegisters();

  //Wait for the si4703 to clear the STC as well
  while(1) {
    readRegisters();
    if( (si4703_registers[STATUSRSSI] & (1<<STC)) == 0) break; //Tuning complete!
  }

  if(valueSFBL) { //The bit was set indicating we hit a band limit or failed to find a station
    return(0);
  }
return getChannel();
}

//Reads the current channel from READCHAN
//Returns a number like 973 for 97.3MHz
uint16_t Si4703::getChannel() {
  readRegisters();
  uint16_t channel = si4703_registers[READCHAN] & 0x03FF; //Mask out everything but the lower 10 bits
  //Freq(MHz) = 0.100(in Europe) * Channel + 87.5MHz
  //X = 0.1 * Chan + 87.5
  channel += 875; //98 + 875 = 973
  return(channel);
}

void Si4703::readRDS(char* buffer) 
{ 
  readRegisters();
  if(si4703_registers[STATUSRSSI] & (1<<RDSR))
  {
    uint16_t b = si4703_registers[RDSB];
    uint16_t index = b & 0x03;
    if (b < 500)
    {
      char Dh = (si4703_registers[RDSD] & 0xFF00) >> 8;
      char Dl = (si4703_registers[RDSD] & 0x00FF);
      buffer[index * 2] = Dh;
      buffer[index * 2 +1] = Dl;
    }
    //newdelay(40); //Wait for the RDS bit to clear
  }
  buffer[8] = '\0';
}

void Si4703::readRDS_Radiotext(char* buffer)
{ 
	int indexHighest = 0;
  if(si4703_registers[STATUSRSSI] & (1<<RDSR)) 
  {
    readRegisters();
  	uint16_t b = si4703_registers[RDSB];
  	int index;
  	index = b & 0xF800;
  	if (index == 0x2000) 
  	{
  	  index = b & 0x0F; 
  	  if (index > (indexHighest -1)) indexHighest = index;
      char Dh = (si4703_registers[RDSC] & 0xFF00) >> 8;
	    char Dl = (si4703_registers[RDSC] & 0x00FF);
	    buffer[(index * 4)] = Dh;
	    buffer[(index * 4) + 1] = Dl;
	    Dh = (si4703_registers[RDSD] & 0xFF00) >> 8;
		  Dl = (si4703_registers[RDSD] & 0x00FF);
	    buffer[(index * 4) + 2] = Dh;
	    buffer[(index * 4) + 3] = Dl;
  	}
  }
  buffer[(indexHighest * 4) + 4] = '\0';
}







