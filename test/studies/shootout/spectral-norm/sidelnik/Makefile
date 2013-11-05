MAKEFLAGS = --no-print-directory

CHPL = chpl

FLAGS = --fast --local

TARGETS = \
	  spectralnorm \
	  spectralnorm_2 \

default: all

all: $(TARGETS)

clean: FORCE
	rm -f $(TARGETS)

%: %.chpl
	$(CHPL) $(FLAGS) -o $@ $<

FORCE:
