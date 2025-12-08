#!/bin/bash

############################################
#Début du chronomètre
############################################

debut=$(date +%s)

############################################
#Vérif nbr arguments
############################################


if [ $# -lt 1 ] || [ $# -gt 3 ]
then
	echo "Nombre d'arguments incorrect"
	exit 1
fi


############################################
#Valeur tmp existence usine
############################################

exist=1
DAT="$3"
if [ ! -f "$DAT" ]
then
	echo "Problème dans la récupération du fichier '$DAT' "
	exit 23
fi

############################################
#Différents cas selon argument
############################################


if [ $# -ne 3 ]
then
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

if [ "$mode" = "max" ]
then
    echo "Filtrage des capacités maximales...."
	# On extrait seulement les lignes décrivant les usines
	awk -F';' '$4 != "-" && $3 == "-" { print $2 ";" $4 }' "$DAT" > histoTout/histo_max.txt
	fichier_filtre="histoTout/histo_max.txt"
	fichier_c="vol_max.dat"
    echo "Tri des usines pour histo max..."

    # Tri en ordre décroissant (plus grandes → plus petites)
    sort -t';' -k2,2nr "$fichier_c" > histoTout/tri_decroissant.txt

    head -n 10 histoTout/tri_decroissant.txt > histoTout/top10_grand.txt
    tail -n 50 histoTout/tri_decroissant.txt > histoTout/top50_petit.txt

    gnuplot <<EOF
set terminal png size 1000,600
set style data histograms
set output "vol_max_small.png"
set datafile separator ";"
set title "50 plus petites usines"
set style fill solid 0.8
set boxwidth 0.5
set xlabel "Usines"
set ylabel "Capacité (k.m3/year)"
set xtics rotate by -45 font ',8'
plot "histoTout/top50_petit.txt" using 2:xtic(1) with boxes title "petites"
EOF

    echo "Image vol_max_small.png créée"

    gnuplot <<EOF
set terminal png size 1000,600
set style data histograms
set output "vol_max_big.png"
set datafile separator ";"
set title "10 plus grandes usines"
set style fill solid 0.8
set boxwidth 0.5
set xlabel "Usines"
set ylabel "Capacité (k.m3/year)"
set xtics rotate by -45 font ',8'
plot "histoTout/top10_grand.txt" using 2:xtic(1) with boxes title "grandes"
EOF

    echo "Image vol_max_big.png créée"
