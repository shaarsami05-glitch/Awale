#ifndef AWALE_H
#define AWALE_H
#define MAX_COUPS 256

#include "client.h"

typedef struct
{
    int nord[6];
    int sud[6];
    int scoreNord;
    int scoreSud;
} Plateau;

typedef enum
{
    EN_COURS = 1,
    TERMINEE = 0
} Statut;

typedef struct
{
    int id;
    Plateau plateau;
    Client *joueurNord;
    Client *joueurSud;
    char nomNord[32];
    char nomSud[32];
    char gagnant[32];
    char coups[MAX_COUPS]; // historique des coups (ex: "aBfC...")
    int nbCoups; 
    int joueur; // A qui le tour
    Statut statut;
} Partie;

void convertis(int *res, char lettre);
Plateau deplacerGraines(int *coordonnees, Plateau plateau);
int estAffame(Plateau plateau, int joueur);
void nourrirAdversaire(Plateau plateau, int joueur);
void serializerPlateau(Plateau plateau, char* joueurNord, char* joueurSud, char *buffer);
void jouerCoup(Partie *partie, char *buffer, int joueur, int *erreur);
Plateau construirePlateauEtape(const Partie *partie, int etape);
int peutJouer(Plateau plateau, int joueur);

#endif