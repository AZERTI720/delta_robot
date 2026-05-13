#include <DynamixelShield.h>

// Configuration pour Arduino Uno
#include <SoftwareSerial.h>
SoftwareSerial soft_serial(7, 8); 
#define DEBUG_SERIAL soft_serial

const uint8_t ANCIEN_ID = 1;
const uint8_t NOUVEL_ID = 2;
const float PROTOCOLE = 1.0; // Confirmé par votre scan

DynamixelShield dxl;

void setup() {
  DEBUG_SERIAL.begin(115200);
  while(!DEBUG_SERIAL);

  // Connexion au bus Dynamixel à 1Mbps (vitesse standard des AX-12A en usine)
  // Si le scan a trouvé le moteur à une autre vitesse, modifiez le chiffre ici.
  dxl.begin(1000000); 
  dxl.setPortProtocolVersion(PROTOCOLE);

  DEBUG_SERIAL.println("--- Tentative de changement d'ID ---");

  // 1. On vérifie si le moteur répond
  if(dxl.ping(ANCIEN_ID)) {
    DEBUG_SERIAL.print("Moteur detecte. Arret du couple (Torque OFF)... ");
    
    // 2. IMPORTANT : Désactiver le couple pour modifier l'EEPROM
    dxl.torqueOff(ANCIEN_ID);
    
    // 3. Changement de l'ID
    if(dxl.setID(ANCIEN_ID, NOUVEL_ID)) {
      DEBUG_SERIAL.println("SUCCES !");
      DEBUG_SERIAL.print("L'ID est maintenant : ");
      DEBUG_SERIAL.println(NOUVEL_ID);
    } else {
      DEBUG_SERIAL.println("ECHEC du changement d'ID.");
    }
  } else {
    DEBUG_SERIAL.println("Erreur : Le moteur ID 1 ne repond pas. Verifiez le switch du Shield.");
  }
}

void loop() {
  // On laisse vide pour ne pas répéter l'opération
}