#!/bin/sh

module load cudatoolkit

> output.dat

make -B

for i in 64 256 512 1024 
do
./transpose.x $i >> output.dat
done

