#include <stdio.h>

// int nord[6] = [4,4,4,4,4,4];
// int sud[6] = [4,4,4,4,4,4];

// int joueur = 0 ou 1;

struct plateau
{
    int *nord;
    int *sud;
};

int *convertis(char *lettre)
{

    printf("Appel convertis\n");
    int *res;
    switch (*lettre)
    {
    case 'a':
        res[0] = 0;
        res[1] = 0;
        break;
    case 'b':
        res[0] = 0;
        res[1] = 1;
        break;
    case 'c':
        res[0] = 0;
        res[1] = 2;
        break;
    case 'd':
        res[0] = 0;
        res[1] = 3;
        break;
    case 'e':
        res[0] = 0;
        res[1] = 4;
        break;
    case 'f':
        res[0] = 0;
        res[1] = 5;
        break;
    case 'A':
        res[0] = 1;
        res[1] = 0;
        break;
    case 'B':
        res[0] = 1;
        res[1] = 1;
        break;
    case 'C':
        res[0] = 1;
        res[1] = 2;
        break;
    case 'D':
        res[0] = 1;
        res[1] = 3;
        break;
    case 'E':
        res[0] = 1;
        res[1] = 4;
        break;
    case 'F':
        res[0] = 1;
        res[1] = 5;
        break;
    }
    return res;
}

int *concat(int *a, int *b)
{
    int* res;
    for (int i = 0; i < 6; i++)
    {
        res[i] = a[i];
    }
    for (int i = 6; i < 12; i++)
    {
        res[i] = b[i];
    }
    return res;
}

void afficherJeu(struct plateau plateau)
{

    printf("f  e  d  c  b  a \n");
    for (int i = 0; i < 6; i++)
    {
        printf("%d ", plateau.nord[5 - i]);
    }
    printf("\n");
    for (int j = 0; j < 6; j++)
    {
        printf("%d ", plateau.sud[j]);
    }
    printf("\nA  B  C  D  E  F  \n");
}

struct plateau deplacerGraines(char *coupJoue, struct plateau plateau)
// Ne jamais metttre de graines dans la case jouée
{
    printf("Appel deplacerGraines \n");

    int *coordonnees = convertis(coupJoue);

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
            printf("%d\n", i);

            if (i < 6 && i != coordonnees[1])
            {
                sud[i]++;
                nbGraines--;
            }

            if (i >= 6)
            {
                nord[i%6]++;
                nbGraines--;
            }

            i = ++i % 12;
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
            printf("%d\n", i);
            if (i < 6 && i != coordonnees[1])
            {
                nord[i]++;
                nbGraines--;
            }

            if (i >= 6)
            {
                sud[i%6]++;
                nbGraines--;
            }

            i = ++i % 12;
        }
    }

    return plateau;
}

int main()
{
    //int estTermine = 0;
    int i = 0;

    int nord[6] = {4, 4, 4, 4, 4, 4};
    int sud[6] = {4, 4, 4, 4, 4, 4};

    struct plateau plateau = {nord, sud};
    afficherJeu(plateau);

    while (i++ < 100)
    {
        char lettreJouee;
        printf("Joueur %d : ", i % 2);
        scanf(" %c", &lettreJouee);
        printf("\n");
        plateau = deplacerGraines(&lettreJouee, plateau);
        afficherJeu(plateau);
        printf("%d\n", i);
    }

    return 0;
}
