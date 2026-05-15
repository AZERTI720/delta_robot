#include <Wire.h>

// Adresses esclaves
#define ADDR_PIXY      0x08
#define ADDR_CONVOYEUR 0x09
#define ADDR_DELTA     0x0A

// Caméra : 315px = 10cm de large
#define TAPIS_LARGEUR_CM  10.0
#define PIXY_MAX_X        315.0

// Distance caméra → delta sur l'axe convoyeur (cm)
#define DIST_CAM_DELTA_CM 15.0

// Vitesse max convoyeur en cm/s (calibrée)
#define VITESSE_MAX_CM_S  12.0

// tps_final du convoyeur (µs) → correspond à vitesse max
#define TPS_FINAL 800

// ---------- Lecture Pixy ----------
struct DonneesPixy {
  int  cube_id;
  int  x;       // pixels
  int  y;
  bool nouveau;
};

DonneesPixy lirePixy() {
  DonneesPixy d = {0, 0, 0, false};
  Wire.requestFrom(ADDR_PIXY, 6);
  if (Wire.available() == 6) {
    d.cube_id = Wire.read();
    d.x       = (Wire.read() << 8) | Wire.read();
    d.y       = (Wire.read() << 8) | Wire.read();
    d.nouveau = Wire.read() == 1;
  }
  return d;
}

// ---------- Lecture vitesse convoyeur ----------
// Retourne vitesse en cm/s à partir du tps reçu
float lireVitesseConvoyeur() {
  Wire.requestFrom(ADDR_CONVOYEUR, 2);
  if (Wire.available() == 2) {
    int tps = (Wire.read() << 8) | Wire.read();
    if (tps <= 0) return 0;
    // Interpolation linéaire : tps_final=800 → 12cm/s, tps_depart=2600 → ~3cm/s
    // Vitesse ∝ 1/tps (approximation)
    float v = VITESSE_MAX_CM_S * (float)TPS_FINAL / (float)tps;
    return v;
  }
  return 0;
}

// ---------- Lecture statut delta ----------
byte lireStatutDelta() {
  Wire.requestFrom(ADDR_DELTA, 1);
  if (Wire.available()) return Wire.read();
  return 1; // par défaut occupé si pas de réponse
}

// ---------- Envoi commande delta ----------
void envoyerCommandeDelta(int16_t x_mm, int16_t z_mm) {
  Wire.beginTransmission(ADDR_DELTA);
  Wire.write((byte)(x_mm >> 8));
  Wire.write((byte)(x_mm & 0xFF));
  Wire.write((byte)(z_mm >> 8));
  Wire.write((byte)(z_mm & 0xFF));
  Wire.write(1); // action = attraper
  Wire.endTransmission();
}

// ---------- Conversion pixels → mm ----------
// X pixy (0-315) → position latérale sur tapis (-50mm à +50mm, centré)
float pixelVersLateral_mm(int px) {
  float cm = ((float)px / PIXY_MAX_X) * TAPIS_LARGEUR_CM - (TAPIS_LARGEUR_CM / 2.0);
  return cm * 10.0; // en mm
}

void setup() {
  Wire.begin(); // maître : pas d'adresse
  Serial.begin(9600);
  delay(500);
  Serial.println("Carte de com demarree");
}

void loop() {
  // 1. Lire Pixy
  DonneesPixy pixy = lirePixy();

  if (pixy.nouveau && pixy.cube_id > 0) {
    Serial.print("Cube detecte id="); Serial.print(pixy.cube_id);
    Serial.print(" x="); Serial.print(pixy.x);
    Serial.print(" y="); Serial.println(pixy.y);

    // 2. Lire vitesse convoyeur
    float vitesse = lireVitesseConvoyeur(); // cm/s
    Serial.print("Vitesse convoyeur: "); Serial.print(vitesse); Serial.println(" cm/s");

    // 3. Vérifier que le delta est libre
    byte statut = lireStatutDelta();
    if (statut == 1) {
      Serial.println("Delta occupe, cube ignore");
      delay(100);
      return;
    }

    // 4. Calculer le délai avant que le cube arrive sous le delta
    // Le cube est à (DIST_CAM_DELTA_CM) cm du delta
    // Temps d'arrivée = distance / vitesse
    float delai_s = DIST_CAM_DELTA_CM / vitesse;
    Serial.print("Delai attente: "); Serial.print(delai_s * 1000); Serial.println(" ms");

    // 5. Attendre
    delay((unsigned long)(delai_s * 1000));

    // 6. Calculer position latérale du cube (Z dans le repère delta)
    int16_t z_mm = (int16_t)pixelVersLateral_mm(pixy.x);
    int16_t x_mm = 0; // cube arrive centré sous le delta sur X

    Serial.print("Envoi delta x="); Serial.print(x_mm);
    Serial.print(" z="); Serial.println(z_mm);

    // 7. Envoyer au delta
    envoyerCommandeDelta(x_mm, z_mm);
  }

  delay(80); // cadence de polling ~12Hz
}
