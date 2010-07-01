CHPLC = $(CHPL_HOME)/bin/linux64/chpl

CLAW_OUTDIR = _output               # Directory for output
CLAW_setplot_file = setplot.py      # File containing function to set plots
CLAW_PLOTDIR = _plots               # Directory for plots
PLOTCMD = python $(CLAW)/python/pyclaw/plotters/plotclaw.py

SOURCES = \
	advection1d_driver.chpl \
	grid1d_definitions.chpl \
	clawpack_output_routines.chpl


NUM_CELLS = 50
NUM_OUTPUT = 10


.PHONY: clean

plots: output $(CLAW_setplot_file)
	rm _plots/*
	$(PLOTCMD) $(CLAW_OUTDIR) $(CLAW_PLOTDIR) $(CLAW_setplot_file)
	@echo $(CLAW_PLOTDIR) > plots

output: advect1d
	rm -f _output/fort.*
	./advect1d --num_cells=$(NUM_CELLS) --num_output_times=$(NUM_OUTPUT)
	@echo "Output generated"
	@echo $(CLAW_OUTDIR) > output

advect1d: $(SOURCES)
	$(CHPLC) $(SOURCES) -o advect1d

clean:
	rm -f _output/fort.* _plots/* advect1d output plots pyclaw.log *.pyc