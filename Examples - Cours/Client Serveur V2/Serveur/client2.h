#ifndef CLIENT_H
#define CLIENT_H

#include "server2.h"

typedef enum
{
    MENU,
    CHOIX_ADVERSAIRE,
    EN_ATTENTE_DEFI,
    EN_PARTIE,
    SPECTATEUR,
    CHAT
} EtatClient;

typedef struct
{
    SOCKET sock;
    char name[32];

    EtatClient etat;

    //int adversaire;

    //int partie;

} Client;

#endif /* guard */
