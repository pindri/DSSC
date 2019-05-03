#!/bin/bash

module load openmpi
N=$1

make -B
mpirun -np $N ./ring_sum.x
