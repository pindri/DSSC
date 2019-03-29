#!/bin/bash

module load openmpi/1.8.3/gnu/4.9.2 
gcc -fopenmp pi.c cptimer.c -o exe.x -std=c11
> Output.dat

for j in {1..4..1}
do
for i in {1..20..1}
do
export OMP_NUM_THREADS=$i
./exe.x >> Output.dat
done
done
