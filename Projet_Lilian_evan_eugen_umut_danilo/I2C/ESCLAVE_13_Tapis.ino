// ============================================================
// ARDUINO ESCLAVE #13 - Tapis Roulant
// Adresse I2C: 0x0D (13)
// Rôle: Contrôle la vitesse et la direction du tapis
// ============================================================

#include <Wire.h>

// Constants
#define MY_ADDRESS 13

// Pins moteur
#define MOTOR_PWM   5   // Pin PWM pour la vitesse
#define MOTOR_DIR   6   // Pin pour la direction
#define MOTOR_EN    7   // Pin enable (optionnel)

// Commandes I2C
#define CMD_SET_SPEED       0x20
#define CMD_START           0x21
#define CMD_STOP            0x22
#define CMD_DIRECTION       0x23
#define CMD_GET_STATUS      0x24
#define CMD_EMERGENCY_STOP  0x25

// États
#define ACK      0xCC
#define RUNNING  0x01
#define STOPPED  0x00
#define ERROR    0xEE

// Direction
#define FORWARD  0
#define REVERSE  1

// Variables globales
uint8_t currentSpeed = 0;      // 0-255
uint8_t currentDirection = FORWARD;
bool isRunning = false;

void setup() {
  // Configuration des pins
  pinMode(MOTOR_PWM, OUTPUT);
  pinMode(MOTOR_DIR, OUTPUT);
  pinMode(MOTOR_EN, OUTPUT);
  
  // État initial
  digitalWrite(MOTOR_PWM, LOW);
  digitalWrite(MOTOR_DIR, FORWARD);
  digitalWrite(MOTOR_EN, LOW);  // Moteur désactivé
  
  Serial.begin(9600);
  
  // I2C setup
  Wire.begin(MY_ADDRESS);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
  
  Serial.println("=== ESCLAVE TAPIS Ready (Addr 13) ===");
  Serial.println("PWM pin: 5, DIR pin: 6, EN pin: 7");
}

void loop() {
  // Peut ajouter des fonctionnalités d'monitoring ici
  
  // Exemple: afficher l'état toutes les 5 secondes
  static unsigned long lastStatus = 0;
  if (millis() - lastStatus > 5000) {
    Serial.print("Tapis - Vitesse: ");
    Serial.print(currentSpeed);
    Serial.print(", Direction: ");
    Serial.print(currentDirection ? "REVERSE" : "FORWARD");
    Serial.print(", État: ");
    Serial.println(isRunning ? "RUNNING" : "STOPPED");
    lastStatus = millis();
  }
  
  delay(100);
}

// ============================================================
// RÉCEPTION DES COMMANDES I2C
// ============================================================

void receiveEvent(int numBytes) {
  if (numBytes < 1) return;
  
  uint8_t command = Wire.read();
  
  switch (command) {
    case CMD_SET_SPEED:
      if (numBytes >= 3) {
        // Format: CMD + MSB + LSB (2 bytes speed)
        uint16_t speed_raw = ((uint16_t)Wire.read() << 8) | Wire.read();
        setSpeed(speed_raw);
      }
      break;
    
    case CMD_START:
      startMotor();
      Serial.println("Tapis START");
      break;
    
    case CMD_STOP:
      stopMotor();
      Serial.println("Tapis STOP");
      break;
    
    case CMD_DIRECTION:
      if (numBytes >= 2) {
        uint8_t dir = Wire.read();
        setDirection(dir);
        Serial.print("Direction set to: ");
        Serial.println(dir ? "REVERSE" : "FORWARD");
      }
      break;
    
    case CMD_GET_STATUS:
      // Status sera envoyé par requestEvent()
      Serial.println("Status request");
      break;
    
    case CMD_EMERGENCY_STOP:
      emergencyStop();
      Serial.println("!!! EMERGENCY STOP !!!");
      break;
  }
  
  // Vider le buffer I2C
  while (Wire.available()) Wire.read();
}

// ============================================================
// ENVOI DES DONNÉES I2C
// ============================================================

void requestEvent() {
  // Retourne l'état du tapis
  if (isRunning) {
    Wire.write(RUNNING);
  } else {
    Wire.write(STOPPED);
  }
}

// ============================================================
// FONCTIONS DE CONTRÔLE
// ============================================================

void setSpeed(uint16_t speed) {
  // Limiter à plage 0-255 (PWM)
  if (speed > 255) {
    currentSpeed = 255;
  } else {
    currentSpeed = (uint8_t)speed;
  }
  
  analogWrite(MOTOR_PWM, currentSpeed);
  
  Serial.print("Speed set to: ");
  Serial.print(currentSpeed);
  Serial.print(" (");
  Serial.print((currentSpeed * 100) / 255);
  Serial.println("%)");
}

void startMotor() {
  digitalWrite(MOTOR_EN, HIGH);  // Activer le moteur
  isRunning = true;
  
  // S'assurer que la vitesse n'est pas zéro
  if (currentSpeed == 0) {
    currentSpeed = 150;  // Vitesse par défaut
    analogWrite(MOTOR_PWM, currentSpeed);
  }
}

void stopMotor() {
  analogWrite(MOTOR_PWM, 0);
  digitalWrite(MOTOR_EN, LOW);  // Désactiver le moteur
  isRunning = false;
}

void setDirection(uint8_t direction) {
  currentDirection = (direction == REVERSE) ? REVERSE : FORWARD;
  digitalWrite(MOTOR_DIR, currentDirection);
}

void emergencyStop() {
  // Arrêt immédiat du moteur
  digitalWrite(MOTOR_PWM, LOW);
  digitalWrite(MOTOR_EN, LOW);
  currentSpeed = 0;
  isRunning = false;
}

// ============================================================
// DIAGNOSTIC
// ============================================================

void printStatus() {
  Serial.println("\n=== Tapis Status ===");
  Serial.print("Speed: ");
  Serial.print(currentSpeed);
  Serial.print(" (");
  Serial.print((currentSpeed * 100) / 255);
  Serial.println("%)");
  
  Serial.print("Direction: ");
  Serial.println(currentDirection ? "REVERSE" : "FORWARD");
  
  Serial.print("Motor: ");
  Serial.println(isRunning ? "RUNNING" : "STOPPED");
  
  Serial.print("PWM Value: ");
  Serial.println(currentSpeed);
}
