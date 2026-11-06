#include <stdio.h>

int connecte = 0;
// char interligne[30] = " ============================== ";


void afficherAuthentification(int choix)
{
    if (choix == 1)
    {
        char *nomUtilisateur;
        printf("============ CONNEXION ============\n");
        printf("Entrez votre nom d'utilisateur :");
        scanf(" %s", nomUtilisateur);
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
        char *nomUtilisateur;
        printf("============ INSCRIPTION ============\n");
        printf("Entrez votre nom d'utilisateur :");
        scanf(" %s", nomUtilisateur);
        // Cherche si déjà existant
        int existeDeja = 0;
        if (existeDeja)
        {
            printf("Le nom d'utilisateur est déjà pris\n");
            afficherMenu(0);
        }
        else
        {
            printf(" Inscription validee\n");
            afficherMenu(1);
        }
    }
    else{
        printf("Erreur d'entree\n");
        afficherMenu(0);
    }
} 

void afficherMenu(int menu)
{
    printf("============ MENU ============\n");
    switch (menu)
    {
    case 0:
        int choix;
        printf("1. Se connecter\n");
        printf("2. S'inscrire\n");
        scanf(" %d", &choix);
        afficherAuthentification(choix);
    case 1:
        printf("1. Jouer\n");
        printf("2. Voir parties en cours\n");
        printf("3. Voir liste utilisateurs\n");
        printf("4. Se déconnecter\n");
    }
    printf("==============================\n");
}



int main()
{
    afficherMenu(0);
    return 0;
}