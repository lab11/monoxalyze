#!/usr/bin/gnuplot

reset
set terminal pngcairo size 6000,3000 enhanced font 'Verdana,10'
set output 'heatmap.png'

set border lw 1.5
set style line 1 lc rgb 'black' lt 1 lw 2

set rmargin screen 0.85

unset key
set cbrange [0:20]
set xrange[0:19]
set yrange[0:3000]
set palette rgbformulae -7,2,-7
plot 'heat.dat' u 1:2:3 with image

