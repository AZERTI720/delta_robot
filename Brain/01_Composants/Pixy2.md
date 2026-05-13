# 👁️ Caméra Pixy2

Bien que le code principal `Delta_robot_XYZ_avec_I2C.ino` ne l'utilise pas directement, la présence de la bibliothèque `Pixy2` dans le projet indique une intention d'intégration pour la détection et le suivi d'objets.

## 📚 Bibliothèque Pixy2
La bibliothèque `Pixy2` est incluse dans le projet, avec plusieurs exemples d'utilisation (`ccc_hello_world`, `line_hello_world`, etc.). Cela suggère que la caméra Pixy2 est destinée à fournir des informations de position ou de détection d'objets au robot.

## 💡 Rôle Potentiel
La Pixy2 pourrait être utilisée pour :
- **Détection d'objets :** Identifier la présence et la position d'objets colorés.
- **Suivi de ligne :** Guider le robot le long d'une ligne.
- **Asservissement visuel :** Ajuster la position du robot en fonction de la cible détectée par la caméra.

## 🔗 Références
- La documentation et les exemples de la bibliothèque `Pixy2` sont disponibles dans le dossier `libraries/Pixy2/examples/` du projet.
