MPICC = mpicc
MPICCFLAGS = -O0 -ggdb -std=c11

SRC = identity.c
EXE = identity.x

all: $(EXE)

$(EXE): $(SRC)
	$(MPICC) $(MPICCFLAGS) -o $@ $^

.PHONY: clean

clean:
	rm -f *.x *.dat *.txt
