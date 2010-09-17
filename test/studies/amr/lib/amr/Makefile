GRID_LIB = ../grid
LEVEL_LIB = ../level

HIERARCHY_SOURCES = \
	amr_hierarchy_defs.chpl \
	cf_level_interface_defs.chpl


ARRAY_MAIN = amr_array_defs.chpl
ARRAY_SOURCES = $(ARRAY_MAIN) $(HIERARCHY_SOURCES)

SOLUTION_MAIN = amr_solution_defs
SOLUTION_SOURCES = $(SOLUTION_MAIN).chpl \
	cf_solution_interface_defs.chpl \
	$(ARRAY_SOURCES)	


.PHONY: clean

DIM = 2



amr_solution_test: amr_solution_test.out
	./amr_solution_test.out

amr_solution_test.out: $(SOLUTION_SOURCES)
	chpl $(SOLUTION_SOURCES) --main-module $(SOLUTION_MAIN) -M $(GRID_LIB) -M $(LEVEL_LIB) --set dimension=$(DIM) -o amr_solution_test.out


amr_array_test: amr_array_test.out
	./amr_array_test.out

amr_array_test.out: $(ARRAY_SOURCES)
	chpl $(ARRAY_SOURCES) --main-module amr_array_defs -M $(GRID_LIB) -M $(LEVEL_LIB) --set dimension=$(DIM) -o amr_array_test.out


amr_hierarchy_test: amr_hierarchy_test.out
	./amr_hierarchy_test.out

amr_hierarchy_test.out: $(HIERARCHY_SOURCES)
	chpl $(HIERARCHY_SOURCES) -M $(GRID_LIB) -M $(LEVEL_LIB) --set dimension=$(DIM) -o amr_hierarchy_test.out

clean:
	rm -f *.out