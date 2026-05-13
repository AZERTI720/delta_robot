import processing.serial.*;

Serial port;

// ============================================
// REGLAGES
int SEUIL       = 7;    // pixels : le cube bouge seulement si deplacement > SEUIL
int SEUIL_JAUNE = 9;    // seuil plus grand pour le jaune (plus instable)
int TIMEOUT     = 1200; // ms avant qu'un cube soit considere absent
int FANTOME_MAX = 4000; // ms de prediction apres sortie du champ
// ============================================

// couleurs
color coulRouge = color(220, 50,  50);
color coulBleu  = color(50,  100, 220);
color coulVert  = color(50,  180, 80);
color coulJaune = color(240, 200, 30);

// position STABLE affichee (ne bouge que si > SEUIL)
float sxRouge = 0;  float syRouge = 0;  float saRouge = 0;
float sxBleu  = 0;  float syBleu  = 0;  float saBleu  = 0;
float sxVert  = 0;  float syVert  = 0;  float saVert  = 0;
float sxJaune = 0;  float syJaune = 0;  float saJaune = 0;

// temps de derniere detection
int tRouge = 0;  int tBleu = 0;  int tVert = 0;  int tJaune = 0;

// comptage
int nbRouge = 0;  int nbBleu = 0;  int nbVert = 0;  int nbJaune = 0;

// etait visible (pour compter quand il disparait)
boolean etaitRouge = false;
boolean etaitBleu  = false;
boolean etaitVert  = false;
boolean etaitJaune = false;

// vitesse
float vRouge = 0;  float vBleu = 0;  float vVert = 0;  float vJaune = 0;

// fantomes
boolean fRouge = false; float fxRouge=0; float fyRouge=0; float fvRouge=0; int ftRouge=0;
boolean fBleu  = false; float fxBleu =0; float fyBleu =0; float fvBleu =0; int ftBleu =0;
boolean fVert  = false; float fxVert =0; float fyVert =0; float fvVert =0; int ftVert =0;
boolean fJaune = false; float fxJaune=0; float fyJaune=0; float fvJaune=0; int ftJaune=0;

int LIMITE = 632;

// ============================================
void setup() {
  size(1032, 416);
  println("Ports serie :");
  printArray(Serial.list());
  port = new Serial(this, Serial.list()[Serial.list().length - 1], 9600);
  port.bufferUntil('\n');
}

