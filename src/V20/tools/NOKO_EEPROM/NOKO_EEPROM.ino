/*
 * NOKO EEPROM writer V2.0 26.02.2018 - Nikolai Radke
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
uint8_t  low,c;


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
  low=c & 0x00FF;
  if (low!=0xC3)
  {
    writeDisc(Disc1,addr,low);
    addr++;
  }
}

void writeDisc(uint8_t discnumber, uint16_t address, uint8_t data) 
{
  switch (data)                 // Converting German character. Set your own language here
  {
    case 0x84: data=1;   break; // Ä -> Not in LCD charset, custom character
    case 0xA4: data=225; break; // ä
    case 0x96: data=2;   break; // Ö -> As above
    case 0xB6: data=239; break; // ö
    case 0x9C: data=3;   break; // Ü -> The same
    case 0xBC: data=245; break; // ü
    case 0x9F: data=226; break; // ß
  }
  Wire.beginTransmission(discnumber);
  Wire.write((uint16_t)(address >> 8));   
  Wire.write((uint16_t)(address & 0xFF)); 
  Wire.write(data);
  Wire.endTransmission();
  delay(5); // better 10?
}


