
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
IPDEP = ip_color_v2.chpl

ex_datapar : bin/ex_datapar
bin/ex_datapar : ex_datapar.chpl
	chpl $(CHPLOPT) -o $@ $^

ex_quicksort : bin/ex_quicksort
bin/ex_quicksort : ex_quicksort.chpl
	chpl $(CHPLOPT) -o $@ $^

lab_limits_ser : bin/lab_limits_ser $(IPDEP)
bin/lab_limits_ser : lab_limits_ser.chpl
	chpl $(CHPLOPT) -o $@ $^

lab_limits_parv1 : bin/lab_limits_parv1 $(IPDEP)
bin/lab_limits_parv1 : lab_limits_parv1.chpl
	chpl $(CHPLOPT) -o $@ $^

lab_limits_parv2 : bin/lab_limits_parv2 $(IPDEP)
bin/lab_limits_parv2 : lab_limits_parv2.chpl
	chpl $(CHPLOPT) -o $@ $^

gaborbank : bin/gaborbank
bin/gaborbank : gaborbank.chpl $(IPDEP) $(CLRDEP)
	chpl $(CHPLOPT) -o $@ $^ -lpng

## general rules

CHPLALL = ex_datapar ex_quicksort
CHPLALL += lab_limits_ser lab_limits_parv1 lab_limits_parv2 gaborbank
CALL = img_png_v3

VPATH = build

all : $(CALL) $(CHPLALL)
# this removes the 'Nothing to be done' empty message when making
	@echo > /dev/null

clean :
	-rm -f build/*.o build/*.dep 
	-rm -f $(addprefix bin/,$(CALL)) $(addprefix bin/,$(CHPLALL))

tarball ../doc/data/chapel_by_ex_parallel.tar.gz :
	(cd .. && tar cf doc/data/chapel_by_ex_parallel.tar -T parallel/MANIFEST)
	(cd .. && zip doc/data/chapel_by_ex_parallel.zip -@ < parallel/MANIFEST)


## auto-create dependencies

build/%.dep : %.c
	@$(CC) -MM $(INCPATH) $< > $@.sed
	@sed 's,\($*\)\.o[ :]*,\1 $@ : ,g' < $@.sed > $@
	@rm -f $@.sed

DEP = $(addprefix build/,$(addsuffix .c,$(COBJ)))
-include $(DEP:.c=.dep)
