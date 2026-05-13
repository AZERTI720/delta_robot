# RÉSUMÉ - Protocole I2C Système de Tri Automatisé

## 📋 Ce qui a été créé

Vous avez maintenant un **protocole I2C complet et fonctionnel** pour votre système de tri de cubes avec :

### 1. 📄 Fichiers de Documentation

| Fichier | Contenu |
|---------|---------|
| **PROTOCOLE_I2C_COMPLET.md** | Documentation complète du protocole avec tous les détails techniques |
| **GUIDE_MISE_EN_PLACE_I2C.md** | Guide étape par étape pour mettre en place et calibrer le système |
| **REFERENCE_RAPIDE_I2C.md** | Fiche de référence pour les codes et configurations |

### 2. 💾 Fichiers de Code Arduino

#### Maître (Communication)
- **MAITRE_Communication.ino** → À charger sur Arduino UNO #1
  - Orchestre tout le système
  - Communique avec les 3 esclaves
  - Gère le cycle de tri complet

#### Esclaves
- **ESCLAVE_11_Delta.ino** → À charger sur Arduino UNO #2
  - Contrôle les moteurs Dynamixel (cinématique inverse)
  - Gère la ventouse/pince
  - Adresse I2C: 11 (0x0B)

- **ESCLAVE_12_Pixy.ino** → À charger sur Arduino UNO #3
  - Détecte les cubes via Pixy2
  - Retourne coordonnées pixel (X, Y)
  - Adresse I2C: 12 (0x0C)

- **ESCLAVE_13_Tapis.ino** → À charger sur Arduino UNO #4
  - Contrôle le moteur du tapis roulant
  - Gère vitesse et direction
  - Adresse I2C: 13 (0x0D)

### 3. 🧪 Code de Test
- **TEST_MODULES.ino** → Contient 5 tests progressifs (décommentez chacun)
  1. Scan I2C (trouver les 3 esclaves)
  2. Test Delta
  3. Test Pixy
  4. Test Tapis
  5. Cycle complet

---

## 🚀 DÉMARRAGE RAPIDE

### Étape 1: Préparation Hardware
```
1. Brancher les 4 Arduinos sur l'USB de l'ordinateur
2. Connecter I2C: A4 (SDA) et A5 (SCL) en parallèle
3. Connecter GND commun entre tous les Arduinos
```

### Étape 2: Installation Librairies
```
Arduino IDE → Manage Libraries → Chercher et installer:
- Dynamixel2Arduino
- Pixy2
```

### Étape 3: Télécharger les Codes
```
1. MAITRE_Communication.ino         → Arduino #1 (Communication)
2. ESCLAVE_11_Delta.ino             → Arduino #2 (Delta)
3. ESCLAVE_12_Pixy.ino              → Arduino #3 (Pixy)
4. ESCLAVE_13_Tapis.ino             → Arduino #4 (Tapis)
```

### Étape 4: Test I2C
```
1. Charger TEST_MODULES.ino (test 1 - Scan)
2. Ouvrir moniteur série (9600 baud)
3. Vérifier que les 3 esclaves (11, 12, 13) sont trouvés
```

### Étape 5: Calibrage (CRITIQUE!)
```
1. Adapter les paramètres du Delta:
   - const float L = 133;   // VOTRE longueur de bras
   - const float l = 47;    // VOTRE longueur de bielles
   
2. Calibrer transformation Pixy → Robot:
   - Mesurer 4 points de calibrage
   - Adapter la fonction pixelToRobotCoordinates()
```

### Étape 6: Tests Progressifs
```
Test 2 → Delta se déplace et ouvre/ferme ventouse
Test 3 → Pixy détecte un cube et retourne sa position
Test 4 → Tapis démarre et s'arrête
Test 5 → Cycle complet de tri (placer un cube devant Pixy)
```

---

## 📊 ARCHITECTURE I2C

```
    MAÎTRE (Communication)
          │
    ┌─────┼─────┬──────────┐
    │     │     │          │
   11    12    13          
 (Delta) (Pixy) (Tapis)
```

**Toutes les communications passent par le BUS I2C:**
- Arduino A4 (SDA) et A5 (SCL)
- GND commun
- Vitesse 100 kHz (par défaut)

---

## 🎯 CYCLE TRI COMPLET

```
┌─────────────────────────────┐
│ 1. Pixy détecte cube        │
│    pixelX, pixelY           │
└──────────────┬──────────────┘
               │
┌──────────────▼──────────────┐
│ 2. Maître calcule position  │
│    robot (X, Y, Z)          │
└──────────────┬──────────────┘
               │
┌──────────────▼──────────────┐
│ 3. Tapis démarre            │
└──────────────┬──────────────┘
               │
┌──────────────▼──────────────┐
│ 4. Delta va au cube         │
└──────────────┬──────────────┘
               │
┌──────────────▼──────────────┐
│ 5. Ventouse se ferme        │
└──────────────┬──────────────┘
               │
┌──────────────▼──────────────┐
│ 6. Delta va à zone de dépôt │
└──────────────┬──────────────┘
               │
┌──────────────▼──────────────┐
│ 7. Ventouse s'ouvre         │
└──────────────┬──────────────┘
               │
┌──────────────▼──────────────┐
│ 8. Tapis s'arrête           │
└──────────────┬──────────────┘
               │
┌──────────────▼──────────────┐
│ 9. Delta retour accueil     │
│ 10. Boucle → étape 1        │
└─────────────────────────────┘
```

