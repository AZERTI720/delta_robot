//Libraries 
#include <Wire.h>//https://www.arduino.cc/en/reference/wire 
 
//Constants 
#define I2C_MASTER_ADDRESS 10 
 
//Parameters 
const bool is_master = false; 
const int I2C_SLAVE_ADDRESS = 11; 
 
//Variables 
int masterStatus = 0; 
byte cmd = 0; 
int slaveStatus = 0; 
 
void setup(){ 
  //Init Serial USB 
  Serial.begin(9600); 
  Serial.println(F("Initialize System")); 
  //Init i2c (Default pins A4, A5) 
  if(is_master){ 
    Wire.begin(I2C_MASTER_ADDRESS); 
    Serial.print("Master @address ");Serial.println(I2C_MASTER_ADDRESS); 
  }else{ 
    Wire.begin(I2C_SLAVE_ADDRESS); 
    Wire.onRequest(requestEvents); 
    Wire.onReceive(receiveEvents); 
    Serial.print("Slave @address ");Serial.println(I2C_SLAVE_ADDRESS); 
  } 
  delay(200); 
 
} 
 
void loop(){ 
  testI2C(); 
 
} 
 
void testI2C( ){ 
  ////Test I2C communication change is_master to 0(slave) or 1(master) 
  if(is_master){ 
    writeTo(I2C_SLAVE_ADDRESS,0x10); 
    readFrom(I2C_SLAVE_ADDRESS,2); 
  } 
 
} 
 
void writeTo(int address,byte cmd){ 
  ////Write bytes to slave 
  Serial.print(F("---> send command to slave @address : ")); 
  Serial.println(address); 
  Wire.beginTransmission(address); 
  Wire.print(cmd);//Wire.write(cmd); 
  Wire.endTransmission(); 
 
} 
 
void readFrom(int address,int numBytes){ 
  ////request from slave 
  Serial.print(F("---> read from slave @address : ")); 
  Serial.println(address); 
  Wire.requestFrom(address, numBytes); 
  int n = Wire.read(); 
  Serial.print(F("received value : ")); 
  Serial.println(n); 
 
} 
 
void requestEvents(){ 
  ////Execute when requested from Master 
  Serial.println(F("---> request received")); 
  Serial.print(F("sending current status : ")); 
  Serial.println(slaveStatus); 
  Wire.write(slaveStatus); 
 
} 
 
void receiveEvents(int numBytes){ 
  ////Execute when bytes are received 
  cmd = Wire.read(); 
  Serial.print(numBytes); 
  Serial.println(F(" bytes received from Master")); 
  Serial.print(F("received value : ")); 
  Serial.println(cmd); 
 
} 
 
void scani2c(){ 
  ////Scan for I2C devices 
  byte error, address; 
  int nDevices; 
 
  Serial.println("Scanning..."); 
 
  nDevices = 0; 
  for(address = 1; address < 127; address++ ) 
  { 
    // The i2c_scanner uses the return value of 
    // the Write.endTransmisstion to see if 
    // a device did acknowledge to the address. 
    Wire.beginTransmission(address); 
    error = Wire.endTransmission(); 
 
    if (error == 0) 
    { 
      Serial.print("I2C device found at address 0x"); 
      if (address<16){ 
        Serial.print("0"); 
      } 
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
  if (nDevices == 0){ 
      Serial.println("No I2C devices found"); 
  }else{ 
    Serial.println("done"); 
  } 
 
} 
 
 
