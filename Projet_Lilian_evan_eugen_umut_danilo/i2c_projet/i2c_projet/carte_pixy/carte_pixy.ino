#include <Pixy2.h>
#include <Wire.h>

Pixy2 pixy;

// Données à envoyer au maître
volatile int  cube_id  = 0;
volatile int  cube_x   = 0;   // pixels (0-315)
volatile int  cube_y   = 0;
volatile bool nouveau   = false;

// Buffer de réponse I2C (6 octets : id, xH, xL, yH, yL, flag)
void onRequest() {
  byte buf[6];
  buf[0] = (byte)cube_id;
  buf[1] = (byte)(cube_x >> 8);
  buf[2] = (byte)(cube_x & 0xFF);
  buf[3] = (byte)(cube_y >> 8);
  buf[4] = (byte)(cube_y & 0xFF);
  buf[5] = nouveau ? 1 : 0;
  Wire.write(buf, 6);
  nouveau = false; // acquitté
}

void setup() {
  Wire.begin(0x08);
  Wire.onRequest(onRequest);
  pixy.init();
}

void loop() {
  pixy.ccc.getBlocks();

  if (pixy.ccc.numBlocks) {
    int sig = pixy.ccc.blocks[0].m_signature;
    int id  = 0;

    if      (sig == 1)  id = 1;  // Rouge
    else if (sig == 2)  id = 2;  // Bleu
    else if (sig == 3)  id = 3;  // Vert
    else if (sig == 4)  id = 4;  // Jaune
    else if (sig == 10) id = 5;  // Rouge+Bleu
    else if (sig == 11) id = 6;  // Rouge+Vert
    else if (sig == 12) id = 7;  // Rouge+Jaune
    else if (sig == 19) id = 8;  // Bleu+Vert
    else if (sig == 20) id = 9;  // Bleu+Jaune
    else if (sig == 28) id = 10; // Vert+Jaune

    if (id > 0) {
      cube_id  = id;
      cube_x   = pixy.ccc.blocks[0].m_x;
      cube_y   = pixy.ccc.blocks[0].m_y;
      nouveau  = true;
    }
  }

  delay(50);
}
