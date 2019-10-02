MPICC = mpicc
MPICCFLAGS = -O0 -ggdb -std=c11
SRC = src

all: ring_sum_b.x ring_sum_non_b.x

%.x: $(SRC)/%.c
	$(MPICC) $(MPICCFLAGS) -o $@ $<

.PHONY: clean

clean:
	rm -f *.x *.dat *.txt
