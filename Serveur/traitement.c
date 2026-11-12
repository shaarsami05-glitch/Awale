#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "traitement.h"
#include "basedonnees.h"
// #include "serveur.h"

void traiter_message(Client clients[], int actual, int indice, char *buffer)
{
   Client *client = &clients[indice];
   printf("serv : message reçu du client %s : %s\n", client->name, buffer);

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
      // traitement_en_cours();
      // traiter_attente_defi(client, buffer);
      break;

   case EN_PARTIE:
      printf("EN_PARTIE\n");
      traiter_en_partie(client, buffer, clients);
      break;

   case SPECTATEUR:
      traitement_en_cours();
      // traiter_spectateur(client, buffer);
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
   }
}

void traiter_menu(Client *client, char *buffer, Client clients[], int actual)
//  printf("============ MENU ============\n");
//  printf("1. Voir utilisateurs connectes\n");
//  printf("2. Voir parties en cours\n");
//  printf("3. Chat general\n");
//  printf("4. Se deconnecter\n");
//  printf("==============================\n");
{
   printf("serv : en attente du choix du client %s\n", client->name);
   printf("serv : choix du client %s : %s\n", client->name, buffer);
   if (strcmp(buffer, "1") == 0)
   {

      client->etat = LISTE_CLIENTS;
      construire_liste_clients(client, clients, actual, buffer);
      printf("serv : %s\n", buffer);
   }
   else if (strcmp(buffer, "2") == 0)
   {
      client->etat = SPECTATEUR;
      snprintf(buffer, BUF_SIZE, "%d|%s", SPECTATEUR, "Vous êtes maintenant spectateur.");
   }
   else if (strcmp(buffer, "3") == 0)
   {
      printf("DEBUG : entrée dans le chat\n");
      client->etat = CHAT;
      snprintf(buffer, BUF_SIZE, "%d|%s", CHAT, "Bienvenue dans le chat. Entrez \"exit\" pour quitter.");
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

   snprintf(buffer, BUF_SIZE, "%d|%s", PROFIL, clients[indice].name);
}

void traiter_profil(Client *client, char *buffer, Client clients[], int indice, int actual)
{
   //  ("=========== PROFIL ===========\n");
   //  ("1. Défier\n");
   //  ("2. Ajouter en ami\n"); ou Retirer des amis
   //  ("3. Chat\n");
   //  ("0. Retour\n");
   //  ("==============================\n");
   printf("PROFIL buffer : %s\n", buffer);
   if (strcmp(buffer, "1") == 0)
   {
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

      // write_client(clients[client->adversaire].sock, buffer);
   }
   else if (strcmp(buffer, "2") == 0)
   {
      // Ajouter en ami
      strcpy(buffer, "Fonctionnalité non implémentée.");
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
      Partie *partie = initialiserPartie(client->sock, clients[client->adversaire].sock);

      // Creer une partie pour les deux joueurs
      client->etat = EN_PARTIE;
      clients[client->adversaire].etat = EN_PARTIE;

      client->partie = partie->id;
      clients[client->adversaire].partie = partie->id;

      // Sérialiser le plateau
      serializerPlateau(partie->plateau, buffer);
      // char message[100];
      // snprintf(message, sizeof(message), "%d|%d|%s \n", EN_PARTIE, 0, buffer);
      // strcpy(buffer, message);
      // send(clients[client->adversaire].sock, buffer, strlen(buffer), 0);

      // Affiche le joueur qui doit jouer
      // if (partie->joueur)
      // { // Sud joue
      //    char msg[100];
      //    snprintf(msg, sizeof(msg), "%d|%d|%s \n", EN_PARTIE, 1, "Joueur SUD ! C'est à vous de jouer !");
      //    send(partie->joueurSud, msg, strlen(msg), 0);

      //    snprintf(msg, sizeof(msg), "%d|%d|%s \n", EN_PARTIE, 1, "Joueur NORD !");
      //    send(partie->joueurNord, msg, strlen(msg), 0);
      // }
      // else
      // { // Nord joue
      //    char msg[100];
      //    snprintf(msg, sizeof(msg), "%d|%d|%s \n", EN_PARTIE, 1, "Joueur NORD ! C'est à vous de jouer !");
      //    send(partie->joueurNord, msg, strlen(msg), 0);

      //    snprintf(msg, sizeof(msg), "%d|%d|%s \n", EN_PARTIE, 1, "Joueur SUD !");
      //    send(partie->joueurSud, msg, strlen(msg), 0);
      // }

      envoyerPlateau(partie->joueurSud, buffer, partie->joueur == 1, 0); // 1 si c'est à Sud

      char msg[512];
      snprintf(msg, sizeof(msg), "%d|%d|%d|%s\n", EN_PARTIE, 0, partie->joueur == 0, buffer);
      strcpy(buffer, msg);

      printf("Statut client  A : %d\n", client->etat);
      printf("Statut client  B : %d\n", clients[client->adversaire].etat);
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
}

void traiter_en_partie(Client *client, char *buffer, Client clients[])
{
   printf("traiter_en_partie : %s\n", buffer);
   Partie *partie = getPartieParId(client->partie);
   if (partie == NULL)
   {
      printf("Partie %d introuvable\n", client->partie);
      return;
   }

   printf("partie : %d\n", partie->id);

   int joueur;
   if (client->sock == partie->joueurNord)
   {
      joueur = 0; // Joueur Nord
   }
   else
   {
      joueur = 1; // Joueur Sud
   }

   // Recevoir un coup, analyser sa validité, jouer le coup, renvoyer le plateau
   int erreur = 0;
   jouerCoup(partie, buffer, joueur, &erreur);

   if (!partie->statut)
   { // Partie Terminee
      // Ajouter status fin de partie ? Rejouer...
      client->etat = MENU;
      clients[client->adversaire].etat = MENU;

      construire_menu(buffer);

      char message[BUF_SIZE];
      construire_menu(message);

      send(clients[client->adversaire].sock,
           message, strlen(message), 0);

      return;
   }

   //  Erreur de saisie
   if (erreur)
   {
      // char message[100];
      // snprintf(message, sizeof(message), "%d|%d|%s \n", EN_PARTIE, 1, buffer);
      // strcpy(buffer, message);
      envoyerPlateau(client->sock, buffer, 1, 1); // 1 si erreur et 1 si doitJouer
      return;
   }

   // Affiche le joueur qui doit jouer
   // int doitJouer;
   // if (partie->joueur && joueur)
   // { // Sud joue
   //    doitJouer = 1;
   // }
   // else
   // { // Nord joue
   //    char msg[100];
   //    snprintf(msg, sizeof(msg), "%d|%d|%s \n", EN_PARTIE, 1, "C'est à vous de jouer !");
   //    send(partie->joueurNord, msg, strlen(msg), 0);
   // }

   // serializerPlateau(partie->plateau, buffer);

   // char message[100];
   // snprintf(message, sizeof(message), "%d|%d|%d|%s \n", EN_PARTIE, 0, doitJouer, buffer);
   // strcpy(buffer, message);
   // // printf("FIN traiter_en_partie : %s\n", buffer);

   // send(clients[client->adversaire].sock, buffer, strlen(buffer), 0);
   // send(client->sock, buffer, strlen(buffer), 0);

   serializerPlateau(partie->plateau, buffer); // contient seulement "4,4,4,4,4,4|4,4,4,4,4,4|0|0"

   if (joueur)
      envoyerPlateau(partie->joueurNord, buffer, partie->joueur == 0, 0); // 1 si c'est à Nord
   else
      envoyerPlateau(partie->joueurSud, buffer, partie->joueur == 1, 0); // 1 si c'est à Sud

   // Envoyer le plateau aux Spectateurs de la partie
   // TODO
}

void traitement_en_cours(void)
{
   printf("La fonction n'a pas encore été implémentée.\n");
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
      if (sender.sock != clients[i].sock &&
          clients[i].etat == CHAT)
      {
         message[0] = '\0';

         if (from_server == 0)
         {
            snprintf(message, sizeof(message),
                     "%s : %s", sender.name, buffer);
         }
         else
         {
            snprintf(message, sizeof(message),
                     "%s %s", sender.name, buffer);
         }

         send(clients[i].sock, message,
              strlen(message), 0);
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

   // tant qu'on a pas ecrit exit, on reste dans le chat privé
   snprintf(buffer, BUF_SIZE, "%d|%s", CHAT_PRIVE, "Message envoyé.");
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

      snprintf(ligne, sizeof(ligne), "%d. %s\n", numero++, clients[i].name);

      strncat(buffer, ligne, BUF_SIZE - strlen(buffer) - 1);
   }
}