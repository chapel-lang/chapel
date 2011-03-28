#
# top-level Chapel Makefile
#

CHAPEL_ROOT = .
include $(CHAPEL_ROOT)/make/Makefile.base

default: all

all: comprt
	@test -r Makefile.devel && $(MAKE) develall || echo ""

comprt: FORCE
	@$(MAKE) compiler
	@$(MAKE) runtime

compiler: FORCE
	cd compiler && $(MAKE)

runtime: FORCE
	cd runtime && $(MAKE)

third-party: FORCE
	cd third-party && $(MAKE)

clean: FORCE
	cd compiler && $(MAKE) clean
	cd runtime && $(MAKE) clean

clobber: FORCE
	cd compiler && $(MAKE) clobber
	cd runtime && $(MAKE) clobber
	rm -rf bin
	rm -rf lib

depend: FORCE
	cd compiler && $(MAKE) depend
	cd runtime && $(MAKE) depend

-include Makefile.devel

FORCE:
