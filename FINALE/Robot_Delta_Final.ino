#include <Dynamixel2Arduino.h>
#include <SoftwareSerial.h>
#include <Wire.h>

SoftwareSerial soft_serial(10, 11);   // DYNAMIXELShield UART RX/TX
Dynamixel2Arduino dxl(Serial, 2);   // DYNAMIXEL Shield DIR PIN = 2

using namespace ControlTableItem;

#define SLAVE_ADDR 11  // Adresse I2C de l'autre Arduino

// Structure pour simplifier les positions
struct Position {
  double x, y, z;
};

// Union pour convertir bytes en double
union DoubleBytes {
  double value;
  byte bytes[8];
};

// Fonction pour lire les positions depuis l'esclave I2C
Position readPositionFromSlave() {
  Position pos;
  DoubleBytes db;

  Wire.requestFrom(SLAVE_ADDR, 24);  // 3 doubles = 24 bytes
  if (Wire.available() == 24) {
    for (int i = 0; i < 8; i++) db.bytes[i] = Wire.read();
    pos.x = db.value;
    for (int i = 0; i < 8; i++) db.bytes[i] = Wire.read();
    pos.y = db.value;
    for (int i = 0; i < 8; i++) db.bytes[i] = Wire.read();
    pos.z = db.value;
  } else {
    // En cas d'erreur, retourner une position par défaut
    pos = {0, 0, -120};
  }
  return pos;
}

// Définir les positions ici pour faciliter les modifications
/*Position pos1 = {-75, -30, -120};
Position pos2 = {-75, -80, -120};
Position pos3 = {50, -30, -120};*/

Position pos1 = {0 -65, -100};
Position pos2 = {0, -65, -150};
Position pos3 = {0, -65, -120};

double x0 = 0;
double y0 = 0;
double z0 = -120;

double v1;
double v2;
double v3;
double v4;
double v5;

byte recep=0;

void setup() 
{
  //relay
  pinMode(4, OUTPUT);

  //initialisation com
  soft_serial.begin(9600);
  while(!soft_serial);
  Wire.begin();  // Initialiser I2C en tant que maître
  dxl.begin(1000000);
  dxl.setPortProtocolVersion(1.0);

  dxl.ping(1);
  dxl.torqueOff(1);
  dxl.setOperatingMode(1, OP_POSITION);
  dxl.torqueOn(1);
  
  dxl.ping(2);
  dxl.torqueOff(2);
  dxl.setOperatingMode(2, OP_POSITION);
  dxl.torqueOn(2);

  dxl.ping(3);
  dxl.torqueOff(3);
  dxl.setOperatingMode(3, OP_POSITION);
  dxl.torqueOn(3);

  // Limitez la vitesse maximale en mode Contrôle de position. Utilisez 0 pour la vitesse maximale.
  dxl.writeControlTableItem(MOVING_SPEED, 1, 80);  // Vitesse réduite pour le moteur 1, 0 vite -> 1023 lent
  dxl.writeControlTableItem(MOVING_SPEED, 2, 80);
  dxl.writeControlTableItem(MOVING_SPEED, 3, 80);
}

/*void loop() 
{

 while (soft_serial.available() > 0)
  {
    v1 = soft_serial.parseInt();
    v2 = soft_serial.parseInt();
    v3 = soft_serial.parseInt();
    v4 = soft_serial.parseInt();
    v5 = soft_serial.parseInt();
    soft_serial.read();

  
  //x0 = 0;
  //y0 = 0;
  //z0 = -130;
  //calculeEtAffiche(x0,y0,z0);
  //delay(1000);


  x0 = v1;
  y0 =  v2;
  z0 = -v3;
  calculeEtAffiche(x0,y0,z0);
  delay(1000);
  
  }
  

}*/

void moveLinear(double xStart, double yStart, double zStart, double xEnd, double yEnd, double zEnd, int steps, int stepDelay)
{
  for (int i = 0; i <= steps; i++) {
    double t = double(i) / double(steps);
    double x = xStart + (xEnd - xStart) * t;
    double y = yStart + (yEnd - yStart) * t;
    double z = zStart + (zEnd - zStart) * t;
    calculeEtAffiche(x, y, z);
    delay(stepDelay);
  }
}

void loop()
{
  // Aller pos1 -> pos2 doucement
  moveLinear(pos1.x, pos1.y, pos1.z,pos2.x, pos2.y, pos2.z,50, 5);

  delay(300);

  // Active ventouse / relais
  digitalWrite(4, HIGH);
  delay(300);

  // pos2 -> retour pos1
  moveLinear(pos2.x, pos2.y, pos2.z,pos1.x, pos1.y, pos1.z,50, 25);

  delay(300);

  // pos1 -> pos3
  moveLinear(pos1.x, pos1.y, pos1.z,pos3.x, pos3.y, pos3.z,50, 25);

  delay(300);

  // Désactive ventouse
  digitalWrite(4, LOW);
  delay(500);

  // Retour pos3 -> pos1
  moveLinear(pos3.x, pos3.y, pos3.z, pos1.x, pos1.y, pos1.z,50, 25);

  delay(1000);
}

void calculeEtAffiche(double x, double y, double z) 
{

	// Constantes
  double L = 105;     //90.316; 105
	double l = 190;      //230;  190
	double Up = 25;      //35; 25
	double Wb = 15;      //70.502;  15
	double Racine3 = sqrt(3);

	// Calcul de la constante de décalage (Base - Effecteur)
  double a = Wb - Up; 
  double b = a / 2.0;               // Projection pour les bras à 120°
  double c = (a * Racine3) / 2.0;   // Projection pour les bras à 120°

  // Bras 1 (Moteur arrière, sur l'axe Y)
  double E1 = L * (Racine3 * x - y - a);
  double F1 = 2 * z * L;
  double G1 = x*x + y*y + z*z + a*a + L*L + 2*y*a - l*l;

  // Bras 2 (Moteur avant gauche)
  double E2 = -L * (Racine3 * x + y + a);
  double F2 = 2 * z * L;
  double G2 = x*x + y*y + z*z + a*a + L*L + Racine3*x*a + y*a - l*l;

  // Bras 3 (Moteur avant droit)
  double E3 = L * (Racine3 * x - y - a);
  double F3 = 2 * z * L;
  double G3 = x*x + y*y + z*z + a*a + L*L - Racine3*x*a + y*a - l*l;

	double t1 = (-F1 - sqrt(E1 * E1 + F1 * F1 - G1 * G1)) / (G1 - E1);
	double t2 = (-F2 - sqrt(E2 * E2 + F2 * F2 - G2 * G2)) / (G2 - E2);
	double t3 = (-F3 - sqrt(E3 * E3 + F3 * F3 - G3 * G3)) / (G3 - E3);
  
	double teta1 = 2 * atan(t1);
	double teta2 = 2 * atan(t2);
	double teta3 = 2 * atan(t3);
        
  double alpha1 = (teta1*180)/3.14159267;
  double alpha2 = (teta2*180)/3.14159267;
  double alpha3 = (teta3*180)/3.14159267;

  alpha1 = 90-alpha1;
  alpha2 = 90-alpha2+25;
  alpha3 = 90-alpha3;

  // Affiche
  /*soft_serial.print("teta1=");soft_serial.print(teta1);soft_serial.print("  teta2=");soft_serial.print(teta2);soft_serial.print("  teta3=");soft_serial.print(teta3);
  soft_serial.print("  alpha1=");soft_serial.print(alpha1);soft_serial.print("  alpha2=");soft_serial.print(alpha2);soft_serial.print("  alpha3=");soft_serial.print(alpha3);
  soft_serial.print("  x=");soft_serial.print(x0);soft_serial.print("  y=");soft_serial.print(y0);soft_serial.print("  z=");soft_serial.println(z0);
  */

  // Deplace
  //alpha1=128;
  dxl.setGoalPosition(1, alpha1, UNIT_DEGREE);
  dxl.setGoalPosition(2, alpha2, UNIT_DEGREE);
  dxl.setGoalPosition(3, alpha3, UNIT_DEGREE);
    
  //}
}
