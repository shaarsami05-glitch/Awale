#ifndef CLIENT_H
#define CLIENT_H

#define MAX_AMIS 20

typedef int SOCKET;

typedef enum
{
    MENU = 1, // Menu initial
    LISTE_CLIENTS = 2, // Liste des users
    PROFIL = 3, // Profil d'un user
    EN_ATTENTE_DEFI = 4, // Défier un user
    PROPOSITION_DEFI = 5, //Recevoir défi
    EN_PARTIE = 6, // Défi accepté
    MENU_SPECTATEUR = 7, 
    CHAT = 8,
    CHAT_PRIVE = 9, // Chat privé avec un user
    SPECTATEUR = 10, // Spectateur d'une partie
    SELF_PROFIL = 11,
    BIO = 12,
    AMIS = 13,
    HISTORIQUE = 14,
    REPLAY = 15
} EtatClient;

typedef struct
{
    
    SOCKET sock;
    char name[32];
    char bio[32];

    EtatClient etat;

    //indice dans la liste clients du serveur
    int adversaire; // idAdversaire --> EN_ATTENTE_DEFI et EN_PARTIE

    int partie; //idPartie --> EN_PARTIE et SPECTATEUR
    
    int prive; // Si prive == 1, limiter visibilité partie à ses amis

    char amis[MAX_AMIS][32]; //liste des noms d'amis
    int nbAmis;
    
    // pouvoir revoir une partie
    int replayPartieId; // ID de la partie en cours de revisionnage
    int replayEtape;    // Numéro du coup actuel (0 = plateau initial, 1 = après 1er coup, etc.)

} Client;

#endif 
