MAKEFLAGS = --no-print-directory

CHPL = chpl

FLAGS = --fast

TARGETS = \
	nbody_orig_1 \
	nbody_record_2 \
	nbody_record_domain_2a \
	nbody_forloop_3 \
	nbody_vector_4 \
	nbody_rangesub_5 \
	nbody_reductions_6 \
	nbody_iterator_7 \

default: all

all: $(TARGETS)

clean: FORCE
	rm -f $(TARGETS)

%: %.chpl
	$(CHPL) $(FLAGS) -o $@ $<

FORCE:
