// ============================================================
// ARDUINO ESCLAVE #12 - Caméra Pixy2
// Adresse I2C: 0x0C (12)
// Rôle: Détecte la position des cubes via Pixy2
// ============================================================

#include <Wire.h>
#include <Pixy2.h>

// Constants
#define MY_ADDRESS 12

// Commandes
#define CMD_GET_POSITION    0x10
#define CMD_SET_TRACKING    0x11
#define CMD_GET_BLOCK_COUNT 0x12
#define CMD_PIXY_STATUS     0x13

// États
#define TRACKING_ON  0x01
#define TRACKING_OFF 0x00
#define NO_OBJECT    0xFF

// Variables globales
Pixy2 pixy;
uint16_t lastPixelX = 0, lastPixelY = 0;
uint8_t blockCount = 0;
bool trackingEnabled = true;
int32_t blockX = 0, blockY = 0;

void setup() {
  Serial.begin(9600);
  
  // Initialise Pixy2 en mode SPI (par défaut)
  pixy.init();
  
  // Initialise I2C comme esclave
  Wire.begin(MY_ADDRESS);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
  
  Serial.println("=== ESCLAVE PIXY Ready (Addr 12) ===");
  delay(1000);
}

void loop() {
  // Met à jour la position des blocs
  if (trackingEnabled) {
    pixy.ccc.getBlocks();
    
    if (pixy.ccc.numBlocks) {
      blockCount = pixy.ccc.numBlocks;
      
      // Prend le bloc le plus grand (indice 0)
      blockX = pixy.ccc.blocks[0].m_x;
      blockY = pixy.ccc.blocks[0].m_y;
      
      lastPixelX = blockX;
      lastPixelY = blockY;
      
      Serial.print("Bloc détecté: X=");
      Serial.print(blockX);
      Serial.print(", Y=");
      Serial.print(blockY);
      Serial.print(", Signatu=");
      Serial.println(pixy.ccc.blocks[0].m_signature);
      
    } else {
      blockCount = 0;
      Serial.println("Aucun bloc détecté");
    }
  }
  
  delay(50);
}

// ============================================================
// RÉCEPTION DES COMMANDES I2C
// ============================================================

void receiveEvent(int numBytes) {
  if (numBytes < 1) return;
  
  uint8_t command = Wire.read();
  
  switch (command) {
    case CMD_GET_POSITION:
      // Juste flag pour signaler que la requête est reçue
      // La réponse sera envoyée par requestEvent()
      Serial.println("CMD: GET_POSITION");
      break;
    
    case CMD_SET_TRACKING:
      if (numBytes >= 2) {
        uint8_t enabled = Wire.read();
        trackingEnabled = (enabled == 1);
        Serial.print("Tracking: ");
        Serial.println(trackingEnabled ? "ON" : "OFF");
      }
      break;
    
    case CMD_GET_BLOCK_COUNT:
      Serial.println("CMD: GET_BLOCK_COUNT");
      break;
    
    case CMD_PIXY_STATUS:
      Serial.println("CMD: PIXY_STATUS");
      break;
  }
  
  // Vider le buffer
  while (Wire.available()) Wire.read();
}

// ============================================================
// ENVOI DES DONNÉES I2C
// ============================================================

void requestEvent() {
  // Retourne la position du bloc détecté
  // Format: X(2 bytes) + Y(2 bytes)
  
  if (blockCount > 0) {
    // Bloc détecté - envoyer les coordonnées
    Wire.write((uint8_t)((lastPixelX >> 8) & 0xFF));  // X high byte
    Wire.write((uint8_t)(lastPixelX & 0xFF));          // X low byte
    Wire.write((uint8_t)((lastPixelY >> 8) & 0xFF));  // Y high byte
    Wire.write((uint8_t)(lastPixelY & 0xFF));          // Y low byte
  } else {
    // Pas de bloc - envoyer 0xFFFF
    Wire.write(0xFF);
    Wire.write(0xFF);
    Wire.write(0xFF);
    Wire.write(0xFF);
  }
}

// ============================================================
// FONCTIONS UTILITAIRES
// ============================================================

void printBlock(int blockIndex) {
  Serial.print("  Block ");
  Serial.println(blockIndex);
  Serial.print("    Signature: ");
  Serial.println(pixy.ccc.blocks[blockIndex].m_signature);
  Serial.print("    X: ");
  Serial.print(pixy.ccc.blocks[blockIndex].m_x);
  Serial.print("    Y: ");
  Serial.print(pixy.ccc.blocks[blockIndex].m_y);
  Serial.print("    Width: ");
  Serial.print(pixy.ccc.blocks[blockIndex].m_width);
  Serial.print("    Height: ");
  Serial.println(pixy.ccc.blocks[blockIndex].m_height);
}
