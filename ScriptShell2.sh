#!/bin/bash

# Vérification arguments
if [ $# -ne 3 ]; then
    echo "Usage : $0 histo <max|src|real> <fichier.dat>"
    exit 1
fi

mode="$2"
DATAFILE="$3"

# Vérifier fichier .dat
if [ ! -f "$DATAFILE" ]; then
    echo "Le fichier '$DATAFILE' est introuvable."
    exit 2
fi

# Vérification mode
case $mode in
    max | src | real)
        echo "Histogramme demandé : $mode"
        ;;
    *)
        echo "Mode histo invalide : $mode"
        exit 3
        ;;
esac

mkdir -p filtres

############################################
# FILTRAGE
############################################

if [ "$mode" = "max" ]; then

    awk -F';' '$4 != "-" && $3 == "-" { print $2 ";" $4 }' "$DATAFILE" > filtres/histo_max.txt
    filtered="filtres/histo_max.txt"
    outfile="vol_max.dat"

elif [ "$mode" = "src" ]; then

    awk -F';' '$1 == "-" && $4 != "-" { print $2 ";" $3 ";" $4 }' "$DATAFILE" > filtres/histo_src.txt
    filtered="filtres/histo_src.txt"
    outfile="vol_src.dat"

elif [ "$mode" = "real" ]; then

    awk -F';' '$1 == "-" && $4 != "-" { print $2 ";" $3 ";" $4 ";" $5 }' "$DATAFILE" > filtres/histo_real.txt
    filtered="filtres/histo_real.txt"
    outfile="vol_real.dat"

fi

if [ "$mode" = "max" ]; then

        echo "Préparation des tris pour histo max..."

        # tri croissant (petites usines)
        sort -t';' -k2,2n "$outfile" > filtres/tri_croissant.txt

        # tri décroissant (grandes usines)
        sort -t';' -k2,2nr "$outfile" > filtres/tri_decroissant.txt

        # 50 plus petites
        head -n 50 filtres/tri_croissant.txt > filtres/top50_small.txt

        # 10 plus grandes
        head -n 10 filtres/tri_decroissant.txt > filtres/top10_big.txt

        # Image pour les 50 plus petites
        gnuplot <<EOF
set terminal png size 1280,720
set output "vol_max_small.png"
set datafile separator ";"
set title "50 plus petites usines"
set xlabel "Usines"
set ylabel "Capacité (k.m3/an)"
set xtics rotate by -45
plot "filtres/top50_small.txt" using 2:xtic(1) with boxes title "petites"
EOF

        echo "Image créée : vol_max_small.png"

        # Image pour les 10 plus grandes
        gnuplot <<EOF
set terminal png size 1280,720
set output "vol_max_big.png"
set datafile separator ";"
set title "10 plus grandes usines"
set xlabel "Usines"
set ylabel "Capacité (k.m3/an)"
set xtics rotate by -45
plot "filtres/top10_big.txt" using 2:xtic(1) with boxes title "grandes"
EOF

        echo "Image créée : vol_max_big.png"

    else

        ###########################################################
        # IMAGE CLASSIQUE POUR SRC ET REAL
        ###########################################################

        image="${outfile%.dat}.png"

        gnuplot <<EOF
set terminal png size 1280,720
set output "$image"
set datafile separator ";"
set title "Histogramme $mode"
set xlabel "Usines"
set ylabel "Volume (k.m3/an)"
set xtics rotate by -45
plot "$outfile" using 2:xtic(1) with boxes title "$mode"
EOF

        echo "Image créée : $image"

    fi

    ;;

