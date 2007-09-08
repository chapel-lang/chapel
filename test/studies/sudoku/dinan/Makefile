CHPL=chpl
CHPLFLAGS=-O --no-bounds-checks

MAKEFLAGS = --no-print-directory
TARGETS   = sudoku sudoku-smart sudoku-simple

all: $(TARGETS)

sudoku: sudoku.chpl
	$(CHPL) $(CHPLFLAGS) -o $@ $+

sudoku-smart: sudoku-smart.chpl
	$(CHPL) $(CHPLFLAGS) -o $@ $+

sudoku-simple: sudoku-simple.chpl
	$(CHPL) $(CHPLFLAGS) -o $@ $+

test: all
	./sudoku-simple easy.dat

clean:
	rm -f $(TARGETS)
