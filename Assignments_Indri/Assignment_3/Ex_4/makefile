MPICC = mpicc
MPICCFLAGS = -O0 -ggdb -std=c11

SRC = ring_sum.c
EXE = ring_sum.x

all: $(EXE)

$(EXE): $(SRC)
	$(MPICC) $(MPICCFLAGS) -o $@ $^

.PHONY: clean

clean:
	rm -f *.x *.dat *.txt
