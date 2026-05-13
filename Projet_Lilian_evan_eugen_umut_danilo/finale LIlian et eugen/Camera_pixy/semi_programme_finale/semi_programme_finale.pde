import processing.serial.*;

Serial port;

// ============================================
// REGLAGES
int   SEUIL;       // pixels : le cube bouge seulement si deplacement > SEUIL
int   TIMEOUT;     // ms avant qu'un cube soit considere absent
int   FANTOME_MAX; // ms de prediction apres sortie du champ
float TAILLE_CUBE; // demi-taille du carre
float PX_TO_METER; // 1 px Pixy = x metres

// --- Sliders ---
// Chaque slider : x, y, largeur, valeur min, valeur max, valeur actuelle
int sliderX    = 650; // position X du panneau sliders
int sliderY    = 60;  // position Y du premier slider
int sliderW    = 300; // largeur de la barre
int sliderH    = 8;   // hauteur de la barre
int sliderActif = -1; // -1 = aucun
// ============================================

// valeurs des sliders [min, max, valeur]
float minSEUIL = 1,      maxSEUIL = 50;
float minTIME  = 200,    maxTIME  = 5000;
float minFAN   = 1000,   maxFAN   = 10000;
float minTAI   = 10,     maxTAI   = 100;
float minPX    = 0.0001, maxPX    = 0.01;

// ============================================

float sSEUIL = 5; 
float sTIMEOUT = 1200;
float sFANTOME = 4000;
float sTAILLE = 28;
float sPX2M = 0.002;
// ============================================
// couleurs des 6 cubes
color coul1 = color(220, 50,  50); // Rouge
color coul2  = color(50,  100, 220); // Bleu
color coul3  = color(50,  180, 80); // Vert 
color coul4 = color(240, 200, 30); // Jaune
color coul5 = color(220, 50,  50);  // Rouge + Bleu   → violet
color coul6 = color(50,  180, 80);  // Rouge + Vert   → vert clair
color coul7 = color(220, 140, 30);  // Rouge + Jaune  → orange
color coul8 = color(50,  100, 220); // Bleu  + Vert   → bleu
color coul9 = color(100, 80,  220); // Bleu  + Jaune  → bleu violet
color coul10 = color(180, 220, 50);  // Vert  + Jaune  → jaune vert

// noms des cubes
String nom1 = "R";
String nom2 = "B";
String nom3 = "V";
String nom4 = "J";
String nom5 = "R+B";
String nom6 = "R+V";
String nom7 = "R+J";
String nom8 = "B+V";
String nom9 = "B+J";
String nom10 = "V+J";

// position et angle stables
float sx1=0; float sy1=0; float sa1=0;
float sx2=0; float sy2=0; float sa2=0;
float sx3=0; float sy3=0; float sa3=0;
float sx4=0; float sy4=0; float sa4=0;
float sx5=0; float sy5=0; float sa5=0;
float sx6=0; float sy6=0; float sa6=0;
float sx7=0; float sy7=0; float sa7=0;
float sx8=0; float sy8=0; float sa8=0;
float sx9=0; float sy9=0; float sa9=0;
float sx10=0; float sy10=0; float sa10=0;

// temps de derniere detection
int t1=0; int t2=0; int t3=0; int t4=0; int t5=0; int t6=0; int t7=0; int t8=0; int t9=0; int t10=0;

// comptage
int nb1=0; int nb2=0; int nb3=0; int nb4=0; int nb5=0; int nb6=0; int nb7=0; int nb8=0; int nb9=0; int nb10=0;

// etait visible
boolean e1=false; boolean e2=false; boolean e3=false;
boolean e4=false; boolean e5=false; boolean e6=false;
boolean e7=false; boolean e8=false; boolean e9=false;
boolean e10=false;

// vitesse
float v1=0; float v2=0; float v3=0; float v4=0; float v5=0; float v6=0; float v7=0; float v8=0; float v9=0; float v10=0;

// fantomes
boolean f1=false; float fx1=0; float fy1=0; float fv1=0; int ft1=0;
boolean f2=false; float fx2=0; float fy2=0; float fv2=0; int ft2=0;
boolean f3=false; float fx3=0; float fy3=0; float fv3=0; int ft3=0;
boolean f4=false; float fx4=0; float fy4=0; float fv4=0; int ft4=0;
boolean f5=false; float fx5=0; float fy5=0; float fv5=0; int ft5=0;
boolean f6=false; float fx6=0; float fy6=0; float fv6=0; int ft6=0;
boolean f7=false; float fx7=0; float fy7=0; float fv7=0; int ft7=0;
boolean f8=false; float fx8=0; float fy8=0; float fv8=0; int ft8=0;
boolean f9=false; float fx9=0; float fy9=0; float fv9=0; int ft9=0;
boolean f10=false; float fx10=0; float fy10=0; float fv10=0; int ft10=0;

