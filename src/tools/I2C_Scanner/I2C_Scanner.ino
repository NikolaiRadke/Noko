// I2C Scanner

#include <Wire.h>

byte number=0;

void setup() {
  Serial.begin (9600);
  Serial.println ("Looking for I2C-devices...");
  Wire.begin();
  for (byte address=8; address<120; address++)
  {
    Wire.beginTransmission(address);
    if (Wire.endTransmission()==0)
    {
      Serial.print(number+1);
      Serial.print(". Device: 0x");
      Serial.println(address, HEX);
      number++;
    }
  }
  Serial.print (number, DEC);
  Serial.println (" device(s) found.");
}

void loop() {}
