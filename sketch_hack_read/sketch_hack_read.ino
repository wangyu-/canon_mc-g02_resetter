#include "Wire.h"

int eeprom_read_byte(byte deviceaddress, unsigned eeaddr)
{
   byte rdata = -1;

   //byte devaddr = deviceaddress | ((eeaddr >> 8) & 0x07);
   byte devaddr = deviceaddress | ((eeaddr >> 8));
   byte addr    = eeaddr;

   Wire.beginTransmission(devaddr);
   Wire.write(int(addr));
   Wire.endTransmission();
   Wire.requestFrom(int(devaddr), 1);
   if (Wire.available()) {
       rdata = Wire.read();
   }
   else
   {
     Serial.print("Wire Not Ready!!!");
   }
   return rdata;
}

void setup()
{
  Wire.begin();
  Serial.begin(9600);
 
  Serial.println("\n\n\nStart Dumping...\n");
  Serial.println("Below is your rom:\n");
  
  Serial.print("\n");
  Serial.print("const unsigned char my_rom1[] PROGMEM=\n{\n");
  for(int i=0;i<2048;i++)
  {
    if(i) Serial.print(",");
    if(i&&i%32==0) Serial.print("\n");
    Serial.print("0x");
    byte r=eeprom_read_byte(0x50, i);
    if(r<16) Serial.print("0");
    Serial.print(r,HEX);
  }
  Serial.print("};\n");

  Serial.print("\nAll done! Copy paste it into the write program to write.\n");
}
void loop()
{

}
 
