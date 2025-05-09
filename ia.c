#include <stdio.h>
#include "ia.h"
#include "affichage.h"
#include "combat.h"
#include "utilitaire.h"

void jouerTourIA(Equipe* ia, Equipe* joueur) {
    printf("\n--- Tour de l'IA ---\n");
    afficherCartesCombat(ia);

    for (int i = 0; i < ia->nbCombattants; i++) {
        Combattant* c = ia->combattants[i];
        if (c->pv <= 0) continue;

        // Si la technique est disponible, l'utiliser aléatoirement 50% des cas
        if (c->cooldown_actuel == 0 && tirageAleatoire(1, 2) == 1) {
            printf("%s utilise sa technique speciale : %s !\n", c->nom, c->technique.nom);
            utiliserTechnique(c, ia, joueur);
        } else {
            // Choisir une cible vivante
            int cibleIndex = -1;
            for (int j = 0; j < joueur->nbCombattants; j++) {
                if (joueur->combattants[j]->pv > 0) {
                    cibleIndex = j;
                    break;
                }
            }
            if (cibleIndex != -1) {
                printf("%s attaque %s !\n", c->nom, joueur->combattants[cibleIndex]->nom);
                attaquer(c, joueur->combattants[cibleIndex]);
            }
        }

        if (c->cooldown_actuel > 0) {
            c->cooldown_actuel--;
        }
    }
}
