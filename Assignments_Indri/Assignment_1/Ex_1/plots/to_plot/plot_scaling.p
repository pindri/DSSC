reset

set key right top
set xlabel "Number of processing nodes"
set ylabel "Execution time [s]"
set title "Paralle computation of {/Symbol p}: Execution time vs processes" enhanced

plot "plot.dat" u 1:3 w lp pointtype 7 ps 0.5 t "omp atomic", "" u 1:5 w lp pointtype 7 ps 0.5 t "omp reduction", "" u 1:7 w lp pointtype 7 ps 0.5 t "omp critical"
