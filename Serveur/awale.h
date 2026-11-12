#ifndef AWALE_H
#define AWALE_H

// #include "basedonnees.h"
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
    int joueur; // A qui le tour
    Statut statut;
} Partie;

typedef enum
{
    PLATEAU = 0,
    MESSAGE = 1,
} TypeMessage;

void convertis(int *res, char lettre);
void afficherJeu(Plateau plateau);
void afficherScore(Plateau plateau);
Plateau deplacerGraines(int *coordonnees, Plateau plateau);
int estAffame(Plateau plateau, int joueur);
void nourrirAdversaire(Plateau plateau, int joueur);
void lancerJeu();
void serializerPlateau(Plateau plateau, char *buffer);
void jouerCoup(Partie *partie, char *buffer, int joueur, int *erreur);

#endif