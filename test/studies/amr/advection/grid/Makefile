MISC_LIB = ../../lib/misc
GRID_LIB = ../../lib/grid

SOURCES = \
	GridArray_AdvectionCTU.chpl \
	GridSolution_AdvectionCTU.chpl \
	GridBC_AdvectionCTU.chpl \
	Grid_AdvectionCTU_driver.chpl

DIM = 2


.PHONY: clean, emacs_clean


output: Grid_AdvectionCTU.out
	rm -f _output/fort.*
	./Grid_AdvectionCTU.out
	@echo "Output is in directory _output."
	@echo $(CLAW_OUTDIR) > output


Grid_AdvectionCTU.out: $(SOURCES)
	chpl $(SOURCES) -M $(MISC_LIB) -M $(GRID_LIB) --set dimension=$(DIM) -o Grid_AdvectionCTU.out


plots: _output/fort.t0000
	rm -f _plots/*
	$(PLOTCMD) $(CLAW_OUTDIR) $(CLAW_PLOTDIR) $(CLAW_setplot_file)
	@echo $(CLAW_PLOTDIR) > plots


clean:
	rm -f _output/fort.* _output/*.png *.out output plots _plots/* pyclaw.log *.pyc

emacs_clean:
	rm -f *~