---

## 🔧 POINTS CRITIQUES À VÉRIFIER

### 1. **Calibrage Cinématique Delta** ⚠️
Dans `ESCLAVE_11_Delta.ino` (ligne ~30):
```cpp
const float L = 133;   // À mesurer sur VOTRE robot!
const float l = 47;    // À mesurer sur VOTRE robot!
const float d = 95;    // À mesurer sur VOTRE robot!
```

### 2. **Calibrage Pixy → Robot** ⚠️
Dans `MAITRE_Communication.ino` (ligne ~140):
```cpp
rx = (px - 160) * 0.4;   // À calibrer!
ry = (py - 100) * 0.3;   // À calibrer!
```

### 3. **Pins Moteur Tapis**
Dans `ESCLAVE_13_Tapis.ino` (ligne ~10):
```cpp
#define MOTOR_PWM   5   // Adapter si besoin
#define MOTOR_DIR   6   // Adapter si besoin
#define MOTOR_EN    7   // Adapter si besoin
```

### 4. **I2C Bus**
- Tous les SDA sur A4
- Tous les SCL sur A5
- GND commun OBLIGATOIRE

---

## ✅ CHECKLIST DE DÉMARRAGE

```
Avant de lancer le système:

[ ] Hardware
    [ ] 4 Arduinos connectés en USB
    [ ] I2C câblé (SDA, SCL, GND)
    [ ] Dynamixel Shield branché
    [ ] Pixy2 allumée
    [ ] Moteur tapis prêt

[ ] Software
    [ ] Librairies Dynamixel2Arduino et Pixy2 installées
    [ ] 4 codes téléchargés sur les 4 Arduinos
    [ ] Scan I2C valide (11, 12, 13 trouvés)

[ ] Calibrage
    [ ] Paramètres L, l, d du Delta adaptés
    [ ] Transformation Pixy calibrée
    [ ] Pixy2 signatures de couleur définies
    [ ] Moteur tapis testé

[ ] Tests
    [ ] Test 1: Scan I2C ✓
    [ ] Test 2: Delta bouge ✓
    [ ] Test 3: Pixy détecte ✓
    [ ] Test 4: Tapis marche ✓
    [ ] Test 5: Cycle complet ✓

[ ] Prêt pour production!
```

---

## 📚 FICHIERS DE RÉFÉRENCE

### Pour comprendre le protocole
→ Lire: **PROTOCOLE_I2C_COMPLET.md**

### Pour mettre en place le système
→ Lire: **GUIDE_MISE_EN_PLACE_I2C.md**

### Pour des codes rapides
→ Lire: **REFERENCE_RAPIDE_I2C.md**

### Pour tester progressivement
→ Utiliser: **TEST_MODULES.ino**

---

## 🐛 DÉPANNAGE RAPIDE

| Symptôme | Cause | Solution |
|----------|-------|----------|
| Aucun esclave trouvé | I2C mal câblé | Vérifier A4, A5, GND |
| Delta ne bouge pas | Cinématique mal calibrée | Adapter L, l, d |
| Pixy ne détecte rien | Signature couleur non définie | Calibrer avec MonitorAndTune |
| Tapis ne démarre pas | Pin PWM mal défini | Adapter MOTOR_PWM, DIR, EN |
| Erreur NACK I2C | Mauvaise adresse esclave | Vérifier 11, 12, 13 |

---

## 📝 NOTES IMPORTANTES

1. **Les valeurs de calibrage sont ESSENTIELLES**
   - Sans calibrage correct, rien ne marche
   - Prendre le temps pour mesurer précisément

2. **Tester module par module**
   - Ne pas lancer le cycle complet d'emblée
   - Utiliser TEST_MODULES.ino progressivement

3. **I2C est synchrone**
   - Le Maître attend les réponses des esclaves
   - Timeouts: si pas de réponse en 500ms, retry

4. **Sécurité**
   - Éloigner les mains pendant les tests delta
   - Vérifier que le tapis est sécurisé avant de démarrer
   - Ventouse peut tourner brusquement

---

## 🎓 POUR ALLER PLUS LOIN

- Ajouter un **checksum** sur les commandes critiques
- Implémenter un **watchdog** pour détecter pannes
- Ajouter des **LEDs de statut** sur chaque Arduino
- Logger les **données de chaque cycle** en EEPROM
- Implémenter un **mode manuel** via boutons

---

**Vous êtes maintenant prêt à utiliser le protocole I2C!** 🎉

Si des problèmes surviennent, consultez les fichiers de documentation détaillés créés dans ce projet.

