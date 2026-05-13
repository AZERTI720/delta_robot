//71
//com11 
#include <Wire.h>
     
     
    void setup()
    {
      Wire.begin();
      Serial.begin(9600);
      while (!Serial);         
      Serial.println(F("\nI2C Scanner"));
    }
     
     
    void loop()
    {
      byte error, address;
      int nDevices;
     
      Serial.println(F("Scanning..."));
     
      nDevices = 0;
      for(address = 1; address < 127; address++ )
      {

          Wire.beginTransmission(address);
          error = Wire.endTransmission();
          
        
        if (error == 0)
        {
          Serial.print("I2C device found at address 0x");
          if (address<16)
            Serial.print("0");
          Serial.print(address,HEX);
          Serial.println("  !");
     
          nDevices++;
        }
        else if (error==4)
        {
          Serial.print("Unknown error at address 0x");
          if (address<16)
            Serial.print("0");
          Serial.println(address,HEX);
        }    
      }
      if (nDevices == 0)
        Serial.println(F("No I2C devices found\n"));
      else
        Serial.println(F("done\n"));
     
      delay(5000);           // wait 5 seconds for next scan
    }
