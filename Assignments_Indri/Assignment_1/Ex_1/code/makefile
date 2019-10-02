CC = gcc
CFLAGS = -O0 -ggdb -fopenmp -std=c11 -I$(IDIR)
EXE = pi.x
BENCHMARK = benchmark.x
D_FLAG = BENCHMARK

SRC = src/pi.c src/cptimer.c
IDIR = include


all: $(EXE)

benchmark: $(BENCHMARK)

$(EXE): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^

$(BENCHMARK): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ -D$(D_FLAG)

.PHONY: clean

clean:
	rm -f *.x *.dat
