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
	cd modules && $(MAKE) clean
	cd runtime && $(MAKE) clean
	cd third-party && $(MAKE) clean

cleanall: FORCE
	cd compiler && $(MAKE) cleanall
	cd modules && $(MAKE) cleanall
	cd runtime && $(MAKE) cleanall
	cd third-party && $(MAKE) cleanall

cleandeps: FORCE
	cd compiler && $(MAKE) cleandeps
	cd runtime && $(MAKE) cleandeps

clobber: FORCE
	cd compiler && $(MAKE) clobber
	cd modules && $(MAKE) clobber
	cd runtime && $(MAKE) clobber
	cd third-party && $(MAKE) clobber
	rm -rf bin
	rm -rf lib

depend:
	@echo "make depend has been deprecated for the time being"

-include Makefile.devel

FORCE:
