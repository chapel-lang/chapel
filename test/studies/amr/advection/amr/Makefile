SOURCES = \
	../grid/GridArray_advection.chpl \
	../level/LevelArray_advection.chpl \
	../level/LevelSolution_advection.chpl \
	AMRSolution_advection.chpl \
	AMRBC_advection.chpl \
	AMR_advection_driver.chpl

GRID_LIB = ../../lib/grid
LEVEL_LIB = ../../lib/level
AMR_LIB = ../../lib/amr


.PHONY: clean

DIM = 2


output: AMR_advection.out
	./AMR_advection.out

AMR_advection.out: $(SOURCES)
	chpl $(SOURCES) -M $(GRID_LIB) -M $(LEVEL_LIB) -M $(AMR_LIB) --set dimension=$(DIM) -o AMR_advection.out

clean:
	rm -f _output/fort.* _output/*.png output_* *.out