int LIMITE = 632;

// ============================================
void setup() {
  size(1220, 440);
  println("Ports serie :");
  printArray(Serial.list());
  port = new Serial(this, Serial.list()[Serial.list().length - 1], 9600);
  port.bufferUntil('\n');
  stroke(160, 160, 150);
}

// ============================================
void draw() {
  background(20, 20, 35);

  stroke(80, 80, 120);
  line(LIMITE, 0, LIMITE, height);

  SEUIL       = round(sSEUIL);
  TIMEOUT     = round(sTIMEOUT);
  FANTOME_MAX = round(sFANTOME);
  TAILLE_CUBE = sTAILLE;
  PX_TO_METER = sPX2M;

  fill(200, 200, 255);
  noStroke();
  textSize(18);
  textAlign(CENTER);
  text("PIXY2 - DETECTEUR DE CUBES", LIMITE / 2, 35);

  gererCube(1, sx1, sy1, sa1, t1, e1, nb1, v1, f1, fx1, fy1, fv1, ft1, coul1, nom1);
  gererCube(2, sx2, sy2, sa2, t2, e2, nb2, v2, f2, fx2, fy2, fv2, ft2, coul2, nom2);
  gererCube(3, sx3, sy3, sa3, t3, e3, nb3, v3, f3, fx3, fy3, fv3, ft3, coul3, nom3);
  gererCube(4, sx4, sy4, sa4, t4, e4, nb4, v4, f4, fx4, fy4, fv4, ft4, coul4, nom4);
  gererCube(5, sx5, sy5, sa5, t5, e5, nb5, v5, f5, fx5, fy5, fv5, ft5, coul5, nom5);
  gererCube(6, sx6, sy6, sa6, t6, e6, nb6, v6, f6, fx6, fy6, fv6, ft6, coul6, nom6);
  gererCube(7, sx7, sy7, sa7, t7, e7, nb7, v7, f7, fx7, fy7, fv7, ft7, coul7, nom7);
  gererCube(8, sx8, sy8, sa8, t8, e8, nb8, v8, f8, fx8, fy8, fv8, ft8, coul8, nom8);
  gererCube(9, sx9, sy9, sa9, t9, e9, nb9, v9, f9, fx9, fy9, fv9, ft9, coul9, nom9);
  gererCube(10, sx10, sy10, sa10, t10, e10, nb10, v10, f10, fx10, fy10, fv10, ft10, coul10, nom10);

  barreStatut();
}

// ============================================
void gererCube(int id, float sx, float sy, float sa, int t,
               boolean etait, int nb, float v,
               boolean fantome, float fxc, float fyc, float fvc, int ftc,
               color c, String nom) {

  boolean actif = (millis() - t) < TIMEOUT;

  // Comptage + lancement fantome
  if (!actif && etait) {
    incrementer(id);
    if (v > 0 && !fantome) lancerFantome(id, sx*2, sy*2, v);
    setEtait(id, false);
  }
  if (actif && !etait) setEtait(id, true);

  // Cube reel
  if (actif) dessin(sx*2, sy*2, sa, c, nom, 255);

  // Fantome
  if (fantome) {
    int   dt = (int)millis() - ftc;
    float xp = fxc + fvc * 2.0 * dt;
    if (xp < width-28 && dt < FANTOME_MAX) dessin(xp, fyc, sa, c, nom, map(dt,0,FANTOME_MAX,200,20));
    else eteindreFan(id);
  }
}

// ============================================
void dessin(float cx, float cy, float angle, color c, String nom, float alpha) {
  float t = 28;
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
  fill(200, 210, 255, alpha);
  noStroke();
  textAlign(CENTER);
  textSize(9);
  text("x = " + round(cx/2) + " ; " + "y = " + round(cy/2) + " ; " + round(angle) + "°", cx, cy + t + 14);
}

