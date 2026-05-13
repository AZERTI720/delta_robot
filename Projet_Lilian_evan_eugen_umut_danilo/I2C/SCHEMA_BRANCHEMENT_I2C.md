# Schéma de Branchement I2C - Détail Complet

## BRANCHEMENT I2C BUS

```
╔════════════════════════════════════════════════════════════════════════╗
║                        BUS I2C PARTAGÉ                                 ║
║                   (Connecté à tous les Arduinos)                        ║
║                                                                         ║
║   Signal SDA   ───────────────────────────────────────────────────    ║
║   Signal SCL   ───────────────────────────────────────────────────    ║
║   Signal GND   ═══════════════════════════════════════════════════    ║
║                                                                         ║
╚════════════════════════════════════════════════════════════════════════╝

         │                    │                    │                   │
         │                    │                    │                   │
    ┌────▼────┐          ┌────▼────┐          ┌────▼────┐         ┌────▼────┐
    │ Arduino │          │ Arduino │          │ Arduino │         │ Arduino │
    │  MAÎTRE │          │  DELTA  │          │  PIXY   │         │  TAPIS  │
    │   #1    │          │  #11    │          │  #12    │         │  #13    │
    │         │          │         │          │         │         │         │
    │A4 (SDA) │──────────│A4 (SDA) │──────────│A4 (SDA) │─────────│A4 (SDA) │
    │A5 (SCL) │──────────│A5 (SCL) │──────────│A5 (SCL) │─────────│A5 (SCL) │
    │GND      │──────────│GND      │──────────│GND      │─────────│GND      │
    │         │          │         │          │         │         │         │
    └─────────┘          └────┬────┘          └────┬────┘         └────┬────┘
                              │                    │                   │
                        Hardware                Hardware             Hardware
                        Dynamixel              Pixy2                 Motor
                        Shield                                       Driver
```

---

## DÉTAIL PINS PAR ARDUINO

### 1️⃣ ARDUINO MAÎTRE - Communication

```
    ┌─────────────┐
    │  ARDUINO    │
    │    UNO      │  [Role: Orchestre le système]
    │  MAÎTRE     │
    │             │
    │ ┌─────────┐ │
    │ │ USB     │ │ ← Connecté à l'ordinateur
    │ │ Serial  │ │   (Moniteur série 9600 baud)
    │ └─────────┘ │
    │             │
    │  [I2C]      │
    │  A4 (SDA)───┼─→ BUS I2C
    │  A5 (SCL)───┼─→ BUS I2C
    │  GND ───────┼─→ GND Commun
    │             │
    │ (Pas d'autres pins utilisés) │
    └─────────────┘

Connexion rapide:
- Charger: MAITRE_Communication.ino
- Accès: Moniteur série pour debug
- I2C: Standard (A4, A5, GND)
```

---

### 2️⃣ ARDUINO DELTA - Dynamixel #11

```
    ┌──────────────────┐
    │    ARDUINO       │
    │     UNO          │  [Role: Pilote le delta robot]
    │   [DELTA #11]    │
    │                  │
    │                  │
    │    ┌──────────┐  │
    │    │ Pins SPI │  │
    │    │  TX1/RX1 │  │
    │    └──┬───┬───┘  │
    │       │   └─────→ Shield Dynamixel
    │       │           (Communications série)
    │       └─────→ Shield Dynamixel
    │               (Communications série)
    │                  │
    │  Pin 2 ──────────┼─→ DIR (Shield Dynamixel)
    │                  │   [Signal de direction]
    │  Pin 4 ──────────┼─→ Relay Ventouse
    │                  │   [HIGH = Ouverte]
    │                  │   [LOW = Fermée]
    │                  │
    │  [I2C]           │
    │  A4 (SDA)────────┼─→ BUS I2C
    │  A5 (SCL)────────┼─→ BUS I2C
    │  GND ────────────┼─→ GND Commun
    │                  │
    └──────────────────┘
            ▲
            │
            │
    ┌───────┴─────────┐
    │ Shield Dynamixel│
    │   Moteurs 1,2,3│
    │   (les Ax-12A)  │
    └─────────────────┘

Connexion rapide:
- Charger: ESCLAVE_11_Delta.ino
- I2C: A4, A5, GND
- Dynamixel: TX1/RX1 + Pin 2
- Ventouse: Pin 4 + GND + +5V

⚠️ À calibrer dans le code:
- const float L = 133;   // Votre longueur bras
- const float l = 47;    // Votre longueur bielles
- const float d = 95;    // Distance moteurs
```

---

### 3️⃣ ARDUINO PIXY - Caméra Pixy2 #12

```
    ┌──────────────────┐
    │    ARDUINO       │
    │     UNO          │  [Role: Détecte les cubes]
    │   [PIXY #12]     │
    │                  │
    │                  │
    │  Pin 8 ──────────┼─→ RX SoftwareSerial
    │  Pin 9 ──────────┼─→ TX SoftwareSerial
    │                  │
    │    ┌──────────┐  │
    │    │ COM Pixy │  │ ← Vers Caméra Pixy2
    │    │  (8,9)   │  │   (Port USB Pixy)
    │    └──────────┘  │
    │                  │
    │  [I2C]           │
    │  A4 (SDA)────────┼─→ BUS I2C
    │  A5 (SCL)────────┼─→ BUS I2C
    │  GND ────────────┼─→ GND Commun
    │                  │
    └──────────────────┘

Connexion rapide:
- Charger: ESCLAVE_12_Pixy.ino
- I2C: A4, A5, GND
- SoftwareSerial: Pin 8 (RX), Pin 9 (TX)
- Pixy: Connecté en mini-USB (alimentation + données)

⚠️ À faire:
- Installer librairie Pixy2
- Calibrer signatures couleur avec Pixy MonitorAndTune
```

---

### 4️⃣ ARDUINO TAPIS - Moteur Tapis #13

```
    ┌──────────────────┐
    │    ARDUINO       │
    │     UNO          │  [Role: Contrôle moteur tapis]
    │  [TAPIS #13]     │
    │                  │
    │  Pin 5 ──────────┼─→ PWM (Vitesse)
    │  Pin 6 ──────────┼─→ DIR (Direction)
    │  Pin 7 ──────────┼─→ EN  (Enable)
    │                  │
    │    ┌──────────┐  │
    │    │ Moteur  │  │
    │    │ Driver  │  │
    │    │ (L298N) │  │
    │    └──┬───┬──┘  │
    │       │   │     │
    │  ┌────┘   └─→ Moteur Tapis
    │  │            (DC moteur)
    │  └──────────→ Alimentation
    │              moteur
    │                  │
    │  [I2C]           │
    │  A4 (SDA)────────┼─→ BUS I2C
    │  A5 (SCL)────────┼─→ BUS I2C
    │  GND ────────────┼─→ GND Commun
    │                  │
    └──────────────────┘

Connexion rapide:
- Charger: ESCLAVE_13_Tapis.ino
- I2C: A4, A5, GND
- Moteur: Pins 5 (PWM), 6 (DIR), 7 (EN)
- Alimentation: Séparée du moteur tapis

⚠️ À adapter selon votre driver moteur:
- Si L298N: Pin5=PWM, Pin6=DIR, Pin7=EN
- Si autre: adapter MOTOR_PWM, MOTOR_DIR, MOTOR_EN
```

---

## TABLEAU RÉSUMÉ DES CONNECTIONS

| Arduino | Pin | Fonction | Connecté à |
|---------|-----|----------|-----------|
| **MAÎTRE** | A4 | SDA I2C | BUS I2C |
| | A5 | SCL I2C | BUS I2C |
| | GND | Ground | GND Commun |
| **DELTA** | A4 | SDA I2C | BUS I2C |
| | A5 | SCL I2C | BUS I2C |
| | GND | Ground | GND Commun |
| | 2 | DIR | Shield Dynamixel |
| | 4 | RELAY | Ventouse |
| | TX1 | UART TX | Shield Dynamixel |
| | RX1 | UART RX | Shield Dynamixel |
| **PIXY** | A4 | SDA I2C | BUS I2C |
| | A5 | SCL I2C | BUS I2C |
| | GND | Ground | GND Commun |
| | 8 | RX Soft | Pixy2 TX |
| | 9 | TX Soft | Pixy2 RX |
| **TAPIS** | A4 | SDA I2C | BUS I2C |
| | A5 | SCL I2C | BUS I2C |
| | GND | Ground | GND Commun |
| | 5 | PWM | Driver PWM IN |
| | 6 | DIR | Driver DIR IN |
| | 7 | EN | Driver EN IN |

