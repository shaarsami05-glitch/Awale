#ifndef TRAITEMENT_H
#define TRAITEMENT_H

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

#define BUF_SIZE 1024

#include "client.h"
#include "awale.h"

void traiter_message(Client clients[], int actual, int indice, char *buffer);
void traiter_menu(Client *client, char *buffer, Client clients[], int actual);
void traiter_liste_clients(Client *client, char *buffer, Client clients[], int actual);
void traiter_profil(Client *client, char *buffer, Client clients[], int indice, int actual);
void traiter_proposition_defi(Client *client, char *buffer, Client clients[]);
void traiter_en_partie(Client *client, char *buffer, Client clients[], int actual);
void traiter_chat(Client *client, char *buffer, Client clients[], int actual);
void send_message_to_all_clients(Client *clients, Client client, int actual, const char *buffer, char from_server);
void traiter_chat_prive(Client *client, char *buffer, Client clients[], int actual);
void construire_menu(char *buffer);
void construire_liste_clients(Client *client, Client clients[], int actual, char *buffer);
static void envoyerPlateau(int sock, const char *plateauStr, int doitJouer, int erreur);
void traiter_menu_spectateur(Client *client, char *buffer);
void traiter_spectateur(Client *client, char *buffer);
void traiter_self_profil(Client *client, char *buffer);
void traiter_bio(Client *client, char *buffer);
int estAmi(const Client *c, const char *nom);
int ajouterAmi(Client *c, const char *nom);
int retirerAmi(Client *c, const char *nom);
void retourSelfProfil(Client *client, char *buffer);
void traiter_historique(Client *client, char *buffer);

#endif /* guard */
