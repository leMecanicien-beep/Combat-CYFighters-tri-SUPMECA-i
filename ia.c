#include <stdio.h>
#include "ia.h"
#include "combat.h"
#include "utilitaire.h"

// L’IA choisit une action simple pour le premier perso vivant de l’équipe
void jouerTourIA(Equipe* equipeIA, Equipe* equipeAdverse) {
    // Cherche le premier combattant vivant
    Combattant* attaquant = NULL;
    for (int i = 0; i < equipeIA->nbCombattants; i++) {
        if (equipeIA->combattants[i]->pv > 0) {
            attaquant = equipeIA->combattants[i];
            break;
        }
    }

    if (!attaquant) return; // Aucun combattant disponible

    // Cherche une cible adverse vivante au hasard
    Combattant* cible = NULL;
    while (!cible) {
        int r = tirageAleatoire(0, equipeAdverse->nbCombattants - 1);
        if (equipeAdverse->combattants[r]->pv > 0) {
            cible = equipeAdverse->combattants[r];
        }
    }

    // Si la technique est prête, 50% de chances de l’utiliser
    if (attaquant->technique.cooldown_restant == 0 && tirageAleatoire(1, 2) == 1) {
        printf("[IA] %s utilise sa technique spéciale !\n", attaquant->nom);
        utiliserTechnique(attaquant, equipeIA, equipeAdverse);  // Fonction dans combat.c
    } else {
        printf("[IA] %s attaque %s !\n", attaquant->nom, cible->nom);
        attaquer(attaquant, cible);  // Fonction dans combat.c
    }
}
