
CHAPEL_ROOT = .
include $(CHAPEL_ROOT)/make/Makefile.base

all:
	@echo No default rule defined

third-party: FORCE
	cd third-party && make

FORCE:
