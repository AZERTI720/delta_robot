# Protocole I2C - Système de Tri Automatisé

## Architecture Système

```
MAÎTRE I2C (Communication)
├── Arduino UNO principal
├── SDA = A4, SCL = A5
└── Pilote les 3 esclaves via I2C

    ├─→ ESCLAVE #11 - Delta Robot (Dynamixel)
    │       Arduino UNO + Shield Dynamixel
    │       Pilotage des 3 moteurs Dynamixel
    │
    ├─→ ESCLAVE #12 - Caméra Pixy
    │       Arduino UNO + Caméra Pixy2
    │       Détecte positions des cubes (X, Y)
    │
    └─→ ESCLAVE #13 - Tapis Roulant
            Arduino UNO + Contrôleur moteur tapis
            Gère la vitesse d'entraînement
```

## Adresses I2C

| Composant | Adresse | Type |
|-----------|---------|------|
| Arduino Maître (Communication) | Maître (sans adresse) | Maître |
| Delta Robot (Dynamixel) | 0x0B (11) | Esclave |
| Caméra Pixy | 0x0C (12) | Esclave |
| Tapis Roulant | 0x0D (13) | Esclave |

## Protocoles de Communication

### 1. ESCLAVE #11 - DELTA ROBOT (Dynamixel)

#### Commandes Maître → Delta (beginTransmission)

| Commande | Format | Description |
|----------|--------|-------------|
| **MOVE_XYZ** | `0x01 + X(4B) + Y(4B) + Z(4B)` | Déplace le delta à (X,Y,Z) en float |
| **MOVE_LINEAR** | `0x02 + X1(4B) + Y1(4B) + Z1(4B) + X2(4B) + Y2(4B) + Z2(4B) + Steps(1B)` | Mouvement linéaire du point 1 au point 2 |
| **GRIPPER_OPEN** | `0x03` | Ouvre la ventouse/pince |
| **GRIPPER_CLOSE** | `0x04` | Ferme la ventouse/pince |
| **STOP** | `0x05` | Arrête le mouvement en cours |
| **STATUS** | `0x06` | Demande l'état du Delta |
| **SPEED_SET** | `0x07 + Speed(2B)` | Définie la vitesse des moteurs (0-1023) |

#### Réponses Delta → Maître (onRequest)

| Type | Format | Description |
|------|--------|-------------|
| **STATUS_OK** | `0xAA` | Prêt à recevoir une nouvelle commande |
| **MOVING** | `0xBB` | En mouvement |
| **ERROR** | `0xEE` | Erreur (manque de données, capteur défaut) |
| **POSITION** | `X(4B) + Y(4B) + Z(4B)` | Position actuelle en float |

**Exemple en Arduino :**
```cpp
// MAÎTRE envoie commande MOVE_XYZ
Wire.beginTransmission(11);
Wire.write(0x01);           // Commande MOVE_XYZ
Wire.write((byte*)&x, 4);   // X en float (4 bytes)
Wire.write((byte*)&y, 4);   // Y en float (4 bytes)
Wire.write((byte*)&z, 4);   // Z en float (4 bytes)
Wire.endTransmission();

// MAÎTRE demande le statut
Wire.requestFrom(11, 1);
byte status = Wire.read();  // 0xAA, 0xBB ou 0xEE
```

---

### 2. ESCLAVE #12 - CAMÉRA PIXY

#### Commandes Maître → Pixy

| Commande | Format | Description |
|----------|--------|-------------|
| **GET_POSITION** | `0x10` | Demande la position du cube détecté |
| **SET_TRACKING** | `0x11 + Enabled(1B)` | Active/désactive le suivi |
| **GET_BLOCK_COUNT** | `0x12` | Nombre de blocs détectés |
| **PIXY_STATUS** | `0x13` | État du capteur Pixy |

#### Réponses Pixy → Maître

| Type | Format | Description |
|------|--------|-------------|
| **POSITION** | `X(2B) + Y(2B)` | Coordonnées du cube (0-319 en X, 0-199 en Y) |
| **NO_OBJECT** | `0xFF` | Aucun cube détecté |
| **BLOCK_COUNT** | `Count(1B)` | Nombre de blocs trouvés |
| **TRACKING_ON/OFF** | `0x01 / 0x00` | État du suivi |

**Exemple :**
```cpp
// Demande position du cube
Wire.beginTransmission(12);
Wire.write(0x10);  // GET_POSITION
Wire.endTransmission();

Wire.requestFrom(12, 4);
uint16_t pixelX = (Wire.read() << 8) | Wire.read();
uint16_t pixelY = (Wire.read() << 8) | Wire.read();
```

---

### 3. ESCLAVE #13 - TAPIS ROULANT

#### Commandes Maître → Tapis

