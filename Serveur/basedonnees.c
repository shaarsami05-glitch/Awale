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

    strncpy(partie->nomNord, joueurA->name, sizeof(partie->nomNord) - 1);
    partie->nomNord[sizeof(partie->nomNord) - 1] = '\0';
    strncpy(partie->nomSud, joueurB->name, sizeof(partie->nomSud) - 1);
    partie->nomSud[sizeof(partie->nomSud) - 1] = '\0';
    partie->nbCoups = 0;
    partie->coups[0] = '\0';
    partie->gagnant[0] = '\0';

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

void sauvegarderPartieFichier(const Partie *partie)
{
    FILE *f = fopen(FICHIER_HISTORIQUE, "a");
    if (f == NULL)
    {
        perror("fopen sauvegarde historique");
        return;
    }

    const char *coupsStr = (partie->nbCoups > 0) ? partie->coups : "-";
    const char *gagnantStr = (strlen(partie->gagnant) > 0) ? partie->gagnant : "INCONNU";

    fprintf(f, "%s|%s|%d|%d|%s|%d|%s\n",
            partie->nomNord,
            partie->nomSud,
            partie->plateau.scoreNord,
            partie->plateau.scoreSud,
            gagnantStr,
            partie->nbCoups,
            coupsStr);

    fclose(f);
}

void chargerPartiesFichier(void)
{
    FILE *f = fopen(FICHIER_HISTORIQUE, "r");
    if (f == NULL)
    {
        // Le fichier n'existe pas encore au tout premier lancement : ce n'est pas une erreur
        return;
    }

    char ligne[512];
    while (fgets(ligne, sizeof(ligne), f) != NULL && nbParties < MAX_PARTIES)
    {
        // Supprimer le saut de ligne
        ligne[strcspn(ligne, "\r\n")] = '\0';
        if (strlen(ligne) == 0)
            continue;

        Partie p = {0};
        char coupsTemp[MAX_COUPS] = "";

        if (sscanf(ligne, "%31[^|]|%31[^|]|%d|%d|%31[^|]|%d|%255s",
                   p.nomNord,
                   p.nomSud,
                   &p.plateau.scoreNord,
                   &p.plateau.scoreSud,
                   p.gagnant,
                   &p.nbCoups,
                   coupsTemp) == 7)
        {
            p.id = nbParties;
            p.statut = TERMINEE;
            p.joueurNord = NULL;
            p.joueurSud = NULL;

            if (strcmp(coupsTemp, "-") == 0)
            {
                p.coups[0] = '\0';
                p.nbCoups = 0;
            }
            else
            {
                strncpy(p.coups, coupsTemp, MAX_COUPS - 1);
                p.coups[MAX_COUPS - 1] = '\0';
            }

            parties[nbParties++] = p;
        }
    }

    fclose(f);
    printf("Historique charge : %d partie(s) recuperee(s).\n", nbParties);
}

