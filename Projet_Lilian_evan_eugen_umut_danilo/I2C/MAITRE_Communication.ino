// ============================================================
// ARDUINO MAÎTRE - Communication Principal
// Adresse I2C : Maître (sans adresse)
// Rôle : Pilote les 3 esclaves et orchestre le tri des cubes
// ============================================================

#include <Wire.h>

// Adresses I2C des esclaves
#define SLAVE_DELTA 11
#define SLAVE_PIXY  12
#define SLAVE_TAPIS 13

// Constantes de commande
#define CMD_DELTA_MOVE_XYZ      0x01
#define CMD_DELTA_GRIPPER_OPEN  0x03
#define CMD_DELTA_GRIPPER_CLOSE 0x04
#define CMD_DELTA_STOP          0x05
#define CMD_DELTA_STATUS        0x06
#define CMD_DELTA_SPEED_SET     0x07

#define CMD_PIXY_GET_POSITION   0x10
#define CMD_PIXY_SET_TRACKING   0x11
#define CMD_PIXY_GET_BLOCK_COUNT 0x12
#define CMD_PIXY_STATUS         0x13

#define CMD_TAPIS_SET_SPEED     0x20
#define CMD_TAPIS_START         0x21
#define CMD_TAPIS_STOP          0x22
#define CMD_TAPIS_DIRECTION     0x23
#define CMD_TAPIS_GET_STATUS    0x24
#define CMD_TAPIS_EMERGENCY_STOP 0x25

// États
#define STATE_IDLE              0
#define STATE_DETECTING         1
#define STATE_MOVING_DELTA      2
#define STATE_GRIPPING          3
#define STATE_PLACING           4

// Statuts I2C
#define STATUS_OK               0xAA
#define STATUS_MOVING           0xBB
#define STATUS_ERROR            0xEE
#define STATUS_ACK              0xCC

// Union pour convertir float en bytes
union FloatBytes {
  float value;
  byte bytes[4];
};

// Variables globales
uint8_t currentState = STATE_IDLE;
uint16_t pixelX = 0, pixelY = 0;
float targetX = 0, targetY = 0, targetZ = -120;
unsigned long lastCommandTime = 0;
#define I2C_TIMEOUT 500  // ms

void setup() {
  Wire.begin();           // Initialise comme MAÎTRE
  Serial.begin(9600);
  
  Serial.println("=== ARDUINO MAÎTRE - Communication ===");
  Serial.println("Initialisation des esclaves...");
  delay(2000);
  
  // Test connexion avec chaque esclave
  testSlaveConnection(SLAVE_DELTA, "Delta");
  testSlaveConnection(SLAVE_PIXY, "Pixy");
  testSlaveConnection(SLAVE_TAPIS, "Tapis");
  
  Serial.println("Système prêt!");
  currentState = STATE_IDLE;
}

void loop() {
  // Cycle de tri principal
  cycleTri();
}

// ============================================================
// CYCLE PRINCIPAL DE TRI
// ============================================================

void cycleTri() {
  static unsigned long lastDetection = 0;
  
  // Étape 1 : Détecte la position du cube via Pixy
  Serial.println("\n--- Détection cube ---");
  if (getPixyPosition()) {
    Serial.print("Cube détecté à Pixel(");
    Serial.print(pixelX);
    Serial.print(", ");
    Serial.print(pixelY);
    Serial.println(")");
    
    // Transforme coordonnées pixel en coordonnées robot (à calibrer!)
    pixelToRobotCoordinates(pixelX, pixelY, targetX, targetY);
    
    Serial.print("Position robot calculée: X=");
    Serial.print(targetX);
    Serial.print(", Y=");
    Serial.print(targetY);
    Serial.println(", Z=-120");
    
    // Étape 2 : Démarre le tapis roulant
    Serial.println("Démarrage du tapis...");
    setTapisSpeed(150);  // 60% vitesse
    delay(500);
    
    // Étape 3 : Déplace le delta vers la position du cube
    Serial.println("Déplacement vers le cube...");
    moveDeltaTo(targetX, targetY, -120);
    
    // Attendre que le delta arrive
    delay(2000);
    
    // Étape 4 : Ferme la ventouse
    Serial.println("Fermeture ventouse...");
    deltaGripperClose();
    delay(500);
    
    // Étape 5 : Déplace vers position de dépôt
    Serial.println("Déplacement vers position de dépôt...");
    moveDeltaTo(0, -100, -120);  // Position fixe de dépôt
    delay(2000);
    
    // Étape 6 : Ouvre la ventouse
    Serial.println("Ouverture ventouse...");
    deltaGripperOpen();
    delay(500);
    
    // Étape 7 : Arrête le tapis
    Serial.println("Arrêt du tapis...");
    setTapisStop();
    delay(500);
    
    // Retour position initiale
    Serial.println("Retour position initiale...");
    moveDeltaTo(-75, -30, -120);
    delay(1000);
    
    lastDetection = millis();
  } else {
    // Pas de cube détecté - continue le tapis si nécessaire
    if (millis() - lastDetection > 10000) {
      // 10 secondes sans détection
      setTapisStop();
    }
  }
  
  delay(500);
}

// ============================================================
// COMMUNICATION DELTA (Adresse 11)
// ============================================================

bool moveDeltaTo(float x, float y, float z) {
  FloatBytes fbX, fbY, fbZ;
  fbX.value = x;
  fbY.value = y;
  fbZ.value = z;
  
  Wire.beginTransmission(SLAVE_DELTA);
  Wire.write(CMD_DELTA_MOVE_XYZ);
  Wire.write(fbX.bytes, 4);
  Wire.write(fbY.bytes, 4);
  Wire.write(fbZ.bytes, 4);
  
  uint8_t error = Wire.endTransmission();
  
  if (error == 0) {
    Serial.println("✓ Commande Delta envoyée");
    return true;
  } else {
    Serial.print("✗ Erreur I2C Delta: ");
    Serial.println(error);
    return false;
  }
}

bool deltaGripperOpen() {
  Wire.beginTransmission(SLAVE_DELTA);
  Wire.write(CMD_DELTA_GRIPPER_OPEN);
  uint8_t error = Wire.endTransmission();
  return (error == 0);
}

bool deltaGripperClose() {
  Wire.beginTransmission(SLAVE_DELTA);
  Wire.write(CMD_DELTA_GRIPPER_CLOSE);
  uint8_t error = Wire.endTransmission();
  return (error == 0);
}

uint8_t getDeltaStatus() {
  Wire.beginTransmission(SLAVE_DELTA);
  Wire.write(CMD_DELTA_STATUS);
  Wire.endTransmission();
  
  Wire.requestFrom(SLAVE_DELTA, 1);
  if (Wire.available()) {
    return Wire.read();
  }
  return STATUS_ERROR;
}

// ============================================================
// COMMUNICATION PIXY (Adresse 12)
// ============================================================

bool getPixyPosition() {
  Wire.beginTransmission(SLAVE_PIXY);
  Wire.write(CMD_PIXY_GET_POSITION);
  Wire.endTransmission();
  
  // Attend la réponse: X(2B) + Y(2B)
  Wire.requestFrom(SLAVE_PIXY, 4);
  
  if (Wire.available() >= 4) {
    pixelX = (Wire.read() << 8) | Wire.read();
    pixelY = (Wire.read() << 8) | Wire.read();
    return true;
  }
  
  return false;
}

uint8_t getPixyBlockCount() {
  Wire.beginTransmission(SLAVE_PIXY);
  Wire.write(CMD_PIXY_GET_BLOCK_COUNT);
  Wire.endTransmission();
  
  Wire.requestFrom(SLAVE_PIXY, 1);
  if (Wire.available()) {
    return Wire.read();
  }
  return 0;
}

// ============================================================
// COMMUNICATION TAPIS (Adresse 13)
// ============================================================

bool setTapisSpeed(uint8_t speed) {  // 0-255
  Wire.beginTransmission(SLAVE_TAPIS);
  Wire.write(CMD_TAPIS_SET_SPEED);
  Wire.write(0x00);
  Wire.write(speed);
  
  uint8_t error = Wire.endTransmission();
  return (error == 0);
}

bool setTapisStart() {
  Wire.beginTransmission(SLAVE_TAPIS);
  Wire.write(CMD_TAPIS_START);
  uint8_t error = Wire.endTransmission();
  return (error == 0);
}

bool setTapisStop() {
  Wire.beginTransmission(SLAVE_TAPIS);
  Wire.write(CMD_TAPIS_STOP);
  uint8_t error = Wire.endTransmission();
  return (error == 0);
}

uint8_t getTapisStatus() {
  Wire.beginTransmission(SLAVE_TAPIS);
  Wire.write(CMD_TAPIS_GET_STATUS);
  Wire.endTransmission();
  
  Wire.requestFrom(SLAVE_TAPIS, 1);
  if (Wire.available()) {
    return Wire.read();
  }
  return STATUS_ERROR;
}

// ============================================================
// FONCTIONS UTILITAIRES
// ============================================================

void testSlaveConnection(uint8_t addr, const char* name) {
  Wire.beginTransmission(addr);
  uint8_t error = Wire.endTransmission();
  
  Serial.print(name);
  Serial.print(" (addr ");
  Serial.print(addr);
  Serial.print("): ");
  
  if (error == 0) {
    Serial.println("✓ Connecté");
  } else {
    Serial.println("✗ Non trouvé!");
  }
}

void pixelToRobotCoordinates(uint16_t px, uint16_t py, float &rx, float &ry) {
  // CALIBRATION À FAIRE !
  // Convertir coordonnées Pixy (0-319 en X, 0-199 en Y)
  // vers coordonnées robot en mm
  
  // Valeurs par défaut - À ADAPTER À VOS CALIBRAGES!
  rx = (px - 160) * 0.4;   // Centre à 160, échelle 0.4 mm/pixel
  ry = (py - 100) * 0.3;   // Centre à 100, échelle 0.3 mm/pixel
  
  // Limiter à zone de travail du delta (exemple)
  rx = constrain(rx, -100, 100);
  ry = constrain(ry, -150, 0);
}

void printI2CError(uint8_t error) {
  switch (error) {
    case 0: Serial.println("Success"); break;
    case 1: Serial.println("Data too long"); break;
    case 2: Serial.println("NACK on address"); break;
    case 3: Serial.println("NACK on data"); break;
    case 4: Serial.println("Other error"); break;
    default: Serial.println("Unknown error");
  }
}
