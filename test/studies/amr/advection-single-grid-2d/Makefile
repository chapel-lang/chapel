CHPLC = chpl

CLAW_OUTDIR = _output               # Directory for output
CLAW_setplot_file = setplot.py      # File containing function to set plots
CLAW_PLOTDIR = _plots               # Directory for plots
PLOTCMD = python $(CLAW)/python/pyclaw/plotters/plotclaw.py

SOURCES = \
	grid_data.chpl

NUM_CELLS = 50
NUM_OUTPUT = 10
VELOCITY = -2.0

.PHONY: clean

# plots: output $(CLAW_setplot_file)
# 	rm -f _plots/*
# 	$(PLOTCMD) $(CLAW_OUTDIR) $(CLAW_PLOTDIR) $(CLAW_setplot_file)
# 	@echo $(CLAW_PLOTDIR) > plots

# output: advect1d
# 	rm -f _output/fort.*
# 	./advect1d --num_cells=$(NUM_CELLS) --num_output_times=$(NUM_OUTPUT) --velocity=$(VELOCITY)
# 	@echo "Output generated"
# 	@echo $(CLAW_OUTDIR) > output

# advect1d: $(SOURCES)
# 	$(CHPLC) $(SOURCES) -o advect1d


advection_test: $(SOURCES) advection_test.chpl
	chpl $(SOURCES) advection_test.chpl -o advection_test

grid_test: $(SOURCES) grid_test.chpl
	chpl $(SOURCES) grid_test.chpl -o grid_test

clean:
	rm -f _output/fort.* _plots/* advect1d output plots pyclaw.log *.pyc *~