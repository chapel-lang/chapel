MISC_LIB = ../../lib/misc
GRID_LIB = ../../lib/grid
LEVEL_LIB = ../../lib/level
CF_LIB = ../../lib/cfboundary

.PHONY: clean

DIM = 2


AMRHierarchy_test: AMRHierarchy_test.out _output
	./AMRHierarchy_test.out

AMRHierarchy_test.out: AMRHierarchy_def.chpl Partitioning.chpl
	chpl AMRHierarchy_def.chpl Partitioning.chpl --main-module=AMRHierarchy_def -M $(MISC_LIB) -M $(GRID_LIB) -M $(LEVEL_LIB) -M $(CF_LIB) --set dimension=$(DIM) -o AMRHierarchy_test.out

_output:
	mkdir _output
	cp ../plot.claw _output

clean:
	rm -rf *.out _output
