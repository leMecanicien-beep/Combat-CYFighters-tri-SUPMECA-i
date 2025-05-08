#ifndef UTILITAIRE_H
#define UTILITAIRE_H

// Initialise le générateur de nombres aléatoires
void initialiserAleatoire();

// Tire un entier entre min et max (inclus)
int tirageAleatoire(int min, int max);

// Demande à l’utilisateur de saisir un entier entre min et max
int demanderEntier(const char* message, int min, int max);

// Pause (attente d’entrée)
void pause();

#endif
