#ifndef CLIENT_H
#define CLIENT_H

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
    SPECTATEUR = 10 // Spectateur d'une partie
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

} Client;

#endif 
