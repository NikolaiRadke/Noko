 /* NOKO V1.0 30.04.2018 - Nikolai Radke
 *
 * Sketch for NOKO-Monster - Deutsch
 * NOTE: Does NOT run without the Si4703 Radio Module! Uncommend line 88 if it's not present.
 * The main loop controls the timing events and gets interrupted by the read_button()-funtion.
 * Otherwise NOKO falls asleep with powerdown_delay() for 120ms. This saves a lot of power.
 * 
 * Flash-Usage: 27.832 (1.8.2 | AVR Core 1.6.19 | Linux x86_64, Windows 10 | Compiler options)
 * 
 * Optional:
 * Compiler Options:   -funsafe-math-optimizations -mcall-prologues -maccumulate-args
 *                     -ffunction-sections  -fdata-sections -fmerge-constants
 * These options save flash but are not needed since IDE 1.6.10. Optiboot is still recommended.
 * See https://github.com/NikolaiRadke/NOKO/blob/master/howto_compile/README.md
 * 
 * char()-list: 32=space 37=% 46=. 47=/ 48=0 58=: 68=D 78=N 80=P 82=R 83=S 86=V 87=W 88=X
 *              110=n 120=x | 225=ä 226=ß 239=ö 245=ü (German only)
 *         
 * TODO:
 *         
 * KNOWN BUGS:
 * Due to bad programming the summertime/wintertime will switch at 04:00, not 02:00.
 * 
 * Circuit:
 * 5V               5V    Regulated   
 * LCD GND          GND
 * LCD VCC          5V
 * LCD SDA          A4    (SDA)
 * LCD SCL          A5    (SCL)
 * RTC GND          GND
 * RTC VCC          5V
 * RTC SDA          A4    (SDA)
 * RTC SCL          A5    (SCL)
 * Radio GND        GND
 * Radio VCC        3,3V  (!!!) 
 * Radio SDA        A4    (SDA)
 * Radio SCL        A5    (SCL)
 * Radio RST        D5  
 * JQ6500 GND   13  GND
 * JQ6500 TX    26  D2    (Software-TX)
 * JQ6500 RX    25  D3    (Software-RX), connect with 1kOhm resistor
 * JQ6500 Busy  23  D4/A1 D4: With 4,7kOhm connected to 5V to get a clear HIGH
 * JQ6500 LOUT  27  L Amp
 * JQ6500 ROUT  28  R Amp
 * ULTRA GND        GND
 * ULTRA VCC        5V
 * ULTRA PingPin    D13   (Trigger)
 * ULTRA inPin      D12   (Echo)
 * 24LC256      1   GND   Pin 1-3 must be connected to GND for the correct I2C-address
 * 24LC256      2   GND
 * 24LC256      3   GND
 * 24LC256      4   GND
 * 24LC256      5   A4    (SDA)
 * 24LC256      6   A5    (SCL)
 * 24LC256      7   GND   Pin 7 set to HIGH would enable write protection
 * 24LC256      8   5V
 * Amplifier GND    GND
 * Amplifier VCC    6,5V
 * Amplifier MOSFET D6    (LOW=ON, HIGH=OFF)
 * AUX Relay        D7    
 * SW1 Belly        A0    (Menue) connected via ADC with 3 x 2,2kOhm
 * SW2 Right        A0    (Mute ULTRA | next menue)
 * SW3 Left         A0    (Mute LCD | prev menue)
 * SW4 Nose         A0    (Quit)
 * LED VCC          D10   (PWM)
 * LED GND          GND
 * Speaker VCC      D11   (PWM)
 * Speaker GND      GND
 * Battery VCC      A6    
 * USB VCC          A7    With 1kOhm
 * 
 * Unused           A1.A2,A3,D8,D9,(ICSP), A0 or D4
 * 
 * I2C address list:
 * SI4703           0x10 (Radio)
 * LCD              Will be scanned by default
 * 24LC256          0x50 (EEPROM 32kb)
 * AT24C32          0x57 (EEPROM 4kb on RTC module)
 * DS3231           0x68 (RTC chip on RTC module)
*/

// Softwareversion
#define Firmware "-300418"
#define Version 10  // 1.0
#define Build_by "by Nikolai Radke" // Your Name. Max. 20 chars, appears in "Mein NOKO" menu

// Features on/off. Comment out to disable
#define def_radio             // Using Radio?
#define def_external_eeprom   // Using external EEPROM?
#define def_stories           // Stories on SD card?

// Display selection
#define display_address 0x00    // Autodetect
//#define display_address 0x27  // Blue (PCF8574)
//#define display_address 0x3F  // Yellow (PCF8574A)

// 24LC256 EEPROM addresses
#define phrase_address 4000    // Starting address of the phrases in 24LC256
#define quote_address 10000    // Quotations
#define poem_address  18000    // Poems

// Timezone GMT+1=1
#define TZ 1 

// Developer options
#define pwd_delay     50  // Button debounce
#define reaction_time 70  // Startup time for the amplifier
#define sensor        25  // Ultrasonic: with cover 10, without 25
#define vol_mp3       30  // JQ6500 volume 0-30
#define vol_radio     10  // Si4703 volume 0-15
#define def_sysinfo       // Sysinfo menu. Comment out for additional 640 bytes
#define busy_analog     // Set if reading the busy signal analog via A1

// Choose your voice set              
//#define voice_set_111   // Old set with 111 files
#define voice_set_226     // New set with 226 files
//#define own_set         // Define your own set below

#ifdef voice_set_111
  #define voice_nose_start    31  // Starts with 31.mp3
  #define voice_sensor_start  61
  #define voice_time_start    81
  #define voice_birthday      111
#endif
#ifdef voice_set_226
  #define voice_nose_start    51
  #define voice_sensor_start  101
  #define voice_time_start    151
  #define voice_birthday      226
#endif
#ifdef own_set
  #define voice_nose_start    0    
  #define voice_sensor_start  0
  #define voice_birthday      0  
  #define voice_birhday       0
#endif  

// Battery calculation
#define min_V         2.85
#define max_V         4.15

// Hardware addresses PINS
#define LED 10            // LED -> PWM
#define Distance_inPin 12 // Ultrasonic
#define Battery 6         // Battery power
#define USB 7             // Is NOKO connected to USB?
#define Speaker 11        // PWM beep 
#define Buttons A0        // ADC buttons
#define Disc0 0x57        // AH24C32 4 kByte EEPROM
#define Disc1 0x50        // 24LC256 32 kByte EEPROM

// Pin operations shortcuts
#define turnOn_pingPin  PORTB |= (1<<5)  // pingPin HIGH
#define turnOff_pingPin PORTB &= ~(1<<5) // pingPin LOW 
#define turnOn_amp      PORTD &= ~(1<<6) // Amplifier LOW=on
#define turnOff_amp     PORTD |= (1<<6)  // Amplifier HIGH=off
#define turnOn_aux      PORTD |= (1<<7)  // AUX HIGH=on
#define turnOff_aux     PORTD &= ~(1<<7) // AUX LOW=off

#ifdef busy_analog                       // Busy=HIGH?
  #define mp3_busy     (analogRead(1)>1) // Analog reading on A1
#else
  #define mp3_busy     PIND & (1<<4)     // Digital reading on D4
#endif

// Libraries
#include <avr/power.h>
#include <avr/wdt.h>
#include <avr/sleep.h>
#include <Wire.h> 
#include <EEPROM.h>
#include "Time.h"
#include "LiquidCrystal_I2C.h"
#include "Si4703.h"
#include "DS3231RTC.h"
#include "TimerFreeTone.h"
#include "JQ6500_Serial.h"

SIGNAL(WDT_vect) // Watchdog to wake NOKO from powerdown_delay()
{
  wdt_disable();
  wdt_reset();
  WDTCSR &= ~_BV(WDIE);
}

uint8_t  selected=0;             // Which menu item selected
uint8_t  xlcd=0;                 // X and
uint8_t  ylcd=0;                 // Y position of LCD cursor
uint8_t  led_val;                // LED brightness 0-9
uint8_t  birth_day,birth_month;  // Birthday gt=day, gm=month
uint8_t  alarm_type;             // Whicht type of alarm
uint8_t  alarm_tone,alarm_mp3;   // Which tone and mp3
uint8_t  alarm_mm,alarm_hh;      // Alarm time hh:mm
uint8_t  alarm_days;             // 8 bit boolean for the weekdays
uint8_t  night_mm,night_hh;      // Nightmode from hh:mm
uint8_t  night_to_mm,night_to_hh;// To hh:mm
uint8_t  distance_val;           // Ultrasonic reaction distance 0-9 
uint8_t  event_val;              // Chance of time event 0-9
uint8_t  equalizer;              // Equalizer mode of MP3 module

boolean  distance_now=false;     // Has there been am ultrasonic event this minute?
boolean  distance_light;         // Swich on LCD light with ultrasonic?
boolean  distance_off=false;     // Ultrasonic off?
boolean  radio_on=false;         // Radio on?
boolean  mp3_pause=false;        // MP3 pause?
boolean  mp3_on=false;           // Is the MP3-Module itself running anything?
boolean  story_on=false;         // Is there a story running?
boolean  file_on=false;          // Is there an own mp3 running?
boolean  birthday=false;         // Is it your birthday today?
boolean  birthday_over=false;    // Was it already celebrated?
boolean  alarm_on;               // Is the Alarm set?
boolean  alarm_mute=false;       // No alarm here?
boolean  alarm_now=false;        // Is the alarm running?
boolean  powersave;              // Powersavemode?
boolean  lcd_dimm=false ;        // Only dim the display?
boolean  lcd_off=false;          // Is powersave active?
boolean  aux_on=false;           // Is AUX active?
boolean  summertime;             // Summertime?
boolean  night_now;              // Is it nighttime? (Time of Nightmode)
boolean  night_over;             // Has the nightmode switched?
boolean  nightmode_on;           // Is nightmode set?
boolean  night_lcd_dimm;         // Will the display mute at nighttime?
boolean  charging;               // Is NOKO's battery charging?

uint16_t file=1;                 // Selected MP3
uint16_t story=1;                // Selected story
uint16_t max_stories;            // Number of stories
uint16_t max_files;              // Number of own MP3s files on SD card
uint16_t offset;                 // EEPROM start position to expand life cyle
uint16_t radio_freq;             // Used frequency * 100
uint16_t radio_station[3];       // 3 radiostationsry

int16_t  thisyear;               // Year is integer type
int16_t  power;                  // Voltage * 100
uint32_t lcd_dimm_time;          // Milliseconds until display mutes
  
const uint8_t event_trigger[10]={0,120,60,45,30,15,10,5,3,1};

