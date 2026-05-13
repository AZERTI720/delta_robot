# INDEX COMPLET - Protocole I2C Système de Tri

## 📁 Structure des Fichiers Créés

```
Projet_Lilian_evan_eugen_umut_danilo/
│
├─ 📘 DOCUMENTATION
│  ├─ README_I2C.md .......................... Résumé complet (LIRE D'ABORD!)
│  ├─ PROTOCOLE_I2C_COMPLET.md .............. Documentation technique détaillée
│  ├─ GUIDE_MISE_EN_PLACE_I2C.md ............ Guide étape par étape (IMPORTANT!)
│  ├─ REFERENCE_RAPIDE_I2C.md .............. Fiche de référence rapide
│  ├─ SCHEMA_BRANCHEMENT_I2C.md ............ Schémas électriques et branchement
│  └─ INDEX_I2C.md (ce fichier) ............ Index et guide de navigation
│
├─ 💻 CODE ARDUINO
│  ├─ Communication intercarte/dossier I2C/
│  │  ├─ MAITRE_Communication.ino .......... Arduino Maître (orchestration)
│  │  ├─ ESCLAVE_11_Delta.ino ............. Arduino Delta (Dynamixel + ventouse)
│  │  ├─ ESCLAVE_12_Pixy.ino .............. Arduino Pixy (détection cubes)
│  │  ├─ ESCLAVE_13_Tapis.ino ............. Arduino Tapis (moteur tapis)
│  │  └─ TEST_MODULES.ino ................. Tests progressifs (5 étapes)
│
└─ 📊 AUTRES
   ├─ [Code existant Delta] ............... Versions antérieures (référence)
   └─ [Librairies Arduino] ................ Dynamixel2Arduino, Pixy2
```

---

## 🎯 PAR OÙ COMMENCER?

