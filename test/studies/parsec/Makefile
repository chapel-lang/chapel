MAKEFLAGS = --no-print-directory

CHPL = chpl

FLAGS = --fast --local

TARGETS = \
	  blackscholes \
	  blackscholes_promote \

default: all

all: $(TARGETS)

clean: FORCE
	rm -f $(TARGETS)

%: %.chpl
	$(CHPL) $(FLAGS) -o $@ $<

FORCE:
