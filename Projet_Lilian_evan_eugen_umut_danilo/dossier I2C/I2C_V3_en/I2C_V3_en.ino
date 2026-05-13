#include <Pixy2.h>
#include <Wire.h>

Pixy2 pixy;
const int server = 11;

// Variables globales pour stocker la position
int posX = 0;
int posY = 0;

void setup() {
  Serial.begin(9600);
  pixy.init();
  
  Wire.begin(server);
  // On attache la fonction de réponse au Maître
  Wire.onRequest(requestEvent); 
  
  Serial.println("Carte Arduino 1 prêt (Pixy2)");
}

void loop() {
  // On récupère les blocs de la Pixy
  pixy.ccc.getBlocks();
  
  if (pixy.ccc.numBlocks) {
    posX = pixy.ccc.blocks[0].m_x;
    posY = pixy.ccc.blocks[0].m_y;
  } else {
    // Si rien n'est détecté, on met à 0 ou une valeur par défaut
    posX = 0;
    posY = 0;
  }
}

// Fonction appelée par le Maître via le bus I2C
void requestEvent() {
  uint8_t buffer[4];
  
  // On décompose les INT (16 bits) en BYTES (8 bits)
  buffer[0] = (posX >> 8) & 0xFF; // X poids fort
  buffer[1] = posX & 0xFF;        // X poids faible
  buffer[2] = (posY >> 8) & 0xFF; // Y poids fort
  buffer[3] = posY & 0xFF;        // Y poids faible

  // On envoie les 4 octets d'un coup
  Wire.write(buffer, 4);
}
