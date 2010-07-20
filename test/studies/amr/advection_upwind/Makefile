CLAW_OUTDIR = _output               # Directory for output
CLAW_setplot_file = setplot1d.py      # File containing function to set plots
CLAW_PLOTDIR = _plots               # Directory for plots
PLOTCMD = python $(CLAW)/python/pyclaw/plotters/plotclaw.py

UPWIND_ADVECTION_SOURCES = \
	grid_base_defs.chpl upwind_advection_defs.chpl upwind_advection_driver.chpl

CTU_ADVECTION_SOURCES = \
	grid_base_defs.chpl ctu_advection_defs.chpl ctu_advection_driver.chpl

FORWARD_EULER_DIFFUSION_SOURCES = \
	grid_base_defs.chpl \
	forward_euler_diffusion_defs.chpl \
	forward_euler_diffusion_driver.chpl

BACKWARD_EULER_DIFFUSION_SOURCES = \
	grid_base_defs.chpl \
	backward_euler_diffusion_defs.chpl \
	backward_euler_diffusion_driver.chpl


.PHONY: clean

plots: _output/fort.t0000
	rm -f _plots/*
	$(PLOTCMD) $(CLAW_OUTDIR) $(CLAW_PLOTDIR) $(CLAW_setplot_file)
	@echo $(CLAW_PLOTDIR) > plots



output_dbe: backward_euler_diffusion.out
	rm -f _output/fort.*
	./backward_euler_diffusion.out
	@echo "Output is in directory _output."
	@echo $(CLAW_OUTDIR) > output_dbe

output_dfe: forward_euler_diffusion.out
	rm -f _output/fort.*
	./forward_euler_diffusion.out
	@echo "Output is in directory _output."
	@echo $(CLAW_OUTDIR) > output_dfe

output_ca: ctu_advection.out
	rm -f _output/fort.*
	./ctu_advection.out
	@echo "Output is in directory _output."
	@echo $(CLAW_OUTDIR) > output_ca

output_ua: upwind_advection.out
	rm -f _output/fort.*
	./upwind_advection.out
	@echo "Output is in directory _output."
	@echo $(CLAW_OUTDIR) > output_ua



backward_euler_diffusion.out: $(BACKWARD_EULER_DIFFUSION_SOURCES)
	chpl $(BACKWARD_EULER_DIFFUSION_SOURCES) -o backward_euler_diffusion.out

forward_euler_diffusion.out: $(FORWARD_EULER_DIFFUSION_SOURCES)
	chpl $(FORWARD_EULER_DIFFUSION_SOURCES) -o forward_euler_diffusion.out

ctu_advection.out: $(CTU_ADVECTION_SOURCES)
	chpl $(CTU_ADVECTION_SOURCES) -o ctu_advection.out

upwind_advection.out: $(UPWIND_ADVECTION_SOURCES)
	chpl $(UPWIND_ADVECTION_SOURCES) -o upwind_advection.out


clean:
	rm -f _output/fort.* _output/*.png *.out output_* plots _plots/* pyclaw.log *.pyc