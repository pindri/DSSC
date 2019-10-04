#!/bin/sh

module load cudatoolkit

make -B

for i in 2 4 8 16 32
do
  for j in 2 4 8 16 32
  do
    ./transpose.x $i $j
  done
done

