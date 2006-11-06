#
# top-level Chapel Makefile
#

CHAPEL_ROOT = .
include $(CHAPEL_ROOT)/make/Makefile.base

all:
	@echo No default target defined
	@echo "Try make-ing 'compiler', 'runtime', or 'comprt'"
	@echo "              to build stuff"
	@echo "         ... 'depend' to build dependences for the compiler and runtime"
	@echo "         ... 'clean' or 'clobber' to clean up"
	@echo "         ... 'test' to run regression tests"

comprt: compiler runtime

compiler: FORCE
	cd compiler && $(MAKE)

runtime: FORCE
	cd runtime && $(MAKE)

test: FORCE
	cd test && start_test

clean: FORCE
	cd compiler && $(MAKE) clean
	cd runtime && $(MAKE) clean

clobber: FORCE
	cd compiler && $(MAKE) clobber
	cd runtime && $(MAKE) clobber

depend: FORCE
	cd compiler && $(MAKE) depend
	cd runtime && $(MAKE) depend

FORCE:
