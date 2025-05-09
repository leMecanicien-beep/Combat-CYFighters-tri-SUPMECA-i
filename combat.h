#ifndef COMBAT_H
#define COMBAT_H

#include "equipe.h"

// Effectue une attaque simple
void attaquer(Combattant* attaquant, Combattant* cible);

// Utilise la technique spéciale de l’attaquant
void utiliserTechnique(Combattant* attaquant, Equipe* equipeAlliee, Equipe* equipeAdverse);

// Fait jouer un tour complet à un joueur humain
int tourCombattant(Combattant* c, Equipe* equipeJoueur, Equipe* equipeAdverse);

void effetsFinTour(Combattant* c);

#endif
