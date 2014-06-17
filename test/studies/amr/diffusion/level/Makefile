MISC_LIB = ../../lib/misc
GRID_LIB = ../../lib/grid
LEVEL_LIB = ../../lib/level

LEVEL_SOURCES = \
	../grid/GridVariable_DiffusionBE.chpl \
	LevelVariable_DiffusionBE.chpl \
	LevelBC_DiffusionBE.chpl \
	LevelSolution_DiffusionBE.chpl \
	Level_DiffusionBE_driver.chpl

.PHONY: clean

DIM = 2

output: _output Level_DiffusionBE.out
	./Level_DiffusionBE.out

Level_DiffusionBE.out: $(LEVEL_SOURCES)
	chpl $(LEVEL_SOURCES) -M $(MISC_LIB) -M $(GRID_LIB) -M $(LEVEL_LIB) --set dimension=$(DIM) -o Level_DiffusionBE.out

_output:
	mkdir _output
	cp ../../lib/plot.claw _output

clean:
	rm -rf _output/ *.out
