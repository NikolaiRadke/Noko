/*
 * NOKO settings V2.1 19.03.2018 - Nikolai Radke
 * 
 * This sketch writes the presets into Arduino EEPROM and never or rare used 
 * constants into the AT24C32-EEPROM. 
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
 * firmware         20 = 20 -> 2.0 - will stay without offset
 * radio_station_1h 21 = 101
 * radio_station_1l 22 = 3
 * radio_station_2h 23 = 89
 * radio_station_2l 24 = 5
 * radio_station_3h 25 = 104
 * radio_station_3l 26 = 0
 * distance_ligh    27 = 1
 * night_lcd_dimm   28 = 0
 * alarm_days       29 = 82 - Mon, Tue, Wed, Thu, Fri
 * 
 * AT24C32:
 * birth_day        0  = Birthday day
 * birth_month      1  = Birhtday month
 * max_stories_h    2 = Number of stories, high byte
 * max_stories_l    3 = low byte
 * Byte 3-19        free
 * Byte 20-39       Owner name
 * Byte 40-79       Owner email
 * Byte 80-4091     Reserverd for... whatever the future tells me

 */

#include <Wire.h>
#include <EEPROM.h>
#include "Time.h"
#include "DS3231RTC.h"

// Hardware address
#define Disc0 0x57  // AH24C32

// Set Timezone
#define tz 1        // Timezone 1 = UTC+1 | CET - Summertime/CEST will be calculated!

// Personal informations
//              "                    "
#define myname  "Nikolai Radke       "
   
//              "                                        "
#define myemail "kontakt@            nikolairadke.de     "

// Birthday: gt=day, gm=month
#define birth_day   7   // 07. December 
#define birth_month 12  // That's my birthday. Keep it in your mind... :-)

// Number of Stories
#define max_stories 40  //

uint8_t  high,low,help;
uint16_t addr;

// Needed to parse __DATE__ but does not touch NOKOs language.
const char *monthName[12] = {
  "Jan", "Feb", "Mar", "Apr", "May", "Jun",
  "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

tmElements_t tm;

void setup() 
{
  Serial.begin(9600);
  // Read time from computer
  if (getDate(__DATE__) && getTime(__TIME__)) RTC.write(tm);

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
  EEPROM.update(20,20);
  EEPROM.update(21,101);
  EEPROM.update(22,30);
  EEPROM.update(23,89);
  EEPROM.update(24,50);
  EEPROM.update(25,104);
  EEPROM.update(26,0);
  EEPROM.update(27,1);
  EEPROM.update(28,0);
  EEPROM.update(29,62);
  
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
  addr=20;
  for (help=0;help<strlen(myname);help++)
  {
    low=myname[help] & 0x00FF;
    if (low!=0xC3)
    {
      writeDisc(Disc0,addr,low);
      addr++;
    }
  }

  // Write owner email
  for (help=0;help<strlen(myemail);help++)
  {
    low=myemail[help] & 0x00FF;
    if (low!=0xC3)
    {
      writeDisc(Disc0,addr,low);
      addr++;
    }
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
  if (tm.Month==3 && (tm.Hour + 24 * tm.Day)>=(1 + tz + 24*(31 - (5 * tm.Year /4 + 4) % 7)) || tm.Month==10 && (tm.Hour + 24 * tm.Day)<(1 + tz + 24*(31 - (5 * tm.Year /4 + 1) % 7))) 
    return true; 
  else 
    return false;
}

void writeDisc(uint8_t discnumber, uint16_t address, uint8_t data) // Write to disc
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
  delay(5);
}


