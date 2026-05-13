#include <Wire.h>
#include <Pixy2.h>

Pixy2 pixy;

#define TAPIS_ADDR   0x20
#define REG_VITESSE  0x01
#define REG_POSITION 0x02

int16_t lireRegistre(uint8_t adresseEsclave, uint8_t reg) {
  Wire.beginTransmission(adresseEsclave);
  Wire.write(reg);
  Wire.endTransmission(false);

  Wire.requestFrom(adresseEsclave, (uint8_t)2);
  if (Wire.available() == 2) {
    return (Wire.read() << 8) | Wire.read(); // corrigé
  }
  return -1;
}

void setup() {
  Serial.begin(115200); // manquait
  Wire.begin();
  Wire.setClock(400000);
  pixy.init();
}
void loop() {
  pixy.ccc.getBlocks();

  if (pixy.ccc.numBlocks) {
    for (int i = 0; i < pixy.ccc.numBlocks; i++) {
      int sig   = pixy.ccc.blocks[i].m_signature;
      int x     = pixy.ccc.blocks[i].m_x;
      int y     = pixy.ccc.blocks[i].m_y;
      int angle = pixy.ccc.blocks[i].m_angle;

      int cube = 0;

      if (sig < 7) {
        if (sig == 1) cube = 1;  // Rouge
        if (sig == 2) cube = 2;  // Bleu
        if (sig == 3) cube = 3;  // Vert
        if (sig == 4) cube = 4;  // Jaune
      }

      if (sig > 7) {
        if (sig == 10) cube = 5;  // Rouge + Bleu
        if (sig == 11) cube = 6;  // Rouge + Vert
        if (sig == 12) cube = 7;  // Rouge + Jaune
        if (sig == 19) cube = 8;  // Bleu + Vert
        if (sig == 20) cube = 9;  // Bleu + Jaune
        if (sig == 28) cube = 10; // Vert + Jaune
      }

      if (cube == 0) continue; // signature non reconnue

      Serial.print(cube);
      Serial.print(",");
      Serial.print(x);
      Serial.print(",");
      Serial.print(y);
      Serial.print(",");
      Serial.println(angle);
    }
  }
  int16_t vitesse  = lireRegistre(TAPIS_ADDR, REG_VITESSE);
  int16_t position = lireRegistre(TAPIS_ADDR, REG_POSITION);

  Serial.print("[TAPIS] Vitesse: ");  Serial.print(vitesse);
  Serial.print(" mm/s  Position: "); Serial.println(position);

  delay(100);
}
