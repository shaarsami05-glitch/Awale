#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "traitement.h"

void traiter_message(Client clients[], int actual, int indice, char *buffer)
// gerer la saisie du client selon son etat
{
   Client *client = &clients[indice];

   switch (client->etat)
   {
   case MENU:
      traiter_menu(client, buffer, clients, actual);
      break;

   case LISTE_CLIENTS:
      traiter_liste_clients(client, buffer, clients, actual);
      break;

   case PROFIL:
      traiter_profil(client, buffer, clients, indice, actual);
      break;

   case EN_ATTENTE_DEFI:
      // Rien à faire
      break;

   case EN_PARTIE:
      traiter_en_partie(client, buffer, clients, actual);
      break;

   case MENU_SPECTATEUR:
      traiter_menu_spectateur(client, buffer);
      break;

   case SPECTATEUR:
      traiter_spectateur(client, buffer);
      break;

   case CHAT:
      traiter_chat(client, buffer, clients, actual);
      break;

   case PROPOSITION_DEFI:
      traiter_proposition_defi(client, buffer, clients);
      break;

   case CHAT_PRIVE:
      traiter_chat_prive(client, buffer, clients, actual);
      break;

   case SELF_PROFIL:
      traiter_self_profil(client, buffer);
      break;

   case BIO:
      traiter_bio(client, buffer);
      break;

   case AMIS:
      retourSelfProfil(client, buffer);
      break;

   case HISTORIQUE:
      traiter_historique(client, buffer);
      break;

   case REPLAY:
      traiter_replay(client, buffer);
      break;

   case FIN_PARTIE:
      // Rien à faire
      break;

   case CLASSEMENT:
      traiter_classement(client, buffer);
      break;
   }
}

void traiter_menu(Client *client, char *buffer, Client clients[], int actual)
//  printf("============ MENU ============\n");
//  printf("1. Voir utilisateurs connectes\n");
//  printf("2. Voir parties en cours\n");
//  printf("3. Chat general\n");
//  printf("4. Profil\n");
//  printf("5. Classement\n");
//  printf("==============================\n");
{
   if (strcmp(buffer, "1") == 0)
   {

      client->etat = LISTE_CLIENTS;
      construire_liste_clients(client, clients, actual, buffer);
   }
   else if (strcmp(buffer, "2") == 0)
   {
      snprintf(buffer, BUF_SIZE, "%d|", MENU_SPECTATEUR);
      client->etat = MENU_SPECTATEUR;
      Partie *partiesEnCours[100];
      int nbEnCours = getPartiesEnCours(partiesEnCours);
      for (int i = 0; i < nbEnCours; i++)
      {
         int amiNord = 1, amiSud = 1;
         if (partiesEnCours[i]->joueurNord->prive)
         {
            amiNord = estAmi(partiesEnCours[i]->joueurNord, client->name);
         }
         if (partiesEnCours[i]->joueurSud->prive)
         {
            amiSud = estAmi(partiesEnCours[i]->joueurSud, client->name);
         }
         if (amiNord && amiSud)
         {
            char ligne[100];
            snprintf(ligne, sizeof(ligne), "%d. %s VS %s |",
                     partiesEnCours[i]->id + 1,
                     partiesEnCours[i]->joueurNord->name,
                     partiesEnCours[i]->joueurSud->name);
            strncat(buffer, ligne, BUF_SIZE - strlen(buffer) - 1);
         }
      }
   }
   else if (strcmp(buffer, "3") == 0)
   {
      client->etat = CHAT;
      snprintf(buffer, BUF_SIZE, "%d|%s", CHAT, "Bienvenue dans le chat. Entrez \"exit\" pour quitter.");
   }
   else if (strcmp(buffer, "4") == 0)
   {
      client->etat = SELF_PROFIL;
      snprintf(buffer, BUF_SIZE, "%d|%d|%s", SELF_PROFIL, client->prive, client->bio);
   }
   else if (strcmp(buffer, "5") == 0)
   {
      client->etat = CLASSEMENT;
      snprintf(buffer, BUF_SIZE, "%d|", CLASSEMENT);

      StatsJoueur classement[MAX_CLIENTS];
      int nb = calculerClassement(classement, MAX_CLIENTS);

      for (int i = 0; i < nb; i++)
      {
         char ligne[128];
         // Format par joueur : rang;nom;points;V;N;D;graines|
         snprintf(ligne, sizeof(ligne), "%d;%s;%d;%d;%d;%d;%d|",
                  i + 1,
                  classement[i].nom,
                  classement[i].points,
                  classement[i].victoires,
                  classement[i].egalites,
                  classement[i].defaites,
                  classement[i].grainesTotal);
         strncat(buffer, ligne, BUF_SIZE - strlen(buffer) - 1);
      }
   }
   else
   {
      strcpy(buffer, "Choix invalide.\n");
   }
}

void traiter_liste_clients(Client *client, char *buffer, Client clients[], int actual)
//"======== UTILISATEURS ========\n");
// 1. Alice
// 2. Bob
// 0. Retour
// "==============================\n");
{
   // retour au menu principal
   if (strcmp(buffer, "0") == 0)
   {
      client->etat = MENU;
      construire_menu(buffer);
      return;
   }

   int numero = atoi(buffer);

   // checker le choix du client
   if (numero <= 0)
   {
      strcpy(buffer, "Choix invalide.\n");
      return;
   }

   int compteur = 0;
   int indice = -1;

   // retenir le bon indice de la personne choisie par le client
   for (int i = 0; i < actual; i++)
   {
      if (clients[i].sock == client->sock)
         continue;

      compteur++;

      if (compteur == numero)
      {
         indice = i;
         break;
      }
   }

   if (indice == -1)
   {
      strcpy(buffer, "Choix invalide.\n");
      return;
   }

   client->adversaire = indice;
   client->etat = PROFIL;

   snprintf(buffer, BUF_SIZE, "%d|%d|%s|%s", PROFIL, estAmi(client, clients[indice].name), clients[indice].name, clients[indice].bio);
}

void traiter_profil(Client *client, char *buffer, Client clients[], int indice, int actual)
{
   //  ("=========== PROFIL ===========\n");
   //  ("1. Défier\n");
   //  ("2. Ajouter en ami\n"); ou Retirer des amis
   //  ("3. Chat\n");
   //  ("0. Retour\n");
   //  ("==============================\n");
   if (strcmp(buffer, "1") == 0)
   {
      if (clients[client->adversaire].etat == EN_PARTIE || clients[client->adversaire].etat == PROPOSITION_DEFI || clients[client->adversaire].etat == EN_ATTENTE_DEFI)
      {
         // Ne peut pas proposer une partie à qqn qui est déjà en train de jouer
         strcpy(buffer, "Utilisateur non disponible");
         return;
      }
      // Envoyer une invitation de partie au client
      client->etat = EN_ATTENTE_DEFI;
      if (client->adversaire < 0 || client->adversaire >= actual)
      {
         snprintf(buffer, BUF_SIZE, "%d|Adversaire invalide.", PROFIL);
         return;
      }
      clients[client->adversaire].etat = PROPOSITION_DEFI;

      // Je suis l'adversaire de mon adversaire
      clients[client->adversaire].adversaire = indice;

      char str[100];
      snprintf(str, sizeof(str), "%d|%s \n", EN_ATTENTE_DEFI, clients[client->adversaire].name);
      strcpy(buffer, str);

      char message[100];
      snprintf(message, sizeof(message), "%d|%s \n", PROPOSITION_DEFI, client->name);
      send(clients[client->adversaire].sock, message, strlen(message), 0);
   }
   else if (strcmp(buffer, "2") == 0)
   {
      if (estAmi(client, clients[client->adversaire].name))
      {
         // Retirer des amis
         int retire = retirerAmi(client, clients[client->adversaire].name);
         client->etat = MENU;
         snprintf(buffer, BUF_SIZE, "%d|%s", client->etat, retire ? "Ami retiré !" : "Probleme dans la suppression");
      }
      else
      {
         // Ajouter aux amis
         int ajoute = ajouterAmi(client, clients[client->adversaire].name);
         client->etat = MENU;
         snprintf(buffer, BUF_SIZE, "%d|%s", client->etat, ajoute ? "Ami ajouté !" : "Probleme dans l'ajout");
      }
   }
   else if (strcmp(buffer, "3") == 0)
   {
      // Chat avec le client
      client->etat = CHAT_PRIVE;
      snprintf(buffer, BUF_SIZE, "%d|%s", client->etat, "Entrez votre message ou 'exit' pour quitter.");
   }
   else if (strcmp(buffer, "0") == 0)
   {
      client->etat = LISTE_CLIENTS;

      construire_liste_clients(client, clients, actual, buffer);
   }
   else
   {
      strcpy(buffer, "Choix invalide.\n");
   }
}

