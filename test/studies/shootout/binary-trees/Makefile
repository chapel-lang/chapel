MAKEFLAGS = --no-print-directory

CHPL = chpl

FLAGS = --fast

TARGETS = \
	binary-trees \
	binary-trees_iter \

default: all

all: $(TARGETS)

clean: FORCE
	rm -f $(TARGETS)

%: %.chpl
	$(CHPL) $(FLAGS) -o $@ $<

FORCE:
