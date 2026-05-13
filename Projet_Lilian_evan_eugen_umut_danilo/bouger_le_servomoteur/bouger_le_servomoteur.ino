#include <DynamixelShield.h>

// Configuration pour le retour d'infos (Debug)
#if defined(ARDUINO_AVR_UNO) || defined(ARDUINO_AVR_MEGA2560)
  #include <SoftwareSerial.h>
  SoftwareSerial soft_serial(7, 8); 
  #define DEBUG_SERIAL soft_serial
#else
  #define DEBUG_SERIAL Serial
#endif

// --- Paramètres de l'AX-12A ---
const uint8_t DXL_ID = 0;           // ID d'usine par défaut
const float DXL_PROTOCOL = 1.0;     // INDISPENSABLE pour AX-12A
#define BAUDRATE 1000000            // Vitesse d'usine (1 Mbps)

// Adresses de la table de contrôle (Protocole 1.0)
#define ADDR_TORQUE_ENABLE    24
#define ADDR_GOAL_POSITION    30
#define ADDR_PRESENT_POSITION 36
#define ADDR_MOVING_SPEED     32

DynamixelShield dxl;

void setup() {
  DEBUG_SERIAL.begin(115200);
  
  // Initialisation du Shield
  dxl.begin(BAUDRATE);
  dxl.setPortProtocolVersion(DXL_PROTOCOL);

  // Activation du couple (Torque)
  dxl.torqueOn(DXL_ID);
  
  // Optionnel : Régler une vitesse modérée (0 à 1023)
  // On écrit 200 sur l'adresse MOVING_SPEED (2 octets)
  uint16_t speed = 200;
  dxl.write(DXL_ID, ADDR_MOVING_SPEED, (uint8_t*)&speed, 2);

  DEBUG_SERIAL.println("AX-12A Pret !");
}

void loop() {
  // Aller à la position 200 (environ 60 degrés)
  DEBUG_SERIAL.println("Mouvement vers : 200");
  dxl.setGoalPosition(DXL_ID, 200);
  delay(1500);

  // Aller à la position 800 (environ 240 degrés)
  DEBUG_SERIAL.println("Mouvement vers : 800");
  dxl.setGoalPosition(DXL_ID, 800);
  delay(1500);
}