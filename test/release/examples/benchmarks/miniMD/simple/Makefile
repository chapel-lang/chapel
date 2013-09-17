MAKEFLAGS = --no-print-directory

CHPL = chpl

CHPL_FLAGS = --fast -sprintOriginal=true -sprintPerf=false -sdebugBlockDistBulkTransfer -sdisableAliasedBulkTransfer=false

TARGETS = \
	  miniMD \

REALS = $(TARGETS:%=%_real)

#initMD.chpl forces.chpl neighbor.chpl thermo.chpl

default: all

all: $(TARGETS)

clean: FORCE
	rm -f $(TARGETS) $(REALS)

miniMD: miniMD.chpl helpers/*.chpl
	$(CHPL) -o $@ $(CHPL_FLAGS) $< helpers/*.chpl 

FORCE:
