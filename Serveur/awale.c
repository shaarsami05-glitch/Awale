#include <stdio.h>
#include <string.h>
#include "awale.h"

// int nord[6] = [4,4,4,4,4,4];
// int sud[6] = [4,4,4,4,4,4];

// int joueur = 0  => NORD;
// int joueur = 1  => SUD;

void convertis(int *res, char lettre)
// permet de faire l'équivalence entre la lettre et l'indice du plateau
{
    if (lettre >= 'a' && lettre <= 'f')
    {
        res[0] = 0;
        res[1] = lettre - 'a';
    }
    else if (lettre >= 'A' && lettre <= 'F')
    {
        res[0] = 1;
        res[1] = lettre - 'A';
    }
    else
    {
        res[0] = 2;
        res[1] = -1;
    }
}

Plateau deplacerGraines(int *coordonnees, Plateau plateau)
// Ne jamais metttre de graines dans la case jouée
{
    int *nord = plateau.nord;
    int *sud = plateau.sud;

    int i = coordonnees[1] + 1;

    // Joueur Sud
    if (coordonnees[0])
    {
        int nbGraines = sud[coordonnees[1]];
        sud[coordonnees[1]] = 0;
        while (nbGraines > 0)
        {
            if (i < 6 && i != coordonnees[1])
            {
                sud[i]++;
                nbGraines--;
            }

            if (i >= 6)
            {
                nord[i % 6]++;
                nbGraines--;
            }

            i = (i + 1) % 12;
        }

        //  Tant que les cases du joueur adverse contiennent 2 ou 3 graines :
        if (i == 0)
        {
            i = 12;
        }
        while (--i >= 6 && (nord[i % 6] == 2 || nord[i % 6] == 3))
        {
            plateau.scoreSud += nord[i % 6];
            nord[i % 6] = 0;
        }
    }

    // Joueur Nord
    else
    {
        int nbGraines = nord[coordonnees[1]];
        nord[coordonnees[1]] = 0;
        while (nbGraines > 0)
        {
            if (i < 6 && i != coordonnees[1])
            {
                nord[i]++;
                nbGraines--;
            }

            if (i >= 6)
            {
                sud[i % 6]++;
                nbGraines--;
            }
            i = (i+1) % 12;
        }

        if (i == 0)
        {
            i = 12;
        }

        //  Tant que les cases du joueur adverse contiennent 2 ou 3 graines :
        while (--i >= 6 && (sud[i % 6] == 2 || sud[i % 6] == 3))
        {
            plateau.scoreNord += sud[i % 6];
            sud[i % 6] = 0;
        }
    }

    return plateau;
}

int estAffame(Plateau plateau, int joueur)
{
    // Vérifie que le coup n'a pas affamé l'adversaire
    // Renvoie 1 si tout le camp de l'adversaire est vide
    int estVide = 1;
    int i = 0;
    if (joueur) // SUD
    {
        while (estVide && i < 6)
        {
            if (plateau.nord[i++] != 0)
            {
                estVide = 0;
            }
        }
        return estVide;
    }
    else
    { // NORD
        while (estVide && i < 6)
        {
            if (plateau.sud[i++] != 0)
            {
                estVide = 0;
            }
        }
        return estVide;
    }
}

void nourrirAdversaire(Plateau plateau, int joueur)
{
    // Si plus aucun coup n'est possible, la partie se termine
    int coupImpossible = 1;
    int i = 0;

    Plateau copiePlateau = plateau;

    while (coupImpossible && i++ < 6)
    {
        int coord[2] = {joueur, i};
        copiePlateau = deplacerGraines(coord, copiePlateau);
        if (!estAffame(copiePlateau, joueur))
        {
            coupImpossible = 0;
        }
        copiePlateau = plateau;
    }

    if (coupImpossible)
    {
        // Partie s'arrete
        for (int j = 0; j < 6; j++)
        {
            plateau.scoreSud += plateau.sud[j];
            plateau.scoreNord += plateau.nord[j];
        }
    }
}

void jouerCoup(Partie *partie, char *buffer, int joueur, int *erreur)
// verifier que le coup est faisable puis l'appliquer
{
    // Analyse le buffer
    Plateau plateau = partie->plateau;
    if (joueur != partie->joueur)
    {
        *erreur = 1;
        strcpy(buffer, "Ce n'est pas votre tour !\n");
        return;
    }

    // Vérifier coups légaux :

    // Une seule lettre
    if (buffer[1] != '\0')
    {
        *erreur = 1;
        strcpy(buffer, "Ecrire une seule lettre svp");
        return;
    }
    // lettreJouee est [A , F] union [a, f]
    int coordonnees[2];
    convertis(coordonnees, buffer[0]);

    // Chaque joueur joue sa propre lettre
    // Vérification de la lettre
    if (coordonnees[0] == 2)
    {
        *erreur = 1;
        strcpy(buffer, "Case invalide !");
        return;
    }
    // Vérification du camp
    if (coordonnees[0] != joueur)
    {
        *erreur = 1;
        strcpy(buffer, "Vous ne pouvez pas jouer dans ce camp !");
        return;
    }
    // Case vide
    if ((coordonnees[0] && plateau.sud[coordonnees[1]] == 0) || (!coordonnees[0] && plateau.nord[coordonnees[1]] == 0))
    {
        *erreur = 1;
        strcpy(buffer, "Case vide. Veuillez rejouer");
        return;
    }

    Plateau sauvegardePlateau = plateau;

    plateau = deplacerGraines(coordonnees, plateau);

    // Prévenir la famine (= obligé de nourrir son adversaire => si toutes les cases adverses sont vides, obligé de le nourrir)
    if (estAffame(plateau, joueur))
    {
        // Restaurer le plateau
        plateau = sauvegardePlateau;
        *erreur = 1;
        strcpy(buffer, "Adversaire en FAMINE ! Coup invalide ! Veuillez rejouer");
        return;
    }

    // Enregistrer le nouveau plateau
    partie->plateau = plateau;

    // Changer de joueur pour le tour suivant
    int prochainJoueur = (partie->joueur + 1) % 2;
    partie->joueur = prochainJoueur;

    // Vérifier si le prochain joueur est bloqué (plus de graines ou impossible de nourrir)
    if (plateau.scoreNord < 25 && plateau.scoreSud < 25 &&
        !(plateau.scoreNord == 24 && plateau.scoreSud == 24) &&
        !peutJouer(plateau, prochainJoueur))
    {
        // Chaque joueur récupère les graines restantes de son camp
        for (int k = 0; k < 6; k++)
        {
            plateau.scoreNord += plateau.nord[k];
            plateau.nord[k] = 0;
            plateau.scoreSud += plateau.sud[k];
            plateau.sud[k] = 0;
        }
        partie->plateau = plateau;
        partie->statut = TERMINEE;
    }

    // Vérifier les conditions de victoire ou d'égalité
    if (plateau.scoreNord >= 25 || plateau.scoreSud >= 25 ||
        (plateau.scoreNord == 24 && plateau.scoreSud == 24) ||
        partie->statut == TERMINEE)
    {
        partie->statut = TERMINEE;
        if (plateau.scoreNord > plateau.scoreSud)
        {
            strcpy(partie->gagnant, partie->nomNord);
        }
        else if (plateau.scoreSud > plateau.scoreNord)
        {
            strcpy(partie->gagnant, partie->nomSud);
        }
        else
        {
            strcpy(partie->gagnant, "EGALITE");
        }
    }

    strcpy(buffer, "Coup valide !");
}

void serializerPlateau(Plateau plateau, char *joueurNord, char *joueurSud, char *buffer)
// créer le buffer à envoyer aux clients
{
    snprintf(buffer, 256, "%d,%d,%d,%d,%d,%d|%d,%d,%d,%d,%d,%d|%d|%d|%s|%s",
             plateau.nord[0], plateau.nord[1], plateau.nord[2],
             plateau.nord[3], plateau.nord[4], plateau.nord[5],
             plateau.sud[0], plateau.sud[1], plateau.sud[2],
             plateau.sud[3], plateau.sud[4], plateau.sud[5],
             plateau.scoreNord, plateau.scoreSud, joueurNord, joueurSud);
}

Plateau construirePlateauEtape(const Partie *partie, int etape)
// permet de review une game en se baladant selon les coups joués à l'étape k
{
    Plateau plateau;
    for (int k = 0; k < 6; k++)
    {
        plateau.nord[k] = 4;
        plateau.sud[k] = 4;
    }
    plateau.scoreNord = 0;
    plateau.scoreSud = 0;

    if (etape > partie->nbCoups)
        etape = partie->nbCoups;

    for (int i = 0; i < etape; i++)
    {
        int coordonnees[2];
        convertis(coordonnees, partie->coups[i]);
        plateau = deplacerGraines(coordonnees, plateau);
    }

    return plateau;
}

int peutJouer(Plateau plateau, int joueur)
{
    for (int i = 0; i < 6; i++)
    {
        int graines = (joueur == 0) ? plateau.nord[i] : plateau.sud[i];
        if (graines > 0)
        {
            int coord[2] = {joueur, i};
            Plateau copie = deplacerGraines(coord, plateau);
            if (!estAffame(copie, joueur))
            {
                return 1; // Au moins un coup valide existe
            }
        }
    }
    return 0; // Aucun coup possible
}
