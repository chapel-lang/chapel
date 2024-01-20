
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
export CHPLFLG = -g

# We'll list -lpng separately because we don't need it for some compiles.
CHPLOPT = $(CHPLFLG)


## C program rules

img_png_v1 build/img_png_v1.o : img_png_v1.c build/img_png_v1.dep
	$(CC) $(COPT) -c -o build/img_png_v1.o img_png_v1.c

img_png_v2 build/img_png_v2.o : img_png_v2.c build/img_png_v2.dep
	$(CC) $(COPT) -c -o build/img_png_v2.o img_png_v2.c

img_png_v3 build/img_png_v3.o : img_png_v3.c build/img_png_v3.dep
	$(CC) $(COPT) -c -o build/img_png_v3.o img_png_v3.c

test_png bin/test_png : test_png.c build/test_png.dep build/img_png_v1.o
	$(CC) $(COPT) -o bin/test_png test_png.c build/img_png_v1.o


## Chapel rules - code snippets

ex_config : bin/ex_config
bin/ex_config : ex_config.chpl
	chpl $(CHPLOPT) -o $@ $^

ex_init : bin/ex_init
bin/ex_init : ex_init.chpl
	chpl $(CHPLOPT) -o $@ $^

ex_fn : bin/ex_fn
bin/ex_fn : ex_fn.chpl
	chpl $(CHPLOPT) -o $@ $^

ex_struct : bin/ex_struct
bin/ex_struct : ex_struct.chpl
	chpl $(CHPLOPT) -o $@ $^

ex_method : bin/ex_method
bin/ex_method : ex_method.chpl
	chpl $(CHPLOPT) -o $@ $^

ex_if : bin/ex_if
bin/ex_if : ex_if.chpl
	chpl $(CHPLOPT) -o $@ $^

IMGPNG_V1 = build/img_png_v1.o img_png_v1.h
IMGPNG_V2 = build/img_png_v2.o img_png_v2.h
IMGPNG_V3 = build/img_png_v3.o img_png_v3.h

rw_png_v1 : bin/rw_png_v1
bin/rw_png_v1 : rw_png_v1.chpl $(IMGPNG_V1)
	chpl $(CHPLOPT) -o $@ $^ -lpng

rw_png_v1b : bin/rw_png_v1b
bin/rw_png_v1b : rw_png_v1.chpl $(IMGPNG_V1)
	chpl $(CHPLOPT) -o $@ rw_png_v1b.chpl

rw_png_v2 : bin/rw_png_v2
bin/rw_png_v2 : rw_png_v2.chpl $(IMGPNG_V2)
	chpl $(CHPLOPT) -o $@ $^ -lpng

rw_png_v3 : bin/rw_png_v3
bin/rw_png_v3 : rw_png_v3.chpl $(IMGPNG_V2)
	chpl $(CHPLOPT) -o $@ $^ -lpng

rw_png_v3b : bin/rw_png_v3b
bin/rw_png_v3b : rw_png_v3b.chpl $(IMGPNG_V2)
	chpl $(CHPLOPT) -o $@ $^ -lpng

rw_png_v4 : bin/rw_png_v4
bin/rw_png_v4 : rw_png_v4.chpl $(IMGPNG_V2)
	chpl $(CHPLOPT) -o $@ $^ -lpng

rw_png_v5 : bin/rw_png_v5
bin/rw_png_v5 : rw_png_v5.chpl $(IMGPNG_V3)
	chpl $(CHPLOPT) -o $@ $^ -lpng



## general rules

CHPLALL = ex_config ex_init ex_fn ex_struct ex_method ex_if
CHPLALL += rw_png_v1 rw_png_v1b rw_png_v2 rw_png_v3 rw_png_v3b 
CHPLALL += rw_png_v4 rw_png_v5
CALL = img_png_v1 img_png_v2 test_png

VPATH = build

all : $(CALL) $(CHPLALL)
# this removes the 'Nothing to be done' empty message when making
	@echo > /dev/null

clean :
	-rm -f build/*.o build/*.dep 
	-rm -f $(addprefix bin/,$(CALL)) $(addprefix bin/,$(CHPLALL))

tarball ../doc/data/chapel_by_ex_png.tar.gz :
	(cd .. && tar cf doc/data/chapel_by_ex_png.tar -T png/MANIFEST)
	(cd .. && zip doc/data/chapel_by_ex_png.zip -@ < png/MANIFEST)


## auto-create dependencies

build/%.dep : %.c
	@$(CC) -MM $(INCPATH) $< > $@.sed
	@sed 's,\($*\)\.o[ :]*,\1 $@ : ,g' < $@.sed > $@
	@rm -f $@.sed

DEP = $(addprefix build/,$(addsuffix .c,$(COBJ)))
-include $(DEP:.c=.dep)
