#include <stdio.h>
#include <string.h>
#include "equipe.h"

// Initialise une équipe avec un nom
void initialiserEquipe(Equipe *e, const char *nom) {
    if(e == NULL || nom == NULL){
        
    strncpy(e->nom, nom, 29);
    e->nom[29] = '\0';
    e->nbCombattants = 0;
}

// Ajoute un combattant à l’équipe
void ajouterCombattant(Equipe *e, Combattant *c) {
    if (e->nbCombattants < NB_EQUIPE_MAX) {
        e->combattants[e->nbCombattants] = c;
        e->nbCombattants++;
    } else {
        printf("Erreur : l’équipe %s est déjà complète.\n", e->nom);
    }
}

// Renvoie 1 si tous les combattants sont KO, sinon 0
int equipeEstKO(Equipe *e) {
    int tousKO = 1;
    for (int i = 0; i < e->nbCombattants; i++) {
        if (e->combattants[i]->pv > 0) {
            tousKO = 0;
        }
    }
    return tousKO;
}
