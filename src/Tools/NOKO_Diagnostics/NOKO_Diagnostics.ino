 /* NOKO Diagnostics V0.2 31.05.2017 - Nikolai Radke
  *  
  *  Sketch for testing NOKO functions.
  *  This sketch aims to be easy. Clear source code and no libraries.
  *  
  *  Just upload and open a serial monitor with 9600 Baud.
  *  
  *  TODO: A lot.
  */
#include <Wire.h> 
#include <SoftwareSerial.h>

#define Disk0 0x57        // AH24C32 4 kByte EEPROM
#define Disk1 0x50        // 24LC256 32 kBye EEPROM

char      select;
boolean   radio,eeprom,rtc,rtc_eeprom,lcd,amp=false;
uint8_t   lcd_address,help;
uint32_t  duration;

SoftwareSerial mp3(2,3);           // TX to D0, RX to D1

void setup()
{
  mp3.begin(9600);
  mp3.write("\x7E\x02\x0C\xEF");     // Reset JQ6500
  delay(500);
  mp3.write("\x7E\x03\x11\x04\xEF"); // No loop
  delay(128);
  pinMode(5,OUTPUT);  // Radio Reset Pin
  pinMode(6,OUTPUT);  // MOSFET
  pinMode(7,OUTPUT);  // AUX
  pinMode(10,OUTPUT); // LED
  pinMode(11,OUTPUT); // Beep
  pinMode(12,INPUT);  // Ultrasonic Echo
  pinMode(13,OUTPUT); // Ultrasonic Trigger

  digitalWrite(5,LOW); // Reset Radio
  delay(1);
  digitalWrite(5, HIGH);
  digitalWrite(6,HIGH); // Amplifier MOSFET Off
  
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
      if (help==0x10) 
      {
        Serial.println(F(" Radio found"));
        radio=true;
      }
      if (help==0x50) 
      {
        Serial.println(F(" 24LCXXX found"));
        eeprom=true;
      }
      if (help==0x57) 
      {
        Serial.println(F(" AT24C32 found"));
        rtc_eeprom=true;
      }
      if (help==0x68) 
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
  Serial.println(F("NOKO Diagnostics"));
  Serial.print(F("Display ["));
  Serial.print(lcd? "X":" ");
  Serial.print(F("] Radio ["));
  Serial.print(radio? "X":" ");
  Serial.print(F("] RTC ["));
  Serial.print(rtc? "X":" ");
  Serial.print(F("] AH24C32 ["));
  Serial.print(rtc_eeprom? "X":" ");
  Serial.print(F("] EEPROM ["));
  Serial.println(eeprom? "X]":" ]");
  Serial.print(F("Amplifier ["));
  Serial.println(amp? "on ]\n":"off]\n");
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
  
  Serial.print(F("\nChoose -> "));
  
  while(Serial.available() == 0);
  select=Serial.read();
  Serial.println(select);
  switch(select)
  {
    case '0':
      Serial.println(F("LED will blink. Send Space to stop."));
      while(Serial.available() == 0)
      {
        digitalWrite(10,HIGH);
        delay(500);
        digitalWrite(10,LOW);
        delay(500);     
      } 
     break;
    case '1':
      if (!amp) Serial.println(F("Amplifier is off!"));
      Serial.println(F("Pin 6 will give short tones. Send Space to stop."));
      digitalWrite(6,LOW);
      while(Serial.available() == 0)
      {
        tone(11,800,100);
        delay(200);
      } 
      digitalWrite(6,HIGH);
     break;
    case '2':
      digitalWrite(6,amp? LOW:HIGH);
      amp=!amp;
      break;
    case '3':
      while(Serial.available() == 0) digitalWrite(7,HIGH);
      digitalWrite(7,LOW);
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
      while(Serial.available() == 0)
      {
        digitalWrite(13, LOW);
        delayMicroseconds(2);
        digitalWrite(13, HIGH);
        delayMicroseconds(10);
        digitalWrite(13, LOW);
        duration=pulseIn(12, HIGH);
        Serial.print(duration/29/2);
        Serial.println("cm");
        delay(100);
      }
      Serial.println();
      break;
    case '7':
      Wire.beginTransmission(0x68);
      Wire.write(byte(0x11));
      Wire.endTransmission();
      Wire.requestFrom(0x68,2);
      help = Wire.read();
      Serial.print(help)+0.25*(Wire.read()>>6);
      Serial.println("°C");
      break;
    case '8':
      for (help=0;help<100;help++)
        Serial.print(char(readDisk(Disk1,help)));
      Serial.println();
      break;
    case '9':
      for (help=20;help<80;help++)
        Serial.print(char(readDisk(Disk0,help)));
      Serial.println();
      break;
    case 'a':
      Serial.println("Will follow...");
      break;
    case 'b':
      mp3.write("\x7E\x04\x03\x01");     // Play file number 
      mp3.write(70);                     // 70
      mp3.write("\xEF");
      delay(1000);
      mp3.write("\x7E\x02\x0D\xEF");     // Play
      break;
  }
  delay(1000);
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

