MISC_LIB = ../../lib/misc
GRID_LIB = ../../lib/grid
LEVEL_LIB = ../../lib/level
CF_LIB = ../../lib/cfboundary
SMR_LIB = ../../lib/static_mr

SOURCES = \
	../grid/GridVariable_AdvectionCTU.chpl \
	../level/LevelVariable_AdvectionCTU.chpl \
	../level/LevelSolution_AdvectionCTU.chpl \
	StaticMRSolution_AdvectionCTU.chpl \
	StaticMRBC_AdvectionCTU.chpl \
	StaticMR_AdvectionCTU_driver.chpl


.PHONY: clean

DIM = 2


output: _output StaticMRAdvectionCTU.out
	./StaticMRAdvectionCTU.out

StaticMRAdvectionCTU.out: $(SOURCES)
	chpl $(SOURCES) -M $(MISC_LIB) -M $(GRID_LIB) -M $(LEVEL_LIB) -M $(CF_LIB) -M $(SMR_LIB) --set dimension=$(DIM) -o StaticMRAdvectionCTU.out

_output:
	mkdir _output
	cp ../../lib/plot.claw _output

clean:
	rm -rf _output/ *.out
