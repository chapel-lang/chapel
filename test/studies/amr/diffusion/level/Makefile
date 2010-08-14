LEVEL_SOURCES = \
	../../lib/grid/grid_base_defs.chpl \
	../../lib/grid/grid_solution_defs.chpl \
	../../lib/grid/grid_bc_defs.chpl \
	../../lib/level/level_base_defs.chpl \
	../../lib/level/level_solution_defs.chpl \
	../../lib/level/level_bc_defs.chpl \
	level_diffusion_defs.chpl \
	level_diffusion_driver.chpl



.PHONY: clean

DIM = 2


output: level_test.out
	./level_test.out

level_test.out: $(LEVEL_SOURCES)
	chpl $(LEVEL_SOURCES) -sdimension=$(DIM) -o level_test.out

clean:
	rm -f _output/fort.* _output/*.png output_* *.out