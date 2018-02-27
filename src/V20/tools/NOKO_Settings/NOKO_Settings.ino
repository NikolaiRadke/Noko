/*
 * NOKO settings V2.0 27.02.2018 - Nikolai Radke
 * 
 * This sketch writes the presets into Arduino EEPROM and never or rare used 
 * constants into the AT24C32-EEPROM. 
 * 
 * NOTE: If you are using Windows10, uncommend line 66.
 * 
 * To write these information, see README in folder write_eeprom.
 * 
 * Arduino-EEPROM:
 * The variables are described in the NOKO sketch.
 * offset           0 = 0
 * equalizer        1 = 0
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
 * firmware         20 = 20 -> 2.0 - will stay without offset.
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
 * max_stories      2 = Number of stories
 * Byte 3-19        free
 * Byte 20-39       Owner name
 * Byte 40-79       Owner email
 * Byte 80-4091     Reserverd for... whatever the future tells me.

 */

#include <Wire.h>
#include <EEPROM.h>
#include "Time.h"
#include "DS3231RTC.h"

// Hardware address
#define Disc0 0x57  // AH24C32

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
#define max_stories 40  //

uint8_t  high,low;
uint16_t addr,c;

// Needed to parse __DATE__ but does not touch NOKOs language
const char *monthName[12] = {
  "Jan", "Feb", "Mar", "Apr", "May", "Jun",
  "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

tmElements_t tm;

void setup() 
{
  // Read time from computer
  if (getDate(__DATE__) && getTime(__TIME__)) RTC.write(tm);
  
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
  
  // Write constants to AT24C32
  writeDisc(Disc0,0,birth_day);
  delay(10);
  writeDisc(Disc0,1,birth_month);
  delay(10);
  low = max_stories & 0x00FF;
  high = max_stories & 0xFF00;
  writeDisc(Disc0,2,high);
  delay(10);
  writeDisc(Disc0,3,low);
  delay(10);

  // Write owner name
  for (addr=0;addr<20;addr++)
  {
    writeDisc(Disc0,addr+20,name[addr]);
    delay(10);
  }

  // Write owner email
  for (addr=0;addr<40;addr++)
  {
    writeDisc(Disc0,addr+40,email[addr]);
    delay(10);
  }
}

void loop() // Empty. Everything is done!
{}

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

void writeDisc(uint8_t discnumber, uint16_t address, uint8_t data) // Write to disc
{
  Wire.beginTransmission(discnumber);
  Wire.write((uint16_t)(address >> 8));   
  Wire.write((uint16_t)(address & 0xFF)); 
  Wire.write(data);
  Wire.endTransmission();
  delay(5);
}

