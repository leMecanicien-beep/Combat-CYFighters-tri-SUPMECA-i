# Nom de l'exécutable
EXEC = cyfighters

# Compilateur
CC = gcc

# Options de compilation
CFLAGS = -Wall -Wextra -std=c99

# Tous les fichiers sources
SRC = main.c \
      fichier.c \
      combat.c \
      ia.c \
      equipe.c \
      combattant.c \
      technique.c \
      utilitaire.c

# Fichiers objets générés (.o)
OBJ = $(SRC:.c=.o)

# Cible par défaut
all: $(EXEC)

# Règle de compilation de l'exécutable
$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Pour tout nettoyer
clean:
	del *.o *.exe 2> nul || rm -f *.o $(EXEC)

# Pour recompiler tout
rebuild: clean all
