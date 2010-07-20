LEVEL_SOURCES = \
	grid_base_defs.chpl \
	linked_list_defs.chpl \
	amr_level_defs.chpl

.PHONY: clean

DIM = 2


level_test.out: $(LEVEL_SOURCES)
	chpl $(LEVEL_SOURCES) -sdimension=$(DIM) -o level_test.out

clean:
	rm -f _output/fort.* _output/*.png output_* *.out