# Guide de Mise en Place - Protocole I2C Système de Tri

## 1. PRÉPARATION HARDWARE

### 1.1 Câblage I2C

Tous les Arduinos doivent être connectés sur le même bus I2C :

```
┌─────────────────────────────────────────┐
│          GND Commun                     │
│  [ARD1] [ARD2] [ARD3] [ARD4]           │
│   |       |       |       |            │
│  [A4]→[A4]→[A4]→[A4]  (SDA)           │
│   |       |       |       |            │
│  [A5]→[A5]→[A5]→[A5]  (SCL)           │
│   |       |       |       |            │
│  GND─GND─GND─GND─GND─GND─GND          │
└─────────────────────────────────────────┘

⚠️ IMPORTANT :
- Tous les GND DOIVENT être reliés ensemble
- Ajouter des pull-ups 4.7kΩ si pas d'adaptateur I2C
- Utiliser un câble torsadé ou blindé si longueur > 50cm
```

### 1.2 Pins Utilisées par Arduino

#### MAÎTRE (Arduino Communication)
```
A4 (SDA) → I2C
A5 (SCL) → I2C
GND      → GND Commun
```

#### ESCLAVE DELTA (Arduino Dynamixel)
```
A4 (SDA)        → I2C
A5 (SCL)        → I2C
GND             → GND Commun
Pin 2           → DIR pin du Shield Dynamixel
Pin 4           → Relay Ventouse (HIGH=ouvert, LOW=fermé)
RX1/TX1         → Shield Dynamixel (série)
```

#### ESCLAVE PIXY (Arduino Caméra)
```
A4 (SDA)        → I2C
A5 (SCL)        → I2C
GND             → GND Commun
Pin 8 (RX)      → Pixy2 TX
Pin 9 (TX)      → Pixy2 RX
```

#### ESCLAVE TAPIS (Arduino Moteur)
```
A4 (SDA)        → I2C
A5 (SCL)        → I2C
GND             → GND Commun
Pin 5 (PWM)     → Entrée PWM du contrôleur moteur
Pin 6           → Pin Direction du contrôleur
Pin 7           → Pin Enable du contrôleur
```

---

## 2. INSTALLATION LOGICIELLE

### 2.1 Librairies Requises (Arduino IDE)

```
Sketch → Include Library → Manage Libraries

À installer:
1. Wire              (Builtin)
2. Dynamixel2Arduino (https://github.com/ROBOTIS-GIT/DynamixelSDK)
3. Pixy2             (https://github.com/charmedlabs/pixy2)
```

### 2.2 Installation Dynamixel2Arduino

```bash
# Via Arduino IDE:
Manage Libraries → Chercher "Dynamixel2Arduino" → Installer

# Ou manuellement:
Télécharger: https://github.com/ROBOTIS-GIT/DynamixelSDK/tree/master/Arduino
Copier dans: Mes documents/Arduino/libraries/
```

### 2.3 Installation Pixy2

```bash
# Via Arduino IDE:
Manage Libraries → Chercher "Pixy2" → Installer

# Ou:
https://github.com/charmedlabs/pixy2
```

---

## 3. TÉLÉCHARGEMENT DES SKETCHES

### Étape 1 : Télécharger le code MAÎTRE

1. **Ouvrir** `MAITRE_Communication.ino` dans Arduino IDE
2. **Sélectionner le port** COM et la carte Arduino UNO
3. **Vérifier** (Ctrl+R)
4. **Télécharger** (Ctrl+U) sur l'Arduino Maître

### Étape 2 : Télécharger l'ESCLAVE DELTA

1. **Ouvrir** `ESCLAVE_11_Delta.ino` dans Arduino IDE
2. **Sélectionner la même carte** (Arduino UNO)
3. **Sélectionner le port COM** du Delta
4. **Télécharger** sur l'Arduino Delta

### Étape 3 : Télécharger l'ESCLAVE PIXY

1. **Ouvrir** `ESCLAVE_12_Pixy.ino` dans Arduino IDE
2. **Sélectionner un nouveau port COM** pour le Pixy
3. **Télécharger** sur l'Arduino Pixy

### Étape 4 : Télécharger l'ESCLAVE TAPIS

1. **Ouvrir** `ESCLAVE_13_Tapis.ino` dans Arduino IDE
2. **Sélectionner le port COM** du Tapis
3. **Télécharger** sur l'Arduino Tapis

---

## 4. TEST DE CONNEXION I2C

### 4.1 Test Simple (Scan I2C)

Charger ce sketch de test sur le Maître :

```cpp
#include <Wire.h>

void setup() {
  Wire.begin();
  Serial.begin(9600);
  Serial.println("I2C Scan starting...");
}

void loop() {
  Serial.println("\nScanning I2C bus...");
  
  byte error, address;
  int nDevices = 0;
  
  for(address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    
    if (error == 0) {
      Serial.print("Device found at address 0x");
      Serial.print(address, HEX);
      Serial.print(" (");
      Serial.print(address);
      Serial.println(")");
      nDevices++;
    }
    else if (error == 4) {
      Serial.print("Unknown error at address 0x");
      Serial.println(address, HEX);
    }
  }
  
  if (nDevices == 0) {
    Serial.println("No I2C devices found\n");
  } else {
    Serial.println("Scan complete\n");
  }
  
  delay(5000);
}
```

**Résultat attendu :**
```
I2C Scan starting...
Device found at address 0x0B (11)  ← Delta
Device found at address 0x0C (12)  ← Pixy
Device found at address 0x0D (13)  ← Tapis
```

---

## 5. CALIBRAGE CRUCIAL

### 5.1 Calibrage Cinématique Delta

Le code du Delta contient des paramètres qui DOIVENT être adaptés à votre robot :

**Fichier:** `ESCLAVE_11_Delta.ino`

```cpp
// Ligne ~30 - Adapter ces paramètres!
const float L = 133;  // Longueur des bras en mm
const float l = 47;   // Longueur des bielles en mm
const float d = 95;   // Distance entre moteurs en mm

// Ligne ~80 - Adapter la fonction IK selon votre géométrie
bool inverseKinematics(float x, float y, float z, float &t1, float &t2, float &t3) {
  // À adapter selon vos calculs exacts!
}

// Ligne ~99 - Adapter les limites d'angle
uint16_t angleToPosition(float angle) {
  // Pour AX-12A: 0-300° = 0-1023
  // Adapter selon votre moteur!
  float degrees = angle * 180.0 / PI;
  uint16_t position = map(degrees, -150, 150, 0, 1023);
  return constrain(position, 0, 1023);
}
```

### 5.2 Calibrage Pixy2

**Fichier:** `ESCLAVE_12_Pixy.ino`

Vous pouvez laisser tel quel. La Pixy détecte automatiquement les blocs de couleur.

**À faire manuellement :**
1. Brancher l'Arduino Pixy sur USB
2. Télécharger Pixy MonitorAndTune (site Pixy)
3. Calibrer les signatures de couleur des cubes

### 5.3 Calibrage Transformation Pixel → Robot

**CRITIQUE!** Dans le Maître, ligne ~140 :

```cpp
void pixelToRobotCoordinates(uint16_t px, uint16_t py, float &rx, float &ry) {
  // À CALIBRER!
  
  // Pixy: 0-319 en X, 0-199 en Y
  // Robot: coordonnées en mm
  
  // Exemple à adapter:
  rx = (px - 160) * 0.4;   // 0.4 mm/pixel - À MESURER!
  ry = (py - 100) * 0.3;   // 0.3 mm/pixel - À MESURER!
  
  rx = constrain(rx, -100, 100);
  ry = constrain(ry, -150, 0);
}
```

**Procédure de calibrage :**

1. **Marquer les positions** : Sur la surface, marquer 4 points aux coins du champ de vue Pixy
2. **Noter les pixels** : Pour chaque point, lire pixelX et pixelY de la console série
3. **Mesurer distances robot** : Mesurer les distances réelles en mm
4. **Calculer les échelles :**
   - Échelle X = (dist_max - dist_min) / (pixel_max - pixel_min)
   - Échelle Y = (dist_max - dist_min) / (pixel_max - pixel_min)

---

## 6. DÉMARRAGE SYSTÈME

### 6.1 Procédure de Démarrage

1. **Allumer tous les Arduinos** et attendre 3 secondes (initialisation)
2. **Ouvrir** le moniteur série du Maître (Ctrl+Shift+M)
3. **Vérifier** l'affichage :
```
=== ARDUINO MAÎTRE - Communication ===
Initialisation des esclaves...
Delta (addr 11): ✓ Connecté
Pixy (addr 12): ✓ Connecté
Tapis (addr 13): ✓ Connecté
Système prêt!
```

4. **Placer un cube** devant la caméra Pixy
5. **Appuyer sur Reset** du Maître pour démarrer le cycle

### 6.2 Sortie Console Attendue

```
--- Détection cube ---
Cube détecté à Pixel(280, 120)
Position robot calculée: X=48.0, Y=-36.0, Z=-120
Démarrage du tapis...
✓ Commande Delta envoyée
Fermeture ventouse...
Déplacement vers position de dépôt...
Ouverture ventouse...
Arrêt du tapis...
Retour position initiale...
```

