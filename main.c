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
    printf("Bienvenue dans un jeu où multiples univers s'affrontent, CY_Fighters\n");
    initialiserAleatoire();

    int rejouer = 1;
    while (rejouer) {
        int nbCombattants;
        Combattant* tous = chargerCombattants("combattants.txt", &nbCombattants);

        if (tous == NULL || nbCombattants < 6) {
            printf("Erreur : chargement des combattants impossible.\n");
            return 1;
        }

        // Saisie du nom de l'équipe joueur
        char nomEquipe[30];
        printf("Entrez le nom de votre equipe : ");
        fgets(nomEquipe, sizeof(nomEquipe), stdin);
        size_t len = strlen(nomEquipe);
        if (len > 0 && nomEquipe[len - 1] == '\n') {
            nomEquipe[len - 1] = '\0';
        }

        // Création des équipes
        Equipe joueur, ia;
        initialiserEquipe(&joueur, nomEquipe);
        initialiserEquipe(&ia, "IA");

        int pris[nbCombattants];
        for (int i = 0; i < nbCombattants; i++) pris[i] = 0;

        afficherChoixEquipe(tous, nbCombattants);

        printf("Choisissez 3 combattants pour votre equipe :\n");
        while (joueur.nbCombattants < 3) {
            int choix = demanderEntier("Votre choix : ", 0, nbCombattants - 1);
            if (!pris[choix]) {
                ajouterCombattant(&joueur, &tous[choix]);
                pris[choix] = 1;
            } else {
                printf("Ce combattant est deja pris.\n");
            }
        }

        // Choix IA automatique
        for (int i = 0; i < nbCombattants && ia.nbCombattants < 3; i++) {
            if (!pris[i]) {
                ajouterCombattant(&ia, &tous[i]);
                pris[i] = 1;
            }
        }

        printf("\n--- Debut du combat ---\n");

        int vainqueur = 0;
        while (!equipeEstKO(&joueur) && !equipeEstKO(&ia)) {
            afficherEquipesCoteACote(&joueur, &ia);

            printf("\n--- Tour du Joueur ---\n");
            jouerTourJoueur(&joueur, &ia);
            if (equipeEstKO(&ia)) {
                vainqueur = 1;
                break;
            }
            
            int tousGeles = 1;
            for (int i = 0; i < ia.nbCombattants; i++) {
                if (!ia.combattants[i]->est_gele) {
                tousGeles = 0;
                break;
                }
            }

            if (!tousGeles) {
                printf("\n--- Tour de l'IA ---\n");
                jouerTourIA(&ia, &joueur);
            } else {
                printf("\n--- Tous les combattants IA sont gelés : tour sauté ---\n");
            }


            if (equipeEstKO(&joueur)) {
                vainqueur = 2;
                break;
            }

            for (int i = 0; i < joueur.nbCombattants; i++) {
                effetsFinTour(joueur.combattants[i]);
            }
            for (int i = 0; i < ia.nbCombattants; i++) {
                effetsFinTour(ia.combattants[i]);
            }
        }

        printf("\n========================================\n");
        printf("||           🔥 FIN DU COMBAT 🔥           ||\n");
        printf("========================================\n");

        if (vainqueur == 1) {
            printf("🎉 Félicitations, l’équipe %s remporte la victoire ! 🎉\n", nomEquipe);
            printf("   Vous avez vaincu l’équipe adverse avec honneur.\n");
        } else if (vainqueur == 2) {
            printf("💀 L’IA a triomphé ! Mieux vaut retenter votre chance... 💀\n");
            printf("   Courage, l’honneur est dans l’effort !\n");
        } else {
            printf("⚔️ Match nul ! Quelle bataille épique jusqu’à la fin. ⚔️\n");
        }

        printf("========================================\n");


        free(tous);

        printf("\nSouhaitez-vous rejouer ? (1 = Oui, 0 = Non) : ");
        scanf("%d", &rejouer);
        while (getchar() != '\n'); // vide le buffer
    }

    printf("Merci d'avoir joue ! A bientot.\n");
    return 0;
}
