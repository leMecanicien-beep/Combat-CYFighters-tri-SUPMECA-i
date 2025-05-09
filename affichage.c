#include <stdio.h>
#include <string.h>
#include "affichage.h"
#include "combattant.h"

void genererLigne(Combattant* c, char lignes[6][100]) {
    if (c == NULL || c->pv <= 0) {
        for (int i = 0; i < 6; i++) {
            strcpy(lignes[i], "                    ");
        }
        return;
    }

    int pourcentage = (c->pv * 100) / c->pv_max;
    const char* couleur;
    if (pourcentage > 60) couleur = "\033[32m"; // vert
    else if (pourcentage > 30) couleur = "\033[33m"; // jaune
    else couleur = "\033[31m"; // rouge

    char barre[30] = "";
    strcat(barre, couleur);
    strcat(barre, "[");
    int nbBarres = (c->pv * 10) / c->pv_max;
    for (int i = 0; i < 10; i++) {
        strcat(barre, (i < nbBarres) ? "█" : " ");
    }
    strcat(barre, "]");
    strcat(barre, "\033[0m");

    snprintf(lignes[0], 100, "╭────────────╮");
    snprintf(lignes[1], 100, "│ %-10s │", c->nom);
    snprintf(lignes[2], 100, "│ PV: %-13s│", barre);
    snprintf(lignes[3], 100, "│ ATK: %-6d │", c->attaque);
    snprintf(lignes[4], 100, "│ DEF: %-5d%% │", c->defense);
    snprintf(lignes[5], 100, "╰────────────╯");
}

void afficherEquipesCoteACote(Equipe* gauche, Equipe* droite) {
    char lignesG[3][6][100];
    char lignesD[3][6][100];

    for (int i = 0; i < 3; i++) {
        genererLigne((i < gauche->nbCombattants) ? gauche->combattants[i] : NULL, lignesG[i]);
        genererLigne((i < droite->nbCombattants) ? droite->combattants[i] : NULL, lignesD[i]);
    }

    for (int ligne = 0; ligne < 6; ligne++) {
        for (int p = 0; p < 3; p++) {
            printf("%s  ", lignesG[p][ligne]);
        }
        printf("       "); // séparation entre les équipes
        for (int p = 0; p < 3; p++) {
            printf("%s  ", lignesD[p][ligne]);
        }
        printf("\n");
    }
}