static void envoyerPlateau(int sock, const char *plateauStr, int doitJouer, int erreur)
{
   char msg[512];
   snprintf(msg, sizeof(msg), "%d|%d|%d|%s\n", EN_PARTIE, erreur, doitJouer, plateauStr);
   send(sock, msg, strlen(msg), 0);
}

void traiter_proposition_defi(Client *client, char *buffer, Client clients[])
{
   // ("======== DEFI PROPOSE ========\n");
   // ("1. Accepter\n");
   // ("2. Décliner\n");
   if (strcmp(buffer, "1") == 0)
   {
      // Créer une partie
      Partie *partie = initialiserPartie(client, &clients[client->adversaire]);

      // Creer une partie pour les deux joueurs
      client->etat = EN_PARTIE;
      clients[client->adversaire].etat = EN_PARTIE;

      client->partie = partie->id;
      clients[client->adversaire].partie = partie->id;

      // Sérialiser le plateau
      serializerPlateau(partie->plateau, partie->joueurNord->name, partie->joueurSud->name, buffer);

      envoyerPlateau(partie->joueurSud->sock, buffer, partie->joueur == 1, 0); // 1 si c'est à Sud

      // Afficher le nom du joueur au lieu de Joueur 1
      char msg[512];
      snprintf(msg, sizeof(msg), "%d|%d|%d|%s\n", EN_PARTIE, 0, partie->joueur == 0, buffer);
      strcpy(buffer, msg);
   }
   else if (strcmp(buffer, "2") == 0)
   {
      // Proposition déclinée
      // Retour au menu pour les deux joueurs
      client->etat = MENU;
      clients[client->adversaire].etat = MENU;

      char str[100];
      snprintf(str, sizeof(str), "%d|%s \n", client->etat, "Défi refusé.");
      strcpy(buffer, str);

      send(clients[client->adversaire].sock, buffer, strlen(buffer), 0);
   }
   else
   {
      strcpy(buffer, "Choix invalide.\n");
   }
}

void traiter_en_partie(Client *client, char *buffer, Client clients[], int actual)
// verifier si coup, message, exit et sauvegarde de la partie pr chaque bon coup
{
   Partie *partie = getPartieParId(client->partie);
   if (partie == NULL)
   {
      printf("Partie %d introuvable\n", client->partie);
      return;
   }

   int joueur;
   if (client->sock == partie->joueurNord->sock)
   {
      joueur = 0; // Joueur Nord
   }
   else
   {
      joueur = 1; // Joueur Sud
   }

   // Vérifier si le joueur souhaite envoyer un message
   char *separateur = strchr(buffer, '|');
   if (separateur != NULL)
   {
      // Récupérer le type
      *separateur = '\0';
      char *type = buffer;

      if (strcmp(type, "MESSAGE") == 0)
      {
         // Récupérer toutes les données après le séparateur
         char *message = separateur + 1;
         printf("Message : %s\n", message);

         // Envoyer le message à l'adversaire
         Client *adversaire;
         if (joueur == 0)
            adversaire = partie->joueurSud;
         else
            adversaire = partie->joueurNord;

         char msg[BUF_SIZE];
         snprintf(msg, sizeof(msg), "9|%s : %s", client->name, message);
         send(adversaire->sock, msg, strlen(msg), 0);

         return;
      }
      else if (strcmp(type, "EXIT") == 0)
      {
         Client *adversaire = (joueur == 0) ? partie->joueurSud : partie->joueurNord;

         // L'adversaire gagne par forfait
         strcpy(partie->gagnant, adversaire->name);
         partie->statut = TERMINEE;
         sauvegarderPartieFichier(partie);

         char msg[BUF_SIZE];
         snprintf(msg, sizeof(msg), "%d|%s a quitte la partie (forfait). Retour au menu.", MENU, client->name);
         send(adversaire->sock, msg, strlen(msg), 0);

         // Retour au menu principal
         client->etat = MENU;
         adversaire->etat = MENU;

         construire_menu(buffer);
         return;
      }
      else
      {
         printf("Commande de message inconnu : %s\n", type);
      }
   }

   // Sinon, il s'agit d'un coup normal
   char coupJoue = buffer[0];
   int erreur = 0;
   jouerCoup(partie, buffer, joueur, &erreur);

   if (erreur)
   {
      envoyerPlateau(client->sock, buffer, 1, 1);
      buffer[0] = '\0';
      return;
   }

   // coup valide, on l'enregistre dans l'historique
   if (partie->nbCoups < MAX_COUPS - 1)
   {
      partie->coups[partie->nbCoups++] = coupJoue;
      partie->coups[partie->nbCoups] = '\0';
   }

   char plateauStr[256];
   serializerPlateau(partie->plateau, partie->nomNord, partie->nomSud, plateauStr);

   // Cas 1 : La partie vient de se terminer
   if (partie->statut == TERMINEE)
   {
      sauvegarderPartieFichier(partie);
      Client *adversaire = (joueur == 0) ? partie->joueurSud : partie->joueurNord;

      // Notifier les spectateurs et les remettre au MENU
      for (int i = 0; i < actual; i++)
      {
         if (clients[i].etat == SPECTATEUR && clients[i].partie == partie->id)
         {
            char msgSpec[BUF_SIZE];
            snprintf(msgSpec, sizeof(msgSpec), "16|%s|%s", partie->gagnant, plateauStr);
            send(clients[i].sock, msgSpec, strlen(msgSpec), 0);
            clients[i].etat = MENU;
         }
      }

      // Envoyer l'écran de fin de partie à l'adversaire
      char msgAdv[BUF_SIZE];
      snprintf(msgAdv, sizeof(msgAdv), "16|%s|%s", partie->gagnant, plateauStr);
      send(adversaire->sock, msgAdv, strlen(msgAdv), 0);

      // Remettre les deux joueurs au MENU
      client->etat = MENU;
      adversaire->etat = MENU;

      // Envoyer l'écran de fin de partie au joueur qui vient de jouer
      snprintf(buffer, BUF_SIZE, "16|%s|%s", partie->gagnant, plateauStr);
      return;
   }

   // Cas 2 : La partie continue
   for (int i = 0; i < actual; i++)
   {
      if (clients[i].etat == SPECTATEUR && clients[i].partie == partie->id)
      {
         char msgSpec[BUF_SIZE];
         snprintf(msgSpec, sizeof(msgSpec), "%d|%s", SPECTATEUR, plateauStr);
         send(clients[i].sock, msgSpec, strlen(msgSpec), 0);
      }
   }

   if (joueur)
      envoyerPlateau(partie->joueurNord->sock, plateauStr, partie->joueur == 0, 0);
   else
      envoyerPlateau(partie->joueurSud->sock, plateauStr, partie->joueur == 1, 0);

   snprintf(buffer, BUF_SIZE, "%d|%d|%d|%s", EN_PARTIE, 0, 0, plateauStr);
}

void traiter_chat(Client *client, char *buffer, Client clients[], int actual)
{
   // si le message est exit on quiite le chat, sinon on envoie le message à tous les clients connectés
   if (strcmp(buffer, "exit") == 0)
   {

      send_message_to_all_clients(clients, *client, actual, "a quitté le chat !", 1);
      client->etat = MENU;
      construire_menu(buffer);
   }
   else
   {
      send_message_to_all_clients(clients, *client, actual, buffer, 0);
      buffer[0] = '\0';
   }
}

void send_message_to_all_clients(Client *clients, Client sender, int actual, const char *buffer, char from_server)
{
   int i = 0;
   char message[BUF_SIZE];
   message[0] = 0;

   for (i = 0; i < actual; i++)
   {
      /* we don't send message to the sender ou s'il est dans le chat*/
      if (sender.sock != clients[i].sock && clients[i].etat == CHAT)
      {
         message[0] = '\0';

         if (from_server == 0)
         {
            snprintf(message, sizeof(message), "%s : %s", sender.name, buffer);
         }
         else
         {
            snprintf(message, sizeof(message), "%s %s", sender.name, buffer);
         }

         send(clients[i].sock, message, strlen(message), 0);
      }
   }
}

void traiter_chat_prive(Client *client, char *buffer, Client clients[], int actual)
{
   // Quitter le chat privé
   if (strcmp(buffer, "exit") == 0)
   {
      client->etat = LISTE_CLIENTS;

      construire_liste_clients(client, clients, actual, buffer);
      return;
   }

   // Vérifier que le destinataire est valide
   if (client->adversaire < 0 || client->adversaire >= actual)
   {
      strcpy(buffer, "Destinataire invalide.");
      return;
   }

   Client *destinataire = &clients[client->adversaire];

   // Construire le message
   char message[BUF_SIZE];

   snprintf(message, sizeof(message), "%d|%s : %s", client->etat, client->name, buffer);

   // Envoyer une seule fois
   if (send(destinataire->sock, message, strlen(message), 0) < 0)
   {
      perror("send");
      strcpy(buffer, "Erreur lors de l'envoi.");
      return;
   }
   buffer[0] = '\0'; // remettre buffer vide

   // tant qu'on a pas ecrit exit, on reste dans le chat privé
}

void construire_menu(char *buffer)
{
   snprintf(buffer, BUF_SIZE, "%d|Bienvenue dans le menu.", MENU);
}

void construire_liste_clients(Client *client, Client clients[], int actual, char *buffer)
{
   snprintf(buffer, BUF_SIZE, "%d|", LISTE_CLIENTS);

   int numero = 1;

   for (int i = 0; i < actual; i++)
   {
      if (clients[i].sock == client->sock)
         continue;

      char ligne[100];
      snprintf(ligne, sizeof(ligne), "%d. %s |", numero++, clients[i].name);
      strncat(buffer, ligne, BUF_SIZE - strlen(buffer) - 1);
   }
}

void traiter_menu_spectateur(Client *client, char *buffer)
// ====== PARTIES EN COURS ======
// Choisissez une partie à regarder
// 1. Alice VS Bob
// 0. Retour
// ==============================
{
   if (strcmp(buffer, "0") == 0)
   {
      client->etat = MENU;
      construire_menu(buffer);
      return;
   }
   else
   {
      int id = atoi(buffer) - 1;
      Partie *p = getPartieParId(id); // la version qui renvoie un pointeur ou NULL

      if (p == NULL || p->statut != EN_COURS)
      {
         strcpy(buffer, "Choix invalide.");
         return;
      }
      client->partie = p->id;
      client->etat = SPECTATEUR;
      serializerPlateau(p->plateau, p->joueurNord->name, p->joueurSud->name, buffer); // contient seulement "4,4,4,4,4,4|4,4,4,4,4,4|0|0"
      char msg[100];
      snprintf(msg, sizeof(msg), "%d|%s", SPECTATEUR, buffer);
      strcpy(buffer, msg);
   }
}