// ============================================
void barreStatut() {
  fill(0, 0, 0, 150);
  noStroke();
  rect(0, height-80, width, 80); // fond plus grand

  int nb = 0;
  if (millis()-t1<TIMEOUT) nb++;
  if (millis()-t2<TIMEOUT) nb++;
  if (millis()-t3<TIMEOUT) nb++;
  if (millis()-t4<TIMEOUT) nb++;
  if (millis()-t5<TIMEOUT) nb++;
  if (millis()-t6<TIMEOUT) nb++;
  if (millis()-t7<TIMEOUT) nb++;
  if (millis()-t8<TIMEOUT) nb++;
  if (millis()-t9<TIMEOUT) nb++;
  if (millis()-t10<TIMEOUT) nb++;

  fill(150, 200, 255);
  textSize(11);
  textAlign(LEFT, CENTER);
  text("Visibles : " + nb, 10, height-68);

  // Ligne 1 : signatures simples (cubes 1-4)
  afficherInfo(coul1, nom1, nb1, v1, 0, height-50);
  afficherInfo(coul2, nom2, nb2, v2, 1, height-50);
  afficherInfo(coul3, nom3, nb3, v3, 2, height-50);
  afficherInfo(coul4, nom4, nb4, v4, 3, height-50);

  // Ligne 2 : color codes (cubes 5-10)
  afficherInfo(coul5,  nom5,  nb5,  v5,  0, height-25);
  afficherInfo(coul6,  nom6,  nb6,  v6,  1, height-25);
  afficherInfo(coul7,  nom7,  nb7,  v7,  2, height-25);
  afficherInfo(coul8,  nom8,  nb8,  v8,  3, height-25);
  afficherInfo(coul9,  nom9,  nb9,  v9,  4, height-25);
  afficherInfo(coul10, nom10, nb10, v10, 5, height-25);
}

void afficherInfo(color c, String nom, int nb, float v, int col, float by) {
  float bx = 10 + col * 170;
  fill(c); noStroke();
  rect(bx, by-7, 10, 10, 2);
  fill(200, 200, 255);
  textAlign(LEFT, CENTER);
  textSize(10);
  float vMS = v * 1000 * PX_TO_METER;
  text(nom + ":" + nb + (v>0 ? "  " + nf(vMS,0,3)+"m/s" : ""), bx+14, by);
}

// ============================================
float calculerVitesse(int xNow, int xOld, int tNow, int tOld) {
  int dt = tNow - tOld;
  if (dt <= 0.1) return 0;
  float v = (float)(xNow - xOld) / dt;
  return v < 0 ? 0: v;
}

// ============================================
// Helpers pour mettre a jour les variables par id
void incrementer(int id) {
  if (id==1) nb1++; else if (id==2) nb2++; else if (id==3) nb3++;
  else if (id==4) nb4++; else if (id==5) nb5++; else if (id==6) nb6++; else if (id==7) nb7++;
  else if (id==8) nb8++; else if (id==9) nb9++; else if (id==10) nb10++;
}
void setEtait(int id, boolean val) {
  if (id==1) e1=val; else if (id==2) e2=val; else if (id==3) e3=val;
  else if (id==4) e4=val; else if (id==5) e5=val; else if (id==6) e6=val; else if (id==7) e7=val;
  else if (id==8) e8=val; else if (id==9) e9=val; else if (id==10) e10=val;
}
void lancerFantome(int id, float x, float y, float v) {
  int now = (int)millis();
  if (id==1) { f1=true; ft1=now; fx1=x; fy1=y; fv1=v; }
  else if (id==2) { f2=true; ft2=now; fx2=x; fy2=y; fv2=v; }
  else if (id==3) { f3=true; ft3=now; fx3=x; fy3=y; fv3=v; }
  else if (id==4) { f4=true; ft4=now; fx4=x; fy4=y; fv4=v; }
  else if (id==5) { f5=true; ft5=now; fx5=x; fy5=y; fv5=v; }
  else if (id==6) { f6=true; ft6=now; fx6=x; fy6=y; fv6=v; }
  else if (id==7) { f7=true; ft7=now; fx7=x; fy7=y; fv7=v; }
  else if (id==8) { f8=true; ft8=now; fx8=x; fy8=y; fv8=v; }
  else if (id==9) { f9=true; ft9=now; fx9=x; fy9=y; fv9=v; }
  else            { f10=true; ft10=now; fx10=x; fy10=y; fv10=v; }
}
void eteindreFan(int id) {
  if (id==1) f1=false; else if (id==2) f2=false; else if (id==3) f3=false;
  else if (id==4) f4=false; else if (id==5) f5=false; else if (id==6) f6=false;
  else if (id==7) f7=false; else if (id==8) f8=false; else if (id==9) f9=false; else f10=false;
}

