#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "affichage.h"
#include "combattant.h"

void genererBarrePV(int pv, int pv_max, char* sortie) {
    if(sortie == NULL){
        exit(10);
    }
    int nb = (pv * 10) / pv_max;
    for (int i = 0; i < 10; i++) {
        sortie[i] = (i < nb) ? '#' : '.';
    }
    sortie[10] = '\0';
}

void genererLigne(Combattant* c, char lignes[7][100]) {
    if (c == NULL || c->pv <= 0) {
        for (int i = 0; i < 7; i++) {
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
    snprintf(lignes[5], 100, "| AGI: %-6d |", c->agilite);
    snprintf(lignes[6], 100, "+--------------+");
}

void afficherEquipesCoteACote(Equipe* gauche, Equipe* droite) {
    if(gauche == NULL || droite == NULL){
        exit(20);
    }
    char lignesG[3][7][100];
    char lignesD[3][7][100];

    for (int i = 0; i < 3; i++) {
        genererLigne((i < gauche->nbCombattants) ? gauche->combattants[i] : NULL, lignesG[i]);
        genererLigne((i < droite->nbCombattants) ? droite->combattants[i] : NULL, lignesD[i]);
    }

    printf("\n=== ETAT DES EQUIPES ===\n\n");
    for (int ligne = 0; ligne < 7; ligne++) {
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

void afficherCarteCombattant(Combattant* c) {
    if(c == NULL){
        exit(30);
    }
    char lignes[7][100];
    genererLigne(c, lignes);
    for (int i = 0; i < 7; i++) printf("%s\n", lignes[i]);
}

void afficherChoixEquipe(Combattant* tous, int nb) {
    if(tous == NULL){
        exit(40);
    }
    printf("\n=== Choisissez 3 combattants pour votre équipe ===\n\n");
    for (int i = 0; i < nb; i++) {
        printf("%d :\n", i);
        afficherCarteCombattant(&tous[i]);
        printf("\n");
    }
}
