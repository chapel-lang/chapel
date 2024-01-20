
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

ex_op : bin/ex_op
bin/ex_op : ex_op.chpl
	chpl $(CHPLOPT) -o $@ $^

ex_stmt : bin/ex_stmt
bin/ex_stmt : ex_stmt.chpl
	chpl $(CHPLOPT) -o $@ $^

ex_enum : bin/ex_enum
bin/ex_enum : ex_enum.chpl
	chpl $(CHPLOPT) -o $@ $^

ex_tuple : bin/ex_tuple
bin/ex_tuple : ex_tuple.chpl
	chpl $(CHPLOPT) -o $@ $^

ex_range : bin/ex_range
bin/ex_range : ex_range.chpl
	chpl $(CHPLOPT) -o $@ $^

IMGPNG_V3 = build/img_png_v3.o img_png_v3.h

lab_v1 : bin/lab_v1
bin/lab_v1 : lab_v1.chpl $(IMGPNG_V3)
	chpl $(CHPLOPT) -o $@ $^ -lpng

lab_v2 : bin/lab_v2
bin/lab_v2 : lab_v2.chpl $(IMGPNG_V3)
	chpl $(CHPLOPT) -o $@ $^ -lpng

color_convert : bin/color_convert
bin/color_convert : color_convert.chpl ip_color.chpl $(IMGPNG_V3)
	chpl $(CHPLOPT) -o $@ $^ -lpng

color_convert_v1b : bin/color_convert_v1b
bin/color_convert_v1b : color_convert_v1b.chpl ip_color_v1b.chpl $(IMGPNG_V3)
	chpl $(CHPLOPT) -o $@ $^ -lpng


## general rules

CHPLALL = ex_op ex_stmt ex_enum ex_tuple ex_range lab_v1 lab_v2
CHPLALL += color_convert color_convert_v1b
CALL = img_png_v3

VPATH = build

all : $(CALL) $(CHPLALL)
# this removes the 'Nothing to be done' empty message when making
	@echo > /dev/null

clean :
	-rm -f build/*.o build/*.dep 
	-rm -f $(addprefix bin/,$(CALL)) $(addprefix bin/,$(CHPLALL))

tarball :
	(cd .. && tar cf doc/data/chapel_by_ex_color.tar -T color/MANIFEST)
	(cd .. && zip doc/data/chapel_by_ex_color.zip -@ < color/MANIFEST)


## auto-create dependencies

build/%.dep : %.c
	@$(CC) -MM $(INCPATH) $< > $@.sed
	@sed 's,\($*\)\.o[ :]*,\1 $@ : ,g' < $@.sed > $@
	@rm -f $@.sed

DEP = $(addprefix build/,$(addsuffix .c,$(COBJ)))
-include $(DEP:.c=.dep)