| Commande | Format | Description |
|----------|--------|-------------|
| **SET_SPEED** | `0x20 + Speed(2B)` | Vitesse 0-255 (0=arrêt, 255=max) |
| **START** | `0x21` | Démarre le tapis |
| **STOP** | `0x22` | Arrête le tapis |
| **DIRECTION** | `0x23 + Dir(1B)` | 0=Forward, 1=Reverse |
| **GET_STATUS** | `0x24` | Demande l'état du tapis |
| **EMERGENCY_STOP** | `0x25` | Arrêt d'urgence |

#### Réponses Tapis → Maître

| Type | Format | Description |
|------|--------|-------------|
| **ACK** | `0xCC` | Commande reçue |
| **RUNNING** | `0x01` | Tapis en marche |
| **STOPPED** | `0x00` | Tapis arrêté |
| **ERROR** | `0xEE` | Erreur |

**Exemple :**
```cpp
// Démarre le tapis à 50% vitesse
Wire.beginTransmission(13);
Wire.write(0x20);      // SET_SPEED
Wire.write(0x00);
Wire.write(127);       // 127 = 50% de 255
Wire.endTransmission();

// Vérifie l'état
Wire.requestFrom(13, 1);
byte status = Wire.read();  // 0xCC
```

---

## Types de Données I2C

### Float (4 bytes)
```cpp
union FloatBytes {
  float value;
  byte bytes[4];
};

FloatBytes fb;
fb.value = 123.45;
Wire.write(fb.bytes, 4);
```

### Double (8 bytes)
```cpp
union DoubleBytes {
  double value;
  byte bytes[8];
};
```

### Int16 (2 bytes)
```cpp
uint16_t value = 12345;
Wire.write((value >> 8) & 0xFF);  // MSB
Wire.write(value & 0xFF);         // LSB
```

---

## Flux de Communication Typique - Cycle de Tri

```
1. MAÎTRE → PIXY (0x10)          [Demande position cube]
   ↓
2. PIXY → MAÎTRE (X, Y, bloc#)   [Retourne position]
   ↓
3. MAÎTRE calcule pos(X,Y,Z) = transformation(pixelX, pixelY)
   ↓
4. MAÎTRE → TAPIS (0x21)         [Démarre tapis vers point capture]
   ↓
5. MAÎTRE → DELTA (0x01+X+Y+Z)   [Déplace delta vers position]
   ↓
6. DELTA → MAÎTRE (0xBB)         [En mouvement]
   ↓
7. Délai attente mouvement
   ↓
8. MAÎTRE → DELTA (0x04)         [Ferme ventouse]
   ↓
9. MAÎTRE → DELTA (0x02+...)     [Mouvement linéaire vers position finale]
   ↓
10. MAÎTRE → TAPIS (0x22)        [Arrête tapis]
    ↓
11. MAÎTRE → DELTA (0x03)        [Ouvre ventouse - lâche cube]
    ↓
12. Retour à l'étape 1
```

---

## Configurations Broches Arduino

### Tous les Arduinos (SDA/SCL)
- **SDA** → A4
- **SCL** → A5
- **GND** → GND commun (IMPORTANT!)

### Arduino Delta (+ Dynamixel Shield)
- **Serial** → Shield Dynamixel (TX1/RX1)
- **Pin 2** → DIR pin du shield
- **Pin 4** → Relay ventouse (HIGH=ouvert, LOW=fermé)
- **A4/A5** → I2C vers Maître

### Arduino Pixy (+ Caméra Pixy2)
- **SoftwareSerial** → Caméra Pixy (RX 8, TX 9)
- **A4/A5** → I2C vers Maître

### Arduino Tapis (+ Contrôleur moteur)
- **Pin 5** → PWM vitesse moteur
- **Pin 6** → Direction moteur
- **A4/A5** → I2C vers Maître

---

## Gestion des Erreurs I2C

```cpp
#define I2C_TIMEOUT 1000  // ms

bool sendCommand(uint8_t slave_addr, uint8_t* data, uint8_t len) {
  Wire.beginTransmission(slave_addr);
  Wire.write(data, len);
  uint8_t error = Wire.endTransmission();
  
  if (error != 0) {
    Serial.print("I2C Error: ");
    Serial.println(error);
    // 0=Success, 1=Buffer overflow, 2=NACK address, 3=NACK data, 4=Other
    return false;
  }
  return true;
}
```

---

## Notes d'Implémentation

1. **Tous les esclaves DOIVENT répondre dans ~50-100ms**
2. **Ajouter des checksums pour les commandes critiques (déplacement delta)**
3. **Implémenter des timeouts : si pas de réponse en 500ms, retry**
4. **Vitesse I2C recommandée : 100kHz (Arduino par défaut)**
5. **Câbler SDA/SCL avec pull-ups 4.7kΩ** (généralement intégrés sur shields)