// Custom characters. Number set was made by Ishan Karve. Awesome!
// Putting this array into PROGMEM caused strange output. Bizarre.
uint8_t custom_char[19][8]=
{
  {B00111,B01111,B11111,B11111,B11111,B11111,B11111,B11111},
  {B11111,B11111,B11111,B00000,B00000,B00000,B00000,B00000},
  {B11100,B11110,B11111,B11111,B11111,B11111,B11111,B11111},
  {B11111,B11111,B11111,B11111,B11111,B11111,B01111,B00111},
  {B00000,B00000,B00000,B00000,B00000,B11111,B11111,B11111},
  {B11111,B11111,B11111,B11111,B11111,B11111,B11110,B11100},
  {B11111,B11111,B11111,B00000,B00000,B00000,B11111,B11111},
  {B11111,B11111,B11111,B11111,B11111,B11111,B11111,B11111},
  {B01000,B01100,B01110,B01111,B01110,B01100,B01000,B00000}, // Play
  {B00000,B00000,B01110,B01110,B01110,B01110,B00000,B00000}, // Stop
  {B00000,B01010,B01010,B01010,B01010,B01010,B00000,B00000}, // Pause 
  {B00000,B00101,B01010,B10100,B01010,B00101,B00000,B00000}, // Prev
  {B00000,B10100,B01010,B00101,B01010,B10100,B00000,B00000}, // Next
  {B00000,B01010,B01110,B10101,B11111,B01110,B01010,B00000}, // Monster
  {B00000,B00100,B00100,B11111,B01110,B01010,B10001,B00000}, // Star
  {B00001,B00011,B00101,B01001,B01001,B01011,B11011,B11000}, // Note
  {B00000,B01110,B10101,B10111,B10001,B01110,B00000,B00000}, // Clock
  {B00100,B01110,B01110,B01110,B11111,B00000,B00100,B00000}, // Bell
  {B00011,B00110,B01100,B11100,B01100,B00110,B00011,B00000}  // Moon
};

tmElements_t tm;

#ifdef def_radio
  Si4703 Radio(5,A4,A5);  // RST, SDA, SCL
#endif
LiquidCrystal_I2C lcd(display_address,20,4);
JQ6500_Serial mp3(2,3); // Software serial connection

//-------------------------------------------------------------------------
// Using main() and init() instead of setup() and loop() saves flash space

int main(void) {  
uint8_t minute_now=99;
uint8_t hour_now=99;
uint8_t help;
init();
{  
  // Powersave options
  SPCR=0;                // Disable SPI
  ACSR=B10000000;        // Disable analog comparator
  DIDR0=0x3F;            // Disable digital input buffer on all analog ports
  power_spi_disable();       
  power_usart0_disable(); 
  power_timer2_disable();
  
  // Portdefinitions. Direct manipulation is much faster and saves flash
  DDRD=B11111000;   // D7-D0 | 1=OUTPUT
  DDRB=B00101100;   // D13-D8
  DDRC=B00001100;   // A7-A0 | Set unused analog pins to output to prevent catching noise from open ports
  PORTD=B01000000;  // D6 MOSFET HIGH: Turn off amplifier to prevent startup noise
  //PORTB=B00000000; 
  PORTC=B00000001;  // A0: INPUT_PULLUP 
 
  // Read Arduino EEPROM
  offset=EEPROM.read(0)*30;
  equalizer=read_EEPROM(1);
  led_val=read_EEPROM(2);
  alarm_on=read_EEPROM(3);
  alarm_mm=read_EEPROM(4);
  alarm_hh=read_EEPROM(5);
  alarm_type=read_EEPROM(6);
  alarm_tone=read_EEPROM(7);
  distance_val=read_EEPROM(8);
  event_val=read_EEPROM(9);
  powersave=read_EEPROM(10);
  nightmode_on=read_EEPROM(11);
  night_hh=read_EEPROM(12);
  night_mm=read_EEPROM(13);
  night_to_hh=read_EEPROM(14);
  night_to_mm=read_EEPROM(15);
  summertime=read_EEPROM(16);
  radio_freq=(read_EEPROM(17)*10)+(read_EEPROM(18));
  alarm_mp3=read_EEPROM(19);
  for (help=0;help<3;help++)
    radio_station[help]=(read_EEPROM(21+(help*2))*10)+(read_EEPROM(22+(help*2)));
  distance_light=read_EEPROM(27);
  night_lcd_dimm=read_EEPROM(28);
  alarm_days=read_EEPROM(29);

  //  Read AT24C32 
  birth_day=read_disc(Disc0,0);          // Birthday day
  birth_month=read_disc(Disc0,1);        // Birthday month
  #ifdef def_stories
    max_stories=read_disc(Disc0,2);      // Number of stories
  #else
    max_stories=0;
  #endif

   // Start JQ6500
  mp3.begin(9600);
  mp3.pause();
  mp3.reset();
  NewDelay(500);
  mp3.setVolume(vol_mp3);
  mp3.setLoopMode(MP3_LOOP_NONE); // Play only once
  mp3.setEqualizer(equalizer);
  
  // Start Radio
  #ifdef def_radio
    Radio.powerOn();  // Needs to start once!
    Radio.setVolume(vol_radio);
    Radio.powerOff();
  #endif
  
  // Start RTC and switch off useless functions for power saving
  Wire.beginTransmission(0x68); 
  Wire.write("\x0F\xF7");       // 32kHz off         
  Wire.endTransmission();
  powerdown_delay(20);
  Wire.beginTransmission(0x68);
  Wire.write("\x0E\x7F");       // SQW off
  Wire.endTransmission();
  read_RTC();                   // Read RTC
  
  // Start LCD and create custom characters 
  lcd.begin(20,4);    
  init_char();

  //  Check for update and compare with internal version number
  help=EEPROM.read(20);
  if (Version>help) // New version > internal version?
  {
    lcd.clear();
    lcd.print(F(" Update erfolgreich"));
    put_char(3,1,86);
    lcd.print(help/10);
    print_dot();
    lcd.print(help%10);
    lcd.print(F(" auf V"));
    lcd.print(Version/10);
    print_dot();
    lcd.print(Version%10); // lcd.print does not support printf-formatting
    lcd.setCursor(4,3);
    lcd.print(F("Nase dr cken"));
    put_char(11,3,245);
    EEPROM.write(20,Version); // Write new internal version
    wait_1m(true,false);      // Wait for button
    lcd.clear();
  }

  // Starting sound
  sound_on();
  play_tone(600,80,false,80);
  play_tone(880,150,false,150);
  //turnOff_amp;
  
  read_time(); // Read RTC
  night_over=check_night(); // Has nightmode switched before startup?
  
  max_files=mp3.countFiles(MP3_SRC_SDCARD)-(voice_birthday+max_stories); // Number own of files on SD card
  
}
// MAIN LOOP
while(1)
{
  read_time();
  if (((analogRead(USB)>800) && (!charging)) || ((analogRead(USB)<800) && (charging))) // USB connected?
  {
    charging=!charging;
    if ((analogRead(USB)<800) && (lcd_dimm) && (powersave)) powerdown();
    else if ((analogRead(USB)>800) && (lcd_off)) powerup();
    else draw_time();
  } 

// Every minute                                             
  if (minute_now!=tm.Minute)                        
  { 
    if ((night_now) && (night_lcd_dimm) && (!lcd_off))   // Power down at nightmode
    {
      if ((powersave) && (!charging)) powerdown();
      else if (!lcd_dimm)
      {
        lcd.noBacklight();
        lcd_dimm=true;
      }
    }
    distance_now=false;

// Every hour
    if (hour_now!=tm.Hour)                          
    {
      if (!lcd_off) 
      {    
        draw_date();                          // Draw date
        if ((birth_month==tm.Month) && (birth_day==tm.Day))   // Check for birthday
        {
          birthday=true;
          if ((!birthday_over) && (!night_now)) party();
        }
        else 
        {
          birthday_over=false;
          birthday=false;
        }                   
      }
      check_summertime();                     // Check for summertime
    }
    if (!lcd_off) draw_time();                // Draw clock
    if (!night_now)
    {
      if ((power<10) && (!charging))          // Read battery power
      {
        sound_on();
        play_tone(920,80,false,80);           // Power <10% beep
      }
      if ((!(mp3_busy)) && (!mp3_pause) && (!radio_on) && (power>1) && ((newrandom(1,event_trigger[event_val]+1)==event_trigger[event_val])))
        event();                              // Time based event
    }
  }
  if ((!lcd_off) && (!distance_off) && (distance_val>0)) check_ultra(); 
                                              // Ultrasonic event
  if ((lcd_dimm) && (distance_light) && (millis()>lcd_dimm_time+150)) lcd.noBacklight();  
                                              // Switch off display light after ~1s. 150 is a workaround for stupid IDLE mode.
  minute_now=tm.Minute;
  hour_now=tm.Hour;
  selected=read_button(false);                // Read buttons
  switch(selected)
  {
    case 1:                                   // Belly: menue
      if (!lcd_off) menue_Main(); 
      break;
    case 2:                                   // Right: toggle ultrasonic
      if ((!lcd_off) && (!lcd_dimm) && (distance_val>0)) 
      { 
        distance_off=!distance_off;
        draw_time();
      }
      break;
    case 3:                                   // Left: toggle display & power save
      if (lcd_off) powerup();
      else if ((powersave) && (!charging)) powerdown();
      else
      {
        lcd_dimm=!lcd_dimm;
        draw_time();
        if (!lcd_dimm)
        {
          analogWrite(LED,0);
          lcd.backlight();  
        }
        else 
        {
          analogWrite(LED,led_val*28);
          lcd.noBacklight();
        }
        powerdown_delay(pwd_delay);
      }
      break;
    //case 4: break; // Nose unused, only voice as random event
    }
    powerdown_delay(lcd_off? 240:120); // Main loop power save!    
}}

//-------------------------------------------------------------------------------------

uint8_t read_button(boolean silent)  // Read pressed button und debounce | silent = NOKO stays silent
{
  uint16_t button_val;
  read_RTC();      // Read RTC. Powerdown_delay() messes up the internal time!
  if ((!mp3_pause) && (!(mp3_busy)) && (mp3_on)) mp3_on=false;
  if (power<5) silent=true;
  if ((!(mp3_busy)) && (!radio_on) && (!aux_on) && (!alarm_now)) turnOff_amp;
  if (check_alarm()) return 4; // Check alarm
  button_val=(analogRead(Buttons));
  if (button_val>200) return 0;
  else 
  {
    if (button_val>150)         // SW4 nose -> voice 31-60
    {
      powerdown_delay(pwd_delay);
      if (analogRead(Buttons)>150)
      {
       if ((!mp3_busy) && (!silent) && (!mp3_pause))
         if (newrandom(1,5)==4) JQ6500_play(newrandom(voice_nose_start,voice_sensor_start));
       return 4;
      }
    }
    else if (button_val>100)    // SW3 left hand
    { 
      powerdown_delay(pwd_delay);
      if (analogRead(Buttons)>100) return 3;
    }
    else if ((button_val>50))   // SW2 right hand
    {
      powerdown_delay(pwd_delay);
      if (analogRead(Buttons)>50) return 2;
    }
    else                        // SW1 belly -> voice 11-30
    {
      if (analogRead(Buttons)>0)
      {
        if ((!silent) && (!mp3_pause))
          JQ6500_play(newrandom(11,voice_nose_start)); 
        powerdown_delay(pwd_delay);
        return 1;
      }
    }
  }
}

// Constantly used functions. Saves a lot of flash.
void put_char(uint8_t x,uint8_t y,uint8_t c) // Char c at x,y
{
  lcd.setCursor(x,y);
  lcd.print(char(c));
}

void print_space(uint8_t x,uint8_t y,uint8_t val) // anz spaces from x,y
{
  lcd.setCursor(x,y);
  for (uint8_t help=val;val>0;val--) print_onespace();
}

void print_icon(uint8_t c[]) // Icon c at 0,0
{
  lcd.createChar(0,c);
  lcd.clear();
  lcd.print(char(0));
  print_onespace();
}

void init_menue()
{
  lcd.clear();
  lcd.setCursor(2,0);
}

// Plays tone with delay. Delay can be stopped by pressing SW4 when stopd is set TRUE
void play_tone(uint16_t h,uint16_t l,boolean stopd,uint16_t d) 
{
  TimerFreeTone(Speaker,h,l);
  if (!stopd) NewDelay(d-l);
  else if (selected!=4) stop_delay(d-l);
}

// Constantly used strings. Saves a lot of flash space. 
void print_onespace() {lcd.print(char(32));}  // Prints a " ". Saves flash. However.
void print_dot()      {lcd.print(char(46));}  // Prints a "."
void print_zero()     {lcd.print(char(48));}  // Prints a "0" 
void print_uhr()      {lcd.print(F(" Uhr"));} // Prints " Uhr"
 
void print_hoeren() // Prints "[h]ören"
{
  lcd.print(char(239));
  lcd.print(F("ren")); 
}

void init_char() // Read custom chars again
{
  lcd.clear();
  for (uint8_t help=0;help<8;help++)
    lcd.createChar(help,custom_char[help]);
}

void clearnum(uint8_t x,uint8_t y) // Delete a big number at x,y
{
  xlcd=x;
  ylcd=y;
  print_space(xlcd,ylcd,3);
  print_space(xlcd,ylcd+1,3);
}

void bignum(uint8_t x,uint8_t y,uint8_t num) // Draws a big number at x,y; num 0-9
{
  xlcd=x;
  ylcd=y;
  clearnum(x,y); // Clear the space
  switch(num) 
  {
    case 0: 
      put_char(xlcd,ylcd,0); 
      lcd.print(char(1));  
      lcd.print(char(2));
      put_char(xlcd,ylcd+1,3); 
      lcd.print(char(4));
      lcd.print(char(5));
      break;
    case 1: 
      put_char(xlcd,ylcd,1);
      lcd.print(char(2));
      put_char(xlcd,ylcd+1,4);
      lcd.print(char(7));
      lcd.print(char(4));
      break;
    case 2: 
      put_char(xlcd,ylcd,6);
      lcd.print(char(6));
      lcd.print(char(2));
      put_char(xlcd,ylcd+1,3);
      lcd.print(char(4));
      lcd.print(char(4));
      break;
    case 3:
      put_char(xlcd,ylcd,6);
      lcd.print(char(6));
      lcd.print(char(2));
      put_char(xlcd,ylcd+1,4);
      lcd.print(char(4));
      lcd.print(char(5));
      break;
    case 4:
      put_char(xlcd,ylcd,3);
      lcd.print(char(4));
      lcd.print(char(7));
      put_char(xlcd+2,ylcd+1,7);
      break;
    case 5:
      put_char(xlcd,ylcd,3);
      lcd.print(char(6));
      lcd.print(char(6));
      put_char(xlcd,ylcd+1,4);
      lcd.print(char(4));
      lcd.print(char(5));
      break;
    case 6: 
      put_char(xlcd,ylcd,0);
      lcd.print(char(6));
      lcd.print(char(6));
      put_char(xlcd,ylcd+1,3);
      lcd.print(char(4));
      lcd.print(char(5));
      break;
    case 7:
      put_char(xlcd,ylcd,1);
      lcd.print(char(1));
      lcd.print(char(2));
      put_char(xlcd+2,ylcd+1,7);
      break;
    case 8: 
      put_char(xlcd,ylcd,0);
      lcd.print(char(6));
      lcd.print(char(2));
      put_char(xlcd,ylcd+1,3);
      lcd.print(char(4));
      lcd.print(char(5));
      break;
    case 9: 
      put_char(xlcd,ylcd,0);
      lcd.print(char(6));
      lcd.print(char(2));
      put_char(xlcd+2,ylcd+1,7);
      break;
    case 10:
      put_char(xlcd+1,ylcd,4);
      put_char(xlcd+1,ylcd+1,4);
    break;
    case 11: 
      put_char(xlcd,ylcd,0);
      lcd.print(char(2));
      lcd.print(char(0));
      put_char(xlcd,ylcd+1,7);
      lcd.print(char(3));
      lcd.print(char(7));
      break;
    case 12: 
      put_char(xlcd,ylcd,7);
      lcd.print(char(0));
      put_char(xlcd,ylcd+1,7);
      lcd.print(char(3));
      break;
  }    
}

void powerdown() // Power save on
{
  analogWrite(LED,0);
  lcd.noDisplay();          // Turn off display
  lcd_off=true;  
  lcd_dimm=false;
  // More powersaving options?
}

void powerup()  // Power save off
{
  lcd.display();           // Turn on display  
  lcd_off=false;
  lcd_dimm=false;
  draw_all();              // Draw clock and date
}

uint8_t newrandom(uint8_t a,uint8_t b) // Better XOR random number generator 
{
    uint16_t rnd;
    rnd+=analogRead(6)+micros();
    rnd^=rnd<<2;rnd^=rnd>>7;rnd^=rnd<<7;
    return (rnd/65535.0)*(b-a)+a;
}

void NewDelay(uint16_t wait_ms)  // New delay function to save flash. It's also in the radio library
{
  #ifdef def_radio 
    Radio.newdelay(wait_ms); 
  #else 
    uint32_t target_millis=millis();
    while (millis()-target_millis<wait_ms);
  #endif
}

void stop_delay(uint16_t wait_ms) // Delay with stop when nose is pressed
{
  uint32_t target_millis=millis();
  while ((millis()-target_millis<wait_ms) && (read_button(true)!=4));
  if (read_button(true)==4) selected=4;
}

void wait_1m(boolean wait_quiet,boolean timeout) // Wait for nose until timeout 1 minute
{
  uint32_t ende=millis()+60000; // 2 MHz -> 60s:8
  selected=0;
  while (selected==0) 
  {
    selected=read_button(wait_quiet);
    if (!timeout) powerdown_delay(120);
    else if (millis()>ende) selected=4;
  }
  powerdown_delay(pwd_delay);
}

void NOKO_sleep(uint8_t wdt_time) // Sleepmode to save power
{
  wdt_enable(wdt_time); // Watchdog wakes NOKO after wdt_time
  wdt_reset();
  WDTCSR |= _BV(WDIE);
  if (lcd_dimm) set_sleep_mode(SLEEP_MODE_IDLE);        // Lowest level for the LED
  else if (lcd_off) set_sleep_mode(SLEEP_MODE_PWR_DOWN);  // Highest level
  else set_sleep_mode(SLEEP_MODE_STANDBY);             // Lower level to wake up faster
  sleep_enable();
  sleep_bod_disable();                                 // No brown-out detection
  power_adc_disable();                                 // No need for ADC during sleep
  sleep_cpu();                                         // power down!
  sleep_disable();                                     // Good morning
  wdt_disable();
  WDTCSR &= ~_BV(WDIE);
  power_adc_enable();                                  // ADC, we need you again
}

void powerdown_delay(uint8_t ms) // Calls NOKO_sleep() with watchdog-times
// Sleep times steps are pre-defined, max 8s
{
  if (lcd_dimm)  PRR = PRR | 0b00100000;           // Workaround. Not sure if IDLE is working...
  // if (ms>=256) {NOKO_sleep(WDTO_250MS); ms-=250;} // NOKO uses max 240ms
  if (ms>=128) {NOKO_sleep(WDTO_120MS); ms-=120;}
  if (ms>=64) {NOKO_sleep(WDTO_60MS); ms-=60;} 
  if (ms>=32) {NOKO_sleep(WDTO_30MS); ms-=30;}
  if (ms>=16) {NOKO_sleep(WDTO_15MS); ms-=15;}
  if (lcd_dimm) PRR = PRR & 0b00000000;            // End workaround. Stupid stupid Atmel!
}

void read_RTC() // Read RTC and store time
{
  RTC.read(tm);
  setTime(tm.Hour,tm.Minute,tm.Second,tm.Day,tm.Month,tmYearToCalendar(tm.Year));
}

boolean calc_summertime() // Summertime?
{
  if ((tm.Month<3) || (tm.Month>10)) return false;
  if ((tm.Month>3) && (tm.Month<10)) return true;
  if ((tm.Month==3) && (tm.Hour+24*tm.Day)>=(1+TZ+24*(31-(5*thisyear/4+4)%7)) || tm.Month==10 && (tm.Hour+24*tm.Day)<(1+TZ+24*(31-(5*thisyear/4+1)%7))) return true;
  return false;
}

void check_summertime() // Compare summertime with EEPROM and set clock
{
  if (((summertime!=calc_summertime()) && (tm.Hour>=4))) // Summertime changed?
  {
    (summertime)? tm.Hour--:tm.Hour++;  // Add or remove an hour
    summertime=!summertime;
    setTime(tm.Hour,tm.Minute,tm.Second,tm.Day,tm.Month,thisyear);
    tm.Year=CalendarYrToTm(thisyear);
    RTC.write(tm);                      // Write RTC
    write_EEPROM(16+offset,summertime); // Write summertime flag
    copy_eeprom();                      // Move EEPROM to new address
  }
}

void copy_eeprom() // Copy EEPROM to new address
{
  uint16_t new_offset=offset+30;
  if (offset==990) new_offset=0;    // After 33 cyles (= 16,5 years) start form 0
  for (uint8_t help=1;help<30;help++)
    write_EEPROM(help+new_offset,read_EEPROM(help+offset));  
  offset=new_offset;        
  EEPROM.write(0,offset/30);
}

void write_station(uint8_t station) // Save radio station in EEPROM
{
  radio_station[station]=radio_freq; // Write station 1
  write_EEPROM(22+station*2,radio_station[station]%10);
  write_EEPROM(21+station*2,(radio_station[station]-(radio_station[station]%10))/10);
  lcd.setCursor(2,1);
  lcd.print(F("speichern...     "));
  NewDelay(1000);
}

void draw_time() // Draw clock, power level and flags
{
  uint8_t help;
  read_time();
  night_now=check_night();
  (tm.Hour<10)? bignum(1,0,0):bignum(1,0,tm.Hour/10);
  bignum(4,0,tm.Hour%10);
  bignum(7,0,10);
  (tm.Minute<10)? bignum(10,0,0):bignum(10,0,tm.Minute/10);
  bignum(13,0,tm.Minute%10);
  print_space(17,1,3);
  lcd.setCursor(17,1);
  //--- Flags
  if ((distance_off) || (distance_val==0)) lcd.print(char(120));
  if (lcd_dimm) lcd.print(char(68));
  if (nightmode_on) (night_now)? lcd.print(char(78)):lcd.print(char(110));
  if (birthday)
  {
    lcd.setCursor(3,2);
    lcd.print(F("Geburtstag!"));
  }
  print_space(17,2,3);
  lcd.setCursor(17,2);
  if (alarm_on) lcd.print(char(244));
  if (mp3_pause) lcd.print(char(112));
  if ((mp3_on) || (aux_on) || (radio_on)) lcd.print(char(80));
  //--- power level and charging
  power=analogRead(Battery); // Calculate power level from 5 measurements
  for (help=4;help>0;help--)
  {
    powerdown_delay(10);
    power+=analogRead(Battery);
  }
  // Voltage: min_V to max_V
  power=(uint16_t)(((power/5)*(5.0/1024)-min_V)/((max_V-min_V)/100)); 
  power=constrain(power,1,99);
  // >57% Voltage are about 92% of overall capacity. Drops at 3,63V
  if (power>57) power=map(power,57,99,8,99); 
  else power=map(power,1,56,1,8);
  lcd.setCursor(17,0);
  if (power<10) 
  {
    put_char(17,0,32);
    lcd.blink(); // Blink when below 10%
  }
  else lcd.noBlink();
  lcd.print(power);
  charging? lcd.print(char(43)):lcd.print(char(37)); // Charging: print + or %
  lcd.setCursor(18,0);
}

void draw_date() // Draw date
{
  thisyear=year();
  tm.Day=day();
  tm.Month=month();
  lcd.setCursor(0,3);
  lcd.print(dayStr(weekday())); 
  lcd.setCursor(10,3);
  if (tm.Day<10) print_zero();
  lcd.print(tm.Day);
  print_dot();
  if (tm.Month<10) print_zero();
  lcd.print(tm.Month);
  print_dot();
  lcd.print(thisyear);
  if (power<10) lcd.setCursor(18,0);
}

void draw_all() // Draw the entire display new
{
  init_char();
  draw_date();
  draw_time();
}

void read_time() // Read internal time
{
  tm.Minute=minute();
  tm.Hour=hour();
}

void print_station(uint8_t s) // Print the stations frequencies
{
  lcd.setCursor(2,1);
  lcd.print(F("Station "));
  lcd.print(s);
  lcd.print(char(58));
  print_onespace();
  lcd.print(float(radio_station[s-1])/10,2);
  print_space(radio_station[s-1]>999? 19:18,1,radio_station[s-1]>999? 1:2);
  lcd.setCursor(xlcd,ylcd);
}

boolean check_night() // Is it nighttime?
{
  boolean yes=false;
  if (nightmode_on)
  {
    if (night_hh>night_to_hh)
      if ((tm.Hour>night_hh) || (tm.Hour<night_to_hh)) yes=true;
    if (night_hh<night_to_hh)
      if ((tm.Hour>night_hh) && (tm.Hour<night_to_hh)) yes=true;
    if (night_hh==night_to_hh)
    {
      if (night_mm<=night_to_mm)
        if ((tm.Minute>=night_mm) && (tm.Minute<=night_to_mm)) yes=true;
      if (night_hh>night_to_mm)
        if ((tm.Minute<=night_to_mm) || (tm.Minute>=night_to_mm)) yes=true;
    }
    if ((tm.Hour==night_hh) && (tm.Minute>=night_mm)) yes=true;
    if ((tm.Hour==night_to_hh) && (tm.Minute<night_to_mm)) yes=true;
  }
  if (yes!=night_over) // When nightmode changes play sound
  {
    #ifdef voice_set_111 
      yes? JQ6500_play(71):JQ6500_play(102); // Off: Rooster. On: Let me sleep!
    #endif
    #ifdef voice_set_226
      yes? JQ6500_play(111):JQ6500_play(172); // Same but other voice set
    #endif
    night_over=yes;
  }
  return yes;
}

void check_ultra()  // Ultrasonic event: Distance_val= 0..9 * 10cm
{
  turnOn_pingPin;
  delayMicroseconds(10);
  turnOff_pingPin;
  if ((pulseIn(Distance_inPin,HIGH) /29 /2)<distance_val*sensor)  // Is there something in distance?
  {
    if ((lcd_dimm) && (distance_light)) // Display light off and light flag set?
    {
      lcd.backlight();              // Turn on light and set timer
      lcd_dimm_time=millis();
    }
    if ((!night_now) && (!distance_now) && (!radio_on) && (!mp3_pause) && (!mp3_busy))
    {
      JQ6500_play(newrandom(voice_sensor_start,voice_time_start)); // Play random voice event. 
      distance_now=true; // Set ultra_evet flag to prevent more events than one in a minute
    }
  }
}  

boolean check_alarm() // Is it time for the alarm?
{
  if ((alarm_on) && (!alarm_now)) // Prevent double check
  {
    read_time();                  // Read internal time
    if ((!alarm_mute) && (alarm_days & (1<<weekday()-1))) // Is alarm allowed and day selected?
      if ((alarm_mm==tm.Minute) && (alarm_hh==tm.Hour)) 
      {
        alarm();                  // Yes! Alarm!
        return true;
      }
    if (alarm_mm!=tm.Minute) alarm_mute=false; // Other time: Mute off
  }
  return false;
}

void alarm() // Play alarm
{
  if (lcd_off) lcd.display();     // Display on
  if (lcd_dimm) lcd.backlight();  // Light on
  turnOn_amp;
  lcd.createChar(0,custom_char[17]);
  lcd.clear();
  lcd.noBlink();
  lcd.setCursor(7,0);
  if (tm.Hour<10) print_zero();
  lcd.print(tm.Hour);
  lcd.print(char(58));
  if (tm.Minute<10) print_zero();
  lcd.print(tm.Minute);
  lcd.setCursor(4,3);
  lcd.print(F("Nase dr cken"));
  put_char(11,3,245);
  alarm_now=true;
  if (aux_on) turnOff_aux;
  if ((alarm_type==1) && (!radio_on) && (!mp3_on))  // Radio alarm
  {
      radio_start();
      radio_on=true;
  }
  selected=0;
  while ((selected!=4) && (alarm_mm==tm.Minute)) // Alarm until one minute timeout or SW4 nose 
  {
    put_char(5,2,0);
    lcd.print(F(" Alarm "));
    lcd.print(char(0));
    selected=read_button(true);
    analogWrite(LED,255);   
    if ((alarm_type==0) || (mp3_on) || (mp3_pause))    // Tone alarm
      play_alarm_tone(alarm_tone); 
    if ((alarm_type==2) && (!(mp3_busy)) && (!mp3_on)) // MP3 alarm
      JQ6500_play(alarm_mp3+1); 
    stop_delay(500);
    print_space(5,2,9); // Clear "Alarm" to let it blink
    analogWrite(LED,0); // Let the LED blink
    stop_delay(500); // Is nose pressed? Wait 500ms else set selected=4
    tm.Minute=minute();
  }
  if (!mp3_on) mp3.pause();
  else if (alarm_type==1) 
    #ifdef def_radio
      Radio.powerOff(); 
    #endif
  if (aux_on) turnOn_aux;
  alarm_now=false;
  if ((alarm_mm==tm.Minute) && (alarm_hh==tm.Hour)) alarm_mute=true;
  lcd.clear();
  if ((lcd_off) && (!radio_on) && (!(mp3_busy))) turnOff_amp; 
  if (lcd_off) lcd.noDisplay();
  if (lcd_dimm) lcd.noBacklight();
  draw_all();
}

//--- MENUES

void menue_Main() // Main menue
{
  uint8_t menue=0; // Selected menue number 0..3
  uint8_t help=0;
  powerdown_delay(80); // Debounce
  lcd.noBlink();
  lcd.clear();
  lcd.setCursor(2,0);
  lcd.print(F("Spiel was vor")); // Play menue
  lcd.setCursor(2,1);
  lcd.print(F("Alarm stellen")); // Alarm menue
  lcd.setCursor(1,2);
  print_uhr();
  lcd.print(F("zeit stellen"));  // Time and nightmode menue
  lcd.setCursor(2,3);
  lcd.print(F("NOKO stellen"));  // Settings menue
  while (selected!=4)
  {
    put_char(0,menue,126);
    wait_1m(false,true);
    switch(selected) 
    {
      case 1:
        switch(menue) 
        {
          case 0: menue_Play(); break;
          case 1: menue_Alarm(); break;
          case 2: menue_Time(); break;
          case 3: menue_Settings(); break;
        }
        break;
      case 2:
        put_char(0,menue,32);
        if (menue<3) menue++;
        break;
      case 3:
        put_char(0,menue,32);
        if (menue>help) menue--;
        break;
    }
  }
  draw_all();
}

void menue_Play() // Play menue "Spiel was vor"
{
  uint8_t menue=0;
  lcd.createChar(0,custom_char[15]);
  init_menue();
  lcd.print(F("Radio h"));           // Radio menue
  print_hoeren();
  lcd.setCursor(2,1);                // MP3 menue
  lcd.print(F("Eigenes h"));  
  print_hoeren();
  lcd.setCursor(2,2);                // Toggle AUX
  lcd.print(F("AUX-Eingang"));
  lcd.setCursor(2,3);                // Story menue
  lcd.print(F("Geschichten h"));     
  print_hoeren();
  while (selected!=4)
  {
    #ifndef def_radio                // Is the radio available?
      put_char(1,0,120);
      #else
      put_char(1,0,radio_on? 0:32);  // Ternary saves flash! Well. Sometimes.
    #endif   
    put_char(1,1,file_on? 0:32);
    put_char(1,2,aux_on? 0:32); 
    #ifndef def_stories              // Are there stories on SD card?
      put_char(1,3,120);
      #else
      put_char(1,3, story_on? 0:32); 
    #endif
    put_char(0,menue,126);
    wait_1m(true,true);
    switch(selected) 
    {
      case 1: 
        switch(menue)
        {
          case 0: menue_Radio(); break;
          case 1: (max_files>0)? menue_MP3(2):menue_MP3(3); break;
          case 2: 
            if (!aux_on)
            {
              if (radio_on) radio_off();
              if ((!mp3_pause) && (mp3_busy)) mp3_off();
              turnOn_amp;
              turnOn_aux;   
            }
            else
            {
              turnOff_amp;    
              turnOff_aux; 
            }
            aux_on=!aux_on;
            NewDelay(500);
            break;
          #ifdef def_stories
            case 3: menue_MP3(1); break;
          #endif         
        }
        break;
      case 2:
        put_char(0,menue,32);
        if (menue<3) menue++;
        break;
      case 3:
        put_char(0,menue,32);
        if (menue>0) menue--;
        break;
    }
  }
  init_char();
}

void menue_Radio() // Radio menue "Radio hoeren"
{
  #ifdef def_radio
  char rdsStation[10]; // RDS station name
  char rdsText[64];    // RDS text 16*4 characters
  boolean rds=true;
  boolean save2=false;
  boolean save=false;
  uint8_t menue=2;
  uint8_t help,help2,count_rows;
  uint8_t count_save=0;
  uint16_t help_freq;
  uint32_t zmillis;
  lcd.createChar(1,custom_char[9]);
  lcd.createChar(2,custom_char[11]);
  lcd.createChar(3,custom_char[12]);
  lcd.createChar(4,custom_char[8]);
  print_icon(custom_char[15]);
  while (selected!=4)
  {
    put_char(2,2,91);
    lcd.print(char(2));
    lcd.print(F("][<][ ][>]["));
    lcd.print(char(3));
    lcd.print(char(93));
    lcd.setCursor(2,3);
    lcd.print(F("[1|s] [2|s] [3|s] "));
    print_space(1,1,19);
    lcd.blink();
    put_char(9,2,radio_on? 1:4);
    lcd.setCursor(2,0);
    lcd.print(float(radio_freq)/10,2);
    print_onespace();
    if (save)
    {
      turnOn_amp;
      radio_start();
      print_space(9,0,10);
      save=false;
    }
    switch(menue)
    {
       case 5: 
         xlcd=3;
         ylcd=3;
         print_station(1);
         rds=false;
         break;
       case 6: 
         xlcd=5;
         ylcd=3;
         break;
       case 7: 
         xlcd=9;
         ylcd=3;
         print_station(2);
         rds=false;
         break;
       case 8: 
         xlcd=11;
         ylcd=3;
         break;
       case 9: 
         xlcd=15;
         ylcd=3;
         print_station(3);
         rds=false;
         break;
       case 10: 
         xlcd=17;
         ylcd=3; 
         break;     
       default:
         xlcd=3+(menue*3);
         ylcd=2;
    }
    if (!(radio_freq==help_freq)) // Clear RDS when frequency was changed
    {
      memset(rdsStation,0,10);
      memset(rdsText,0,64);
      print_space(9,0,8);
    }
    lcd.setCursor(xlcd,ylcd);
    if ((radio_on) && (rds)) 
      do // Wait for button and poll RDS data       
      {
        for (count_rows=count_save;count_rows<4;count_rows++) // Read RDS text
        {
          count_save=count_rows;
          for (help2=0;help2<3;help2++) // Read 4x16 chars
          {
            lcd.setCursor(2,1);
            for (help=0;help<16;help++) 
              ((rdsText[help+(count_rows*16)])<16)? lcd.print(char(32)):lcd.print(rdsText[help+(count_rows*16)]);
            lcd.setCursor(xlcd,ylcd);
            zmillis=millis();
            while (((millis()-zmillis)<500) && ((selected=read_button(false))==0))
            {
              Radio.readRDS_Radiotext(rdsText);
              Radio.readRDS(rdsStation); // Read station name
            }
            lcd.setCursor(9,0);
            lcd.print(rdsStation); 
            lcd.setCursor(xlcd,ylcd);
            if (selected>0) break;
            powerdown_delay(pwd_delay);
          }  
          if (count_save==3) count_save=0;
          if (selected>0) break;
        }
      } while (selected==0); 
    else wait_1m(false,true);
    rds=true;
    help_freq=radio_freq;
    switch(selected)
    {
      case 1:
        save2=true;
        switch(menue)
        {
          case 0: if (radio_on) radio_freq=Radio.seekDown(); break;
          case 1: if (radio_freq>=875) radio_freq--; break;
          case 2:
            if (radio_on) 
            {
              radio_off();
              save=false;
            }
            else
            {
              if ((!mp3_pause) && (mp3_busy)) mp3_off();
              aux_off();
              radio_on=true;
            }
            break;
          case 3: if (radio_freq<=1079) radio_freq++; break;
          case 4: if (radio_on) radio_freq=Radio.seekUp(); break;
          case 5: 
            radio_freq=radio_station[0]; 
            menue=2; 
            break;
          case 6: write_station(0); break;
          case 7: 
            radio_freq=radio_station[1]; 
            menue=2;
            break;
          case 8: write_station(1); break;
          case 9: 
            radio_freq=radio_station[2]; 
            menue=2; 
            break;
          case 10: write_station(2); break;
        }
        if (radio_on) save=true;
        break;
      case 2: if (menue<10) menue++; break;
      case 3: if (menue>0) menue--;  break;
    } 
  }
  if (save2)
  {
    write_EEPROM(18,radio_freq%10); // Keep actual frequence
    write_EEPROM(17,(radio_freq-(radio_freq%10))/10);    
  }
  save2=false;
  init_char();
  #endif
}