### Pour les Impatients (5 min)
1. Lire [README_I2C.md](README_I2C.md) - Vue d'ensemble
2. Regarder [SCHEMA_BRANCHEMENT_I2C.md](SCHEMA_BRANCHEMENT_I2C.md#branchement-i2c-bus) - Câblage
3. Télécharger les 4 fichiers .ino dans Arduino IDE

### Pour une Installation Complète (1-2h)
1. **Lire** [PROTOCOLE_I2C_COMPLET.md](PROTOCOLE_I2C_COMPLET.md) - Comprendre le système
2. **Suivre** [GUIDE_MISE_EN_PLACE_I2C.md](GUIDE_MISE_EN_PLACE_I2C.md) - Installation matérielle
3. **Télécharger** les codes sur les 4 Arduinos
4. **Tester** avec [TEST_MODULES.ino](Communication%20intercarte/dossier%20I2C/TEST_MODULES.ino)

### Pour du Dépannage
→ Consulter [GUIDE_MISE_EN_PLACE_I2C.md#7-diagnostic--dépannage](GUIDE_MISE_EN_PLACE_I2C.md#7-diagnostic--dépannage)

### Besoin d'Une Référence Rapide?
→ [REFERENCE_RAPIDE_I2C.md](REFERENCE_RAPIDE_I2C.md)

---

## 📖 GUIDE DE LECTURE

### 1. COMPRENDRE LE SYSTÈME
**Fichier:** [PROTOCOLE_I2C_COMPLET.md](PROTOCOLE_I2C_COMPLET.md)

Contient:
- ✓ Architecture complète
- ✓ Toutes les commandes I2C
- ✓ Protocoles pour chaque esclave
- ✓ Types de données
- ✓ Flux de communication

**À lire si:** Vous voulez comprendre comment ça marche

---

### 2. INSTALLER LE SYSTÈME
**Fichier:** [GUIDE_MISE_EN_PLACE_I2C.md](GUIDE_MISE_EN_PLACE_I2C.md)

Contient:
- ✓ Préparation hardware détaillée
- ✓ Installation logicielle (librairies)
- ✓ Téléchargement des sketches
- ✓ Tests de connexion
- ✓ Procédures de calibrage
- ✓ Troubleshooting

**À lire si:** Vous installez le système pour la première fois

---

### 3. RÉFÉRENCE RAPIDE
**Fichier:** [REFERENCE_RAPIDE_I2C.md](REFERENCE_RAPIDE_I2C.md)

Contient:
- ✓ Codes de commandes principaux
- ✓ Adresses I2C
- ✓ Exemples de code court
- ✓ Conversions données (float, int)
- ✓ Checklist
- ✓ Diagramme architecture

**À lire si:** Vous besoin d'une info rapide

---

### 4. BRANCHEMENT
**Fichier:** [SCHEMA_BRANCHEMENT_I2C.md](SCHEMA_BRANCHEMENT_I2C.md)

Contient:
- ✓ Schémas I2C physiques
- ✓ Détail des connections par Arduino
- ✓ Tableau de correspondance
- ✓ Checklist branchement
- ✓ Dépannage branchement

**À lire si:** Vous ne savez pas comment câbler

---

### 5. CODE DE TEST
**Fichier:** [Communication intercarte/dossier I2C/TEST_MODULES.ino](Communication%20intercarte/dossier%20I2C/TEST_MODULES.ino)

Contient 5 tests progressifs:
1. ✓ Scan I2C (trouver les esclaves)
2. ✓ Test Delta (mouvement + ventouse)
3. ✓ Test Pixy (détection)
4. ✓ Test Tapis (moteur)
5. ✓ Cycle complet

**À utiliser:** Pour valider chaque module avant le cycle complet

---

## 🔗 NAVIGATION PAR SUJET

### I2C et Communication
- [Architecture I2C](PROTOCOLE_I2C_COMPLET.md#architecture-système)
- [Commandes et protocoles](PROTOCOLE_I2C_COMPLET.md#protocoles-de-communication)
- [Erreurs I2C](PROTOCOLE_I2C_COMPLET.md#gestion-des-erreurs-i2c)

### Hardware et Branchement
- [Configuration broches Arduino](GUIDE_MISE_EN_PLACE_I2C.md#4-configurations-broches-arduino)
- [Schémas détaillés](SCHEMA_BRANCHEMENT_I2C.md)
- [Tableau connections](SCHEMA_BRANCHEMENT_I2C.md#tableau-résumé-des-connections)

### Software et Librairies
- [Installation librairies](GUIDE_MISE_EN_PLACE_I2C.md#2-installation-logicielle)
- [Téléchargement des sketches](GUIDE_MISE_EN_PLACE_I2C.md#3-téléchargement-des-sketches)
- [Code de test](GUIDE_MISE_EN_PLACE_I2C.md#4-test-de-connexion-i2c)

### Calibrage
- [Calibrage Delta](GUIDE_MISE_EN_PLACE_I2C.md#51-calibrage-cinématique-delta)
- [Calibrage Pixy](GUIDE_MISE_EN_PLACE_I2C.md#52-calibrage-pixy2)
- [Calibrage transformation pixel](GUIDE_MISE_EN_PLACE_I2C.md#53-calibrage-transformation-pixel--robot)

### Tests et Validation
- [Procédure de démarrage](GUIDE_MISE_EN_PLACE_I2C.md#61-procédure-de-démarrage)
- [Tests étape par étape](GUIDE_MISE_EN_PLACE_I2C.md#9-protocole-de-test-étape-par-étape)
- [Code de test](Communication%20intercarte/dossier%20I2C/TEST_MODULES.ino)

### Dépannage
- [Problèmes courants](GUIDE_MISE_EN_PLACE_I2C.md#problème--aucun-esclave-détecté)
- [Codes d'erreur I2C](REFERENCE_RAPIDE_I2C.md#codes-derreur-i2c)
- [Checklist diagnostic](SCHEMA_BRANCHEMENT_I2C.md#dépannage-branchement)

---

## 📋 ADRESSES I2C - MÉMORANDUM

```
Maître (Communication)   ← Pas d'adresse (Maître)
  ↓
Esclave #11 (Delta)      ← 0x0B
Esclave #12 (Pixy)       ← 0x0C
Esclave #13 (Tapis)      ← 0x0D

Bus I2C: A4 (SDA), A5 (SCL), GND commun
```

---

## 🚀 CHECKLIST RAPIDE

```
Phase 1: Hardware
[ ] Tous les Arduinos connectés en USB
[ ] I2C câblé (A4, A5, GND x4 Arduinos)
[ ] Shields/modules connectés (Dynamixel, Pixy, Driver moteur)

Phase 2: Software
[ ] Librairies installées (Dynamixel2Arduino, Pixy2)
[ ] 4 codes téléchargés sur 4 Arduinos
[ ] Scan I2C valide (11, 12, 13 trouvés)

Phase 3: Calibrage
[ ] Paramètres Delta adaptés
[ ] Transformation Pixy calibrée
[ ] Pins moteur correctes

Phase 4: Test
[ ] Test 1: Scan ✓
[ ] Test 2: Delta ✓
[ ] Test 3: Pixy ✓
[ ] Test 4: Tapis ✓
[ ] Test 5: Cycle complet ✓

→ Prêt!
```

---

## 🔑 POINTS CRITIQUES

### À Adapter Dans Votre Code

**ESCLAVE_11_Delta.ino** (ligne ~30)
```cpp
const float L = 133;   // À mesurer!
const float l = 47;    // À mesurer!
const float d = 95;    // À mesurer!
```

**MAITRE_Communication.ino** (ligne ~140)
```cpp
rx = (px - 160) * 0.4;   // À calibrer!
ry = (py - 100) * 0.3;   // À calibrer!
```

**ESCLAVE_13_Tapis.ino** (ligne ~10)
```cpp
#define MOTOR_PWM   5   // Adapter si nécessaire
#define MOTOR_DIR   6   // Adapter si nécessaire
#define MOTOR_EN    7   // Adapter si nécessaire
```

---

## 📞 BESOIN D'AIDE?

| Problème | Lire |
|----------|------|
| Aucun esclave trouvé | [Dépannage](GUIDE_MISE_EN_PLACE_I2C.md#problème--aucun-esclave-détecté) |
| Delta ne bouge pas | [Calibrage Delta](GUIDE_MISE_EN_PLACE_I2C.md#51-calibrage-cinématique-delta) |
| Pixy ne détecte rien | [Calibrage Pixy](GUIDE_MISE_EN_PLACE_I2C.md#52-calibrage-pixy2) |
| Tapis ne démarre pas | [Configuration pins](GUIDE_MISE_EN_PLACE_I2C.md#8-configuration-moteur-tapis-exemple) |
| I2C lent/bugué | [Gestion erreurs](PROTOCOLE_I2C_COMPLET.md#gestion-des-erreurs-i2c) |
| Cycle complet échoue | [Tests progressifs](GUIDE_MISE_EN_PLACE_I2C.md#9-protocole-de-test-étape-par-étape) |

---

## 🎓 RESSOURCES EXTERNES

- **Dynamixel2Arduino**: https://github.com/ROBOTIS-GIT/DynamixelSDK
- **Pixy2**: https://github.com/charmedlabs/pixy2
- **Arduino Wire Library**: https://www.arduino.cc/en/Reference/Wire
- **I2C Spécification**: https://en.wikipedia.org/wiki/I%C2%B2C

---

## 📝 RÉSUMÉ FICHIERS CRÉES

| Fichier | Type | Utilité |
|---------|------|---------|
| README_I2C.md | 📘 Doc | Vue d'ensemble complète |
| PROTOCOLE_I2C_COMPLET.md | 📘 Doc | Spécifications techniques |
| GUIDE_MISE_EN_PLACE_I2C.md | 📘 Doc | Installation détaillée |
| REFERENCE_RAPIDE_I2C.md | 📘 Doc | Fiche mémento |
| SCHEMA_BRANCHEMENT_I2C.md | 📘 Doc | Schémas électriques |
| MAITRE_Communication.ino | 💻 Code | Orchestration système |
| ESCLAVE_11_Delta.ino | 💻 Code | Delta + ventouse |
| ESCLAVE_12_Pixy.ino | 💻 Code | Détection cubes |
| ESCLAVE_13_Tapis.ino | 💻 Code | Moteur tapis |
| TEST_MODULES.ino | 🧪 Test | Validation progressive |

**TOTAL: 10 fichiers pour un système complet et documenté!**

---

## ✅ PRÊT À DÉMARRER?

1. ✓ Vérifier le branchement ([SCHEMA_BRANCHEMENT_I2C.md](SCHEMA_BRANCHEMENT_I2C.md))
2. ✓ Télécharger les 4 sketches
3. ✓ Lancer le scan I2C
4. ✓ Calibrer les paramètres critiques
5. ✓ Exécuter TEST_MODULES.ino progressivement
6. ✓ Lancer le cycle complet!

**Bon développement! 🎉**

---

*Dernière mise à jour: 28 avril 2026*
*Protocole I2C - Système de Tri Automatisé*
*Pour: Lilian, Evan, Eugen, Umut, Danilo*
