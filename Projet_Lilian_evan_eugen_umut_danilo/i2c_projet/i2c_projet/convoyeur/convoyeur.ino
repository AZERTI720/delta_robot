#include <Wire.h>

int pwm_a = 3;
int pwm_b = 9;
int dir_a = 2;
int dir_b = 8;

int tps_depart = 2600;
int tps_final  = 800;
int pas        = 100;
int nb_cycles  = 20;
boolean avant  = true;

// Vitesse courante partagée avec I2C
volatile int tps_courant = 2600;
volatile byte commande_i2c = 0;

// Maître demande la vitesse courante (2 octets)
void onRequest() {
  Wire.write((byte)(tps_courant >> 8));
  Wire.write((byte)(tps_courant & 0xFF));
}

// Maître envoie une commande : R, T, S, A, B
void onReceive(int n) {
  if (Wire.available()) {
    commande_i2c = Wire.read();
  }
}

// ---------- Moteur ----------
void pasMoteur(int tps) {
  tps_courant = tps;

  if (avant) {
    analogWrite(pwm_a, 255); analogWrite(pwm_b, 0);
    digitalWrite(dir_a, LOW); digitalWrite(dir_b, LOW);
    delayMicroseconds(tps);

    analogWrite(pwm_a, 0); analogWrite(pwm_b, 255);
    digitalWrite(dir_a, LOW); digitalWrite(dir_b, LOW);
    delayMicroseconds(tps);

    analogWrite(pwm_a, 255); analogWrite(pwm_b, 0);
    digitalWrite(dir_a, HIGH); digitalWrite(dir_b, LOW);
    delayMicroseconds(tps);

    analogWrite(pwm_a, 0); analogWrite(pwm_b, 255);
    digitalWrite(dir_a, LOW); digitalWrite(dir_b, HIGH);
    delayMicroseconds(tps);
  } else {
    analogWrite(pwm_a, 0); analogWrite(pwm_b, 255);
    digitalWrite(dir_a, LOW); digitalWrite(dir_b, HIGH);
    delayMicroseconds(tps);

    analogWrite(pwm_a, 255); analogWrite(pwm_b, 0);
    digitalWrite(dir_a, HIGH); digitalWrite(dir_b, LOW);
    delayMicroseconds(tps);

    analogWrite(pwm_a, 0); analogWrite(pwm_b, 255);
    digitalWrite(dir_a, LOW); digitalWrite(dir_b, LOW);
    delayMicroseconds(tps);

    analogWrite(pwm_a, 255); analogWrite(pwm_b, 0);
    digitalWrite(dir_a, LOW); digitalWrite(dir_b, LOW);
    delayMicroseconds(tps);
  }
}

void traiterCommande() {
  if (commande_i2c == 0) return;
  byte cmd = commande_i2c;
  commande_i2c = 0;

  if (cmd == 'R') avant = true;
  if (cmd == 'T') avant = false;
  if (cmd == 'S') {
    analogWrite(pwm_a, 0);
    analogWrite(pwm_b, 0);
  }
}

void setup() {
  pinMode(pwm_a, OUTPUT); pinMode(pwm_b, OUTPUT);
  pinMode(dir_a, OUTPUT); pinMode(dir_b, OUTPUT);

  Wire.begin(0x09);
  Wire.onRequest(onRequest);
  Wire.onReceive(onReceive);
}

void loop() {
  // Phase accélération
  for (int tps = tps_depart; tps >= tps_final; tps -= pas) {
    for (int i = 0; i < nb_cycles; i++) {
      pasMoteur(tps);
      traiterCommande();
    }
  }

  // Vitesse max en continu
  while (true) {
    pasMoteur(tps_final);
    traiterCommande();
  }
}
