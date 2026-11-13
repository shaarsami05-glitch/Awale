#include <stdio.h>
#include <string.h>
#include "awale.h"
#include "basedonnees.h"
#include "client.h"

// int nord[6] = [4,4,4,4,4,4];
// int sud[6] = [4,4,4,4,4,4];

// int joueur = 0  => NORD;
// int joueur = 1  => SUD;

void convertis(int *res, char lettre)
{
    // printf("Appel convertis\n");
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

void afficherJeu(Plateau plateau)
{

    printf("------------ PLATEAU ------------\n");

    printf("f e d c b a \n");
    for (int i = 0; i < 6; i++)
    {
        printf("%d ", plateau.nord[5 - i]);
    }
    printf("\n");
    for (int j = 0; j < 6; j++)
    {
        printf("%d ", plateau.sud[j]);
    }
    printf("\nA B C D E F\n");
    printf("-------------------------------\n");
    printf("------------ SCORE ------------\n");
    printf("Joueur 0 (NORD) : %d\n", plateau.scoreNord);
    printf("Joueur 1 (SUD) : %d\n", plateau.scoreSud);
    printf("-------------------------------\n");
}

void afficherScore(Plateau plateau)
{
    printf("------------ SCORE ------------\n");
    printf("Joueur 0 (NORD) : %d\n", plateau.scoreNord);
    printf("Joueur 1 (SUD) : %d\n", plateau.scoreSud);
    printf("-------------------------------\n");
}

Plateau deplacerGraines(int *coordonnees, Plateau plateau)
// Ne jamais metttre de graines dans la case jouée
{
    printf("Appel deplacerGraines\n");

    // int *coordonnees = convertis(&coupJoue);

    int *nord = plateau.nord;
    int *sud = plateau.sud;

    int i = coordonnees[1] + 1;

    // Joueur Sud
    if (coordonnees[0])
    {
        printf("Sud\n");
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

            i = ++i % 12;
        }

        //  Tant que les cases du joueur adverse contiennent 2 ou 3 graines :
        // printf("i avant : %d\n", i);
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
        printf("Nord\n");
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
            i = ++i % 12;
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
        printf("coupImpossible");
        for (int j = 0; j < 6; j++)
        {
            plateau.scoreSud += plateau.sud[j];
            plateau.scoreNord += plateau.nord[j];
        }
    }
}

void lancerJeu()
{
    int estTermine = 0;
    int i = 0;

    // int nord[6] = {1, 0, 0, 0, 0, 0};
    // int sud[6] = {4, 4, 4, 4, 4, 1};
    // int scoreNord = 0;
    // int scoreSud = 0;
    // Plateau plateau = {nord, sud, scoreNord, scoreSud};

    Plateau plateau;
    for (int k = 0; k < 6; k++)
    {
        plateau.nord[k] = 4;
        plateau.sud[k] = 4;
    }
    plateau.scoreNord = 0;
    plateau.scoreSud = 0;

    afficherJeu(plateau);

    char ligne[10];

    while (!estTermine && i++ < 100)
    {
        // Si un joueur n'a plus de billes dans son camp, l'autre s'empare des dernieres graines
        nourrirAdversaire(plateau, i % 2);

        int estValide = 0;
        int coordonnees[2];
        while (!estValide)
        {
            // Vérifier coups légaux :
            printf("Joueur %d : ", i % 2);
            if (!fgets(ligne, sizeof ligne, stdin))
                continue;
            printf("%c\n", ligne[0]);

            // Une seule lettre
            if (ligne[1] != '\n')
            {
                printf("Ecrire une seule lettre svp\n");
            }
            else
            {
                printf("else \n");
                // lettreJouee est [A , F] union [a, f]
                convertis(coordonnees, ligne[0]);

                // Chaque joueur joue sa propre lettre
                if (coordonnees[0] == i % 2) // Nord = 0 et Sud = 1 (SUD commence)
                {
                    // Verifier case non vide
                    if (coordonnees[0] && plateau.sud[coordonnees[1]] != 0)
                    {
                        estValide = 1;
                    }
                    else if (!coordonnees[0] && plateau.nord[coordonnees[1]] != 0)
                    {
                        estValide = 1;
                    }
                    else
                    {
                        printf("Case vide. Veuillez rejouer\n");
                    }
                }
                else
                {
                    printf("Coup invalide veuillez rejouer\n");
                }
            }
        }
        printf("plateau\n");

        Plateau sauvegardePlateau = plateau;

        plateau = deplacerGraines(coordonnees, plateau);

        // Prévenir la famine (= obligé de nourrir son adversaire => si toutes les cases adverses sont vides, obligé de le nourrir)
        if (estAffame(plateau, i % 2))
        {
            // Restaurer le plateau
            plateau = sauvegardePlateau;
            i--;
        }
        afficherJeu(plateau);
        afficherScore(plateau);
        if (plateau.scoreNord > 24 || plateau.scoreSud > 24)
        {
            estTermine = 1;
            printf("Partie terminee \n");
        }
    }
}

void jouerCoup(Partie *partie, char *buffer, int joueur, int *erreur)
{
    printf("jouerCoup buffer : %s\n", buffer);
    // Analyse le buffer
    Plateau plateau = partie->plateau;
    if (joueur != partie->joueur)
    {
        *erreur = 1;
        strcpy(buffer, "Ce n'est pas votre tour !\n");
        return;
    }
    // afficherJeu(plateau);

    // Vérifier coups légaux :

    // Une seule lettre
    if (buffer[1] != '\0')
    {
        printf("buffer 1 : %c\n", buffer[1]);
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

    // verifier si un joueur a gagné
    if (plateau.scoreNord > 24 || plateau.scoreSud > 24)
    {
        partie->statut = 0;
        strcpy(buffer, "Partie Terminee !");
    }
    //changer de joueur
    partie->joueur = (partie->joueur + 1) % 2;

    strcpy(buffer, "Coup valide !");
    printf("FIN JouerCoup : %s\n", buffer);
}

void serializerPlateau(Plateau plateau, char* joueurNord, char* joueurSud, char *buffer)
// créer le buffer à envoyer aux clients
{
    snprintf(buffer, 256, "%d,%d,%d,%d,%d,%d|%d,%d,%d,%d,%d,%d|%d|%d|%s|%s",
             plateau.nord[0], plateau.nord[1], plateau.nord[2],
             plateau.nord[3], plateau.nord[4], plateau.nord[5],
             plateau.sud[0], plateau.sud[1], plateau.sud[2],
             plateau.sud[3], plateau.sud[4], plateau.sud[5],
             plateau.scoreNord, plateau.scoreSud, joueurNord, joueurSud);
}

// int main()
// {
//     lancerJeu();
//     return 0;
// }

// cd /mnt/c/Users/shaar/OneDrive/Documents/INSA/4A/Awale/bin
