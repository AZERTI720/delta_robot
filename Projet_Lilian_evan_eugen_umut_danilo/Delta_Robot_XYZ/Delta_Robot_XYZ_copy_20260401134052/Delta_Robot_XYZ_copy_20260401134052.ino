#include <Dynamixel2Arduino.h>
#include <SoftwareSerial.h>

SoftwareSerial soft_serial(10, 11);   // DYNAMIXELShield UART RX/TX
Dynamixel2Arduino dxl(Serial, 2);   // DYNAMIXEL Shield DIR PIN = 2

using namespace ControlTableItem;

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
  soft_serial.begin(9600);
  while(!soft_serial);
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
  dxl.writeControlTableItem(PROFILE_VELOCITY, 1, 0);
  dxl.writeControlTableItem(PROFILE_VELOCITY, 2, 0);
  dxl.writeControlTableItem(PROFILE_VELOCITY, 3, 0);
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

void loop() 
{
  for (int i = -50; i<=50; i++){
    calculeEtAffiche(i, 0, -120);
    delay(50);
  }
  for (int j = -50; j<=50; j++){
    calculeEtAffiche(0, j, -120);
    delay(50);
  }
  for (int k = -50; k<=50; k++){
    calculeEtAffiche(0, 0, k);
    delay(50);
  }
  soft_serial.println("Fin des essai");
  while (true) {
    delay(1000);
  }
    
}

void calculeEtAffiche(double x, double y, double z) 
{
  // --- 1. Dimensions (mm) ---
  double Wb = 75.0;  
  double Up = 25.0;  
  double L = 105.0;  
  double l = 190.0;  
  
  double Racine3 = sqrt(3);
  double a = Wb - Up; 

  // --- 2. Calculs Cinématiques Corrigés ---
  // Bras 1 (Arrière - Axe Y+)
  double E1 = 2 * L * (a - y);
  double F1 = -2 * z * L;
  double G1 = x*x + y*y + z*z + a*a + L*L - l*l - 2*y*a;

  // Bras 2 (Avant Gauche - 210°)
  double E2 = L * (2*a + Racine3*x + y);
  double F2 = -2 * z * L;
  double G2 = x*x + y*y + z*z + a*a + L*L - l*l + a*(Racine3*x + y);

  // Bras 3 (Avant Droit - 330°)
  double E3 = L * (2*a - Racine3*x + y);
  double F3 = -2 * z * L;
  double G3 = x*x + y*y + z*z + a*a + L*L - l*l - a*(Racine3*x - y);

  // --- 3. Calcul des angles ---
  // Note : Utilisation du signe + devant sqrt pour l'assemblage "coudes vers l'extérieur"
  double teta1 = 2 * atan((-F1 + sqrt(E1*E1 + F1*F1 - G1*G1)) / (G1 - E1));
  double teta2 = 2 * atan((-F2 + sqrt(E2*E2 + F2*F2 - G2*G2)) / (G2 - E2));
  double teta3 = 2 * atan((-F3 + sqrt(E3*E3 + F3*F3 - G3*G3)) / (G3 - E3));

  float alpha1 = (teta1 * 180.0) / PI;
  float alpha2 = (teta2 * 180.0) / PI;
  float alpha3 = (teta3 * 180.0) / PI;

  // --- 4. Envoi aux moteurs ---
  // On assume que 150° est la position horizontale (0° mathématique)
  dxl.setGoalPosition(1, 90 - alpha1, UNIT_DEGREE);
  dxl.setGoalPosition(2, 90 - alpha2, UNIT_DEGREE);
  dxl.setGoalPosition(3, 90 - alpha3, UNIT_DEGREE);
  
  // Debug
  soft_serial.print("Angles: "); 
  soft_serial.print(alpha1); soft_serial.print(" | ");
  soft_serial.print(alpha2); soft_serial.print(" | ");
  soft_serial.println(alpha3);
}