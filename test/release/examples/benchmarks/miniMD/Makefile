MAKEFLAGS = --no-print-directory

CHPL = chpl

CHPL_FLAGS = --fast -sprintOriginal=true -sprintPerf=false -sdisableAliasedBulkTransfer=false

TARGETS = \
	miniMD \

REALS = $(TARGETS:%=%_real)

default: all

all: $(TARGETS)

clean: FORCE
	rm -f $(TARGETS) $(REALS)

stencil: FORCE
stencil: CHPL_FLAGS += -suseStencilDist=true
stencil: $(TARGETS)

miniMD: miniMD.chpl helpers/*.chpl
	+$(CHPL) -o $@ $(CHPL_FLAGS) -M helpers $<

FORCE:
