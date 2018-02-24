/*
 * NOKO_Time - V1.0 12.01.2018 - Nikolai Radke
 * Tool to set the Computer time on NOKO without touching the EEPROMs
*/ 

#include "Time.h"
#include "DS3231RTC.h"

// Set Timezone
#define zz 1        // Timezone 1 = UTC+1 | CET - Summertime/CEST will be calculated!

// Needed to parse __DATE__ but does not touch NOKOs language
const char *monthName[12] = {
  "Jan", "Feb", "Mar", "Apr", "May", "Jun",
  "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

tmElements_t tm;

void setup() 
{
  if (getDate(__DATE__) && getTime(__TIME__)) RTC.write(tm);
}

void loop() 
{
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

