/*
 * NOKO Settings EEPROM and Disk0 V1.0 12.01.2016 - Nikolai Radke
 * 
 * This sketch writes the presets into Arduino EEPROM and the owner name in email
 * into the AT24C32-EEPROM. If there are stories on the SD card an def_stories is
 * set to 1, additional informations can be written to AT24C32, which is limited 
 * to 4kb. 
 * 
 * To write these information, see README in folder Write_EEPROM
 * 
 * Arduino-EEPROM:
 * The variables are described in the NOKO sketch.
 * stumm            1 = 0
 * led_dimm         2 = 6
 * alarm_an         3 = 0
 * alarmmm          4 = 9
 * alarmhh          5 = 15
 * klang            6 = 0
 * klang_ton        7 = 3
 * ultra_distanz    8 = 5
 * eventsteller     9 = 4
 * powersave        10 = 1
 * nachtmodus       11 = 1
 * nachtahh         12 = 22
 * nachtamm         13 = 0
 * nachtbhh         14 = 9
 * nachtbmm         15 = 0
 * sommer           16 = will be calculated
 * freqhigh         17 = 89
 * freqlow          18 = 9
 * klang_mp3        19 = 2
 * firmware         20 = 10 -> 1.0
 * station1h        21 = 101
 * station1l        22 = 3
 * station2h        23 = 89
 * station2l        24 = 5
 * station3h        25 = 104
 * station3l        26 = 0
 * ultra_licht      27 = 1
 * nachtdimm        28 = 0
 * 
 * AT24C32:
 * gt               0  = Birthday day
 * gm               1  = Birhtday month
 * geschichten      2 = Number of stories; Max 95
 * Byte 3-19        free
 * Byte 20-39       Owner name
 * Byte 40-79       Owner email
 * Byte 80-99       Reserverd for... whatever the future tells me
 * Byte 100-1699    Author and title geschichtenx2x20 Byte blocks
 * Byte 1700-1779   Length mm:ss geschichtenx2 Byte
 * Byte 1800-4096   free
 */

#include <Wire.h>
#include <EEPROM.h>
#include <Time.h>
#include <DS3231RTC.h>

#define Disk0 0x57    // AH24C32
#define def_stories 1 // Stories on SD card?
#define zz 1          // Timezone 1 = MEZ


// Personal informations
//                {"                    "}
const char name[]={"Nikolai Radke       "};

//                 {"                                        "}
const char email[]={"kontakt@            nikolairadke.de     "};

// Birthday: gt=day, gm=month
const byte gt=7; // Ich
const byte gm=12;

// Number of Stories
const byte geschichten=40; // Max 95

// Length of stories - mm:ss
const byte laenge[]={53,6, 45,6, 53,23, 48,40, 48,22, 47,9, 52,44, 43,56, 48,28, 49,38, 46,16, 53,20, 53,1,
50,15, 52,46, 52,29, 53,8, 50,15, 52,20, 35,25, 50,27, 51,33, 51,23, 48,4, 25,20, 42,39, 47,4, 52,14, 49,36,
53,51, 50,3, 49,27, 52,47, 49,53, 31,13, 32,14, 50,38, 51,8, 47,45, 65,12};

unsigned int addr,c;

