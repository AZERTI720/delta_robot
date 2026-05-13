// ============================================================
// ARDUINO ESCLAVE #11 - Delta Robot (Dynamixel)
// Adresse I2C: 0x0B (11)
// Rôle: Contrôle les 3 moteurs Dynamixel et la ventouse
// ============================================================

#include <Dynamixel2Arduino.h>
#include <SoftwareSerial.h>
#include <Wire.h>

// Dynamixel Shield setup
SoftwareSerial soft_serial(10, 11);
Dynamixel2Arduino dxl(soft_serial, 2);

using namespace ControlTableItem;

// Constants
#define MY_ADDRESS 11
#define RELAY_PIN 4

// Commandes
#define CMD_MOVE_XYZ      0x01
#define CMD_MOVE_LINEAR   0x02
#define CMD_GRIPPER_OPEN  0x03
#define CMD_GRIPPER_CLOSE 0x04
#define CMD_STOP          0x05
#define CMD_STATUS        0x06
#define CMD_SPEED_SET     0x07

// États
#define STATUS_OK     0xAA
#define STATUS_MOVING 0xBB
#define STATUS_ERROR  0xEE

// Union pour les conversions bytes
union FloatBytes {
  float value;
  byte bytes[4];
};

// Variables globales
struct Position {
  float x, y, z;
};

Position currentPos = {-75, -30, -120};
Position targetPos = {0, 0, -120};
bool isMoving = false;
uint16_t motorSpeed = 100;

// Paramètres cinématiques du delta
const float L = 133;  // Longueur des bras
const float l = 47;   // Longueur des bielles
const float d = 95;   // Distance entre moteurs

// Angles des moteurs
float theta1 = 0, theta2 = 0, theta3 = 0;

void setup() {
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);  // Ventouse ouverte par défaut
  
  Serial.begin(9600);
  while (!soft_serial);
  
  Wire.begin(MY_ADDRESS);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
  
  // Initialise Dynamixel
  dxl.begin(1000000);
  dxl.setPortProtocolVersion(1.0);
  
  // Configure les 3 moteurs
  for (int id = 1; id <= 3; id++) {
    if (dxl.ping(id)) {
      dxl.torqueOff(id);
      dxl.setOperatingMode(id, OP_POSITION);
      dxl.writeControlTableItem(MOVING_SPEED, id, motorSpeed);
      dxl.torqueOn(id);
      Serial.print("Moteur ");
      Serial.print(id);
      Serial.println(" OK");
    }
  }
  
  Serial.println("=== ESCLAVE DELTA Ready (Addr 11) ===");
}

void loop() {
  // Peut ajouter de la logique ici si besoin
  delay(10);
}

// ============================================================
// RÉCEPTION DES COMMANDES I2C
// ============================================================

void receiveEvent(int numBytes) {
  if (numBytes < 1) return;
  
  uint8_t command = Wire.read();
  
  switch (command) {
    case CMD_MOVE_XYZ:
      if (numBytes >= 13) {  // 1 + 4 + 4 + 4
        FloatBytes fbX, fbY, fbZ;
        for (int i = 0; i < 4; i++) fbX.bytes[i] = Wire.read();
        for (int i = 0; i < 4; i++) fbY.bytes[i] = Wire.read();
        for (int i = 0; i < 4; i++) fbZ.bytes[i] = Wire.read();
        
        targetPos.x = fbX.value;
        targetPos.y = fbY.value;
        targetPos.z = fbZ.value;
        
        Serial.print("MOVE_XYZ: ");
        Serial.print(targetPos.x);
        Serial.print(", ");
        Serial.print(targetPos.y);
        Serial.print(", ");
        Serial.println(targetPos.z);
        
        moveToPosition(targetPos.x, targetPos.y, targetPos.z);
        isMoving = true;
      }
      break;
    
    case CMD_GRIPPER_OPEN:
      Serial.println("Gripper OPEN");
      digitalWrite(RELAY_PIN, HIGH);
      break;
    
    case CMD_GRIPPER_CLOSE:
      Serial.println("Gripper CLOSE");
      digitalWrite(RELAY_PIN, LOW);
      break;
    
    case CMD_STOP:
      Serial.println("STOP");
      dxl.torqueOff(1);
      dxl.torqueOff(2);
      dxl.torqueOff(3);
      isMoving = false;
      break;
    
    case CMD_STATUS:
      // Juste pour marquer que le statut est demandé
      break;
    
    case CMD_SPEED_SET:
      if (numBytes >= 3) {
        uint16_t speed = ((uint16_t)Wire.read() << 8) | Wire.read();
        motorSpeed = constrain(speed, 0, 1023);
        Serial.print("Speed set to: ");
        Serial.println(motorSpeed);
        
        for (int id = 1; id <= 3; id++) {
          dxl.writeControlTableItem(MOVING_SPEED, id, motorSpeed);
        }
      }
      break;
  }
  
  // Vider le buffer I2C
  while (Wire.available()) Wire.read();
}

// ============================================================
// ENVOI DU STATUT I2C
// ============================================================

void requestEvent() {
  if (isMoving) {
    Wire.write(STATUS_MOVING);
  } else {
    Wire.write(STATUS_OK);
  }
}

// ============================================================
// CINÉMATIQUE INVERSE - DELTA
// ============================================================

void moveToPosition(float x, float y, float z) {
  // Calcule les angles pour les 3 moteurs
  if (!inverseKinematics(x, y, z, theta1, theta2, theta3)) {
    Serial.println("ERROR: IK failed");
    Wire.write(STATUS_ERROR);
    return;
  }
  
  // Convertir les angles en positions Dynamixel (0-1023)
  uint16_t pos1 = angleToPosition(theta1);
  uint16_t pos2 = angleToPosition(theta2);
  uint16_t pos3 = angleToPosition(theta3);
  
  Serial.print("Angles: ");
  Serial.print(theta1);
  Serial.print(", ");
  Serial.print(theta2);
  Serial.print(", ");
  Serial.println(theta3);
  
  // Envoyer aux moteurs
  dxl.writeControlTableItem(GOAL_POSITION, 1, pos1);
  dxl.writeControlTableItem(GOAL_POSITION, 2, pos2);
  dxl.writeControlTableItem(GOAL_POSITION, 3, pos3);
}

// Cinématique inverse simplifié (à adapter selon géométrie exacte)
bool inverseKinematics(float x, float y, float z, float &t1, float &t2, float &t3) {
  // Simplifié - à adapter selon vos calculs
  
  float r = sqrt(x*x + y*y);
  float h = z + d/2;  // d = distance plateforme
  
  // Formule classique delta
  float u = (r*r + h*h + L*L - l*l) / (2*L);
  
  if (u*u > L*L) return false;  // Hors zone
  
  // Calculs angles (simplifié)
  t1 = atan2(sqrt(L*L - u*u), u);
  t2 = atan2(sqrt(L*L - u*u), u);
  t3 = atan2(sqrt(L*L - u*u), u);
  
  return true;
}

// Convertir angle (radians) en position Dynamixel (0-1023)
uint16_t angleToPosition(float angle) {
  // Pour Dynamixel AX-12A: 0-300 degrés = 0-1023
  // Adapter selon votre type de moteur!
  
  float degrees = angle * 180.0 / PI;
  uint16_t position = map(degrees, -150, 150, 0, 1023);
  return constrain(position, 0, 1023);
}

// Convertir position Dynamixel en angle (radians)
float positionToAngle(uint16_t position) {
  float degrees = map(position, 0, 1023, -150, 150);
  return degrees * PI / 180.0;
}
