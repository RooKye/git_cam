# 📹 Projet Caméra RTSP - Qt / VLC

Application de supervision développée en **C++ avec Qt**, permettant d'afficher un flux vidéo **RTSP** directement dans une fenêtre principale grâce à l'intégration de **libVLC**.

Le projet a été conçu dans le cadre d'une application de surveillance avec caméra réseau, où le flux vidéo est récupéré depuis une source RTSP locale puis affiché dans une interface graphique simple, fluide et légère.

---

## 🎯 Objectif du projet

L'objectif est d'intégrer proprement un flux caméra dans une application Qt, sans passer par un navigateur embarqué ou une solution trop lourde.

L'application permet de :

- afficher un flux vidéo RTSP dans une `MainWindow` Qt ;
- utiliser VLC comme moteur vidéo pour améliorer la fluidité ;
- garder un code simple, lisible et facilement modifiable ;
- préparer une intégration dans une application de supervision plus complète.

> [!NOTE]
> Ce projet utilise **libVLC** afin d'afficher la vidéo dans un widget Qt natif. Cette solution est plus adaptée qu'un affichage web lorsque l'objectif est d'avoir un rendu vidéo fluide dans une application desktop.

---

## 🧱 Technologies utilisées

| Technologie | Rôle |
|---|---|
| **C++** | Langage principal |
| **Qt 6** | Interface graphique |
| **QMainWindow** | Fenêtre principale de l'application |
| **libVLC** | Lecture du flux vidéo RTSP |
| **RTSP** | Protocole de diffusion vidéo |
| **qmake** | Compilation du projet Qt |

---

## 📁 Structure du projet

```txt
git_cam/
├── main.cpp
├── mainwindow.cpp
├── mainwindow.h
├── projet_cam.pro
├── .gitignore
└── README.md
```

### Rôle des fichiers

| Fichier | Description |
|---|---|
| `main.cpp` | Point d'entrée de l'application Qt |
| `mainwindow.h` | Déclaration de la fenêtre principale et des objets VLC |
| `mainwindow.cpp` | Création de l'interface et affichage du flux RTSP |
| `projet_cam.pro` | Configuration du projet Qt et des dépendances libVLC |
| `.gitignore` | Fichiers à exclure du dépôt Git |

---

## ⚙️ Fonctionnement général

Le fonctionnement est volontairement simple :

1. Qt crée une fenêtre principale avec `QMainWindow`.
2. Un `QWidget` noir est ajouté au centre de la fenêtre.
3. Ce widget devient la zone d'affichage vidéo.
4. libVLC est initialisé dans l'application.
5. Le flux RTSP est chargé par VLC.
6. La vidéo est affichée directement dans le widget Qt.

```txt
Caméra / flux RTSP
        ↓
     libVLC
        ↓
 QWidget natif Qt
        ↓
 MainWindow
```

---

## 🔗 Configuration du flux RTSP

L'adresse du flux vidéo se trouve dans le fichier `mainwindow.cpp` :

```cpp
const char *url = "rtsp://200.26.16.20:8554/rascam";
```

Pour changer de caméra ou de serveur vidéo, il suffit de modifier cette ligne :

```cpp
const char *url = "rtsp://ADRESSE_IP:PORT/NOM_DU_FLUX";
```

Exemple :

```cpp
const char *url = "rtsp://192.168.1.50:8554/rascam";
```

> [!IMPORTANT]
> Avant de lancer l'application, il faut vérifier que le flux RTSP fonctionne correctement avec VLC ou FFplay.

Test avec VLC :

```bash
vlc rtsp://200.26.16.20:8554/rascam
```

Test avec FFplay :

```bash
ffplay rtsp://200.26.16.20:8554/rascam
```

---

## ✅ Prérequis

Avant de compiler le projet, il faut installer :

- Qt 6 ;
- un compilateur C++ compatible ;
- VLC ;
- les fichiers de développement libVLC ;
- qmake ou qmake6.

---

## 🐧 Installation sous Linux / Ubuntu

### 1. Installer les dépendances

```bash
sudo apt update
sudo apt install -y build-essential qt6-base-dev qt6-base-dev-tools libvlc-dev vlc pkg-config
```

### 2. Cloner le dépôt

```bash
git clone https://github.com/RooKye/git_cam.git
cd git_cam
```

### 3. Créer un dossier de compilation

```bash
mkdir build
cd build
```

### 4. Générer le Makefile

```bash
qmake6 ../projet_cam.pro
```

Si `qmake6` n'est pas reconnu, essayer :

```bash
qmake ../projet_cam.pro
```

### 5. Compiler le projet

```bash
make -j$(nproc)
```

### 6. Lancer l'application

```bash
./projet_cam
```

> [!TIP]
> Si l'application se lance mais que la vidéo ne s'affiche pas, vérifier d'abord le flux RTSP avec VLC ou FFplay avant de chercher une erreur dans le code.

---

## 🪟 Installation sous Windows

### 1. Installer Qt

Installer **Qt 6** avec un kit compatible, par exemple :

```txt
Desktop Qt 6.x MinGW 64-bit
```

### 2. Installer VLC

Installer VLC en version 64 bits, de préférence dans le chemin classique :

```txt
C:\Program Files\VideoLAN\VLC
```

### 3. Vérifier les chemins dans `projet_cam.pro`

Le fichier `.pro` contient déjà la configuration Windows :

```pro
win32 {
    INCLUDEPATH += "C:/Program Files/VideoLAN/VLC/sdk/include"
    LIBS += -L"C:/Program Files/VideoLAN/VLC/sdk/lib" -llibvlc -llibvlccore
}
```

Si VLC est installé ailleurs, modifier les chemins :

```pro
INCLUDEPATH += "CHEMIN/VERS/VLC/sdk/include"
LIBS += -L"CHEMIN/VERS/VLC/sdk/lib" -llibvlc -llibvlccore
```

> [!WARNING]
> L'architecture doit être cohérente : Qt 64 bits avec VLC 64 bits, ou Qt 32 bits avec VLC 32 bits. Mélanger les deux peut provoquer des erreurs de compilation ou de lancement.

### 4. Ouvrir le projet dans Qt Creator

Depuis Qt Creator :

```txt
File > Open File or Project > projet_cam.pro
```

Puis sélectionner le kit Qt et lancer :

```txt
Build > Run
```

---

## 🧪 Commandes Git utiles

### Ajouter le README au dépôt

```bash
git add README.md
git commit -m "Ajout du README du projet caméra RTSP"
git push origin master
```

### Voir l'état du dépôt

```bash
git status
```

### Récupérer la dernière version du dépôt

```bash
git pull origin master
```

---

## 🧹 Nettoyage conseillé du dépôt

Les dossiers de compilation générés par Qt ne doivent normalement pas être versionnés dans Git.

Exemple de dossier à éviter :

```txt
build/
```

Pour le retirer du suivi Git sans le supprimer du PC :

```bash
git rm -r --cached build
```

Puis ajouter cette règle dans `.gitignore` :

```gitignore
build/
*.pro.user
*.o
*.obj
*.exe
*.dll
Makefile
```

Ensuite valider la modification :

```bash
git add .gitignore
git commit -m "Nettoyage des fichiers de build"
git push origin master
```

> [!TIP]
> Git doit contenir le code source, pas les fichiers temporaires générés par Qt Creator ou par la compilation.

---

## 🛠️ Problèmes fréquents

### Erreur : `vlc/vlc.h` introuvable

Vérifier que les fichiers de développement libVLC sont installés.

Sous Linux :

```bash
sudo apt install libvlc-dev
```

Sous Windows, vérifier que ce chemin existe :

```txt
C:\Program Files\VideoLAN\VLC\sdk\include
```

Si le dossier `sdk` n'existe pas, il faut installer ou récupérer les fichiers de développement de VLC/libVLC correspondant à votre version.

---

### Erreur : `libvlc.dll` introuvable au lancement

Sous Windows, ajouter VLC au `PATH` ou copier les fichiers nécessaires dans le dossier de l'exécutable.

Exemples de fichiers souvent nécessaires :

```txt
libvlc.dll
libvlccore.dll
plugins/
```

---

### La vidéo est noire

Vérifier que :

- le flux RTSP est actif ;
- l'adresse IP est correcte ;
- le port est ouvert ;
- le flux fonctionne dans VLC ;
- le widget Qt est bien natif avec `Qt::WA_NativeWindow`.

Commande de test :

```bash
vlc rtsp://200.26.16.20:8554/rascam
```

---

### La vidéo lag

Le projet utilise déjà VLC avec une option de cache réseau :

```cpp
libvlc_media_add_option(media, ":network-caching=150");
```

Pour tester plus de stabilité, augmenter légèrement la valeur :

```cpp
libvlc_media_add_option(media, ":network-caching=300");
```

Pour réduire la latence, essayer une valeur plus basse :

```cpp
libvlc_media_add_option(media, ":network-caching=80");
```

> [!NOTE]
> Plus le cache est élevé, plus la vidéo est stable, mais plus la latence augmente. Plus le cache est bas, plus la latence diminue, mais le flux peut devenir moins stable.

---

## 🚀 Améliorations possibles

Quelques pistes d'évolution pour la suite du projet :

- ajouter un champ pour modifier l'adresse RTSP depuis l'interface ;
- ajouter un bouton de reconnexion au flux ;
- afficher un message si la caméra est inaccessible ;
- intégrer plusieurs flux caméra ;
- ajouter une page de supervision avec les capteurs ;
- séparer la configuration dans un fichier externe ;
- améliorer la gestion des erreurs libVLC.

---

## 👤 Auteur

Projet réalisé dans le cadre d'une application de supervision vidéo avec Qt/C++.

**Auteur :** RooKye  
**Dépôt :** `git_cam`

---

## 📌 Résumé

Ce dépôt contient une base simple et fonctionnelle pour afficher un flux RTSP dans une application Qt grâce à libVLC.

L'objectif principal est de garder une architecture légère, claire et efficace pour intégrer la vidéo dans une IHM de supervision.
