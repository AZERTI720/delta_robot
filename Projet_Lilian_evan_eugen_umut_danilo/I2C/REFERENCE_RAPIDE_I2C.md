# Référence Rapide I2C - Fiche Technique

## ARCHITECTURE SYSTÈME

```
┌──────────────────────────────────────────────────────────────┐
│                    BUS I2C (SDA/SCL)                          │
├──────────────────────────────────────────────────────────────┤
│
├─ MAÎTRE (Communication)
│  └─ Orchestre le tri des cubes
│     • Demande position Pixy
│     • Calcule coordonnées robot
│     • Pilote Delta et Tapis
│
├─ ESCLAVE #11 (Delta Robot)
│  └─ Reçoit: Positions XYZ, commandes ventouse
│  └─ Envoie: Statut mouvement
│
├─ ESCLAVE #12 (Caméra Pixy)
│  └─ Reçoit: Demandes position
│  └─ Envoie: Coordonnées pixel (X, Y)
│
└─ ESCLAVE #13 (Tapis Roulant)
   └─ Reçoit: Vitesse, démarrage/arrêt
   └─ Envoie: Statut moteur (actif/inactif)
```

---

## ADRESSES I2C

| Adresse | Hexadécimal | Composant |
|---------|-------------|-----------|
| 11 | 0x0B | Delta Robot (Dynamixel) |
| 12 | 0x0C | Caméra Pixy2 |
| 13 | 0x0D | Tapis Roulant |

---

## BROCHES I2C (Tous les Arduinos)

```
Arduino A4  ──────→ SDA (Data line)
Arduino A5  ──────→ SCL (Clock line)
Arduino GND ──────→ GND Commun
```

---

## COMMANDES PRINCIPALES

### Delta (Esclave 11)

```cpp
// 1. Déplacer vers position XYZ
Wire.beginTransmission(11);
Wire.write(0x01);           // CMD_MOVE_XYZ
Wire.write((byte*)&x, 4);   // X (float)
Wire.write((byte*)&y, 4);   // Y (float)
Wire.write((byte*)&z, 4);   // Z (float)
Wire.endTransmission();

// 2. Fermer ventouse
Wire.beginTransmission(11);
Wire.write(0x04);           // CMD_GRIPPER_CLOSE
Wire.endTransmission();

// 3. Ouvrir ventouse
Wire.beginTransmission(11);
Wire.write(0x03);           // CMD_GRIPPER_OPEN
Wire.endTransmission();

// 4. Demander statut
Wire.requestFrom(11, 1);
uint8_t status = Wire.read();  // 0xAA=OK, 0xBB=Mouvement, 0xEE=Erreur
```

### Pixy (Esclave 12)

```cpp
// Demander position
Wire.beginTransmission(12);
Wire.write(0x10);           // CMD_GET_POSITION
Wire.endTransmission();

// Recevoir position
Wire.requestFrom(12, 4);
uint16_t pixelX = (Wire.read() << 8) | Wire.read();
uint16_t pixelY = (Wire.read() << 8) | Wire.read();
```

### Tapis (Esclave 13)

```cpp
// Fixer vitesse (0-255)
Wire.beginTransmission(13);
Wire.write(0x20);           // CMD_SET_SPEED
Wire.write(0x00);           // MSB
Wire.write(127);            // LSB (50% vitesse)
Wire.endTransmission();

// Démarrer tapis
Wire.beginTransmission(13);
Wire.write(0x21);           // CMD_START
Wire.endTransmission();

// Arrêter tapis
Wire.beginTransmission(13);
Wire.write(0x22);           // CMD_STOP
Wire.endTransmission();
```

---

## CYCLE TRI COMPLET

```
1. Pixy détecte cube à pixel(X, Y)
2. Maître → Tapis: Démarrer
3. Maître → Delta: Aller à (X_robot, Y_robot, -120)
4. Maître → Delta: Fermer ventouse
5. Maître → Delta: Aller à position dépôt
6. Maître → Delta: Ouvrir ventouse
7. Maître → Tapis: Arrêter
8. Maître → Delta: Retour position initiale
9. Boucle → 1
```

---

## PINS UTILISÉES

### Arduino Maître (Communication)
```
A4 ← SDA I2C
A5 ← SCL I2C
```

