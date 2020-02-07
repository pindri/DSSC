#!/bin/bash

module load openmpi

N=$1
NP=$2

make -B
echo BLOCKING
mpirun -np $NP ./identity.x $N
echo NON BLOCKING
mpirun -np $NP ./identity_non_b.x $N

