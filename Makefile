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

ifndef CHPL_DEVELOPER
man:

else
man: FORCE
	cd man && $(MAKE)

endif

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
	rm -rf bin/$(CHPL_MAKE_PLATFORM)
	rm -rf lib/$(CHPL_MAKE_PLATFORM)
	-rmdir bin > /dev/null 2>&1
	-rmdir lib > /dev/null 2>&1

depend: FORCE
	cd compiler && $(MAKE) depend
	cd runtime && $(MAKE) depend

FORCE:
