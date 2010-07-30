CLAW_OUTDIR = _output               # Directory for output
CLAW_setplot_file = setplot1d.py      # File containing function to set plots
CLAW_PLOTDIR = _plots               # Directory for plots
PLOTCMD = python $(CLAW)/python/pyclaw/plotters/plotclaw.py

SOURCES = \
	../lib/grid_base_defs.chpl \
	../lib/grid_ctu_defs.chpl \
	advection_ctu_driver.chpl


DIM = 2


.PHONY: clean, emacs_clean


output: advection_ctu.out
	rm -f _output/fort.*
	./advection_ctu.out
	@echo "Output is in directory _output."
	@echo $(CLAW_OUTDIR) > output


advection_ctu.out: $(SOURCES)
	chpl $(SOURCES) -s dimension=$(DIM) -o advection_ctu.out


plots: _output/fort.t0000
	rm -f _plots/*
	$(PLOTCMD) $(CLAW_OUTDIR) $(CLAW_PLOTDIR) $(CLAW_setplot_file)
	@echo $(CLAW_PLOTDIR) > plots


clean:
	rm -f _output/fort.* _output/*.png *.out output plots _plots/* pyclaw.log *.pyc

emacs_clean:
	rm -f *~