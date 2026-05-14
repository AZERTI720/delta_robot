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
  dxl.writeControlTableItem(PROFILE_VELOCITY, 1, 30);
  dxl.writeControlTableItem(PROFILE_VELOCITY, 2, 30);
  dxl.writeControlTableItem(PROFILE_VELOCITY, 3, 30);
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
	// Vérification des minimums / maximums
	//if ((x < -150) || (x > 150) || (y < -150) || (y > 150) || (z < -260) || (z > -130)) 
  /*      if ((x < -70) || (x > 70) || (y < -70) || (y > 70) || (z < -270) || (z > -200)) 
        {
	  Serial.print("ERREUR DE VALEUR x=");Serial.print(x);Serial.print(" y=");Serial.print(y);Serial.print(" z=");Serial.println(z);
 	}
        else
        {*/

	// Constantes
	double L = 105;     //90.316;
	double l = 190;      //230;
	double Up = 25;      //35;
	double Wb = 15;      //70.502;
	double Racine3 = sqrt(3);

	// Calculs
	double Wp = Up / 2;
	double Sp = 3 * Up / Racine3;
	double a = Wb - Up;
	double b = Sp / 2 - Racine3 / 2 * Wb;
	double c = Wp - Wb / 2;
	double E1 = 2 * L * (y+a);
	double F1 = 2 * z * L;
	double G1 = x * x + y * y + z * z + a * a + L * L + 2 * y * a - l * l;
	double E2 = -L * (Racine3 * (x + b) + y + c);
	double F2 = 2 * z * L;
	double G2 = x * x + y * y + z * z + b * b + c * c + L * L + 2 * (x * b + y * c) - l * l;
	double E3 = L * (Racine3 * (x - b) - y - c);
	double F3 = 2 * z * L;
	double G3 = x * x + y * y + z * z + b * b + c * c + L * L + 2 * (-x * b + y * c) - l * l;
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
  alpha2 = 90-alpha2;
  alpha3 = 90-alpha3;

  // Affiche
  soft_serial.print("teta1=");soft_serial.print(teta1);soft_serial.print("  teta2=");soft_serial.print(teta2);soft_serial.print("  teta3=");soft_serial.print(teta3);
  soft_serial.print("  alpha1=");soft_serial.print(alpha1);soft_serial.print("  alpha2=");soft_serial.print(alpha2);soft_serial.print("  alpha3=");soft_serial.print(alpha3);
  soft_serial.print("  x=");soft_serial.print(x0);soft_serial.print("  y=");soft_serial.print(y0);soft_serial.print("  z=");soft_serial.println(z0);
        
  // Deplace
  dxl.setGoalPosition(1, alpha1, UNIT_DEGREE);
  dxl.setGoalPosition(2, alpha2, UNIT_DEGREE);
  dxl.setGoalPosition(3, alpha3, UNIT_DEGREE);
    
  //}
}
