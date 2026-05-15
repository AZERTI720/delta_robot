# 🤖 Projet Delta Robot

Bienvenue sur le dépôt officiel du projet **Delta Robot**. Ce projet regroupe l'ensemble du code source, de la documentation technique et des ressources nécessaires à la fabrication et au pilotage d'un robot delta asservi.

## 📁 Structure du Projet

| Dossier | Description |
| :--- | :--- |
| [**`Firmware/Final`**](./Firmware/Final) | **Versions les plus complètes et testées du projet.** |
| [**`Firmware/Tests`**](./Firmware/Tests) | Scripts de test pour les moteurs, capteurs et ventouses. |
| [**`Firmware/Libraries`**](./Firmware/Libraries) | Bibliothèques Arduino nécessaires (Dynamixel, Pixy2, etc.). |
| [**`Docs/`**](./Docs) | Fiches techniques (PDF) et guides d'utilisation. |
| [**`Hardware/`**](./Hardware) | Photos du montage et schémas du robot. |
| [**`Brain/`**](./Brain) | **Cerveau Parallèle Obsidian** (Documentation auto-générée). |
| [**`Archive/`**](./Archive) | Anciennes versions et sauvegardes du projet. |

## 🚀 Contenu du dossier Final

Après analyse, les versions suivantes ont été identifiées comme les plus complètes :
- **`Robot_Pilotage/Robot_Pilotage_I2C_Complet.ino`** : Version finale intégrant la communication I2C, la cinématique inverse corrigée (offset +20 sur moteur 2) et la gestion de la ventouse.
- **`Pixy_Camera/finale.ino`** : Code de détection de couleurs optimisé pour la caméra Pixy2.
- **`Robot_Pilotage/Convoyeur_Final.ino`** : Pilotage du tapis roulant synchronisé avec le robot.

## 🧠 Le Cerveau Parallèle
Ce dépôt est équipé d'un système de documentation **Obsidian** qui s'auto-alimente. 
- Pour le consulter, ouvrez le dossier `Brain/` avec l'application Obsidian.
- Il contient les explications détaillées de la cinématique inverse et des composants.

---
*Projet réalisé par Lilian, Evan, Eugen, Umut et Danilo.*
