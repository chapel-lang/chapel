
CHPL = chpl

CHPL_FLAGS = --fast

TARGETS = \
	stream32-zipper \
	stream64-zipper \
	stream32-domain \
	stream64-domain 

REALS = $(TARGETS:%=%_real)

default: all

all: $(TARGETS)

clean: FORCE
	rm -f $(TARGETS) $(REALS)

stream32-zipper: probSize.chpl stream32-zipper.chpl
	$(CHPL) -o $@ $(CHPL_FLAGS) $^

stream64-zipper: probSize.chpl stream64-zipper.chpl
	$(CHPL) -o $@ $(CHPL_FLAGS) $^

stream32-domain: probSize.chpl stream32-domain.chpl
	$(CHPL) -o $@ $(CHPL_FLAGS) $^

stream64-domain: probSize.chpl stream64-domain.chpl
	$(CHPL) -o $@ $(CHPL_FLAGS) $^

FORCE:
