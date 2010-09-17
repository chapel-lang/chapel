SOURCES = \
	level_advection_defs.chpl \
	level_advection_driver.chpl

GRID_LIB = ../grid


.PHONY: clean


level_base_test: level_base_test.out
	./level_base_test.out

level_base_test.out: level_base_defs.chpl
	chpl level_base_defs.chpl -M $(GRID_LIB) --set dimension=2 -o level_base_test.out

clean:
	rm -f *.out