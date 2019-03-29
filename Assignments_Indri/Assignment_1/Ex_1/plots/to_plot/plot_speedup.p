reset

set key right bottom
set xlabel "Number of processing nodes"
set ylabel "Speedup"
set title "Paralle computation of {/Symbol p}: speedup vs processes" enhanced

plot "speedup.dat" u 1:2 w lp pointtype 7 ps 0.5 t "omp atomic", "" u 1:3 w lp pointtype 7 ps 0.5 t "omp reduction", "" u 1:4 w lp pointtype 7 ps 0.5 t "omp critical", x w l dashtype 2 t ""
