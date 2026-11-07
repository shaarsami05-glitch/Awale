#include <stdio.h>
#include "interface.h"

// char interligne[30] = " ============================== ";

void afficherAuthentification()
{
    int choix;
    printf("1. Se connecter\n");
    printf("2. S'inscrire\n");
    scanf(" %d", &choix);

    if (choix == 1)
    {
        char nomUtilisateur[10];
        printf("============ CONNEXION ============\n");
        printf("Entrez votre nom d'utilisateur : ");
        scanf(" %s", nomUtilisateur);
        printf("%s\n", nomUtilisateur);
        // Chercher si correspond à un user
        int connecte = 0;
        if (connecte)
        {
            printf("Connexion validee\n");
            afficherMenu(1);
        }
        else
        {
            printf("Le nom d'utilisateur n'existe pas\n");
            afficherMenu(0);
        }
    }
    else if (choix == 2)
    {
        char nomUtilisateur[10];
        printf("============ INSCRIPTION ============\n");
        printf("Entrez votre nom d'utilisateur : ");
        scanf(" %s", nomUtilisateur);
        printf("%s\n", nomUtilisateur);
        // Cherche si déjà existant
        int existeDeja = 0;
        if (existeDeja)
        {
            printf("Le nom d'utilisateur est déjà pris\n");
            afficherMenu(0);
        }
        else
        {
            printf("Inscription validee\n");
            afficherMenu(1);
        }
    }
    else
    {
        printf("Erreur d'entree\n");
        afficherMenu(0);
    }
}

void afficherUtilisateurs()
{
    // affiche la liste des utilisateurs
    // 1. Bob
    // 2. Alice
    printf("======== UTILISATEURS ========\n");
    char *listeUsers[2] = {"sami", "clemence"};
    int n = 2;
    for (int i = 0; i < n; i++)
    {
        printf("%d. %s\n", i + 1, listeUsers[i]);
    }
}

void afficherMenuConnecte()
{
    printf("1. Jouer\n");
    printf("2. Voir parties en cours\n");
    printf("3. Voir liste utilisateurs\n");
    printf("4. Se deconnecter\n");
    int choix;
    scanf(" %d", &choix);
    switch (choix)
    {
    case 1:
        lancerJeu();
        break;
    case 2:
        lancerJeu();
        break;
    case 3:
        afficherUtilisateurs();
        break;
    case 4:
        afficherMenu(0);
        break;
    }
}

void afficherMenu(int menu)
{
    printf("============ MENU ============\n");
    switch (menu)
    {
    case 0:
        afficherAuthentification();
        break;
    case 1:
        afficherMenuConnecte();
        break;
    }
    printf("==============================\n");
}

int main()
{
    afficherMenu(0);
    return 0;
}