/*
 * NOKO Settings Disk1 V1.0 12.01.2016 - Nikolai Radke
 * 
 * This sketch writes the event poems, phrases and stuff into 24LC256.
 * Umlaute are converted, NOKOs LCD uses another charset!
 * #=ä, $=ö, %=ü, *=ß. I never used capital letters.
 * 
 * NOTE: Flashing takes more than 15 Minutes!
 * 
 * See README in folder TTY.
*/

#include <Wire.h>

#define Disk1 0x50  // 24LC512

unsigned int addr=0;
byte c;

void setup() 
{
  Serial.begin(9600);
  Serial.flush();
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
  writeDisk(Disk1,addr,c);
  addr++;
}

void writeDisk(int disknummer, unsigned int adresse, byte data) 
{
  Wire.beginTransmission(disknummer);
  Wire.write((int)(adresse >> 8));   
  Wire.write((int)(adresse & 0xFF)); 
  Wire.write(data);
  Wire.endTransmission();
  delay(5); // better 10?
}


