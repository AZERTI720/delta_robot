# ⚙️ Moteurs Dynamixel

Le robot utilise trois servomoteurs numériques **Dynamixel** (probablement de la série AX ou XL au vu de la bibliothèque `Dynamixel2Arduino`).

## 🔌 Configuration Matérielle
- **IDs :** 1, 2, et 3.
- **Protocole :** 1.0.
- **Baudrate :** 1,000,000 bps.
- **Interface :** Dynamixel Shield avec DIR PIN sur la broche 2.

## 🛠️ Paramètres de Fonctionnement
Les moteurs sont configurés en **Mode Position** (`OP_POSITION`).

| Paramètre | Valeur | Description |
| :--- | :--- | :--- |
| Vitesse (MOVING_SPEED) | 100 | Vitesse limitée pour la sécurité et la précision |
| Unité | Degrés | Utilisation de `UNIT_DEGREE` pour le pilotage |

## 🕹️ Pilotage
La commande `dxl.setGoalPosition(ID, angle, UNIT_DEGREE)` est utilisée pour déplacer chaque bras individuellement après le calcul de la cinématique inverse.
