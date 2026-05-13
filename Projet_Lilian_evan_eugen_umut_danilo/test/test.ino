#include <Dynamixel2Arduino.h>
#include <SoftwareSerial.h>

SoftwareSerial soft_serial(7, 8); 
Dynamixel2Arduino dxl(Serial, 2); 

void setup() {
  soft_serial.begin(9600);
  dxl.begin(1000000);
  dxl.setPortProtocolVersion(1.0); // Ou 2.0 selon tes moteurs

  soft_serial.println("--- MODE DIAGNOSTIC DELTA ---");
  soft_serial.println("1. Placez les bras noirs parfaitement HORIZONTAUX à la main.");
  soft_serial.println("2. Attendez la lecture des positions...");
  
  // On coupe le couple pour bouger les bras librement
  for(int id=1; id<=3; id++) {
    dxl.torqueOff(id);
  }
  delay(2000);
}

void loop() {
  soft_serial.println("\n--- RELEVÉ DES POSITIONS (Bras à l'horizontale) ---");
  
  for(int id=1; id<=3; id++) {
    if(dxl.ping(id)) {
      int32_t pos = dxl.getPresentPosition(id, UNIT_RAW);
      float deg = dxl.getPresentPosition(id, UNIT_DEGREE);
      
      soft_serial.print("Moteur ["); soft_serial.print(id); soft_serial.print("] : ");
      soft_serial.print("Valeur RAW = "); soft_serial.print(pos);
      soft_serial.print(" | Valeur Degrés = "); soft_serial.println(deg);
    } else {
      soft_serial.print("Moteur ["); soft_serial.print(id); soft_serial.println("] NON DÉTECTÉ !");
    }
  }

  soft_serial.println("\n--- VÉRIFICATION DES DIMENSIONS ---");
  soft_serial.println("Veuillez mesurer et me donner :");
  soft_serial.println("- WB : Distance centre plateau haut -> Axe rotation moteur");
  soft_serial.println("- UP : Distance centre plateau bas -> Axe rotule");
  soft_serial.println("- L  : Longueur bras noir (entre les 2 axes)");
  soft_serial.println("- l  : Longueur tiges métal (entre les 2 rotules)");

  delay(5000); // Répète toutes les 5 secondes
}