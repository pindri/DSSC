#!/bin/bash

module load openmpi/1.8.3/gnu/4.9.2 
make

export OMP_NUM_THREADS=10
./exe.x
