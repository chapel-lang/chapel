SOURCES = \
	amr_base_defs.chpl \
	amr_array_defs.chpl \
	amr_array_test.chpl

GRID_LIB = ../grid
LEVEL_LIB = ../level


.PHONY: clean

DIM = 2


amr_test.out: $(SOURCES)
	chpl $(SOURCES) -M $(GRID_LIB) -M $(LEVEL_LIB) --set dimension=$(DIM) -o amr_test.out

clean:
	rm -f *.out