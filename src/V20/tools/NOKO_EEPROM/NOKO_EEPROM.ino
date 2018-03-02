/*
 * NOKO EEPROM writer V2.0 26.02.2017 - Nikolai Radke
 * 
 * This sketch writes the event poems, phrases and stuff into 24LC256.
 * Umlaute are converted, NOKOs LCD uses another charset!
 * #=ä, $=ö, %=ü, *=ß. I never used capital letters.
 * 
 * NOTE: Flashing takes about 18 Minutes!
 *       If you are using Windows10, uncommend line 17.
 * 
 * See README in folder write_eeprom..
*/

#include <Wire.h>

#define Disc1 0x50  // 24LC256
//#define Windows10

uint16_t addr=0;
uint8_t  c;


void setup() 
{
  #ifdef Windows10
    Serial.begin(115200);
  #else
    Serial.begin(9600);
  #endif
  Wire.begin();
  delay(100);
}

void loop() 
{
  while(Serial.available()==0);
  c=Serial.read();
  switch(c) // Convert Umlaute
  {
      case 35:c=225;break; // #=ä
      case 36:c=239;break; // $=ö
      case 37:c=245;break; // %=ü
      case 42:c=226;break; // *=ß
  }
  writeDisk(Disc1,addr,c);
  addr++;
}

void writeDisk(uint8_t discnumber, uint16_t address, uint8_t data) 
{
  Wire.beginTransmission(discnumber);
  Wire.write((uint16_t)(address >> 8));   
  Wire.write((uint16_t)(address & 0xFF)); 
  Wire.write(data);
  Wire.endTransmission();
  delay(5); // better 10?
}


