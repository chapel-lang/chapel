#
# top-level Chapel Makefile
#

CHAPEL_ROOT = .
include $(CHAPEL_ROOT)/make/Makefile.base

default: all

all: comprt man

comprt: FORCE
	@$(MAKE) compiler
	@$(MAKE) runtime

compiler: FORCE
	cd compiler && $(MAKE)

man: FORCE
	@test -r man/Makefile && $(MAKE) manhelp || echo ""

manhelp: FORCE
	cd man && $(MAKE)

runtime: FORCE
	cd runtime && $(MAKE)

third-party: FORCE
	cd third-party && $(MAKE)

test: FORCE
	cd test && start_test

SPECTEST_DIR = ./test/spec/autogen
spectests: FORCE
	rm -rf $(SPECTEST_DIR)
	util/test/extract_tests -o $(SPECTEST_DIR) spec/*.tex

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
