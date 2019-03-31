CC = gcc
CFLAGS = -O0 -ggdb -fopenmp -std=c11
EXE = exe.x

SRC = loop_schedule.c

all: $(EXE)

$(EXE): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^

.PHONY: clean

clean:
	rm -f *.x *.dat
