Etapes de compilation pour lancer notre jeu CY_Fighters :
1.Sous Windows :  Télécharger le dossier zip du code disponible sur https://github.com/leMecanicien-beep/Combat-CYFighters-tri-SUPMECA-i (pour maintenant on nomme ce dossier zip ZIP);
                  Extraire tout dans un dossier normal sur le bureau par exemple (pour cet exemple on nomme ce dossier TEST);
                  Entrer dans ce dossier TEST et normalement on trouve le dossier ZIP décompressé;
                  Copier le dossier ZIP décompréssé en tant que chemin d'accès;
                  Appuyer Windows + R (puis taper "cmd" et puis OK -> ouvre le terminal de commande);
                  Taper "cd" puis espace puis coller le chemin d'accès puis entrée;
                  Taper "gcc affichage.c combat.c combattant.c equipe.c fichier.c ia.c main.c technique.c utilitaire.c" ( -> créé un exécutable dans le dossier ZIP décompréssé);
                  Entrer dans cet exécutable puis le jeu se lance.
2.Sous Linux :    Appuyer Alt + F2;
                  Taper gnome-terminal puis entrée ( -> ouvre le terminal);
                  Taper git clone https://github.com/leMecanicien-beep/Combat-CYFighters-tri-SUPMECA-i puis entrée;
                  Taper gcc -o jeu (-> créé un exécutable appelé jeu);
                  Taper ./jeu .

Fonctionnement du jeu : Notre jeu de combat oppose une équipe de trois combattants que vous pourrez choisir contre une équipe gérée par l'ordinateur composée des autres combattants. La première équipe dont tous les combattants meurent en premier perd et l'autre équipe remporte la partie. Vous pourrez choisir un nom à votre équipe au tout début de la partie et vous pourrez rejouer si vous perdez par malheur contre un ordinateur moins intelligent que vous (no offense ;) ). Le jeu est très simple à chaque tour vous aurez l'état des équipes et à chaque tour vous aurez le choix entre attaquer une cible choisie ou utiliser une technique spéciale. Chaque combattant possède plusieurs caractéristiques (nom, attaque, défense en pourcentage -> si un perso a 10% de défense il ne recevra à chaque fois que 90% des dégâts de base infligés, esquive -> agilité exprimée par un entier, plus il est grand plus le perso a moins de chances d'esquiver l'attaque, et par souci d'équité le premier affichage des combattants vous montre une barre de PV en # identique pour chaque perso et ne vous montre pas les techniques spéciales de chaque combattant -> au premier tour, vous prendrez connaissance des réels PV de vos combattants et de ceux de l'adversaire et des techniques spéciales de vos combattants et celles adverses). Le jeu est très instinctif vous comprendrez la mécanique des choix sur le moment (mais en général vous devez séléctionner un chiffre pour faire un choix, les mécaniques sont très simples).
