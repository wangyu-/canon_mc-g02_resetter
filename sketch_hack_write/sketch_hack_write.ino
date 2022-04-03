#include "Wire.h"

// copy your dumped rom from the reader program,
// paste it here, replace the below commented code:

/*
const unsigned char my_rom1[] PROGMEM=
{
  ................................   ,
  ................................   ,
  .....  
};
*/

int eeprom_read_byte(byte deviceaddress, unsigned eeaddr)
{
   byte rdata = 0xff;

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

void eeprom_write_test()
{
  Wire.beginTransmission(0x50);
  Wire.write(2);
  Wire.write(0xab);
  Wire.endTransmission();
  delay(10);
}

// slow but easy write
void eeprom_write_byte(byte deviceaddress, int eeaddr, byte data)
{
//byte devaddr = deviceaddress | ((eeaddr >> 8) & 0x07);
byte devaddr = deviceaddress | ((eeaddr >> 8));
byte addr = eeaddr;
Wire.beginTransmission(devaddr);
Wire.write(addr);
Wire.write(data);
Wire.endTransmission();
delay(10);
}

//more efficient write
void eeprom_write_page(byte deviceaddress, unsigned eeaddr,
                      const byte * data, byte length)
{
   //byte devaddr = deviceaddress | ((eeaddr >> 8) & 0x07);
   byte devaddr = deviceaddress | ((eeaddr >> 8));
   byte addr    = eeaddr;
   Wire.beginTransmission(devaddr);
   Wire.write(int(addr));
   for (int i = 0; i < length; i++) {
       Wire.write(data[i]);
   }
   Wire.endTransmission();
   delay(10);
}

const int buf_size=16;
unsigned char buf[buf_size];

void setup()
{
  Wire.begin();
  Serial.begin(9600);
 
  Serial.println("\n\n\nStart Writing Rom...\n");

  //eeprom_write_test();

  //more efficient write
  for(int i=0;i<2048/buf_size;i++)
  {
      if(i%16==0)
      {
        Serial.print("current writing page:");
        Serial.println(i);
      }
      for(int j=0;j<buf_size;j++)
      {
        buf[j]=pgm_read_byte_near(my_rom1+i*buf_size +j); //!!!!!!!!switch from my_rom1 and my_rom2 if need
      }
      eeprom_write_page(0x50, i*buf_size, buf, buf_size);  

      //Serial.println(pgm_read_byte_near(my_rom1+i)  ,HEX);
  }

  // slow but easy write
  /*
  for(int i=0;i<2048;i++)
  {
      if(i%32==0)
      {
         Serial.print("current:");
         Serial.println(i);
      }

      eeprom_write_byte(0x50, i, pgm_read_byte_near(my_rom2+i));  //!!!!!!!!switch from my_rom1 and my_rom2 if need

      //Serial.println(pgm_read_byte_near(my_rom2+i)  ,HEX);
  }*/

  Serial.print("Write Done!\n");

  Serial.println("\n\nStart Dumping for Verification...\n");
  Serial.println("Check if this dump matches with before by youself:\n");
  Serial.println("(possibly with some diff tool)\n");
  
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
}
void loop()
{

}
 
