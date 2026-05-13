#include <Wire.h>

// Adresses I2C des esclaves
#define SLAVE_DELTA 11
#define SLAVE_PIXY  12
#define SLAVE_TAPIS 13

// Commandes Delta
#define CMD_DELTA_MOVE_XYZ      0x01
#define CMD_DELTA_GRIPPER_OPEN  0x03
#define CMD_DELTA_GRIPPER_CLOSE 0x04
#define CMD_DELTA_STATUS        0x06

// Commandes Pixy
#define CMD_PIXY_GET_POSITION   0x10

// Commandes Tapis
#define CMD_TAPIS_SET_SPEED     0x20
#define CMD_TAPIS_START         0x21
#define CMD_TAPIS_STOP          0x22
#define CMD_TAPIS_GET_STATUS    0x24

// Statuts
#define STATUS_OK               0xAA
#define STATUS_MOVING           0xBB
#define STATUS_ERROR            0xEE
#define TAPIS_RUNNING           0x01
#define TAPIS_STOPPED           0x00

union FloatBytes {
  float value;
  uint8_t bytes[4];
};

uint16_t pixelX = 0;
uint16_t pixelY = 0;
float targetX = 0;
float targetY = 0;
float targetZ = -120.0;

void setup() {
  Serial.begin(9600);
  while (!Serial) {}

  Wire.begin();
  Serial.println("=== MAÎTRE I2C - Exemple ===");
  delay(500);

  testSlaveConnection(SLAVE_PIXY, "Pixy");
  testSlaveConnection(SLAVE_DELTA, "Delta");
  testSlaveConnection(SLAVE_TAPIS, "Tapis");
  Serial.println("--- Prêt ---\n");
}

void loop() {
  if (!requestPixyPosition(pixelX, pixelY)) {
    Serial.println("Pixy: aucun cube ou erreur I2C");
    delay(500);
    return;
  }

  Serial.print("Pixy position: ");
  Serial.print(pixelX);
  Serial.print(" , ");
  Serial.println(pixelY);

  pixelToRobotCoordinates(pixelX, pixelY, targetX, targetY);
  Serial.print("Position Delta calculée: ");
  Serial.print(targetX);
  Serial.print(" , ");
  Serial.println(targetY);

  // Envoi de la vitesse au tapis en temps réel
  setTapisSpeed(160); // Valeur 0-255
  startTapis();
  delay(300);

  // Déplacement du Delta vers le cube
  moveDeltaTo(targetX, targetY, targetZ);
  waitForDeltaStop(3000);

  // Ferme la ventouse
  deltaGripperClose();
  delay(400);

  // Déplacement vers la zone de dépôt
  moveDeltaTo(0.0, -100.0, targetZ);
  waitForDeltaStop(3000);

  // Relâche le cube
  deltaGripperOpen();
  delay(400);

  stopTapis();
  delay(300);

  // Retour position de repos
  moveDeltaTo(-75.0, -30.0, targetZ);
  waitForDeltaStop(3000);

  Serial.println("Cycle terminé, attente avant prochain cube...\n");
  delay(2000);
}

bool requestPixyPosition(uint16_t &x, uint16_t &y) {
  Wire.beginTransmission(SLAVE_PIXY);
  Wire.write(CMD_PIXY_GET_POSITION);
  if (Wire.endTransmission() != 0) {
    Serial.println("Erreur I2C en envoyant la commande Pixy");
    return false;
  }

  Wire.requestFrom(SLAVE_PIXY, (uint8_t)4);
  if (Wire.available() < 4) {
    Serial.println("Pas assez de bytes reçus de Pixy");
    return false;
  }

  x = ((uint16_t)Wire.read() << 8) | Wire.read();
  y = ((uint16_t)Wire.read() << 8) | Wire.read();
  return true;
}

void pixelToRobotCoordinates(uint16_t px, uint16_t py, float &rx, float &ry) {
  // Ajuster les coefficients selon votre calibration réelle
  rx = (px - 160) * 0.45;  // 0.45 mm/pixel
  ry = (py - 100) * 0.35;

  rx = constrain(rx, -100.0, 100.0);
  ry = constrain(ry, -150.0, 0.0);
}

bool moveDeltaTo(float x, float y, float z) {
  FloatBytes fx, fy, fz;
  fx.value = x;
  fy.value = y;
  fz.value = z;

  Wire.beginTransmission(SLAVE_DELTA);
  Wire.write(CMD_DELTA_MOVE_XYZ);
  Wire.write(fx.bytes, 4);
  Wire.write(fy.bytes, 4);
  Wire.write(fz.bytes, 4);
  uint8_t error = Wire.endTransmission();

  if (error != 0) {
    Serial.print("Erreur I2C Delta MOVE: ");
    Serial.println(error);
    return false;
  }

  Serial.println("Commande Delta envoyée");
  return true;
}

void deltaGripperOpen() {
  Wire.beginTransmission(SLAVE_DELTA);
  Wire.write(CMD_DELTA_GRIPPER_OPEN);
  Wire.endTransmission();
  Serial.println("Delta: Ouvre la ventouse");
}

void deltaGripperClose() {
  Wire.beginTransmission(SLAVE_DELTA);
  Wire.write(CMD_DELTA_GRIPPER_CLOSE);
  Wire.endTransmission();
  Serial.println("Delta: Ferme la ventouse");
}

bool waitForDeltaStop(unsigned long timeoutMs) {
  unsigned long start = millis();
  while (millis() - start < timeoutMs) {
    Wire.beginTransmission(SLAVE_DELTA);
    Wire.write(CMD_DELTA_STATUS);
    Wire.endTransmission();

    Wire.requestFrom(SLAVE_DELTA, (uint8_t)1);
    if (Wire.available()) {
      uint8_t status = Wire.read();
      if (status == STATUS_OK) {
        Serial.println("Delta: Arrêté / prêt");
        return true;
      }
      if (status == STATUS_ERROR) {
        Serial.println("Delta: Erreur détectée");
        return false;
      }
    }
    delay(200);
  }
  Serial.println("Delta: timeout attente stop");
  return false;
}

void setTapisSpeed(uint8_t speed) {
  Wire.beginTransmission(SLAVE_TAPIS);
  Wire.write(CMD_TAPIS_SET_SPEED);
  Wire.write((uint8_t)0x00);
  Wire.write(speed);
  Wire.endTransmission();
  Serial.print("Tapis: vitesse envoyée = ");
  Serial.println(speed);
}

void startTapis() {
  Wire.beginTransmission(SLAVE_TAPIS);
  Wire.write(CMD_TAPIS_START);
  Wire.endTransmission();
  Serial.println("Tapis: démarrage");
}

void stopTapis() {
  Wire.beginTransmission(SLAVE_TAPIS);
  Wire.write(CMD_TAPIS_STOP);
  Wire.endTransmission();
  Serial.println("Tapis: arrêt");
}

uint8_t getTapisStatus() {
  Wire.beginTransmission(SLAVE_TAPIS);
  Wire.write(CMD_TAPIS_GET_STATUS);
  Wire.endTransmission();

  Wire.requestFrom(SLAVE_TAPIS, (uint8_t)1);
  if (Wire.available()) {
    return Wire.read();
  }
  return 0xFF;
}

void testSlaveConnection(uint8_t addr, const char *name) {
  Wire.beginTransmission(addr);
  uint8_t error = Wire.endTransmission();
  Serial.print(name);
  Serial.print(" (addr ");
  Serial.print(addr);
  Serial.print("): ");
  if (error == 0) {
    Serial.println("connecté");
  } else {
    Serial.println("non trouvé");
  }
}
