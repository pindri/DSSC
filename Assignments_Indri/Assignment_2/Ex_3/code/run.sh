#!/bin/bash

module load openmpi/1.8.3/gnu/4.9.2 
module load openmpi
make -B
> output_mpi.dat
> output_omp.dat

for i in {1..40..1}
do
export OMP_NUM_THREADS=$i
./omp_pi.x >> output_omp.dat
mpirun -np $i ./mpi_pi.x >> output_mpi.dat
done
