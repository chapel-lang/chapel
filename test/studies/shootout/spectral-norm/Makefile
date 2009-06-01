MAKEFLAGS = --no-print-directory

CHPL = chpl

FLAGS = --fast

TARGETS = \
	  spectralnorm \

default: all

all: $(TARGETS)

clean: FORCE
	rm -f $(TARGETS)

%: %.chpl
	$(CHPL) $(FLAGS) -o $@ $<

FORCE:
