CC = nvcc
EXE = transpose.x

SRC = src/transpose.cu

all: $(EXE)

$(EXE): $(SRC)
	$(CC) -o $@ $^

.PHONY: clean

clean:
	rm -f *.x *.dat
