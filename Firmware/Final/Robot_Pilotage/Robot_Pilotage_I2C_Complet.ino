#include <Dynamixel2Arduino.h>
#include <SoftwareSerial.h>
#include <Wire.h>

SoftwareSerial soft_serial(10, 11);
Dynamixel2Arduino dxl(Serial, 2);
using namespace ControlTableItem;

struct Position {
  double x;
  double yVertical;
  double zLateral;
};

// Position attente au-dessus du tapis
Position pos_attente = {0, -100, 0};

// Commande reçue par I2C : {x, z, action}
// x, z en mm (int16), action : 1=attrape 0=rien
volatile int16_t cmd_x      = 0;
volatile int16_t cmd_z      = 0;
volatile byte    cmd_action = 0;
volatile bool    nouvelle_cmd = false;

// Maître envoie 5 octets : xH xL zH zL action
void onReceive(int n) {
  if (n >= 5) {
    int16_t rx = (Wire.read() << 8) | Wire.read();
    int16_t rz = (Wire.read() << 8) | Wire.read();
    byte    ra = Wire.read();
    cmd_x      = rx;
    cmd_z      = rz;
    cmd_action = ra;
    nouvelle_cmd = true;
  }
  while (Wire.available()) Wire.read();
}

// Maître peut demander statut : 1=occupé 0=libre
volatile byte statut = 0;
void onRequest() {
  Wire.write(statut);
}

// ---------- Cinématique ----------
void calculeEtAffiche(double x, double y, double z) {
  double L = 105, l = 190, Up = 25, Wb = 15;
  double Racine3 = sqrt(3);
  double a = Wb - Up;

  double E1 = L * (Racine3*x - y - a);
  double F1 = 2*z*L;
  double G1 = x*x + y*y + z*z + a*a + L*L + 2*y*a - l*l;

  double E2 = -L*(Racine3*x + y + a);
  double F2 = 2*z*L;
  double G2 = x*x + y*y + z*z + a*a + L*L + Racine3*x*a + y*a - l*l;

  double E3 = L*(Racine3*x - y - a);
  double F3 = 2*z*L;
  double G3 = x*x + y*y + z*z + a*a + L*L - Racine3*x*a + y*a - l*l;

  double t1 = (-F1 - sqrt(E1*E1 + F1*F1 - G1*G1)) / (G1 - E1);
  double t2 = (-F2 - sqrt(E2*E2 + F2*F2 - G2*G2)) / (G2 - E2);
  double t3 = (-F3 - sqrt(E3*E3 + F3*F3 - G3*G3)) / (G3 - E3);

  double a1 = 90 - (2*atan(t1)*180/3.14159267);
  double a2 = 90 - (2*atan(t2)*180/3.14159267) + 20;
  double a3 = 90 - (2*atan(t3)*180/3.14159267);

  dxl.setGoalPosition(1, a1, UNIT_DEGREE);
  dxl.setGoalPosition(2, a2, UNIT_DEGREE);
  dxl.setGoalPosition(3, a3, UNIT_DEGREE);
}

void moveTo(double x, double yVertical, double zLateral) {
  calculeEtAffiche(x, zLateral, yVertical);
}

void moveLinear(double x0, double y0, double z0,
                double x1, double y1, double z1,
                int steps, int stepDelay) {
  for (int i = 0; i <= steps; i++) {
    double t = (double)i / steps;
    moveTo(x0+(x1-x0)*t, y0+(y1-y0)*t, z0+(z1-z0)*t);
    delay(stepDelay);
  }
}

// Position de dépôt hors tapis
Position pos_depot = {100, -120, 80};

void attraperEtDeposer(double x_cube, double z_cube) {
  statut = 1;

  // Descendre sur le cube
  Position pos_cube = {x_cube, -130, z_cube};
  moveTo(pos_cube.x, pos_cube.yVertical, pos_cube.zLateral);
  delay(400);

  // Activer ventouse/pince
  digitalWrite(4, HIGH);
  delay(200);

  // Monter
  moveTo(pos_cube.x, -90, pos_cube.zLateral);
  delay(400);

  // Déplacer vers dépôt
  moveLinear(pos_cube.x, -90, pos_cube.zLateral,
             pos_depot.x, pos_depot.yVertical, pos_depot.zLateral,
             20, 50);

  // Lâcher
  digitalWrite(4, LOW);
  delay(200);

  // Retour attente
  moveTo(pos_attente.x, pos_attente.yVertical, pos_attente.zLateral);
  delay(500);

  statut = 0;
}

void setup() {
  pinMode(4, OUTPUT);
  soft_serial.begin(9600);
  dxl.begin(1000000);
  dxl.setPortProtocolVersion(1.0);

  for (int id = 1; id <= 3; id++) {
    dxl.ping(id);
    dxl.torqueOff(id);
    dxl.setOperatingMode(id, OP_POSITION);
    dxl.torqueOn(id);
    dxl.writeControlTableItem(MOVING_SPEED, id, 100);
  }

  Wire.begin(0x0A);
  Wire.onReceive(onReceive);
  Wire.onRequest(onRequest);

  moveTo(pos_attente.x, pos_attente.yVertical, pos_attente.zLateral);
}

void loop() {
  if (nouvelle_cmd && cmd_action == 1 && statut == 0) {
    nouvelle_cmd = false;
    attraperEtDeposer((double)cmd_x, (double)cmd_z);
  }
}
