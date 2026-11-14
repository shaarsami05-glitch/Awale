#include <stdio.h>
#include <string.h>
#include "interface.h"

// char interligne[30] = " ============================== ";

// void afficherAuthentification()
// {
//     int choix;
//     printf("======= AUTHENTIFICATION =======\n");
//     printf("1. Se connecter\n");
//     printf("2. S'inscrire\n");
//     printf("==============================\n");

//     scanf(" %d", &choix);

//     if (choix == 1)
//     {
//         char nomUtilisateur[10];
//         printf("============ CONNEXION ============\n");
//         printf("Entrez votre nom d'utilisateur : ");
//         scanf(" %s", nomUtilisateur);
//         printf("%s\n", nomUtilisateur);
//         // Chercher si correspond à un user
//         int connecte = 0;
//         if (connecte)
//         {
//             printf("Connexion validee\n");
//             afficherMenu(1);
//         }
//         else
//         {
//             printf("Le nom d'utilisateur n'existe pas\n");
//             afficherMenu(0);
//         }
//     }
//     else if (choix == 2)
//     {
//         char nomUtilisateur[10];
//         printf("============ INSCRIPTION ============\n");
//         printf("Entrez votre nom d'utilisateur : ");
//         scanf(" %s", nomUtilisateur);
//         printf("%s\n", nomUtilisateur);
//         // Cherche si déjà existant
//         int existeDeja = 0;
//         if (existeDeja)
//         {
//             printf("Le nom d'utilisateur est déjà pris\n");
//             afficherMenu(0);
//         }
//         else
//         {
//             printf("Inscription validee\n");
//             afficherMenuConnecte();
//         }
//     }
//     else
//     {
//         printf("Erreur d'entree\n");
//         afficherMenuConnecte();
//     }
// }

void afficherUtilisateurs(char *donnees)
{
    printf("======== UTILISATEURS ========\n");

    if (strlen(donnees) == 0)
    {
        printf("Aucun autre joueur connecte pour le moment.\n");
    }
    else
    {
        for (char *p = donnees; *p != '\0'; p++)
        {
            if (*p == '|')
                putchar('\n');
            else
                putchar(*p);
        }

        size_t len = strlen(donnees);
        if (len > 0 && donnees[len - 1] != '|')
            putchar('\n');
    }

    printf("0. Retour\n");
    printf("==============================\n");
}

void afficherSpectateur(char *donnees)
{
    printf("======== SPECTATEUR ========\n");

    Plateau p = {0};
    char joueurNord[100];
    char joueurSud[100];
    if (deserializerPlateau(donnees, &p, joueurNord, joueurSud))
    {
        afficherJeu(p, joueurNord, joueurSud);
    }
    printf("Vous êtes spectateur de la partie\n");
    printf("Tapez 0 pour quitter\n");
    printf("==============================\n");
}

void afficherProfil(char *donnees)
{
    char nom[100];
    char bio[100];
    int estAmi;
    if (sscanf(donnees, "%d|%99[^|]|%99[^|\n]", &estAmi, nom, bio) == 3)
    {
        printf("=========== PROFIL DE %s ===========\n", nom);
        printf("Bio : %s\n", bio);
        printf("1. Défier\n");
        if (estAmi)
            printf("2. Retirer des amis\n");
        else
            printf("2. Ajouter en ami\n");
        printf("3. Chat\n");
        printf("0. Retour\n");
        printf("==============================\n");
    }
}

void afficherMenuConnecte()
{
    printf("============ MENU ============\n");
    printf("1. Voir utilisateurs connectes\n");
    printf("2. Voir parties en cours\n");
    printf("3. Chat general\n");
    printf("4. Profil\n");
    printf("==============================\n");
}

void afficherAttenteDefi(char *donnees)
{
    printf("==============================\n");
    printf("En attente de %s...\n", donnees);
    printf("==============================\n");
}

void afficherPropositionDefi(char *donnees)
{
    printf("======== DEFI PROPOSE ========\n");
    printf("De %s\n", donnees);
    printf("1. Accepter\n");
    printf("2. Décliner\n");
    printf("==============================\n");
}

void afficherJeu(Plateau plateau, char *joueurNord, char *joueurSud)
{
    printf("\n");
    printf("                    A W A L E\n");
    printf("====================================================\n");

    // Informations joueur nord
    printf("  JOUEUR NORD : %-20s SCORE : %02d\n",
           joueurNord, plateau.scoreNord);

    printf("\n");

    // Coordonnees nord
    printf("          f      e      d      c      b      a\n");

    printf("       +------+------+------+------+------+------+ \n");

    // Rangee nord (de F vers A)
    printf("       |");

    for (int i = 5; i >= 0; i--)
    {
        printf("  %02d  |", plateau.nord[i]);
    }

    printf("\n");

    printf("       +------+------+------+------+------+------+ \n");

    // Separation centrale
    printf("       |                                      |\n");
    printf("       |             PLATEAU AWALÉ            |\n");
    printf("       |                                      |\n");

    printf("       +------+------+------+------+------+------+ \n");

    // Rangee sud (de A vers F)
    printf("       |");

    for (int i = 0; i < 6; i++)
    {
        printf("  %02d  |", plateau.sud[i]);
    }

    printf("\n");

    printf("       +------+------+------+------+------+------+ \n");

    // Coordonnees sud
    printf("          A      B      C      D      E      F\n");

    printf("\n");

    // Informations joueur sud
    printf("  JOUEUR SUD  : %-20s SCORE : %02d\n",
           joueurSud, plateau.scoreSud);

    printf("\n");
    printf("====================================================\n");
}

int deserializerPlateau(const char *donnees, Plateau *p, char *joueurNord, char *joueurSud)
{
    return sscanf(donnees, "%d,%d,%d,%d,%d,%d|%d,%d,%d,%d,%d,%d|%d|%d|%99[^|]|%99[^|\n]",
                  &p->nord[0], &p->nord[1], &p->nord[2],
                  &p->nord[3], &p->nord[4], &p->nord[5],
                  &p->sud[0], &p->sud[1], &p->sud[2],
                  &p->sud[3], &p->sud[4], &p->sud[5],
                  &p->scoreNord, &p->scoreSud, joueurNord, joueurSud) == 16; // 16 valeurs lues = succès
}

void traiteEnPartie(char *donnees)
{
    int erreur, doitJouer;
    char reste[800] = "";

    if (sscanf(donnees, "%d|%d|%799[^\n]", &erreur, &doitJouer, reste) == 3)
    {
        if (erreur == 0)
        {
            Plateau p = {0};
            char joueurNord[100];
            char joueurSud[100];
            if (deserializerPlateau(reste, &p, joueurNord, joueurSud))
            {
                afficherJeu(p, joueurNord, joueurSud);
                printf("/msg pour envoyer un message\n");
                printf("/exit pour quitter la partie\n");
            }
        }
        else
        {
            printf("%s\n", reste);
        }
        if (doitJouer)
            printf("C'est à vous de jouer !\n");
        else
            printf("En attente de l'adversaire...\n");
    }
}

void afficherSelfProfil(char *donnees)
{
    int prive;
    char bio[100];
    if (sscanf(donnees, "%d|%799[^\n]", &prive, bio) == 2)
    {
        printf("=========== PROFIL ===========\n");
        printf("Bio : %s\n", bio);
        printf("1. Modifier sa bio\n");
        printf("2. Voir ses amis\n");
        printf("3. Changer de statut ");
        if (prive)
            printf("(prive)\n");
        else
            printf("(public)\n");
        printf("4. Historique des parties\n");

        printf("0. Retour\n");
        printf("==============================\n");
    }
}

void afficherListeAmis(char *donnees)
{
    printf("========= LISTE AMIS =========\n");

    if (strlen(donnees) == 0)
    {
        printf("Vous n'avez aucun ami pour le moment.\n");
    }
    else
    {
        for (char *p = donnees; *p != '\0'; p++)
        {
            if (*p == '|')
                putchar('\n');
            else
                putchar(*p);
        }

        size_t len = strlen(donnees);
        if (len > 0 && donnees[len - 1] != '|')
            putchar('\n');
    }

    printf("0. Retour\n");
    printf("==============================\n");
}

void afficherPartiesEnCours(char *donnees)
{
    printf("====== PARTIES EN COURS ======\n");

    if (strlen(donnees) == 0)
    {
        printf("Aucune partie en cours disponible.\n");
    }
    else
    {
        printf("Choisissez une partie a regarder :\n");
        for (char *p = donnees; *p != '\0'; p++)
        {
            if (*p == '|')
                putchar('\n');
            else
                putchar(*p);
        }

        size_t len = strlen(donnees);
        if (len > 0 && donnees[len - 1] != '|')
            putchar('\n');
    }

    printf("0. Retour\n");
    printf("==============================\n");
}

void afficherHistorique(char *donnees)
{
    printf("===== HISTORIQUE PARTIES =====\n");

    if (strlen(donnees) == 0)
    {
        printf("Aucune partie jouee pour le moment.\n");
    }
    else
    {
        printf("Choisissez une partie a revoir :\n");
        for (char *p = donnees; *p != '\0'; p++)
        {
            if (*p == '|')
                putchar('\n');
            else
                putchar(*p);
        }

        size_t len = strlen(donnees);
        if (len > 0 && donnees[len - 1] != '|')
            putchar('\n');
    }

    printf("0. Retour\n");
    printf("==============================\n");
}

void afficherReplay(char *donnees)
{
    int etape, totalCoups;
    char coup;
    char reste[800] = "";

    if (sscanf(donnees, "%d|%d|%c|%799[^\n]", &etape, &totalCoups, &coup, reste) == 4)
    {
        printf("=========== REPLAY ===========\n");
        if (etape == 0)
            printf("Position initiale (Coup 0 / %d)\n", totalCoups);
        else
            printf("Coup %d / %d (Case jouee : %c)\n", etape, totalCoups, coup);

        Plateau p = {0};
        char joueurNord[100];
        char joueurSud[100];
        if (deserializerPlateau(reste, &p, joueurNord, joueurSud))
        {
            afficherJeu(p, joueurNord, joueurSud);
        }

        if (etape < totalCoups)
            printf("[n] Coup suivant  ");
        if (etape > 0)
            printf("[p] Coup precedent  ");
        printf("[0] Retour historique\n");
        printf("==============================\n");
    }
}

void afficherFinPartie(char *donnees)
{
    char gagnant[64];
    char reste[800] = "";

    if (sscanf(donnees, "%63[^|]|%799[^\n]", gagnant, reste) == 2)
    {
        Plateau p = {0};
        char joueurNord[100];
        char joueurSud[100];
        if (deserializerPlateau(reste, &p, joueurNord, joueurSud))
        {
            afficherJeu(p, joueurNord, joueurSud);
        }

        printf("====================================================\n");
        printf("                  FIN DE LA PARTIE                  \n");
        if (strcmp(gagnant, "EGALITE") == 0)
        {
            printf("  Resultat : MATCH NUL (EGALITE) !\n");
        }
        else
        {
            printf("  Vainqueur : %s !\n", gagnant);
        }
        printf("====================================================\n\n");

        afficherMenuConnecte();
    }
}