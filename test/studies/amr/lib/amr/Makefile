GRID_LIB = ../grid
LEVEL_LIB = ../level

HIERARCHY_SOURCES = \
	AMRHierarchy_def.chpl \
	CFLevelInterface_def.chpl


ARRAY_MAIN = AMRArray_def
ARRAY_SOURCES = $(ARRAY_MAIN).chpl $(HIERARCHY_SOURCES)

SOLUTION_MAIN = AMRSolution_def
SOLUTION_SOURCES = $(SOLUTION_MAIN).chpl \
	CFSolutionInterface_def.chpl \
	$(ARRAY_SOURCES)	


.PHONY: clean

DIM = 2



AMRSolution_test: AMRSolution_test.out
	./AMRSolution_test.out

AMRSolution_test.out: $(SOLUTION_SOURCES)
	chpl $(SOLUTION_SOURCES) --main-module $(SOLUTION_MAIN) -M $(GRID_LIB) -M $(LEVEL_LIB) --set dimension=$(DIM) -o AMRSolution_test.out


AMRArray_test: amr_array_test.out
	./AMRArray_test.out

AMRArray_test.out: $(ARRAY_SOURCES)
	chpl $(ARRAY_SOURCES) --main-module $(ARRAY_MAIN) -M $(GRID_LIB) -M $(LEVEL_LIB) --set dimension=$(DIM) -o AMRArray_test.out


AMRHierarchy_test: AMRHierarchy_test.out
	./AMRHierarchy_test.out

AMRHierarchy_test.out: $(HIERARCHY_SOURCES)
	chpl $(HIERARCHY_SOURCES) -M $(GRID_LIB) -M $(LEVEL_LIB) --set dimension=$(DIM) -o AMRHierarchy_test.out

clean:
	rm -f *.out