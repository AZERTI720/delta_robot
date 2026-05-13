#include <Wire.h>

const int SLAVE_ADDR = 11;

void setup() {
  Wire.begin(); 
  Serial.begin(115200);
}

void loop() {
  Wire.requestFrom(SLAVE_ADDR, 4); // Demande 4 octets
  
  if (Wire.available() >= 4) {
    // Lecture et reconstruction de X
    int xHigh = Wire.read();
    int xLow = Wire.read();
    int posX = (xHigh << 8) | xLow;

    // Lecture et reconstruction de Y
    int yHigh = Wire.read();
    int yLow = Wire.read();
    int posY = (yHigh << 8) | yLow;

    Serial.print("X: "); Serial.print(posX);
    Serial.print(" | Y: "); Serial.println(posY);
  }
  
  delay(50); // Petite pause pour ne pas saturer le bus
}
