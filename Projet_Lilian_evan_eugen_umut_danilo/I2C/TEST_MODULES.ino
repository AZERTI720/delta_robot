// ============================================================
// CODE DE TEST - Pour valider chaque module séparément
// À utiliser pendant la phase de calibrage
// ============================================================

// ============================================================
// TEST 1: SCAN I2C - Charger sur le Maître
// ============================================================

#include <Wire.h>

void setup() {
  Wire.begin();
  Serial.begin(9600);
  delay(2000);
  Serial.println("=== I2C SCAN TEST ===");
}

void loop() {
  Serial.println("\nScanning I2C bus...");
  int found = 0;
  
  for(int addr = 1; addr < 127; addr++) {
    Wire.beginTransmission(addr);
    int error = Wire.endTransmission();
    
    if (error == 0) {
      Serial.print("✓ Device found at address: ");
      Serial.print(addr);
      Serial.print(" (0x");
      Serial.print(addr, HEX);
      Serial.println(")");
      found++;
    }
  }
  
  Serial.print("\nTotal devices found: ");
  Serial.println(found);
  Serial.println("\nExpected: 11 (Delta), 12 (Pixy), 13 (Tapis)");
  
  delay(5000);
}

// ============================================================
// TEST 2: COMMUNICATION DELTA - Charger sur le Maître
// ============================================================

/*
#include <Wire.h>

void setup() {
  Wire.begin();
  Serial.begin(9600);
  delay(2000);
  Serial.println("=== DELTA COMMUNICATION TEST ===");
}

void loop() {
  Serial.println("\n--- Test 1: Demander statut ---");
  Wire.beginTransmission(11);
  Wire.write(0x06);  // CMD_STATUS
  int error = Wire.endTransmission();
  
  if (error == 0) {
    Wire.requestFrom(11, 1);
    if (Wire.available()) {
      uint8_t status = Wire.read();
      Serial.print("Status: 0x");
      Serial.println(status, HEX);
    }
  } else {
    Serial.print("ERROR: ");
    Serial.println(error);
  }
  
  delay(2000);
  
  Serial.println("\n--- Test 2: Envoyer position ---");
  float x = -75, y = -30, z = -120;
  
  union FloatBytes {
    float value;
    byte bytes[4];
  } fbX, fbY, fbZ;
  
  fbX.value = x;
  fbY.value = y;
  fbZ.value = z;
  
  Wire.beginTransmission(11);
  Wire.write(0x01);  // CMD_MOVE_XYZ
  Wire.write(fbX.bytes, 4);
  Wire.write(fbY.bytes, 4);
  Wire.write(fbZ.bytes, 4);
  error = Wire.endTransmission();
  
  if (error == 0) {
    Serial.println("Position sent successfully");
  } else {
    Serial.print("ERROR: ");
    Serial.println(error);
  }
  
  delay(3000);
  
  Serial.println("\n--- Test 3: Ventouse ---");
  
  Serial.println("Closing gripper...");
  Wire.beginTransmission(11);
  Wire.write(0x04);  // CMD_GRIPPER_CLOSE
  Wire.endTransmission();
  delay(1000);
  
  Serial.println("Opening gripper...");
  Wire.beginTransmission(11);
  Wire.write(0x03);  // CMD_GRIPPER_OPEN
  Wire.endTransmission();
  
  delay(5000);
}
*/

// ============================================================
// TEST 3: COMMUNICATION PIXY - Charger sur le Maître
// ============================================================

/*
#include <Wire.h>

void setup() {
  Wire.begin();
  Serial.begin(9600);
  delay(2000);
  Serial.println("=== PIXY COMMUNICATION TEST ===");
}

void loop() {
  Serial.println("\n--- Requesting Pixy position ---");
  
  Wire.beginTransmission(12);
  Wire.write(0x10);  // CMD_GET_POSITION
  int error = Wire.endTransmission();
  
  if (error == 0) {
    Wire.requestFrom(12, 4);
    
    if (Wire.available() >= 4) {
      uint16_t pixelX = ((uint16_t)Wire.read() << 8) | Wire.read();
      uint16_t pixelY = ((uint16_t)Wire.read() << 8) | Wire.read();
      
      if (pixelX == 0xFFFF && pixelY == 0xFFFF) {
        Serial.println("No object detected");
      } else {
        Serial.print("Pixel X: ");
        Serial.print(pixelX);
        Serial.print(", Y: ");
        Serial.println(pixelY);
      }
    }
  } else {
    Serial.print("ERROR: ");
    Serial.println(error);
  }
  
  delay(1000);
}
*/

// ============================================================
// TEST 4: COMMUNICATION TAPIS - Charger sur le Maître
// ============================================================

/*
#include <Wire.h>

void setup() {
  Wire.begin();
  Serial.begin(9600);
  delay(2000);
  Serial.println("=== TAPIS COMMUNICATION TEST ===");
}

void loop() {
  // Test 1: Fixer vitesse à 50%
  Serial.println("\n--- Setting speed to 50% ---");
  Wire.beginTransmission(13);
  Wire.write(0x20);  // CMD_SET_SPEED
  Wire.write(0x00);  // MSB
  Wire.write(127);   // LSB (50% of 255)
  int error = Wire.endTransmission();
  
  if (error == 0) {
    Serial.println("Speed set successfully");
  } else {
    Serial.print("ERROR: ");
    Serial.println(error);
  }
  
  delay(1000);
  
  // Test 2: Démarrer moteur
  Serial.println("\n--- Starting motor ---");
  Wire.beginTransmission(13);
  Wire.write(0x21);  // CMD_START
  error = Wire.endTransmission();
  
  if (error == 0) {
    Serial.println("Motor started");
  } else {
    Serial.print("ERROR: ");
    Serial.println(error);
  }
  
  delay(3000);
  
  // Test 3: Demander statut
  Serial.println("\n--- Getting status ---");
  Wire.beginTransmission(13);
  Wire.write(0x24);  // CMD_GET_STATUS
  Wire.endTransmission();
  
  Wire.requestFrom(13, 1);
  if (Wire.available()) {
    uint8_t status = Wire.read();
    Serial.print("Status: ");
    Serial.println(status == 0x01 ? "RUNNING" : "STOPPED");
  }
  
  delay(2000);
  
  // Test 4: Arrêter moteur
  Serial.println("\n--- Stopping motor ---");
  Wire.beginTransmission(13);
  Wire.write(0x22);  // CMD_STOP
  error = Wire.endTransmission();
  
  if (error == 0) {
    Serial.println("Motor stopped");
  } else {
    Serial.print("ERROR: ");
    Serial.println(error);
  }
  
  delay(5000);
}
*/

// ============================================================
// TEST 5: CYCLE COMPLET SIMPLIFIÉ - Charger sur le Maître
// ============================================================

/*
#include <Wire.h>

void setup() {
  Wire.begin();
  Serial.begin(9600);
  delay(2000);
  Serial.println("=== COMPLETE CYCLE TEST ===");
  Serial.println("Place a cube in front of Pixy...");
  delay(3000);
}

void loop() {
  Serial.println("\n========== CYCLE START ==========");
  
  // ÉTAPE 1: Détecte position du cube
  Serial.println("\n1. Detecting cube position...");
  Wire.beginTransmission(12);
  Wire.write(0x10);
  Wire.endTransmission();
  
  Wire.requestFrom(12, 4);
  uint16_t pixelX = ((uint16_t)Wire.read() << 8) | Wire.read();
  uint16_t pixelY = ((uint16_t)Wire.read() << 8) | Wire.read();
  
  if (pixelX == 0xFFFF) {
    Serial.println("   ✗ No cube detected!");
    delay(1000);
    return;
  }
  
  Serial.print("   ✓ Cube detected at pixel(");
  Serial.print(pixelX);
  Serial.print(", ");
  Serial.print(pixelY);
  Serial.println(")");
  
  // ÉTAPE 2: Démarre tapis
  Serial.println("\n2. Starting conveyor belt...");
  Wire.beginTransmission(13);
  Wire.write(0x20);  // Set speed
  Wire.write(0x00);
  Wire.write(150);   // 60% speed
  Wire.endTransmission();
  
  Wire.beginTransmission(13);
  Wire.write(0x21);  // Start
  Wire.endTransmission();
  Serial.println("   ✓ Conveyor started");
  
  delay(1000);
  
  // ÉTAPE 3: Déplace delta vers cube
  Serial.println("\n3. Moving delta to cube...");
  float x = -50, y = -80, z = -120;  // Coordonnées adaptées
  
  union FloatBytes {
    float value;
    byte bytes[4];
  } fbX, fbY, fbZ;
  
  fbX.value = x;
  fbY.value = y;
  fbZ.value = z;
  
  Wire.beginTransmission(11);
  Wire.write(0x01);
  Wire.write(fbX.bytes, 4);
  Wire.write(fbY.bytes, 4);
  Wire.write(fbZ.bytes, 4);
  Wire.endTransmission();
  Serial.println("   ✓ Delta moving");
  
  delay(2000);
  
  // ÉTAPE 4: Ferme ventouse
  Serial.println("\n4. Closing gripper...");
  Wire.beginTransmission(11);
  Wire.write(0x04);
  Wire.endTransmission();
  Serial.println("   ✓ Gripper closed");
  
  delay(1000);
  
  // ÉTAPE 5: Déplace vers position dépôt
  Serial.println("\n5. Moving to drop position...");
  fbX.value = 0;
  fbY.value = -100;
  fbZ.value = -120;
  
  Wire.beginTransmission(11);
  Wire.write(0x01);
  Wire.write(fbX.bytes, 4);
  Wire.write(fbY.bytes, 4);
  Wire.write(fbZ.bytes, 4);
  Wire.endTransmission();
  Serial.println("   ✓ Moving to drop");
  
  delay(2000);
  
  // ÉTAPE 6: Ouvre ventouse
  Serial.println("\n6. Opening gripper...");
  Wire.beginTransmission(11);
  Wire.write(0x03);
  Wire.endTransmission();
  Serial.println("   ✓ Gripper opened");
  
  delay(1000);
  
  // ÉTAPE 7: Arrête tapis
  Serial.println("\n7. Stopping conveyor...");
  Wire.beginTransmission(13);
  Wire.write(0x22);
  Wire.endTransmission();
  Serial.println("   ✓ Conveyor stopped");
  
  delay(1000);
  
  // ÉTAPE 8: Retour position initiale
  Serial.println("\n8. Returning to home position...");
  fbX.value = -75;
  fbY.value = -30;
  fbZ.value = -120;
  
  Wire.beginTransmission(11);
  Wire.write(0x01);
  Wire.write(fbX.bytes, 4);
  Wire.write(fbY.bytes, 4);
  Wire.write(fbZ.bytes, 4);
  Wire.endTransmission();
  Serial.println("   ✓ Returning home");
  
  delay(2000);
  
  Serial.println("\n========== CYCLE COMPLETE ==========");
  delay(5000);
}
*/
