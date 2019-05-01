reset

set key right top
set xlabel "Number of processing nodes"
set ylabel "Execution time [s]"
set title "Parallel computation of {/Symbol p}: Execution time vs processes" enhanced

plot "processed_mpi.dat" u 1:2 w lp pointtype 7 ps 0.5 t "MPI", "processed_omp.dat" u 1:2 w lp pointtype 7 ps 0.5 t "OMP"
