#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "basedonnees.h"

Partie parties[MAX_PARTIES];
int nbParties = 0;

Partie *initialiserPartie(int joueurA, int joueurB)
{
    if (nbParties >= MAX_PARTIES)
        return NULL;

    Partie *partie = &parties[nbParties];

    partie->id = nbParties++;
    for (int k = 0; k < 6; k++)
    {
        partie->plateau.nord[k] = 4;
        partie->plateau.sud[k] = 4;
    }
    partie->plateau.scoreNord = 0;
    partie->plateau.scoreSud = 0;

    partie->joueurNord = joueurA;
    partie->joueurSud = joueurB;
    partie->statut = EN_COURS;

    return partie;
}

Partie *getPartieParId(int id)
{
    for (int i = 0; i < nbParties; i++)
    {
        if (parties[i].id == id)
        {
            return &parties[i];
        }
    }
    return NULL;
}