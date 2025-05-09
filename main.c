#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utilitaire.h"
#include "fichier.h"
#include "equipe.h"
#include "combat.h"
#include "ia.h"
#include "combattant.h"
#include "affichage.h"

int main() {
    initialiserAleatoire();

    int rejouer = 1;
    while (rejouer) {
        int nbCombattants;
        Combattant* tous = chargerCombattants("combattants.txt", &nbCombattants);
        if (tous == NULL || nbCombattants < 6) {
            printf("Erreur : chargement des combattants impossible.\n");
            return 1;
        }

        char nomEquipe[30];
        printf("Entrez le nom de votre équipe : ");
        fgets(nomEquipe, sizeof(nomEquipe), stdin);
        size_t len = strlen(nomEquipe);
        if (len > 0 && nomEquipe[len - 1] == '\n') {
            nomEquipe[len - 1] = '\0';
        }

        Equipe joueur, ia;
        initialiserEquipe(&joueur, nomEquipe);
        initialiserEquipe(&ia, "IA");

        int pris[10] = {0};

        printf("\n--- Sélection des combattants ---\n");
        afficherChoixEquipe(tous, nbCombattants);
        for (int i = 0; i < 3; i++) {
            int choix;
            do {
                choix = demanderEntier("Votre choix : ", 0, nbCombattants - 1);
                if (pris[choix]) {
                    printf("Ce combattant est déjà pris.\n");
                }
            } while (pris[choix]);
            ajouterCombattant(&joueur, &tous[choix]);
            pris[choix] = 1;
        }

        // Sélection IA
        for (int i = 0; i < nbCombattants && ia.nbCombattants < 3; i++) {
            if (!pris[i]) {
                ajouterCombattant(&ia, &tous[i]);
                pris[i] = 1;
            }
        }

        printf("\n--- Début du combat ! ---\n");

        int tour = 1;
        while (!equipeEstKO(&joueur) && !equipeEstKO(&ia)) {
            printf("\n===== TOUR %d =====\n", tour);
            afficherEquipesCoteACote(&joueur, &ia);

            for (int i = 0; i < joueur.nbCombattants; i++) {
                if (joueur.combattants[i]->pv > 0) {
                    while (!tourCombattant(joueur.combattants[i], &joueur, &ia, 0));
                }
            }

            for (int i = 0; i < ia.nbCombattants; i++) {
                if (ia.combattants[i]->pv > 0) {
                    while (!tourCombattant(ia.combattants[i], &ia, &joueur, 1));
                }
            }

            for (int i = 0; i < joueur.nbCombattants; i++) {
                effetsFinTour(joueur.combattants[i]);
            }
            for (int i = 0; i < ia.nbCombattants; i++) {
                effetsFinTour(ia.combattants[i]);
            }

            tour++;
        }

        printf("\n--- Fin du combat ---\n");
        if (equipeEstKO(&joueur)) {
            printf("L'IA remporte le combat.\n");
        } else {
            printf("Bravo, vous avez gagné le combat !\n");
        }

        free(tous);

        printf("Voulez-vous rejouer ? (1 = Oui, 0 = Non) : ");
        scanf("%d", &rejouer);
        while (getchar() != '\n'); // vider le buffer
    }

    printf("Merci d'avoir joué !\n");
    return 0;
}

