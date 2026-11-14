# Awalé en Réseau (C / Sockets TCP)

Implémentation client-serveur du jeu traditionnel **Awalé** en C, permettant à plusieurs joueurs de s'affronter en temps réel, de discuter, d'observer des parties et de revoir leurs anciens matchs.

---

## Fonctionnalités

* **Jeu d'Awalé complet** : semis, captures, gestion de la famine, victoire à 25 graines, égalité (24–24) et forfait en cas d'abandon ou de déconnexion.
* **Multijoueur & Social** : liste des joueurs connectés, défis entre joueurs, profil personnalisé (bio, mode privé), liste d'amis et chat général ou privé en partie.
* **Mode Spectateur** : possibilité d'observer les parties publiques (ou privées entre amis) en direct.
* **Historique & Replay** : sauvegarde automatique des parties terminées dans `historique_parties.txt` et relecture coup par coup depuis le profil.
* **Classement** : calcul dynamique des scores (3 pts victoire, 1 pt nul, 0 pt défaite) et départage au nombre total de graines capturées.

---

## Structure du projet

* `Serveur/`
  * `serveur.c / .h` : Gestion des sockets TCP et boucle `select()`
  * `traitement.c / .h` : Logique des états clients et protocole réseau
  * `awale.c / .h` : Règles du jeu, semis, captures et fin de partie
  * `basedonnees.c / .h` : Gestion mémoire, persistance fichier et classement
  * `client.h` : Structure `Client` et énumération des états
* `Client/`
  * `client.c / .h` : Connexion au serveur et écoute des entrées
  * `interface.c / .h` : Affichage console (plateau, menus, replay, classement)
  * `awale.h` : Structures partagées du plateau
* `historique_parties.txt` : Fichier de sauvegarde généré automatiquement par le serveur

---

## Compilation et Lancement

### 1. Compiler le serveur et le client
Dans chacun des dossiers (`Serveur/` et `Client/`), exécuter :
```bash
make
```
puis pour lancer le serveur
```bash
./bin/serveur
```
et pour lancer un ou plusieurs clients
```bash
./bin/client 127.0.0.1 [Pseudo]
```
/!\ Les pseudos sont sensibles à la casse : Alice et alice sont deux clients différents.
Les pseudos suivant ont des parties pré-enregistré dans le fichier txt (Charlie, BOB, Alice)

## Commandes en jeu

| Contexte | Action/Commande | Effet |
| --- | --- | --- |
| **En partie** | `a` à `f` (Nord) ou `A` à `F` (Sud) | Jouer la case correspondante |
| **En partie** | `/msg <text>` | Envoyer un message privé à son adversaire |
| **En partie** | `/exit` (ou Ctrl+C) | Abandonner la partie (victoire par forfait à l'adversaire)|
| **Replay** | `1` | Passer au coup suivant |
| **Replay** | `2` | Revenir au coup précédent |
| **Menus** | `0` | Revenir à l'écran précédent ou au menu principal |
