#include <Dynamixel2Arduino.h>
#include <SoftwareSerial.h>

SoftwareSerial soft_serial(7, 8);   // DYNAMIXELShield UART RX/TX
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
  dxl.writeControlTableItem(PROFILE_VELOCITY, 1, 10);
  dxl.writeControlTableItem(PROFILE_VELOCITY, 2, 10);
  dxl.writeControlTableItem(PROFILE_VELOCITY, 3, 10);
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
  for (int i = -120; i <= 120; i = i+5) {
    calculeEtAffiche(i, 0, -120);
    soft_serial.print("  x=");
    soft_serial.println(i);
    delay(500);
  }

  for (int j = -120; j <= 50; j = j+5) {
    calculeEtAffiche(0, j, -120);
    soft_serial.print("  y=");
    soft_serial.println(j);
    delay(500);
  }

  for (int k = -220; k <= -100; k = k+5) {
    calculeEtAffiche(0, 0, k);
    soft_serial.print("  z=");
    soft_serial.println(k);
    delay(500);
  } 
}

void calculeEtAffiche(double x, double y, double z) 
{
    double L = 105.0;
    double l = 190.0;
    double Up = 25.0;
    double Wb = 15.0;
    double R3 = sqrt(3.0);

    // Rayon effecteur et base
    double Wp = Up / 2.0;
    double Sp = 3.0 * Up / R3;
    double rb = Sp / R3;      // rayon base
    double rp = Up / R3;      // rayon plateforme

    // Angles réels des 3 moteurs (en radians)
    double a1 = 210.0 * PI / 180.0;  // ID:1 droite-arrière
    double a2 =  90.0 * PI / 180.0;  // ID:2 avant
    double a3 = 330.0 * PI / 180.0;  // ID:3 gauche-arrière

    // Position de chaque pivot moteur dans le plan XY
    double bx1 = rb * cos(a1),  by1 = rb * sin(a1);
    double bx2 = rb * cos(a2),  by2 = rb * sin(a2);
    double bx3 = rb * cos(a3),  by3 = rb * sin(a3);

    // Position de chaque pivot effecteur dans le plan XY
    double px1 = x + rp * cos(a1),  py1 = y + rp * sin(a1);
    double px2 = x + rp * cos(a2),  py2 = y + rp * sin(a2);
    double px3 = x + rp * cos(a3),  py3 = y + rp * sin(a3);

    // Cinématique inverse pour chaque bras
    // On cherche theta tel que le bout du bras L atteigne le pivot effecteur
    auto solveArm = [&](double bx, double by, double px, double py) -> double {
        double dx = px - bx;
        double dy = py - by;
        // Distance horizontale entre pivot base et pivot effecteur
        double dh = sqrt(dx*dx + dy*dy);
        // Le bras L doit atteindre (dh, z) dans son plan vertical
        double E = 2.0 * z * L;
        double F = 2.0 * dh * L;
        double G = z*z + dh*dh + L*L - l*l;
        double disc = E*E + F*F - G*G;
        if (disc < 0) return NAN;
        double t = (-E - sqrt(disc)) / (G - F);  // attention: signe F ici
        // attends: formule standard tangente demi-angle
        // theta = 2*atan(t)
        return 2.0 * atan(t);
    };

    double theta1 = solveArm(bx1, by1, px1, py1);
    double theta2 = solveArm(bx2, by2, px2, py2);
    double theta3 = solveArm(bx3, by3, px3, py3);

    if (isnan(theta1) || isnan(theta2) || isnan(theta3)) {
        soft_serial.println("ERREUR: hors espace de travail");
        return;
    }

    double alpha1 = 90.0 - theta1 * 180.0 / PI;
    double alpha2 = 90.0 - theta2 * 180.0 / PI;
    double alpha3 = 90.0 - theta3 * 180.0 / PI;

    if (alpha1 < 0 || alpha1 > 300 || alpha2 < 0 || alpha2 > 300 || alpha3 < 0 || alpha3 > 300) {
        soft_serial.println("ERREUR: angle hors plage");
        return;
    }

    soft_serial.print("a1="); soft_serial.print(alpha1);
    soft_serial.print(" a2="); soft_serial.print(alpha2);
    soft_serial.print(" a3="); soft_serial.println(alpha3);

    dxl.setGoalPosition(1, alpha1, UNIT_DEGREE);
    dxl.setGoalPosition(2, alpha2, UNIT_DEGREE);
    dxl.setGoalPosition(3, alpha3, UNIT_DEGREE);
}