const char *monthName[12] = {
  "Jan", "Feb", "Mar", "Apr", "May", "Jun",
  "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

tmElements_t tm;

void setup() 
{
  bool parse=false;

  // Read time from computer
  if (getDate(__DATE__) && getTime(__TIME__)) 
  {
    parse=true;
    RTC.write(tm);
  }
  
  // Read RTC
  Serial.begin(9600);
  Wire.begin();
  delay(100);

  while (!Serial);
  delay(200);
  if (parse)
  {
    Serial.print(__TIME__);
    Serial.println(__DATE__);
    if (sommer()) Serial.println("Summertime");
    else Serial.println("Wintertime");
  } 
  else Serial.print("ERROR!");
    
  
  // Write EEPROM
  EEPROM.write(1,0);
  EEPROM.write(2,6);
  EEPROM.write(3,0);
  EEPROM.write(4,10);
  EEPROM.write(5,15);
  EEPROM.write(6,0);
  EEPROM.write(7,3);
  EEPROM.write(8,5);
  EEPROM.write(9,4);
  EEPROM.write(10,1);
  EEPROM.write(11,1);
  EEPROM.write(12,22);
  EEPROM.write(13,0);
  EEPROM.write(14,9);
  EEPROM.write(15,0);
  EEPROM.write(16,sommer());
  EEPROM.write(17,99);
  EEPROM.write(18,25);
  EEPROM.write(19,2);
  EEPROM.write(20,10);
  EEPROM.write(21,101);
  EEPROM.write(22,30);
  EEPROM.write(23,89);
  EEPROM.write(24,50);
  EEPROM.write(25,104);
  EEPROM.write(26,0);
  EEPROM.write(27,1);
  EEPROM.write(28,0);
  
  // Write AT24C32
  writeDisk(Disk0,0,gt);
  delay(30);
  writeDisk(Disk0,1,gm);
  delay(30);

  // Write owner name
  for (addr=0;addr<20;addr++)
  {
    writeDisk(Disk0,addr+20,name[addr]);
    delay(50);
  }

  // Write owner email
  for (addr=0;addr<40;addr++)
  {
    writeDisk(Disk0,addr+40,email[addr]);
    delay(50);
  }

  //Write number of stories
  writeDisk(Disk0,2,geschichten);
  delay(30);
  #ifdef def_stories
    for (addr=0;addr<(geschichten*2);addr++)
      writeDisk(Disk0,addr+(geschichten-1)*80+100,laenge[addr]);
    addr=100; // Starting adress
  #endif
}

void loop() 
{
  #ifdef def_stories
    while(Serial.available()==0);
    c=Serial.read();
    switch(c) // Convert Umlaute
    {
      case 35:c=225;break; // #=ä
      case 36:c=239;break; // $=ö
      case 37:c=245;break; // %=ü
      case 42:c=226;break; // *=ß
    }
    writeDisk(Disk0,addr,c);
    // delay(30);
    addr++;
  #else
    while(1)
  #endif
}

//----------------------------------------------------------------------

bool getTime(const char *str) // Read time
{
  int Hour, Min, Sec;

  if (sscanf(str, "%d:%d:%d", &Hour, &Min, &Sec) != 3) return false;
  tm.Hour = Hour;
  tm.Minute = Min;
  tm.Second = Sec;
  return true;
}

bool getDate(const char *str) // Read date
{
  char Month[12];
  int Day, Year;
  uint8_t monthIndex;

  if (sscanf(str, "%s %d %d", Month, &Day, &Year) != 3) return false;
  for (monthIndex = 0; monthIndex < 12; monthIndex++) {
    if (strcmp(Month, monthName[monthIndex]) == 0) break;
  }
  if (monthIndex >= 12) return false;
  tm.Day = Day;
  tm.Month = monthIndex + 1;
  tm.Year = CalendarYrToTm(Year);
  return true;
}

bool sommer() // Check summer time
{
  if ((tm.Month<3) || (tm.Month>10)) return false;
  if ((tm.Month>3) && (tm.Month<10)) return true;
  if (tm.Month==3 && (tm.Hour + 24 * tm.Day)>=(1 + zz + 24*(31 - (5 * tm.Year /4 + 4) % 7)) || tm.Month==10 && (tm.Hour + 24 * tm.Day)<(1 + zz + 24*(31 - (5 * tm.Year /4 + 1) % 7))) 
    return true; 
  else 
    return false;
}

void writeDisk(int disknummer, int adresse, byte data) // Write to disk
{
  Wire.beginTransmission(disknummer);
  Wire.write((int)(adresse >> 8));   
  Wire.write((int)(adresse & 0xFF)); 
  Wire.write(data);
  Wire.endTransmission();
  delay(5);
}

