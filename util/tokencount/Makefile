all:
	bison -vdt tokencount.y
	flex tokencount.l
	gcc -o tokencount tokencount.tab.c lex.yy.c tokencount.c
	gcc -o tokencount.cron -DCRON tokencount.tab.c lex.yy.c tokencount.c
	gcc -o tokencount.dbg -DDEBUG tokencount.tab.c lex.yy.c tokencount.c

clean:
	rm -f *.tab.c *.tab.h lex.yy.c tokencount tokencount.cron tokencount.dbg tokencount.output

DIRS = \
	../../../compiler/*.h \
	../../../compiler/*.cpp \
	../../../compiler/AST/*.h \
	../../../compiler/AST/*.cpp \
	../../../runtime/include/*.h \
	../../../runtime/src/*.c

countem:
	-tokencount $(DIRS) > tokencount.out 2>&1
#	tokencount ../../../compiler/*.h > tokencount.out 2>&1


checkdiffs:
	find ../../../compiler/*.h -exec checkdiff {} \;
	find ../../../compiler/*.cpp -exec checkdiff {} \;
	find ../../../compiler/AST/*.h -exec checkdiff {} \;
	find ../../../compiler/AST/*.cpp -exec checkdiff {} \;
	find ../../../runtime/include/*.h -exec checkdiff {} \;
	find ../../../runtime/src/*.c -exec checkdiff {} \;
