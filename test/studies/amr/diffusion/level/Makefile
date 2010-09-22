MISC_LIB = ../../lib/misc
GRID_LIB = ../../lib/grid
LEVEL_LIB = ../../lib/level


LEVEL_SOURCES = \
	../grid/GridArray_DiffusionBE.chpl \
	LevelArray_DiffusionBE.chpl \
	LevelBC_DiffusionBE.chpl \
	LevelSolution_DiffusionBE.chpl \
	Level_DiffusionBE_driver.chpl


.PHONY: clean

DIM = 2


output: Level_DiffusionBE.out
	./Level_DiffusionBE.out

Level_DiffusionBE.out: $(LEVEL_SOURCES)
	chpl $(LEVEL_SOURCES) -M $(MISC_LIB) -M $(GRID_LIB) -M $(LEVEL_LIB) --set dimension=$(DIM) -o Level_DiffusionBE.out

clean:
	rm -f _output/fort.* _output/*.png output_* *.out