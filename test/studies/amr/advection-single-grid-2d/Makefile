CLAW_OUTDIR = _output               # Directory for output
CLAW_setplot_file = setplot2d.py      # File containing function to set plots
CLAW_PLOTDIR = _plots               # Directory for plots
PLOTCMD = python $(CLAW)/python/pyclaw/plotters/plotclaw.py

SOURCES = \
	grid_data.chpl

.PHONY: clean

plots: output $(CLAW_setplot_file)
	rm -f _plots/*
	$(PLOTCMD) $(CLAW_OUTDIR) $(CLAW_PLOTDIR) $(CLAW_setplot_file)
	@echo $(CLAW_PLOTDIR) > plots

output: advection_test
	rm -f _output/fort.*
	./advection_test
	@echo "Output is in directory _output."
	@echo $(CLAW_OUTDIR) > output


advection_test: $(SOURCES) advection_test.chpl
	chpl $(SOURCES) advection_test.chpl -o advection_test

grid_test: $(SOURCES) grid_test.chpl
	chpl $(SOURCES) grid_test.chpl -o grid_test

clean:
	rm -f _output/fort.* _plots/* advect1d output plots pyclaw.log *.pyc *~