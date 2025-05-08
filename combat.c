#include <stdio.h>
#include <string.h>
#include "combat.h"
#include "utilitaire.h"

// Applique une attaque classique
void attaquer(Combattant* attaquant, Combattant* cible) {
    printf("%s attaque %s !\n", attaquant->nom, cible->nom);
    if (esquive(cible)) {
        printf("%s esquive l’attaque !\n", cible->nom);
    } else {
        appliquerDegats(cible, attaquant->attaque);
    }
}

void utiliserTechnique(Combattant* attaquant, Equipe* equipeAlliee, Equipe* equipeAdverse) {
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
    int i;
    printf("Choisissez votre combattant :\n");
    for (i = 0; i < equipeJoueur->nbCombattants; i++) {
        if (equipeJoueur->combattants[i]->pv > 0) {
            printf("%d - ", i);
            afficherCombattant(equipeJoueur->combattants[i]);
        }
    }

    int choix = demanderEntier("Votre choix : ", 0, equipeJoueur->nbCombattants - 1);
    Combattant* c = equipeJoueur->combattants[choix];

    if (c->est_gele) {
        printf("%s est gelé et ne peut pas jouer ce tour.\n", c->nom);
        return;
    }

    printf("1 - Attaquer\n2 - Utiliser technique spéciale\n");
    int action = demanderEntier("Action : ", 1, 2);

    if (action == 1) {
        printf("Cible :\n");
        for (i = 0; i < equipeAdverse->nbCombattants; i++) {
            if (equipeAdverse->combattants[i]->pv > 0) {
                printf("%d - ", i);
                afficherCombattant(equipeAdverse->combattants[i]);
            }
        }
        int cibleChoisie = demanderEntier("Votre cible : ", 0, equipeAdverse->nbCombattants - 1);
        attaquer(c, equipeAdverse->combattants[cibleChoisie]);
    } else {
        utiliserTechnique(c, equipeJoueur, equipeAdverse);
    }
}
