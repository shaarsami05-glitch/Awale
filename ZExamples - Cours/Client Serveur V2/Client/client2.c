#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "client2.h"

static void init(void)
{
#ifdef WIN32
   WSADATA wsa;
   int err = WSAStartup(MAKEWORD(2, 2), &wsa);
   if (err < 0)
   {
      puts("WSAStartup failed !");
      exit(EXIT_FAILURE);
   }
#endif
}

static void end(void)
{
#ifdef WIN32
   WSACleanup();
#endif
}

static void app(const char *address, const char *name)
{
   SOCKET sock = init_connection(address);
   char buffer[BUF_SIZE];

   fd_set rdfs;

   /* send our name */
   write_server(sock, name);
   afficherMenu(1, sock); // 0 pour menu de connexion, 1 pour menu connecté

   while (1)
   {
      FD_ZERO(&rdfs);

      /* add STDIN_FILENO */
      FD_SET(STDIN_FILENO, &rdfs);

      /* add the socket */
      FD_SET(sock, &rdfs);

      if (select(sock + 1, &rdfs, NULL, NULL, NULL) == -1)
      {
         perror("select()");
         exit(errno);
      }

      /* something from standard input : i.e keyboard */
      // quand on tape sur le clavier, on envoie le message au serveur
      if (FD_ISSET(STDIN_FILENO, &rdfs))
      {
         fgets(buffer, BUF_SIZE - 1, stdin);
         {
            char *p = NULL;
            p = strstr(buffer, "\n");
            if (p != NULL)
            {
               *p = 0;
            }
            else
            {
               /* fclean */
               buffer[BUF_SIZE - 1] = 0;
            }
         }
         write_server(sock, buffer);
      }
      // lorsque le serveur envoie un message, on l'affiche
      else if (FD_ISSET(sock, &rdfs))
      {
         int n = read_server(sock, buffer);
         /* si server down */
         if (n == 0)
         {
            printf("Server disconnected !\n");
            break;
         }
         puts(buffer);
      }
   }

   end_connection(sock);
}

static int init_connection(const char *address)
{
   SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
   SOCKADDR_IN sin = {0};
   struct hostent *hostinfo;

   if (sock == INVALID_SOCKET)
   {
      perror("socket()");
      exit(errno);
   }

   hostinfo = gethostbyname(address);
   if (hostinfo == NULL)
   {
      fprintf(stderr, "Unknown host %s.\n", address);
      exit(EXIT_FAILURE);
   }

   sin.sin_addr = *(IN_ADDR *)hostinfo->h_addr;
   sin.sin_port = htons(PORT);
   sin.sin_family = AF_INET;

   if (connect(sock, (SOCKADDR *)&sin, sizeof(SOCKADDR)) == SOCKET_ERROR)
   {
      perror("connect()");
      exit(errno);
   }

   return sock;
}

static void end_connection(int sock)
{
   closesocket(sock);
}

static int read_server(SOCKET sock, char *buffer)
{
   int n = 0;

   if ((n = recv(sock, buffer, BUF_SIZE - 1, 0)) < 0)
   {
      perror("recv()");
      exit(errno);
   }

   buffer[n] = 0;

   return n;
}

static void write_server(SOCKET sock, const char *buffer)
{
   if (send(sock, buffer, strlen(buffer), 0) < 0)
   {
      perror("send()");
      exit(errno);
   }
}

void afficherMenuConnecte(SOCKET sock)
{
   printf("1. Voir la liste des joueurs\n");
   printf("2. Voir parties en cours\n");
   printf("3. Rejoindre le chat\n");
   printf("0. Se deconnecter\n");
   int choix;
   //scanf(" %d", &choix); remplacé par le fgets pour éviter les problèmes de buffer
   switch (choix)
   {
   case 1:
      write_server(sock, "1");
      printf("choix 1\n");
      break;
   case 2:
      printf("choix 2\n");
      break;
   case 3:
      printf("choix 3\n");
      break;
   case 4:
      afficherMenu(0, sock);
      break;
   }
}

void afficherMenu(int menu, SOCKET sock)
{
   printf("============ MENU ============\n");
   switch (menu)
   {
   case 0:
      printf("authentification\n");
      // afficherAuthentification();
      break;
   case 1:
      afficherMenuConnecte(sock);
      break;
   }
   printf("==============================\n");
}

int main(int argc, char **argv)
{
   if (argc < 2)
   {
      printf("Usage : %s [address] [pseudo]\n", argv[0]);
      return EXIT_FAILURE;
   }

   init();

   app(argv[1], argv[2]);

   end();

   return EXIT_SUCCESS;
}
