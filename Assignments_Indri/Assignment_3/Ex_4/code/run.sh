#!/bin/bash

module load openmpi
N=$1

make -B
echo NON BLOCKING
mpirun -np $N ./ring_sum_non_b.x
echo BLOCKING
mpirun -np $N ./ring_sum_b.x