// ============================================
// Format Serial : cube,x,y,angle
void serialEvent(Serial p) {
  String ligne = trim(p.readStringUntil('\n'));
  if (ligne == null) return;
  String[] v = split(ligne, ',');
  if (v.length < 3) return;

  int   id    = int(v[0]);
  int   x     = int(v[1]);
  int   y     = int(v[2]);
  float angle = (v.length == 4) ? float(v[3]) : 0;
  if (id < 1 || id > 10) return;

  int now = (int) millis();

  if (id == 1) {
    v1 = calculerVitesse(x, round(sx1), now, t1);
    if (abs(x-sx1)>SEUIL || abs(y-sy1)>SEUIL) { sx1=x; sy1=y; }
    if (abs(angle-sa1)>5) sa1=angle;
    t1=now;
    if (!f1 && x*2>=LIMITE-28 && v1>0) lancerFantome(1, x*2.0, y*2.0, v1);
  } else if (id == 2) {
    v2 = calculerVitesse(x, round(sx2), now, t2);
    if (abs(x-sx2)>SEUIL || abs(y-sy2)>SEUIL) { sx2=x; sy2=y; }
    if (abs(angle-sa2)>5) sa2=angle;
    t2=now;
    if (!f2 && x*2>=LIMITE-28 && v2>0) lancerFantome(2, x*2.0, y*2.0, v2);
  } else if (id == 3) {
    v3 = calculerVitesse(x, round(sx3), now, t3);
    if (abs(x-sx3)>SEUIL || abs(y-sy3)>SEUIL) { sx3=x; sy3=y; }
    if (abs(angle-sa3)>5) sa3=angle;
    t3=now;
    if (!f3 && x*2>=LIMITE-28 && v3>0) lancerFantome(3, x*2.0, y*2.0, v3);
  } else if (id == 4) {
    v4 = calculerVitesse(x, round(sx4), now, t4);
    if (abs(x-sx4)>SEUIL || abs(y-sy4)>SEUIL) { sx4=x; sy4=y; }
    if (abs(angle-sa4)>5) sa4=angle;
    t4=now;
    if (!f4 && x*2>=LIMITE-28 && v4>0) lancerFantome(4, x*2.0, y*2.0, v4);
  } else if (id == 5) {
    v5 = calculerVitesse(x, round(sx5), now, t5);
    if (abs(x-sx5)>SEUIL || abs(y-sy5)>SEUIL) { sx5=x; sy5=y; }
    if (abs(angle-sa5)>5) sa5=angle;
    t5=now;
    if (!f5 && x*2>=LIMITE-28 && v5>0) lancerFantome(5, x*2.0, y*2.0, v5);
  } else if (id == 6) {
    v6 = calculerVitesse(x, round(sx6), now, t6);
    if (abs(x-sx6)>SEUIL || abs(y-sy6)>SEUIL) { sx6=x; sy6=y; }
    if (abs(angle-sa6)>5) sa6=angle;
    t6=now;
    if (!f6 && x*2>=LIMITE-28 && v6>0) lancerFantome(6, x*2.0, y*2.0, v6);
  } else if (id == 7) {
    v7 = calculerVitesse(x, round(sx7), now, t7);
    if (abs(x-sx7)>SEUIL || abs(y-sy7)>SEUIL) { sx7=x; sy7=y; }
    if (abs(angle-sa7)>5) sa7=angle;
    t7=now;
    if (!f7 && x*2>=LIMITE-28 && v7>0) lancerFantome(7, x*2.0, y*2.0, v7);
  } else if (id == 8) {
    v8 = calculerVitesse(x, round(sx8), now, t8);
    if (abs(x-sx8)>SEUIL || abs(y-sy8)>SEUIL) { sx8=x; sy8=y; }
    if (abs(angle-sa8)>5) sa8=angle;
    t8=now;
    if (!f8 && x*2>=LIMITE-28 && v8>0) lancerFantome(8, x*2.0, y*2.0, v8);
  } else if (id == 9) {
    v9 = calculerVitesse(x, round(sx9), now, t9);
    if (abs(x-sx9)>SEUIL || abs(y-sy9)>SEUIL) { sx9=x; sy9=y; }
    if (abs(angle-sa9)>5) sa9=angle;
    t9=now;
    if (!f9 && x*2>=LIMITE-28 && v9>0) lancerFantome(9, x*2.0, y*2.0, v9);
  } else if (id == 10) {
    v10 = calculerVitesse(x, round(sx10), now, t10);
    if (abs(x-sx10)>SEUIL || abs(y-sy10)>SEUIL) { sx10=x; sy10=y; }
    if (abs(angle-sa10)>5) sa10=angle;
    t10=now;
    if (!f10 && x*2>=LIMITE-28 && v10>0) lancerFantome(10, x*2.0, y*2.0, v10);
  }
}
