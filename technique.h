#ifndef TECHNIQUE_H
#define TECHNIQUE_H

// Représente une technique spéciale (ex : poison, buff, soin, etc.)
typedef struct {
    char nom[50];
    int valeur;
    int duree;
    int cooldown;
    int cooldown_restant;
    char description[100];  // Description affichée au joueur
} Technique;

#endif
