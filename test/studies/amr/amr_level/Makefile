LEVEL_SOURCES = \
	../lib/grid_base_defs.chpl \
	amr_level_defs.chpl \
	static_function_test.chpl

.PHONY: clean

DIM = 2


level_test.out: $(LEVEL_SOURCES)
	chpl $(LEVEL_SOURCES) -sdimension=$(DIM) -o level_test.out

clean:
	rm -f _output/fort.* _output/*.png output_* *.out