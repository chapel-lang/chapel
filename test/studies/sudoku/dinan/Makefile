CHPL      = chpl
CHPLFLAGS = -O --no-checks

TARGETS   = sudoku sudoku-smart sudoku-simple

all: $(TARGETS)

sudoku: sudoku.chpl
	$(CHPL) $(CHPLFLAGS) -o $@ $+

sudoku-smart: sudoku-smart.chpl
	$(CHPL) $(CHPLFLAGS) -o $@ $+

sudoku-simple: sudoku-simple.chpl
	$(CHPL) $(CHPLFLAGS) -o $@ $+

test:
	@$(MAKE) CHPL=$(CHPL) > /dev/null
	./sudoku-simple -nl 1 easy.dat

clean:
	rm -f $(TARGETS)