// Story menu & MP3 menu: "Geschichten hören" "Eigenes hören"
// 1=Story 2=MP3 3=No MP3 files found
void menue_MP3(uint8_t modus) 
{
  uint8_t menue=1;
  uint8_t help;
  uint16_t file_length,index_start,index,max_index;
  boolean must_update=true;
  char name_buffer[12];
  lcd.createChar(1,custom_char[9]);
  lcd.createChar(2,custom_char[10]);
  lcd.createChar(3,custom_char[8]);
  print_icon(custom_char[15]);
  lcd.setCursor(4,3);
  lcd.print(F("[<][ ][ ][>]"));
  put_char(11,3,2);
  if (mp3_pause) menue=2;
  if (!mp3_on) while (mp3_busy);
  lcd.blink();
  while (selected!=4)
  {
    if (must_update)
    {
      must_update=false;
      lcd.setCursor(2,0);
      if (modus==1)
      {
       index_start=voice_birthday;
       index=story;
       max_index=max_stories;
      }
      if (modus==2)
      {
        index_start=voice_birthday+max_stories;
        index=file;
        max_index=max_files;
      }
      if ((!mp3_busy) && (!mp3_pause)) 
      {
        powerdown_delay(120);
        mp3.playFileByIndexNumber(index_start+index); // Set index to file
        powerdown_delay(100);
        mp3_off(); // Stop at once to keep index set
        powerdown_delay(100);         
      }
      if (index<10) print_zero();
      if (index<100) print_zero();
      lcd.print(index); // Print story number
      lcd.print(char(47));
      if (max_index<10) print_zero();
      if (max_index<100) print_zero();
      lcd.print(max_index); // Print last story number
      print_space(12,0,8);
      if (((modus==1) && (!file_on)) || ((modus==2) && (!story_on))) // Print file length
      {
        lcd.setCursor(12,0);
        powerdown_delay(100);
        file_length=mp3.currentFileLengthInSeconds(); // Read file length 
        help=file_length/3600;
        if (help<10) print_zero();
        lcd.print(help);
        lcd.print(char(58));
        help=(file_length%3600)/60;
        if (help<10) print_zero();
        lcd.print(help);
        lcd.print(char(58));
        help=(file_length%3600)%60;
        if (help<10) print_zero();
        lcd.print(help);  
      }    
      if (modus==1) // Story menu "Geschichten hören"
      {
        for (help=0;help<20;help++) // Print name and author
        {
          put_char(help,1,(read_disc(Disc0,291+((story-1)*40)+help)));
          put_char(help,2,(read_disc(Disc0,291+((story-1)*40)+help+20)));
        }  
      }
      if (modus==2) // MP3 menue "Eigenes hören"
      {    
        lcd.setCursor(2,1);
        if (story_on) lcd.print(F("Geschichte")); // Is a story running?
        else
        {
          powerdown_delay(100); // Read filename. Only 8 chars possible :-(
          mp3.currentFileName(name_buffer,sizeof(name_buffer));
          for (help=0;help<11;help++)
          {
            lcd.print(name_buffer[help]);
            if (help==7) lcd.print(char(46));
          }
        }
        if (file_on)
        {
          powerdown_delay(100);
          file=mp3.currentFileIndexNumber(MP3_SRC_SDCARD)-(voice_birthday+max_stories); // Read file number
        }
      }
      if (modus==3) lcd.print(F("Keine MP3s"));// No MP3s found!
      lcd.setCursor(12,0);
      if (mp3_pause) lcd.print(F("   PAUSE"));
      put_char(8,3,((mp3_busy) || (mp3_pause))? 1:3);
    }
    lcd.setCursor(5+(menue*3),3);
    wait_1m(true,true);
    switch(selected)
    {
     case 1:
        must_update=true;
        switch(menue)
        {
        case 0: // Prev
          if (modus==1)
          {
            if (story>1) story--;
            else story=max_stories;
            if (file_on) {file_on=false; story_on=true;}     
            index=story;
            }
          if (modus==2)
          {
            if (file>1) file--;
            else file=max_files;
            if (story_on) {story_on=false; file_on=true;}
            index=file;
          }
          if (mp3_busy) JQ6500_play(index_start+index);
          break;
        case 1: // On/Off
          radio_off(); 
          aux_off();
          story_on=false;
          file_on=false;
          if (!mp3_busy)
          {
            if (modus==1) story_on=true;
            if (modus==2) file_on=true;
            JQ6500_play(index_start+index);
            mp3_on=true;
          }
          else mp3_off();
          break;
        case 2: // Pause
          if ((mp3_busy) || (mp3_pause))
          {
            if (radio_on) radio_off();
            if (mp3_pause)
            {
              turnOn_amp;
              mp3_pause=false;
              mp3.play();
            }
            else 
            {
              mp3_pause=true;
              mp3.pause();
            }
          }
          break;
        case 3: // Next
          if (modus==1)
          {
            if (story<max_stories) story++;
            else story=1;
            if (file_on) {file_on=false; story_on=true;}
            index=story;
          }
          if (modus==2)
          {
            if (file<max_files) file++;
            else file=1;
            if (story_on) {story_on=false; file_on=true;}
            index=file;
          }
          if (mp3_busy) JQ6500_play(index_start+index);
          break;
        }
        break;
      case 2:
        if ((!mp3_pause) && (menue<3)) menue++;
        break; 
      case 3:
        if ((!mp3_pause) && (menue>0)) menue--;
        break;
      }
   }
  init_char();
}
void menue_Alarm()  // Set alarm. No alarm allowed in this menue
{
  uint8_t menue=0;
  boolean save=false;
  alarm_now=true;
  print_icon(custom_char[17]);
  if (alarm_hh<10) print_zero();
  lcd.print(alarm_hh);
  lcd.print(char(58));
  if (alarm_mm<10) print_zero();
  lcd.print(alarm_mm);
  print_uhr();
  lcd.setCursor(2,2);
  lcd.print(F("Wecktage [       ]"));
  lcd.setCursor(2,3);
  lcd.print(F("Weckton    [     ]"));
  lcd.setCursor(14,3);
  switch(alarm_type)
  {
    case 0:lcd.print(F("Ton")); break; // Choose alarm type
    case 1:lcd.print(F("Radio")); break;
    case 2:lcd.print(F("MP3")); break;
  }
  lcd.blink();
  while(selected!=4)
  {
    lcd.setCursor(12,2);
    lcd.print(bool(alarm_days & 2)? char(77):char(32));  // M
    lcd.print(bool(alarm_days & 4)? char(68):char(32));  // D
    lcd.print(bool(alarm_days & 8)? char(77):char(32));  // M 
    lcd.print(bool(alarm_days & 16)? char(68):char(32)); // D
    lcd.print(bool(alarm_days & 32)? char(70):char(32)); // F
    lcd.print(bool(alarm_days & 64)? char(83):char(32)); // S
    lcd.print(bool(alarm_days & 1)? char(83):char(32));  // S
    put_char(0,1,32);
    put_char(0,2,32);
    put_char(0,3,32);
    if (menue<5) put_char(0,1,126);
    else if (menue<12) put_char(0,2,126);
    else put_char(0,3,126);
    lcd.setCursor(2,1);  
    if (alarm_hh<10) print_zero();
    lcd.print(alarm_hh);
    lcd.print(char(58));
    if (alarm_mm<10) print_zero();
    lcd.print(alarm_mm);
    print_uhr();
    print_space(11,1,4);
    (alarm_on)? lcd.print(F("[an ]")):lcd.print(F("[aus]")); // Alarm on/off?
    switch(menue)
    {
      case 0: lcd.setCursor(2,1); break;
      case 1: lcd.setCursor(3,1); break;
      case 2: lcd.setCursor(5,1); break;
      case 3: lcd.setCursor(6,1); break;
      case 4: lcd.setCursor(16,1); break;
      case 5: lcd.setCursor(12,2); break;
      case 6: lcd.setCursor(13,2); break;
      case 7: lcd.setCursor(14,2); break;
      case 8: lcd.setCursor(15,2); break;
      case 9: lcd.setCursor(16,2); break;
      case 10: lcd.setCursor(17,2); break;
      case 11: lcd.setCursor(18,2); break;     
      case 12: lcd.setCursor(14,3); break;
    }
    wait_1m(false,true);
    switch(selected)
    {
      case 1:
        save=true;
        switch(menue) // Set alarm: Belly adds one
        {
          case 0: (alarm_hh<14)? alarm_hh+=10:alarm_hh%=10; break;
          case 1: (alarm_hh%10==9)? alarm_hh-=9:alarm_hh++;
                  if (alarm_hh==24) alarm_hh=20; break;
          case 2: (alarm_mm<49)? alarm_mm+=10:alarm_mm%=10; break;
          case 3: (alarm_mm%10==9)? alarm_mm-=9:alarm_mm++; break;
          case 4: alarm_on=!alarm_on; break;
          case 5: bool(alarm_days & 2)? alarm_days &= ~(2):alarm_days |= 2; break;
          case 6: bool(alarm_days & 4)? alarm_days &= ~(4):alarm_days |= 4; break;
          case 7: bool(alarm_days & 8)? alarm_days &= ~(8):alarm_days |= 8; break;
          case 8: bool(alarm_days & 16)? alarm_days &= ~(16):alarm_days |= 16; break;
          case 9: bool(alarm_days & 32)? alarm_days &= ~(32):alarm_days |= 32; break;
          case 10: bool(alarm_days & 64)? alarm_days &= ~(64):alarm_days |= 64; break;
          case 11: bool(alarm_days & 1)? alarm_days &= ~(1):alarm_days |= 1; break;
          case 12:
            lcd.noBlink();
            menue_AlarmType();
            break;
        }
        break;
      case 2: if (menue<12) menue++; break;
      case 3: if (menue>0) menue--; break;
    }
  }
  if (save) // Write alarm to EEPROM
  {
    write_EEPROM(3,alarm_on);
    write_EEPROM(4,alarm_mm);
    write_EEPROM(5,alarm_hh);
    write_EEPROM(29,alarm_days);
  }
  lcd.createChar(0,custom_char[0]);
  alarm_now=false;
  save=false;
}

void menue_AlarmType() // Which alarm type? No alarm allowed here
{
  uint8_t menue=0;
  boolean alarm_on_help=alarm_on;
  alarm_on=false;
  init_menue();
  lcd.print(F("[ ] Weckton")); // Tone 0-5
  lcd.setCursor(2,1);
  lcd.print(F("[ ] Radio"));   // Radio
  lcd.setCursor(2,2);
  lcd.print(F("[ ] MP3"));     // MP3 0-9
  while (selected!=4)
  {
    for (uint8_t help=0;help>2;help++) 
      put_char(3,help,32);
    put_char(3,alarm_type,88);
    put_char(0,menue,126);
    #ifndef def_radio
      put_char(3,1,45);
    #endif
    lcd.setCursor(3,menue);
    wait_1m(false,true);
    switch(selected) 
    {
      case 1:
        #ifndef def_radio
          if (menue==1) break;
        #endif
        alarm_type=menue;
        write_EEPROM(6,alarm_type);
        if (alarm_type==0) menue_SelectAlarm(1);
        if (alarm_type==2) menue_SelectAlarm(2);
        break;
      case 2:
        put_char(0,menue,32);
        if (menue<2) menue++;
        break;
      case 3:
        put_char(0,menue,32);
        if (menue>0) menue--;
        break;
    }
  }
  alarm_on=alarm_on_help;
  lcd.createChar(0,custom_char[0]);
}

// Select sound number
// 1=Tone 0-5; 2=MP3 0-9
void menue_SelectAlarm(uint8_t modus)
{
  boolean save=false;
  lcd.clear();
  lcd.print(char(126));
  lcd.print(F(" Weckton +/- [ ]"));
  lcd.setCursor(15,0);
  lcd.blink();
  if (modus==1) lcd.print(alarm_tone);
  else lcd.print(alarm_mp3);
  sound_on();
  alarm_now=true;
  while (selected!=4)
  {
    lcd.setCursor(15,0);
    wait_1m(true,true);
    if (selected==1)
    {
      save=true;
      if (modus==1) // Press belly to add one
      {
        (alarm_tone==5)? alarm_tone=0:alarm_tone++;
        lcd.print(alarm_tone);
        play_alarm_tone(alarm_tone);
      }
      else
      {
        (alarm_mp3==10)? alarm_mp3=0:alarm_mp3++;
        lcd.print(alarm_mp3);
        if (!mp3_on) JQ6500_play(alarm_mp3+1);
      }
    }
  }
  if (save)
  {
    if (modus==1) write_EEPROM(7,alarm_tone); // Save
    else write_EEPROM(19,alarm_mp3);
  }
  if (!mp3_on) mp3.pause();
  alarm_now=false;
  save=false;
}

void menue_Time()  // Set time and nightmode
{
  uint8_t menue=0;
  uint8_t new_hour=tm.Hour;
  uint8_t new_minute=tm.Minute;
  uint8_t new_day=tm.Day;
  uint8_t new_month=tm.Month;
  boolean save=false;
  print_icon(custom_char[16]);
  lcd.print(F("Neue Uhrzeit/Datum"));
  lcd.setCursor(2,1);
  lcd.print(F("Nachtmodus [   ]")); // To nightmode menue
  lcd.blink();
  thisyear-=2000;
  while(selected!=4)
  {
    lcd.setCursor(2,2);  
    if (new_hour<10) print_zero();
    lcd.print(new_hour);
    lcd.print(char(58));
    if (new_minute<10) print_zero();
    lcd.print(new_minute);
    print_uhr();
    lcd.setCursor(2,3);  
    if (new_day<10) print_zero();
    lcd.print(new_day);
    print_dot();
    if (new_month<10) print_zero();
    lcd.print(new_month);
    lcd.print(F(".20"));
    if (thisyear<10) print_zero();
    lcd.print(thisyear);
    lcd.setCursor(14,1);
    (nightmode_on)? lcd.print(F("an ")):lcd.print(F("aus"));
    if (menue==0)                
    {
      put_char(0,2,32);
      lcd.setCursor(0,1);
    }
    if (menue>0)             
    {
      put_char(0,1,32);
      put_char(0,3,32);
      lcd.setCursor(0,2); 
    }
    if ((menue>4) && (menue<11))
    {
      put_char(0,2,32);
      lcd.setCursor(0,3);
    }
    lcd.print(char(126));
    switch(menue)
    {
      case 0: lcd.setCursor(14,1); break;
      case 1: lcd.setCursor(2,2); break;
      case 2: lcd.setCursor(3,2); break;
      case 3: lcd.setCursor(5,2); break;
      case 4: lcd.setCursor(6,2); break;
      case 5: lcd.setCursor(2,3); break;
      case 6: lcd.setCursor(3,3); break;
      case 7: lcd.setCursor(5,3); break;
      case 8: lcd.setCursor(6,3); break;
      case 9: lcd.setCursor(10,3); break;
      case 10: lcd.setCursor(11,3); break;
    }
    wait_1m(false,true);
    switch(selected)
      {
      case 1:
        if (menue>0) save=true;
        switch(menue)
        {
          case 0: menue_NightMode(); break;
          case 1: (new_hour<14)? new_hour+=10:new_hour%=10; break;
          case 2: (new_hour%10==9)? new_hour-=9:new_hour++;
                  if (new_hour==24) new_hour=20; break;
          case 3: (new_minute<49)? new_minute+=10:new_minute%=10; break;
          case 4: (new_minute%10==9)? new_minute-=9:new_minute++; break;
          case 5:
            (new_day<22)? new_day+=10:new_day%=10;
            if (new_day==0) new_day=1;
            break;
          case 6: 
            (new_day%10==9)? new_day-=9:new_day++; 
            if (new_day>31) new_day=30; 
            if (new_day==0) new_day=1; 
            break;
          case 7:
            (new_month<3)? new_month+=10:new_month%=10;
            if (new_month==0) new_month=1;
            break;
          case 8: (new_month<12)? new_month++:new_month=10; break;
          case 9: (thisyear<89)? thisyear+=10:thisyear%=10; break;
          case 10: (thisyear%10==9)? thisyear-=9:thisyear++; break;
        }
        break;
      case 2: if (menue<10) menue++; break;
      case 3: if (menue>0) menue--; break;
    }
  }  
  thisyear+=2000;
  if (save)
  {
    setTime(new_hour,new_minute,0,new_day,new_month,thisyear);
    tm.Hour=new_hour; 
    tm.Minute=new_minute; 
    tm.Second=0; 
    tm.Day=new_day;
    tm.Month=new_month;
    tm.Year=CalendarYrToTm(thisyear);
    RTC.write(tm);
    save=false;
    draw_all();
    summertime=calc_summertime();
    write_EEPROM(16,summertime);
  }
  lcd.createChar(0,custom_char[0]);
  new_minute=255;
}

