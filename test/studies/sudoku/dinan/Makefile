CC=chpl
CCFLAGS=-O --no-bounds-checks

TARGETS=sudoku sudoku-smart

all: $(TARGETS)

sudoku: sudoku.chpl
	$(CC) $(CCFLAGS) -o $@ $+

sudoku-smart: sudoku-smart.chpl
	$(CC) $(CCFLAGS) -o $@ $+

clean:
	rm -f $(TARGETS)
