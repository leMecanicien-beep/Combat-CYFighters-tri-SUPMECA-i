#ifndef FICHIER_H
#define FICHIER_H

#include "combattant.h" // Lit les combattants depuis un fichier texte.

// Renvoie un tableau dynamique de combattants, et stocke le nombre dans *nb
Combattant* chargerCombattants(const char* nomFichier, int* nb);

#endif
