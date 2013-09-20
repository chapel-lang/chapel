MAKEFLAGS = --no-print-directory

CHPL = chpl

CHPL_FLAGS = --fast --no-cc-warnings -sprintOriginal=true -sprintPerf=false -sdebugBlockDistBulkTransfer=false -sdisableAliasedBulkTransfer=false

TARGETS = \
	  miniMD \

REALS = $(TARGETS:%=%_real)

default: all

all: $(TARGETS)

clean: FORCE
	rm -f $(TARGETS) $(REALS)

stencil: FORCE
stencil: CHPL_FLAGS += -suseStencilDist=true -sdefaultDoRADOpt=false
stencil: $(TARGETS)

miniMD: miniMD.chpl helpers/*.chpl
	$(CHPL) -o $@ $(CHPL_FLAGS) $< helpers/*.chpl

FORCE:
