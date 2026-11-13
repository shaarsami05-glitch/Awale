#ifndef INTERFACE_H
#define INTERFACE_H

#ifdef WIN32

#include <winsock2.h>
#include "interface.h"

#elif defined (linux)

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> /* close */
#include <netdb.h> /* gethostbyname */
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

#define CRLF     "\r\n"
#define PORT     1977

#define BUF_SIZE 1024

typedef struct
{
    int nord[6];
    int sud[6];
    int scoreNord;
    int scoreSud;
} Plateau;

void afficherMenuConnecte();
void afficherUtilisateurs(char* donnees);
void afficherProfil(char* donnees);
void afficherAttenteDefi(char *donnees);
void afficherPropositionDefi(char* donnees);
int deserializerPlateau(const char *donnees, Plateau *p, char *joueurNord, char *joueurSud);
void afficherJeu(Plateau plateau, char* joueurNord, char* joueurSud);
void traiteEnPartie(char *donnees);
void afficherPartiesEnCours(char *donnees);
void afficherSpectateur(char *donnees);
void afficherSelfProfil(char *donnees);




#endif /* guard */
