MISC_LIB = ../../lib/misc
GRID_LIB = ../../lib/grid
LEVEL_LIB = ../../lib/level
AMR_LIB = ../../lib/amr

SOURCES = \
	../grid/GridArray_AdvectionCTU.chpl \
	../level/LevelArray_AdvectionCTU.chpl \
	../level/LevelSolution_AdvectionCTU.chpl \
	AMRSolution_AdvectionCTU.chpl \
	AMRBC_AdvectionCTU.chpl \
	AMR_AdvectionCTU_driver.chpl


.PHONY: clean

DIM = 2


output: _output AMRAdvectionCTU.out
	./AMRAdvectionCTU.out

AMRAdvectionCTU.out: $(SOURCES)
	chpl $(SOURCES) -M $(MISC_LIB) -M $(GRID_LIB) -M $(LEVEL_LIB) -M $(AMR_LIB) --set dimension=$(DIM) -o AMRAdvectionCTU.out

_output:
	mkdir _output
	cp ../../lib/plot.claw _output

clean:
	rm -rf _output/ *.out
