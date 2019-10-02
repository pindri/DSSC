MPICC = mpicc
MPICFLAGS = -O0 -ggdb -std=c11
CC = gcc
CFLAGS = -O0 -fopenmp -ggdb -std=c11

MPI = mpi_pi.x
OMP = omp_pi.x

all: $(MPI) $(OMP)

SRC_MPI = src/mpi_pi.c 
SRC_OMP = src/omp_pi.c 

$(MPI): $(SRC_MPI)
	$(MPICC) $(MPICFLAGS) -o $@ $^

$(OMP): $(SRC_OMP)
	$(CC) $(CFLAGS) -o $@ $^

.PHONY: clean

clean:
	rm -f *.x *.dat
