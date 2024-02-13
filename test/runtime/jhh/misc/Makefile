# This Makefile creates the test stub.

all: tester

ifeq ($(CHPL_COMPILER),)
  CHPL_COMPILER = chpl
endif

COMPILE = $(shell $(CHPL_HOME)/util/config/compileline --compile)
LIBS = $(shell $(CHPL_HOME)/util/config/compileline --libraries)

tester: lib/libtester.a tester.c
	$(COMPILE) -g tester.c -Llib -ltester $(LIBS) -o $@

lib/libtester.a: tester.chpl
	$(CHPL_COMPILER) tester.chpl --library

clean::
	rm -rf tester lib *.dSYM
