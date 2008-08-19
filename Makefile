#
# top-level Chapel Makefile
#

CHAPEL_ROOT = .
include $(CHAPEL_ROOT)/make/Makefile.base

default: all

all: comprt man

comprt: compiler runtime

compiler: FORCE
	cd compiler && $(MAKE)

man: FORCE
	@test -r man/Makefile && $(MAKE) manhelp || echo ""

manhelp: FORCE
	cd man && $(MAKE)

runtime: FORCE
	cd runtime && $(MAKE) all

third-party: FORCE
	cd third-party && $(MAKE)

test: FORCE
	cd test && start_test

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

FORCE:
