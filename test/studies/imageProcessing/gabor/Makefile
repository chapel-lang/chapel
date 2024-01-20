
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

ex_rangeop : bin/ex_rangeop
bin/ex_rangeop : ex_rangeop.chpl
	chpl $(CHPLOPT) -o $@ $^

ex_domainop : bin/ex_domainop
bin/ex_domainop : ex_domainop.chpl
	chpl $(CHPLOPT) -o $@ $^

ex_arrayop : bin/ex_arrayop
bin/ex_arrayop : ex_arrayop.chpl
	chpl $(CHPLOPT) -o $@ $^

plot_kernel : bin/plot_kernel
bin/plot_kernel : plot_kernel.chpl $(CLRDEP)
	chpl $(CHPLOPT) -o $@ $^ -lpng

gabor_v1 : bin/gabor_v1
bin/gabor_v1 : gabor_v1.chpl $(IPDEP) $(CLRDEP)
	chpl $(CHPLOPT) -o $@ $^ -lpng

gabor_v2 : bin/gabor_v2
bin/gabor_v2 : gabor_v2.chpl $(IPDEP) $(CLRDEP)
	chpl $(CHPLOPT) -o $@ $^ -lpng

gabor_v3 : bin/gabor_v3
bin/gabor_v3 : gabor_v3.chpl $(IPDEP) $(CLRDEP)
	chpl $(CHPLOPT) -o $@ $^ -lpng

gabor_v4 : bin/gabor_v4
bin/gabor_v4 : gabor_v4.chpl $(IPDEP) $(CLRDEP)
	chpl $(CHPLOPT) -o $@ $^ -lpng

gabor_v5 : bin/gabor_v5
bin/gabor_v5 : gabor_v5.chpl $(IPDEP) $(CLRDEP)
	chpl $(CHPLOPT) -o $@ $^ -lpng

gabor_v6 : bin/gabor_v6
bin/gabor_v6 : gabor_v6.chpl $(IPDEP) $(CLRDEP)
	chpl $(CHPLOPT) -o $@ $^ -lpng

diff_png : bin/diff_png
bin/diff_png : diff_png.chpl $(IPDEP) $(CLRDEP)
	chpl $(CHPLOPT) -o $@ $^ -lpng


## general rules

CHPLALL = ex_rangeop ex_domainop ex_arrayop plot_kernel diff_png
CHPLALL += gabor_v1 gabor_v2 gabor_v3 gabor_v4 gabor_v5 gabor_v6
CALL = img_png_v3

VPATH = build

all : $(CALL) $(CHPLALL)
# this removes the 'Nothing to be done' empty message when making
	@echo > /dev/null

clean :
	-rm -f build/*.o build/*.dep 
	-rm -f $(addprefix bin/,$(CALL)) $(addprefix bin/,$(CHPLALL))

tarball ../doc/data/chapel_by_ex_gabor.tar.gz :
	(cd .. && tar cf doc/data/chapel_by_ex_gabor.tar -T gabor/MANIFEST)
	(cd .. && zip doc/data/chapel_by_ex_gabor.zip -@ < gabor/MANIFEST)


## auto-create dependencies

build/%.dep : %.c
	@$(CC) -MM $(INCPATH) $< > $@.sed
	@sed 's,\($*\)\.o[ :]*,\1 $@ : ,g' < $@.sed > $@
	@rm -f $@.sed

DEP = $(addprefix build/,$(addsuffix .c,$(COBJ)))
-include $(DEP:.c=.dep)