---

## 7. DIAGNOSTIC & DÉPANNAGE

### Problème : Aucun esclave détecté

**Cause possible :** Câblage I2C incorrect

```
Vérifier:
1. SDA et SCL bien connectés sur A4 et A5
2. GND commun entre toutes les cartes
3. Pas de court-circuit
4. Plug-ups 4.7kΩ en place
```

### Problème : Erreur "NACK on address"

**Cause possible :** Adresse I2C incorrecte ou pas d'esclave

```cpp
// Dans le code esclave, vérifier:
#define MY_ADDRESS 11  // Doit être 11, 12 ou 13
Wire.begin(MY_ADDRESS);
```

### Problème : Moteurs Dynamixel ne bougent pas

**Vérifier :**
1. Moteurs allumés (Tx1/Rx1 branché)
2. Vitesse moteur > 0 : `motorSpeed = 100`
3. Limite d'angle pas dépassée (position 0-1023)

### Problème : Pixy ne détecte rien

1. Vérifier alimentation Pixy
2. Lancer Pixy MonitorAndTune et calibrer signatures
3. Vérifier que `trackingEnabled = true`

### Problème : Tapis ne démarre pas

1. Vérifier pin Enable (pin 7) en HIGH
2. Vérifier PWM (pin 5) > 0
3. Tester avec ce code de test:

```cpp
void setup() {
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
}

void loop() {
  digitalWrite(7, HIGH);      // Enable
  digitalWrite(6, LOW);       // Forward
  analogWrite(5, 200);        // 78% vitesse
  delay(3000);
  
  analogWrite(5, 0);          // Stop
  delay(1000);
}
```

---

## 8. CONFIGURATION MOTEUR TAPIS EXEMPLE

### L'298N (Module bridge courant) - Configuration Typique

```
         [L298N]
            |
        IN1 IN2 OUT
         |   |   |
       [6] [5] [Moteur]
       Dir PWM
       
- Pin 5 (PWM)  → IN2 (Vitesse)
- Pin 6 (DIR)  → IN1 (Direction)
- Pin 7 (EN)   → ENA (Enable)
```

**Code adaptation :**
```cpp
void setSpeed(uint16_t speed) {
  currentSpeed = (uint8_t)speed;
  analogWrite(MOTOR_PWM, currentSpeed);  // Pin 5
}

void setDirection(uint8_t direction) {
  currentDirection = direction;
  digitalWrite(MOTOR_DIR, direction);    // Pin 6
}

void startMotor() {
  digitalWrite(MOTOR_EN, HIGH);          // Pin 7 - HIGH pour activer
}

void stopMotor() {
  digitalWrite(MOTOR_EN, LOW);           // Pin 7 - LOW pour arrêter
}
```

---

## 9. PROTOCOLE DE TEST ÉTAPE PAR ÉTAPE

### Test 1 : Scan I2C
```
✓ Tous les esclaves trouvés
```

### Test 2 : Contrôle Delta
```cpp
moveDeltaTo(-75, -30, -120);    // Position A
delay(2000);
moveDeltaTo(0, 0, -120);        // Position B
delay(2000);
moveDeltaTo(75, -30, -120);     // Position C
```

### Test 3 : Contrôle Ventouse
```cpp
deltaGripperClose();
delay(1000);
deltaGripperOpen();
```

### Test 4 : Lecture Pixy
```cpp
getPixyPosition();
Serial.print("Pixel: ");
Serial.print(pixelX);
Serial.print(", ");
Serial.println(pixelY);
```

### Test 5 : Contrôle Tapis
```cpp
setTapisSpeed(100);    // 39%
setTapisStart();
delay(3000);
setTapisStop();
```

### Test 6 : Cycle Complet
```cpp
// Voir la fonction cycleTri() du Maître
```

---

## 10. NOTES IMPORTANTES

⚠️ **AVANT DE DÉMARRER LE SYSTÈME:**

1. ✓ Tous les câbles I2C bien branchés
2. ✓ Les 4 Arduinos chacun avec adresse unique (11, 12, 13, Maître)
3. ✓ Librairies Dynamixel2Arduino et Pixy2 installées
4. ✓ Code adapté aux paramètres de VOTRE robot
5. ✓ Calibrage Pixy2 effectué
6. ✓ Calibrage transformation pixel effectué
7. ✓ Moniteur série lancé pour voir les erreurs

**SÉCURITÉ:**
- Éloignez les mains du delta pendant les tests
- Le moteur tapis peut démarrer brutalement
- Testez chaque module individuellement d'abord

