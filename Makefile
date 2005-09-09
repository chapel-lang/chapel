#
# top-level Chapel Makefile
#

CHAPEL_ROOT = .
include $(CHAPEL_ROOT)/make/Makefile.base

all:
	@echo No default target defined
	@echo "Try make-ing 'compiler', 'runtime', 'comprt' 'third-party', or 'everything'"
	@echo "              to build stuff"
	@echo "         ... 'depend' to build dependences for the compiler and runtime"
	@echo "         ... 'nogc' to build a version of the compiler without GC (suitable "
	@echo "              for valgrind)"
	@echo "         ... 'clean' or 'clobber' to clean up"
	@echo "         ... 'test' to run regression tests"

everything: third-party comprt

comprt: compiler runtime

compiler: FORCE
	cd compiler && $(MAKE)

runtime: FORCE
	cd runtime && $(MAKE)

nogc: FORCE
	cd compiler && $(MAKE) nogc

test: FORCE
	cd test && start_test

third-party: FORCE
	cd third-party && $(MAKE)

clean: FORCE
	cd compiler && $(MAKE) clean
	cd runtime && $(MAKE) clean
	cd third-party && $(MAKE) clean

clobber: FORCE
	cd compiler && $(MAKE) clobber
	cd runtime && $(MAKE) clobber
	cd third-party && $(MAKE) clobber

depend: FORCE
	cd compiler && $(MAKE) depend
	cd runtime && $(MAKE) depend

-include make/Makefile.release

FORCE:
