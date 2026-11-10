#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "traitement.h"
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
      traiter_liste_clients(client, buffer, clients);
      break;

   case PROFIL:
      traiter_profil(client, buffer, clients, indice);
      break;

   case EN_ATTENTE_DEFI:
      // Rien à faire
      // traitement_en_cours();
      // traiter_attente_defi(client, buffer);
      break;

   case EN_PARTIE:
      traitement_en_cours();
      // traiter_partie(client, buffer);
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
      // printf("je suis rentré dans le if\n");
      client->etat = LISTE_CLIENTS;
      strcpy(buffer, "2|");
      // Envoyer la liste des clients connectés
      for (int i = 0; i < actual; i++)
      {
         char str[100];
         snprintf(str, sizeof(str), "%d. %s | ", i + 1, clients[i].name);
         strcat(buffer, str);
      }
      printf("serv : %s\n", buffer);
   }
   else if (strcmp(buffer, "2") == 0)
   {
      client->etat = SPECTATEUR;
      strcpy(buffer, "Vous êtes maintenant spectateur.");
   }
   else if (strcmp(buffer, "3") == 0)
   {
      printf("DEBUG : entrée dans le chat\n");
      client->etat = CHAT;
      strcpy(buffer, "Bienvenue dans le chat. Entrez \"exit\" pour quitter.");
   }
   else
   {
      strcpy(buffer, "Choix invalide.");
   }
}

void traiter_liste_clients(Client *client, char *buffer, Client clients[])
//"======== UTILISATEURS ========\n");
// 1. Alice
// 2. Bob
// 0. Retour
// "==============================\n");
{
   if (strcmp(buffer, "0") == 0)
   {
      // strcpy(buffer, "");
      client->etat = MENU;
      char str[100];
      snprintf(str, sizeof(str), "%d|%s \n", MENU, "Vous êtes de retour au menu principal.");
      strcpy(buffer, str);
   }
   else
   {
      client->etat = PROFIL;
      // Afficher le profil du client choisi

      client->adversaire = atoi(buffer) - 1; // stocker utilisateur selectionne
      char str[100];
      snprintf(str, sizeof(str), "%d|%s \n", PROFIL, clients[atoi(buffer) - 1].name);
      strcpy(buffer, str);
      printf("buffer : %s\n", buffer);
   }
}

void traiter_profil(Client *client, char *buffer, Client clients[], int indice)
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
}

void traiter_proposition_defi(Client *client, char *buffer, Client clients[])
{
   // ("======== DEFI PROPOSE ========\n");
   // ("1. Accepter\n");
   // ("2. Décliner\n");
   if (strcmp(buffer, "1") == 0)
   {
      // Creer une partie pour les deux joueurs
      client->etat = EN_PARTIE;
      clients[client->adversaire].etat = EN_PARTIE;

      char str[100];
      snprintf(str, sizeof(str), "%d|%s \n", EN_PARTIE, "Début de la partie...");
      strcpy(buffer, str);

      send(clients[client->adversaire].sock, buffer, strlen(buffer), 0);
   }
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
      char str[100];
      snprintf(str, sizeof(str), "%d|%s \n", MENU, "Vous avez quitté le chat.");
      strcpy(buffer, str);
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

   for (i = 0 ; i < actual; i++)
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