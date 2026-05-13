#include <Wire.h>

#define SLAVE_ADDR 11 // L'adresse de ton unique esclave

void setup() {
  Wire.begin();        
  Serial.begin(9600); 
  Serial.println(F("--- Master Board Ready ---"));
  delay(2000); 
}

void loop() {
  int n = 0;
  int valeurAEnvoyer = 67; // Tu peux changer cette valeur

  Serial.println(F("Envoi de la valeur..."));
  
  // Envoyer la valeur à l'esclave
  Wire.beginTransmission(SLAVE_ADDR); 
  Wire.write(valeurAEnvoyer); 
  Wire.endTransmission();

  delay(500); 
  //  Demander à l'esclave de renvoyer la valeur
  Wire.requestFrom(SLAVE_ADDR, 1); 
  if (Wire.available()) {
    n = Wire.read();
    Serial.print(F("Reponse de l'esclave : "));
    Serial.println(n);
  }

  Serial.println(F("-----------------------"));
  delay(2000); 
}
