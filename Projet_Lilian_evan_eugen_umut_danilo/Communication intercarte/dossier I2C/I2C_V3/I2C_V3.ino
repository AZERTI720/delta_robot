#include <Wire.h>

const int server = 11;

void setup() {
  Wire.begin();
  Serial.begin(9600);
  Serial.println("Carte Arduino 2 prêt à recevoir");
}

void loop() {
  // On demande 4 octets à l'esclave
  Wire.requestFrom(server, 4);
  
  if (Wire.available() >= 4) {
    // Lecture et reconstruction de X
    byte xHigh = Wire.read();
    byte xLow = Wire.read();
    int posX = (xHigh << 8) | xLow;

    // Lecture et reconstruction de Y
    byte yHigh = Wire.read();
    byte yLow = Wire.read();
    int posY = (yHigh << 8) | yLow;

    // Affichage des résultats
    Serial.print("Position détectée -> X: ");
    Serial.print(posX);
    Serial.print(" | Y: ");
    Serial.println(posY);
  } else {
    Serial.println("Erreur: Carte Arduino 1 non trouvé ou données incomplètes");
  }
  
  delay(50);
}
