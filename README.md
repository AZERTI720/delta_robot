# 🤖 Projet Delta Robot

Bienvenue sur le dépôt officiel du projet **Delta Robot**. Ce projet regroupe l'ensemble du code source, de la documentation technique et des ressources nécessaires à la fabrication et au pilotage d'un robot delta asservi.

## 📁 Structure du Projet

| Dossier | Description |
| :--- | :--- |
| [**`Firmware/Final`**](./Firmware/Final) | Code source final (avec et sans I2C) pour le robot. |
| [**`Firmware/Tests`**](./Firmware/Tests) | Scripts de test pour les moteurs, capteurs et ventouses. |
| [**`Firmware/Libraries`**](./Firmware/Libraries) | Bibliothèques Arduino nécessaires (Dynamixel, Pixy2, etc.). |
| [**`Docs/`**](./Docs) | Fiches techniques (PDF) et guides d'utilisation. |
| [**`Hardware/`**](./Hardware) | Photos du montage et schémas du robot. |
| [**`Brain/`**](./Brain) | **Cerveau Parallèle Obsidian** (Documentation auto-générée). |
| [**`Archive/`**](./Archive) | Anciennes versions et sauvegardes du projet. |

## 🧠 Le Cerveau Parallèle
Ce dépôt est équipé d'un système de documentation **Obsidian** qui s'auto-alimente. 
- Pour le consulter, ouvrez le dossier `Brain/` avec l'application Obsidian.
- Il contient les explications détaillées de la cinématique inverse et des composants.

## 🚀 Installation Rapide
1. Clonez le dépôt : `git clone https://github.com/AZERTI720/delta_robot.git`
2. Installez les bibliothèques du dossier `Firmware/Libraries` dans votre dossier Arduino.
3. Téléversez le code de `Firmware/Final/Delta_robot_XYZ_avec_I2C/` sur votre carte maître.

---
*Projet réalisé par Lilian, Evan, Eugen, Umut et Danilo.*
