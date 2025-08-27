# This Makefile creates the colocales test stub.

all: colocales

ifeq ($(CHPL_COMPILER),)
  CHPL_COMPILER = chpl
endif

COMPILE = $(shell $(CHPL_HOME)/util/config/compileline --compile)
LIBS = $(shell $(CHPL_HOME)/util/config/compileline --libraries)

colocales: lib/libcolocales.a colocales.c
	$(COMPILE) -g colocales.c -Llib -l$@ $(LIBS) -o $@

lib/libcolocales.a: colocales.chpl
	$(CHPL_COMPILER) colocales.chpl -o colocales --library

clean::
	rm -rf colocales lib colocales.dSYM
