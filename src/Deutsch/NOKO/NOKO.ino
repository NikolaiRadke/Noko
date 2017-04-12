/* NOKO V1.0 12.04.2017 - Nikolai Radke
 *
 * Sketch for NOKO-Monster - Deutsch
 * NOTE: Does NOT run without the Si4703 Radio Module!
 * The main loop controls the timing events and gets interrupted by the taste()-funtion.
 * Otherwise NOKO falls asleep with powerdowndelay() for 120ms. This saves a lot of power.
 * 
 * Flash-Usage: 29.044 (1.8.2 | AVR Core 1.6.18 | Linux x86_64, Windows 10 | No compiler options)
 * 
 * Optional:
 * Compiler Options:   -funsafe-math-optimizations -mcall-prologues -maccumulate-args
 * (<= AVR 1.6.12)     -ffunction-sections  -fdata-sections -fmerge-constants
 * These options save flash. but are not needed since IDE 1.6.10. Optiboot is still recommended.
 * See https://github.com/NikolaiRadke/NOKO/blob/master/howto_compile/README.md
 * 
 * char()-list: 32=space 37=% 46=. 47=/ 48=0 58=: 68=D 78=N 80=P 82=R 83=S 86=V 87=W
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
 * JQ6500 Busy  23  D4    with 4,7kOhm connected to 5V to get a clear HIGH
 * JQ6500 LOUT  27  L Amp
 * JQ6500 ROUT  28  R Amp
 * ULTRA GND        GND
 * ULTRA VCC        5V
 * ULTRA PingPin    D13   (Trigger)
 * ULTRA inPin      D12   (Echo)
 * 24LC256      1   GND   1-3 must be connected to GND for the correct I2C-address
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
 * USB VCC          A7    with 1kOhm and zenerdiode
 * 
 * Unused           A1.A2,A3,D8,D9,(ICSP)
 * 
 * I2C address list:
 * SI4703           0x10 (Radio)
 * LCD              0x27 (Yes it's the LCD. Some Displays have 0x3F!)
 * 24LC256          0x50 (EEPROM 32kb)
 * AT24C32          0x57 (EEPROM 4kb on RTC module)
 * DS3231           0x68 (RTC chip on RTC module)
*/

// Softwareversion
#define Firmware "-120417"
#define Version 10  // 1.0
#define Build_by "by Nikolai Radke" // Your Name. Max. 20 chars, appears in "Mein NOKO" menu

// Features on/off - comment out to disable
#define def_radio             // Using Radio?
#define def_external_eeprom   // Using external EEPROM?
#define def_stories           // Stories on SD card?

// Display selection
//#define display_address 0x27   // Blue 
#define display_address 0x3F // Yellow

// 24LC256 EEPROM addresses
#define phrase_address 4000    // Starting address of the phrases in 24LC256
#define quote_address 10000    // Quotations
#define poem_address  18000    // Poems

// Timezone GMT+1
#define ZZ 1 

// Developer options
#define pwd_delay     50  // Button debounce
#define reaktionszeit 70  // Startup time for the amplifier
#define sensor        25  // Ultrasonic: with cover 10, without 25
#define vol_mp3       30  // JQ6500 volume 0-30
#define vol_radio     10  // Si4703 volume 0-15

// Battery calculation
#define minV          2.85
#define maxV          4.15

// Hardwareaddresses PINS
#define LED 10            // LED -> PWM
#define inPin 12          // Ultrasonic
#define Akku 6            // Battery power
#define USB 7             // Is NOKO connected to USB?
#define Speaker 11        // PWM beep 
#define Tasten A0         // ADC buttons
#define Disk0 0x57        // AH24C32 4 kByte EEPROM
#define Disk1 0x50        // 24LC256 32 kByte EEPROM

// Libraries
// I2CIO.h and LCD.h are used by LiquidCrystal_I2C.h
#include <avr/power.h>
#include <avr/wdt.h>
#include <avr/sleep.h>
#include <Wire.h> 
#include <EEPROM.h>
#include "Time.h"
#include "LiquidCrystal_I2C.h"
#include "Si4703.h"
#include "DS3231RTC.h"
#include "NewTone.h"
#include "JQ6500_Serial.h"

SIGNAL(WDT_vect) // Watchdog to wake NOKO from powerdowndelay()
{
  wdt_disable();
  wdt_reset();
  WDTCSR &= ~_BV(WDIE);
}

uint8_t  wahl=0;              // Which menu item selected
uint8_t  xlcd=0;              // X and
uint8_t  ylcd=0;              // Y position of LCD cursor
uint8_t  geschichte=1;        // Selected story
uint8_t  geschichten;         // Number of stories
uint8_t  eigenes=1;           // Selected MP3
uint8_t  files;               // Number of own MP3s files on SD card
uint8_t  led_dimm;            // LED brightness 0-9
uint8_t  gt,gm;               // Birthday gt=day, gm=month
uint8_t  klang,klang_ton,klang_mp3;// Alarm: Which type, which tone or mp3
uint8_t  alarmmm,alarmhh;     // Alarm time hh:mm
uint8_t  nachtahh,nachtbhh;   // Nachtmodus from hh:mm
uint8_t  nachtamm,nachtbmm;   // to hh:mm
uint8_t  ultra_distanz;       // Ultrasonic reaction distance 0-9 
uint8_t  eventsteller;        // Chance of time event 0-9

boolean  ultra_event=false;   // Has there been am ultrasonic event this minute?
boolean  ultra_light;         // Swich on LCD light with ultrasonic?
boolean  radio=false;         // Radio on?
boolean  pause=false;         // MP3 pause?
boolean  geschichte_an=false; // Is there a story running?
boolean  eigenes_an=false;    // Is there an own mp3 running?
boolean  mp3_an=false;        // Is the MP3-Module itself running anything?
boolean  geburtstag=false;    // Is it your birthday today?
boolean  gefeiert=false;      // Was it already celebrated?
boolean  alarm_mute=false;    // No alarm here?
boolean  powersave;           // Powersavemode?
boolean  lcddimm=false;       // Only dim the display?
boolean  dimm=false;          // Is powersave active?
boolean  ultra_dimm=false;    // Ultrasonic off?
boolean  alarm_jetzt=false;   // Is the alarm running?
boolean  aux=false;           // Is AUX active?
boolean  sommer;              // Summertime?
boolean  alarm_an;            // Is the Alarm set?
boolean  nacht;               // Is it nighttime? (Time of Nachtmodus)
boolean  nachtwechsel;        // Has the nightmode switched?
boolean  nachtmodus;          // Is nightmode set?
boolean  nachtdimm;           // Will the display mute at nighttime?
boolean  stumm;               // All sound off?
boolean  laden;               // Is NOKO's battery charging?

uint16_t freq;                // Used frequency * 100
uint16_t station[3];          // 3 radiostations

int16_t   j;                  // Year is integer type
int16_t   power;              // Voltage * 100
uint32_t  dimmmillis;         // Milliseconds until display mutes

const uint8_t eventtrigger[10]={0,120,60,45,30,15,10,5,3,1};

// Custom characters. Number set was made by Ishan Karve. Awesome!
PROGMEM const char custom_char[18][8]=
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
  {B00100,B01110,B01110,B01110,B11111,B00000,B00100,B00000}  // Bell
};

tmElements_t tm;

#ifdef def_radio
  Si4703 Radio(5,A4,A5);  // RST, SDA, SCL
#endif
LiquidCrystal_I2C lcd(display_address,2,1,0,4,5,6,7,3,POSITIVE);
JQ6500_Serial mp3(2,3); // Software serial connection

//-------------------------------------------------------------------------
// Using main() and init() instead of setup() and loop() saves flash space

