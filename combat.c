#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "combat.h"
#include "utilitaire.h"

// Applique une attaque classique
void attaquer(Combattant* attaquant, Combattant* cible) {
    if(attaquant == NULL || cible == NULL){
        exit(50);
    }
    printf("%s attaque %s !\n", attaquant->nom, cible->nom);
    if (esquive(cible)) {
        printf("%s esquive l’attaque !\n", cible->nom);
    } else {
        appliquerDegats(cible, attaquant->attaque);
    }
}

void utiliserTechnique(Combattant* attaquant, Equipe* equipeAlliee, Equipe* equipeAdverse) {
    if(attaquant == NULL || equipeAlliee == NULL || equipeAdverse == NULL){
        exit(60);
    }
    Technique* t = &attaquant->technique;

    if (t->cooldown_restant > 0) {
        printf("La technique de %s n’est pas encore prête (%d tour(s) restant(s)).\n", attaquant->nom, t->cooldown_restant);
        return;
    }

    // TOXINE DU CHAOS (JOKER) - poison tous les ennemis
    if (strcmp(t->nom, "Toxine du chaos") == 0) {
        for (int i = 0; i < equipeAdverse->nbCombattants; i++) {
            Combattant* cible = equipeAdverse->combattants[i];
            if (cible->pv > 0) {
                cible->poison_tours = t->duree;
                cible->poison_degats = t->valeur;
                cible->pv = cible->pv - t->valeur;
                printf("%s est empoisonné pour %d tour(s) !\n", cible->nom, t->duree);
            }
        }

    // COUP D'INFINI (THANOS) - dégâts directs à un ennemi
    } else if (strcmp(t->nom, "Coup d’infini") == 0) {
        int cibleChoisie = -1;

        if (strcmp(equipeAlliee->nom, "IA") != 0) {
            // Joueur humain → choisir une cible
            printf("Choisissez une cible pour Coup d’infini :\n");
            for (int i = 0; i < equipeAdverse->nbCombattants; i++) {
                if (equipeAdverse->combattants[i]->pv > 0) {
                    printf("%d - ", i);
                    afficherCombattant(equipeAdverse->combattants[i]);
                }
            }
            do {
                cibleChoisie = demanderEntier("Votre choix : ", 0, equipeAdverse->nbCombattants - 1);
            } while (equipeAdverse->combattants[cibleChoisie]->pv <= 0);
        } else {
            // IA → choisit première cible vivante
            for (int i = 0; i < equipeAdverse->nbCombattants; i++) {
                if (equipeAdverse->combattants[i]->pv > 0) {
                    cibleChoisie = i;
                    break;
                }
            }
        }

        if (cibleChoisie != -1) {
            appliquerDegats(equipeAdverse->combattants[cibleChoisie], t->valeur);
        }

    // MUR NOIR (ASTA) - boost défense perso
    } else if (strcmp(t->nom, "Mur noir") == 0) {
        attaquant->buff_defense = t->valeur;
        attaquant->buff_defense_tours = t->duree;
        printf("%s augmente sa défense de +%d%% pour %d tour(s) !\n", attaquant->nom, t->valeur, t->duree);

    // VOILE D’OMBRE (SHADOW) - boost agilité de tous les alliés
    } else if (strcmp(t->nom, "Voile d’ombre") == 0) {
        for (int i = 0; i < equipeAlliee->nbCombattants; i++) {
            Combattant* cible = equipeAlliee->combattants[i];
            if (cible->pv > 0) {
                cible->buff_agilite = 1;
                cible->buff_agilite_tours = t->duree;
                printf("%s devient insaisissable pendant %d tour(s) !\n", cible->nom, t->duree);
            }
        }

    // BENEDICTION OBSCURE (SHADE) - soin complet sur un allié
    } else if (strcmp(t->nom, "Bénédiction obscure") == 0) {
        int cibleChoisie = -1;

        if (strcmp(equipeAlliee->nom, "IA") != 0) {
            // Joueur humain → choisir un allié blessé
            printf("Choisissez un allié à soigner :\n");
            for (int i = 0; i < equipeAlliee->nbCombattants; i++) {
                Combattant* c = equipeAlliee->combattants[i];
                if (c->pv > 0 && c->pv < c->pv_max) {
                    printf("%d - ", i);
                    afficherCombattant(c);
                }
            }
            do {
                cibleChoisie = demanderEntier("Votre choix : ", 0, equipeAlliee->nbCombattants - 1);
            } while (equipeAlliee->combattants[cibleChoisie]->pv <= 0 ||
                     equipeAlliee->combattants[cibleChoisie]->pv == equipeAlliee->combattants[cibleChoisie]->pv_max);
        } else {
            // IA → soigne premier allié blessé
            for (int i = 0; i < equipeAlliee->nbCombattants; i++) {
                if (equipeAlliee->combattants[i]->pv > 0 &&
                    equipeAlliee->combattants[i]->pv < equipeAlliee->combattants[i]->pv_max) {
                    cibleChoisie = i;
                    break;
                }
            }
        }

        if (cibleChoisie != -1) {
            equipeAlliee->combattants[cibleChoisie]->pv = equipeAlliee->combattants[cibleChoisie]->pv_max;
            printf("%s est soigné totalement !\n", equipeAlliee->combattants[cibleChoisie]->nom);
        }

    // KAMEHAMEHA PSYCHIQUE (GOKU) - gel tous les ennemis
    } else if (strcmp(t->nom, "Kamehameha psychique") == 0) {
        for (int i = 0; i < equipeAdverse->nbCombattants; i++) {
            Combattant* cible = equipeAdverse->combattants[i];
            if (cible->pv > 0) {
                cible->est_gele = 1;
                cible->tours_gele = t->duree;
                printf("%s est figé pendant %d tour(s) !\n", cible->nom, t->duree);
            }
        }
    }

    // Après utilisation, on active le cooldown
    t->cooldown_restant = t->cooldown;
}


void jouerTourJoueur(Equipe* equipeJoueur, Equipe* equipeAdverse) {
    if(equipeJoueur == NULL || equipeAdverse == NULL){
        exit(70);
    }
    printf("\nVos combattants :\n");

    // Affiche uniquement les combattants vivants
    for (int i = 0; i < equipeJoueur->nbCombattants; i++) {
        if (equipeJoueur->combattants[i]->pv > 0) {
            printf("%d - ", i);
            afficherCombattant(equipeJoueur->combattants[i]);
        }
    }

    // Choix du combattant qui joue
    int indexCombattant;
    do {
        indexCombattant = demanderEntier("Quel combattant voulez-vous utiliser ? ", 0, equipeJoueur->nbCombattants - 1);
        if (equipeJoueur->combattants[indexCombattant]->pv <= 0) {
            printf("Ce combattant est KO ! Choisissez-en un autre.\n");
        }
    } while (equipeJoueur->combattants[indexCombattant]->pv <= 0);

    Combattant* attaquant = equipeJoueur->combattants[indexCombattant];

    // Choix : attaque normale ou technique
    printf("\nQue voulez-vous faire avec %s ?\n", attaquant->nom);
    printf("1 - Attaquer\n");
    printf("2 - Utiliser la technique spéciale (%s) [%d tour(s) de recharge]\n",
           attaquant->technique.nom, attaquant->technique.cooldown_restant);
    int action = demanderEntier("Votre choix : ", 1, 2);

    // Choix de la cible vivante
    printf("\nCibles disponibles :\n");
    for (int i = 0; i < equipeAdverse->nbCombattants; i++) {
        if (equipeAdverse->combattants[i]->pv > 0) {
            printf("%d - ", i);
            afficherCombattant(equipeAdverse->combattants[i]);
        }
    }

    int indexCible;
    do {
        indexCible = demanderEntier("Quelle cible voulez-vous attaquer ? ", 0, equipeAdverse->nbCombattants - 1);
        if (equipeAdverse->combattants[indexCible]->pv <= 0) {
            printf("Cette cible est déjà KO ! Choisissez-en une autre.\n");
        }
    } while (equipeAdverse->combattants[indexCible]->pv <= 0);

    Combattant* cible = equipeAdverse->combattants[indexCible];

    // Exécution de l’action
    if (action == 1) {
        attaquer(attaquant, cible);
    } else {
        utiliserTechnique(attaquant, equipeJoueur, equipeAdverse);
    }
}
