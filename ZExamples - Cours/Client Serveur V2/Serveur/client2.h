#ifndef CLIENT_H
#define CLIENT_H

#include "server2.h"

typedef enum
{
    MENU, // Menu initial
    LISTE_CLIENTS, // Liste des users
    PROFIL, // Profil d'un user
    EN_ATTENTE_DEFI, // Défier un user
    EN_PARTIE, // Défi accepté
    SPECTATEUR, // Spectateur d'une partie
    CHAT
} EtatClient;

typedef struct
{
    SOCKET sock;
    char name[32];
    char bio[32];

    EtatClient etat;

    int adversaire; // idAdversaire --> EN_ATTENTE_DEFI et EN_PARTIE

    int partie; //idPartie --> EN_PARTIE et SPECTATEUR

} Client;

#endif 
