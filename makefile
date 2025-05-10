CC = gcc
CFLAGS = -Wall
EXEC = cyfighters.exe

SRC = main.c combat.c affichage.c equipe.c fichier.c ia.c technique.c utilitaire.c combattant.c
OBJ = $(SRC:.c=.o)


ifeq ($(OS),Windows_NT)
	RM = del /Q
	EXEC_RUN = $(EXEC)
else
	RM = rm -f
	EXEC_RUN = ./cyfighters.exe
endif

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	$(RM) *.o $(EXEC)

run: all
	$(EXEC_RUN)