int main(void) {  
uint8_t ma=99;
uint8_t ha=99;
uint8_t help;
init();
{  
  // Powersafe options
  SPCR=0;                // Disable SPI
  ACSR=B10000000;        // Disable analog comparator
  DIDR0=0x3F;            // Disable digital input buffer on all analog ports
  power_spi_disable();       
  power_usart0_disable(); 
  power_timer2_disable();

  // Portdefinitions - direct manipulation is much faster and saves flash
  DDRD=B11001000;   // D7-D0 | 1=OUTPUT
  DDRB=B00101100;   // D13-D8
  DDRC=B00111110;   // A7-A0 | Set unused analog pins to output to prevent catching noise from open ports
  PORTD=B01000000;  // D6 MOSFET HIGH: Turn off amplifier to prevent startup noise
  //PORTB=B00000000; 
  PORTC=B00000001;  // A0: INPUT_PULLUP 

  // Start JQ6500
  mp3.begin(9600);
  mp3.pause();
  mp3.reset();
  NewDelay(500);
  mp3.setVolume(vol_mp3);
  mp3.setLoopMode(MP3_LOOP_NONE); // Play only once
  
  // Start Radio
  #ifdef def_radio
    Radio.powerOn();  // Needs to start once!
    Radio.setVolume(vol_radio);
    Radio.powerOff();
  #endif
 
  // Read Arduino EEPROM
  stumm=EEPROM.read(1); 
  led_dimm=EEPROM.read(2);
  alarm_an=EEPROM.read(3);
  alarmmm=EEPROM.read(4);
  alarmhh=EEPROM.read(5);
  klang=EEPROM.read(6);
  klang_ton=EEPROM.read(7);
  ultra_distanz=EEPROM.read(8);
  eventsteller=EEPROM.read(9);
  powersave=EEPROM.read(10);
  nachtmodus=EEPROM.read(11);
  nachtahh=EEPROM.read(12);
  nachtamm=EEPROM.read(13);
  nachtbhh=EEPROM.read(14);
  nachtbmm=EEPROM.read(15);
  sommer=EEPROM.read(16);
  freq=(EEPROM.read(17)*10)+(EEPROM.read(18));
  klang_mp3=EEPROM.read(19);
  for (help=0;help<3;help++)
    station[help]=(EEPROM.read(21+(help*2))*10)+(EEPROM.read(22+(help*2)));
  ultra_light=EEPROM.read(27);
  nachtdimm=EEPROM.read(28);

  //  Read AT24C32 
  gt=readDisk(Disk0,0);          // Birthday day
  gm=readDisk(Disk0,1);          // Birthday month
  #ifdef def_stories
    geschichten=readDisk(Disk0,2); // Number of stories
  #else
    geschichten=0;
  #endif
  
  // Start RTC and switch off useless functions for power saving
  Wire.beginTransmission(0x68); 
  Wire.write("\x0F\xF7");       // 32kHz off         
  Wire.endTransmission();
  powerdowndelay(20);
  Wire.beginTransmission(0x68);
  Wire.write("\x0E\x7F");       // SQW off
  Wire.endTransmission();
  echtzeit();                   // Read RTC
  
  // Start LCD and create custom characters 
  lcd.begin(20,4);    
  init_char();

  //  Check for update and compare with internal version number
  help=EEPROM.read(20);
  if (Version>help) // New version > internal version?
  {
    lcd.clear();
    lcd.print(F(" Update erfolgreich"));
    zeichen(3,1,86);
    lcd.print(help/10);
    lcd.print(char(46));
    lcd.print(help%10);
    lcd.print(F(" auf V"));
    lcd.print(Version/10);
    lcd.print(char(46));
    lcd.print(Version%10);
    lcd.setCursor(4,4);
    lcd.print(F("Nase dr cken"));
    zeichen(11,4,245);
    EEPROM.write(20,Version); // Write new internal version
    schleife(true,false);     // Wait for button
    lcd.clear();
  }

  // Starting sound
  sound_an();
  ton(600,80,false,80);
  ton(880,150,false,150);
  PORTD |= (1<<6); // Amplifier HIGH=off
  
  zeit(); // Read RTC
  nachtwechsel=nachtjetzt(); // Has nightmode switched before startup?

  files=mp3.countFiles(MP3_SRC_SDCARD)-(111+geschichten); // Number own of files on SD card
}

// MAIN LOOP
while(1)
{
  zeit();
  if (((analogRead(USB)>800) && (!laden)) || ((analogRead(USB)<800) && (laden))) // USB connected?
  {
    laden=!laden;
    if ((analogRead(USB)<800) && (lcddimm) && (powersave)) powerdown();
    else if ((analogRead(USB)>800) && (dimm)) powerup();
    else uhrzeit();
  } 
  //if ((dimm) && (!radio) && (!mp3_an) && (!aux) && (!(PIND & (1<<4)))) powerdown(); 
                                             // Power down while MP3 is running und NOKO is set to mute
// Every minute                                             
  if (ma!=tm.Minute)                        
  { 
    if ((nacht) && (nachtdimm) && (!dimm))   // Power down at nightmode
    {
      if ((powersave) && (!laden)) powerdown();
      else if (!lcddimm)
      {
        lcd.noBacklight();
        lcddimm=true;
      }
    }
    ultra_event=false;

// Every hour
    if (ha!=tm.Hour)                          
    {
      if (!dimm) 
      {    
        datum();                              // Draw date
        if ((gm==tm.Month) && (gt==tm.Day))   // Check for birthday
        {
          geburtstag=true;
          if ((!gefeiert) && (!nacht)) feiern();
        }
        else 
        {
          gefeiert=false;
          geburtstag=false;
        }                   
      }
      check_sommerzeit();                     // Check for summertime
    }
    if (!dimm) uhrzeit();                     // Draw clock
    if (!(nacht) && (!stumm)) 
    {
      if ((power<10) && (!laden))             // Read battery power
      {
        sound_an();
        ton(920,80,false,80);                 // Power <10% beep
      }
      if ((!(PIND & (1<<4))) && (!pause) && (!radio) && (power>1) && ((newrandom(1,eventtrigger[eventsteller]+1)==eventtrigger[eventsteller])))
        event();                              // Time based event
    }
  }
  if ((!dimm) && (!ultra_dimm) && (ultra_distanz>0)) check_ultra(); 
                                              // Ultrasonic event
  if ((lcddimm) && (ultra_light) && (millis()>dimmmillis+150)) lcd.noBacklight();  
                                              // Switch off display light after ~1s. 150 is a workaround for stupid IDLE mode.
  ma=tm.Minute;
  ha=tm.Hour;
  wahl=taste(false);                          // Read buttons
  switch(wahl)
  {
    case 1:                                   // Belly: menue
      if (!dimm) menue_Hauptmenue(); 
      break;
    case 2:                                   // Right: toggle ultrasonic
      if ((!dimm) && (!lcddimm) && (ultra_distanz>0)) 
      { 
        ultra_dimm=!ultra_dimm;
        uhrzeit();
      }
      break;
    case 3:                                   // Left: toggle display & power save
      if (dimm) powerup();
      else if ((powersave) && (!laden)) powerdown();
      else
      {
        lcddimm=!lcddimm;
        uhrzeit();
        if (!lcddimm)
        {
          analogWrite(LED,0);
          lcd.backlight();  
        }
        else 
        {
          analogWrite(LED,led_dimm*28);
          lcd.noBacklight();
        }
        powerdowndelay(pwd_delay);
      }
      break;
    //case 4: break; // Nose unused - only voice as random event
    }
    powerdowndelay(dimm? 240:120); // Main loop power save!    
}}

//-------------------------------------------------------------------------------------

uint8_t taste(boolean leise)  // Read pressed button und debounce | leise = NOKO stays silent
{
  uint16_t tastenwert;
  if ((!pause) && (!(PIND & (1<<4))) && (mp3_an)) mp3_an=false;
  if (power<5) leise=true;
  if ((!(PIND & (1<<4))) && (!radio) && (!aux) && (!alarm_jetzt)) 
  {
    PORTD |= (1<<6); // Amplifier off
    echtzeit();      // Read RTC - powerdowndelay() messes up the internal time!
  }
  if (check_alarm()) return 4; // Check alarm
  tastenwert=(analogRead(Tasten));
  if (tastenwert>200) return 0;
  else 
  {
    if (tastenwert>150)    // SW4 nose -> voice 31-60
    {
      powerdowndelay(pwd_delay);
      if (analogRead(Tasten)>150)
      {
       if ((!(PIND & (1<<4))) && (!stumm) && (!leise) && (!pause))
         if (newrandom(1,5)==4) JQ6500_play(newrandom(31,61));
       return 4;
      }
    }
    else if (tastenwert>100)    // SW3 left hand
    { 
      powerdowndelay(pwd_delay);
      if (analogRead(Tasten)>100) return 3;
    }
    else if ((tastenwert>50))   // SW2 right hand
    {
      powerdowndelay(pwd_delay);
      if (analogRead(Tasten)>50)  return 2;
    }
    else                        // SW1 belly -> voice 11-30
    {
      powerdowndelay(pwd_delay);
      if (analogRead(Tasten)>0)
      {
        if ((!(PIND & (1<<4))) && (!stumm) && (!leise) && (!pause))
          if (newrandom(1,8)==4) JQ6500_play(newrandom(11,31)); 
      return 1;
      }
    }
  }
}

// Constantly used functions - saves a lot of flash.
void zeichen(uint8_t x,uint8_t y,uint8_t c) // Char c at x,y
{
  lcd.setCursor(x,y);
  lcd.print(char(c));
}

void leer(uint8_t x,uint8_t y,uint8_t anz) // anz spaces from x,y
{
  uint8_t a;
  lcd.setCursor(x,y);
  for (a=anz;a>0;a--) lcd.print(char(32));
}

void icon(uint8_t c[]) // Icon c at 0,0
{
  lcd.createChar(0,c);
  lcd.clear();
  lcd.print(char(0));
  lcd.print(char(32));
}

// Plays tone with delay. Delay can be stopped by pressing SW4 when stopd is set TRUE
void ton(uint16_t h,uint16_t l,boolean stopd,uint16_t d) 
{
  NewTone(Speaker,h,l);
  if (!stopd) NewDelay(d);
  else if (wahl!=4) stopdelay(d);
}

// Constantly used strings. Saves a lot of flash space. 
void null()  {lcd.print(char(48));} // Prints a "0" - saves flash. However.

void uhr()   {lcd.print(F(" Uhr"));} // Prints " Uhr"
 
void hoeren() // Prints "hören"
{
  lcd.print(char(239));
  lcd.print(F("ren")); 
}

void zeige_speichern() // Prints "speichern..."
{
  lcd.setCursor(1,1);
  lcd.print(F("speichern..."));
  powerdowndelay(pwd_delay);
}

void init_char() // Read custom chars again
{
  for (uint8_t help=0;help<8;help++)
    lcd.createChar(help,custom_char[help]);
}

void clearnum(uint8_t x,uint8_t y) // Delete a big number at x,y
{
  xlcd=x;
  ylcd=y;
  leer(xlcd,ylcd,3);
  leer(xlcd,ylcd+1,3);
}

void bignum(uint8_t x,uint8_t y,uint8_t num) // Draws a big number at x,y; num 0-9
{
  xlcd=x;
  ylcd=y;
  clearnum(x,y); // Clear the space
  switch(num) 
  {
    case 0: 
      zeichen(xlcd,ylcd,0); 
      lcd.print(char(1));  
      lcd.print(char(2));
      zeichen(xlcd,ylcd+1,3); 
      lcd.print(char(4));
      lcd.print(char(5));
      break;
    case 1: 
      zeichen(xlcd,ylcd,1);
      lcd.print(char(2));
      zeichen(xlcd,ylcd+1,4);
      lcd.print(char(7));
      lcd.print(char(4));
      break;
    case 2: 
      zeichen(xlcd,ylcd,6);
      lcd.print(char(6));
      lcd.print(char(2));
      zeichen(xlcd,ylcd+1,3);
      lcd.print(char(4));
      lcd.print(char(4));
      break;
    case 3:
      zeichen(xlcd,ylcd,6);
      lcd.print(char(6));
      lcd.print(char(2));
      zeichen(xlcd,ylcd+1,4);
      lcd.print(char(4));
      lcd.print(char(5));
      break;
    case 4:
      zeichen(xlcd,ylcd,3);
      lcd.print(char(4));
      lcd.print(char(7));
      zeichen(xlcd+2,ylcd+1,7);
      break;
    case 5:
      zeichen(xlcd,ylcd,3);
      lcd.print(char(6));
      lcd.print(char(6));
      zeichen(xlcd,ylcd+1,4);
      lcd.print(char(4));
      lcd.print(char(5));
      break;
    case 6: 
      zeichen(xlcd,ylcd,0);
      lcd.print(char(6));
      lcd.print(char(6));
      zeichen(xlcd,ylcd+1,3);
      lcd.print(char(4));
      lcd.print(char(5));
      break;
    case 7:
      zeichen(xlcd,ylcd,1);
      lcd.print(char(1));
      lcd.print(char(2));
      zeichen(xlcd+2,ylcd+1,7);
      break;
    case 8: 
      zeichen(xlcd,ylcd,0);
      lcd.print(char(6));
      lcd.print(char(2));
      zeichen(xlcd,ylcd+1,3);
      lcd.print(char(4));
      lcd.print(char(5));
      break;
    case 9: 
      zeichen(xlcd,ylcd,0);
      lcd.print(char(6));
      lcd.print(char(2));
      zeichen(xlcd+2,ylcd+1,7);
      break;
    case 10:
      zeichen(xlcd+1,ylcd,4);
      zeichen(xlcd+1,ylcd+1,4);
    break;
    case 11: 
      zeichen(xlcd,ylcd,0);
      lcd.print(char(2));
      lcd.print(char(0));
      zeichen(xlcd,ylcd+1,7);
      lcd.print(char(3));
      lcd.print(char(7));
      break;
    case 12: 
      zeichen(xlcd,ylcd,7);
      lcd.print(char(0));
      zeichen(xlcd,ylcd+1,7);
      lcd.print(char(3));
      break;
  }    
}

void powerdown() // power save on
{
  analogWrite(LED,0);
  lcd.off();        // Turn off display
  dimm=true;  
  lcddimm=false;
  // More powersaving options?
}

void powerup()  // power save off
{
  lcd.on();           // Turn on display  
  dimm=false;
  lcddimm=false;
  anzeigen();         // Draw clock and date
}

uint8_t newrandom(uint8_t a,uint8_t b) // Better XOR random number generator 
{
    uint16_t rnd;
    rnd+=micros();
    rnd^=rnd<<2;rnd^=rnd>>7;rnd^=rnd<<7;
    return (rnd/65535.0)*(b-a)+a;
}

void NewDelay(uint16_t z)  // New delay function to save flash - it's also in the Radio library
{
  #ifdef def_radio 
    Radio.newdelay(z); 
  #else 
    uint32_t zmillis=millis();
    while (millis()-zmillis<z);
  #endif
}

void stopdelay(uint16_t z) // Delay with stop when nose is pressed
{
  uint32_t zmillis=millis();
  while ((millis()-zmillis<z) && (taste(true)!=4));
  if (taste(true)==4) wahl=4;
}

void schleife(boolean leise,boolean timeout) // Wait for nose until timeout 1 minute
{
  uint32_t ende=millis()+60000; // 2 MHz -> 60s:8
  wahl=0;
  while (wahl==0) 
  {
    wahl=taste(leise);
    if (!timeout) powerdowndelay(120);
    else if (millis()>ende) wahl=4;
  }
  powerdowndelay(pwd_delay);
}

void schlafe(uint8_t wdt_time) // Sleepmode to save power
{
  wdt_enable(wdt_time); // Watchdog wakes NOKO after wdt_time
  wdt_reset();
  WDTCSR |= _BV(WDIE);
  if (lcddimm) set_sleep_mode(SLEEP_MODE_IDLE);        // Lowest level for the LED
  else if (dimm) set_sleep_mode(SLEEP_MODE_PWR_DOWN);  // Highest level
  else set_sleep_mode(SLEEP_MODE_STANDBY);             // Lower level to wake up faster
  sleep_enable();
  sleep_bod_disable();                                 // No brown-out detection
  power_adc_disable();                                 // No need for ADC during sleep
  sleep_cpu ();                                        // power down !
  wdt_disable();
  WDTCSR &= ~_BV(WDIE);
}

void powerdowndelay(uint8_t ms) // Calls schlafe() with watchdog-times
// Sleep times steps are pre-defined, max 8s
{
  if (lcddimm)  PRR = PRR | 0b00100000;           // Workaround. Not sure if IDLE is working...
  // if (ms>=256) {schlafe(WDTO_250MS); ms-=250;} // NOKO uses max 240ms
  if (ms>=128) {schlafe(WDTO_120MS); ms-=120;}
  if (ms>=64) {schlafe(WDTO_60MS); ms-=60;} 
  if (ms>=32) {schlafe(WDTO_30MS); ms-=30;}
  if (ms>=16) {schlafe(WDTO_15MS); ms-=15;}
  if (lcddimm) PRR = PRR & 0b00000000;            // End workaround. Stupid stupid Atmel!
}

void echtzeit() // Read RTC and store time
{
  RTC.read(tm);
  setTime(tm.Hour,tm.Minute,tm.Second,tm.Day,tm.Month,tmYearToCalendar(tm.Year));
}

boolean sommerzeit() // Summertime?
{
  if ((tm.Month<3) || (tm.Month>10)) return false;
  if ((tm.Month>3) && (tm.Month<10)) return true;
  if ((tm.Month==3) && (tm.Hour+24*tm.Day)>=(1+ZZ+24*(31-(5*j/4+4)%7)) || tm.Month==10 && (tm.Hour+24*tm.Day)<(1+ZZ+24*(31-(5*j/4+1)%7))) return true;
  return false;
}

void check_sommerzeit() // Compare summertime with EEPROM and set clock
{
  if (((sommer!=sommerzeit()) && (tm.Hour>=4))) // Summertime changed?
  {
    (sommer)? tm.Hour--:tm.Hour++;  // Add or remove an hour
    sommer=!sommer;
    setTime(tm.Hour,tm.Minute,tm.Second,tm.Day,tm.Month,j);
    tm.Year=CalendarYrToTm(j);
    RTC.write(tm);                  // Write RTC
    EEPROM.write(16,sommer);        // Write summertime flag
  }
}

void uhrzeit() // Draw clock, power level and flags
{
  uint8_t help;
  zeit();
  nacht=nachtjetzt();
  (tm.Hour<10)? bignum(1,0,0):bignum(1,0,tm.Hour/10);
  bignum(4,0,tm.Hour%10);
  bignum(7,0,10);
  (tm.Minute<10)? bignum(10,0,0):bignum(10,0,tm.Minute/10);
  bignum(13,0,tm.Minute%10);
  leer(17,1,3);
  lcd.setCursor(17,1);
  //--- Flags
  if ((ultra_dimm) || (ultra_distanz==0)) lcd.print(char(120));
  if (lcddimm) lcd.print(char(68));
  if (nachtmodus) (nacht)? lcd.print(char(78)):lcd.print(char(110));
  if (geburtstag)
  {
    lcd.setCursor(3,2);
    lcd.print(F("Geburtstag!"));
  }
  leer(17,2,3);
  lcd.setCursor(17,2);
  if (alarm_an) lcd.print(char(244));
  if (pause) lcd.print(char(112));
  if (stumm) lcd.print(char(83));
  if ((mp3_an) || (aux) || (radio)) lcd.print(char(80));
  //--- power level and charging
  power=analogRead(Akku); // Calculate power level from 5 measurements
  for (help=4;help>0;help--)
  {
    powerdowndelay(10);
    power+=analogRead(Akku);
  }
  // Voltage: maxV to maxV
  power=(uint16_t)(((power/5)*(5.0/1024)-minV)/((maxV-minV)/100)); 
  power=constrain(power,1,99);
  // >57% Voltage are about 92% of overall capacity. Drops at 3,63V
  if (power>57) power=map(power,57,99,8,99); 
  else power=map(power,1,56,1,8);
  lcd.setCursor(17,0);
  if (power<10) 
  {
    zeichen(17,0,32);
    lcd.blink(); // Blink when below 10%
  }
  else lcd.noBlink();
  lcd.print(power);
  laden? lcd.print(char(43)):lcd.print(char(37)); // Charging: print + or %
  lcd.setCursor(18,0);
}

void datum() // Draw date
{
  j=year();
  tm.Day=day();
  tm.Month=month();
  lcd.setCursor(0,3);
  switch(weekday()) // German day strings
  {
    case 1: lcd.print(F("Sonntag")); break;
    case 2: lcd.print(F("Montag")); break;
    case 3: lcd.print(F("Dienstag")); break;
    case 4: lcd.print(F("Mittwoch")); break;
    case 5: lcd.print(F("Donn.tag")); break;
    case 6: lcd.print(F("Freitag")); break;
    case 7: lcd.print(F("Samstag")); break;
    
  }
  lcd.setCursor(10,3);
  if (tm.Day<10) null();
  lcd.print(tm.Day);
  lcd.print(char(46));
  if (tm.Month<10) null();
  lcd.print(tm.Month);
  lcd.print(char(46));
  lcd.print(j);
  if (power<10) lcd.setCursor(18,0);
}

void anzeigen() // Draw the entire display new
{
  lcd.clear();
  init_char();
  datum();
  uhrzeit();
}

void zeit() // Read internal time
{
  tm.Minute=minute();
  tm.Hour=hour();
}

void zeige_station(uint8_t s) // Print the stations frequencies
{
  lcd.setCursor(1,1);
  lcd.print(F("Station "));
  lcd.print(s);
  lcd.print(char(58));
  lcd.print(char(32));
  lcd.print(float(station[s-1])/10,2);
}

boolean nachtjetzt() // Is it nighttime?
{
  boolean ja=false;
  if (nachtmodus)
  {
    if (nachtahh>nachtbhh)
      if ((tm.Hour>nachtahh) || (tm.Hour<nachtbhh)) ja=true;
    if (nachtahh<nachtbhh)
      if ((tm.Hour>nachtahh) && (tm.Hour<nachtbhh)) ja=true;
    if (nachtahh==nachtbhh)
    {
      if (nachtamm<=nachtbmm)
        if ((tm.Minute>=nachtamm) && (tm.Minute<=nachtbmm)) ja=true;
      if (nachtahh>nachtbmm)
        if ((tm.Minute<=nachtbmm) || (tm.Minute>=nachtbmm)) ja=true;
    }
    if ((tm.Hour==nachtahh) && (tm.Minute>=nachtamm)) ja=true;
    if ((tm.Hour==nachtbhh) && (tm.Minute<nachtbmm)) ja=true;
  }
  if (ja!=nachtwechsel) // When nightmode changes play sound
  {
    ja? JQ6500_play(71):JQ6500_play(102);
    nachtwechsel=ja;
  }
  return ja;
}

void check_ultra() // Ultrasonic event - ultra_distanz= 0..9 * 10cm
{
  PORTB &= ~(1<<5); // (pingPin,LOW);
  delayMicroseconds(5);
  PORTB |= (1<<5);  // (pingPin,HIGH);
  delayMicroseconds(10);
  PORTB &= ~(1<<5); // (pingPin,LOW);
  if ((pulseIn(inPin,HIGH) /29 /2)<ultra_distanz*sensor)  // Is there something in distance?
  {
    if ((lcddimm) && (ultra_light)) // Display light off and light flag set?
    {
      lcd.backlight();              // Turn on light and set timer
      dimmmillis=millis();
    }
    if ((!nacht) && (!ultra_event) && (!radio) && (!pause) && (!stumm) && (!(PIND & (1<<4))))
    {
      JQ6500_play(newrandom(61,81)); // Play random voice event. Voice 61-80
      ultra_event=true; // Set ultra_evet flag to prevent more events than one in a minute
    }
  }
}  

boolean check_alarm() // Is it time for the alarm?
{
  if ((alarm_an) && (!alarm_jetzt))  // Prevent double check
  {
    zeit();                          // Check RTC
    if ((!alarm_mute) && (alarm_an)) // Is alarm allowed?
      if ((alarmmm==tm.Minute) && (alarmhh==tm.Hour)) 
      {
        alarm();                     // Yes! Alarm!
        return true;
      }
    if ((alarm_an) && (alarmmm!=tm.Minute)) alarm_mute=false; // Other time - mute off
  }
  return false;
}

void alarm() // Play alarm
{
  if (dimm) lcd.on();           // Display on
  if (lcddimm) lcd.backlight(); // Light on
  PORTD &= ~(1<<6);             // Amplifier on
  lcd.createChar(0,custom_char[17]);
  lcd.clear();
  lcd.noBlink();
  lcd.setCursor(7,0);
  if (tm.Hour<10) null();
  lcd.print(tm.Hour);
  lcd.print(char(58));
  if (tm.Minute<10) null();
  lcd.print(tm.Minute);
  lcd.setCursor(4,3);
  lcd.print(F("Nase dr cken"));
  zeichen(11,3,245);
  alarm_jetzt=true;
  if (aux) PORTD &= ~(1<<7);                // AUX off
  if ((klang==1) && (!radio) && (!mp3_an))  // Radio alarm
  {
      radio_ein();
      radio=true;
  }
  wahl=0;
  while ((wahl!=4) && (alarmmm==tm.Minute)) // Alarm until one minute timeout or Nose 
  {
    zeichen(5,2,0);
    lcd.print(F(" Alarm "));
    lcd.print(char(0));
    wahl=taste(true);
    analogWrite(LED,255);   
    if ((klang==0) || (mp3_an) || (pause))             // Tone alarm
      alarmton(klang_ton); 
    if ((klang==2) && (!(PIND & (1<<4))) && (!mp3_an)) // MP3 alarm
      JQ6500_play(klang_mp3+1); 
    stopdelay(500);
    leer(5,2,9); // Clear "Alarm" to let it blink
    analogWrite(LED,0); // Let the LED blink
    stopdelay(500); // Is nose pressed? Wait 500ms else set wahl=4
    tm.Minute=minute();
  }
  if (!mp3_an) mp3.pause();
  else if (klang==1) 
    #ifdef def_radio
      Radio.powerOff(); 
    #endif
  if (aux) PORTD |= (1<<7); // AUX on;
  alarm_jetzt=false;
  if ((alarmmm==tm.Minute) && (alarmhh==tm.Hour)) alarm_mute=true;
  lcd.clear();
  if ((dimm) && (!radio) && (!(PIND & (1<<4)))) PORTD |= (1<<6); // Amplifier off
  if (dimm) lcd.off();
  if (lcddimm) lcd.noBacklight();
  anzeigen();
}

//--- MENUES

void menue_Hauptmenue() // Main menue
{
  uint8_t menue=0; // Selected menue number 0..3
  uint8_t help=0;
  powerdowndelay(80); // Debounce
  lcd.noBlink();
  lcd.clear();
  if (stumm) // Sound off? Disable play menue
  {
    lcd.print(char(120));
    menue=1;
    help=1;
  }    
  lcd.setCursor(2,0);
  lcd.print(F("Spiel was vor")); // Play menue
  lcd.setCursor(2,1);
  lcd.print(F("Alarm stellen")); // Alarm menue
  lcd.setCursor(1,2);
  uhr();
  lcd.print(F("zeit stellen"));  // Time and nightmode menue
  lcd.setCursor(2,3);
  lcd.print(F("NOKO stellen"));  // Settings menue
  while (wahl!=4)
  {
    zeichen(0,menue,126);
    schleife(false,true);
    switch(wahl) 
    {
      case 1:
        switch(menue) 
        {
          case 0: menue_Abspielen(); break;
          case 1: menue_Alarm(); break;
          case 2: menue_Uhrzeit(); break;
          case 3: menue_Einstellungen(); break;
        }
        break;
      case 2:
        zeichen(0,menue,32);
        if (menue<3) menue++;
        break;
      case 3:
        zeichen(0,menue,32);
        if (menue>help) menue--;
        break;
    }
  }
  anzeigen();
}

void menue_Abspielen() // Play menue "Spiel was vor"
{
  uint8_t menue=0;
  lcd.createChar(0,custom_char[15]);
  lcd.clear();
  lcd.setCursor(2,0);
  lcd.print(F("Radio h"));           // Radio menue
  hoeren();
  lcd.setCursor(2,1);                // MP3 menue
  lcd.print(F("Eigenes h"));  
  hoeren();
  lcd.setCursor(2,2);                // Toggle AUX
  lcd.print(F("AUX-Eingang"));
  lcd.setCursor(2,3);                // Story menue
  lcd.print(F("Geschichten h"));     
  hoeren();
  while (wahl!=4)
  {
    #ifndef def_radio                // Is the radio available?
      zeichen(1,0,120);
      #else
      zeichen(1,0,radio? 0:32);      // Ternary saves flash! Well. Sometimes.
    #endif   
    zeichen(1,1,eigenes_an? 0:32);
    zeichen(1,2,aux? 0:32); 
    #ifndef def_stories              // Are there stories on SD card?
      zeichen(1,3,120);
      #else
      zeichen(1,3, geschichte_an? 0:32); 
    #endif
    zeichen(0,menue,126);
    schleife(true,true);
    switch(wahl) 
    {
      case 1: 
        switch(menue)
        {
          case 0: menue_Radio(); break;
          case 1: (files>0)? menue_mp3(2):menue_mp3(3); break;
          case 2: 
            if (!aux)
            {
              if (radio) radio_aus();
              if ((!pause) && (PIND & (1<<4))) mp3_aus();
              PORTD &= ~(1<<6);   // Amplifier on
              PORTD |= (1<<7);    // AUX on;
            }
            else
            {
              PORTD |= (1<<6);    // Amplifier off
              PORTD &= ~(1<<7);   // AUX off
            }
            aux=!aux;
            NewDelay(500);
            break;
          #ifdef def_stories
            case 3: menue_mp3(1); break;
          #endif         
        }
        break;
      case 2:
        zeichen(0,menue,32);
        if (menue<3) menue++;
        break;
      case 3:
        zeichen(0,menue,32);
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
  boolean save2=false;
  boolean save=false;
  boolean rds=false;
  uint8_t menue=2;
  uint8_t help,help2,count;
  lcd.createChar(1,custom_char[9]);
  lcd.createChar(2,custom_char[11]);
  lcd.createChar(3,custom_char[12]);
  lcd.createChar(4,custom_char[8]);
  icon(custom_char[15]);
  if (radio) Radio.readRDS(rdsStation,1000);
  while (wahl!=4)
  {
    zeichen(1,2,91);
    lcd.print(char(2));
    lcd.print(F("][<][ ][ ][>]["));
    lcd.print(char(3));
    lcd.print(char(93));
    lcd.setCursor(1,3);
    lcd.print(F("[1|s] [2|s] [3|s] "));
    leer(1,1,19);
    lcd.blink();
    zeichen(8,2,radio? 1:4);
    zeichen(11,2,radio? 82:32);
    lcd.setCursor(2,0);
    lcd.print(float(freq)/10,2);
    lcd.print(char(32));
    if (save) 
    {
      PORTD &= ~(1<<6); // Amplifier on
      radio_ein();
      leer(9,0,10);
      Radio.readRDS(rdsStation,1000); // Read station name, timeout 1000ms
      save=false;
    }
    lcd.setCursor(9,0);
    if (radio) lcd.print(rdsStation);
    switch (menue)
    {
       case 6: 
         zeige_station(1);
         lcd.setCursor(2,3);
         break;
       case 7: lcd.setCursor(4,3); break;
       case 8: 
         zeige_station(2);
         lcd.setCursor(8,3);
         break;
       case 9: lcd.setCursor(10,3); break;
       case 10: 
         zeige_station(3);
         lcd.setCursor(14,3);
         break;
       case 11: lcd.setCursor(16,3); break;
       default: lcd.setCursor(2+(menue*3),2); break;
       
    }
    if (rds) // RDS text loop - no multitasking... Press left to exit
    {
      wahl=0;
      for (help=0;help<64;help++) rdsText[help]=char(32); 
      lcd.noBlink();
      leer(0,2,20);
      lcd.setCursor(1,2);
      lcd.print(F("RDS Beenden: Links"));
      lcd.setCursor(1,3);
      lcd.print(F("gedr ckt halten  "));
      zeichen(5,3,245);
      while (wahl<3)
      {  
        Radio.readRDS_Radiotext(rdsText,500); // Read RDS text block timeout 500ms
        for (count=0;count<4;count++)
        {
          for (help2=0;help2<3;help2++) // Read 4x16 chars
          {
            lcd.setCursor(2,1);
            for (help=0;help<16;help++) 
              ((rdsText[help+(count*16)])<16)? lcd.print(char(32)):lcd.print(rdsText[help+(count*16)]);
            Radio.readRDS_Radiotext(rdsText,500);
            lcd.setCursor(11,2);
            wahl=taste(false);
            if (wahl>2) break;
          }  
          if (wahl>2) break;
        }
      }
      rds=false;
    }
    else schleife(false,true);
    switch(wahl)
    {
      case 1:
        save2=true;
        switch(menue)
        {
          case 0: if (radio) freq=Radio.seekDown(); break;
          case 1: if (freq>=875) freq--; break;
          case 2:
            if (radio) 
            {
              radio_aus();
              save=false;
            }
            else
            {
              if ((!pause) && (PIND & (1<<4))) mp3_aus();
              aux_aus();
              radio=true;
            }
            break;
          case 3: if (radio) rds=true; break;
          case 4: if (freq<=1079) freq++; break;
          case 5: if (radio) freq=Radio.seekUp(); break;
          case 6: freq=station[0]; break;
          case 7: 
            station[0]=freq; // Write station 1
            EEPROM.write(22,station[0]%10);
            EEPROM.write(21,(station[0]-(station[0]%10))/10);
            zeige_speichern();
            break;
          case 8: freq=station[1]; break;
          case 9: 
            station[1]=freq; // Write station 2
            EEPROM.write(24,station[1]%10);
            EEPROM.write(23,(station[1]-(station[1]%10))/10);
            zeige_speichern();
            break;
          case 10: freq=station[2]; break;
          case 11: 
            station[2]=freq; // Write station 3
            EEPROM.write(26,station[2]%10);
            EEPROM.write(25,(station[2]-(station[2]%10))/10);
            zeige_speichern();
            break;
        }
        if (radio) save=true;
        break;
      case 2:
        if (menue<11) menue++;
        if ((!radio) && (menue==3)) menue++;
        break;
      case 3:
        if (menue>0) menue--;
        if ((!radio) && (menue==3)) menue--;
        break;
    } 
  }
  if (save2)
  {
    EEPROM.write(18,freq%10); // Keep actual frequence
    EEPROM.write(17,(freq-(freq%10))/10);    
  }
  save2=false;
  init_char();
  #endif
}

// Story menu & MP3 menu: "Geschichten hören" "Eigenes hören"
// 1=Story 2=MP3 3=No MP3 files found
void menue_mp3(uint8_t modus) 
{
  if (!mp3_an) while (PIND & (1<<4));
  uint8_t menue=1;
  uint8_t help;
  char buff[12];
  lcd.createChar(1,custom_char[9]);
  lcd.createChar(2,custom_char[10]);
  lcd.createChar(3,custom_char[8]);
  icon(custom_char[15]);
  lcd.setCursor(4,3);
  lcd.print(F("[<][ ][ ][>]"));
  zeichen(11,3,2);
  if (pause) menue=2;
  lcd.blink();
  while (wahl!=4)
  {
    if (modus==1) // Story menu "Geschichten hören"
    {
      lcd.setCursor(2,0);
      if (geschichte<10) null();
      lcd.print(geschichte); // Print story number
      lcd.print(char(32));
      lcd.print(readDisk(Disk0,((geschichten-1)*80+100)+((geschichte-1)*2))); // Print lenght in minutes
      lcd.print(char(58));
      help=readDisk(Disk0,((geschichten-1)*80+101)+((geschichte-1)*2)); // And print seconds
      if (help<10) null();
      lcd.print(help);
      for (help=0;help<20;help++) // Print name and author
      {
        zeichen(help,1,(readDisk(Disk0,100+((geschichte-1)*geschichten)+help)));
        zeichen(help,2,(readDisk(Disk0,100+((geschichte-1)*geschichten)+help+20)));
      }  
    }
    if (modus==2) // MP3 menue "Eigenes hören"
    {
      if (!(PIND & (1<<4)) && (!pause) && (!geschichte_an)) 
      {
        mp3.playFileByIndexNumber(111+geschichten+eigenes); // Set index to first file
        powerdowndelay(100);
        mp3_aus(); // Stop at once to keep index set
        powerdowndelay(100);
      }
      lcd.setCursor(2,0);
      if (geschichte_an) lcd.print(F("Geschichte")); // Is a story running?
      else
      {
        powerdowndelay(100); // Read filename. Only 8 chars possible :-(
        mp3.currentFileName(buff,sizeof(buff));
        lcd.print(buff);
      }
      lcd.setCursor(7,2);
      if ((PIND & (1<<4)) && (!geschichte_an))
      {
        powerdowndelay(100);
        eigenes=mp3.currentFileIndexNumber(MP3_SRC_SDCARD)-151; // Read file number
      }
      if (eigenes<10) null();
      lcd.print(eigenes); // Print file number
      lcd.print(char(47));
      if (files<10) null();
      lcd.print(files); // Print last file number
    }
    if (modus==3) // No MP3s found!
    {
      lcd.setCursor(0,1);
      lcd.print(F("Keine MP3s gefunden"));
    }
    lcd.setCursor(15,0);
    if (pause) lcd.print(F("PAUSE")); else leer(15,0,5);
    zeichen(8,3,((PIND & (1<<4)) || (pause))? 1:3);
    lcd.setCursor(5+(menue*3),3);
    schleife(true,true);
    switch(wahl)
    {
      case 1:
        switch(menue)
        {
        case 0: // Prev
          if (modus==1)
          {
            if (geschichte>1) geschichte--;
            if (eigenes_an) {eigenes_an=false; geschichte_an=true;}
            if (PIND & (1<<4)) JQ6500_play(geschichte+111);
            }
          if (modus==2)
          {
            if (eigenes>1) eigenes--;
            if (geschichte_an) {geschichte_an=false; eigenes_an=true;}
            if (PIND & (1<<4)) JQ6500_play(eigenes+111+geschichten);
          }
          break;
        case 1: // On/Off
          if (radio) radio_aus(); 
          if (!(PIND & (1<<4))) 
          {
            if (modus==1)
            {
              geschichte_an=true;
              JQ6500_play(geschichte+111);
            }
            if (modus==2)
            {
              eigenes_an=true;
              JQ6500_play(eigenes+111+geschichten);
            }
            mp3_an=true;
          }
          else mp3_aus();
          aux_aus();
          break;
        case 2: // Pause
          if ((PIND & (1<<4)) || (pause))
          {
            if (radio) radio_aus();
            if (pause)
            {
              PORTD &= ~(1<<6); // Amplifier on
              pause=false;
              mp3.play();
            }
            else 
            {
              pause=true;
              mp3.pause();
            }
          }
          break;
        case 3: // Next
          if (modus==1)
          {
            if (geschichte<geschichten) geschichte++;
            if (eigenes_an) {eigenes_an=false; geschichte_an=true;}
            if (PIND & (1<<4)) JQ6500_play(geschichte+111);
          }
          if (modus==2)
          {
            if (eigenes<files) eigenes++;
            if (geschichte_an) {geschichte_an=false; eigenes_an=true;}
            if (PIND & (1<<4)) JQ6500_play(eigenes+111+geschichten);
          }
          break;
      }
      break;
      case 2:
        if ((!pause) && (menue<3)) menue++;
        break; 
      case 3:
        if ((!pause) && (menue>0)) menue--;
        break;
    }
  }
  init_char();
}

void menue_Alarm()  // Set alarm - no alarm allowd in this menue
{
  uint8_t menue=0;
  alarm_jetzt=true;
  boolean save=false;
  icon(custom_char[17]);
  if (alarmhh<10) null();
  lcd.print(alarmhh);
  lcd.print(char(58));
  if (alarmmm<10) null();
  lcd.print(alarmmm);
  uhr();
  lcd.setCursor(0,1);
  lcd.print(F("Neue Weckzeit"));
  lcd.setCursor(2,3);
  lcd.print(F("Weckton    [     ]"));
  lcd.setCursor(14,3);
  switch(klang)
  {
    case 0:lcd.print(F("Ton")); break; // Choose alarm type
    case 1:lcd.print(F("Radio")); break;
    case 2:lcd.print(F("MP3")); break;
  }
  lcd.blink();
  while(wahl!=4)
  {
    zeichen(0,(menue<5)? 3:2,32);
    zeichen(0,(menue<5)? 2:3,126);
    lcd.setCursor(2,2);  
    if (alarmhh<10) null();
    lcd.print(alarmhh);
    lcd.print(char(58));
    if (alarmmm<10) null();
    lcd.print(alarmmm);
    uhr();
    (alarm_an)? lcd.print(F("  [an ]")):lcd.print(F("  [aus]")); // Alarm on/off?
    switch(menue)
    {
      case 0: lcd.setCursor(2,2); break;
      case 1: lcd.setCursor(3,2); break;
      case 2: lcd.setCursor(5,2); break;
      case 3: lcd.setCursor(6,2); break;
      case 4: lcd.setCursor(14,2); break;
      case 5: lcd.setCursor(14,3); break;
    }
    schleife(false,true);
    switch(wahl)
    {
      case 1:
        save=true;
        switch(menue) // Set Alarm - belly adds one
        {
          case 0: (alarmhh<14)? alarmhh+=10:alarmhh%=10; break;
          case 1: (alarmhh%10==9)? alarmhh-=9:alarmhh++;
                  if (alarmhh==24) alarmhh=20; break;
          case 2: (alarmmm<49)? alarmmm+=10:alarmmm%=10; break;
          case 3: (alarmmm%10==9)? alarmmm-=9:alarmmm++; break;
          case 4: alarm_an=!alarm_an; break;
          case 5:
            lcd.noBlink();
            menue_Alarmwahl();
            break;
        }
        break;
      case 2: if (menue<5) menue++; break;
      case 3: if (menue>0) menue--; break;
    }
  }
  if (save) // Write alarm to EEPROM
  {
    EEPROM.write(3,alarm_an);
    EEPROM.write(4,alarmmm);
    EEPROM.write(5,alarmhh);
  }
  lcd.createChar(0,custom_char[0]);
  alarm_jetzt=false;
  save=false;
}

void menue_Alarmwahl() // Which alarm type? No alarm allowed hier
{
  uint8_t menue=0;
  boolean alarm_an2=alarm_an;
  alarm_an=false;
  lcd.clear();
  lcd.setCursor(2,0);
  lcd.print(F("[ ] Weckton")); // Tone 0-5
  lcd.setCursor(2,1);
  lcd.print(F("[ ] Radio"));   // Radio
  lcd.setCursor(2,2);
  lcd.print(F("[ ] MP3"));     // MP3 0-9
  while (wahl!=4)
  {
    for (uint8_t help=0;help>2;help++) 
      zeichen(3,help,32);
    zeichen(3,klang,88);
    zeichen(0,menue,126);
    #ifndef def_radio
      zeichen(3,1,45);
    #endif
    lcd.setCursor(3,menue);
    schleife(false,true);
    switch(wahl) 
    {
      case 1:
        #ifndef def_radio
          if (menue==1) break;
        #endif
        klang=menue;
        EEPROM.write(6,klang);
        if (klang==0) menue_Alarmton(1);
        if (klang==2) menue_Alarmton(2);
        break;
      case 2:
        zeichen(0,menue,32);
        if (menue<2) menue++;
        break;
      case 3:
        zeichen(0,menue,32);
        if (menue>0) menue--;
        break;
    }
  }
  alarm_an=alarm_an2;
  lcd.createChar(0,custom_char[0]);
}

// Select sound number
// 1=Tone 0-5; 2=MP3 0-9
void menue_Alarmton(uint8_t modus)
{
  boolean save=false;
  lcd.clear();
  lcd.print(char(126));
  lcd.print(F(" Weckton +/- [ ]"));
  lcd.setCursor(15,0);
  lcd.blink();
  if (modus==1) lcd.print(klang_ton);
  else lcd.print(klang_mp3);
  sound_an();
  alarm_jetzt=true;
  while (wahl!=4)
  {
    lcd.setCursor(15,0);
    schleife(true,true);
    if (wahl==1)
    {
      save=true;
      if (modus==1) // Press belly to add one
      {
        (klang_ton==5)? klang_ton=0:klang_ton++;
        lcd.print(klang_ton);
        alarmton(klang_ton);
      }
      else
      {
        (klang_mp3==10)? klang_mp3=0:klang_mp3++;
        lcd.print(klang_mp3);
        if (!mp3_an) JQ6500_play(klang_mp3+1);
      }
    }
  }
  if (save)
  {
    if (modus==1) EEPROM.write(7,klang_ton); // Save
    else EEPROM.write(19,klang_mp3);
  }
  if (!mp3_an) mp3.pause();
  alarm_jetzt=false;
  save=false;
}

void menue_Uhrzeit()  // Set time and nightmode
{
  uint8_t menue=0;
  uint8_t ha=tm.Hour;
  uint8_t ma=tm.Minute;
  uint8_t dd=tm.Day;
  uint8_t dm=tm.Month;
  boolean save=false;
  icon(custom_char[16]);
  lcd.print(F("Neue Uhrzeit/Datum"));
  lcd.setCursor(2,1);
  lcd.print(F("Nachtmodus [   ]")); // To nightmode menue
  lcd.blink();
  j-=2000;
  while(wahl!=4)
  {
    lcd.setCursor(2,2);  
    if (ha<10) null();
    lcd.print(ha);
    lcd.print(char(58));
    if (ma<10) null();
    lcd.print(ma);
    uhr();
    lcd.setCursor(2,3);  
    if (dd<10) null();
    lcd.print(dd);
    lcd.print(char(46));
    if (dm<10) null();
    lcd.print(dm);
    lcd.print(F(".20"));
    if (j<10) null();
    lcd.print(j);
    lcd.setCursor(14,1);
    (nachtmodus)? lcd.print(F("an ")):lcd.print(F("aus"));
    if (menue==0)                
    {
      zeichen(0,2,32);
      lcd.setCursor(0,1);
    }
    if (menue>0)             
    {
      zeichen(0,1,32);
      zeichen(0,3,32);
      lcd.setCursor(0,2); 
    }
    if ((menue>4) && (menue<11))
    {
      zeichen(0,2,32);
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
    schleife(false,true);
    switch(wahl)
      {
      case 1:
        if (menue>0) save=true;
        switch(menue)
        {
          case 0: menue_Nachtmodus(); break;
          case 1: (ha<14)? ha+=10:ha%=10; break;
          case 2: (ha%10==9)? ha-=9:ha++;
                  if (ha==24) ha=20; break;
          case 3: (ma<49)? ma+=10:ma%=10; break;
          case 4: (ma%10==9)? ma-=9:ma++; break;
          case 5:
            (dd<22)? dd+=10:dd%=10;
            if (dd==0) dd=1;
            break;
          case 6: 
            (dd%10==9)? dd-=9:dd++; 
            if (dd>31) dd=30; 
            if (dd==0) dd=1; 
            break;
          case 7:
            (dm<3)? dm+=10:dm%=10;
            if (dm==0) dm=1;
            break;
          case 8: (dm<12)? dm++:dm=10; break;
          case 9: (j<89)? j+=10:j%=10; break;
          case 10: (j%10==9)? j-=9:j++; break;
        }
        break;
      case 2: if (menue<10) menue++; break;
      case 3: if (menue>0) menue--; break;
    }
  }  
  j+=2000;
  if (save)
  {
    setTime(ha,ma,0,dd,dm,j);
    tm.Hour=ha; 
    tm.Minute=ma; 
    tm.Second=0; 
    tm.Day=dd;
    tm.Month=dm;
    tm.Year=CalendarYrToTm(j);
    RTC.write(tm);
    save=false;
    anzeigen();
    sommer=sommerzeit();
    EEPROM.write(16,sommer);
  }
  lcd.createChar(0,custom_char[0]);
  ma=255;
}

void menue_Nachtmodus() // Set nightmode
{
  uint8_t menue=0;
  boolean save=false;
  icon(custom_char[16]);
  lcd.print(F("Nachtmodus stellen"));
  lcd.setCursor(2,1);
  lcd.print(F("[   ] Dimmen [ ]")); // dim light wenn NOKO enters nightmode
  lcd.setCursor(2,2);
  lcd.print(F("von"));
  lcd.setCursor(2,3);
  lcd.print(F("bis"));
  while(wahl!=4)
  {
    lcd.setCursor(3,1);
    (nachtmodus)? lcd.print(F("an ")):lcd.print(F("aus")); // Nightmode on or off?
    zeichen(16,1,nachtdimm? 88:32);
    lcd.setCursor(6,2);  
    if (nachtahh<10) null();
    lcd.print(nachtahh);
    lcd.print(char(58));
    if (nachtamm<10) null();
    lcd.print(nachtamm);
    uhr();
    lcd.setCursor(6,3);  
    if (nachtbhh<10) null();
    lcd.print(nachtbhh);
    lcd.print(char(58));
    if (nachtbmm<10) null();
    lcd.print(nachtbmm);
    uhr();
    if (menue==0)
    {
      zeichen(0,2,32);
      lcd.setCursor(0,1); 
    }
    if (menue==1)
    {
      zeichen(0,2,32);
      lcd.setCursor(0,1); 
    }
    if ((menue<6) && (menue>1)) // Set arrow
    {
      zeichen(0,1,32);
      zeichen(0,3,32);
      lcd.setCursor(0,2); 
    }
    if (menue>5) 
    {
      zeichen(0,2,32);
      lcd.setCursor(0,3);
    }
    lcd.print(char(126));
    switch (menue)
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
    schleife(false,true);
    switch(wahl)
    {
      case 1:
        save=true;
        switch(menue)
        {
          case 0: nachtmodus=!nachtmodus; break;
          case 1: nachtdimm=!nachtdimm; break;          
          case 2: (nachtahh<14)? nachtahh+=10:nachtahh%=10; break;
          case 3: (nachtahh%10==9)? nachtahh-=9:nachtahh++;
                  if (nachtahh==24) nachtahh=20; break;
          case 4: (nachtamm<49)? nachtamm+=10:nachtamm%=10; break;
          case 5: (nachtamm%10==9)? nachtamm-=9:nachtamm++; break;
          case 6: (nachtbhh<14)? nachtbhh+=10:nachtbhh%=10; break;
          case 7: (nachtbhh%10==9)? nachtbhh-=9:nachtbhh++;
                  if (nachtbhh==24) nachtbhh=20; break;
          case 8: (nachtbmm<49)? nachtbmm+=10:nachtbmm%=10; break;
          case 9: (nachtbmm%10==9)? nachtbmm-=9:nachtbmm++; break;
        }
        if (save)
        {
          EEPROM.write(11,nachtmodus);
          EEPROM.write(12,nachtahh);
          EEPROM.write(13,nachtamm);
          EEPROM.write(14,nachtbhh);
          EEPROM.write(15,nachtbmm);
          EEPROM.write(28,nachtdimm);
        }
        save=false;
        break;
      case 2: if (menue<9) menue++; break;
      case 3: if (menue>0) menue--; break;
    }
  }
  lcd.createChar(0,custom_char[0]);
}

void menue_Einstellungen()  // Settings "NOKO stellen"
{
  uint8_t menue=0;
  lcd.clear();
  lcd.setCursor(2,0);
  lcd.print(F("LED     +/- [ ]")); // LED brightness 0-9 * 28 (0..255)
  lcd.setCursor(2,1);
  lcd.print(F("Events  +/- [ ]")); // Chance for time event 0-9 (1=ca. every two hours; 9=nearly every minute)
  lcd.setCursor(2,2);
  lcd.print(F("Distanz +/- [ ]")); // Reaction distance for ultrasonic 0-9 *10cm
  lcd.setCursor(2,3);
  lcd.print(F("weiter..."));
  while(wahl!=4)
  {
    lcd.blink();
    analogWrite(LED,(menue==0)? led_dimm*28:0);
    if (menue==3) lcd.noBlink();
    zeichen(0,menue,126);
    lcd.setCursor(15,0);
    lcd.print(led_dimm);
    lcd.setCursor(15,1);
    lcd.print(eventsteller);
    lcd.setCursor(15,2);
    lcd.print(ultra_distanz);
    lcd.setCursor(15,menue);
    while ((wahl=taste(false))==0) NewDelay(100); // No power down while LED is in use!
    NewDelay(50);
    switch(wahl)
    {
      case 1:  
        switch(menue)
        {
          case 0:
            (led_dimm<9)? led_dimm++:led_dimm=0;
            analogWrite(LED,led_dimm*28);
            EEPROM.write(2,led_dimm);
            break;
          case 1:
            (eventsteller<9)? eventsteller++:eventsteller=0;
            EEPROM.write(9,eventsteller);
            break;
          case 2:
            (ultra_distanz<9)? ultra_distanz++:ultra_distanz=0;
            EEPROM.write(8,ultra_distanz);
            break;
          case 3: menue_Einstellungen2(); break;
        }
        break;
      case 2:
        zeichen(0,menue,32);
        if (menue<3) menue++;
        lcd.setCursor(0,menue);
        break;
      case 3:
        zeichen(0,menue,32);
        if (menue>0) menue--;
        lcd.setCursor(0,menue);
        break;
    }
  }
  if (!lcddimm) analogWrite(LED,0);
}

void menue_Einstellungen2() // "weiter..." - more settings
{
  uint8_t menue=0;
  lcd.clear();
  lcd.setCursor(2,0);
  lcd.print(F("Akku sparen    [ ]")); // Battery saving - not only lights out
  lcd.setCursor(2,1);
  lcd.print(F("Stumm          [ ]")); // No sounds at all
  lcd.setCursor(2,2);
  lcd.print(F("Distanzlicht   [ ]")); // Use ultrasonic to turn on light when it's off
  lcd.setCursor(2,3);
  lcd.print(F("Mein NOKO"));          // About NOKO menue
  while (wahl!=4)
  {
    zeichen(18,0,powersave? 88:32);
    zeichen(18,1,stumm? 88:32);
    zeichen(18,2,ultra_light? 88:32);
    zeichen(0,menue,126);
    lcd.setCursor(18,menue);
    (menue<3)? lcd.blink():lcd.noBlink();  
    schleife(false,true);
    switch(wahl) 
    {
      case 1:
        switch(menue) 
        {
          case 0: // Set power save mode
            powersave=!powersave; 
            EEPROM.write(10,powersave);
            powerdowndelay(pwd_delay);
            break;
          case 1: // Toggle sound
            lcd.noBlink();
            if (!stumm)
            {
              radio_aus();
              zeichen(18,1,46);
              mp3_aus();
              aux_aus();
            }
            stumm=!stumm;
            EEPROM.write(1,stumm);
            break;
          case 2: // Set to turn on LCD light with ultrasonic
            ultra_light=!ultra_light; 
            EEPROM.write(26,ultra_light);
            powerdowndelay(pwd_delay);
            break;
          case 3: menue_NOKO(); break;
        }
        break;
      case 2:
        zeichen(0,menue,32);
        if (menue<3) menue++;
        break;
      case 3:
        zeichen(0,menue,32);
        if (menue>0) menue--;
        break;
    }
  }
}

void menue_NOKO() // "Mein NOKO" - about NOKO and secret menue
{
  uint8_t help;
  icon(custom_char[13]);
  lcd.print(F("Dieser NOKO geh rt"));
  zeichen(17,0,239);
  lcd.setCursor(0,1);
  for (help=0;help<60;help++) // Print owner and email
  {
    if (help==20) lcd.setCursor(0,2);
    if (help==40) lcd.setCursor(0,3);
    lcd.print(char(readDisk(Disk0,20+help)));   
  }
  schleife(false,false);
  lcd.createChar(0,custom_char[0]);
  lcd.clear();
  bignum(0,0,11); // Draw "NOKO"
  bignum(3,0,0);
  bignum(6,0,12);
  bignum(8,0,0);
  bignum(13,0,Version/10); // Draw version number - version/10
  zeichen(16,1,4);
  bignum(17,0,Version%10);
  lcd.setCursor(0,3);
  lcd.print(F(Build_by)); 
  while (wahl!=4) // Secret menu, when right then left are pressed
  {
    wahl=taste(false);
    if (taste(false)==2) 
      while (wahl!=4) 
      {
        wahl=taste(false);
        if (taste(false)==3) 
        {
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print(F("Firmware: "));
          lcd.print(Version/10); // Firmware
          lcd.print(char(46));
          lcd.print(Version%10);
          lcd.print(F(Firmware));
          lcd.setCursor(0,1);
          lcd.print(F("I2C: "));  // Show all I2C devices
          for (help=8;help<120;help++)
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
          lcd.print(getTemperature(),0); // Internal temperature
          lcd.print(char(223));
          lcd.setCursor(0,3);
          lcd.print(F("Akku:"));
          lcd.setCursor(11,3); // Print sommertime/wintertime flag
          lcd.print(F("Z: "));
          (sommer)? lcd.print(char(83)):lcd.print(char(87));
          while (wahl!=4)
          {
            wahl=taste(true);
            lcd.setCursor(6,3);
            lcd.print(float(analogRead(Akku)*5.0/1024.0));
            stopdelay(100);
          }      
        }
      }
  }
}

void event() // Time bases events
{
  uint8_t help2,ma;
  uint8_t help=4;
  #ifdef def_external_eeprom  // Check if EEPROM is used and defined
    help=0;  
  #endif
  help2=newrandom(help,12);
  if (dimm) help2=10;    // If display is turned off, only start the voice event
  lcd.noBlink();
  if ((help2<8) && (lcddimm) && !((nachtmodus) && (nacht))) lcd.backlight();
  if (help2<5) lcd.clear();
  switch(help2)
  {
    #ifdef def_external_eeprom      // External EEPROM
      case 0: gedicht(); break;     // Poem "Alle Kinder..."
      case 1: schimpfwort(); break; // Swearword generator
      case 2: phrase(); break;      // Phrase "Wusstest Du schon..."
      case 3: zitat(); break;       // Quotation - without source. Shame on me. :-(
    #endif
    case 4:  // Ich bin hier!
      lcd.setCursor(3,1);
      lcd.print(F("Ich bin hier!"));
      analogWrite(LED,led_dimm*28);
      sound_an();
      ton(800,250,false,270);
      ton(800,150,false,180); 
      ton(660,250,false,250);
      stopdelay(2000);
      analogWrite(LED,0);   
      break;
    case 5:  // Monster party
      ma=minute();
      lcd.createChar(0,custom_char[13]);
      while ((ma==minute()) && (wahl!=4))
      {  
        alarm_jetzt=true;
        lcd.clear();
        for (help=newrandom(1,20);help>0;help--)
          zeichen(newrandom(0,20),newrandom(0,4),0);
        stopdelay(500);
      }
      alarm_jetzt=false;
      break;
    case 6:  // Scroll display
      for (help=40;help>1;help--)
      {
        lcd.scrollDisplayLeft();
        stopdelay(200);
        if (wahl==4) help=1;
      }
      break;
    case 7:  // Monster cleans display
      lcd.createChar(0,custom_char[13]);
      for (ma=0;ma<4;ma++)
      {
        for (help=0;help<20;help++)
        {
          zeichen(help,ma,0);
          stopdelay(150);
          if (wahl==4)
          {
            ma=4;
            help=20;
          }
          zeichen(help,ma,32);
        }
      }
      stopdelay(2000);
      break;
    default:
      JQ6500_play(newrandom(81,111)); // Voice event - NOKO says something. Voice 81-110
      break;
  }
  if (help2<8) 
  {
    anzeigen();
    if ((lcddimm) && !((nachtmodus) && (nacht))) lcd.noBacklight();
  }
}

void schimpfwort() // Swearword event on display
{
  uint16_t num;
  uint8_t geschlecht=newrandom(0,2);
  lcd.setCursor(0,1);
  num=(newrandom(0,101)*10);
  schreibe_schimpfwort(num);
  if (geschlecht==0) lcd.print("r");
  lcd.setCursor(0,2);
  num=(newrandom(0,101)*10)+1000;
  schreibe_schimpfwort(num);
  num=(newrandom(0,101)*10)+((geschlecht==0)? 2000:3000);
  schreibe_schimpfwort(num);
  JQ6500_play(11);
  schleife(true,true);
}

void schreibe_schimpfwort(uint16_t num) // Helps swearword event
{
  uint8_t c;
  uint16_t help;
  for (help=num;help<num+10;help++)
  {
    c=readDisk(Disk1,help);
    if (c!=32) lcd.print(char(c));
  }
}

void phrase() // Phrase event on display
{
  uint8_t help;
  uint8_t help2=newrandom(0,100);
  lcd.print(F("Wusstest Du, dass..."));
  for (help=0;help<20;help++)
  {
    zeichen(help,1,(readDisk(Disk1,phrase_address+(help2*60)+help)));
    zeichen(help,2,(readDisk(Disk1,phrase_address+(help2*60)+help+20)));
    zeichen(help,3,(readDisk(Disk1,phrase_address+(help2*60)+help+40)));
  }
  schleife(true,true);
}

void zitat() // Quotation event on display
{
  uint8_t help;
  uint8_t help2=newrandom(0,100);
  for (help=0;help<20;help++)
  {
    zeichen(help,0,(readDisk(Disk1,quote_address+(help2*80)+help)));
    zeichen(help,1,(readDisk(Disk1,quote_address+(help2*80)+help+20)));
    zeichen(help,2,(readDisk(Disk1,quote_address+(help2*80)+help+40)));
    zeichen(help,3,(readDisk(Disk1,quote_address+(help2*80)+help+60)));
  }
  schleife(true,true);
}

void gedicht() // Poem event on display
{
  uint8_t help;
  uint8_t help2=newrandom(0,26);
  for (help=0;help<20;help++)
  {
    zeichen(help,0,(readDisk(Disk1,poem_address+(help2*80)+help)));
    zeichen(help,1,(readDisk(Disk1,poem_address+(help2*80)+help+20)));
    zeichen(help,2,(readDisk(Disk1,poem_address+(help2*80)+help+40)));
    zeichen(help,3,(readDisk(Disk1,poem_address+(help2*80)+help+60)));
  }
  sound_an();
  ton(880,80,false,150);
  ton(880,400,false,400);
  PORTD |= (1<<6); // Amplifier off
  schleife(true,true);
}

void feiern() // Birthdaytime! Party! Party!
{
  uint8_t help,ma;
  lcd.createChar(0,custom_char[14]);
  #ifdef def_radio
    if (radio) Radio.powerOff();
  #endif
  if (aux) PORTD &= ~(1<<7); // AUX aus;
  if ((!stumm) && (!mp3_an)) JQ6500_play(111); // Play birthday song 111
  NewDelay(2000);
  ma=minute();
  while (((ma==minute()) || (PIND & (1<<4))) && (wahl!=4))
  {  
    lcd.clear();
    lcd.setCursor(7,1);
    lcd.print(F("Happy"));
    lcd.setCursor(6,2);
    lcd.print(F("Birthday"));
    for (help=newrandom(1,5);help>0;help--)
    {
      zeichen(newrandom(0,20),0,0);
      zeichen(newrandom(0,20),3,0);
    }
    stopdelay(500);
  }
  gefeiert=true;
  if (!mp3_an) mp3.pause();
  if (radio) radio_ein();
  if (aux) PORTD |= (1<<7);
  anzeigen();
}

void alarmton(uint8_t klangnummer) // 6 alarm tunes
{
  uint8_t help;
  switch (klangnummer)
  {
    case 0:
      ton(262,250,true,333);
      ton(196,125,true,167);
      ton(196,125,true,167);
      ton(220,250,true,333);
      ton(196,250,true,666);
      ton(247,250,true,333);
      ton(262,250,true,333);
      break;
    case 1:
      for (help=10;help>0;help--)
      {
        ton(440,50,true,100);
        if (wahl==4) help=0;
      }
      break;
    case 2:
      ton(440,200,true,500);
      break;
    case 3:
      ton(600,100,true,100);
      ton(700,100,true,100);
      ton(800,100,true,300);
      break;
    case 4:
      ton(600,80,true,100);
      ton(400,80,true,100);
      ton(500,80,true,100);
      ton(400,80,true,200);      
      break;
    case 5:
      ton(600,200,true,200);
      ton(500,200,true,200);
      ton(400,200,true,300);
      break;
  }  
} 

float getTemperature() // Internal temperature
{
  uint8_t help;
  Wire.beginTransmission(0x68);
  Wire.write(uint8_t(0x11));
  Wire.endTransmission();
  Wire.requestFrom(0x68,2);
  help = Wire.read();
  return float(help)+0.25*(Wire.read()>>6);
}

void radio_ein() // Radio on
{
  #ifdef def_radio
    Radio.powerOn();
    Radio.setChannel(freq);
  #endif
}

void radio_aus() // Radio off
{
  #ifdef def_radio
    Radio.powerOff();
    radio=false;
  #endif
}

void aux_aus() // AUX relay off
{
  PORTD &= ~(1<<7); 
  aux=false;
}

void mp3_aus() // Stop MP3 playing
{
  mp3.pause();  
  powerdowndelay(100);
  geschichte_an=false;    
  eigenes_an=false;
  mp3_an=false;
  mp3.setLoopMode(MP3_LOOP_NONE); // Change loop mode to once
}

void sound_an() // Turns on amplifier with a small delay for beep tones
{
  PORTD &= ~(1<<6); // Amplifier ein
  NewDelay(reaktionszeit);
}

void JQ6500_play(uint8_t v) // Plays MP3 number v
{
  sound_an(); // Amplifier on
  mp3.playFileByIndexNumber(v);
  powerdowndelay(100);
  if (eigenes_an) mp3.setLoopMode(MP3_LOOP_FOLDER);
  if (geschichte_an) mp3.setLoopMode(MP3_LOOP_NONE);  
}

int16_t freeRam() // Free RAM in bytes
{
  extern uint8_t __heap_start,*__brkval;
  int16_t v;
  return (int16_t)&v-(__brkval==0? (int16_t)&__heap_start:(int16_t)__brkval);
}

uint8_t readDisk(uint8_t disknummer,uint16_t adresse) // Read an EEPROM
{
  uint8_t rdata = 0xFF; 
  Wire.beginTransmission(disknummer);
  Wire.write((uint16_t)(adresse >> 8));   
  Wire.write((uint16_t)(adresse & 0xFF)); 
  Wire.endTransmission();
  Wire.requestFrom(disknummer,1);
  if (Wire.available()) rdata = Wire.read();
  return rdata;
}


