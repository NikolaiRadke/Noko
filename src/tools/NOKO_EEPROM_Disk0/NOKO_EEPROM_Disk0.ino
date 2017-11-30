/*
 * NOKO settings EEPROM and Disk0 V1.2 30.11.2017 - Nikolai Radke
 * 
 * This sketch writes the presets into Arduino EEPROM and the owner name in email
 * into the AT24C32-EEPROM. If there are stories on the SD card an def_stories is
 * set, additional informations can be written to AT24C32, which is limited 
 * to 4kb. 
 * 
 * NOTE: If you are using Windows10, uncommend line 66.
 * 
 * To write these information, see README in folder write_eeprom.
 * 
 * Arduino-EEPROM:
 * The variables are described in the NOKO sketch.
 * offset           0 = 0
 * quiet            1 = 0
 * led_val          2 = 6
 * alarm_on         3 = 0
 * alarm_mm         4 = 9
 * alarm_hh         5 = 15
 * alarm_type       6 = 0
 * alarm_tone       7 = 3
 * distance_val     8 = 5
 * event_val        9 = 4
 * powersave        10 = 1
 * nightmode_on     11 = 1
 * night_hh         12 = 22
 * night_mm         13 = 0
 * night_to_hh      14 = 9
 * night_to_mm      15 = 0
 * summertime       16 = will be calculated
 * radio_freq_high  17 = 89
 * radio_freq_low   18 = 9
 * alarm_mp3        19 = 2
 * firmware         20 = 10 -> 1.0 - will stay without offset.
 * radio_station_1h 21 = 101
 * radio_station_1l 22 = 3
 * radio_station_2h 23 = 89
 * radio_station_2l 24 = 5
 * radio_station_3h 25 = 104
 * radio_station_3l 26 = 0
 * distance_ligh    27 = 1
 * night_lcd_dimm   28 = 0
 * 
 * AT24C32:
 * birth_day        0  = Birthday day
 * birth_month      1  = Birhtday month
 * max_stories      2 = Number of stories; Max 95
 * Byte 3-19        free
 * Byte 20-39       Owner name
 * Byte 40-79       Owner email
 * Byte 80-99       Reserverd for... whatever the future tells me
 * Byte 100-290     Length of story mm:ss
 * Byte 291-4091    Author and title of the stories 2 x 20 Byte blocks
 */

#include <Wire.h>
#include <EEPROM.h>
#include "Time.h"
#include "DS3231RTC.h"

// Hardware address
#define Disk0 0x57  // AH24C32

// Story feature. Comment out to disable
#define def_stories // Stories on SD card?

// Set Timezone
#define zz 1        // Timezone 1 = UTC+1 | CET - Summertime/CEST will be calculated!

// Set your OS. Only Windows 10 needs this option
//#define Windows10

// Personal informations
//            "                    "
#define name  "Nikolai Radke       "

//            "                                        "
#define email "kontakt@            nikolairadke.de     "

// Birthday: gt=day, gm=month
#define birth_day   7   // 07. December 
#define birth_month 12  // That's my birthday. Keep it in your mind... :-)

// Number of Stories
#define max_stories 40 // Max 95

// Length of stories - mm:ss
const uint8_t stories_length[]={53,6, 45,6, 53,23, 48,40, 48,22, 47,9, 52,44, 43,56, 48,28, 49,38, 46,16, 53,20, 53,1,
50,15, 52,46, 52,29, 53,8, 50,15, 52,20, 35,25, 50,27, 51,33, 51,23, 48,4, 25,20, 42,39, 47,4, 52,14, 49,36,
53,51, 50,3, 49,27, 52,47, 49,53, 31,13, 32,14, 50,38, 51,8, 47,45, 65,1};

uint16_t addr,c;

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
  #ifdef Windows10
    Serial.begin(115200);
  #else
    Serial.begin(9600);
  #endif
  Wire.begin();
  delay(100);
     
  // Write EEPROM
  EEPROM.update(0,0);
  EEPROM.update(1,0);
  EEPROM.update(2,6);
  EEPROM.update(3,0);
  EEPROM.update(4,10);
  EEPROM.update(5,15);
  EEPROM.update(6,0);
  EEPROM.update(7,3);
  EEPROM.update(8,5);
  EEPROM.update(9,4);
  EEPROM.update(10,1);
  EEPROM.update(11,1);
  EEPROM.update(12,22);
  EEPROM.update(13,0);
  EEPROM.update(14,9);
  EEPROM.update(15,0);
  EEPROM.update(16,check_summertime());
  EEPROM.update(17,99);
  EEPROM.update(18,25);
  EEPROM.update(19,2);
  EEPROM.update(20,10);
  EEPROM.update(21,101);
  EEPROM.update(22,30);
  EEPROM.update(23,89);
  EEPROM.update(24,50);
  EEPROM.update(25,104);
  EEPROM.update(26,0);
  EEPROM.update(27,1);
  EEPROM.update(28,0);
  
  // Write AT24C32
  writeDisk(Disk0,0,birth_day);
  delay(10);
  writeDisk(Disk0,1,birth_month);
  delay(10);

  // Write owner name
  for (addr=0;addr<20;addr++)
  {
    writeDisk(Disk0,addr+20,name[addr]);
    delay(10);
  }

  // Write owner email
  for (addr=0;addr<40;addr++)
  {
    writeDisk(Disk0,addr+40,email[addr]);
    delay(10);
  }

  // Write number of stories
  writeDisk(Disk0,2,max_stories);
  delay(10);

  // Write length of stories
  #ifdef def_stories
    for (addr=0;addr<(max_stories*2);addr++)
      writeDisk(Disk0,addr+100,stories_length[addr]);
      delay(10);
  #endif

   // Starting adress of stories
  addr=291;
}

void loop() 
{
  #ifdef def_stories
    while(Serial.available()==0);
    c=Serial.read();
    switch(c) // Convert german Umlaute
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
    while(1);
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
  int16_t Day, Year;
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

bool check_summertime() // Check summertime
{
  if ((tm.Month<3) || (tm.Month>10)) return false;
  if ((tm.Month>3) && (tm.Month<10)) return true;
  if (tm.Month==3 && (tm.Hour + 24 * tm.Day)>=(1 + zz + 24*(31 - (5 * tm.Year /4 + 4) % 7)) || tm.Month==10 && (tm.Hour + 24 * tm.Day)<(1 + zz + 24*(31 - (5 * tm.Year /4 + 1) % 7))) 
    return true; 
  else 
    return false;
}

void writeDisk(uint8_t disknumber, uint16_t adresse, uint8_t data) // Write to disk
{
  Wire.beginTransmission(disknumber);
  Wire.write((uint16_t)(adresse >> 8));   
  Wire.write((uint16_t)(adresse & 0xFF)); 
  Wire.write(data);
  Wire.endTransmission();
  delay(5);
}


