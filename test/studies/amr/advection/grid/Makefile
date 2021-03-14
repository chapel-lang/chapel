MISC_LIB = ../../lib/misc
GRID_LIB = ../../lib/grid

SOURCES = \
	GridVariable_AdvectionCTU.chpl \
	GridSolution_AdvectionCTU.chpl \
	GridBC_AdvectionCTU.chpl \
	Grid_AdvectionCTU_driver.chpl

DIM = 2


.PHONY: clean


output: _output Grid_AdvectionCTU.out
	rm -f _output/fort.*
	./Grid_AdvectionCTU.out
	@echo "Output is in directory _output."

Grid_AdvectionCTU.out: $(SOURCES)
	chpl $(SOURCES) -M $(MISC_LIB) -M $(GRID_LIB) --set dimension=$(DIM) -o Grid_AdvectionCTU.out

_output:
	mkdir _output
	cp ../../lib/plot.claw _output

clean:
	rm -rf _output/ *.out