### Arduino Esclave Delta (11)
```
A4       ← SDA I2C
A5       ← SCL I2C
Pin 2    → DIR Shield Dynamixel
Pin 4    → Relay ventouse
RX1/TX1  → Shield Dynamixel
```

### Arduino Esclave Pixy (12)
```
A4       ← SDA I2C
A5       ← SCL I2C
Pin 8    ← RX (SoftwareSerial)
Pin 9    → TX (SoftwareSerial)
```

### Arduino Esclave Tapis (13)
```
A4       ← SDA I2C
A5       ← SCL I2C
Pin 5    → PWM Vitesse moteur
Pin 6    → Direction moteur
Pin 7    → Enable moteur
```

---

## CODES D'ERREUR I2C

```cpp
Wire.endTransmission() retourne:
0 = Succès
1 = Données trop longues
2 = NACK à l'adresse
3 = NACK à la donnée
4 = Autre erreur
```

---

## STATUTS RETOURNÉS

### Delta
- `0xAA` = Prêt
- `0xBB` = En mouvement
- `0xEE` = Erreur

### Pixy
- Coordonnées XY en 16-bit
- `0xFFFF` = Pas de cube détecté

### Tapis
- `0x01` = En marche
- `0x00` = Arrêté
- `0xCC` = OK

---

## TEST RAPIDE - SCAN I2C

```cpp
#include <Wire.h>

void setup() {
  Wire.begin();
  Serial.begin(9600);
}

void loop() {
  Serial.println("\nScanning...");
  for(int addr = 1; addr < 127; addr++) {
    Wire.beginTransmission(addr);
    if (Wire.endTransmission() == 0) {
      Serial.print("Device at: ");
      Serial.println(addr);
    }
  }
  delay(5000);
}
```

Résultat attendu: 11, 12, 13

---

## CONVERSIONS DONNÉES

### Float (4 bytes)
```cpp
union FloatBytes {
  float value;
  byte bytes[4];
};

// Envoyer
FloatBytes fb;
fb.value = 123.45;
Wire.write(fb.bytes, 4);

// Recevoir
FloatBytes fb;
for(int i=0; i<4; i++) fb.bytes[i] = Wire.read();
float value = fb.value;
```

### Int16 (2 bytes)
```cpp
// Envoyer
uint16_t val = 12345;
Wire.write((val >> 8) & 0xFF);  // MSB
Wire.write(val & 0xFF);         // LSB

// Recevoir
uint16_t val = ((uint16_t)Wire.read() << 8) | Wire.read();
```

---

## TIMINGS CRITIQUES

```
I2C Clock : 100 kHz (par défaut)
Timeout lecture : 50-100ms
Délai entre commandes : 10-50ms
Délai mouvement delta : 1-3 secondes
```

---

## CALIBRAGE CLÉS

**ESCLAVE_11_Delta.ino**
```cpp
// À adapter selon VOTRE géométrie:
const float L = 133;    // Longueur bras
const float l = 47;     // Longueur bielles
const float d = 95;     // Distance moteurs
```

**MAITRE_Communication.ino**
```cpp
// À calibrer sur VOS positions réelles:
rx = (px - 160) * 0.4;   // À adapter
ry = (py - 100) * 0.3;   // À adapter
```

---

## FICHIERS À TÉLÉCHARGER

| Fichier | Arduino | Rôle |
|---------|---------|------|
| `MAITRE_Communication.ino` | UNO #1 | Orchestre tout |
| `ESCLAVE_11_Delta.ino` | UNO #2 | Delta + Ventouse |
| `ESCLAVE_12_Pixy.ino` | UNO #3 | Détection cubes |
| `ESCLAVE_13_Tapis.ino` | UNO #4 | Moteur tapis |

---

## CHECKLIST DÉMARRAGE

- [ ] I2C câble (SDA A4, SCL A5, GND)
- [ ] Tous les Arduinos détectés au scan I2C
- [ ] Librairies installées (Dynamixel2Arduino, Pixy2)
- [ ] Code calibré selon votre robot
- [ ] Monit série lancé
- [ ] Tapis, Delta et Pixy testés individuellement
- [ ] Cycle complet vérifié sans cube
- [ ] Système prêt pour production !

