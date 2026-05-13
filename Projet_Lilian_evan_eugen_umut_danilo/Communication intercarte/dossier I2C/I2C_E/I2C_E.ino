#include <Wire.h>

#define MY_ADDRESS 11 

int n = 0; 

void setup() {
  Wire.begin(MY_ADDRESS); // S'enregistre avec l'adresse 11
  Serial.begin(9600); 
  Serial.println(F("--- Slave Board Ready (Addr 11) ---"));
               
  // Fonctions de rappel (callbacks)
  Wire.onRequest(requestEvents);
  Wire.onReceive(receiveEvents);
}

void loop() {
  // Rien ici, tout se passe dans les "Events"
  delay(100);
}

// Fonction appelée quand le Maître demande des données
void requestEvents() {
  Wire.write(n); 
}

// Fonction appelée quand le Maître envoie des données
void receiveEvents(int numBytes) {  
  if (Wire.available()) {
    n = Wire.read(); 
    Serial.print(F("Valeur recue du Maitre : "));
    Serial.println(n);
  }
}
