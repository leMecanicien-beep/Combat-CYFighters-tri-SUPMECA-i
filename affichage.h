#ifndef AFFICHAGE_H
#define AFFICHAGE_H

#include "equipe.h"

void afficherEquipesCoteACote(Equipe* equipeGauche, Equipe* equipeDroite);
void afficherCarteCombattant(Combattant* c);
void afficherChoixEquipe(Combattant* tous, int nb);
void afficherCartesCombat(Equipe* equipe);

#endif