void traiter_spectateur(Client *client, char *buffer)
{ // Si t'écris 0 tu reviens au MENU
   if (strcmp(buffer, "0") == 0)
   {
      client->etat = MENU;
      construire_menu(buffer);
      return;
   }
   else
   {
      strcpy(buffer, "Commande invalide.\n");
   }
}

void traiter_self_profil(Client *client, char *buffer)
{ // Si t'écris 0 tu reviens au MENU
   if (strcmp(buffer, "0") == 0)
   {
      client->etat = MENU;
      construire_menu(buffer);
      return;
   }
   else if (strcmp(buffer, "1") == 0)
   {
      client->etat = BIO;
      snprintf(buffer, BUF_SIZE, "%d|Nouvelle bio :", CHAT_PRIVE);
      return;
   }
   else if (strcmp(buffer, "2") == 0)
   {
      client->etat = AMIS;
      snprintf(buffer, BUF_SIZE, "%d|", AMIS);
      for (int i = 0; i < client->nbAmis; i++)
      {
         char ligne[100];
         snprintf(ligne, sizeof(ligne), "- %s |", client->amis[i]);
         strncat(buffer, ligne, BUF_SIZE - strlen(buffer) - 1);
      }
      return;
   }
   else if (strcmp(buffer, "3") == 0)
   {
      char msg[100];
      if (client->prive)
      {
         client->prive = 0;
         strcpy(msg, "Vous êtes désormais en PUBLIC, tout le monde peut voir vos parties");
      }
      else
      {
         client->prive = 1;
         strcpy(msg, "Vous êtes désormais en PRIVE, seuls vos amis peuvent voir vos parties");
      }
      client->etat = MENU;
      snprintf(buffer, BUF_SIZE, "%d|%s", MENU, msg);
      return;
   }
   else if (strcmp(buffer, "4") == 0)
   {
      client->etat = HISTORIQUE;
      snprintf(buffer, BUF_SIZE, "%d|", HISTORIQUE);

      Partie *partiesJoueur[MAX_PARTIES];
      int nb = getPartiesTermineesJoueur(client->name, partiesJoueur);

      for (int i = 0; i < nb; i++)
      {
         char ligne[128];
         snprintf(ligne, sizeof(ligne), "%d. %s (%d) VS %s (%d) |",
                  i + 1,
                  partiesJoueur[i]->nomNord, partiesJoueur[i]->plateau.scoreNord,
                  partiesJoueur[i]->nomSud, partiesJoueur[i]->plateau.scoreSud);
         strncat(buffer, ligne, BUF_SIZE - strlen(buffer) - 1);
      }
      return;
   }
   else
   {
      strcpy(buffer, "Choix invalide.\n");
   }
}

void traiter_bio(Client *client, char *buffer)
{
   strcpy(client->bio, buffer);
   client->etat = MENU;
   snprintf(buffer, BUF_SIZE, "%d|Bio modifiée !", MENU);
}

int estAmi(const Client *c, const char *nom)
// renvoie 1 si ami, 0 sinon
{
   for (int i = 0; i < c->nbAmis; i++)
      if (strcmp(c->amis[i], nom) == 0)
         return 1;
   return 0;
}

int ajouterAmi(Client *c, const char *nom)
// renvoie 0 si pas possible, 1 sinon
{
   if (c->nbAmis >= MAX_AMIS || estAmi(c, nom))
      return 0;
   strncpy(c->amis[c->nbAmis], nom, sizeof(c->amis[0]) - 1);
   c->amis[c->nbAmis][sizeof(c->amis[0]) - 1] = '\0';
   c->nbAmis++;
   return 1;
}

int retirerAmi(Client *c, const char *nom)
{
   for (int i = 0; i < c->nbAmis; i++)
   {
      if (strcmp(c->amis[i], nom) == 0)
      {
         // Décaler les amis suivants d'une case vers la gauche
         for (int j = i; j < c->nbAmis - 1; j++)
            strcpy(c->amis[j], c->amis[j + 1]);

         c->nbAmis--;
         return 1; // retiré
      }
   }
   return 0; // pas trouvé
}

void retourSelfProfil(Client *client, char *buffer)
{
   if (strcmp(buffer, "0") == 0)
   {
      client->etat = SELF_PROFIL;
      snprintf(buffer, BUF_SIZE, "%d|%d|%s", SELF_PROFIL, client->prive, client->bio);
   }
}

static void construireMessageReplay(Client *client, const Partie *p, char *buffer)
// gerer le replay d'une partie
{
   Plateau plateauEtape = construirePlateauEtape(p, client->replayEtape);
   char plateauStr[256];
   serializerPlateau(plateauEtape, (char *)p->nomNord, (char *)p->nomSud, plateauStr);

   char coupActuel = (client->replayEtape == 0) ? '-' : p->coups[client->replayEtape - 1];

   snprintf(buffer, BUF_SIZE, "%d|%d|%d|%c|%s", REPLAY, client->replayEtape, p->nbCoups, coupActuel, plateauStr);
}

void traiter_historique(Client *client, char *buffer)
{
   if (strcmp(buffer, "0") == 0)
   {
      client->etat = SELF_PROFIL;
      snprintf(buffer, BUF_SIZE, "%d|%d|%s", SELF_PROFIL, client->prive, client->bio);
      return;
   }

   int choix = atoi(buffer);
   Partie *partiesJoueur[MAX_PARTIES];
   int nb = getPartiesTermineesJoueur(client->name, partiesJoueur);

   if (choix <= 0 || choix > nb)
   {
      strcpy(buffer, "Choix invalide.\n");
      return;
   }

   Partie *p = partiesJoueur[choix - 1];
   client->etat = REPLAY;
   client->replayPartieId = p->id;
   client->replayEtape = 0; // On commence au plateau initial (coup 0)

   construireMessageReplay(client, p, buffer);
}

void traiter_replay(Client *client, char *buffer)
{
   Partie *p = getPartieParId(client->replayPartieId);
   if (p == NULL)
   {
      client->etat = MENU;
      construire_menu(buffer);
      return;
   }

   if (strcmp(buffer, "0") == 0)
   {
      // Retour à la liste de l'historique des parties
      client->etat = HISTORIQUE;
      snprintf(buffer, BUF_SIZE, "%d|", HISTORIQUE);

      Partie *partiesJoueur[MAX_PARTIES];
      int nb = getPartiesTermineesJoueur(client->name, partiesJoueur);

      for (int i = 0; i < nb; i++)
      {
         char ligne[128];
         snprintf(ligne, sizeof(ligne), "%d. %s (%d) VS %s (%d) |",
                  i + 1,
                  partiesJoueur[i]->nomNord, partiesJoueur[i]->plateau.scoreNord,
                  partiesJoueur[i]->nomSud, partiesJoueur[i]->plateau.scoreSud);
         strncat(buffer, ligne, BUF_SIZE - strlen(buffer) - 1);
      }

      return;
   }
   else if (strcmp(buffer, "n") == 0 || strcmp(buffer, "N") == 0)
   {
      if (client->replayEtape < p->nbCoups)
         client->replayEtape++;
   }
   else if (strcmp(buffer, "p") == 0 || strcmp(buffer, "P") == 0)
   {
      if (client->replayEtape > 0)
         client->replayEtape--;
   }

   construireMessageReplay(client, p, buffer);
}

void traiter_classement(Client *client, char *buffer)
{
   if (strcmp(buffer, "0") == 0)
   {
      client->etat = MENU;
      construire_menu(buffer);
   }
}