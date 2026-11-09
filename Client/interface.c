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
    printf("%s\n", donnees);
    printf("0. Retour\n");
    printf("==============================\n");
}

void afficherProfil(char *donnees)
{
    printf("=========== PROFIL DE %s ===========\n", donnees);
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
    printf("En attente de %s...\n", donnees);
}

void afficherPropositionDefi(char *donnees)
{
    printf("======== DEFI PROPOSE ========\n");
    printf("De %s\n", donnees);
    printf("1. Accepter\n");
    printf("2. Décliner\n");
}