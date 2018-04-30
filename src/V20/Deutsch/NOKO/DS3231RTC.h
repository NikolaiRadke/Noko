/*
 * DS3231RTC.h - library for DS3231 RTC
 * This library is intended to be uses with Arduino Time.h library functions
 */

#ifndef DS3231RTC_h
#define DS3231RTC_h

#include "Time.h"

/*
status register 0Fh
bit7 OSF      Oscillator Stop Flag (if 1 then oscillator has stopped and date might be innacurate)
bit3 EN32kHz  Enable 32kHz output (1 if needed)
bit2 BSY      Busy with TCXO functions
bit1 A2F      Alarm 2 Flag - (1 if alarm2 was triggered)
bit0 A1F      Alarm 1 Flag - (1 if alarm1 was triggered)

control register 0Eh
bit7 EOSC   Enable Oscillator (1 if oscillator must be stopped when on battery)
bit6 BBSQW  Battery Backed Square Wave
bit5 CONV   Convert temperature (1 forces a conversion NOW)
bit4 RS2    Rate select - frequency of square wave output
bit3 RS1    Rate select
bit2 INTCN  Interrupt control (1 for use of the alarms and to disable square wave)
bit1 A2IE   Alarm2 interrupt enable (1 to enable)
bit0 A1IE   Alarm1 interrupt enable (1 to enable)
*/

// registers
#define DS3231_CONTROL_ADDR         0x0E
#define DS3231_STATUS_ADDR          0x0F

// library interface description
class DS3231RTC
{
  // user-accessible "public" interface
  public:
    DS3231RTC();
    static time_t get();
	static void set(time_t t);
	static void read(tmElements_t &tm);
	static void write(tmElements_t &tm);
	static float getTemp();

  private:
	static uint8_t dec2bcd(uint8_t num);
    static uint8_t bcd2dec(uint8_t num);

  protected:
	static void set_sreg(uint8_t val);
	static uint8_t get_sreg();
	
	static void set_creg(uint8_t val);
	static uint8_t get_creg();
};

extern DS3231RTC RTC;

#endif
 






