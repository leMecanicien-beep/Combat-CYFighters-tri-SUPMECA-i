#include <stdio.h>
#include <string.h>
#include "technique.h"

void initialiserTechnique(Technique *t, const char *nom, int valeur, int duree, int cooldown, const char *desc) {
    strncpy(t->nom, nom, 49);
    t->nom[49] = '\0';
    t->valeur = valeur;
    t->duree = duree;
    t->cooldown = cooldown;

    // La technique doit attendre son cooldown complet avant d’être utilisable (pas utilisable immédiatement)
    t->cooldown_restant = cooldown;
  
    strncpy(t->description, desc, 99);
    t->description[99] = '\0';
}

// Affiche les informations de la technique
void afficherTechnique(const Technique *t) {
    printf("  > %s : %s (valeur: %d, durée: %d tour(s), cooldown: %d, prêt dans %d tour(s))\n",
           t->nom, t->description, t->valeur, t->duree, t->cooldown, t->cooldown_restant);
}
