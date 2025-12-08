#!/bin/bash

############################################
#Début du chronomètre
############################################

debut=$(date +%s)

############################################
#Vérif nbr arguments
############################################


if [ $# -lt 1 ] || [ $# -gt 3 ] then
	echo "Nombre d'arguments incorrect"
	exit 1
fi


############################################
#Valeur tmp existence usine
############################################

exist=1
DAT="$3"
if [ ! -f "$DAT" ] then
	echo "Le fichier de données '$DAT' n'existe pas"
	exit 23
fi

############################################
#Différents cas selon argument
############################################


case $1 in
	"histo")
		#vérifier deuxième argument existe
		if [ $# -ne 3 ] then
			echo "Il manque l'argument "
			exit 6
		fi
		
		mode=$2
		
    	case $mode in
        	max | src | real)
            	echo "Mode histogramme : $mode"
        	*)
            	echo "Argument incorrect : $mode"
            	exit 11
    	esac

		###############################################
	    # Filtrage du CSV selon le mode choisi
	    ###############################################
    	mkdir -p histoTout

    	if [ "$mode" = "max" ] then
        	echo "Filtrage des capacités maximales...."
	        # On extrait seulement les lignes décrivant les usines
	        awk -F';' '$4 != "-" && $3 == "-" { print $2 ";" $4 }' "$DAT" > histoTout/histo_max.txt
	        fichier_filtre="histoTout/histo_max.txt"
	        fichier_c="vol_max.dat"

    	elif [ "$mode" = "src" ] then
	        echo "Filtrage des volumes captés par les sources..."
	        # On extrait seulement les lignes SOURCE → USINE (Spring → Facility)
	        awk -F';' '$1 == "-" && $4 != "-" { print $2 ";" $3 ";" $4 }' "$DAT" > histoTout/histo_src.txt
	        fichier_filtre="histoTout/histo_src.txt"
	        fichier_c="vol_src.dat"

    	elif [ "$mode" = "real" ] then
	        echo "Filtrage des volumes réellement traités..."
	        # Même chose que src mais on garde aussi les fuites (colonne 5)
	        awk -F';' '$1 == "-" && $4 != "-" { print $2 ";" $3 ";" $4 ";" $5 }' "$DAT" > histoTout/histo_real.txt
	       	fichier_filtre="histoTout/histo_real.txt"
	        fichier_c="vol_real.dat"

    	fi


	    ###############################################
	    # Exécution du programme C avec le fichier filtré
	    ###############################################
		
	    echo "Exécution du programme C..."
	    ./prog histo "$mode" "$fichier_filtre" "$DAT" > "$fichier_c"
	
	    if [ $? -ne 0 ] then
	        echo "Erreur dans le programme C"
	        exit 12
	    fi

    	echo "Fichier généré par le C : $fichier_c"


	    ###############################################
	    # Génération de l'histogramme PNG via gnuplot
	    ###############################################
    	echo "Génération de l'image PNG..."

    	image="${fichier_c%.dat}.png"

    	gnuplot <<EOF
set terminal png size 1280,720
set output "$image"
set datafile separator ";"
set title "Histogramme $mode"
set xlabel "Usines"
set ylabel "Volume (k.m3.year-1)"
set xtics rotate by -45
plot "$fichier_c" using 2:xtic(1) with boxes title "$mode"
EOF

    	echo "Image créée : $image"

	"leaks")
	
		if [ $# -ne 2 ] then
			echo "Il manque l'identifiant de l'usine"
			exit 38
		fi

		id="$2"
		echo "Mode leaks pour l'usine dont l'identifiant est : $id"

		mkdir -p leaksTout

		echo "Recherche..."

		amont="leaksTout/leaks_amont.txt"
		aval="leaksTout/leaks_aval.txt"

		#Vider fichiers ou pas ?
		
		#Recherche des lignes utiles amont
		awk -F';' -v id="$id" '$1 == id { print }' "$DAT" >> "$amont"
		awk -F';' -v id="$id" '$2 == id { print }' "$DAT" >> "$amont"

		#Et dcp maintenant aval
		echo "Recherche pour les lignes aval"
		cut -d';' -f3 "$amont" | sort -u > leaksTout/aval_nodes.txt

		#Partie à demander à la prof
		while read node do
			awk -F';' -v n="$node" '$2 == n { print }' "$CSV"
		done < leaksTout/aval_nodes.txt > "$aval"

		#Programme C
		echo "Exécution partie en C pour calculs des fuites"
		res=$(./prog leaks "$id" "$aval" "$DAT")
		retour=$?

		if [ $ret -ne 0 ] then
			echo "Erreur dans l'exécution du C"
			exit 32
		fi

		echo "Le volume de fuite calculé pour cette usine est : $res"

		#On ajoute la recherche effectuée dans le fichier de l'historique
		echo "$id;$res" >> leaks.dat
		echo "Résultat ajouté à l'historique"

esac


############################################
#Exécutable donc à mettre avant ou après (demander à la prof)
############################################

make

############################################
#Fin du chronomètre et affichage du temps 
############################################

fin=$(date +%s)
echo "Temps total de l'exécution du script: $((fin-debut)) secondes"
