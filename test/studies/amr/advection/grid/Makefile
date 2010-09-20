CLAW_OUTDIR = _output               # Directory for output
CLAW_setplot_file = setplot1d.py      # File containing function to set plots
CLAW_PLOTDIR = _plots               # Directory for plots
PLOTCMD = python $(CLAW)/python/pyclaw/plotters/plotclaw.py

SOURCES = \
	GridArray_advection.chpl \
	GridSolution_advection.chpl \
	GridBC_advection.chpl \
	Grid_advection_driver.chpl

GRID_LIB = ../../lib/grid

DIM = 2


.PHONY: clean, emacs_clean


output: grid_advection.out
	rm -f _output/fort.*
	./grid_advection.out
	@echo "Output is in directory _output."
	@echo $(CLAW_OUTDIR) > output


grid_advection.out: $(SOURCES)
	chpl $(SOURCES) -M $(GRID_LIB) --set dimension=$(DIM) -o grid_advection.out


plots: _output/fort.t0000
	rm -f _plots/*
	$(PLOTCMD) $(CLAW_OUTDIR) $(CLAW_PLOTDIR) $(CLAW_setplot_file)
	@echo $(CLAW_PLOTDIR) > plots


clean:
	rm -f _output/fort.* _output/*.png *.out output plots _plots/* pyclaw.log *.pyc

emacs_clean:
	rm -f *~