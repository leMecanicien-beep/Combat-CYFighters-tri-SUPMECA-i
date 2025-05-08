#include <stdio.h>
#include <stdlib.h>
#include "utilitaire.h"
#include "fichier.h"
#include "equipe.h"
#include "combat.h"
#include "ia.h"
#include <string.h>

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
    char nomEquipe[30];
    printf("Entrez le nom de votre équipe : ");
    fgets(nomEquipe, sizeof(nomEquipe), stdin);

    // Supprimer le saut de ligne si présent
    size_t len = strlen(nomEquipe);
    if (len > 0 && nomEquipe[len - 1] == '\n') {
    nomEquipe[len - 1] = '\0';
    }
    initialiserEquipe(&joueur, nomEquipe);
    initialiserEquipe(&ia, "IA");

    // Tableau pour marquer les combattants déjà pris
    int pris[6] = {0}; // 0 = dispo, 1 = déjà pris

    printf("Choisissez 3 combattants pour votre équipe :\n");

    for (int i = 0; i < nbCombattants; i++) {
        printf("%d - ", i);
        afficherCombattant(&tous[i]);
    }

    for (int c = 0; c < 3; c++) {
        int choix;
        do {
            choix = demanderEntier("Votre choix : ", 0, nbCombattants - 1);
            if (pris[choix]) {
                printf("Ce combattant est déjà pris. Choisissez-en un autre.\n");
            }
        } while (pris[choix]);

        ajouterCombattant(&joueur, &tous[choix]);
        pris[choix] = 1;
    }

    // Donner les combattants non pris à l’IA
    for (int i = 0; i < nbCombattants; i++) {
        if (!pris[i] && ia.nbCombattants < 3) {
            ajouterCombattant(&ia, &tous[i]);
            pris[i] = 1;
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
