
CHAPEL_ROOT = .
include $(CHAPEL_ROOT)/make/Makefile.base

all:
	@echo No default target defined
	@echo "Try make 'third-party' or 'compiler'"

compiler: FORCE
	cd compiler && $(MAKE)

third-party: FORCE
	cd third-party && $(MAKE)

FORCE:
