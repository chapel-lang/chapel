MISC_LIB = ../../lib/misc
GRID_LIB = ../../lib/grid

SOURCES = \
	GridVariable_DiffusionBE.chpl \
	GridSolution_DiffusionBE.chpl \
	GridBC_DiffusionBE.chpl \
	Grid_DiffusionBE_driver.chpl

DIM = 2

.PHONY: clean, emacs_clean


output: _output Grid_DiffusionBE.out
	rm -f _output/fort.*
	./Grid_DiffusionBE.out
	@echo "Output is in directory _output."

Grid_DiffusionBE.out: $(SOURCES)
	chpl $(SOURCES) -M $(MISC_LIB) -M $(GRID_LIB) --set dimension=$(DIM) -o Grid_DiffusionBE.out

_output:
	mkdir _output
	cp ../../lib/plot.claw _output

clean:
	rm -rf _output/ *.out
