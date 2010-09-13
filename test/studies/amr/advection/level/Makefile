SOURCES = \
	level_advection_defs.chpl \
	level_advection_driver.chpl

GRID_LIB = ../../lib/grid
LEVEL_LIB = ../../lib/level


.PHONY: clean

DIM = 2


output: level_advection.out
	./level_advection.out

level_advection.out: $(SOURCES)
	chpl $(SOURCES) -M $(GRID_LIB) -M $(LEVEL_LIB) --set dimension=$(DIM) -o level_advection.out

clean:
	rm -f _output/fort.* _output/*.png output_* *.out