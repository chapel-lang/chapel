
CHAPEL_ROOT = .
include $(CHAPEL_ROOT)/make/Makefile.base

all:
	@echo No default target defined
	@echo Try making 'compiler', 'runtime', 'test', or 'third-party'

compiler: FORCE
	cd compiler && $(MAKE)

runtime: FORCE
	cd runtime && $(MAKE)

test: FORCE
	cd test && start_test

third-party: FORCE
	cd third-party && $(MAKE)

FORCE:
