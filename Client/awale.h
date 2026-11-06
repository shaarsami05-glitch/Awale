
typedef struct 
{
    int nord[6];
    int sud[6];
    int scoreNord;
    int scoreSud;
} Plateau;

int *convertis(char *lettre);
void afficherJeu(Plateau plateau);
void afficherScore(Plateau plateau);
Plateau deplacerGraines(int *coordonnees, Plateau plateau);
int estAffame(Plateau plateau, int joueur);
void nourrirAdversaire(Plateau plateau, int joueur);
void lancerJeu();
