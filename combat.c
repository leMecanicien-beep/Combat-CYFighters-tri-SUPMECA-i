#include <stdio.h>
#include <string.h>
#include "combat.h"
#include "utilitaire.h"
#include "affichage.h"

void attaquer(Combattant* attaquant, Combattant* cible) {
    if (esquive(cible)) {
        printf("%s esquive l’attaque de %s !\n", cible->nom, attaquant->nom);
        return;
    }

    printf("%s attaque %s !\n", attaquant->nom, cible->nom);
    appliquerDegats(cible, attaquant->attaque);
}

void utiliserTechnique(Combattant* c, Equipe* equipeAlliee, Equipe* equipeAdverse) {
    Technique* t = &c->technique;

    if (t->cooldown_restant > 0) {
        printf("La technique de %s n’est pas encore prête (%d tour(s) restant(s)).\n", c->nom, t->cooldown_restant);
        return;
    }

    printf("%s utilise sa technique spéciale : %s !\n", c->nom, t->nom);

    if (strcmp(t->nom, "Toxine du chaos") == 0) {
        for (int i = 0; i < equipeAdverse->nbCombattants; i++) {
            Combattant* cible = equipeAdverse->combattants[i];
            if (cible->pv > 0) {
                cible->poison_tours = t->duree;
                cible->poison_degats = t->valeur;
                printf("%s est empoisonné !\n", cible->nom);
            }
        }
    } else if (strcmp(t->nom, "Voile d’ombre") == 0) {
        for (int i = 0; i < equipeAlliee->nbCombattants; i++) {
            Combattant* allie = equipeAlliee->combattants[i];
            if (allie->pv > 0) {
                allie->buff_agilite = 1;
                allie->buff_agilite_tours = t->duree;
                printf("%s devient insaisissable !\n", allie->nom);
            }
        }
    } else if (strcmp(t->nom, "Bénédiction obscure") == 0) {
        printf("Choisissez l’allié à soigner :\n");
        for (int i = 0; i < equipeAlliee->nbCombattants; i++) {
            Combattant* cible = equipeAlliee->combattants[i];
            printf("%d - %s (%d/%d PV)\n", i, cible->nom, cible->pv, cible->pv_max);
        }
        int choix = demanderEntier("Votre choix : ", 0, equipeAlliee->nbCombattants - 1);
        equipeAlliee->combattants[choix]->pv = equipeAlliee->combattants[choix]->pv_max;
        printf("%s est entièrement soigné !\n", equipeAlliee->combattants[choix]->nom);
    } else if (strcmp(t->nom, "Coup d’infini") == 0) {
        printf("Choisissez la cible à attaquer :\n");
        for (int i = 0; i < equipeAdverse->nbCombattants; i++) {
            Combattant* cible = equipeAdverse->combattants[i];
            printf("%d - %s (%d/%d PV)\n", i, cible->nom, cible->pv, cible->pv_max);
        }
        int choix = demanderEntier("Votre choix : ", 0, equipeAdverse->nbCombattants - 1);
        appliquerDegats(equipeAdverse->combattants[choix], t->valeur);
    } else if (strcmp(t->nom, "Kamehameha psychique") == 0) {
        for (int i = 0; i < equipeAdverse->nbCombattants; i++) {
            if (equipeAdverse->combattants[i]->pv > 0) {
                equipeAdverse->combattants[i]->est_gele = 1;
                equipeAdverse->combattants[i]->tours_gele = t->duree;
                printf("%s est gelé et ne pourra pas jouer !\n", equipeAdverse->combattants[i]->nom);
            }
        }
    } else if (strcmp(t->nom, "Mur noir") == 0) {
        c->buff_defense = t->valeur;
        c->buff_defense_tours = t->duree;
        printf("%s augmente sa défense temporairement.\n", c->nom);
    }

    t->cooldown_restant = t->cooldown;
}

int tourCombattant(Combattant* c, Equipe* equipeJoueur, Equipe* equipeAdverse, int estIA) {
    if (c->pv <= 0 || c->est_gele) {
        if (c->est_gele) {
            printf("%s est gelé et ne peut pas jouer ce tour.\n", c->nom);
        }
        pause();
        return 1;  // tour terminé même si aucune action
    }

    int choixAction;

    if (!estIA) {
        printf("\nQue voulez-vous faire avec %s ?\n", c->nom);
        printf("1 - Attaquer\n");
        printf("2 - Utiliser la technique spéciale (%s) [%d tour(s) de recharge]\n",
               c->technique.nom, c->technique.cooldown_restant);
        choixAction = demanderEntier("Votre choix : ", 1, 2);
    } else {
        choixAction = (tirageAleatoire(1, 3) == 1 && c->technique.cooldown_restant == 0) ? 2 : 1;
    }

    if (choixAction == 1) {
        // Attaque normale
        printf(estIA ? "[IA] " : "");
        printf("%s attaque !\n", c->nom);

        // Affiche les cibles possibles
        for (int i = 0; i < equipeAdverse->nbCombattants; i++) {
            if (equipeAdverse->combattants[i]->pv > 0) {
                printf("%d - %s\n", i, equipeAdverse->combattants[i]->nom);
            }
        }

        int cibleIndex;
        if (!estIA) {
            cibleIndex = demanderEntier("Quelle cible voulez-vous attaquer ? ", 0, equipeAdverse->nbCombattants - 1);
        } else {
            do {
                cibleIndex = tirageAleatoire(0, equipeAdverse->nbCombattants - 1);
            } while (equipeAdverse->combattants[cibleIndex]->pv <= 0);
        }

        attaquer(c, equipeAdverse->combattants[cibleIndex]);
        return 1;
    }

    // Si le joueur ou l’IA choisit la technique spéciale
    if (c->technique.cooldown_restant > 0) {
        printf("La technique de %s n’est pas encore prête (%d tour(s) restant(s)).\n",
               c->nom, c->technique.cooldown_restant);
        if (!estIA) pause();
        return 0; // Le joueur pourra refaire un choix
    }

    utiliserTechnique(c, equipeJoueur, equipeAdverse);
    return 1;
}
