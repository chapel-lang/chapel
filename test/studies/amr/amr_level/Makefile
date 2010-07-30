LEVEL_SOURCES = \
	../lib/grid_base_defs.chpl \
	../lib/grid_ctu_defs.chpl \
	../lib/level_base_defs.chpl \
	../lib/level_ctu_defs.chpl \
	level_ctu_driver.chpl


.PHONY: clean

DIM = 2


output: level_test.out
	./level_test.out

level_test.out: $(LEVEL_SOURCES)
	chpl $(LEVEL_SOURCES) -sdimension=$(DIM) -o level_test.out

clean:
	rm -f _output/fort.* _output/*.png output_* *.out