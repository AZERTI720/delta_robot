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
const uint8_t DXL_ID = 1;           // ID du premier servo
const uint8_t DXL_ID1 = 2;           // ID du second servo
const uint8_t DXL_ID2 = 3;           // ID du troisieme servo
const float DXL_PROTOCOL = 1.0;     // INDISPENSABLE pour AX-12A
#define BAUDRATE 1000000            // Vitesse d'usine (1 Mbps)

// Adresses de la table de contrôle (Protocole 1.0)
#define ADDR_TORQUE_ENABLE    24
#define ADDR_GOAL_POSITION    30
#define ADDR_PRESENT_POSITION 36
#define ADDR_MOVING_SPEED     23

DynamixelShield dxl;

int t = 100;
int i = 200;

void setup() {
  DEBUG_SERIAL.begin(115200);
  
  // Initialisation du Shield
  dxl.begin(BAUDRATE);
  dxl.setPortProtocolVersion(DXL_PROTOCOL);

  // Activation du couple (Torque)
  dxl.torqueOn(DXL_ID1);

  
  // On écrit 200 sur l'adresse MOVING_SPEED (2 octets)
  uint16_t speed = 200;
  dxl.write(DXL_ID, ADDR_MOVING_SPEED, (uint8_t*)&speed, 2);

  DEBUG_SERIAL.println("AX-12A Pret !");

  DEBUG_SERIAL.println("---Initialisation---");
  dxl.setGoalPosition(DXL_ID, 160);
  delay(t*5);
}

void loop() {

    if (i>= 160)
    {
      for (i=160; i < 750; i=i+5)
      {
        DEBUG_SERIAL.print("déplacement des servo vers : ");
        DEBUG_SERIAL.println(i);
        dxl.setGoalPosition(DXL_ID, i);
        dxl.setGoalPosition(DXL_ID1, i);
        dxl.setGoalPosition(DXL_ID2, i);
        delay(t);
      }
    }
    if (i>= 750)
    {
      for (i=750; i > 160; i=i-5)
      {
        DEBUG_SERIAL.print("déplacement des servo vers : ");
        DEBUG_SERIAL.println(i);
        dxl.setGoalPosition(DXL_ID, i);
        dxl.setGoalPosition(DXL_ID1, i);
        dxl.setGoalPosition(DXL_ID2, i);
        delay(t);
      }
    }

}