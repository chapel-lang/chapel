#
# top-level Chapel Makefile
#

export CHPL_MAKE_HOME=$(shell pwd)
include ./make/Makefile.base

default: all

all: comprt
	@test -r Makefile.devel && $(MAKE) develall || echo ""

comprt: FORCE
	@$(MAKE) compiler
	@$(MAKE) runtime
	@$(MAKE) modules

compiler: FORCE
	cd compiler && $(MAKE)

modules: FORCE
	cd modules && $(MAKE)

runtime: FORCE
	cd runtime && $(MAKE)

third-party: FORCE
	cd third-party && $(MAKE)

clean: FORCE
	cd compiler && $(MAKE) clean
	cd runtime && $(MAKE) clean
	cd modules && $(MAKE) clean

clobber: FORCE
	cd compiler && $(MAKE) clobber
	cd runtime && $(MAKE) clobber
	cd modules && $(MAKE) clobber
	rm -rf bin
	rm -rf lib

depend: FORCE
	cd compiler && $(MAKE) depend
	cd runtime && $(MAKE) depend

-include Makefile.devel

FORCE:
