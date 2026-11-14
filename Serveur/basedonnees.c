#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#include "basedonnees.h"

Partie parties[MAX_PARTIES];
int nbParties = 0;

Partie *initialiserPartie(Client *joueurA, Client *joueurB)
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

    // Nouveaux paramètres pour la sauvegarde ---------------------
    strncpy(partie->nomNord, joueurA->name, sizeof(partie->nomNord) - 1);
    partie->nomNord[sizeof(partie->nomNord) - 1] = '\0';
    strncpy(partie->nomSud, joueurB->name, sizeof(partie->nomSud) - 1);
    partie->nomSud[sizeof(partie->nomSud) - 1] = '\0';
    partie->nbCoups = 0;
    partie->coups[0] = '\0';
    // -------------------------------------------------------------

    partie->joueur = rand() % 2; // Détermine aléatoirement le joueur qui commence
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


int getPartiesEnCours(Partie** partiesEnCours)
{
    //Partie **partiesEnCours;
    int nbPartiesEnCours = 0;
    for (int i = 0; i < nbParties; i++)
    {
        if (parties[i].statut == EN_COURS)
        {
            partiesEnCours[nbPartiesEnCours++] = &parties[i];
        }
    }
    return nbPartiesEnCours;
}

int getPartiesTermineesJoueur(const char *nomJoueur, Partie **partiesJoueur)
{
    int count = 0;
    for (int i = 0; i < nbParties; i++)
    {
        if (parties[i].statut == TERMINEE && (strcmp(parties[i].nomNord, nomJoueur) == 0 || strcmp(parties[i].nomSud, nomJoueur) == 0))
        {
            partiesJoueur[count++] = &parties[i];
        }
    }
    return count;
}

