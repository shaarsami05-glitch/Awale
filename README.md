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

```text
├── Serveur/
│   ├── serveur.c / .h        # Gestion des sockets TCP et boucle select()
│   ├── traitement.c / .h     # Logique des états clients et protocole réseau
│   ├── awale.c / .h          # Règles du jeu, semis, captures et fin de partie
│   ├── basedonnees.c / .h    # Gestion mémoire, persistance fichier et classement
│   └── client.h              # Structure Client et énumération des états
├── Client/
│   ├── client.c / .h         # Connexion au serveur et écoute des entrées
│   ├── interface.c / .h      # Affichage console (plateau, menus, replay, classement)
│   └── awale.h               # Structures partagées du plateau
└── historique_parties.txt    # Fichier de sauvegarde généré par le serveur
