#
# top-level Chapel Makefile
#

CHAPEL_ROOT = .
include $(CHAPEL_ROOT)/make/Makefile.base

all:
	@echo No default target defined
	@echo "Try make-ing 'compiler', 'runtime', 'third-party', or 'everything' to build stuff"
	@echo "          ...'clean' or 'clobber' to clean up"
	@echo "          ...'test' to run regression tests"

everything: third-party compiler runtime

compiler: FORCE
	cd compiler && $(MAKE)

runtime: FORCE
	cd runtime && $(MAKE)

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

-include make/Makefile.release

FORCE:


# shannon was here
