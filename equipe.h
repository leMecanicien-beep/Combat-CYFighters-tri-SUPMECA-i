#ifndef EQUIPE_H
#define EQUIPE_H

#include "combattant.h"  // on inclut les combattants

#define NB_EQUIPE_MAX 3

// Structure représentant une équipe de joueurs
typedef struct {
    char nom[30];
    Combattant *combattants[NB_EQUIPE_MAX];
    int nbCombattants;                // Combien de combattants sont ajoutés
} Equipe;

// Fonctions associées
void initialiserEquipe(Equipe *e, const char *nom);
void ajouterCombattant(Equipe *e, Combattant *c);
int equipeEstKO(Equipe *e);

#endif
