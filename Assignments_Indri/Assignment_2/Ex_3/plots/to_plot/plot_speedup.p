reset

set key right bottom
set xlabel "Number of processing nodes"
set ylabel "Speedup"
set title "Parallel computation of {/Symbol p}: speedup" enhanced

plot "processed_mpi.dat" u 1:3 w lp pointtype 7 ps 0.5 t "MPI", "processed_omp.dat" u 1:3 w lp pointtype 7 ps 0.5 t "OMP", x w l dashtype 2 t ""
