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

    // On applique ici chaque effet selon le nom (version simple pour l’instant)
    if (strcmp(t->nom, "Toxine du chaos") == 0) {
        for (int i = 0; i < equipeAdverse->nbCombattants; i++) {
            equipeAdverse->combattants[i]->poison_tours = t->duree;
            equipeAdverse->combattants[i]->poison_degats = t->valeur;
            printf("%s est empoisonné pour %d tour(s) !\n", equipeAdverse->combattants[i]->nom, t->duree);
        }
    } else if (strcmp(t->nom, "Coup d’infini") == 0) {
        for (int i = 0; i < equipeAdverse->nbCombattants; i++) {
            if (equipeAdverse->combattants[i]->pv > 0) {
                appliquerDegats(equipeAdverse->combattants[i], t->valeur);
                break;
            }
        }
    } else if (strcmp(t->nom, "Mur noir") == 0) {
        attaquant->buff_defense = t->valeur;
        attaquant->buff_defense_tours = t->duree;
        printf("%s augmente sa défense de +%d%% pour %d tours !\n", attaquant->nom, t->valeur, t->duree);
    } else if (strcmp(t->nom, "Voile d’ombre") == 0) {
        for (int i = 0; i < equipeAlliee->nbCombattants; i++) {
            equipeAlliee->combattants[i]->buff_agilite = 1;
            equipeAlliee->combattants[i]->buff_agilite_tours = t->duree;
            printf("%s devient insaisissable pendant %d tour(s) !\n", equipeAlliee->combattants[i]->nom, t->duree);
        }
    } else if (strcmp(t->nom, "Bénédiction obscure") == 0) {
        for (int i = 0; i < equipeAlliee->nbCombattants; i++) {
            if (equipeAlliee->combattants[i]->pv > 0 && equipeAlliee->combattants[i]->pv < equipeAlliee->combattants[i]->pv_max) {
                equipeAlliee->combattants[i]->pv = equipeAlliee->combattants[i]->pv_max;
                printf("%s est soigné totalement !\n", equipeAlliee->combattants[i]->nom);
                break;
            }
        }
    } else if (strcmp(t->nom, "Kamehameha psychique") == 0) {
        for (int i = 0; i < equipeAdverse->nbCombattants; i++) {
            if (equipeAdverse->combattants[i]->pv > 0) {
                equipeAdverse->combattants[i]->est_gele = 1;
                equipeAdverse->combattants[i]->tours_gele = t->duree;
                printf("%s est figé pendant %d tour(s) !\n", equipeAdverse->combattants[i]->nom, t->duree);
                break;
            }
        }
    }

    // Après l’utilisation, le cooldown est remis
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
