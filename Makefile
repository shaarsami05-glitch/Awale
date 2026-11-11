CC     = gcc
CFLAGS = -Wall -Wextra -g -MMD -MP -IClient -IServeur

CLIENT_SRCS  = Client/client.c Client/interface.c
SERVEUR_SRCS = Serveur/serveur.c Serveur/awale.c Serveur/traitement.c Serveur/basedonnees.c

CLIENT_OBJS  = $(CLIENT_SRCS:.c=.o)
SERVEUR_OBJS = $(SERVEUR_SRCS:.c=.o)
DEPS         = $(CLIENT_OBJS:.o=.d) $(SERVEUR_OBJS:.o=.d)

all: bin/client bin/serveur

bin/client: $(CLIENT_OBJS)
	@mkdir -p bin
	$(CC) $(CFLAGS) -o $@ $^

bin/serveur: $(SERVEUR_OBJS)
	@mkdir -p bin
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(CLIENT_OBJS) $(SERVEUR_OBJS) $(DEPS)
	rm -rf bin

-include $(DEPS)

.PHONY: all clean