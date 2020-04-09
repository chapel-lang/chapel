MISC_LIB = ../misc
GRID_LIB = ../grid


.PHONY: clean


LevelVariableSet_test: LevelVariableSet_test.out
	./LevelVariableSet_test.out

LevelVariableSet_test.out: Level_def.chpl LevelVariableSet_def.chpl
	chpl LevelVariableSet_def.chpl Level_def.chpl --main-module=LevelVariableSet_def -M $(GRID_LIB) --set dimension=2 -o LevelVariableSet_test.out


Level_test: Level_test.out
	./Level_test.out

Level_test.out: Level_def.chpl
	chpl Level_def.chpl -M $(GRID_LIB) --set dimension=2 -o Level_test.out

clean:
	rm -f *.out