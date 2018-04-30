 /* NOKO Diagnostics V0.8 03.02.2017 - Nikolai Radke
  *  
  *  Sketch for testing NOKO functions.
  *  This sketch aims to be easy. Clear source code and no libraries.
  *  The modified Si4703-code was taken from Nathan Seidle.
  *  
  *  Just upload and open a serial monitor with 9600 Baud.
  *  
  *  TODO: A lot.
  */
#include <Wire.h> 
#include <SoftwareSerial.h>

#define Radio 0x10        // Si4703 radio
#define Disk1 0x50        // 24LC256 32 kBye EEPROM
#define Disk0 0x57        // AH24C32 4 kByte EEPROM
#define RTC   0x68        // Real time clock DS3231

#define Freq  997         // Radio station

// Battery calculation
#define min_V         2.85
#define max_V         4.15

char      select;
boolean   radio,eeprom,rtc,rtc_eeprom,lcd;
boolean   amp=false;
uint8_t   help,lcd_address;
uint16_t  help16;
uint16_t  newFreq=Freq;
uint32_t  duration;
uint16_t  si4703_registers[16];      // There are 16 registers, each 16 bits large

SoftwareSerial mp3(2,3);             // TX to D0, RX to D1

void setup()
{
  mp3.begin(9600);
  mp3.write("\x7E\x02\x0C\xEF");     // Reset JQ6500
  delay(500);
  mp3.write("\x7E\x03\x11\x04\xEF"); // No loop
  delay(128);
  pinMode(5,OUTPUT);     // Radio Reset Pin
  pinMode(6,OUTPUT);     // MOSFET
  pinMode(7,OUTPUT);     // AUX
  pinMode(10,OUTPUT);    // LED
  pinMode(11,OUTPUT);    // Beep
  pinMode(12,INPUT);     // Ultrasonic Echo
  pinMode(13,OUTPUT);    // Ultrasonic Trigger
 
  digitalWrite(5,LOW);   // Reset Radio
  delay(1);
  digitalWrite(5,HIGH);
  digitalWrite(6,HIGH);  // Amplifier MOSFET Off
  digitalWrite(A0,HIGH); // Input pullup
  
  Serial.begin(9600);
  Serial.println(F("\nAnalysing NOKO..."));
  Wire.begin();
  for (help=8;help<120;help++)
  {
    Wire.beginTransmission(help);
    if (Wire.endTransmission()==0)
    {
      Serial.print("x");
      Serial.print(help,HEX);
      delay(1);
     if ((help>0x1F) && (help<0x40)) 
      {
        Serial.println(F(" Display found"));
        lcd_address=help;
        lcd=true;
      }
      if (help==Radio) 
      {
        Serial.println(F(" Radio found"));
        radio=true;
      }
      if (help==Disk1) 
      {
        Serial.println(F(" 24LCXXX found"));
        eeprom=true;
      }
      if (help==Disk0) 
      {
        Serial.println(F(" AT24C32 found"));
        rtc_eeprom=true;
      }
      if (help==RTC) 
      {
        Serial.println(F(" RTC found"));
        rtc=true;
      }
    }
  }
  Serial.println();
}

void loop()
{
  Serial.println(F("NOKO Diagnostics V0.8\n\n"));
  Serial.print(F("Display ["));
  Serial.print(lcd? "X":" ");
  Serial.print(F("] Radio ["));
  Serial.print(radio? "X":" ");
  Serial.print(F("] RTC ["));
  Serial.print(rtc? "X":" ");
  Serial.print(F("] AT24C32 ["));
  Serial.print(rtc_eeprom? "X":" ");
  Serial.print(F("] EEPROM ["));
  Serial.println(eeprom? "X]":" ]");
  Serial.print(F("\nAmplifier ["));
  Serial.print(amp? "on ]":"off]");
  Serial.print(F(" Charging ["));
  Serial.print((analogRead(7)>800)? "X]":" ]");
  Serial.print(F(" MP3 playing ["));
  Serial.println((analogRead(1)>0)? "X]\n":" ]\n");
  Serial.println(F("Menue"));
  Serial.println(F("---------------------------------"));
  Serial.println(F("0: Pin D10 - LED"));
  Serial.println(F("1: Pin D11 - Beep"));
  Serial.println(F("2: Pin  D6 - MOSFET Amplifier on/off"));
  Serial.println(F("3: Pin  D7 - AUX"));
  Serial.println(F("4: Pin  A6 - Read battery current"));
  Serial.println(F("5: Scan I2C"));
  Serial.println(F("6: Test Distance sensor"));
  Serial.println(F("7: Show RTC Temperature"));
  Serial.println(F("8: Test 24LC256"));
  Serial.println(F("9: Test AH24C32"));
  Serial.println(F("a: Test Display"));
  Serial.println(F("b: Play MP3 file"));
  Serial.println(F("c: Test Si4703 radio"));
  Serial.println(F("d: Button test"));
  
  Serial.print(F("\nChoose -> "));
  
  while(Serial.available()==0);
  select=Serial.read();
  Serial.println(select);
  Serial.println();
  switch(select)
  {
    case '0':
      Serial.println(F("LED will blink. Send Space to stop."));
      while(Serial.available()==0)
      {
        digitalWrite(10,HIGH);
        delay(500);
        digitalWrite(10,LOW);
        delay(500);     
      } 
     break;
    case '1':
      if (!amp) Serial.println(F("Amplifier is off! (HIGH)"));
      Serial.println(F("Pin 6 will give short tones. Send Space to stop."));
      digitalWrite(6,LOW);
      while(Serial.available()==0)
      {
        tone(11,800,100);
        delay(200);
      } 
      digitalWrite(6,HIGH);
     break;
    case '2':
      digitalWrite(6,amp? HIGH:LOW);
      amp=!amp;
      break;
    case '3':
      Serial.println(F("Setting AUX (D7) High. Send Space to stop."));
      while(Serial.available()==0) digitalWrite(7,HIGH);
      digitalWrite(7,LOW);
      break;
    case '4':
      help16=analogRead(6); // Calculate power level from 5 measurements
      for (help=4;help>0;help--)
      {
        delay(10);
        help16+=analogRead(6);
      }
      help16=(uint16_t)(((help16/5)*(5.0/1024)-min_V)/((max_V-min_V)/100)); 
      help16=constrain(help16,1,99);
      if (help16>57) help16=map(help16,57,99,8,99); 
      else help16=map(help16,1,56,1,8);
      Serial.print(help16);
      Serial.println("%");
      break;
    case '5':
      Serial.print("I2C: ");  // Show all I2C devices
      for (help=8;help<120;help++)
      {
        Wire.beginTransmission(help);
        if (Wire.endTransmission()==0)
        {
          Serial.print("x");
          Serial.print(help,HEX);
          delay(1);
        }
      }
      Serial.println();
      break;
    case '6':
      Serial.println(F("Reading distance 10 times:"));
      for (help=0;help<10;help++)
      {
        digitalWrite(13, LOW);
        delayMicroseconds(2);
        digitalWrite(13, HIGH);
        delayMicroseconds(10);
        digitalWrite(13, LOW);
        duration=pulseIn(12, HIGH);
        Serial.print(duration/29/2);
        Serial.println("cm");
        delay(250);
      }
      Serial.println();
      break;
    case '7':
      Wire.beginTransmission(0x68);
      Wire.write(byte(0x11));
      Wire.endTransmission();
      Wire.requestFrom(0x68,2);
      help=Wire.read();
      Serial.print(help)+0.25*(Wire.read()>>6);
      Serial.println("°C");
      break;
    case '8':
      Serial.println(F("Reading first 100 characters from EEPROM:"));
      for (help=0;help<100;help++)
        Serial.print(char(readDisk(Disk1,help)));
      Serial.println();
      break;
    case '9':
      Serial.println(F("Reading owner name and email address:"));
      for (help=20;help<80;help++)
        Serial.print(char(readDisk(Disk0,help)));
      Serial.println();
      break;
    case 'a':
      Serial.println(F("Display should turn off and on..."));
      Wire.beginTransmission(lcd_address);
      Wire.write(0x00);
      Wire.endTransmission();
      delay(500);
      Wire.beginTransmission(lcd_address);
      Wire.write(0x08);
      Wire.endTransmission();
      break;
    case 'b':
      if (!amp) Serial.println(F("Warning: Amplifier is off!"));
      Serial.println(F("Playing first file..."));
      mp3.write("\x7E\x02\x0D\xEF");     // Play first title
      break;
    case 'c':
      digitalWrite(5, LOW);              // Reset radio
      delay(1);
      digitalWrite(5, HIGH);             // Restart radio
      delay(1);
      si4703_readRegisters();   
      si4703_registers[0x07]=0x8100;          // Enable the oscillator
      si4703_updateRegisters();               // Always write the registers if one is changed
      delay(500);                             // Wait for clock 
      si4703_readRegisters(); 
      si4703_registers[0x02]=0x4001;          // Enable the IC
      si4703_registers[0x04] |= (1<<12);      // Enable RDS
      si4703_registers[0x04] |= (1<<11);      // 50kHz Europe setup
      si4703_registers[0x05] |= (1<<4);       // 100kHz channel spacing for Europe
      si4703_registers[0x05] &= 0xFFF0;       // Clear volume bits
      si4703_registers[0x05] |= 0x000C;       // Set volume 
      si4703_updateRegisters(); //Update
      delay(110);                             // Max powerup time,
      newFreq*=10;
      newFreq-=8750;
      newFreq/=10;
      si4703_readRegisters();
      si4703_registers[0x03] &= 0xFE00;       // Clear out the channel bits
      si4703_registers[0x03] |= newFreq;      // Mask in the new channel
      si4703_registers[0x03] |= (1<<15);      // Set the TUNE bit to start
      si4703_updateRegisters();
      Serial.println(F("Playing Radio. Send space to stop"));
      delay(100);
      while(Serial.available()==0);
      si4703_readRegisters();
      si4703_registers[0x02] |= (1<<6);       // Disable radio
      si4703_updateRegisters();   
      break;
    case 'd':
      help16=(analogRead(0));
      if (help16>200) Serial.print(F("No "));
      else if (help16>150) Serial.print(F("Nose "));
      else if (help16>100) Serial.print(F("Left "));
      else if (help16>50) Serial.print(F("Right "));
      else Serial.print(F("Belly "));
      Serial.print(F("Button: "));
      Serial.println(help16);
      break;
  }
  Serial.read();
  delay(100);
  Serial.println("\nDone.\n");
}

byte readDisk(uint8_t disknummer, int adresse) // Read an EEPROM
{
  uint8_t rdata = 0xFF; 
  Wire.beginTransmission(disknummer);
  Wire.write((int)(adresse >> 8));   
  Wire.write((int)(adresse & 0xFF)); 
  Wire.endTransmission();
  Wire.requestFrom(disknummer,1);
  if (Wire.available()) rdata = Wire.read();
  return rdata;
}

//Read the entire register control set from 0x00 to 0x0F
void si4703_readRegisters()
{
  Wire.requestFrom(Radio,32);      // Read all 32 bytes
  for(byte x=0x0A;;x++) 
  { 
    if(x==0x10) x=0;              // Loop back to zero
    si4703_registers[x]=Wire.read()<<8;
    si4703_registers[x]|=Wire.read();
    if(x == 0x09) break;
  }
}

void si4703_updateRegisters() 
{
  Wire.beginTransmission(Radio);             // Writing begins with 0x02
  for (byte regSpot=0x02 ;regSpot<0x08;regSpot++) 
  {
    byte high_byte=si4703_registers[regSpot] >> 8;
    byte low_byte=si4703_registers[regSpot] & 0x00FF;
    Wire.write(high_byte);                  // Upper 8 bits
    Wire.write(low_byte);                   // Lower 8 bits
  }
  Wire.endTransmission();
}

