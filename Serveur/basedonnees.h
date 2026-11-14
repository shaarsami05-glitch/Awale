#ifndef BASEDONNEES_H
#define BASEDONNEES_H

#ifdef WIN32

#include <winsock2.h>

#elif defined(linux)

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> /* close */
#include <netdb.h>  /* gethostbyname */
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(s) close(s)
typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;

#else

#error not defined for this platform

#endif

#define CRLF "\r\n"
#define PORT 1977
#define MAX_CLIENTS 100
#define MAX_PARTIES 100

#define BUF_SIZE 1024
#define FICHIER_HISTORIQUE "historique_parties.txt"

#include "client.h"
#include "traitement.h"
#include "awale.h"

typedef struct
{
    char nom[32];
    int victoires;
    int egalites;
    int defaites;
    int points;       // 3 pts victoire, 1 pt nul, 0 pt défaite
    int grainesTotal; // Cumul des graines capturées (départage)
} StatsJoueur;

Partie *initialiserPartie(Client *joueurA, Client *joueurB);
Partie *getPartieParId(int id);
int getPartiesEnCours(Partie** partiesEnCours);
int getPartiesTermineesJoueur(const char *nomJoueur, Partie **partiesJoueur);
void sauvegarderPartieFichier(const Partie *partie);
void chargerPartiesFichier(void);
int calculerClassement(StatsJoueur *classement, int maxJoueurs);

#endif /* guard */