---

## SCHÉMA I2C PHYSIQUE (Vue de haut)

```
            [USB Arduino 1 - Maître]  [USB Arduino 2 - Delta]
                    │                           │
                    │                           │
        A4 ─────────┼────────────────────────── A4
        A5 ─────────┼────────────────────────── A5
        GND─────────┼────────────────────────── GND
                    │        ▲                   │
                    │        │                   │
                    │     [Câble I2C]           │
                    │        │                   │
        ┌───────────┼────────┼───────────────────┼──────────────┐
        │           │        │                   │              │
        │      A4 ──────────►SDA                │          A4 ──┼── SDA
        │      A5 ──────────►SCL                │          A5 ──┼── SCL
        │      GND─────────►GND                │          GND──┼── GND
        │           │        │                   │              │
        │    [Arduino 3]     │                   │    [Arduino 4]│
        │    (Pixy #12)      │                   │    (Tapis #13)│
        │           │        │                   │              │
        └───────────┼────────┼───────────────────┼──────────────┘
                    │        │                   │
            A4/A5/GND─────SDA/SCL/GND─────A4/A5/GND

⚠️ IMPORTANT:
- SDA et SCL DOIVENT être en parallèle
- GND DOIT être commun à tous
- Utiliser câble torsadé pour longueurs > 50cm
```

---

## LISTE DE VÉRIFICATION - AVANT DE DÉMARRER

```
☐ Hardware connections:
  ☐ Arduino 1 (Maître) - A4, A5, GND reliés
  ☐ Arduino 2 (Delta) - A4, A5, GND reliés
  ☐ Arduino 3 (Pixy) - A4, A5, GND reliés
  ☐ Arduino 4 (Tapis) - A4, A5, GND reliés
  ☐ GND COMMUN entre tous (confirmé!)
  ☐ Pas de court-circuit entre SDA et SCL
  
☐ Hardware supplémentaire:
  ☐ Shield Dynamixel branché + TX1/RX1
  ☐ Moteurs Dynamixel alimentés
  ☐ Pixy2 en USB (alimentation)
  ☐ Driver moteur tapis câblé (pins 5,6,7)
  ☐ Moteur tapis alimenté
  ☐ Ventouse testée (relais fonctionnel)
  
☐ Software:
  ☐ 4 codes téléchargés sur 4 Arduinos différents
  ☐ Librairies Dynamixel2Arduino + Pixy2 installées
  ☐ Moniteur série ouvert (9600 baud)
  ☐ Scan I2C remet 11, 12, 13 ✓
  
☐ Calibrage:
  ☐ L, l, d du delta adaptés
  ☐ Transformation pixel calibrée
  ☐ Pins moteur correct
  ☐ Pixy signatures calibrées

👉 Tous les ☐ cochés? → Prêt à démarrer!
```

---

## DÉPANNAGE BRANCHEMENT

| Symptôme | Vérification |
|----------|--------------|
| I2C scan échoue | A4/A5/GND reliés? GND commun? |
| Delta ne bouge pas | TX1/RX1 connecté au shield? Pin 2 OK? |
| Ventouse inerte | Pin 4 OK? Relais alimenté? |
| Pixy ne détecte rien | USB Pixy branché? Pins 8/9 OK? |
| Tapis ne démarre pas | Pins 5/6/7 OK? Driver alimenté? |
| Erreur 2 (NACK addr) | Arduino esclave mal adressé? |
| Erreur 3 (NACK data) | Esclave pas présent? |
| Erreur 4 (Other) | Conflit I2C? Autre appareil? |