void menue_NightMode() // Set nightmode
{
  uint8_t menue=0;
  boolean save=false;
  print_icon(custom_char[18]);
  lcd.print(F("Nachtmodus stellen"));
  lcd.setCursor(2,1);
  lcd.print(F("[   ] Dimmen [ ]")); // Dim light when NOKO enters nightmode
  lcd.setCursor(2,2);
  lcd.print(F("von"));
  lcd.setCursor(2,3);
  lcd.print(F("bis"));
  while(selected!=4)
  {
    lcd.setCursor(3,1);
    (nightmode_on)? lcd.print(F("an ")):lcd.print(F("aus")); // Nightmode on or off?
    put_char(16,1,night_lcd_dimm? 88:32);
    lcd.setCursor(6,2);  
    if (night_hh<10) print_zero();
    lcd.print(night_hh);
    lcd.print(char(58));
    if (night_mm<10) print_zero();
    lcd.print(night_mm);
    print_uhr();
    lcd.setCursor(6,3);  
    if (night_to_hh<10) print_zero();
    lcd.print(night_to_hh);
    lcd.print(char(58));
    if (night_to_mm<10) print_zero();
    lcd.print(night_to_mm);
    print_uhr();
    if (menue==0)
    {
      put_char(0,2,32);
      lcd.setCursor(0,1); 
    }
    if (menue==1)
    {
      put_char(0,2,32);
      lcd.setCursor(0,1); 
    }
    if ((menue<6) && (menue>1)) // Set arrow
    {
      put_char(0,1,32);
      put_char(0,3,32);
      lcd.setCursor(0,2); 
    }
    if (menue>5) 
    {
      put_char(0,2,32);
      lcd.setCursor(0,3);
    }
    lcd.print(char(126));
    switch(menue)
    {
      case 0: lcd.setCursor(3,1); break;
      case 1: lcd.setCursor(16,1); break;
      case 2: lcd.setCursor(6,2); break;
      case 3: lcd.setCursor(7,2); break;
      case 4: lcd.setCursor(9,2); break;
      case 5: lcd.setCursor(10,2); break;
      case 6: lcd.setCursor(6,3); break;
      case 7: lcd.setCursor(7,3); break;
      case 8: lcd.setCursor(9,3); break;
      case 9: lcd.setCursor(10,3); break;
    }
    wait_1m(false,true);
    switch(selected)
    {
      case 1:
        save=true;
        switch(menue)
        {
          case 0: nightmode_on=!nightmode_on; break;
          case 1: night_lcd_dimm=!night_lcd_dimm; break;          
          case 2: (night_hh<14)? night_hh+=10:night_hh%=10; break;
          case 3: (night_hh%10==9)? night_hh-=9:night_hh++;
                  if (night_hh==24) night_hh=20; break;
          case 4: (night_mm<49)? night_mm+=10:night_mm%=10; break;
          case 5: (night_mm%10==9)? night_mm-=9:night_mm++; break;
          case 6: (night_to_hh<14)? night_to_hh+=10:night_to_hh%=10; break;
          case 7: (night_to_hh%10==9)? night_to_hh-=9:night_to_hh++;
                  if (night_to_hh==24) night_to_hh=20; break;
          case 8: (night_to_mm<49)? night_to_mm+=10:night_to_mm%=10; break;
          case 9: (night_to_mm%10==9)? night_to_mm-=9:night_to_mm++; break;
        }
        if (save)
        {
          write_EEPROM(11,nightmode_on);
          write_EEPROM(12,night_hh);
          write_EEPROM(13,night_mm);
          write_EEPROM(14,night_to_hh);
          write_EEPROM(15,night_to_mm);
          write_EEPROM(28,night_lcd_dimm);
        }
        save=false;
        break;
      case 2: if (menue<9) menue++; break;
      case 3: if (menue>0) menue--; break;
    }
  }
  lcd.createChar(0,custom_char[0]);
}

void menue_Settings()  // Settings "NOKO stellen"
{
  uint8_t menue=0;
  init_menue();
  lcd.print(F("LED     +/- [ ]")); // LED brightness 0-9 * 28 (0..255)
  lcd.setCursor(2,1);
  lcd.print(F("Events  +/- [ ]")); // Chance for time event 0-9 (1=ca. every two hours; 9=nearly every minute)
  lcd.setCursor(2,2);
  lcd.print(F("Distanz +/- [ ]")); // Reaction distance for ultrasonic 0-9 * 10cm
  lcd.setCursor(2,3);
  lcd.print(F("weiter..."));
  while(selected!=4)
  {
    lcd.blink();
    analogWrite(LED,(menue==0)? led_val*28:0);
    if (menue==3) lcd.noBlink();
    put_char(0,menue,126);
    lcd.setCursor(15,0);
    lcd.print(led_val);
    lcd.setCursor(15,1);
    lcd.print(event_val);
    lcd.setCursor(15,2);
    lcd.print(distance_val);
    lcd.setCursor(15,menue);
    wait_1m(false,true);
    switch(selected)
    {
      case 1:  
        switch(menue)
        {
          case 0:
            (led_val<9)? led_val++:led_val=0;
            analogWrite(LED,led_val*28);
            write_EEPROM(2,led_val);
            break;
          case 1:
            (event_val<9)? event_val++:event_val=0;
            write_EEPROM(9,event_val);
            break;
          case 2:
            (distance_val<9)? distance_val++:distance_val=0;
            write_EEPROM(8,distance_val);
            break;
          case 3: menue_Settings2(); break;
        }
        break;
      case 2:
        put_char(0,menue,32);
        if (menue<3) menue++;
        lcd.setCursor(0,menue);
        break;
      case 3:
        put_char(0,menue,32);
        if (menue>0) menue--;
        lcd.setCursor(0,menue);
        break;
    }
  }
  if (!lcd_dimm) analogWrite(LED,0);
}

void menue_Settings2() // "weiter...":  More settings
{
  uint8_t menue=0;
  init_menue();
  lcd.print(F("Akku sparen    [ ]")); // Battery saving, not only lights out
  lcd.setCursor(2,1);
  lcd.print(F("Distanzlicht   [ ]")); // Use ultrasonic to turn on light when it's off
  lcd.setCursor(2,2);
  lcd.print(F("Equalizer & Upload")); // Equalizer menue and upload mode
  lcd.setCursor(2,3);
  lcd.print(F("Mein NOKO"));          // About NOKO menue
  while (selected!=4)
  {
    put_char(18,0,powersave? 88:32);
    put_char(18,1,distance_light? 88:32);
    put_char(0,menue,126);
    lcd.setCursor(18,menue);
    (menue<2)? lcd.blink():lcd.noBlink();  
    wait_1m(false,true);
    switch(selected) 
    {
      case 1:
        switch(menue) 
        {
          case 0: // Set power save mode
            powersave=!powersave; 
            write_EEPROM(10,powersave);
            powerdown_delay(pwd_delay);
            break;
          case 1: // Set to turn on LCD light with ultrasonic
            distance_light=!distance_light; 
            write_EEPROM(27,distance_light);
            powerdown_delay(pwd_delay);
            break;
          case 2: menue_Equalizer(); break;
          case 3: menue_NOKO(); break;
        }
        break;
      case 2:
        put_char(0,menue,32);
        if (menue<3) menue++;
        break;
      case 3:
        put_char(0,menue,32);
        if (menue>0) menue--;
        break;
    }
  }
}

void menue_Equalizer() // Set equalizer mode of MP3 module
{
   uint8_t menue=0;
   print_icon(custom_char[15]);
   lcd.blink();
   lcd.print(F("Equalizer & Upload"));
   lcd.setCursor(0,1);
   lcd.print(F("Normal  [ ] Pop  [ ]"));
   lcd.setCursor(0,2);
   lcd.print(F("Rock    [ ] Jazz [ ]"));
   lcd.setCursor(0,3);
   lcd.print(F("KlassiK [ ] Bass [ ]"));
   while (selected!=4)
   {
     put_char(equalizer%2==0? 9:18,(equalizer/2)+1,88);
     lcd.setCursor(menue%2==0? 9:18,(menue/2)+1);  
     selected=read_button(false);
     switch(selected)
     {
        case 1:
          put_char(equalizer%2==0? 9:18,(equalizer/2)+1,32);
          equalizer=menue;         
          mp3.setEqualizer(equalizer);
          write_EEPROM(1,equalizer);
          break;
        case 2: if (menue<5) menue++; break;
        case 3: if (menue>0) menue--; break;
     }
     powerdown_delay(pwd_delay);
   }
}

