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

############################################
# SIMULATION du programme C
############################################
# (juste pour tester le gnuplot)
cp "$filtered" "$outfile"

############################################
# GENERATION PNG
############################################
image="${outfile%.dat}.png"

gnuplot <<EOF
set terminal png size 1280,720
set output "$image"
set datafile separator ";"
set title "Histogramme $mode"
set xlabel "Usines"
set ylabel "Volume (k.m3.year-1)"
set xtics rotate by -45
plot "$outfile" using 2:xtic(1) with boxes title "$mode"
EOF

echo "Image créée : $image"

