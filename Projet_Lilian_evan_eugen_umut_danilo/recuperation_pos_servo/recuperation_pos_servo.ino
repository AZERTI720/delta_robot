#include <DynamixelShield.h>
#include <SoftwareSerial.h>

DynamixelShield dxl;
SoftwareSerial mySerial(10, 11); // RX, TX  ← vers Arduino 2

void setup() {
  mySerial.begin(115200);   // vers Arduino 2
  dxl.begin(1000000);
  dxl.setPortProtocolVersion(1.0);
}

void loop() {

  float position1raw = dxl.getPresentPosition(1, UNIT_RAW); //UNIT_DEGREE
  float position1deg = dxl.getPresentPosition(1, UNIT_DEGREE); //UNIT_DEGREE
  mySerial.print("position 1 raw: ");
  mySerial.print(position1raw);
  mySerial.print("    en raw : ");
  mySerial.println(position1deg);

  float position2raw = dxl.getPresentPosition(2, UNIT_RAW); //UNIT_DEGREE
  float position2deg = dxl.getPresentPosition(2, UNIT_DEGREE); //UNIT_DEGREE
  mySerial.print("position 2 raw: ");
  mySerial.print(position2raw);
  mySerial.print("    en raw : ");
  mySerial.println(position2deg);

  float position3raw = dxl.getPresentPosition(3, UNIT_RAW); //UNIT_DEGREE
  float position3deg = dxl.getPresentPosition(3, UNIT_DEGREE); //UNIT_DEGREE
  mySerial.print("position 3 raw: ");
  mySerial.print(position3raw);
  mySerial.print("    en raw : ");
  mySerial.println(position3deg);
  
  
  delay(2000);
}  