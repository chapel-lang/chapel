CHPL = chpl

CHPL_FLAGS = --fast

TARGETS = \
	nbody
  thread-ring

REALS = $(TARGETS:%=%_real)

default: all

all: $(TARGETS)

clean: FORCE
	rm -f $(TARGETS) $(REALS)

nbody: nbody.chpl
	$(CHPL) -o $@ $(CHPL_FLAGS) --no-warnings $<

thread-ring: thread-ring.chpl
  $(CHPL) -o $@ $(CHPL_FLAGS) --ccflags -march=native $<

FORCE:
