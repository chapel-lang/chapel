MISC_LIB = ../../lib/misc
GRID_LIB = ../../lib/grid
LEVEL_LIB = ../../lib/level


SOURCES = \
	../grid/GridVariable_AdvectionCTU.chpl \
	LevelVariable_AdvectionCTU.chpl \
	LevelSolution_AdvectionCTU.chpl \
	LevelBC_AdvectionCTU.chpl \
	Level_AdvectionCTU_driver.chpl


.PHONY: clean

DIM = 2


output: _output Level_AdvectionCTU.out
	./Level_AdvectionCTU.out

Level_AdvectionCTU.out: $(SOURCES)
	chpl $(SOURCES) -M $(MISC_LIB) -M $(GRID_LIB) -M $(LEVEL_LIB) --set dimension=$(DIM) -o Level_AdvectionCTU.out

_output:
	mkdir _output
	cp ../../lib/plot.claw _output

clean:
	rm -rf _output/ *.out

