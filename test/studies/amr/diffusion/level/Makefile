LEVEL_SOURCES = \
	../grid/GridArray_diffusion.chpl \
	LevelArray_diffusion.chpl \
	LevelBC_diffusion.chpl \
	LevelSolution_diffusion.chpl \
	Level_diffusion_driver.chpl

GRID_LIB = ../../lib/grid
LEVEL_LIB = ../../lib/level

.PHONY: clean

DIM = 2


output: level_test.out
	./level_test.out

level_test.out: $(LEVEL_SOURCES)
	chpl $(LEVEL_SOURCES) -M $(GRID_LIB) -M $(LEVEL_LIB) --set dimension=$(DIM) -o level_test.out

clean:
	rm -f _output/fort.* _output/*.png output_* *.out