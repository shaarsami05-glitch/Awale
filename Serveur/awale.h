#ifndef AWALE_H
#define AWALE_H

// #include "basedonnees.h"

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
    int joueurNord;
    int joueurSud;
    Statut statut;
} Partie;

typedef enum
{
    PLATEAU = 0,
    MESSAGE = 1,
} TypeMessage;

int *convertis(char *lettre);
void afficherJeu(Plateau plateau);
void afficherScore(Plateau plateau);
Plateau deplacerGraines(int *coordonnees, Plateau plateau);
int estAffame(Plateau plateau, int joueur);
void nourrirAdversaire(Plateau plateau, int joueur);
void lancerJeu();
void serializerPlateau(Plateau plateau, char* buffer);
Partie initialiserPartie(int joueurA, int joueurB);
void jouerCoup(Partie *partie, char *buffer, int joueur);

#endif