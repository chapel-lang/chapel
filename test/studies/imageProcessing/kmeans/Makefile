
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

ex_file : bin/ex_file
bin/ex_file : ex_file.chpl
	chpl $(CHPLOPT) -o $@ $^

kmeans_v1 : bin/kmeans_v1
bin/kmeans_v1 : kmeans_v1.chpl $(IPDEP) $(CLRDEP)
	chpl $(CHPLOPT) -o $@ $^ -lpng

kmeans_v2 : bin/kmeans_v2
bin/kmeans_v2 : kmeans_v2.chpl $(IPDEP) $(CLRDEP)
	chpl $(CHPLOPT) -o $@ $^ -lpng

# This is an internal version, not released with the tutorial.
kmeans_int : bin/kmeans_int
bin/kmeans_int : kmeans_int.chpl $(IPDEP) $(CLRDEP)
	chpl $(CHPLOPT) -o $@ $^ -lpng

color_checker : bin/color_checker
bin/color_checker : color_checker.chpl $(CLRDEP)
	chpl $(CHPLOPT) -o $@ $^ -lpng


## general rules

CHPLALL = ex_file
CHPLALL += kmeans_v1 kmeans_v2 color_checker
CALL = img_png_v3

VPATH = build

all : $(CALL) $(CHPLALL)
# this removes the 'Nothing to be done' empty message when making
	@echo > /dev/null

clean :
	-rm -f build/*.o build/*.dep 
	-rm -f $(addprefix bin/,$(CALL)) $(addprefix bin/,$(CHPLALL))

tarball ../doc/data/chapel_by_ex_kmeans.tar.gz :
	(cd .. && tar cf doc/data/chapel_by_ex_kmeans.tar -T kmeans/MANIFEST)
	(cd .. && zip doc/data/chapel_by_ex_kmeans.zip -@ < kmeans/MANIFEST)


## auto-create dependencies

build/%.dep : %.c
	@$(CC) -MM $(INCPATH) $< > $@.sed
	@sed 's,\($*\)\.o[ :]*,\1 $@ : ,g' < $@.sed > $@
	@rm -f $@.sed

DEP = $(addprefix build/,$(addsuffix .c,$(COBJ)))
-include $(DEP:.c=.dep)
