# 🔌 Communication I2C

Le projet utilise la communication I2C pour échanger des données entre deux cartes Arduino (maître et esclave).

## 📌 Rôle de l'Arduino Maître (ce code)
Cette carte agit en tant que **maître I2C**. Elle demande des données (positions X, Y, Z) à l'Arduino esclave.

## 📡 Détails Techniques
- **Adresse Esclave :** `SLAVE_ADDR = 11`
- **Bibliothèque :** `Wire.h`
- **Fonction de Lecture :** `readPositionFromSlave()`

### Processus de Lecture
1. L'Arduino maître envoie une requête (`Wire.requestFrom(SLAVE_ADDR, 24)`) pour 24 octets (correspondant à 3 valeurs `double` pour X, Y, Z).
2. Il vérifie la disponibilité des données (`Wire.available() == 24`).
3. Les octets sont lus et convertis en valeurs `double` à l'aide d'une `union DoubleBytes`.
4. En cas d'erreur ou de données incomplètes, une position par défaut `{0, 0, -120}` est retournée.

## ⚠️ Points Importants
- L'esclave doit être configuré pour répondre à l'adresse 11 et envoyer les 24 octets de données de position.
- La conversion `double` vers `byte` et inversement doit être gérée avec soin pour éviter la perte de précision.