// ============================================
void draw() {
  background(20, 20, 35);

  stroke(80, 80, 120);
  line(LIMITE, 0, LIMITE, height);

  fill(200, 200, 255);
  noStroke();
  textSize(18);
  textAlign(CENTER);
  text("PIXY2 - DETECTEUR DE CUBES", LIMITE / 2, 35);

  // --- ROUGE ---
  boolean actifRouge = (millis() - tRouge) < TIMEOUT;
  if (!actifRouge && etaitRouge) {
    nbRouge++; etaitRouge = false;
    if (vRouge > 0 && !fRouge) { fRouge=true; ftRouge=(int)millis(); fxRouge=sxRouge*2; fyRouge=syRouge*2; fvRouge=vRouge; }
  }
  if (actifRouge && !etaitRouge) etaitRouge = true;
  if (actifRouge) dessin(sxRouge*2, syRouge*2, saRouge, coulRouge, "ROUGE", 255);
  if (fRouge) {
    int dt = (int)millis() - ftRouge;
    float xp = fxRouge + fvRouge * 2.0 * dt;
    if (xp < width-28 && dt < FANTOME_MAX) dessin(xp, fyRouge, saRouge, coulRouge, "ROUGE", map(dt,0,FANTOME_MAX,200,20));
    else fRouge = false;
  }

  // --- BLEU ---
  boolean actifBleu = (millis() - tBleu) < TIMEOUT;
  if (!actifBleu && etaitBleu) {
    nbBleu++; etaitBleu = false;
    if (vBleu > 0 && !fBleu) { fBleu=true; ftBleu=(int)millis(); fxBleu=sxBleu*2; fyBleu=syBleu*2; fvBleu=vBleu; }
  }
  if (actifBleu && !etaitBleu) etaitBleu = true;
  if (actifBleu) dessin(sxBleu*2, syBleu*2, saBleu, coulBleu, "BLEU", 255);
  if (fBleu) {
    int dt = (int)millis() - ftBleu;
    float xp = fxBleu + fvBleu * 2.0 * dt;
    if (xp < width-28 && dt < FANTOME_MAX) dessin(xp, fyBleu, saBleu, coulBleu, "BLEU", map(dt,0,FANTOME_MAX,200,20));
    else fBleu = false;
  }

  // --- VERT ---
  boolean actifVert = (millis() - tVert) < TIMEOUT;
  if (!actifVert && etaitVert) {
    nbVert++; etaitVert = false;
    if (vVert > 0 && !fVert) { fVert=true; ftVert=(int)millis(); fxVert=sxVert*2; fyVert=syVert*2; fvVert=vVert; }
  }
  if (actifVert && !etaitVert) etaitVert = true;
  if (actifVert) dessin(sxVert*2, syVert*2, saVert, coulVert, "VERT", 255);
  if (fVert) {
    int dt = (int)millis() - ftVert;
    float xp = fxVert + fvVert * 2.0 * dt;
    if (xp < width-28 && dt < FANTOME_MAX) dessin(xp, fyVert, saVert, coulVert, "VERT", map(dt,0,FANTOME_MAX,200,20));
    else fVert = false;
  }

  // --- JAUNE ---
  boolean actifJaune = (millis() - tJaune) < TIMEOUT;
  if (!actifJaune && etaitJaune) {
    nbJaune++; etaitJaune = false;
    if (vJaune > 0 && !fJaune) { fJaune=true; ftJaune=(int)millis(); fxJaune=sxJaune*2; fyJaune=syJaune*2; fvJaune=vJaune; }
  }
  if (actifJaune && !etaitJaune) etaitJaune = true;
  if (actifJaune) dessin(sxJaune*2, syJaune*2, saJaune, coulJaune, "JAUNE", 255);
  if (fJaune) {
    int dt = (int)millis() - ftJaune;
    float xp = fxJaune + fvJaune * 2.0 * dt;
    if (xp < width-28 && dt < FANTOME_MAX) dessin(xp, fyJaune, saJaune, coulJaune, "JAUNE", map(dt,0,FANTOME_MAX,200,20));
    else fJaune = false;
  }

  barreStatut();
}

// ============================================
// Dessine le cube avec rotation + position + angle
void dessin(float cx, float cy, float angle, color c, String nom, float alpha) {
  float t = 28;

  // Carré qui tourne
  pushMatrix();
  translate(cx, cy);
  rotate(radians(angle));
  fill(red(c), green(c), blue(c), alpha);
  stroke(255, 255, 255, alpha);
  strokeWeight(2);
  rectMode(CENTER);
  rect(0, 0, t*2, t*2, 5);
  rectMode(CORNER);
  fill(255, 255, 255, alpha);
  noStroke();
  textAlign(CENTER, CENTER);
  textSize(10);
  text(nom, 0, 0);
  popMatrix();

  // Position et angle affiches sous le cube (sans rotation)
  fill(255, 255, 255, alpha);
  noStroke();
  textAlign(CENTER);
  textSize(9);
  text("x = " + round(cx/2) + " ; " + "y = " + round(cy/2) + " ; " + round(angle) + "°", cx, cy + t + 14);
}

// ============================================
void barreStatut() {
  fill(0, 0, 0, 150);
  noStroke();
  rect(0, height-60, width, 60);

  int nb = 0;
  if (millis()-tRouge < TIMEOUT) nb++;
  if (millis()-tBleu  < TIMEOUT) nb++;
  if (millis()-tVert  < TIMEOUT) nb++;
  if (millis()-tJaune < TIMEOUT) nb++;
  fill(150, 200, 255);
  textSize(11);
  textAlign(LEFT, CENTER);
  text("Visibles : " + nb, 10, height-48);

  afficherInfo(coulRouge, "ROUGE", nbRouge, vRouge, 0);
  afficherInfo(coulBleu,  "BLEU",  nbBleu,  vBleu,  1);
  afficherInfo(coulVert,  "VERT",  nbVert,  vVert,  2);
  afficherInfo(coulJaune, "JAUNE", nbJaune, vJaune, 3);
}

void afficherInfo(color c, String nom, int nb, float v, int col) {
  float bx = 10 + col * 200;
  float by = height - 25;
  fill(c); noStroke();
  rect(bx, by-7, 12, 12, 2);
  fill(200, 200, 255);
  textAlign(LEFT, CENTER);
  textSize(11);
  text(nom + " : " + nb, bx+16, by);
  if (v > 0) {
    fill(180, 230, 180);
    textSize(10);
    text(nf(v*1000,0,1) + " px/s  |  " + nf(v*1000*0.002,0,3) + " m/s", bx+16, by-16);
  }
}

// ============================================
float calculerVitesse(int xNow, int xOld, int tNow, int tOld) {
  int dt = tNow - tOld;
  if (dt <= 0) return 0;
  float v = (float)(xNow - xOld) / dt;
  if (v < 0) return 0;
  return v;
}

// Met a jour la position stable seulement si deplacement > seuil
void mettreAJour(int x, int y, float angle, int seuil) {
  // Cette fonction n'est pas appelee directement,
  // la logique est dans serialEvent par cube
}

// ============================================
// Format Serial : sig,x,y,angle
void serialEvent(Serial p) {
  String ligne = trim(p.readStringUntil('\n'));
  if (ligne == null) return;
  String[] v = split(ligne, ',');
  if (v.length != 3) return;

  int   sig   = int(v[0]);
  int   x     = int(v[1]);
  int   y     = int(v[2]);
  float angle = (v.length == 4) ? float(v[3]) : 0;
  
  if (sig < 1 || sig > 4) return;

  int now = (int) millis();

  if (sig == 1) {
    vRouge = calculerVitesse(x, round(sxRouge), now, tRouge);
    // Seuil : ne bouge que si deplacement > SEUIL
    if (abs(x - sxRouge) > SEUIL || abs(y - syRouge) > SEUIL) { sxRouge = x; syRouge = y; }
    if (abs(angle - saRouge) > 5) saRouge = angle;
    tRouge = now;
    if (!fRouge && x*2 >= LIMITE-28 && vRouge > 0) { fRouge=true; ftRouge=now; fxRouge=x*2.0; fyRouge=y*2.0; fvRouge=vRouge; }
  }
  else if (sig == 2) {
    vBleu = calculerVitesse(x, round(sxBleu), now, tBleu);
    if (abs(x - sxBleu) > SEUIL || abs(y - syBleu) > SEUIL) { sxBleu = x; syBleu = y; }
    if (abs(angle - saBleu) > 5) saBleu = angle;
    tBleu = now;
    if (!fBleu && x*2 >= LIMITE-28 && vBleu > 0) { fBleu=true; ftBleu=now; fxBleu=x*2.0; fyBleu=y*2.0; fvBleu=vBleu; }
  }
  else if (sig == 3) {
    vVert = calculerVitesse(x, round(sxVert), now, tVert);
    if (abs(x - sxVert) > SEUIL || abs(y - syVert) > SEUIL) { sxVert = x; syVert = y; }
    if (abs(angle - saVert) > 5) saVert = angle;
    tVert = now;
    if (!fVert && x*2 >= LIMITE-28 && vVert > 0) { fVert=true; ftVert=now; fxVert=x*2.0; fyVert=y*2.0; fvVert=vVert; }
  }
  else if (sig == 4) {
    vJaune = calculerVitesse(x, round(sxJaune), now, tJaune);
    if (abs(x - sxJaune) > SEUIL_JAUNE || abs(y - syJaune) > SEUIL_JAUNE) { sxJaune = x; syJaune = y; }
    if (abs(angle - saJaune) > 5) saJaune = angle;
    tJaune = now;
    if (!fJaune && x*2 >= LIMITE-28 && vJaune > 0) { fJaune=true; ftJaune=now; fxJaune=x*2.0; fyJaune=y*2.0; fvJaune=vJaune; }
  }
}
