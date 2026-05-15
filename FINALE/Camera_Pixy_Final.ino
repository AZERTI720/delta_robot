#include <Pixy2.h>
Pixy2 pixy;

void setup() {
  Serial.begin(9600);
  pixy.init();
}

void loop() {
  pixy.ccc.getBlocks();

  if (pixy.ccc.numBlocks) {
    for (int i = 0; i < pixy.ccc.numBlocks; i++) {

      int sig   = pixy.ccc.blocks[i].m_signature;
      int x     = pixy.ccc.blocks[i].m_x;
      int y     = pixy.ccc.blocks[i].m_y;
      int angle = pixy.ccc.blocks[i].m_angle; // toujours 0 si pas color code
      int cube = 0;

      if (sig < 7)
      {
        if (sig == 1)  cube = 1; // Rouge
        if (sig == 2)  cube = 2; // Bleu
        if (sig == 3)  cube = 3; // Vert
        if (sig == 4)  cube = 4; // Jaune
      }
      if (sig > 7)
      {
        if (sig == 10)  cube = 5; // Rouge + Bleu
        if (sig == 11)  cube = 6; // Rouge + Vert
        if (sig == 12)  cube = 7; // Rouge + Jaune


        if (sig == 19)  cube = 8; // Bleu + Vert
        if (sig == 20)  cube = 9; // Bleu + Jaune


        if (sig == 28)  cube = 10; // Vert + Jaune
      }
      if (cube == 0) continue; // signature non reconnue, on ignore
      // Envoi : cube,x,y,angle
      Serial.print(cube);
      Serial.print(",");
      Serial.print(x);
      Serial.print(",");
      Serial.print(y);
      Serial.print(",");
      Serial.println(angle);
    }
  }

  delay(100);
}
