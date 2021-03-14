
CHPL = chpl

CHPL_FLAGS = --fast -suseSparseMaterials=false

TARGETS = \
	lulesh

REALS = $(TARGETS:%=%_real)

default: all

all: $(TARGETS)

clean: FORCE
	rm -f $(TARGETS) $(REALS)

lulesh: lulesh.chpl luleshInit.chpl
	+$(CHPL) -o $@ $(CHPL_FLAGS) $<

FORCE:
