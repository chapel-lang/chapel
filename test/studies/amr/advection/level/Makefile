MISC_LIB = ../../lib/misc
GRID_LIB = ../../lib/grid
LEVEL_LIB = ../../lib/level


SOURCES = \
	../grid/GridArray_AdvectionCTU.chpl \
	LevelArray_AdvectionCTU.chpl \
	LevelSolution_AdvectionCTU.chpl \
	LevelBC_AdvectionCTU.chpl \
	Level_AdvectionCTU_driver.chpl


.PHONY: clean

DIM = 2


output: Level_AdvectionCTU.out
	./Level_AdvectionCTU.out

Level_AdvectionCTU.out: $(SOURCES)
	chpl $(SOURCES) -M $(MISC_LIB) -M $(GRID_LIB) -M $(LEVEL_LIB) --set dimension=$(DIM) -o Level_AdvectionCTU.out

clean:
	rm -f _output/fort.* _output/*.png output_* *.out