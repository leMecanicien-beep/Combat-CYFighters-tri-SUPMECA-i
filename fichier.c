#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fichier.h"
#include "technique.h"

// Fonction utilitaire : lire un champ jusqu’à un ';' ou fin de ligne
void lireChamp(char* dest, const char* ligne, int* i) {
    int j = 0;
    while (ligne[*i] != ';' && ligne[*i] != '\0' && ligne[*i] != '\n') {
        dest[j++] = ligne[*i];
        (*i)++;
    }
    dest[j] = '\0';
    if (ligne[*i] == ';') {
        (*i)++; // On saute la ponctuation
    }
}

Combattant* chargerCombattants(const char* nomFichier, int* nb) {
    FILE* f = fopen(nomFichier, "r");
    if (f==NULL) {
        printf("Erreur : impossible d’ouvrir le fichier %s\n", nomFichier);
        *nb = 0;
    }

    Combattant* tab = malloc(6 * sizeof(Combattant));  // on sait qu’il y en a 6
    char ligne[300];
    *nb = 0;

    while (fgets(ligne, sizeof(ligne), f) && *nb < 6) {
        int i = 0;
        char champ[100];

        // Lecture champ par champ
        lireChamp(tab[*nb].nom, ligne, &i);               // nom
        lireChamp(champ, ligne, &i); tab[*nb].pv_max = atoi(champ); tab[*nb].pv = tab[*nb].pv_max;
        lireChamp(champ, ligne, &i); tab[*nb].attaque = atoi(champ);
        lireChamp(champ, ligne, &i); tab[*nb].defense = atoi(champ);
        lireChamp(champ, ligne, &i); tab[*nb].agilite = atoi(champ);
        lireChamp(champ, ligne, &i);                      // nom technique
        char nomTech[50]; strcpy(nomTech, champ);
        lireChamp(champ, ligne, &i);                      // valeur (vide possible)
        int valeur = (champ[0] == '\0') ? 0 : atoi(champ);
        lireChamp(champ, ligne, &i); int duree = atoi(champ);
        lireChamp(champ, ligne, &i); int cooldown = atoi(champ);
        lireChamp(champ, ligne, &i);                      // description
        char descTech[100]; strcpy(descTech, champ);

        // Initialiser la technique
        initialiserTechnique(&tab[*nb].technique, nomTech, valeur, duree, cooldown, descTech);

        // Initialiser les effets temporaires à 0
        tab[*nb].poison_tours = 0;
        tab[*nb].poison_degats = 0;
        tab[*nb].buff_defense = 0;
        tab[*nb].buff_defense_tours = 0;
        tab[*nb].buff_agilite = 0;
        tab[*nb].buff_agilite_tours = 0;
        tab[*nb].est_gele = 0;
        tab[*nb].tours_gele = 0;

        (*nb)++;
    }

    fclose(f);
    return tab;
}

