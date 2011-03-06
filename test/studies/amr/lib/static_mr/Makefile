MISC_LIB = ../misc
GRID_LIB = ../grid
LEVEL_LIB = ../level
CF_LIB = ../cfboundary

.PHONY: clean

DIM = 2


StaticMRSolution_test: StaticMRSolution_test.out _output
	./StaticMRSolution_test.out

StaticMRSolution_test.out: StaticMRSolution_def.chpl StaticMRHierarchy_def.chpl
	chpl StaticMRSolution_def.chpl StaticMRHierarchy_def.chpl -M $(MISC_LIB) -M $(GRID_LIB) -M $(LEVEL_LIB) -M $(CF_LIB) --main-module=StaticMRSolution_def -o StaticMRSolution_test.out

_output:
	mkdir _output
	cp ../plot.claw _output

StaticMRHierarchy_test: StaticMRHierarchy_test.out
	./StaticMRHierarchy_test.out

StaticMRHierarchy_test.out: StaticMRHierarchy_def.chpl
	chpl StaticMRHierarchy_def.chpl -M $(MISC_LIB) -M $(GRID_LIB) -M $(LEVEL_LIB) -M $(CF_LIB) --set dimension=$(DIM) -o StaticMRHierarchy_test.out

clean:
	rm -rf *.out _output