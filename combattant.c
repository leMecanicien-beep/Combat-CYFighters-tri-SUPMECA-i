#include <stdio.h>
#include "combattant.h"
#include "utilitaire.h" // pour tirageAleatoire()
#include "technique.h"

// Affiche les informations d’un combattant
void afficherCombattant(Combattant *c) {
    printf("%s (PV: %d/%d | ATQ: %d | DEF: %d%% | AGI: %d)\n",
           c->nom, c->pv, c->pv_max, c->attaque, c->defense, c->agilite);
    afficherTechnique(&c->technique);  // On affiche aussi sa technique
}

int esquive(Combattant *c) {
    int agi = c->agilite;

    // Si le personnage a un bonus d’agilité temporaire (ex: tech de Shadow)
    if (c->buff_agilite_tours > 0) {
        agi = 1;  // esquive garantie
    }

    // Si agilité effective = 1 ou moins, esquive garantie
    if (agi <= 1) {
        return 1;
    }

    // Tirage entre 1 et agi
    return (tirageAleatoire(1, agi) == 1);
}


// Applique des dégâts à un combattant en tenant compte de sa défense
void appliquerDegats(Combattant *cible, int degats) {
    int defense_totale = cible->defense + cible->buff_defense;
    if (defense_totale > 100) defense_totale = 100;

    int degats_reels = degats * (100 - defense_totale) / 100;
    cible->pv -= degats_reels;
    if (cible->pv < 0) cible->pv = 0;

    printf("%s subit %d points de dégâts.\n", cible->nom, degats_reels);
}

// Applique les effets de début de tour (poison, gel)
void effetsDebutTour(Combattant *c) {
    if (c->poison_tours > 0) {
        printf("%s subit %d dégâts de poison.\n", c->nom, c->poison_degats);
        c->pv -= c->poison_degats;
        if (c->pv < 0) c->pv = 0;
        c->poison_tours--;
    }

    if (c->tours_gele > 0) {
        c->tours_gele--;
        if (c->tours_gele == 0) c->est_gele = 0;
    }
}

// Applique les effets de fin de tour (réduction des buffs)
void effetsFinTour(Combattant *c) {
    if (c->buff_defense_tours > 0) {
        c->buff_defense_tours--;
        if (c->buff_defense_tours == 0) {
            c->buff_defense = 0;
        }
    }

    if (c->buff_agilite_tours > 0) {
        c->buff_agilite_tours--;
        if (c->buff_agilite_tours == 0) {
            c->buff_agilite = 0;
        }
    }

    if (c->technique.cooldown_restant > 0) {
        c->technique.cooldown_restant--;
    }

    if (c->tours_gele > 0) {
    c->tours_gele--;
        if (c->tours_gele == 0) {
        c->est_gele = 0;
        }
    }
}
