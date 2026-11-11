#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "basedonnees.h"

Partie parties[100];
int nbPartie = 0;

Partie creerPartie()
{
    Partie partie;
    partie.id = nbPartie;
    parties[nbPartie++] = partie;
    return partie;
}

Partie getPartieParId(int id)
{
    for (int i = 0; i < nbPartie; i++){
        if(parties[i].id == id){
            return parties[i];
        }
    }
}