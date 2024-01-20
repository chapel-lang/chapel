
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
IPDEP = ip_color_v3.chpl ip_corner.chpl kdtree.chpl

ransac_st : bin/ransac_st
bin/ransac_st : ransac_st.chpl $(IPDEP) $(CLRDEP)
	chpl $(CHPLOPT) -o $@ $^ -lpng

ransac_rst : bin/ransac_rst
bin/ransac_rst : ransac_rst.chpl $(IPDEP) $(CLRDEP)
	chpl $(CHPLOPT) -o $@ $^ -lpng

test_fit_st : bin/test_fit_st
bin/test_fit_st : test_fit_st.chpl ransac_st.chpl $(IPDEP) $(CLRDEP)
	chpl $(CHPLOPT) --main-module test_fit_st -o $@ $^ -lpng

test_fit_rst : bin/test_fit_rst
bin/test_fit_rst : test_fit_rst.chpl ransac_rst.chpl $(IPDEP) $(CLRDEP)
	chpl $(CHPLOPT) --main-module test_fit_rst -o $@ $^ -lpng

test_map_rst : bin/test_map_rst
bin/test_map_rst : test_map_rst.chpl ransac_rst.chpl $(IPDEP)
	chpl $(CHPLOPT) --main-module test_map_rst -o $@ $^

test_kdtree : bin/test_kdtree
bin/test_kdtree : test_kdtree.chpl
	chpl $(CHPLOPT) -o $@ $^


## general rules

CHPLALL = ransac_st ransac_rst 
CHPLALL += test_fit_st test_fit_rst test_kdtree test_map_rst
CALL = img_png_v3

VPATH = build

all : $(CALL) $(CHPLALL)
# this removes the 'Nothing to be done' empty message when making
	@echo > /dev/null

clean :
	-rm -f build/*.o build/*.dep 
	-rm -f $(addprefix bin/,$(CALL)) $(addprefix bin/,$(CHPLALL))

tarball ../doc/data/chapel_by_ex_ransac.tar.gz :
	(cd .. && tar cf doc/data/chapel_by_ex_ransac.tar -T ransac/MANIFEST)
	(cd .. && zip doc/data/chapel_by_ex_ransac.zip -@ < ransac/MANIFEST)


## auto-create dependencies

build/%.dep : %.c
	@$(CC) -MM $(INCPATH) $< > $@.sed
	@sed 's,\($*\)\.o[ :]*,\1 $@ : ,g' < $@.sed > $@
	@rm -f $@.sed

DEP = $(addprefix build/,$(addsuffix .c,$(COBJ)))
-include $(DEP:.c=.dep)
