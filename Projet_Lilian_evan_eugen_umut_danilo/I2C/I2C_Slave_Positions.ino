#include <Wire.h>

#define MY_ADDRESS 11

// Structure pour les positions
struct Position {
  double x, y, z;
};

// Positions à envoyer (à remplacer par tes capteurs ou calculs)
Position currentPos = {0.0, 0.0, -120.0};

// Union pour convertir double en bytes
union DoubleBytes {
  double value;
  byte bytes[8];
};

void setup() {
  Wire.begin(MY_ADDRESS);
  Serial.begin(9600);
  Serial.println(F("--- Slave Board Ready (Addr 11) ---"));

  Wire.onRequest(requestEvents);
  Wire.onReceive(receiveEvents);
}

void loop() {
  // Mets à jour currentPos ici avec tes capteurs
  // currentPos.x = lireCapteurX();
  // etc.
  delay(100);
}

// Envoie les positions quand demandé
void requestEvents() {
  DoubleBytes db;
  db.value = currentPos.x;
  Wire.write(db.bytes, 8);
  db.value = currentPos.y;
  Wire.write(db.bytes, 8);
  db.value = currentPos.z;
  Wire.write(db.bytes, 8);
}

// Reçoit des données du maître si besoin
void receiveEvents(int numBytes) {
  // Optionnel : recevoir des commandes
}