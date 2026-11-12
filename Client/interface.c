#include <stdio.h>
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
    // affiche la liste des utilisateurs CONNECTES
    // 1. Bob
    // 2. Alice
    printf("======== UTILISATEURS ========\n");
    printf("%s", donnees);
    printf("0. Retour\n");
    printf("==============================\n");
}

void afficherProfil(char *donnees)
{
    printf("=========== PROFIL DE %s ===========\n", donnees);
    printf("Bio : Aucune bio pour le moment\n");
    printf("1. Défier\n");
    printf("2. Ajouter en ami\n");
    printf("3. Chat\n");
    printf("0. Retour\n");
    printf("==============================\n");
}

void afficherMenuConnecte()
{
    printf("============ MENU ============\n");
    printf("1. Voir utilisateurs connectes\n");
    printf("2. Voir parties en cours\n");
    printf("3. Chat general\n");
    printf("4. Se deconnecter\n");
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

int deserializerPlateau(const char *donnees, Plateau *p)
{
    return sscanf(donnees, "%d,%d,%d,%d,%d,%d|%d,%d,%d,%d,%d,%d|%d|%d",
                  &p->nord[0], &p->nord[1], &p->nord[2],
                  &p->nord[3], &p->nord[4], &p->nord[5],
                  &p->sud[0], &p->sud[1], &p->sud[2],
                  &p->sud[3], &p->sud[4], &p->sud[5],
                  &p->scoreNord, &p->scoreSud) == 14; // 14 valeurs lues = succès
}

void traiteEnPartie(char *donnees)
{
    int erreur;
    char message[100];
    if (sscanf(donnees, "%d|%899[^\n]", &erreur, message) >= 1)
    {
        if (erreur)
        {
            printf("%s\n", message);
        }
        else
        {
            Plateau p = {0};
            deserializerPlateau(message, &p);
            afficherJeu(p);
        }
    }
}
