# 📐 Cinématique Inverse

La cinématique inverse est le cœur mathématique du robot delta. Elle permet de calculer les angles des bras (moteurs) en fonction de la position (X, Y, Z) souhaitée de l'effecteur.

## 🔬 Paramètres Géométriques
| Paramètre | Symbole | Valeur | Description |
| :--- | :--- | :--- | :--- |
| Longueur Bras Supérieur | L | 105 mm | Bras relié au moteur |
| Longueur Bras Inférieur | l | 190 mm | Tiges parallèles |
| Rayon Effecteur | Up | 25 mm | Distance centre-articulation effecteur |
| Rayon Base | Wb | 15 mm | Distance centre-moteur base |

## 🛠️ Logique de Calcul
Le calcul repose sur la fonction `calculeEtAffiche(double x, double y, double z)`.

### Étapes du calcul :
1. **Décalage (Offset) :** Calcul de la différence entre la base et l'effecteur (`a = Wb - Up`).
2. **Coordonnées Polaires :** Calcul des variables intermédiaires `E`, `F`, et `G` pour chaque bras.
3. **Résolution Quadratique :** Utilisation de la racine carrée pour trouver `t`, puis conversion en angle `teta` via `atan`.
4. **Conversion en Degrés :** Passage des radians aux degrés pour les servomoteurs Dynamixel.

> **Note :** Des corrections d'offset (ex: `+20` pour le moteur 2) sont appliquées pour compenser les imperfections mécaniques du montage.

## 🔗 Code Source
Le code est situé dans `Delta_robot_XYZ_avec_I2C.ino`.
