# Projet Caméra RTSP — Qt / mpv

Application desktop développée en **C++ avec Qt**, destinée à afficher un flux vidéo **RTSP** directement dans une fenêtre graphique.

Le projet utilise **mpv** comme lecteur vidéo externe, lancé depuis Qt avec `QProcess`, puis intégré dans un `QWidget` natif grâce à l’option `--wid`.

> [!NOTE]
> Version actuelle du projet : affichage caméra simple, propre et orienté Linux.  
> L’objectif est de voir le flux caméra, sans interface lourde ni fonctions inutiles.

---

## Sommaire

- [Objectif du projet](#objectif-du-projet)
- [Fonctionnement général](#fonctionnement-général)
- [Structure du dépôt](#structure-du-dépôt)
- [Technologies utilisées](#technologies-utilisées)
- [Prérequis](#prérequis)
- [Installation sous Linux](#installation-sous-linux)
- [Compilation](#compilation)
- [Lancement](#lancement)
- [Configuration du flux RTSP](#configuration-du-flux-rtsp)
- [Tester le flux sans Qt](#tester-le-flux-sans-qt)
- [Dépannage](#dépannage)
- [Commandes Git utiles](#commandes-git-utiles)
- [Nettoyage du dépôt](#nettoyage-du-dépôt)

---

## Objectif du projet

Ce dépôt sert à intégrer proprement un flux caméra RTSP dans une application Qt.

L’application permet de :

- afficher une caméra RTSP dans une fenêtre Qt ;
- utiliser `mpv` pour obtenir un rendu vidéo fluide ;
- forcer le transport RTSP en TCP pour stabiliser le flux ;
- garder un code léger, lisible et facile à modifier ;
- préparer l’intégration dans une application de supervision plus complète.

> [!IMPORTANT]
> Le projet ne récupère pas directement le flux vidéo avec Qt.  
> Qt crée la fenêtre et le widget vidéo, puis lance `mpv` pour afficher le flux dans ce widget.

---

## Fonctionnement général

```text
Caméra / Raspberry Pi
        │
        │ Flux RTSP
        ▼
rtsp://127.0.0.1:8554/rascam
        │
        ▼
      mpv
        │ --wid=<ID_WIDGET_QT>
        ▼
  QWidget natif Qt
        │
        ▼
   MainWindow Qt
```

Étapes côté application :

1. Qt initialise l’application.
2. Une `MainWindow` est créée.
3. Un `QWidget` noir est placé au centre de la fenêtre.
4. Le widget est forcé en fenêtre native avec `Qt::WA_NativeWindow`.
5. Qt attend brièvement que la fenêtre existe réellement.
6. `mpv` est lancé avec `QProcess`.
7. `mpv` affiche le flux RTSP dans le widget Qt.

---

## Structure du dépôt

```text
git_cam/
├── .gitignore
├── README.md
├── main.cpp
├── mainwindow.cpp
├── mainwindow.h
└── projet_cam.pro
```

| Fichier | Rôle |
|---|---|
| `main.cpp` | Point d’entrée de l’application Qt |
| `mainwindow.h` | Déclaration de la fenêtre principale, du widget vidéo et du processus mpv |
| `mainwindow.cpp` | Création de l’interface, lancement de mpv et intégration du flux RTSP |
| `projet_cam.pro` | Configuration qmake du projet Qt |
| `.gitignore` | Exclusion des fichiers de build, fichiers temporaires Qt et fichiers IDE |

---

## Technologies utilisées

| Technologie | Utilisation |
|---|---|
| C++17 | Langage principal |
| Qt Widgets | Interface graphique desktop |
| QMainWindow | Fenêtre principale |
| QWidget | Zone d’affichage vidéo |
| QProcess | Lancement et gestion de mpv |
| QTimer | Délai avant injection de mpv dans la fenêtre Qt |
| mpv | Lecture du flux RTSP |
| RTSP | Protocole du flux caméra |
| qmake | Génération du Makefile |

---

## Prérequis

Avant de compiler, installer :

- Qt 6 ;
- un compilateur C++ ;
- qmake ;
- mpv ;
- Git.

> [!WARNING]
> Le projet dépend de `mpv`.  
> Si `mpv` n’est pas installé ou pas accessible dans le `PATH`, l’application peut se lancer mais la vidéo ne s’affichera pas.

---

## Installation sous Linux

### Ubuntu / Debian

```bash
sudo apt update
sudo apt install -y \
  build-essential \
  qt6-base-dev \
  qt6-base-dev-tools \
  mpv \
  git
```

Vérifier que les commandes nécessaires sont disponibles :

```bash
qmake6 -v
mpv --version
git --version
```

> [!TIP]
> Si `qmake6` n’existe pas sur la machine, tester avec `qmake`.

```bash
qmake -v
```

---

## Récupération du projet

```bash
git clone https://github.com/RooKye/git_cam.git
cd git_cam
```

Mettre à jour le dépôt local :

```bash
git pull origin master
```

---

## Compilation

Créer un dossier de compilation séparé :

```bash
mkdir -p build
cd build
```

Générer le Makefile avec qmake :

```bash
qmake6 ../projet_cam.pro
```

Si `qmake6` n’est pas reconnu :

```bash
qmake ../projet_cam.pro
```

Compiler :

```bash
make -j$(nproc)
```

---

## Lancement

Depuis le dossier `build/` :

```bash
./projet_cam
```

Si l’affichage Qt ne se lance pas correctement sous Linux, tester :

```bash
QT_QPA_PLATFORM=xcb ./projet_cam
```

> [!NOTE]
> Le code force déjà `QT_QPA_PLATFORM=xcb` sous Linux au démarrage de l’application.  
> Cette commande reste utile pour diagnostiquer un problème d’environnement graphique.

---

## Configuration du flux RTSP

L’adresse du flux est définie dans `mainwindow.cpp` :

```cpp
QString url = "rtsp://127.0.0.1:8554/rascam";
```

Pour changer le flux :

```cpp
QString url = "rtsp://ADRESSE_IP:PORT/NOM_DU_FLUX";
```

Exemple :

```cpp
QString url = "rtsp://192.168.1.50:8554/rascam";
```

> [!IMPORTANT]
> Dans cette version, l’application attend un flux accessible en local sur :
>
> ```text
> rtsp://127.0.0.1:8554/rascam
> ```
>
> Si le flux vient d’une autre machine, il faut soit modifier l’URL dans le code, soit créer un tunnel local vers `127.0.0.1:8554`.

---

## Tester le flux sans Qt

Avant de chercher une erreur dans l’application, toujours vérifier que le flux fonctionne directement avec `mpv`.

```bash
mpv --no-config \
  --vo=x11 \
  --hwdec=no \
  --demuxer-lavf-o=rtsp_transport=tcp \
  rtsp://127.0.0.1:8554/rascam
```

Commande plus simple :

```bash
mpv rtsp://127.0.0.1:8554/rascam
```

> [!TIP]
> Si le flux fonctionne dans `mpv` mais pas dans l’application Qt, le problème vient probablement de l’intégration du widget, du mode graphique ou du lancement du processus `mpv`.

---

## Tunnel RTSP local

Si la caméra ou le serveur RTSP est distant, il est possible de ramener le flux sur la machine locale avec un tunnel SSH.

Commande générique :

```bash
ssh -f -N \
  -i /chemin/vers/cle_ssh \
  -o "ExitOnForwardFailure=yes" \
  -o "ServerAliveInterval=30" \
  -o "ServerAliveCountMax=3" \
  -L 127.0.0.1:8554:127.0.0.1:8554 \
  utilisateur@ADRESSE_DU_SERVEUR
```

Ensuite, l’application peut continuer à lire :

```text
rtsp://127.0.0.1:8554/rascam
```

> [!WARNING]
> Ne pas mettre d’adresse IP publique, de clé privée ou d’identifiants sensibles directement dans le README public du dépôt.

---

## Dépannage

### La fenêtre s’ouvre mais la vidéo reste noire

Vérifier dans l’ordre :

```bash
mpv --demuxer-lavf-o=rtsp_transport=tcp rtsp://127.0.0.1:8554/rascam
```

Puis vérifier :

- le flux RTSP est actif ;
- le port `8554` est bien ouvert ou redirigé ;
- le tunnel SSH est actif si le flux est distant ;
- `mpv` est installé ;
- l’application est lancée depuis un vrai environnement graphique ;
- le mode `xcb` est disponible sous Linux.

---

### Erreur : `mpv` introuvable

Installer mpv :

```bash
sudo apt update
sudo apt install -y mpv
```

Vérifier :

```bash
which mpv
mpv --version
```

---

### Erreur Qt liée à `xcb`

Installer les dépendances Qt/XCB utiles :

```bash
sudo apt install -y \
  libxcb-cursor0 \
  libxcb-xinerama0 \
  libxkbcommon-x11-0
```

Relancer :

```bash
QT_QPA_PLATFORM=xcb ./projet_cam
```

---

### Le flux marche en terminal mais pas dans Qt

Tester avec la commande proche de celle utilisée par l’application :

```bash
mpv \
  --no-terminal \
  --no-osc \
  --no-border \
  --force-window=yes \
  --demuxer-lavf-o=rtsp_transport=tcp \
  --profile=low-latency \
  rtsp://127.0.0.1:8554/rascam
```

Si cette commande fonctionne, vérifier dans `mainwindow.cpp` :

```cpp
args << "--wid=" + windowId;
```

et vérifier que le widget est bien natif :

```cpp
videoWidget->setAttribute(Qt::WA_NativeWindow);
videoWidget->setAttribute(Qt::WA_DontCreateNativeAncestors);
```

---

### Nettoyer et recompiler entièrement

Depuis la racine du projet :

```bash
rm -rf build
mkdir build
cd build
qmake6 ../projet_cam.pro
make -j$(nproc)
./projet_cam
```

---

## Commandes Git utiles

### Voir l’état du dépôt

```bash
git status
```

### Ajouter le README

```bash
git add README.md
git commit -m "Mise à jour du README"
git push origin master
```

### Envoyer toutes les modifications

```bash
git add .
git commit -m "Mise à jour du projet caméra"
git push origin master
```

### Récupérer la dernière version

```bash
git pull origin master
```

### Vérifier la branche actuelle

```bash
git branch
```

### Changer de branche

```bash
git checkout nom_de_branche
```

---

## Nettoyage du dépôt

Le dossier `build/` ne doit pas être versionné dans Git.

Le fichier `.gitignore` exclut déjà les éléments de compilation et fichiers temporaires courants :

```gitignore
build/
cmake-build-*/
CMakeFiles/
CMakeCache.txt
Makefile
*.o
*.obj
*.exe
*.dll
*.so
*.a
*.pro.user
*.user
*.autosave
moc_*
qrc_*
ui_*
.vscode/
.idea/
.vs/
.DS_Store
Thumbs.db
```

Si un dossier `build/` a déjà été ajouté au dépôt par erreur :

```bash
git rm -r --cached build
git add .gitignore
git commit -m "Retire les fichiers de build du suivi Git"
git push origin master
```

> [!TIP]
> Git doit contenir le code source, pas les fichiers générés automatiquement par Qt Creator ou par la compilation.

---

## Évolution possible

Pistes d’amélioration pour une future version :

- rendre l’URL RTSP configurable sans modifier le code ;
- ajouter un bouton de reconnexion ;
- afficher un message d’erreur si le flux est indisponible ;
- journaliser les erreurs de `mpv` ;
- ajouter une interface de supervision capteurs ;
- intégrer plusieurs flux caméra ;
- ajouter une configuration externe en `.ini` ou `.json`.

---

## Résumé

Ce projet fournit une base simple et efficace pour afficher un flux RTSP dans une application Qt sous Linux.

Le principe est volontairement minimal :

```text
Qt crée la fenêtre.
mpv lit le flux.
Le widget Qt affiche la vidéo.
```

C’est une solution légère, lisible et adaptée à une IHM de supervision caméra.
