#include <stdio.h>
#include <string.h>
#include "affichage.h"
#include "combattant.h"

// Génère une barre de PV en ASCII pur (ex : ########..)
void genererBarrePV(int pv, int pv_max, char* sortie) {
    int nb = (pv * 10) / pv_max;
    for (int i = 0; i < 10; i++) {
        sortie[i] = (i < nb) ? '#' : '.';
    }
    sortie[10] = '\0';
}

// Remplit un tableau de lignes représentant un combattant
void genererLigne(Combattant* c, char lignes[6][100]) {
    if (c == NULL || c->pv <= 0) {
        for (int i = 0; i < 6; i++) {
            strcpy(lignes[i], "                ");
        }
        return;
    }

    char barre[16];
    genererBarrePV(c->pv, c->pv_max, barre);

    snprintf(lignes[0], 100, "+--------------+");
    snprintf(lignes[1], 100, "| %-12s|", c->nom);
    snprintf(lignes[2], 100, "| PV: [%-10s]|", barre);
    snprintf(lignes[3], 100, "| ATK: %-6d |", c->attaque);
    snprintf(lignes[4], 100, "| DEF: %-5d%% |", c->defense);
    snprintf(lignes[5], 100, "+--------------+");
}

// Affiche deux équipes côte à côte en ASCII simple
void afficherEquipesCoteACote(Equipe* gauche, Equipe* droite) {
    char lignesG[3][6][100];
    char lignesD[3][6][100];

    for (int i = 0; i < 3; i++) {
        genererLigne((i < gauche->nbCombattants) ? gauche->combattants[i] : NULL, lignesG[i]);
        genererLigne((i < droite->nbCombattants) ? droite->combattants[i] : NULL, lignesD[i]);
    }

    for (int ligne = 0; ligne < 6; ligne++) {
        for (int p = 0; p < 3; p++) {
            printf("%-20s", lignesG[p][ligne]);
        }
        printf("       ");
        for (int p = 0; p < 3; p++) {
            printf("%-20s", lignesD[p][ligne]);
        }
        printf("\n");
    }

    printf("\nETATS :\n");
    for (int p = 0; p < 3; p++) {
        if (gauche->combattants[p] != NULL && gauche->combattants[p]->pv > 0) {
            printf("G%d: ", p);
            if (gauche->combattants[p]->poison_tours > 0) printf("Poison (%d) ", gauche->combattants[p]->poison_tours);
            if (gauche->combattants[p]->buff_defense_tours > 0) printf("DEF+ (%d) ", gauche->combattants[p]->buff_defense_tours);
            if (gauche->combattants[p]->buff_agilite_tours > 0) printf("AGI+ (%d) ", gauche->combattants[p]->buff_agilite_tours);
            if (gauche->combattants[p]->est_gele) printf("Gele (%d) ", gauche->combattants[p]->tours_gele);
            printf("\n");
        }
    }
    for (int p = 0; p < 3; p++) {
        if (droite->combattants[p] != NULL && droite->combattants[p]->pv > 0) {
            printf("D%d: ", p);
            if (droite->combattants[p]->poison_tours > 0) printf("Poison (%d) ", droite->combattants[p]->poison_tours);
            if (droite->combattants[p]->buff_defense_tours > 0) printf("DEF+ (%d) ", droite->combattants[p]->buff_defense_tours);
            if (droite->combattants[p]->buff_agilite_tours > 0) printf("AGI+ (%d) ", droite->combattants[p]->buff_agilite_tours);
            if (droite->combattants[p]->est_gele) printf("Gele (%d) ", droite->combattants[p]->tours_gele);
            printf("\n");
        }
    }
}
