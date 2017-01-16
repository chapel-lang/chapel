MAKEFLAGS = --no-print-directory

CHPL = chpl

CHPL_FLAGS = --fast -sprintOriginal=true -sprintPerf=false -sdisableAliasedBulkTransfer=false

TARGETS = \
	miniMD \

REALS = $(TARGETS:%=%_real)

default: all explicitMiniMD

all: $(TARGETS)

clean: FORCE
	rm -f $(TARGETS) $(REALS)
	cd explicit && $(MAKE) clean

stencil: FORCE
stencil: CHPL_FLAGS += -suseStencilDist=true
stencil: $(TARGETS)

miniMD: miniMD.chpl helpers/*.chpl
	+$(CHPL) -o $@ $(CHPL_FLAGS) -M helpers $<

explicitMiniMD: FORCE
	cd explicit && $(MAKE)

FORCE:
