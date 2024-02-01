
# Makefile for native and Chapel code.
# c 2015-2018 Primordial Machine Vision Systems, Inc.

## C compiler setup
export CC = gcc
export GCCFLG = -O2 -march=native
# Turn off clobbered.  Using the PNG library's setjmp/longjmp raises warnings.
export CCFLG = -Wall -Wextra -Wno-clobbered -fpic -pipe -g $(GCCFLG)

INCPATH = -I.
LDFLG = -lpng
COPT = $(CCFLG) $(INCPATH) $(LDFLG)

## Chapel compiler setup
export CHPLFLG =

# We'll list -lpng separately because we don't need it for some compiles.
CHPLOPT = $(CHPLFLG)


## C program rules

img_png_v3 build/img_png_v3.o : img_png_v3.c build/img_png_v3.dep
	$(CC) $(COPT) -c -o build/img_png_v3.o img_png_v3.c


## Chapel rules - code snippets

CLRDEP = img_png_v3.h build/img_png_v3.o
IPDEP = ip_color_v3.chpl

ex_iterator : bin/ex_iterator
bin/ex_iterator : ex_iterator.chpl
	chpl $(CHPLOPT) -o $@ $^

ex_class : bin/ex_class
bin/ex_class : ex_class.chpl
	chpl $(CHPLOPT) -o $@ $^

ex_genclass : bin/ex_genclass
bin/ex_genclass : ex_genclass.chpl
	chpl $(CHPLOPT) -o $@ $^

ex_genproc : bin/ex_genproc
bin/ex_genproc : ex_genproc.chpl
	chpl $(CHPLOPT) -o $@ $^

fast_v1 : bin/fast_v1
bin/fast_v1 : fast_v1.chpl $(IPDEP) $(CLRDEP)
	chpl $(CHPLOPT) -o $@ $^ -lpng

fast_v2 : bin/fast_v2
bin/fast_v2 : fast_v2.chpl $(IPDEP) $(CLRDEP)
	chpl $(CHPLOPT) -o $@ $^ -lpng --no-overload-sets-checks

test_fast_v1 : bin/test_fast_v1
bin/test_fast_v1 : test_fast_v1.chpl fast_v1.chpl
	chpl $(CHPLOPT) --main-module test_fast_v1 -o $@ $^

test_fast_v2 : bin/test_fast_v2
bin/test_fast_v2 : test_fast_v2.chpl fast_v2.chpl
	chpl $(CHPLOPT) --main-module test_fast_v2 -o $@ $^

## general rules

CHPLALL = ex_iterator ex_class ex_genclass ex_genproc
CHPLALL += fast_v1 fast_v2 test_fast_v1 test_fast_v2
CALL = img_png_v3

VPATH = build

all : $(CALL) $(CHPLALL)
# this removes the 'Nothing to be done' empty message when making
	@echo > /dev/null

clean :
	-rm -f build/*.o build/*.dep 
	-rm -f $(addprefix bin/,$(CALL)) $(addprefix bin/,$(CHPLALL))

tarball:
	(cd .. && tar cf doc/data/chapel_by_ex_fast.tar -T fast/MANIFEST)
	(cd .. && zip doc/data/chapel_by_ex_fast.zip -@ < fast/MANIFEST)


## auto-create dependencies

build/%.dep : %.c
	@$(CC) -MM $(INCPATH) $< > $@.sed
	@sed 's,\($*\)\.o[ :]*,\1 $@ : ,g' < $@.sed > $@
	@rm -f $@.sed

DEP = $(addprefix build/,$(addsuffix .c,$(COBJ)))
-include $(DEP:.c=.dep)
