#include <stdio.h>
#include <stdlib.h>
#include "utilitaire.h"
#include "fichier.h"
#include "equipe.h"
#include "combat.h"
#include "ia.h"

int main() {
    initialiserAleatoire();

    int nbCombattants;
    Combattant* tous = chargerCombattants("combattants.txt", &nbCombattants);

    if (tous == NULL || nbCombattants < 6) {
        printf("Erreur : chargement des combattants impossible.\n");
        return 1;
    }

    // Créer deux équipes
    Equipe joueur, ia;
    initialiserEquipe(&joueur, "Joueur 1");
    initialiserEquipe(&ia, "IA");

    printf("Choisissez 3 combattants pour votre équipe :\n");

    for (int i = 0; i < nbCombattants; i++) {
        printf("%d - ", i);
        afficherCombattant(&tous[i]);
    }

    for (int c = 0; c < 3; c++) {
        int choix = demanderEntier("Votre choix : ", 0, nbCombattants - 1);
        ajouterCombattant(&joueur, &tous[choix]);
        tous[choix].pv = -1; // Marquer comme pris
    }

    // Donner les 3 restants à l’IA
    for (int i = 0; i < nbCombattants; i++) {
        if (tous[i].pv != -1 && ia.nbCombattants < 3) {
            ajouterCombattant(&ia, &tous[i]);
        }
    }

    printf("\n--- Début du combat ! ---\n");

    while (!equipeEstKO(&joueur) && !equipeEstKO(&ia)) {
        printf("\n--- Tour du Joueur ---\n");
        jouerTourJoueur(&joueur, &ia);

        for (int i = 0; i < joueur.nbCombattants; i++) {
            effetsFinTour(joueur.combattants[i]);
        }

        if (equipeEstKO(&ia)) break;

        printf("\n--- Tour de l’IA ---\n");
        jouerTourIA(&ia, &joueur);

        for (int i = 0; i < ia.nbCombattants; i++) {
            effetsFinTour(ia.combattants[i]);
        }
    }

    printf("\n--- Fin du combat ---\n");

    if (equipeEstKO(&joueur)) {
        printf("L’IA a gagné !\n");
    } else {
        printf("Vous avez gagné !\n");
    }

    free(tous);
    return 0;
}
