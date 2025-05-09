#ifndef COMBATTANT_H
#define COMBATTANT_H

#include "technique.h"

// Structure représentant un combattant du jeu
typedef struct {
    char nom[50];
    int pv;                 // Points de vie actuels
    int pv_max;             // Points de vie max
    int attaque;
    int defense;            // Défense en %
    int agilite;            // Agilité (1 = esquive garantie, 2 = 1 chance sur 2, etc.)

    Technique technique;    // Sa technique spéciale
    int cooldown_actuel;

    // Effets temporaires :
    int poison_tours;       // Nombre de tours de poison restants
    int poison_degats;      // Dégâts de poison par tour

    int buff_defense;       // Bonus temporaire de défense
    int buff_defense_tours; // Combien de tours restants pour ce bonus

    int buff_agilite;       // Agilité temporairement forcée à une valeur
    int buff_agilite_tours; // Durée restante de ce bonus

    int est_gele;           // 1 si le joueur est gelé (tour sauté)
    int tours_gele;         // Nombre de tours restants gelé
} Combattant;

// Fonctions associées
void afficherCombattant(Combattant *c);
int esquive(Combattant *c);
void appliquerDegats(Combattant *cible, int degats);
void effetsDebutTour(Combattant *c);
void effetsFinTour(Combattant *c);

#endif
