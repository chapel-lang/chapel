MISC_LIB = ../../lib/misc
GRID_LIB = ../../lib/grid
LEVEL_LIB = ../../lib/level
CF_LIB = ../../lib/cfboundary
AMR_LIB = ../../lib/amr

SOURCES = \
	../grid/GridVariable_AdvectionCTU.chpl \
	../level/LevelVariable_AdvectionCTU.chpl \
	../level/LevelSolution_AdvectionCTU.chpl \
	AMRHierarchy_AdvectionCTU.chpl \
	AMRBC_AdvectionCTU.chpl \
	AMR_AdvectionCTU_driver.chpl


.PHONY: clean touch

DIM = 2



output: _output AMRAdvectionCTU.out
	./AMRAdvectionCTU.out

AMRAdvectionCTU.out: $(SOURCES)
	chpl $(SOURCES) -M $(MISC_LIB) -M $(GRID_LIB) -M $(LEVEL_LIB) -M $(CF_LIB) -M $(AMR_LIB) --set dimension=$(DIM) -o AMRAdvectionCTU.out

touch:
	python ../../lib/python/touch_output.py

_output:
	mkdir _output
	cp ../../lib/plot.claw _output

clean:
	rm -rf _output/ *.out
