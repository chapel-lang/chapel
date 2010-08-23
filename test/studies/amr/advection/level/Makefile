SOURCES = \
	level_ctu_defs.chpl \
	level_ctu_driver.chpl

GRID_LIB = ../../lib/grid
LEVEL_LIB = ../../lib/level


.PHONY: clean

DIM = 2


output: level_test.out
	./level_test.out

level_test.out: $(SOURCES)
	chpl $(SOURCES) -M $(GRID_LIB) -M $(LEVEL_LIB) --set dimension=$(DIM) -o level_test.out

clean:
	rm -f _output/fort.* _output/*.png output_* *.out