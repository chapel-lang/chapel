MAKEFLAGS = --no-print-directory

CHPL = chpl

FLAGS = --fast

TARGETS = \
	PARACR-BC \
	PARACR-B \

default: PARACR-BC

all: $(TARGETS)

clean: FORCE
	rm -f $(TARGETS)

%: %.chpl
	$(CHPL) $(FLAGS) -o $@ $<

FORCE:
