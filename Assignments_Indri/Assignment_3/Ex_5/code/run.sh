#!/bin/bash

module load openmpi

> plain.txt
> output.dat

N=$1
NP=$2

make -B
mpirun -np $NP ./identity.x $N

hexdump -v -e ''$N'/4 "%2d "' -e '"\n"' output.dat > plain.txt
