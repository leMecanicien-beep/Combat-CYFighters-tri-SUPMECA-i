int main() {
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
        printf("Entrez le nom de votre équipe : ");
        fgets(nomEquipe, sizeof(nomEquipe), stdin);
        size_t len = strlen(nomEquipe);
        if (len > 0 && nomEquipe[len - 1] == '\n') {
            nomEquipe[len - 1] = '\0';
        }

        // Création des équipes
        Equipe joueur, ia;
        initialiserEquipe(&joueur, nomEquipe);
        initialiserEquipe(&ia, "IA");

        int pris[6] = {0};

        // Affichage des combattants disponibles
        printf("\nChoisissez 3 combattants pour votre équipe :\n");
        for (int i = 0; i < nbCombattants; i++) {
            printf("%d - ", i);
            afficherCombattant(&tous[i]);
        }

        // Sélection du joueur
        for (int c = 0; c < 3; c++) {
            int choix;
            do {
                choix = demanderEntier("Votre choix : ", 0, nbCombattants - 1);
                if (pris[choix]) {
                    printf("Ce combattant est déjà pris. Choisissez-en un autre.\n");
                }
            } while (pris[choix]);

            ajouterCombattant(&joueur, &tous[choix]);
            pris[choix] = 1;
        }

        // Sélection IA
        for (int i = 0; i < nbCombattants; i++) {
            if (!pris[i] && ia.nbCombattants < 3) {
                ajouterCombattant(&ia, &tous[i]);
                pris[i] = 1;
            }
        }

        printf("\n--- Début du combat ! ---\n");

        int vainqueur = 0;

        while (!equipeEstKO(&joueur) && !equipeEstKO(&ia)) {
            printf("\n--- Tour du Joueur ---\n");
            jouerTourJoueur(&joueur, &ia);

            if (equipeEstKO(&ia)) {
                vainqueur = 1;
                break;
            }

            printf("\n--- Tour de l’IA ---\n");
            jouerTourIA(&ia, &joueur);

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

        printf("\n--- Fin du combat ---\n");

        if (vainqueur == 1) {
            printf("🎉 Vous avez gagné !\n");
        } else if (vainqueur == 2) {
            printf("🤖 L’IA a gagné !\n");
        } else {
            printf("Match nul ou erreur inattendue.\n");
        }

        free(tous);

        printf("\nSouhaitez-vous rejouer ? (1 = Oui, 0 = Non) : ");
        scanf("%d", &rejouer);
        while (getchar() != '\n'); // vide le buffer
    }

    printf("Merci d’avoir joué ! À bientôt.\n");
    return 0;
}
