CC = gcc
CFLAGS = -Wall
EXEC = cyfighters.exe

SRC = main.c combat.c affichage.c equipe.c fichier.c ia.c technique.c utilitaire.c combattant.c
OBJ = $(SRC:.c=.o)

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	del /Q *.o $(EXEC)

run: all
	$(EXEC)