void menue_NOKO() // "Mein NOKO": About NOKO and secret sysinfo
{
  uint8_t help;
  print_icon(custom_char[13]);
  lcd.print(F("Dieser NOKO geh rt"));
  put_char(17,0,239);
  lcd.setCursor(0,1);
  for (help=0;help<60;help++) // Print owner and email
  {
    if (help==20) lcd.setCursor(0,2);
    if (help==40) lcd.setCursor(0,3);
    lcd.print(char(read_disc(Disc0,20+help)));   
  }
  wait_1m(false,false);
  lcd.createChar(0,custom_char[0]);
  lcd.clear();
  bignum(0,0,11); // Draw "NOKO"
  bignum(3,0,0);
  bignum(6,0,12);
  bignum(8,0,0);
  bignum(13,0,Version/10); // Draw version number: Version/10
  put_char(16,1,4);
  bignum(17,0,Version%10);
  lcd.setCursor(0,3);
  lcd.print(F(Build_by)); 
  while (selected!=4) // Secret menu, when right then left are pressed
  {
    #ifdef def_sysinfo
    selected=read_button(false);
    if (read_button(false)==2) 
      while (selected!=4) 
      {
        selected=read_button(false);
        if (read_button(false)==3) sysinfo();
      }  
    #endif
  }
}

void sysinfo() // Hidden status menu
{
  lcd.clear();
  lcd.print(F("Firmware: "));
  lcd.print(Version/10); // Firmware
  print_dot();
  lcd.print(Version%10);
  lcd.print(F(Firmware));
  lcd.setCursor(0,1);
  lcd.print(F("I2C: ")); // Show all I2C devices
  for (uint8_t help=8;help<120;help++)
  {
    Wire.beginTransmission(help);
    if (Wire.endTransmission()==0)
    {
      lcd.print(char(120));
      lcd.print(help,HEX);
    }
  }
  lcd.setCursor(0,2);
  lcd.print(F("RAM:  "));
  lcd.print(freeRam()); // Print free bytes
  lcd.print(F(" T: "));
  lcd.print(RTC.getTemp(),0); // Internal temperature
  lcd.print(char(223));
  lcd.setCursor(0,3);
  lcd.print(F("Akku:"));
  lcd.setCursor(11,3); // Print summertime/wintertime flag
  lcd.print(F("Z: "));
  (summertime)? lcd.print(char(83)):lcd.print(char(87));
  while (selected!=4)
  {
    selected=read_button(true);
    lcd.setCursor(6,3);
    lcd.print(float(analogRead(Battery)*5.0/1024.0));
    stop_delay(100);
  }      
}

void event() // Time bases events
{
  uint8_t help2,ma;
  uint8_t help=4;
  #ifdef def_external_eeprom        // Check if EEPROM is used and defined
    help=0;  
  #endif
  help2=newrandom(help,12);
  if (lcd_off) help2=10;            // If display is turned off, only start the voice event
  lcd.noBlink();
  if ((help2<8) && (lcd_dimm) && !((nightmode_on) && (night_now))) lcd.backlight();
  if (help2<5) lcd.clear();
  switch(help2)
  {
    #ifdef def_external_eeprom      // External EEPROM
      case 0: poem(); break;        // Poem "Alle Kinder..."
      case 1: swearword(); break;   // Swearword generator
      case 2: phrase(); break;      // Phrase "Wusstest Du schon..."
      case 3: quote(); break;       // Quotation - Without source. Shame on me. :-(
    #endif
    case 4:  // Ich bin hier!
      lcd.setCursor(3,1);
      lcd.print(F("Ich bin hier!"));
      analogWrite(LED,led_val*28);
      sound_on();
      play_tone(800,250,false,270);
      play_tone(800,150,false,180); 
      play_tone(660,250,false,250);
      stop_delay(2000);
      analogWrite(LED,0);   
      break;
    case 5:  // Monster party
      ma=minute();
      lcd.createChar(0,custom_char[13]);
      while ((ma==minute()) && (selected!=4))
      {  
        alarm_now=true;
        lcd.clear();
        for (help=newrandom(1,20);help>0;help--)
          put_char(newrandom(0,20),newrandom(0,4),0);
        stop_delay(500);
      }
      alarm_now=false;
      break;
    case 6:  // Scroll display
      for (help=40;help>1;help--)
      {
        lcd.scrollDisplayLeft();
        stop_delay(200);
        if (selected==4) help=1;
      }
      break;
    case 7:  // Monster cleans display
      lcd.createChar(0,custom_char[13]);
      for (ma=0;ma<4;ma++)
      {
        for (help=0;help<20;help++)
        {
          put_char(help,ma,0);
          stop_delay(150);
          if (selected==4)
          {
            ma=4;
            help=20;
          }
          put_char(help,ma,32);
        }
      }
      stop_delay(2000);
      break;
    default:
      JQ6500_play(newrandom(voice_time_start,voice_birthday)); // Voice event: NOKO says something. 
      break;
  }
  if (help2<8) 
  {
    draw_all();
    if ((lcd_dimm) && !((nightmode_on) && (night_now))) lcd.noBacklight();
  }
}

void swearword() // Swearword event on display
{
  uint16_t num;
  uint8_t gender=newrandom(0,2);
  lcd.setCursor(0,1);
  num=(newrandom(0,101)*10);
  write_swearword(num);
  if (gender==0) lcd.print("r");
  lcd.setCursor(0,2);
  num=(newrandom(0,101)*10)+1000;
  write_swearword(num);
  num=(newrandom(0,101)*10)+((gender==0)? 2000:3000);
  write_swearword(num);
  JQ6500_play(11); // NOKO laughs
  wait_1m(true,true);
}

void write_swearword(uint16_t num) // Helps swearword event
{
  uint8_t character;
  uint16_t help;
  for (help=num;help<num+10;help++)
  {
    character=read_disc(Disc1,help);
    if (character!=32) lcd.print(char(character));
  }
}

void print_block(uint16_t address, uint8_t event) // Prints text blocks from EEPROM
{
  uint8_t help;
  uint8_t y=0;
  uint8_t text_offset=80;
  uint8_t help2=newrandom(0,(event==3)? 26:100);
  if (event==1) 
  {
    y=1;
    text_offset=60;
  }
  for (help=0;help<20;help++)
  {
    put_char(help,y,(read_disc(Disc1,address+(help2*text_offset)+help)));
    put_char(help,y+1,(read_disc(Disc1,address+(help2*text_offset)+help+20)));
    put_char(help,y+2,(read_disc(Disc1,address+(help2*text_offset)+help+40)));
    if (event>1) put_char(help,3,(read_disc(Disc1,address+(help2*80)+help+60)));
  }
  wait_1m(true,true);
}

void phrase() // Text event 1: Phrase event on display 
{
  lcd.print(F("Wusstest Du, dass..."));
  print_block(phrase_address,1);
}

void quote() // Text event 2: Quotation event on display
{
  print_block(quote_address,2);
}

void poem() // Text event 3: Poem event on display
{
  sound_on();
  play_tone(880,80,false,150);
  play_tone(880,400,false,400);
  turnOff_amp; 
  print_block(poem_address,3);
}

void party() // Birthdaytime! Party! Party!
{
  uint8_t help,minute_now;
  lcd.createChar(0,custom_char[14]);
  #ifdef def_radio
    if (radio_on) Radio.powerOff();
  #endif
  if (aux_on) turnOff_aux;
  if (!mp3_on) JQ6500_play(voice_birthday); // Play birthday song 
  NewDelay(2000);
  minute_now=minute();
  while (((minute_now==minute()) || (mp3_busy)) && (selected!=4))
  {  
    lcd.clear();
    lcd.setCursor(7,1);
    lcd.print(F("Happy"));
    lcd.setCursor(6,2);
    lcd.print(F("Birthday"));
    for (help=newrandom(1,5);help>0;help--)
    {
      put_char(newrandom(0,20),0,0);
      put_char(newrandom(0,20),3,0);
    }
    stop_delay(500);
  }
  birthday_over=true;
  if (!mp3_on) mp3.pause();
  if (radio_on) radio_start();
  if (aux_on) turnOn_aux;
  draw_all();
}

void play_alarm_tone(uint8_t tone_number) // 6 alarm tunes
{
  uint8_t help;
  switch(tone_number)
  {
    case 0:
      play_tone(262,250,true,333);
      play_tone(196,125,true,167);
      play_tone(196,125,true,167);
      play_tone(220,250,true,333);
      play_tone(196,250,true,666);
      play_tone(247,250,true,333);
      play_tone(262,250,true,333);
      break;
    case 1:
      for (help=10;help>0;help--)
      {
        play_tone(440,50,true,100);
        if (selected==4) help=0;
      }
      break;
    case 2:
      play_tone(440,200,true,500);
      break;
    case 3:
      play_tone(600,100,true,100);
      play_tone(700,100,true,100);
      play_tone(800,100,true,300);
      break;
    case 4:
      play_tone(600,80,true,100);
      play_tone(400,80,true,100);
      play_tone(500,80,true,100);
      play_tone(400,80,true,200);      
      break;
    case 5:
      play_tone(600,200,true,200);
      play_tone(500,200,true,200);
      play_tone(400,200,true,300);
      break;
  }  
} 

void radio_start() // Radio on
{
  #ifdef def_radio
    Radio.powerOn();
    Radio.setChannel(radio_freq);
  #endif
}

void radio_off() // Radio off
{
  #ifdef def_radio
    Radio.powerOff();
    radio_on=false;
  #endif
}

void aux_off() // AUX relay off
{
  turnOff_aux; 
  aux_on=false;
}

void mp3_off() // Stop MP3 playing
{
  mp3.pause();  
  powerdown_delay(100);
  story_on=false;    
  file_on=false;
  mp3_on=false;
  mp3.setLoopMode(MP3_LOOP_NONE); // Change loop mode to once
}

void sound_on() // Turns on amplifier with a small delay for beep tones
{
  turnOn_amp;
  NewDelay(reaction_time);
}

void JQ6500_play(uint16_t file) // Plays MP3 number v
{
  sound_on(); // Amplifier on
  mp3.playFileByIndexNumber(file);
  if (file_on) mp3.setLoopMode(MP3_LOOP_FOLDER);
  if (story_on) mp3.setLoopMode(MP3_LOOP_NONE);  
}

int16_t freeRam() // Free RAM in bytes
{
  extern uint8_t __heap_start,*__brkval;
  int16_t v;
  return (int16_t)&v-(__brkval==0? (int16_t)&__heap_start:(int16_t)__brkval);
}

uint8_t read_disc(int8_t disc_number,uint16_t address) // Read an EEPROM
{
  Wire.beginTransmission(disc_number);
  Wire.write((uint16_t)(address >> 8));   
  Wire.write((uint16_t)(address & 0xFF)); 
  Wire.endTransmission();
  Wire.requestFrom(disc_number,1);
  return Wire.read();
}

uint8_t read_EEPROM(uint8_t address) // Read internal EEPROM with offset
{
  return EEPROM.read(address+offset);
}

void write_EEPROM(uint8_t address, uint8_t data) // Write internal EEPROM with offset
{
  EEPROM.update(address+offset,data);
}

