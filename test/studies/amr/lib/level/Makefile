MISC_LIB = ../misc
GRID_LIB = ../grid


.PHONY: clean


LevelArraySet_test: LevelArraySet_test.out
	./LevelArraySet_test.out

LevelArraySet_test.out: Level_def.chpl LevelArraySet_def.chpl
	chpl LevelArraySet_def.chpl Level_def.chpl --main-module=LevelArraySet_def -M $(GRID_LIB) --set dimension=2 -o LevelArraySet_test.out


Level_test: Level_test.out
	./Level_test.out

Level_test.out: Level_def.chpl
	chpl Level_def.chpl -M $(GRID_LIB) --set dimension=2 -o Level_test.out

clean:
	rm -f *.out