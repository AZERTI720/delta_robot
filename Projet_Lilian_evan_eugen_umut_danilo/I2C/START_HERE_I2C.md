# 🎯 DÉMARRAGE RAPIDE - 3 MINUTES CHRONO

## ✨ VOUS VENEZ DE RECEVOIR:

### 📄 8 Fichiers de Documentation
Tous les détails pour comprendre et installer le système

### 💻 5 Fichiers de Code Arduino
Code prêt à charger sur les 4 Arduinos

### 🧪 Tests Progressifs Intégrés
5 tests pour valider chaque module

---

## 🚀 EN 3 ÉTAPES:

### ÉTAPE 1️⃣ : LIRE (5 min)
```
Ouvrir et lire:
→ README_I2C.md
```

### ÉTAPE 2️⃣ : BRANCHER (30 min)
```
Consulter:
→ SCHEMA_BRANCHEMENT_I2C.md
```

### ÉTAPE 3️⃣ : CHARGER CODE (1h)
```
Suivre:
→ GUIDE_MISE_EN_PLACE_I2C.md
```

---

## 📁 FICHIERS CLÉS

| Fichier | Action | Temps |
|---------|--------|-------|
| **README_I2C.md** | Lire EN PREMIER | 5 min |
| **SCHEMA_BRANCHEMENT_I2C.md** | Brancher hardware | 10 min |
| **GUIDE_MISE_EN_PLACE_I2C.md** | Installation complète | 45 min |
| **TEST_MODULES.ino** | Valider système | 1h |

---

## 🎯 ARCHITECTURE

```
MAÎTRE #1 (Communication)
    ↓
    ├→ DELTA #11 (Dynamixel)
    ├→ PIXY #12 (Caméra)
    └→ TAPIS #13 (Moteur)
```

---

## 📊 EN RÉSUMÉ

```
✅ 13 fichiers créés
✅ ~3,700 lignes de code/doc
✅ Protocole I2C complet
✅ 4 Arduinos en réseau
✅ Cycle de tri automatisé
✅ Tests progressifs
✅ Documentation complète
✅ Guides de calibrage
✅ Dépannage inclus

→ PRÊT À DÉMARRER! 🚀
```

---

## 🔗 FICHIERS PRINCIPAUX

### 👈 Commencer ici:
- [README_I2C.md](README_I2C.md) ← LISEZ D'ABORD

### 📖 Puis lire:
- [SCHEMA_BRANCHEMENT_I2C.md](SCHEMA_BRANCHEMENT_I2C.md) (câblage)
- [GUIDE_MISE_EN_PLACE_I2C.md](GUIDE_MISE_EN_PLACE_I2C.md) (installation)

### 💾 Charger sur Arduino:
- [MAITRE_Communication.ino](Communication%20intercarte/dossier%20I2C/MAITRE_Communication.ino)
- [ESCLAVE_11_Delta.ino](Communication%20intercarte/dossier%20I2C/ESCLAVE_11_Delta.ino)
- [ESCLAVE_12_Pixy.ino](Communication%20intercarte/dossier%20I2C/ESCLAVE_12_Pixy.ino)
- [ESCLAVE_13_Tapis.ino](Communication%20intercarte/dossier%20I2C/ESCLAVE_13_Tapis.ino)
- [TEST_MODULES.ino](Communication%20intercarte/dossier%20I2C/TEST_MODULES.ino) (tests)

### 📚 Référence:
- [REFERENCE_RAPIDE_I2C.md](REFERENCE_RAPIDE_I2C.md) (codes/commandes)
- [PROTOCOLE_I2C_COMPLET.md](PROTOCOLE_I2C_COMPLET.md) (détails techniques)

---

## ⚠️ NE PAS OUBLIER:

1. **Calibrer le Delta** (L, l, d)
2. **Calibrer Pixy** (transformation pixel)
3. **Vérifier I2C** (A4, A5, GND commun)
4. **Tester progressivement** (test 1→5)

---

## ✅ CHECKLIST DE DÉMARRAGE

```
☐ Lire README_I2C.md
☐ Consulter SCHEMA_BRANCHEMENT_I2C.md
☐ Brancher I2C (A4, A5, GND)
☐ Installer librairies (Dynamixel2Arduino, Pixy2)
☐ Télécharger 4 codes sur 4 Arduinos
☐ Lancer TEST_MODULES.ino (test 1)
☐ Calibrer paramètres
☐ Tester chaque module
☐ Cycle complet!

✅ Prêt!
```

---

🎉 **BON DÉVELOPPEMENT!**

*Ouvrez README_I2C.md maintenant!*
