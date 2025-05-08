#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "utilitaire.h"

// Initialise le générateur de nombres aléatoires
void initialiserAleatoire() {
    srand(time(NULL));
}

// Tire un nombre entier entre min et max inclus
int tirageAleatoire(int min, int max) {
    return rand() % (max - min + 1) + min;
}

// Demande un entier entre min et max à l’utilisateur
int demanderEntier(const char* message, int min, int max) {
    int valeur;
    int resultat;

    do {
        printf("%s", message);
        resultat = scanf("%d", &valeur);
        while(getchar() != '\n');  // vide le buffer

        if (resultat != 1 || valeur < min || valeur > max) {
            printf("Veuillez entrer un nombre entre %d et %d.\n", min, max);
        }
    } while (valeur < min || valeur > max);

    return valeur;
}

// Attend que l’utilisateur appuie sur Entrée
void pause() {
    printf("Appuyez sur Entrée pour continuer...");
    while (getchar() != '\n');
}
