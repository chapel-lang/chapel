SOURCES = \
	grid_base_defs.chpl

.PHONY: clean


grid_test.out: $(SOURCES) grid_test.chpl
	chpl $(SOURCES) grid_test.chpl -o grid_test.out

clean:
	rm -f _output/fort.* _output/*.png _plots/* advect1d output plots pyclaw.log *.pyc